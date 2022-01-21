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

#include <WString.h>

class Environment
{
    public:
        unsigned long timeToConnect;
        unsigned long timeToSleep;
        unsigned long numberOfCycle;
        String NTPServerName;
        String MQTTBrokerName;
        String MQTTBrokerID;
        String MQTTBrokerLogin;
        String MQTTBrokerPasswd;
        void setEnvir(String name, String value);
        String getEnvir(String name);
        unsigned long getCycleAndIncrease();
        void resetCycles();
        void store();
        void load();
    private:
        void init();
};