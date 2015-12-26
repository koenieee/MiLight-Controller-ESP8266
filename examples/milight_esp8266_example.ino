#include <ESP8266WiFi.h>
#include <Milight.h>
#include <WiFiUDP.h>

/*************************************************************************
This example assumes that you have:
1) a Milight / LimitlessLED Wifi gateway
2) the gateway is paired with an RGBW Milight LED strip driver
3) the gateway is connected to your wireless network
4) the wireless network is bridged to your wired network
5) and that the wired network is connected to a standard Ethernet Shield
    attached to your Arduino
Things you'll need to configure:
1) The IP address of your milight gateway
2) The port that your milight gateway speaks on
3) The SSID of your Home WiFi
4) The Password of your Home Wifi
*************************************************************************/

// wifi connection variables
const char* ssid     = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const unsigned int milight_port = 8899;
IPAddress milight_ip(192, 168, 178, 20);

boolean wifiConnected = false;

WiFiUDP UDP;

Milight ml;


boolean connectWifi();

unsigned long nextAdvance = 0;
uint8_t lighting_state;

void setup() {
  // Initialise Serial connection
  Serial.begin(115200);

  // Initialise wifi connection
  wifiConnected = connectWifi();

  // Initialize Milight / LimitlessLED controller with the chosen IP and port, as well as
  // access to the UDP object to actually send packets with
  ml.begin(&UDP, &milight_ip, milight_port);

  // Set nextAdvance to the current time
  nextAdvance = millis();


}

void loop() {
  // check if the WiFi connection were successful
  if (wifiConnected) {
    // Milight.workQueue() must be called as often as possible
    // Its job is to send packets on prescribed time intervals without
    // stalling the main loop.  Thus, your main loop() function should be free-running
    // without any long stalls.
    ml.workQueue();

    // If it's not time to advance the state machine, just keep running around the loop
    if (millis() < nextAdvance)
      return;

    // This is the state machine that shows off what the library can do
    // Its job is to check if a certain amount of time has passed, and then change the light
    // configuration
    switch (lighting_state)
    {
      case 0:
        Serial.print("Lighting off\n");
        ml.off(MILIGHT_CH_1);
        nextAdvance = millis() + 4000;
        lighting_state = 1;
        break;

      case 1:
        Serial.print("Lighting white at 100%\n");
        ml.on(MILIGHT_CH_1, MILIGHT_COLOR_WHITE, 100);
        nextAdvance = millis() + 3000;
        lighting_state = 2;
        break;

      case 2:
        Serial.print("Lighting white at 75%\n");
        ml.on(MILIGHT_CH_1, MILIGHT_COLOR_WHITE, 75);
        nextAdvance = millis() + 3000;
        lighting_state = 3;
        break;

      case 3:
        Serial.print("Lighting white at 25%\n");
        ml.on(MILIGHT_CH_1, MILIGHT_COLOR_WHITE, 25);
        nextAdvance = millis() + 3000;
        lighting_state = 4;
        break;

      case 4:
        Serial.print("Lighting blue at 60%\n");
        ml.on(MILIGHT_CH_1, MILIGHT_COLOR_BLUE, 60);
        nextAdvance = millis() + 3000;
        lighting_state = 5;
        break;

      case 5:
        Serial.print("Lighting green at 60%\n");
        ml.on(MILIGHT_CH_1, MILIGHT_COLOR_GREEN, 60);
        nextAdvance = millis() + 3000;
        lighting_state = 6;
        break;

      case 6:
        Serial.print("Lighting red at 60%\n");
        ml.on(MILIGHT_CH_1, MILIGHT_COLOR_RED, 60);
        nextAdvance = millis() + 3000;
        lighting_state = 0;
        break;
    }
  }
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }
  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  return state;
}
