/*
    Name:       inlupp1_Azure.ino
    Created:	2019-01-30 15:33:23
    Author:     LAPTOP-FTI3RT4H\gusta
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>

#include <AzureIoTHub.h>
#include <AzureIoTProtocol_MQTT.h>
#include <AzureIoTUtility.h>

#include "config.h"
static bool messagePending = false;
static bool messageSending = true;

static char *connectionString = "HostName=assignment-3-IoTHub.azure-devices.net;DeviceId=TempSensor;SharedAccessKey=8r0KvOubhuSpyH7MOdXYgaJBbQU/BhTjIvjmMdOi4+w=";


static int send_interval = SEND_INTERVAL * 1e3;
static int check_interval = CHECK_INTERVAL * 1e6;
ESP8266WiFiMulti wifiMulti;

unsigned long currentMillis;
unsigned long lastSentMillis;

static IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;

void blinkLED()
{
	digitalWrite(LED_PIN, HIGH);
	delay(500);
	digitalWrite(LED_PIN, LOW);
}

void initMultiWifi() {

	wifiMulti.addAP("iPhone x", "hejhejhej");
	wifiMulti.addAP("Caesar's Palace 2.0", "kingofpop");
	wifiMulti.addAP("IoT", "IoT2018!");
	Serial.println("Connecting");
	while (wifiMulti.run() != WL_CONNECTED) {
		delay(250);
		Serial.print('.');
	}
	Serial.println("\r\n");
	Serial.print("Connected to ");
	Serial.println(WiFi.SSID());
	Serial.print("IP address:\t");
	Serial.print(WiFi.localIP());
	Serial.println("\r\n");
}

void initTime()
{
	time_t epochTime;
	configTime(0, 0, "pool.ntp.org", "time.nist.gov");

	while (true)
	{
		epochTime = time(NULL);

		if (epochTime == 0)
		{
			Serial.println("Fetching NTP epoch time failed! Waiting 2 seconds to retry.");
			delay(2000);
		}
		else
		{
			Serial.printf("Fetched NTP epoch time is: %lu.\r\n", epochTime);
			break;
		}
	}
}

void initIoTClient() {
	iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionString, MQTT_Protocol);
	if (iotHubClientHandle == NULL)
	{
		Serial.println("Failed on IoTHubClient_CreateFromConnectionString.");
		while (1);
	}
	//lastSentMillis = 0;

	IoTHubClient_LL_SetOption(iotHubClientHandle, "product_info", "FeatherHuzzah");
	IoTHubClient_LL_SetMessageCallback(iotHubClientHandle, receiveMessageCallback, NULL);
	IoTHubClient_LL_SetDeviceMethodCallback(iotHubClientHandle, deviceMethodCallback, NULL);
	IoTHubClient_LL_SetDeviceTwinCallback(iotHubClientHandle, twinCallback, NULL);
}

void setup()
{
	pinMode(LED_PIN, OUTPUT);
	initSerial();
	Serial.setTimeout(2000);

	String resetReason = ESP.getResetReason();
	Serial.println(resetReason);
	if (resetReason == "Deep-Sleep Wake") {
		initMultiWifi();
		initTime();
		initSensor();
		String fromEEPROM = readEEPROM();
		String currentTime = (String)time(NULL);
		if (fromEEPROM.length > 1) {
			writeEEPROM(currentTime);
			if (currentTime.toInt() - fromEEPROM.toInt() >= send_interval) {
				
				initIoTClient();
				delay(200);
				char messagePayload[MESSAGE_MAX_LEN];
				bool temperatureAlert = readMessage(messagePayload);
				sendMessage(iotHubClientHandle, messagePayload, temperatureAlert, "temperature");
				IoTHubClient_LL_DoWork(iotHubClientHandle);
				delay(200);
				ESP.deepSleep(check_interval);
			}
			else {
				char messagePayload[MESSAGE_MAX_LEN];
				bool temperatureAlert = readMessage(messagePayload);
				if (temperatureAlert) {
					initIoTClient();
					delay(200);

				}

			}
		}
		else {
			writeEEPROM(currentTime);
		}
	}
	else {
		ESP.deepSleep(1e6);
	}

	//initMultiWifi();
	//initTime();
	//initSensor();

	
}

void loop()
{
//	currentMillis = millis();
	/*
	if (!messagePending && messageSending && currentMillis - lastSentMillis > send_interval)
	{
		lastSentMillis = currentMillis;
		char messagePayload[MESSAGE_MAX_LEN];
		bool temperatureAlert = readMessage(messagePayload);
		sendMessage(iotHubClientHandle, messagePayload, temperatureAlert, "temperature");
		//delay(interval);
		//ESP.deepSleep(20e6);
	}
	IoTHubClient_LL_DoWork(iotHubClientHandle);
	delay(10);
	*/
}