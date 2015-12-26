# Milight
An ESP8266 library to control Milight / LimitlessLED LED strip controllers via the WiFi interface of the ESP8266 using the bridge that is connected to the MiLight bulbs.

#Thanks To

The original MiLight code worked only with the Arduino Ethernet shield, I modified the existing code so it now also works on the ESP8266 WiFi microcontroller.

(Arduino) repo can be found here: https://github.com/IowaScaledEngineering/Milight

A working example for the ESP8266 can be found in the examples directory.

# Functions
```cpp
void Milight::begin(WiFiUDP * udp, IPAddress * addr, uint16_t port)
```

The begin() function sets up the Milight control object, connects it with an active WiFi UDP connection, and tells it where to find the Milight Wifi gateway.  
When the WiFiUDP object is passed in, it should already be connected to the same network the MiLight WiFi Bridge is on.

addr is the address of the target Milight Wifi gateway, and port is the UDP port on which to connect.


```cpp
void Milight::on(uint8_t channel, uint8_t color, uint8_t intensity)
```
The on() method sets a given channel to a certain color and intensity.

channel is a number, 0-4. 0 means 'all channels', whereas 1-4 correspond to channels 1-4 on the controller

color is a number 0x00-0xFF.  The colors are as defined by Milight / LimitlessLED, except for 0x00, which is overloaded to be "white"

intensity is a percentage, 0-100.  0 is the same as executing the off() method, and will shut off the LEDs on a given channel.  100 is full on.


```cpp
void Milight::off(uint8_t channel)
```

The off() method shuts off a given channel.

channel is a number, 0-4. 0 means 'all channels', whereas 1-4 correspond to channels 1-4 on the controller

```cpp
void Milight::workQueue()
```

The workQueue() function should be called each time around the loop.  
It checks if there are commands queued up to be sent (they can only be sent to the gateway so quickly) and if enough time has passed, it will transmit one.  
Otherwise it returns immediately.  Your main loop() function should not stall, otherwise packets will not get transmitted in a timely fashion.

