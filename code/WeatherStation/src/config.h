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

#pragma once

#define NTPServer "time.coi.pw.edu.pl"//"pool.ntp.org" - dziala//"time.coi.pw.edu.pl" - działa aż za dobrze (mój client pokazuje "za 0 sekund" w pierwszej chwili)//"time.elka.pw.edu.pl" - Nie dziala
#define MQTTBroker "192.168.0.2"//"localhost"//IPAddress(192, 168, 0, 104)//"test.mosquitto.org"
#define LOCALHOST IPAddress(192, 168, 0, 2)//IP lokalnego brokera
//#define LOCALHOST "192.168.0.2"//IP lokalnego brokera
#define TIME_TO_CONNECT 15000 //ms
#define MEASURE_FREQUENCY_MS 60000 // ms

#define MQTTBroker_ID "ESP8266Client"
#define MQTTBroker_Login "user"
#define MQTTBroker_Passwd "user2020"

#define HotSpotName "WS_PoJo"
#define HotSpotPassword "123456789"

#define CycleLength 10000
#define ResistanceParam 0.0158// - na PCB// 0.0161// - na płytce stykowej //k
/*
* Opory w omach
* Wzór na to -> k = (R1/322000 + R1/R2 + 1) / 329034
*/


#define DHTTYPE DHT22
#define DHT_PIN D2

#define SDA D5
#define SCL D6

#define BUTTON_SIMULATION_PIN D1

//const char* mqtt_server="5.196.95.208";
//const char* mqtt_server="test.mosquitto.org";

#define SCRIPT "<script>function myFunction(x) {var xmlhttp = new XMLHttpRequest();xmlhttp.open(\"GET\", \"/delete?val=\" + x, true);xmlhttp.send();location.reload();}</script>"