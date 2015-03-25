#include <Arduino.h>
#include "Client.h"
#include <string.h>


#define SERVER "dev.thingsguru.com"
#define PORT 2355
#define VARPACKTIME 10000UL

class ThingsGuru {
private:
	Client* client;
	String msgBuffer;
	bool blkd;
	String deviceId;
	String deviceKey;
	String server;
	String port;
	void msg(String& data);
	void (*callback)(String msg, String error);
	unsigned long lastVarPack = millis();
	String dataPack;
	void sendData();
	bool notAuth = 1;
	bool waitForAuth();
public:
	bool loop();
	void disconnect();
	bool connect();
	void addData(String key, String val);
	ThingsGuru(Client& client, String deviceId, String deviceKey, void(*)(String msg, String error));
};