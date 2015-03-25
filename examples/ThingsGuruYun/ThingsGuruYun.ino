#include <SPI.h>
#include <YunClient.h>
#include <ThingsGuru.h>
#include <Console.h>

YunClient yunClient;


void commands(String msg, String err)
{
  Console.println("Message: "+msg);
  Console.println("Err: "+err);
  if(msg == "lightOn\n")
  {
  Console.println("ON");
    digitalWrite(13, HIGH);
  }
  if(msg == "lightOff\n")
  {
    digitalWrite(13, LOW);
  }
}

ThingsGuru guru(yunClient, "radiotoxic-NyyTmaq", "4Jl1T7a9-tapaderas", commands);


void setup() {
  pinMode(13, OUTPUT);

  Bridge.begin();
  Console.begin();
  Console.println("Starting to connect");
  if(guru.connect())
  {
     Console.println("Connected to ThingsGuru");
  }
  else
  {
     Console.println("Error Connecting to ThingsGuru"); 
  }

}

float temp;
void loop() {
  // put your main code here, to run repeatedly:
    temp = analogRead(0);
    temp = temp * 0.48828125;
    String tempStr = String(temp);
    char tempString[15];
    tempStr.toCharArray(tempString, 15);
    guru.addData("temp",tempString);
    guru.addData("myData","newStringData");

    guru.loop();

}
