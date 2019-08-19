/*************************************************************************
Title:    Milight / LimitlessLED LED Strip Network Library for Arduino
Authors:  Nathan Holmes <maverick@drgw.net>, Colorado, USA
          Michael Petersen <railfan@drgw.net>, Colorado, USA
		  Koen, Utrecht, Netherlands
File:     Milight.cpp
License:  GNU General Public License v3

LICENSE:
    Copyright (C) 2015 Nathan Holmes
    
    The latest source can be obtained from ISE's Github repository here:
	Arduino: https://github.com/IowaScaledEngineering/Milight
	ESP8266: https://github.com/koen-github/MiLight-Controller-ESP8266

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License along
    with this program. If not, see http://www.gnu.org/licenses/
    
*************************************************************************/

#include <Milight.h>

Milight::Milight()
{
  this->destAddr = 0;
  this->udp = 0;
  this->port = 8899;
  this->lastUpdateMillis=0;
  this->queue.headIdx = this->queue.tailIdx = 0;
  this->queue.full = false; 
  this->useShortPkt = false;
  this->pktRepeats = 3;
  
}

uint8_t Milight::packetsQueued()
{
  if(true == this->queue.full)
    return(MILIGHT_QUEUE_DEPTH);
  return((this->queue.headIdx - this->queue.tailIdx ) % MILIGHT_QUEUE_DEPTH);
}

void Milight::workQueue()
{
  if (0 == this->packetsQueued())
    return;

  if ((millis() - lastUpdateMillis) > 50)
  {
    uint8_t command[3];

/*    Serial.print(" Depth ");
    Serial.print(this->packetsQueued());
    Serial.print("\n");*/
    
    command[0] = this->queue.pktData[this->queue.tailIdx].data[0];
    command[1] = this->queue.pktData[this->queue.tailIdx].data[1];
    command[2] = 0x55;
    this->udp->beginPacket(*(this->destAddr), this->port);
    this->udp->write(command, this->useShortPkt?2:3);
    this->udp->endPacket();  

/*    Serial.print("Sending [");
    Serial.print(command[0], HEX);
    Serial.print(" ");
    Serial.print(command[1], HEX);
    Serial.print(" ");
    Serial.print(command[2], HEX);
    Serial.print("]\n");*/

    if (--this->queue.pktData[this->queue.tailIdx].repeats == 0)
    {
      if( ++this->queue.tailIdx >= MILIGHT_QUEUE_DEPTH )
        this->queue.tailIdx = 0;
      this->queue.full = false;
    }
    lastUpdateMillis = millis();
  
  }
}

void Milight::send(uint8_t cmd1, uint8_t cmd2)
{

    
  // If full, bail with a false
  if (true == this->queue.full)
  {
      // Not sure what to do here - wait and work queue?
      return;
  }

  this->queue.pktData[this->queue.headIdx].data[0] = cmd1;
  this->queue.pktData[this->queue.headIdx].data[1] = cmd2;
  this->queue.pktData[this->queue.headIdx].repeats = this->pktRepeats;
  
  if( ++this->queue.headIdx >= MILIGHT_QUEUE_DEPTH )
    this->queue.headIdx = 0;
  if (this->queue.headIdx == this->queue.tailIdx)
    this->queue.full = true;

/*    Serial.print("Queuing [");
    Serial.print(cmd1, HEX);
    Serial.print(" ");
    Serial.print(cmd2, HEX);
    Serial.print(" 0x55]\n");*/
}

void Milight::begin(WiFiUDP * udp, IPAddress * addr, uint16_t port)
{
  this->destAddr = addr; 
  this->udp = udp;
  this->port = port;
}

void Milight::on(uint8_t channel, uint8_t color, uint8_t intensity)
{
  if (intensity > 100)
    intensity = 100;
  
  if (channel > 4)
    return;
  
  if (0 == intensity)
  {
    // Eh, basically an off request
    this->off(channel);
    return;
  }

  intensity = ((uint16_t)(intensity-1)*66) >> 8;

  if (0 == intensity)
    intensity++;

  // Color 0 is specially redefined as white here
  if (0 == color)
  {
    this->send((0==channel)?0x42:(0x43 + channel*2), 0x00);    
    // Need to send the "change to white command"
    this->send((0==channel)?0xC2:(0xC3 + channel*2), 0);    
    this->send(0x4E, intensity);     
  }  
  else
  {
  // 0x03 is pure blue
  // 0x58 is pure green
  // 0xAE is pure red
      this->send((0==channel)?0x42:(0x43 + channel*2), 0);
      this->send(0x40, color);
      this->send(0x4E, intensity);     
  }
}

void Milight::off(uint8_t channel)
{
  if (channel > 4)
    return;  
  this->send((0==channel)?0x41:(0x44 + channel*2), 0);
}



