#pragma once
// Physical device information for board and sensor
#define DEVICE_ID "TempSensor"
#define DHT_TYPE DHT11

// Pin layout configuration
#define LED_PIN 5
#define DHT_PIN 13
#define BUTTON_PIN 12

#define TEMPERATURE_ALERT 25

// Interval time(s) for sending message to IoT Hub
#define SEND_INTERVAL 60

// Interval time(s) for checking temperatur for alerting
#define CHECK_INTERVAL 10

// If don't have a physical DHT sensor, can send simulated data to IoT hub
#define SIMULATED_DATA false

// EEPROM address configuration
#define EEPROM_SIZE 512

// SSID and SSID password's length should < 32 bytes
// http://serverfault.com/a/45509
#define CONNECTION_STRING_LEN 256

#define MESSAGE_MAX_LEN 256

void initSerial()
{
	// Start serial and initialize stdout
	Serial.begin(115200);
	Serial.setDebugOutput(true);
	Serial.println("Serial successfully initiated.");
}