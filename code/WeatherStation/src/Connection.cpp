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

#include "Connection.h"
//#include "EEPROM.h"
#include "FS.h"
#include <Arduino.h>

#define MAX_ELEMENTS 5

Network::Network(char* SSID, char* Password)
{
    //SSID = SSID;//sprawdzić czy to nie za płytka kopia
    size_t len = strlen(SSID) + 1;
    this->SSID = new char[len];
    strcpy(this->SSID, SSID);
    len = strlen(Password) + 1;
    this->Password = new char[len];
    strcpy(this->Password, Password);
    //Password = Password;
}

Network::Network(char* SSID)
{
    size_t len = strlen(SSID) + 1;
    this->SSID = new char[len];
    strcpy(this->SSID, SSID);
    //this->Password = nullptr;
    this->Password = (char*) __null;
}

Network::~Network()
{
    delete SSID;
    //if (Password != nullptr) delete Password;
    delete Password;
}

NetworkElement::~NetworkElement()
{
    delete data;
}


NetworkElement::NetworkElement(Network* data)
{
    this->data = data;
    this->nextElement = nullptr;
}

NetworkList::NetworkList()
{
    Length = 0;
    firstElement = nullptr;
    init();
    load();
}
bool NetworkList::addNetwork(Network* network)
{
    if (Length == MAX_ELEMENTS) return false;
    if (Length == 0)
        firstElement = new NetworkElement(network);
    else
    {
        NetworkElement* temp = firstElement;
        while (temp->nextElement != nullptr) temp = temp->nextElement;
        temp->nextElement = new NetworkElement(network);
    }
    Length++;
    return true;
}
bool NetworkList::deleteNetwork(unsigned int ID)
{
    if (ID >= Length) return false;
    if (Length == 1) 
    {
        delete firstElement;
        firstElement = nullptr;
    }
    else if (ID == 0)
    {
        NetworkElement* temp = firstElement->nextElement;
        delete firstElement;
        firstElement = temp;
    }
    else
    {
        NetworkElement* temp = firstElement;
        for (unsigned int i = 1; i < ID; i++)
            temp = temp->nextElement;
        if (temp->nextElement->nextElement == nullptr)
        {
            delete temp->nextElement;
            temp->nextElement = nullptr;
        }
        else
        {
            NetworkElement* temp2 = temp->nextElement->nextElement;
            delete temp->nextElement;
            temp->nextElement = temp2;
        }
    }  
    Length--;
    return true;
}

Network* NetworkList::getNetworkAt(unsigned int ID)
{
    if (ID >= Length) return nullptr;//sprawdzic/zastapic wyjatkiem
    else
    {
        NetworkElement* temp = firstElement;
        for (unsigned int i = 0; i < ID; i++)
            temp = temp->nextElement;
        return temp->data;
    }
}

int NetworkList::find(char* SSID)
{
    NetworkElement* temp = firstElement;
    for (unsigned int i = 0; i < Length; i++)
    {
        if(!strcmp(temp->data->SSID, SSID)) return (int)i;
        temp = temp->nextElement;
    }
    return -1;
}

/*void NetworkList::store()
{
    //zapisywanie w EEPROM
    //
    //ewentualny clear();
    //
    NetworkElement* temp = firstElement;
    int j = 1;
    int len = 0;
    EEPROM.write(0, (unsigned char)Length);
    for(unsigned int i = 0; i < Length; i++)
    {
        len = strlen(temp->data->SSID);
        EEPROM.write(j++, len);
        for(int k = 0; k < len; k++)
            EEPROM.write(j++, temp->data->SSID[k]);
        if (temp->data->Password == nullptr)
        {
            EEPROM.write(j++, 0);
        }
        else
        {
            len = strlen(temp->data->Password);
            EEPROM.write(j++, len);
            for(int k = 0; k < len; k++)
                EEPROM.write(j++, temp->data->Password[k]);
        }
        temp = temp->nextElement;
    }
}

void NetworkList::load()
{
    int count = EEPROM.read(0);
    char SSID[33];
    char Passwd[64];
    int j = 1;
    int len = 0;
    for (int i = 0; i < count; i++)
    {
        len = EEPROM.read(j++);
        for (int k = 0; k < len; k++)
            SSID[k] = EEPROM.read(j++);
        SSID[len] = '\0';

        len = EEPROM.read(j++);
        if(len == 0) addNetwork(new Network(SSID));
        else
        {
            for (int k = 0; k < len; k++)
                Passwd[k] = EEPROM.read(j++);
            Passwd[len] = '\0';
            addNetwork(new Network(SSID, Passwd));
        }
    }
}
bool NetworkList::init()//false if inited, true if already existed
{
    if (EEPROM.length() > 0) return true;
    EEPROM.begin(97*5+1);//(MAX_SSID+MAX_Passwd+L1+L2)*5+count    (+CRC)
    EEPROM.write(0, 0);
    return false;  
}*/
void NetworkList::store()
{
    SPIFFS.remove("/data");
    File temp = SPIFFS.open("/data", "w");
    //temp.print((unsigned char)Length);
    NetworkElement* list = firstElement;
    /*while(list != nullptr);
    {
        Serial.println("STORE");
        unsigned char len = (unsigned char)strlen(list->data->SSID);
        temp.write(len);
        //temp.write(list->data->SSID);
        temp.print(list->data->SSID);
        len = (unsigned char)strlen(list->data->Password);
        temp.write(len);
        if (len > 0) temp.print(list->data->Password);
        list = list->nextElement;
    }*/
    for(unsigned int i = 0; i < Length; i++)
    {
        temp.print(list->data->SSID); temp.print(';');
        if (list->data->Password != (char*) __null) temp.print(list->data->Password);
        temp.print(';');
        list = list->nextElement;
    }
    temp.close();
}

void NetworkList::load()
{
    if(!SPIFFS.exists("/data")) init();
    File temp = SPIFFS.open("/data", "r");
    //int count = temp.read();
    char SSID[33];
    char Passwd[64];
    /*for (int i = 0; i < count; i++)
    {
        Serial.println("LAODING ELEMENT");
        len = temp.read();
        temp.read(SSID, len);
        len = temp.read();
        if(len == 0) addNetwork(new Network((char*)SSID));
        else
        {
            temp.read(Passwd, len);
            addNetwork(new Network((char*)SSID, (char*)Passwd));
        }
    }*/
    while(temp.available())
    {
        strcpy(SSID, temp.readStringUntil(';').c_str());
        strcpy(Passwd, temp.readStringUntil(';').c_str());
        addNetwork(new Network(SSID, Passwd));
    }
    temp.close();
}

bool NetworkList::init()//false if inited, true if already existed
{
    File temp = SPIFFS.open("/data", "w");
    temp.close();
    return true;
}

NetworkElement* NetworkList::getFirstElement()
{
    return firstElement;
}

unsigned int NetworkList::getLength()
{
    return Length;
}