/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
//#define BLYNK_PRINT Serial


// /* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "ENTER YOUR TEMPLATE ID HERE"
#define BLYNK_TEMPLATE_NAME "ENTER YOUR TEMPLATE NAME HERE"
#define blynk_auth_token "ENTER YOUR AUTH TOKEN HERE"




#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "ENTER YOU AUTH TOKEN HERE";


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ENTER YOUR WIFI SSID HERE";
char pass[] = "ENTER YOUR WIFI PASSWORD HERE";
const char *ssid_ap = "ESP32 Garage Door Control";

int RELAY = 8;
int doorStatus = 5;
int state = 0;

BlynkTimer timer;

WidgetLED led3(V3);
WidgetLED led4(V4);


void doorCheck() {
  state = digitalRead(doorStatus);
  //Serial.println(state);


  if (state == LOW) {
    //    led3.on();
    //    led4.on();
    Blynk.virtualWrite(V0, state);
    Blynk.virtualWrite(V3, state);
    Blynk.virtualWrite(V4, !state);
  }
  else {
    //    led3.off();
    //    led4.off();
    Blynk.virtualWrite(V0, state);
    Blynk.virtualWrite(V3, state);
    Blynk.virtualWrite(V4, !state);
  }
}

void connectionCheck() {

  int tryCount = 0;
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("wifi disconected, trying to reconnect");
    tryCount++;
    WiFi.reconnect();
    Serial.println(".");
    vTaskDelay(4000);
    
    if (tryCount == 10) {
      Serial.println("restarting...");
      ESP.restart();
    }
    else {
      tryCount = 0;
    }
  }
  if (!Blynk.connected()){
    vTaskDelay(5000);
    Blynk.connect();
  }

}

void restartSystem() {
  ESP.restart();
}


void setup()
{
  // Debug console

  pinMode(RELAY, OUTPUT);
  pinMode(doorStatus, INPUT_PULLUP);


  Serial.begin(9600);



  Blynk.begin(auth, ssid, pass);

  timer.setInterval(500L, doorCheck);
  timer.setInterval(10000L, connectionCheck);
  timer.setInterval(86400000L,restartSystem);

}

void loop()
{
  Blynk.run();
  timer.run();
}

BLYNK_WRITE(V13) {
  int pinVal = param.asInt();
  digitalWrite(RELAY, pinVal);

}
