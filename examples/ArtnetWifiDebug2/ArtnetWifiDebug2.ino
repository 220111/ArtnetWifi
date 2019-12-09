/*
Example, transmit all received ArtNet messages (Op-Code DMX) out of the
serial port in plain text.
Function pointer with the C++11 function-object "std::function".

Stephan Ruloff 2019
https://github.com/rstephan/ArtnetWifi

*/
#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <WiFiUdp.h>
#include <ArtnetWifi.h>

//Wifi settings
const char* ssid = "ssid";
const char* password = "pAsSwOrD";

WiFiUDP UdpSend;
ArtnetWifi artnet;

// connect to wifi – returns true if successful or false if not
boolean ConnectWifi(void)
{
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
    if (i > 20){
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
  } else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}

void setup()
{
  // set-up serial for debug output
  Serial.begin(115200);
  ConnectWifi();

  // this will be called for each packet received
  artnet.setArtDmxFunc([](DMX_FUNC_PARAM){
    boolean tail = false;

    Serial.printf("DMX: Univ: %i, Seq: %i, Data (%i): ", universe, sequence, length);

    if (length > 16) {
      length = 16;
      tail = true;
    }
    // send out the buffer
    for (int i = 0; i < length; i++) {
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
    if (tail) {
      Serial.print("...");
    }
    Serial.println();
  });
  artnet.begin();
}

void loop()
{
  // we call the read function inside the loop
  artnet.read();
}
