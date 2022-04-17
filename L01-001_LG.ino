/* ตัวรับ ฝั่ง Node MCU */
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>
#include <WiFiClient.h>

SoftwareSerial ArduinoSerial(D1, D0); // RX | TX

//Set these to your desired credentials.
const char *ssid = "ssid";
const char *password = "password";

String host = "http://192.168.43.108/Api/add.php";


int WTime;
int Mode;
String State;
String WID = "L01-006";
String Model = "WD-14180FDS";
String Location_ID = "L01";

String payload;

void setup() {

  ArduinoSerial.begin(9600);

  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  //ส่วน Hardware

 //ส่งข้อมูล mode ให้ arduino
  while (Serial.available() > 0) {
    Mode = Serial.read();
    if ( Mode == '1' || Mode == '2' || Mode == '3' || Mode == '4' || Mode == '5' || Mode == '6' || Mode == '7' || Mode == '8' ) {
      Serial.print(" Mode : ");
      Serial.write(Mode);
      Serial.write("\n");
      ArduinoSerial.print(Mode);
    }
  }
  //รับข้อมูล เวลา และ โพรเสจ จาก Arduino
  while (ArduinoSerial.available()) {
    WTime = ArduinoSerial.read ();
    Serial.print("\nTime is :");
    Serial.println(WTime);
    State = ArduinoSerial.readString();
    Serial.print("State is :");
    Serial.print(State);
    Serial.flush();

    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

      http.begin(client, "http://192.168.43.108/Api/add.php");

      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      int WTimeData = WTime;

      String httpRequestData = "WID=" + WID + "&Model=" + Model + "&Location_ID=" + Location_ID + "&WTime=" + String (WTimeData) + "&State=" + State + "";

      Serial.print("httpRequestData: ");
      Serial.print(httpRequestData);

      int httpResponseCode = http.POST(httpRequestData);

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end(); //Close connection
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
}
