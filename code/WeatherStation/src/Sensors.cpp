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

#include "Sensors.h"


Sensors::Sensors()  :

dht(DHT_PIN, DHTTYPE),
//BH(0x23)
BH()
{
    dht.begin();
    Wire.begin(SDA, SCL);
    //LumOK = BH.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
    LumOK = BH.begin();
}

String Sensors::getTemperature()
{
    return String(Temperature = dht.readTemperature());
}

String Sensors::getHumidity()
{
    return String(Humidity = dht.readHumidity());
}

void Sensors::refreshLum()
{
    Wire.begin(SDA, SCL);
    LumOK = BH.begin();
}

String Sensors::getLum()
{
    if(LumOK)
        return String(Lum = BH.readLightLevel());
    return "NaN";
}

String Sensors::readTemperature()
{
    return String(Temperature);
}

String Sensors::readHumidity()
{
    return String(Humidity);
}

String Sensors::readLum()
{
    if(LumOK)
        return String(Lum);
    return "NaN";
}

float Sensors::readRawTemperature()
{
    return Temperature;
}

float Sensors::readRawHumidity()
{
    return Humidity;
}

float Sensors::readRawLum()
{
    return Lum;
}

void Sensors::setTime(long time)
{
    UnixTime = time;
    //LocalTime = millis()/1000;
    LocalTime = millis();
}

void Sensors::refreshTime()
{
    if(isTimeSet())
    {
        unsigned long temp = millis();
        UnixTime += (temp - LocalTime)/1000;
        LocalTime = temp;
    }
}

String Sensors::readTemperatureJSON()
{
    if (isTimeSet())
        return "{\"temperature\": " + String(Temperature) + ", \"time\": " + String(UnixTime) + "000}";//for my app convert to millis
    else
         return "{\"temperature\": " + String(Temperature) + "}";
}

String Sensors::readHumidityJSON()
{
    if (isTimeSet())
        return "{\"humidity\": " + String(Humidity) + ", \"time\": " + String(UnixTime) + "000}";
    else
        return "{\"humidity\": " + String(Humidity) + "}"; 
}

String Sensors::readLumJSON()
{
    if (isTimeSet())
        return "{\"lum\": " + String(Lum) + ", \"time\": " + String(UnixTime) + "000}";
    else
        return "{\"lum\": " + String(Lum) + "}"; 
}

String Sensors::readVoltageJSON()
{
    if (isTimeSet())
        return "{\"Vcc\": " + String(Voltage) + ", \"time\": " + String(UnixTime) + "000}";
    else
        return "{\"Vcc\": " + String(Voltage) + "}"; 
}

bool Sensors::isTimeSet()
{
    return UnixTime != 0;
}

bool Sensors::isLumOk()
{
    return LumOK;
}
void Sensors::setVoltage(float voltage)
{
    Voltage = voltage;
}