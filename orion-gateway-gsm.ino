


/*

Orion Gateway GSM
- Added GSM SMS functionality using  SIM 800L GSM Module
----------------------------------
Author: Bryce Narciso C. Mercines
*/

// API URL
String API_URL = "http://kraken-demo.herokuapp.com";
#include <HTTPClient.h>
#include "WiFi.h"

// gsm setup
#include <SoftwareSerial.h>
SoftwareSerial sim(12, 13);

int _timeout;
String _buffer;
// Change Mobile Number
String number = "MOBILE_NUMBER";

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

// Most performant ISM Band (Philippines) good RSSI
#define BAND 923E6

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//LED INDICATOR
#define BROADCAST_INDICATOR 23

// Initialize Screen
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

// LoRa Data
String LoRaData;

HTTPClient http;

// Replace with your network credentials
const char *ssid = "ZTE_2.4G_MK5p9F";
const char *password = "W6mPecNb";

// gateway unique id
const char *uid = "HTX-00001";

void setup()
{

  pinMode(BROADCAST_INDICATOR, OUTPUT);

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false))
  { // Address 0x3C for 128x32
    //Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("ORION GATEWAY");
  display.display();

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND))
  {
    //Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }
  //Serial.println("LoRa Initializing OK!");
  display.setCursor(0, 10);
  display.print("LoRa Initializing OK!");
  display.display();
  delay(2000);

  WiFi.begin(ssid, password);


  // set up GSM
   delay(7000); //delay for 7 seconds to make sure the modules get the signal
  Serial.begin(9600);
  _buffer.reserve(50);
//  Serial.println("GSM System Started...");
// sim.begin(9600);
  delay(1000);
}

// set GSM Message
void SendMessage()
{
  //Serial.println ("Sending Message");
  Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  //Serial.println ("Set SMS Number");
  Serial.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(1000);
  String SMS = "GSM TEST";
  Serial.println(SMS);
  delay(100);
  Serial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
//  _buffer = _readSerial();
}

// Send LoRa packet

void sendData(String LORA_DATA)
{
  LoRa.beginPacket();
  LoRa.print(LORA_DATA);
  LoRa.endPacket();
}

// update OLED Screen

void setScreen(int yrssi, String message)
{
  // Dsiplay information
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("ORION GATEWAY");
  display.println(WiFi.localIP());
  display.setCursor(0, 20);
  display.print("Activity");
  display.setCursor(0, 30);
  display.print(message);
  display.setCursor(0, 40);
  display.print("RSSI:");
  display.setCursor(30, 40);
  display.print(yrssi);
  display.display();
}

// attempt to send data to the internet

void gateWayConnect(String LORA_DATA, int xrssi)
{

  setScreen(xrssi, "Connection Attempt!");
  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(BROADCAST_INDICATOR,HIGH);
    // send report data
    String reqURL = "data=" + LORA_DATA;
    http.begin(API_URL + "/report");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.POST(reqURL);
    setScreen(xrssi, "report sent!");
    http.end(); //Close connection

    // get data feed
    //    http.begin(API_URL+"/network/feed");
    //    int getFeed = http.GET();
    //    if(getFeed > 0){
    //    setScreen(xrssi, "EMIT DATA..");
    //    String FeedData = String(http.getString());
    //
    //    // broadcast advisories multiple times
    //    if(FeedData.length() > 10){
    //    Serial.println("Broadcasting Feed");
    //      sendData(FeedData);
    //      delay(2000);
    //      sendData(FeedData);
    //      delay(2000);
    //      sendData(FeedData);
    //      delay(2000);
    //       setScreen(xrssi, "READY");
    //    }
    //
    //    }
    //    http.end();

    delay(2000);
  }
  else
  {
    setScreen(xrssi, "Echoing..");
    sendData(LORA_DATA);
    delay(2000);
    sendData(LORA_DATA);
    delay(2000);
    sendData(LORA_DATA);
    delay(2000);
    setScreen(xrssi, "READY");
        digitalWrite(BROADCAST_INDICATOR,LOW);
  }
}

void loop()
{
  //try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    //received a packet

    //read packet
    while (LoRa.available())
    {
      LoRaData = LoRa.readString();
    }

    //print RSSI of packet
    int rssi = LoRa.packetRssi();
    gateWayConnect(LoRaData, rssi);
  }

  // GSM test

  SendMessage();
  delay(10000);
}