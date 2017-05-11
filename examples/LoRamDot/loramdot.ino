/*
Copyright (c) 2016 Shaun Price.  All right reserved.
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
////////////////////////////////////////////////////////////////
// IMPORTANT NOTE: This code only works on Arduino board that support multiple hardware serial port such as the Leonardo, 101 and Mega
//                 To use this code with other boards you will need to use the SoftSerial Libraries (https://www.arduino.cc/en/Reference/softwareSerial)
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
#define VERSION "0.2.0002"	// Code version (major.minor.build) 
////////////////////////////////////////////////////////////////
//#define DEBUG 1           // Uncomment for debugging.
							// NOTE: The code will not work with debugging on while not connected to a computer
////////////////////////////////////////////////////////////////

#ifdef DEBUG
#define DEBUG_PRINT(x)  Serial.print(x)
#define DEBUG_PRINTLN(x)  Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

#include "LoRamDot.h"

// These settings are for connecting to teh Australian TTN Network using an Australian configured 915MHz mDot
#define DATA_RATE DATA_RATE_US_AU_D0_11					// DR0 to DR4 for AU - Data Rates Max Payload (bytes):DR0 : 11; DR1 : 53; DR2 : 129; DR3 : 242; DR4 : 242
#define PUBLIC_NETWORK (ENABLED)						// Public Network
#define NETWORK_JOIN_MODE (NETWORK_JOIN_MODE_OTA)		// Automatically join
#define FREQUENCY_SUBBAND (2)							// Subband used in Australian The Things Network gateways
#define NETWORK_ACKKNOWLEDGMENT (0)						// Don't require network acknowledgement

// Obtain these settings after registering your device (https://www.thethingsnetwork.org/docs/devices/registration.html)
#define TTN_APP_EUI ("0000000000000000")				// Application EUI from the Things Network
#define TTN_APP_KEY ("00000000000000000000000000000000")//App Key  from the Things Network

#define loraSerial (Serial1)							// Assign the loraSerial a name

const long interval = 1000;								// interval at which to blink (milliseconds)
unsigned long previousMillis = 0;						// will store last time LED was updated

LoRamDot loRaWAN;										// Initialise the LoRamDot library

// Setup the Arduino 101
void setup()
{
#ifdef DEBUG
	while (!Serial);
	Serial.begin(115200); // USB Port
#endif

	while (!loraSerial);								// Wait for the serial comms to begin

	loraSerial.begin(115200);							// Lora mdot comms
	loRaWAN.begin(loraSerial);							// Initialise the LoRaWAN object wrapper

	// Set up the data rate to the smallest packet/longest distance
	loRaWAN.TXDataRate(DATA_RATE);
	DEBUG_PRINTLN("DATA_RATE: " + loRaWAN.LastResponse());

	// Set it to talk to public networks
	loRaWAN.PublicNetworkMode(ENABLED);
	DEBUG_PRINTLN("PUB_NET: " + loRaWAN.LastResponse());

	// Set up the mode to join
	loRaWAN.NetworkJoinMode(NETWORK_JOIN_MODE);
	DEBUG_PRINTLN("NET_JOIN_MODE: " + loRaWAN.LastResponse());

	// Set up the frequency subband to use
	loRaWAN.FrequencySubBand(FREQUENCY_SUBBAND);
	DEBUG_PRINTLN("FREQ_SB: " + loRaWAN.LastResponse());

	// Require network acknowledgment?
	loRaWAN.RequireAcknowledgment(NETWORK_ACKKNOWLEDGMENT);
	DEBUG_PRINTLN("ACK: " + loRaWAN.LastResponse());

	// The Things Network Application EUI
	loRaWAN.TTNApplicationEUI(TTN_APP_EUI);
	DEBUG_PRINTLN("TTN_APP_EUI: " + loRaWAN.LastResponse());

	// The Things Network APP Key
	loRaWAN.TTNAppKey(TTN_APP_KEY);
	DEBUG_PRINTLN("TTN_APP_KEY: " + loRaWAN.LastResponse());

	// Join the nework
	loRaWAN.Join();
	DEBUG_PRINTLN("JOIN: " + loRaWAN.LastResponse());

#ifdef DEBUG
	// Print the setting if in debug mode
	loRaWAN.SettingsAndStatus();
	DEBUG_PRINTLN("Settings: \r\n" + loRaWAN.LastResponse());
#endif // DEBUG
}

void loop()
{
	// Delay 5 mins.
	// Note: There are limits on the The Things Network to how much data can be sent and for how long.
	unsigned long currentMillis = millis();

	if (currentMillis - previousMillis >= 300000)
	{
		previousMillis = currentMillis;

		String myMessage = "Test";

		DEBUG_PRINTLN("SENDING MESSAGE: " + String(myMessage));
		loRaWAN.Send(myMessage);
		
		// NOTE: If there is no response it is likely your TTN settings are not correct.
		DEBUG_PRINTLN("RESPONSE: " + loRaWAN.LastResponse());
	}
}

