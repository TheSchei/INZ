/***************************************************************************
 *   Copyright (C) 2021 by Paweł Jasiński                                  *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <user_interface.h> <-- Miala zawierac metode pozwalajaca zdetekowac wpisanie zlego hasle, nie dziala poprawnie

#include <NTPClient.h>
#include <WiFiUdp.h>

#include <WiFiClient.h>
//#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#include "Connection.h"
#include "Sensors.h"
 
#include "config.h"
#include "Environment.h"

Environment environment;
Sensors sensors;
NetworkList networks;
unsigned long workTime;

IPAddress mosquito(5, 196, 95, 208);
WiFiClient espClient;
PubSubClient client(espClient);
ESP8266WebServer* server;

boolean serviceMode = false;
boolean lowPower = false;
float Voltage = 0;

void handleSleepStatus();
unsigned long calculateTimeToSleep();
void handlePower();

void normalSetup();
void settingSetup();
void normalWork();
void settingWork();

bool handleFileRead(String path, HTTPMethod method);
String getContentType(String filename);
String CreateNetworkListHTML();
String CreateScannedNetworkListJSON();
void handleAddAP();
void handleChangeEnvir();
boolean sendEnvir(String path);

bool reconnect();
void ledON();
void ledOFF();

void refreshSensors();
void publishToBroker();
void handleSleep();
void tryPublishTemp();
void tryPublishHumi();
void tryPublishLum();
void tryPublishVcc();


void setup()
{
  workTime = millis();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_SIMULATION_PIN, INPUT);
  ledOFF();
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("Stacja pogodowa - praca inzynierska."));
  Serial.print(__FILE__); Serial.println(F("."));
  Serial.print(__DATE__); Serial.println(F("."));

  handlePower();

  SPIFFS.begin();
  environment.load();
  if (digitalRead(BUTTON_SIMULATION_PIN) == LOW)
    handleSleepStatus();
  else
    serviceMode = true;

  networks.load();
  Serial.println("Number of loaded networks: " + String(networks.getLength()));
  if ((networks.getLength() == 0) || serviceMode)
  {
    Serial.println(F("Entering service mode."));
    settingSetup();
    serviceMode = true;
  }
  else
  {
    Serial.println(F("Entering normal mode."));
    normalSetup();
  }
}

void loop() {
  if(serviceMode) settingWork();
  else normalWork();
}

void handleSleepStatus()
{
  if (environment.getCycleAndIncrease() * CycleLength < calculateTimeToSleep())
  {
    Serial.println(F("Keep sleeping"));
    ESP.deepSleep(((uint64_t)CycleLength)<<10);
  }
  environment.resetCycles();
}
unsigned long calculateTimeToSleep()
{
  if (lowPower) return environment.timeToSleep << 2;
  return environment.timeToSleep;
}

void handlePower()
{
  //Voltage = (float)analogRead(A0) * ResistanceParam;
  Voltage = (float)analogRead(A0);
  Serial.print(F("Raw voltage: ")); Serial.println(Voltage);
  Voltage *= ResistanceParam;
  Serial.print(F("Voltage: ")); Serial.println(Voltage);
  if (Voltage < 2) Serial.println(F("Failed to read voltage."));//zakladam, ze to nie mozliwe, zeby przy takim napieciu uklad dzialal, wiec oznacza to blad pomiaru lub zasilanie z USB
  else if (Voltage < 3.3) ESP.deepSleep((uint64_t)CycleLength << 10);
  else if (Voltage < 3.6) lowPower = true;
}

void normalSetup()
{
  WiFi.mode(WIFI_AP);
  //networks.addNetwork(new Network("HUAWEI P20 lite", "12345678"));
  //networks.addNetwork(new Network(mySSID, myPassword));
  sensors.refreshLum();
  if (reconnect())
  {
    Serial.print(F("Time to connect to Wi-Fi = ")); Serial.print(millis() - workTime); Serial.println(F("ms."));
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, environment.NTPServerName.c_str());
    timeClient.begin();
    if(timeClient.update())
    {
      Serial.print(F("Unixtime = "));
      Serial.println(timeClient.getEpochTime());
      sensors.setTime(timeClient.getEpochTime());
    }
    else
    {
      Serial.println(F("Failed to get time"));
    }
  }
  Serial.print(F("Setup Time = ")); Serial.print(millis() - workTime); Serial.println(F("ms."));
  Serial.println(F("Let's start"));
  /*if (strcmp(environment.MQTTBrokerName.c_str(),"localhost"))//dziala z podanym IP w formie stringu, ale tylko trochę
  {
    Serial.println("Setting local Broker");
    client.setServer(LOCALHOST, 1883);
  }
  else client.setServer(environment.MQTTBrokerName.c_str(), 1883);*/
  /*IPAddress BrokerIP;
  if(BrokerIP.fromString(environment.MQTTBrokerName.c_str()))
  {
    Serial.println(F("Setting Broker address as IP"));
    client.setServer(BrokerIP, 1883);
  }
  else
  {*/
    client.setServer(environment.MQTTBrokerName.c_str(), 1883);
  //}
}
void settingSetup()
{
  ledON();
  pinMode(BUTTON_SIMULATION_PIN, OUTPUT);
  digitalWrite(BUTTON_SIMULATION_PIN, LOW);//TEST
  WiFi.mode(WIFI_STA);//check
  WiFi.softAP(HotSpotName, HotSpotPassword);
  server = new ESP8266WebServer(80);
  /*if (MDNS.begin("esp8266")) {              // Wydaje się niepotrzebne
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }*/
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  server->onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(server->uri(), server->method()))// send it if it exists
      server->send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });
  server->begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

bool handleFileRead(String path, HTTPMethod method) { // send the right file to the client (if it exists)
  Serial.println("Received request: " + path);
  if(method == HTTP_POST)
  {
    Serial.print("POST args: ");Serial.println(server->args());
    if (path == "/add.html")
    {
      if (server->args() < 2) return false;
      handleAddAP();
      File file = SPIFFS.open(path, "r");                 // Open it
      server->streamFile(file, getContentType(path));     // And send it to the client
      file.close();
      return true;
    }
    if (path == "/environment.html")
    {
      if (server->args() < 2) return false;
      handleChangeEnvir();
      File file = SPIFFS.open(path, "r");                 // Open it
      server->streamFile(file, getContentType(path));     // And send it to the client
      file.close();
      return true;
    }
  }
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    server->streamFile(file, contentType);              // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  else if (path == "/object.html")
  {
    String page = CreateNetworkListHTML();
    server->send(200, "text/html", page);
    return true;
  }
  else if (path == "/delete")
  {
    Serial.println("Deleting network at: " + String((unsigned int)server->arg(0).toInt()));
    networks.deleteNetwork((unsigned int)server->arg(0).toInt());
    networks.store();
    File file = SPIFFS.open("/list.html", "r");           
    server->streamFile(file, getContentType(".html"));         
    file.close();
    return true;
  }
  else if (path == "/reset")
  {
    //bye page
    server->send(200, "text/html", "ok");//temp :p
    ESP.reset();
    return true;
  }
  else if (path == "/APlist")
  {
    server->send(200, "application/json", CreateScannedNetworkListJSON());
    return true;
  }
  else if (sendEnvir(path))
    return true;
  Serial.println("File Not Found");
  return false;// If the file doesn't exist, return false
}

String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  return "text/plain";
}

String CreateNetworkListHTML()
{
  String page = "";
  NetworkElement* temp = networks.getFirstElement();
    for(unsigned int i = 0; i < networks.getLength(); i++)
    {
      Serial.println("Network: " + String(temp->data->SSID));
      page += "<a>" + String(temp->data->SSID) + "</a> <button type=\"button\" onclick=\"myFunction(" + String(i) + ")\">Delete</button></br>";
      temp = temp->nextElement;
    }
  page += SCRIPT;
  return page;
}

String CreateScannedNetworkListJSON()
{
  String output = "[";
  int x = WiFi.scanNetworks();
  Serial.print("Scannednetworks: ");Serial.println(x);
  if (x > 0)
  {
    output += "{";
    output += "\"n\":\"" + WiFi.SSID(0) + "\",";
    output += "\"v\":" + String(WiFi.RSSI(0));
    output += "}";
  }
  for (int i=1; i < x; i++)
  {
    output += ",{";
    output += "\"n\":\"" + WiFi.SSID(i) + "\",";
    output += "\"v\":" + String(WiFi.RSSI(i));
    output += "}";
  }
  output += ']';
  WiFi.scanDelete();
  Serial.println(output);
  return output;
}

void handleAddAP()
{
  Serial.println("Trying to add AP: " + server->arg(0) + "::" + server->arg(1).c_str() + ".");
  if (server->arg(1) == "")
  {
    char ssid[server->arg(0).length()];
    strcpy(ssid, server->arg(0).c_str());
    if(!networks.addNetwork(new Network(ssid)))
    {
      Serial.println("Failed to add AP");
      return;
    }
  }else
   {
    char ssid[server->arg(0).length()];
    strcpy(ssid, server->arg(0).c_str());
    char passwd[server->arg(1).length()];
    strcpy(passwd, server->arg(1).c_str());
    if(!networks.addNetwork(new Network(ssid, passwd)))
    {
      Serial.println("Failed to add AP");
      return;
    }
  }
  Serial.println("AP added successfully");
  networks.store();
}

void handleChangeEnvir()
{
  Serial.print("args = ");Serial.println(server->args());
  Serial.print("arg0 = ");Serial.println(server->arg(0));
  Serial.print("arg1 = ");Serial.println(server->arg(1));

  environment.setEnvir(server->arg(1), server->arg(0));

  Serial.print("NTP = ");Serial.println(environment.NTPServerName);
  Serial.print("MQTT = ");Serial.println(environment.MQTTBrokerName);
  Serial.print("TTC = ");Serial.println(String(environment.timeToConnect));
  Serial.print("TTS = ");Serial.println(String(environment.timeToSleep));
}

boolean sendEnvir(String path)
{
  Serial.print("Envir path = ");Serial.println(path);
  if(path == "/envir1") 
  {
    server->send(200, "text/plain", environment.NTPServerName);
    return true;
  }
  if(path == "/envir2") 
  {
    server->send(200, "text/plain", environment.MQTTBrokerName);
    return true;
  }
  if(path == "/envir3") 
  {
    server->send(200, "text/plain", String(environment.timeToConnect));
    return true;
  }
  if(path == "/envir4") 
  {
    server->send(200, "text/plain", String(environment.timeToSleep));
    return true;
  }
  if(path == "/envir5") 
  {
    server->send(200, "text/plain", environment.MQTTBrokerID);
    return true;
  }
  if(path == "/envir6") 
  {
    server->send(200, "text/plain", environment.MQTTBrokerLogin);
    return true;
  }
  if(path == "/envir7") 
  {
    server->send(200, "text/plain", environment.MQTTBrokerPasswd);
    return true;
  }
  return false;
}

void normalWork()
{
  refreshSensors();
  switch (WiFi.status())
  {
    case WL_CONNECTED:
        publishToBroker();
      break;
    default:
      Serial.println(F("Not connected"));
      break;
  }
  handleSleep();
}

bool reconnect() // true = connected, false = not found // sprobowac przeniesc do Connection.h // priorytet - NAJSILNIEJSZA PIERWSZA (druga opcja, PIERWSZA Z LISTY pierwsza)
{
  unsigned long temp;
  int numberOfNetworks = WiFi.scanNetworks();
  int foundNetwork = -1;
  char temps[WL_SSID_MAX_LENGTH];//brzydkie
  for(int i = 0; i < numberOfNetworks; i++)
  {
    WiFi.SSID(i).toCharArray(temps, sizeof(temps));
    Serial.print(F("SSID: "));
    Serial.println(temps);
    if ((foundNetwork = networks.find(temps)) != -1)
    {
      Serial.println(F("connecting..."));
      char* password = networks.getNetworkAt((unsigned int)foundNetwork)->Password;
      Serial.println(F("Trying to connect to:"));
      Serial.print(temps); Serial.print(F("::"));Serial.println(password);
      WiFi.begin(temps,password);

      temp = millis();
      //while (millis() - temp < TIME_TO_CONNECT)  // 15 seconds to TIMEOUT
      while (millis() - temp < environment.timeToConnect)
      {
        if (WiFi.status() == WL_CONNECTED)//it's a Match
        {
          Serial.println();
          Serial.println(F("WiFi connected..!"));
          Serial.print(F("Got IP: "));  Serial.println(WiFi.localIP());
          return true;
        }
        else if (WiFi.status() == WL_CONNECT_FAILED)//keep looking
        {
          Serial.println();
          Serial.println(F("WiFi failed to connected..!"));
          break;
        }
        else//keep waiting
        {
          delay(500); //<------ lepiej sprawdzac caly czas, czy czekac
                      // <---- mozna by tu upchac robienie pomiarow :O
          Serial.print(F("."));
        }
      }
      WiFi.disconnect();
    }
  }
  return false;
}
void settingWork()
{
  server->handleClient();
}

void refreshSensors()
{
  Serial.print(F("Temperature = ")); Serial.print(sensors.getTemperature()); Serial.println(F(" C"));
  Serial.print(F("Humidity = ")); Serial.print(sensors.getHumidity()); Serial.println(F(" %"));
  Serial.print(F("Lum = ")); Serial.print(sensors.getLum()); Serial.println(F(" Lux"));//ZAKLADAM CZUJNIK GY-30 - BH1750
  sensors.setVoltage(Voltage);
  sensors.refreshTime();
  Serial.println();
}

void publishToBroker()
{
  Serial.println(F("Connecting with Mosquito"));
  //if (client.connect("ESP8266Client"))
  //if (client.connect(MQTTBroker_ID, MQTTBroker_Login, MQTTBroker_Passwd))
  Serial.print("NoC: " + (String)environment.numberOfCycle);Serial.println(".");
  Serial.print("ID: " + environment.MQTTBrokerID);Serial.println(".");
  Serial.print("Addr: " + environment.MQTTBrokerName);Serial.println(".");
  Serial.print("Log: " + environment.MQTTBrokerLogin);Serial.println(".");
  Serial.print("Pass: " + environment.MQTTBrokerPasswd);Serial.println(".");
  if (client.connect(environment.MQTTBrokerID.c_str(), environment.MQTTBrokerLogin.c_str(), environment.MQTTBrokerPasswd.c_str()))
  {
    tryPublishTemp();
    tryPublishHumi();
    tryPublishLum();
    tryPublishVcc();//testVoltage
    client.disconnect();
  }
  else Serial.println(F("Failed to connect with broker"));
}

void tryPublishTemp()
{
  if (!isnan(sensors.readRawTemperature()))
  {
      if(client.publish("PoJo2020/BoS/Temp", sensors.readTemperatureJSON().c_str(), true))
        Serial.println(F("Published Temp"));
      else
        Serial.println(F("Failed to publish Temp"));
  }
  else
  {
    Serial.println(F("Failed to read Temp"));
  }
}

void tryPublishHumi()
{
  if (!isnan(sensors.readRawHumidity()))
  {
      if(client.publish("PoJo2020/BoS/Humi", sensors.readHumidityJSON().c_str(), true))
        Serial.println(F("Published Humi"));
      else
        Serial.println(F("Failed to publish Humi"));
  }
  else
  {
    Serial.println(F("Failed to read Humi"));
  }
}

void tryPublishLum()
{
  if (sensors.isLumOk())
  {
      if(client.publish("PoJo2020/BoS/Lum", sensors.readLumJSON().c_str(), true))
        Serial.println(F("Published Lum"));
      else
        Serial.println(F("Failed to publish Lum"));
  }
  else
  {
    Serial.println(F("Failed to read Lum"));
  }
}
void tryPublishVcc()//test napięcia
{
  //if(client.publish("PoJo2020/BoS/Vcc", String(Voltage).c_str(), true))
  if(client.publish("PoJo2020/BoS/Vcc", sensors.readVoltageJSON().c_str(), true))
        Serial.println(F("Published Vcc"));
      else
        Serial.println(F("Failed to publish Vcc"));
}

void handleSleep()
{
  Serial.println(F("Task finished"));
  workTime = millis() - workTime;//czas pracy
  Serial.print(F("Worktime = ")); Serial.print(workTime); Serial.println(F("ms."));
  Serial.print(F("Going to sleep."));
  //ESP.deepSleep(((uint64_t)MEASURE_FREQUENCY_MS)<<10);//czas w ms*1024, czyli ~us
  //ESP.deepSleep(((uint64_t)environment.timeToSleep)<<10);//czas w ms*1024, czyli ~us
  ESP.deepSleep(((uint64_t)CycleLength)<<10);
  /*if(workTime > MEASURE_FREQUENCY_MS)
  {
    Serial.println(F("Too long, reseting"));
    ESP.reset();
  }
  workTime = MEASURE_FREQUENCY_MS - workTime;
  Serial.print(F("Going to sleep for ")); Serial.print(workTime); Serial.print(F("ms."));
  ESP.deepSleep(((uint64_t)workTime)<<10);//pozostaly czas w ms*1024, czyli ~us*/
}

void ledON()
{
  digitalWrite(LED_BUILTIN, LOW);
}

void ledOFF()
{
  digitalWrite(LED_BUILTIN, HIGH);
}