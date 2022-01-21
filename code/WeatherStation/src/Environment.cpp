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

#include "Environment.h"

#include "FS.h"
#include "config.h"

void Environment::setEnvir(String name, String value)
{
         if (name == "1") NTPServerName = value;
    else if (name == "2") MQTTBrokerName = value;
    else if (name == "3") timeToConnect = (unsigned long)value.toInt();
    else if (name == "4") timeToSleep = (unsigned long)value.toInt();
    else if (name == "5") MQTTBrokerID = value;
    else if (name == "6") MQTTBrokerLogin = value;
    else if (name == "7") MQTTBrokerPasswd = value;
    store();
}

void Environment::init()
{
    if(!SPIFFS.exists("/env"))
    {
        File temp = SPIFFS.open("/env", "w");

        temp.println(NTPServer);
        temp.println(MQTTBroker);
        temp.println(String(TIME_TO_CONNECT));
        temp.println(String(MEASURE_FREQUENCY_MS));
        temp.println(String(MQTTBroker_ID));
        temp.println(String(MQTTBroker_Login));
        temp.println(String(MQTTBroker_Passwd));
        temp.println(String(MEASURE_FREQUENCY_MS/CycleLength + 1));

        temp.close();
    }
}

void Environment::store()
{
    SPIFFS.remove("/env");
    File temp = SPIFFS.open("/env", "w");

    temp.println(NTPServerName);
    temp.println(MQTTBrokerName);
    temp.println(String(timeToConnect));
    temp.println(String(timeToSleep));
    temp.println(MQTTBrokerID);
    temp.println(MQTTBrokerLogin);
    temp.println(MQTTBrokerPasswd);
    temp.println(String(numberOfCycle));

    temp.close();
}

void Environment::load()
{
    init();
    File temp = SPIFFS.open("/env", "r");

    NTPServerName = temp.readStringUntil('\n');
    NTPServerName.remove(NTPServerName.length() - 1);
    MQTTBrokerName = temp.readStringUntil('\n');
    MQTTBrokerName.remove(MQTTBrokerName.length() - 1);
    timeToConnect = (unsigned long)temp.readStringUntil('\n').toInt();
    timeToSleep = (unsigned long)temp.readStringUntil('\n').toInt();
    MQTTBrokerID = temp.readStringUntil('\n');
    MQTTBrokerID.remove(MQTTBrokerID.length() - 1);
    MQTTBrokerLogin = temp.readStringUntil('\n');
    MQTTBrokerLogin.remove(MQTTBrokerLogin.length() - 1);
    MQTTBrokerPasswd = temp.readStringUntil('\n');
    MQTTBrokerPasswd.remove(MQTTBrokerPasswd.length() - 1);
    numberOfCycle = (unsigned long)temp.readStringUntil('\n').toInt();

    temp.close();
}
String Environment::getEnvir(String value)
{
    
}

unsigned long Environment::getCycleAndIncrease()
{
    numberOfCycle++;
    store();
    return numberOfCycle;
}

void Environment::resetCycles()
{
    numberOfCycle = 0;
    store();
}