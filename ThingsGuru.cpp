#include "ThingsGuru.h"

bool ThingsGuru::connect(){
    if(blkd) 
    {
      //Serial.println("BLOCKED");
      return false;
    }
    if (client->connect(SERVER, PORT))
    {
      //MSG Type Connect :: Device Key :: Device Id \n
      String authJson = "{\"type\":\"con\", \"deviceId\":\""+deviceId+"\", \"deviceKey\":\""+deviceKey+"\"}";
      client->println(authJson);
	  if(waitForAuth())
	  {
      	return true;
	  }
	  else
	  {
		  return false;
	  }
    }
    else
    {
      return false;
    }
}

bool ThingsGuru::waitForAuth()
{
	String authBuffer = "";
	//Serial.println("Waiting Auth");
	while(notAuth)
	{
	  	  if (client->available()){
	      	char c = client->read();
	      	authBuffer += c;
	      	if(c == '\n')
	      	{
	      	   //Serial.println("End Of Auth Msg-->");
	      	   //Serial.println(authBuffer);
			   if(authBuffer == "thingsguruisauth\n")
			   {
				   notAuth = false;
				   return true;
			   }
			   else
			   {
				   notAuth = true;
				   return false;
			   }
	      	}
		}
	}
}


void ThingsGuru::msg(String& data)
{
	//Serial.println("---MSGFUNCTION-----");
	//Serial.println(data);
	//Serial.println("-------------------");
  if(data == "thingsguruis!auth\n")
  {
    //Serial.println("HERE NOT AUTH");
    blkd = true;
    client->stop();
	callback("","Incorrect deviceId or deviceKey.");
  }
  else if(data == "thingsgurutoofreq\n")
  {
      //Serial.println("HERE FREQ");
      blkd = true;
      client->stop();
	  callback("","Sending Data Too Frequent.");
  }
  else if(data == "thingsguruisauth\n")
  {
      //Serial.println("HERE AUTH");
      notAuth = false;
	  callback("","Auth. Success");
  }
  else
  {
	  callback(data, "");
  }
  
}

bool ThingsGuru::loop(){
  
  if (!client->connected() && !blkd && !notAuth) {
    //Serial.println();
    //Serial.println("Reconnecting");
    if(!connect())
    {
      return false;
    }
  }
  else  
  {
	  if(!notAuth)
	  {
	  	sendData();
	  }
	  if (client->available()){
    	char c = client->read();
    	msgBuffer += c;
    	if(c == '\n')
    	{
    	   //Serial.println("End Of Msg-->");
    	   //Serial.println(msgBuffer);
		   msg(msgBuffer);
    	   msgBuffer = "";
    	}
	  }
	  return true;
  }
}

void ThingsGuru::disconnect()
{
	client->stop();
}

void ThingsGuru::addData(String key, String val)
{
	//TODO: escape single quote
	dataPack += "\""+key+"\":\""+val+"\",";
}

void ThingsGuru::sendData()
{
	//Replace ' witj \' (TODO:NODEJS AFTER JSON PARSE RETURN)
	//TODO: Only Send data if auth && time framed
	if (millis() - lastVarPack > VARPACKTIME) {
		//Serial.println("SendPack OK!");
		dataPack = "{"+dataPack+"\"type\":\"data\"}";
	    client->println(dataPack);
	    lastVarPack = millis();
		dataPack ="";
	}
	else
	{
		dataPack = "";
	}
}


ThingsGuru::ThingsGuru(Client& client, String deviceId, String deviceKey, void (*callback)(String msg, String error)){
	this->client = &client;
	this->callback = callback;
	this->deviceId = deviceId;
	this->deviceKey = deviceKey;
}
