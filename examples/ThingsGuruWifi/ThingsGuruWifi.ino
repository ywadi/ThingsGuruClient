#include <SPI.h>
#include <ThingsGuru.h>
#include <WiFi.h>

char ssid[] = "ArabiaWeather2";          //  your network SSID (name)
char pass[] = "arabia2015weather";   // your network password

int status = WL_IDLE_STATUS;
IPAddress server(188, 166, 20, 19);

WiFiClient wifiClient;

void commands(String msg, String err)
{
  Serial.println("Message: "+msg);
  Serial.println("Err: "+err);
}

ThingsGuru guru(wifiClient, "jetson-V1Smpjq", "EkxB7pic-sorbets", commands);

void setup() {

  Serial.begin(9600);
  Serial.println("Attempting to connect to WPA network...");
  Serial.print("SSID: ");
  Serial.println(ssid);

  status = WiFi.begin(ssid, pass);
  if ( status != WL_CONNECTED) {
    Serial.println("Couldn't get a wifi connection");
    // don't do anything else:
    while (true);
  }
  else {
    Serial.println("Connected to wifi");
    Serial.println("\nStarting connection...");
    // if you get a connection, report back via serial:
    guru.connect();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  guru.loop();
  guru.addData("yousef","wadi");
  guru.addData("maisa","tanni");
}
