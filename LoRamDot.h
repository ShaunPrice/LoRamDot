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
// LoRamDot.h

#ifndef _LORAMDOT_h
	#define _LORAMDOT_h
#endif
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

const int MANUAL = 0;									// Manual Network Join Mode

const boolean DISABLED = 0;								// Disabled
const boolean ENABLED = 1;								// Enabled

														// Last Command Status IDs
const int COMMAND_STATUS_ID_OK = 0;						// Command Status was OK.
const int COMMAND_STATUS_ID_TIMED_OUT = 1;				// Command Status was Timed-Out.
const int COMMAND_STATUS_INPUT_OUT_OF_RANGE = 2;		// Command Status was that the Input to the function to call the command was out of range.

														// Wake PINs
const byte WAKE_PIN_DIN = 1;							// Wke PIN is DIN
const byte WAKE_PIN_AD2_DIO2 = 2;						// 
const byte WAKE_PIN_AD3_DIO3 = 3;						// 
const byte WAKE_PIN_AD4_DIO4 = 4;						// 
const byte WAKE_PIN_ASSOCIATE_AD5_DIO5 = 5;				// 
const byte WAKE_PIN_RTS_AD6_DIO6 = 6;					// (Not available with AT&K3)
const byte WAKE_PIN_NCTS_DIO7 = 7;						// (Not available with AT&K3)
const byte WAKE_PIN_NDTR_SLEEPRQ_DI8 = 8;				// (Default)	

														// Serial Speeds
const unsigned int SERIAL_SPEED_1200 = 1200;			// 1200 baud
const unsigned int SERIAL_SPEED_2400 = 2400;			// 2400 baud
const unsigned int SERIAL_SPEED_4800 = 4800;			// 4800 baod
const unsigned int SERIAL_SPEED_9600 = 9600;			// 9600 baud
const unsigned int SERIAL_SPEED_19200 = 19200;			// 19200 baud
const unsigned int SERIAL_SPEED_38400 = 38400;			// 38400 baud
const unsigned int SERIAL_SPEED_57600 = 57600;			// 38400 baud
const unsigned int SERIAL_SPEED_115200 = 115200;		// 115200 baud (Default)
const unsigned int SERIAL_SPEED_230500 = 230500;		// 230500 baud
const unsigned int SERIAL_SPEED_460800 = 460800;		// 460800 baud
const unsigned int SERIAL_SPEED_921600 = 921600;		// 921600 baud

														// US AU Data Rates
const String DATA_RATE_US_AU_D0_11 = "DR0";				// Data Rate for D0 on US and AU devices for 11 bytes payload. 
const String DATA_RATE_US_AU_D1_53 = "DR1";				// Data Rate for D0 on US and AU devices for 11 bytes payload.
const String DATA_RATE_US_AU_D2_129 = "DR2";			// Data Rate for D0 on US and AU devices for 11 bytes payload.
const String DATA_RATE_US_AU_D3_242 = "DR3";			// Data Rate for D0 on US and AU devices for 11 bytes payload.
const String DATA_RATE_US_AU_D4_242 = "DR4";			// Data Rate for D0 on US and AU devices for 11 bytes payload.

														// EU Data Rates
const String DATA_RATE_EU_D0_51 = "DR0";				// Data Rate for D0 on EU devices for 51 bytes payload.
const String DATA_RATE_EU_D1_51 = "DR1";				// Data Rate for D1 on EU devices for 51 bytes payload.
const String DATA_RATE_EU_D2_51 = "DR2";				// Data Rate for D2 on EU devices for 51 bytes payload.
const String DATA_RATE_EU_D3_115 = "DR3";				// Data Rate for D3 on EU devices for 115 bytes payload.
const String DATA_RATE_EU_D4_242 = "DR4";				// Data Rate for D4 on EU devices for 242 bytes payload.
const String DATA_RATE_EU_D5_242 = "DR5";				// Data Rate for D5 on EU devices for 242 bytes payload.
const String DATA_RATE_EU_D6_242 = "DR6";				// Data Rate for D6 on EU devices for 242 bytes payload.
const String DATA_RATE_EU_D7_50 = "DR7";				// Data Rate for D7 on EU devices for 50 bytes payload.
														
														// Debug Levels
const byte DEBUG_LOG_LEVEL_OFF = 0;						// Off – No debug messages(Default)
const byte DEBUG_LOG_LEVEL_FATAL = 1;					// Output FATAL debug messages.
const byte DEBUG_LOG_LEVEL_ERROR = 2;					// Outputs ERROR and FATAL debug messages
const byte DEBUG_LOG_LEVEL_WARNING = 3;					// Outputs WARNING and all lower level debug messages
const byte DEBUG_LOG_LEVEL_INFO = 4;					// Outputs INFO and all lower level debug messages
const byte DEBUG_LOG_LEVEL_DEBUG = 5;					// Output DEBUG and all lower level debug messages
const byte DEBUG_LOG_LEVEL_TRACE = 6;					// Output TRACE and all lower level debug messages

														// Join Byte Order
const byte JOIN_BYTE_ORDER_LSB = 0;						// LSB first (Default)
const byte JOIN_BYTE_ORDER_MSB = 1;						// MSB first

														// Network Join Modes
const byte NETWORK_JOIN_MODE_MANUAL = 0;				// Manual configuration
const byte NETWORK_JOIN_MODE_OTA = 1;					// Over The Air (OTA) network join (Default)
const byte NETWORK_JOIN_MODE_AUTO_OTA = 2;				// Auto OTA network join on start up
														// ***CAUTION ***: Setting + NJM = 2 causes the Dot to join immediately.
														// Configure network settings and OTA mode before setting to AUTO_OTA mode.
const byte NETWORK_JOIN_MODE_PEER_TO_PEER = 3;			// Peer-to-peer mode

														// Over-The-Air Activation (OTA)

const byte KEY_TYPE_HEX = 0;							// Network ID or Key parameter is a HEX Key
const byte KEY_TYPE_STRING = 1;							// Network ID or Key parameter is a String Key up to 128 characters

														// Network Join

const boolean NETWORK_NOT_JOINED = false;				// Network is not joined
const boolean NETWORK_JOINED = true;					// Network is joined

														// Device Class
const String DEVICE_CLASS_A = "A";						// LoRaWAN 1.0 Device class A (Bi-directional End Devices)
const String DEVICE_CLASS_B = "B";						// LoRaWAN 1.0 Device class B (Bi-directional end devices with scheduled receive slots)
const String DEVICE_CLASS_C = "C";						// LoRaWAN 1.0 Device class C (Bi-directional end devices with maximal receive slots)

														// Forward Error Redundancy
const byte FORWARD_ERROR_CORRECTION_REDUNDANCY_5_BITS = 1;	// Sends 5 bits to represent 4 bits
const byte FORWARD_ERROR_CORRECTION_REDUNDANCY_6_BITS = 2;	// Sends 6 bits to represent 4 bits
const byte FORWARD_ERROR_CORRECTION_REDUNDANCY_7_BITS = 3;	// Sends 7 bits to represent 4 bits
const byte FORWARD_ERROR_CORRECTION_REDUNDANCY_8_BITS = 4;	// Sends 8 bits to represent 4 bits

														// Data Format
const byte DATA_FORMAT_HEX = 0;							// Hexidecimal data format (e.g. 6174 = at)
const byte DATA_FORMAT_RAW = 1;							// Raw/Unprocessed data

														// Data Mode											// Data Format
const byte DATA_MODE_AT = 0;							// AT command mode (Default)
const byte DATA_MODE_SERIAL = 1;						// Serial data mode

														// Sleep Mode
const byte SLEEP_MODE_DEEP_SLEEP = 0;					// Deep sleep (ST Micro standby mode)
const byte SLEEP_MODE_STOP_MODE = 1;					// Sleep (ST Micro stop mode)

														// Wake Mode
const byte WAKE_MODE_DEEP_SLEEP = 0;					// ST Micro standby mode
const byte WAKE_MODE_STOP_MODE = 1;						// Sleep(ST Micro stop mode)

const String CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="; // Base64 string

class LoRamDot
{
public:
	LoRamDot();
	LoRamDot(Stream &serial);

	void begin(Stream &serial);

	void setTimeout(unsigned long timeout);

	// General AT Commands

	boolean Attention();								// Attention, used to verify the COM channel is working
	String RequestID();									// Request ID returns product and software identification information.
	boolean ResetCPU();									// Resets the CPU, the same way as pressing the reset button. The program is reloaded from flash and begins execution at the main function.Reset takes about 3 seconds.
	boolean EchoMode(boolean mode);						// Enable or disable command mode echo.
	boolean VerbosMode(boolean mode);					// Enable or disable verbose mode. Affects the verbosity of command query responses.
	boolean HardWareFlowControl(boolean mode);			// Enable or disable hardware flow control. Hardware flow control is useful in serial data mode to keep from overflowing the input buffers.
	boolean ResetToFactory();							// Reset to Factory Defaults changes the current settings to the factory defaults, but does not store them.
	boolean SaveConfiguration();						// Writes all configuration settings displayed in AT&V to flash memory.
	boolean WakePin(byte pin);							// Sets the pin that the end device monitors if wake mode is set to interrupt mode.
	boolean SerialSpeed(int speed);						// Sets serial baud rate for interface on header pins 2 and 3. Changes to this setting take effect after a save and reboot of the Dot.
	boolean DebugSerialSpeed(int speed);				// Sets debug serial baud rate for interface on DEBUG header pins 30 and 31. Changes to this setting take effect after a save and reboot of the Dot.power - cycle or reset.
	boolean DebugLogLevel(byte level);					// Sets the debug message logging level. Messages are output on the debug port. Higher settings log more messages.

														// LoRa utility functions

	String LastResponse();								// Returns the last message received.
	boolean LastCommandStatus();						// Returns the status of the last command (true: success, false: failure).
	String LastCommandStatusMessage();					// Returns the status message of the last command.
	int LastCommandStatusId();							// Returns the status ID of the last command (0:OK, 1:TIMED-OUT, 2:INPUT-OUT-OF-RANGE).

														// Network Management Commands

	String DeviceID();									// The device ID is an EUI.The EUI is programmed at the factory.
	String FrequencyBand();								// Use to query the supported frequency band.
	boolean FrequencySubBand(byte sub_band);			// 1-8 (915MHz models only) Configures the frequency sub-band.This enables hybrid mode for private network channel management.
	boolean PublicNetworkMode(byte mode);				// Configures the end device to function on either a public or private LoRa network. (ENABLED or DISABLED)
	boolean JoinByteOrder(byte order);					// Sets the byte order (LSB [Default] or MSB first) in which the device EUI is sent to the gateway in a join request.
	boolean NetworkJoinMode(byte mode);					// Controls how the end device establishes communications with the gateway.
	boolean Join();										// Join network. For US915 and EU868 models +NI, +NK must match gateway settings in order to join. US915 must also match + FSB setting.
	boolean JoinRetries(byte retries);					// This is the maximum number of join attempts that will be made if none are successful. 0: Disable; 1-255: Retries (Default: 2)
	boolean JoinDelay(byte delay);						// Allows the dot to use non-default join receive windows, if required by the network it is attempting to connect to.
														// Initiating a join request opens a receive window to listen for the response.This command allows you to alter the default timing of the window.
														// 1-15 seconds (Defailt: 1)

														// Over-the-Air Activation (OTA)

	boolean NetworkID(byte type, String id);			// Configures network EUI/Name. (App EUI in LoRaMac.)
	boolean NetworkKey(byte type, String key);			// Configures network key/passphrase. (App key in LoRaMac.)
	boolean TTNApplicationEUI(String id);				// Configures The Things Network Application EUI. (Calls NetworkID)
	boolean TTNAppKey(String key);						// Configures The Things Network App Key. (Calls NetworkKey)
	boolean AESEncryption(boolean mode);				// Enables or disables AES encryption of payload data.

														// Manual Activation

	boolean NetworkAddress(String address);				// Sets network address in MANUAL join mode, the server will assign an address in OTA modes.
														// 4 bytes of hex data using a colon (:) to separate each byte from the next byte (e.g. =01:fa:b0:1c)
	boolean NetworkSessionKey(String key);				// Sets network session key in MANUAL join mode, will be automatically set in OTA modes.
														// 16 bytes of hex data using a colon (:) to separate each byte from the next byte. (e.g. 00.11.22.33.44.55.66.77.88.99.aa.bb.cc.dd.ee.ff)
	boolean DataSessionKey(String key);					// Sets data session key in MANUAL join mode, will be automatically set in OTA modes. Used for AES-128 encryption of transferred data.
														// 16 bytes of hex data using a colon (:) to separate each byte from the next byte. (e.g. 00.11.22.33.44.55.66.77.88.99.aa.bb.cc.dd.ee.ff)
	boolean UplinkCounter(unsigned long counter);		// A device using MANUAL join mode a network server may reject uplink packets, if they do not have the correct counter value.
														// This setting is available for an application to manage this session parameter.Otherwise, use AT + SS and AT + RS to save this setting to flash in any join mode.
														// 0-4294967295 (Default is 1)
	boolean DownlinkCounter(unsigned long counter);		// A device using MANUAL join mode, it may reject downlink packets if they do not have the correct counter value.
														// This setting is available for an application to manage this session parameter.Otherwise, use AT + SS and AT + RS to save this setting to flash in any join mode.
														// 0-4294967295 (Default is 1)

														// Network Joining

	boolean NetworkJoinStatus();						// (NETWORK_NOT_JOINED or NETWORK_JOINED) Displays the last known network join state, which helps determine if communication has been lost.
	String Ping();										// Sends a ping to the gateway. The gateway responds with a pong containing RSSI and SNR, which the end device
														// displays.RSSI ranges from - 140dB to –0dB and SNR ranges from - 20dBm to 20dBm
	boolean RequireAcknowledgment(byte attempts);		// The maximum number of times the end device tries to retransmit an unacknowledged packet.
														// Options are from 0 (default) not required or 1 to 8 maximum number of attempts without an acknowledgment.
	String NetworkLinkCheck();							// Performs a network link check. The first number in the response is the dBm level above the demodulation floor
														// (not be confused with the noise floor).This value is from the perspective of the signal sent from the end device
														// and received by the gateway.The second number is the number of gateways in the end device's range.
	boolean LinkCheckCount(byte count);					// Performs periodic connectivity checking. This feature is an alternative to enabling ACK for all packets in order to
														// detect when the network is not available or the session information has been reset on the server. 
														// 0: Disabled (Default); 1 - 255: Number of packets sent before a link check is performed.Link checks are not be sent if ACKs are enabled.

														// Preserving, Saving, and Restoring Sessions

	boolean SaveNetworkSession();						// Saves the network session information (join) over resets allowing for a session restore (AT+RS) without requiring a
														// join.This command should be issued after the Dot has joined.See AT + PS if using auto join mode.
	boolean RestoreNetworkSession();					// Restores the network session information (join) that was saved with the AT+SS command.
	boolean PreserveSession(boolean preserve);			// (false: Off [Default]; true: On) Preserves the network session information over resets when using auto join mode (AT+NJM). If not using auto join mode, use with the save session command(AT + SS).

														// Sending and Receiving Packets
	String TransmitChannel();							// For reference, use the +TXCH command to display channels used with frequency hopping.
	unsigned long TransmitNext();						// Returns the time, in milliseconds, until the next free channel is available to transmit data. The time can range from 0 - 2793000 milliseconds.
	unsigned long TimeOnAir(byte bytes);				// Displays the amount of on air time, in milliseconds, required to transmit the number of bytes specified at the current data rate.
														// bytes: 0-242 The number of bytes used to calculate the time on air.
														// Configuring
	String SettingsAndStatus();							// Displays device settings and status in a tabular format.
	boolean DeviceClass(String deviceClass);			// Sets the device class. The LoRaWAN 1.0 specification defines the three device classes, Class A, B and C. Note : Currently only Class A is supported.
	boolean ApplicationPort(byte applicationPort);		// Sets the port used for application data. Each LoRaWAN packet containing data has an associated port value. 
														// Port 0 is reserved for MAC commands, ports 1 - 223 are available for application use, and port 233 - 255 are reserved for future LoRaWAN use.
	boolean TransmitPower(byte transmitPower);			// Configures the output power of the radio in dBm, before antenna gain. The mac layer will attempt to reach this output level but limit any transmission to the local regulations for the chosen frequency.
														// transmitPower: 0-20 dB. (Default is 11).
	boolean TransmitInverted(boolean inverted);			// Sets TX signal inverted. inverted: false = Not Inverted (default), 1 = Inverted 
														// Note: Transmitted signals are inverted so motes/gateways do not see other mote/gateway packets.
	boolean ReceiveSignalInverted(boolean inverted);	// Sets RX signal inverted. inverted: false = Not Inverted (default), 1 = Inverted 
														// Note: Transmitted signals are inverted so motes/gateways do not see other mote/gateway packets.
	boolean ReceiveDelay(byte delay);					// Allows the dot to use non-default rx windows, if required by the network it is attempting to communicate with.
														// Opens receive window to listen for a response when sending packets with one of the + SEND commands.
														// Note: Setting must match that of network server, in case of OTA join the value sent in Join Accept message overwrites this setting
														// delay: 1-15 seconds (Default)
	boolean ForwardErrorCorrection(byte redundancy);	// Sends redundant data to compensate for unreliable communication with the goal of reducing the need to retransmit data.Increasing redundancy increases time - on - air, LoRaWAN specifies a setting of 1 (4 / 5).
														// redundancy:	
														//		FORWARD_ERROR_CORRECTION_REDUNDANCY_5_BITS = Sends 5 bits to represent 4 bits.
														//		FORWARD_ERROR_CORRECTION_REDUNDANCY_6_BITS = Sends 6 bits to represent 4 bits.
														//		FORWARD_ERROR_CORRECTION_REDUNDANCY_7_BITS = Sends 7 bits to represent 4 bits.
														//		FORWARD_ERROR_CORRECTION_REDUNDANCY_8_BITS = Sends 8 bits to represent 4 bits.
	boolean CyclicalRedundancyCheck(boolean enabled);	// Enable or disable Cyclical Redundancy Check(CRC) for uplink and downlink packets. Must be enabled to be compliant with LoRaWAN.Packets received with a bad CRC are discarded.
														// enabled: false = CRC disabled, true = CRC enabled(Default)
	boolean AdaptiveDataRate(boolean enabled);			// Enable or disable adaptive data rate for your device. For more information on Adpative Data Rate, refer to your device's Developer Guide.
														// enabled: false = ADR disabled (Default), true = ADR enabled
	boolean TXDataRate(String dataRate);				// Sets the current data rate to use, DR0-DR15 can be entered as input in addition to (7-12) or (SF_7-SF_12).
														// dataRate:
														// 7-10 915MHz Models (Default is 9)
														// 7-12 868MHz Models
														// 
														// US Data Rates Max Payload (bytes)
														//		DR0: 11
														//		DR1 : 53
														//		DR2 : 129
														//		DR3 : 242
														//		DR4 : 242
														// EU Data Rates Max Payload(bytes)
														//		DR0 : 51
														//		DR1 : 51
														//		DR2 : 51
														//		DR3 : 115
														//		DR4 : 242
														//		DR5 : 242
														//		DR6 : 242
														//		DR7 : 50
														//
														//	Note: Data rate is directly related to spreading factor.Spreading factor determines the amount of redundant
														//		data spread across the transmission.A higher spreading factor means more redundant data is transmitted,
														//		which results in a longer range but a lower data rate.For more information on spreading factor, refer to the
														//		device's developer guide
	String SessionDataRate();							// Display the current data rate the LoRaMAC layer is using. It can be changed by the network server if ADR is enabled.
	boolean RepeatPacket(byte repeats);					// Repeats each frame as many times as indicated or until downlink from network server is received. This setting
														//		increases redundancy to increase change of packet to be received by the gateway at the expense of increasing
														//		network congestion.When enabled, debug output shows multiple packets being sent.On the Conduit, an MQTT
														//		client can listen to the 'packet_recv' topic to see that duplicate packets are received, but not forwarded to the up topic.
														//	repeats: Number of send attempts. (Default)
														// Sending Packets

	boolean Send(String data);							// Sends supplied data and opens a receive window to receive data from the gateway/network server. If a data
														//		packet is received, it is output following AT + SEND.To configure the receive data format, use the AT + RXO
														//		command.Although parameter1 can be up to 242 bytes, it is limited by the payload size as determined by + TXDR setting
														// data: Up to 242 bytes of data or the maximum payload size based on spreading factor (See AT+TXDR)
	boolean SendBinary(String data);					// Functions as the +SEND command, but sends hexadecimal data.
														// data: String of up to 242 eight bit hexadecimal values. Each value may range from 00 to FF.

	boolean SendBinary(byte data[]);					// Functions as the +SEND command, but sends hexadecimal data.
														// data: String of up to 242 eight bit hexadecimal values. Each value may range from 00 to FF.
														// Receiving Packets

	String ReceiveOnce();								// Displays the last payload received. It does not initiate reception of new data. Use +SEND to initiate receiving data from the network server.
	boolean ReceiveOutput(byte format);					// Formats the receive data output. Data is either processed into hexadecimal data or left unprocessed/raw.
														// Hexadecimal outputs the byte values in the response.Raw / Unprocessed outputs the actual bytes on the serial interface.
														// format: DATA_FORMAT_HEX = 0, DATA_FORMAT_RAW = 1. 
	boolean DataPending();								// Indicates there is at least one packet pending on the gateway for this end device. This indication is communicated
														// to the end device in any packet coming from the server.Each packet contains a data pending bit.
	boolean TransmitWait(boolean wait);					// Enables or disables waiting for RX windows to expire after sending.
														// wait: false (0) = Do not wait. Not recommended. true (1) Wait(Default)

														// Statistics
	boolean ResetStatistics();							// Resets device statistics displayed with the Statistics (AT&S) command.
	String Statistics();								// Displays device statistics including join attempts, join failures, packets sent, packets received and missed acks. Use AT&R to reset / clear the statistics.
	String SignalStrength();							// Displays device statistics including join attempts, join failures, packets sent, packets received and missed acks. Use AT&R to reset / clear the statistics.
	String SignalToNoiseRatio();						// Displays signal to noise ratio for all packets received from the gateway since the last reset. There are four signal to
														//   noise ratio values, which, in order, are: last packet SNR, minimum SNR, maximum SNR and average SNR.Values range from - 20dBm to 20dBm.

														// Serial Data Mode

	boolean SerialDataMode();							// Reads serial data, sends packets, and then sleeps using wake settings.The escape sequence is++ + .
														//		- When++ + is received to escape serial data mode all buffer data will be discarded.
														//		- CTS is handled by the serial driver and is relative to its buffer size.When flow control is enabled, see AT&K.
														//		- mDot firmware serial buffer size is 512 bytes.

	boolean StartupMode(byte dataMode);					// Configures which operation mode the end device powers up in, either AT command mode or serial data mode.
														//		- AT Command mode : The end device powers up or resets in command mode.AT commands are used to send and receive data.
														//		- Serial data mode : Allows the end device to send and receive data without entering AT commands.Data
														//			is sent and received based on wake command settings.This mode requires network join mode to be set
														//			for either auto join or peer - to - peer mode. (AT + NJM = 2 or 3).
														// dataMode: ATA_MODE_AT = 0 (AT command mode (Default)) or DATA_MODE_SERIAL = 1 (Serial data mode)
	boolean SerialDataClearOnError(boolean discardBuffer);	// Sets the device to either keep or discard data in the serial buffer when an error occurs.
															// discardBuffer: false(0) Data that cannot be sent remains in the serial buffer for later transmission true (1) Data that cannot be sent is discarded

															// Power Management

	boolean SleepMode(byte sleepMode);					// Puts the end device in sleep mode. The end device wakes on interrupt or interval based on AT+WM setting. Once awakened, use AT + SLEEP again to return to sleep mode.
														// mode: (0) Deep sleep (ST Micro standby mode) or (1) Sleep (ST Micro stop mode)
	boolean WakeMode(byte wakeMode);					// 0 Deep sleep (ST Micro standby mode), 1 Sleep(ST Micro stop mode)
	boolean WakeInterval(unsigned long interval);		// When using wake mode set to interval, use this command to configure the number of seconds the end device
														//	sleeps when in sleep mode.Upon waking, it waits + WD amount of time for an initial character then + WTO amount of time for each additional character.
														// interval: 2-2147483647 seconds (Default is 2)
	boolean WakeDelay(unsigned long delay);				// Configures the maximum amount of time to wait for data when the device wakes up from sleep mode. If this timer
														//	expires, the device goes back to sleep.If the device received at least one character before this timer expires, the
														//	device continues to read input until either the payload is reached or the + WTO timer expires at which time it sends
														//	the collected data and goes to sleep.
														// delay: 2-2147483647 seconds (Default is 100)
	boolean WakeTimeout(unsigned long timeout);			// Configures the amount of time that the device waits for subsequent characters following the first character
														//	received upon waking.Once this timer expires, the collected data is sent and the end device goes back to sleep.
														// timeout: 0-65000 milliseconds (Default is 20)
	boolean AntennaGain(int gain);						// 	Allows a non-default antenna to be used while still adhering to transmit power regulations.
														// gain: -128 to 127 (Default is 3)			

	boolean SendCommand(String command);				// Send a command that instructs the mDot to send the data and wait for the "OK" response.
	boolean SendCommand(String command, String *response);	// Send a command that instructs the mDot to send the command and wait for the respnse string.
	boolean ReceiveResponse(String *response, unsigned long timeout); // Read the the serial response.
																	  // The timeout is set seperately to enable the use of -1 in SendCommand
protected:

private:
	Stream *_Serial;
	unsigned long _timeout = 15000;						// Timeout for the serial stream in milliseconds. 0 disables the timeout (no timeout).
	String _lastResponse = "";							// Last response received. Partial response if timed out.
	boolean _lastCommandStatus = false;					// The response status of the last command. Used by code to determin if the last response was successful especially after receiving an empty string.
	String _lastCommandStatusMessage = "";				// Message to give context as to why the command failed
	int _lastCommandStatusId = 0;						// Status ID of the last command (0:OK, 1:TIMED-OUT, 3:INPUT-OUT-OF-RANGE).
	
	// Value to receive the Serial incoming data 
	String _inputString = "";							// String to hold incoming Serial data
	boolean _stringComplete = false;					// True when the stream has received a full line of data
};
