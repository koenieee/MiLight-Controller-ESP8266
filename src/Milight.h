/*************************************************************************
Title:    Milight / LimitlessLED LED Strip Network Library for Arduino
Authors:  Nathan Holmes <maverick@drgw.net>, Colorado, USA
          Michael Petersen <railfan@drgw.net>, Colorado, USA
		  Koen, Utrecht, Netherlands
File:     Milight.h
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

#ifndef _MILIGHT_H_
#define _MILIGHT_H_

#include <SPI.h>
#include <WiFiUDP.h>


#define MILIGHT_QUEUE_DEPTH 64

#define MILIGHT_COLOR_WHITE 0x00

#define MILIGHT_COLOR_BLUE    0x01
#define MILIGHT_COLOR_AQUA    0x20
#define MILIGHT_COLOR_CYAN    0x40
#define MILIGHT_COLOR_MINT    0x50
#define MILIGHT_COLOR_GREEN   0x60
#define MILIGHT_COLOR_LIME    0x7E
#define MILIGHT_COLOR_YELLOW  0x90
#define MILIGHT_COLOR_ORANGE  0xA0
#define MILIGHT_COLOR_RED     0xAE
#define MILIGHT_COLOR_PINK    0xC0
#define MILIGHT_COLOR_FUCHSIA 0xD0
#define MILIGHT_COLOR_PURPLE  0xF0

#define MILIGHT_CH_ALL        0
#define MILIGHT_CH_1          1
#define MILIGHT_CH_2          2
#define MILIGHT_CH_3          3
#define MILIGHT_CH_4          4

typedef struct
{
  uint8_t data[2];
  uint8_t repeats;
} MilightCmd;

typedef struct
{
  uint8_t headIdx;
  uint8_t tailIdx;
  bool full;
  MilightCmd pktData[MILIGHT_QUEUE_DEPTH];
} MilightCmdQueue;


class Milight
{
  public:
    Milight();
    void begin(WiFiUDP * udp, IPAddress * addr, uint16_t port);
    void on(uint8_t channel, uint8_t color, uint8_t intensity);
    void off(uint8_t channel);
    void workQueue();
    uint8_t packetsQueued();
    
  private:
    IPAddress * destAddr;
    WiFiUDP * udp;
    bool useShortPkt;
    uint16_t port;
    void send(uint8_t cmd1, uint8_t cmd);
    uint32_t lastUpdateMillis;
    MilightCmdQueue queue;
    uint8_t pktRepeats;
    
};

#endif

