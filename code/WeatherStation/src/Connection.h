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

class Network
{
    public:
        char* SSID;//moze string i pozniej konwersja za pomocą .c_str
        char* Password;//wolałbym przechowywać skrót :/
        Network(char* SSID, char* Password);
        Network(char* SSID);
        ~Network();
};


class NetworkElement
{
    public: 
        Network* data;
        NetworkElement* nextElement;
        NetworkElement(Network* data);
        NetworkElement(Network* data, NetworkElement* nextElement);
        ~NetworkElement();
};


class NetworkList
{
    private:
        NetworkElement* firstElement;
        unsigned int Length;
    public: 
        NetworkList();
        bool addNetwork(Network *network);//adding new network to list
        bool deleteNetwork(unsigned int ID); // deleting network from list
        unsigned int getLength();//getting number of Networks
        Network* getNetworkAt(unsigned int ID);//getting Network
        NetworkElement* getFirstElement();
        void store();//saving networks in EEPROM//private?
        void load();//loading networks from EEPROM//private?
        bool init();//loading networks from EEPROM//private?
        int find(char* SSID);
};