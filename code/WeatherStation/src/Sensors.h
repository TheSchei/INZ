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

#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>
#include <ArduinoJson.h>
#include "config.h"

class Sensors
{
    private:
        DHT dht;
        BH1750 BH;//ZAKLADAM CZUJNIK GY-30 - BH1750
        float Temperature = 0;
        float Humidity = 0;
        float Lum = 0;
        float Voltage = 0;
        long UnixTime = 0;

        bool LumOK = false;

        unsigned long LocalTime;
        bool isTimeSet();
    public:
        Sensors();
        String getTemperature();
        String getHumidity();
        String getLum();
        String readTemperature();
        String readHumidity();
        String readLum();
        float readRawTemperature();
        float readRawHumidity();
        float readRawLum();
        String readTemperatureJSON();
        String readHumidityJSON();
        String readLumJSON();
        String readVoltageJSON();
        void setVoltage(float voltage);
        void setTime(long time);
        void refreshTime();
        void refreshLum();

        bool isLumOk();
};