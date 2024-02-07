/*
 * This is a program to take inputs from buttons on and off to control smart lights in my room via ifttt and show the current status of lights in the room on the lcd display
 *
 */


#include <Arduino.h>

#include <ESP8266WiFi.h>


#define WIFI_SSID         "Adisap"
#define WIFI_PASS         "R0ckstars"

#include <LiquidCrystal.h>
#define D0 3 // GPIO3 maps to Arduino D0
#define D1 1 // GPIO1 maps to Arduino D1
#define D2 16 // GPIO16 maps to Arduino D2
#define D3 5 // GPIO5 maps to Arduino D3
#define D4 4 // GPIO4 maps to Arduino D4
#define D5 14 // GPIO14 maps to Arduino D5
#define D6 12 // GPIO12 maps to Arduino D6
#define D7 13 // GPIO13 maps to Arduino D7
#define D8 0 // GPIO0 maps to Arduino D8
#define D9 2 // GPIO2 maps to Arduino D9
#define D10 15 // GPIO15 maps to Arduino D10
#define APP_SECRET        "9c8bf099-4319-4741-9c54-4501ce2df9d8-80fa0a0d-b530-49f4-a56d-67b4725fd8a6"   // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"
#define MOTIONSENSOR_ID   "65c3261accc93539a134c2c1"    // Should look like "5dc1564130xxxxxxxxxxxxxx"
#define BAUD_RATE         115200                // Change baudrate to your need
#define APP_KEY          "b45e2197-0e03-4691-85d8-623051198f55"
//button up is on pin 1 which is for on
//button down in on 2 which is for off
int buttonvalue;
#include "SinricPro.h"
#include "SinricProMotionsensor.h"
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
int adc_key_in = 0;
bool lastMotionState = false;
unsigned long lastChange = 0;
int onoroff;
LiquidCrystal lcd(D8,D9,D4,D5,D6,D7);
int but()
{
    int adc_key_in = analogRead(0);      // read the value from the sensor
    // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
    // we add approx 50 to those values and check to see if we are close
    if (adc_key_in > 1500) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 195)  return btnUP;
    if (adc_key_in < 500)  return btnDOWN;
    if (adc_key_in < 700)  return btnLEFT;
    //return nothing when nothing is pressed
    return btnNONE;  // when all others fail, return this...
}
void setupWiFi() {
    Serial.printf("\r\n[Wifi]: Connecting");
    lcd.print("Connecting WiFi");


    WiFi.setSleepMode(WIFI_NONE_SLEEP);
    WiFi.setAutoReconnect(true);


    WiFi.begin(WIFI_SSID, WIFI_PASS);
    lcd.setCursor(0,1);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.printf(".");
        lcd.print(".");
        delay(250);
    }
    IPAddress localIP = WiFi.localIP();
    Serial.printf("connected!\r\n[WiFi]: IP-Address is %d.%d.%d.%d\r\n", localIP[0], localIP[1], localIP[2], localIP[3]);
    lcd.clear();
    lcd.print("Connected WiFi");
    delay(2000);
    lcd.clear();
}
void motion() {
    if (buttonvalue == 1) {
        onoroff = 1;
    }
    else if (buttonvalue == 2) {
        onoroff = 0;
    }
}
void handleMotionsensor() {
    unsigned long actualMillis = millis();
    if (actualMillis - lastChange < 250) return;          // debounce motionsensor state transitions (same as debouncing a pushbutton)

    bool actualMotionState = onoroff;   // read actual state of motion sensor

    if (actualMotionState != lastMotionState) {         // if state has changed
        Serial.printf("Motion %s\r\n", actualMotionState?"detected":"not detected");
        lastMotionState = actualMotionState;              // update last known state
        lastChange = actualMillis;                        // update debounce time
        SinricProMotionsensor &myMotionsensor = SinricPro[MOTIONSENSOR_ID]; // get motion sensor device
        bool success = myMotionsensor.sendMotionEvent(actualMotionState);
        if(!success) {
            Serial.printf("Something went wrong...could not send Event to server!\r\n");
        }
    }
}

void setupSinricPro() {
    // add doorbell device to SinricPro
    SinricProMotionsensor& myMotionsensor = SinricPro[MOTIONSENSOR_ID];
    // setup SinricPro
    SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); lcd.clear(); lcd.print("Connected Sinric"); delay(2000); lcd.clear();});
    SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); lcd.clear(); lcd.print("Disconnected"); lcd.clear(); lcd.print("RESET"); lcd.setCursor(0,1); lcd.print("DEVICE"); });
    SinricPro.begin(APP_KEY, APP_SECRET);
}

void setup() {
// write your initialization code here
Serial.begin(9600);
    lcd.begin(16, 2);
    setupWiFi();

    setupSinricPro();

}

void loop() {
    // write your code here
    buttonvalue = but();
    if (buttonvalue == 1) {
        lcd.clear();
        lcd.print("Lights On");
        onoroff = 1;
    }
    else if (buttonvalue == 2) {
        lcd.clear();
        lcd.print("Lights Off");
        onoroff = 0;
    }
    handleMotionsensor();
    SinricPro.handle();



    delay(100);

}