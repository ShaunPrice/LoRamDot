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

#include "LoRamDot.h"

// LoRa Constructor
// Wrapper library for the Multitech mDot LoRaWan module with version 2.0.x firmware.
LoRamDot::LoRamDot()
{

}

// LoRa Constructor with stream initialise the LoRa serial reference
LoRamDot::LoRamDot(Stream &serial) : _Serial(&serial) // initialise references before body
{
	_Serial = &serial;
}

// LoRa begin function initialises the LoRa serial reference
void LoRamDot::begin(Stream &serial)
{
	_Serial = &serial;
}

// Set Timeout on Serial Stream.
// 0 disables the timeout (no timeout - WARNING: May loop forever).
// -1 disables the timeout and the response is not retrieved. 
void LoRamDot::setTimeout(unsigned long timeout)
{
	_timeout = timeout;
	_Serial->setTimeout(_timeout);
}

// Private Methods //////////////////////////////////////////////////////////////

// Protected Methods ////////////////////////////////////////////////////////////

// Send a command that instructs the mDot to send the data and wait for the "OK" response.
boolean LoRamDot::SendCommand(String command)
{
	return SendCommand(command, & _lastResponse);
}

// Send a command that instructs the mDot to send the command and wait for the respnse string.
// Returns true if response received otherwise returns false.
boolean LoRamDot::SendCommand(String command, String *response)
{
	// Clear the buffers and last response
	_Serial->flush();

	// Re-initialise last response information
	_lastCommandStatus = false;
	_lastCommandStatusId = 0;
	_lastCommandStatusMessage = "";
	_lastResponse = "";

	// Send the AT command
	_Serial->println(command);

	// If timeout is >= 0 get the response
	// if < 0 return imediately and get the response using ReceiveResponse(*response)
	if (_timeout >= 0)
		return ReceiveResponse(response, _timeout);
	else
		return true;
}

// Read the the serial response.
// The timeout is set seperately to enable the use of -1 in SendCommand
boolean LoRamDot::ReceiveResponse(String *response, unsigned long timeout)
{
	// Timeout time
	unsigned long l_timeout = millis() + timeout;

	// Get the LoRaWAN data
	// If timeout = 0 there is no timeout (may loop forever)
	while (millis() < l_timeout || timeout == 0)
	{
		if (_Serial->available())
		{
			_lastResponse += (char)_Serial->read();
			
			if (_lastResponse.endsWith("OK\r\n"))
			{
				_lastResponse.trim();
				*response = _lastResponse;

				_lastCommandStatusMessage = "OK";
				_lastCommandStatusId = COMMAND_STATUS_ID_OK;

				return true;
			}
		}
	}

	// No data available so return an empty string
	*response = "";
	_lastCommandStatusMessage = "TIMED-OUT";
	_lastCommandStatusId = COMMAND_STATUS_ID_TIMED_OUT;

	return false;
}

// Public Methods //////////////////////////////////////////////////////////////

// Returns the status of the last command (true: success, false: failure).
boolean LoRamDot::LastCommandStatus()
{
	return _lastCommandStatus;
}

// Returns the status message of the last command.
String LoRamDot::LastCommandStatusMessage()
{
	return _lastCommandStatusMessage;
}

// Returns the status message ID of the last command (0:OK, 1:TIMED-OUT, 2:INPUT-OUT-OF-RANGE).
int LoRamDot::LastCommandStatusId()
{
	return _lastCommandStatusId;
}

// Returns the last message received.
String LoRamDot::LastResponse()
{
	return _lastResponse;
}

// Attention, used to verify the COM channel is working
boolean LoRamDot::Attention()
{
	return SendCommand("AT");
}

// Request ID returns product and software identification information.
String LoRamDot::RequestID()
{
	String response = "";

	if (SendCommand("ATI", &response))
		return response;
	else
		return "";
}

// Resets the CPU, the same way as pressing the reset button. The program is reloaded from flash and begins execution at the main function.Reset takes about 3 seconds.
boolean LoRamDot::ResetCPU()
{
	return SendCommand("ATZ");
}

// Enable or disable command mode echo.
boolean LoRamDot::EchoMode(boolean mode)
{
	if (mode == 0 || mode == 1)
		return SendCommand("ATE=" + (mode) ? "1" : "0");
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Enable or disable verbose mode. Affects the verbosity of command query responses.
boolean LoRamDot::VerbosMode(boolean mode)
{
	if (mode == 0 || mode == 1)
		return SendCommand("ATV=" + (mode) ? "1" : "0");
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Enable or disable hardware flow control. Hardware flow control is useful in serial data mode to keep from overflowing the input buffers.
boolean LoRamDot::HardWareFlowControl(boolean mode)
{
	if (mode == 3 || mode == 0)
		return SendCommand("AT&K=" + (mode) ? "3" : "0");
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Reset to Factory Defaults changes the current settings to the factory defaults, but does not store them.
boolean LoRamDot::ResetToFactory()
{
	return SendCommand("AT&F");
}

// Writes all configuration settings displayed in AT&V to flash memory.
boolean LoRamDot::SaveConfiguration()
{
	return SendCommand("AT&W");
}

// Sets the pin that the end device monitors if wake mode is set to interrupt mode.
boolean LoRamDot::WakePin(byte pin)
{
	if (pin >= 1 && pin <= 8)
		return SendCommand("AT+WP=" + String(pin));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Sets serial baud rate for interface on header pins 2 and 3. Changes to this setting take effect after a save and reboot of the Dot.
boolean LoRamDot::SerialSpeed(int speed)
{
	// Check if the level is within the valid range
	if ((speed == 1200) | (speed == 2400) | (speed == 4800) | (speed == 9600) | (speed == 19200) | (speed == 38400) | (speed == 57600) | (speed == 115200) | (speed == 230500) | (speed == 460800) | (speed == 921600))
		return SendCommand("AT+IPR=" + String(speed));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Sets debug serial baud rate for interface on DEBUG header pins 30 and 31. Changes to this setting take effect after a save and reboot of the Dot.power - cycle or reset.
boolean LoRamDot::DebugSerialSpeed(int speed)
{
	// Check if the level is within the valid range
	if (speed == 2400 | speed == 4800 | speed == 9600 | speed == 19200 | speed == 38400 | speed == 57600 | speed == 115200 | speed == 230500 | speed == 460800 | speed == 921600)
		return SendCommand("AT+DIPR=" + String(speed));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Sets the debug message logging level. Messages are output on the debug port. Higher settings log more messages.
boolean LoRamDot::DebugLogLevel(byte level)
{
	// Check if the level is within the valid range
	if (level >= 0 && level <= 6)
		return SendCommand("AT+LOG=" + String(level));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

/////////////////////////////////////////////
// Network Management Commands
/////////////////////////////////////////////

// The device ID is an EUI.The EUI is programmed at the factory.
String LoRamDot::DeviceID()
{
	String response = "";

	if (SendCommand("AT+DI", &response))
		return response;
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return "";
	}
}

// Use to query the supported frequency band.
String LoRamDot::FrequencyBand()
{
	String response = "";

	if (SendCommand("AT+FREQ", &response))
		return response;
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return "";
	}
}

// 1-8 (915MHz models only) Configures the frequency sub-band.This enables hybrid mode for private network channel management.
boolean LoRamDot::FrequencySubBand(byte sub_band)
{
	// Check if the sub_band is within the valid range
	if (sub_band >= 1 && sub_band <= 8)
		return SendCommand("AT+FSB=" + String(sub_band));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Configures the end device to function on either a public or private LoRa network. (ENABLED or DISABLED)
boolean LoRamDot::PublicNetworkMode(byte mode)
{
	// Check if the mode is within the valid range
	if (mode == 0 || mode == 1)
	{
		return SendCommand("AT+PN=" + String(mode));
	}
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Sets the byte order (LSB [Default] or MSB first) in which the device EUI is sent to the gateway in a join request.
boolean LoRamDot::JoinByteOrder(byte order)
{
	// Check if the order is within the valid range
	if (order == 0 || order == 1)
		return SendCommand("AT+JBO=" + String(order));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Controls how the end device establishes communications with the gateway.
boolean LoRamDot::NetworkJoinMode(byte mode)
{
	// Check if the mode is within the valid range
	if (mode >= 0 && mode <= 3)
		return SendCommand("AT+NJM=" + String(mode));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Join network. For US915 and EU868 models +NI, +NK must match gateway settings in order to join. US915 must also match + FSB setting.
boolean LoRamDot::Join()
{
	return SendCommand("AT+JOIN");
}

// This is the maximum number of join attempts that will be made if none are successful. 0: Disable; 1-255: Retries (Default: 2)
boolean LoRamDot::JoinRetries(byte retries)
{
	// Check if the retries is within the valid range
	if (retries >= 0 && retries <= 255)
		return SendCommand("AT+JR=" + String(retries));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Allows the dot to use non-default join receive windows, if required by the network it is attempting to connect to.
// Initiating a join request opens a receive window to listen for the response.This command allows you to alter the default timing of the window.
// 1-15 seconds (Defailt: 1)
boolean LoRamDot::JoinDelay(byte delay)
{
	// Check if the delay is within the valid range
	if (delay >= 1 && delay <= 15)
		return SendCommand("AT+JR=" + String(delay));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

/////////////////////////////////////////////
// Over-the-Air Activation (OTA)
/////////////////////////////////////////////

// Configures network EUI/Name. (App EUI in LoRaMac.)
// Parameter1
//		0 Second parameter is a hex key.
//		1 Second parameter is a string up to 128 characters long.
// Parameter2
//		If Parameter1 is set to 0, 8 bytes of hex data and may be using a colon(:) to separate each byte from the next byte. (e.g. 001A2F3344556677 or 00:1A:2F:33:44:55:66:77)
//		If Parameter1 is set to 1, a character string up to 128 characters. 
boolean LoRamDot::NetworkID(byte type, String id)
{
	// Check if the type and id is within the valid range
	if ((type == 0 && ((id.length() == 16) || (id.length() == 23))) || (type == 1 && id.length() <= 128))
		return SendCommand("AT+NI=" + String(type) + "," + id);
	
	_lastCommandStatus = false;
	_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
	_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

	return false;
}

// Configures network key/passphrase. (App key in LoRaMac.)
// Parameter1
//		0 Second parameter is a hex key.
//		1 Second parameter is a string up to 128 characters long.
// Parameter2
//		If Parameter1 is set to 0, 16 bytes of hex data and may be using a colon(:) to separate each byte from the next byte. (e.g. 001A2F33445566778899102030405060 or 00:1A:2F:33:44:55:66:77:88:99:10:20:30:40:50:60)
//		If Parameter1 is set to 1, a character string up to 128 characters. 
boolean LoRamDot::NetworkKey(byte type, String key)
{
	// Check if the type and key is within the valid range
	if ((type == 0 && ((key.length() == 32) || (key.length() == 47))) || (type == 1 && key.length() <= 128))
		return SendCommand("AT+NK=" + String(type) + "," + key);
	
	_lastCommandStatus = false;
	_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
	_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

	return false;
}

// Configures The Things Network Application EUI. (Calls LoRamDot::NetworkID)
// Parameter1
//		8 bytes of hex data. (e.g. 0011223344556677)
boolean LoRamDot::TTNApplicationEUI(String id)
{
	return LoRamDot::NetworkID(0, id);
}

// Configures The Things Network App Key. (Calls LoRamDot::NetworkKey)
// Parameter1
//		16 bytes of hex data. (e.g. 001A2F3344556677DABE98F91652A8E1)
boolean LoRamDot::TTNAppKey(String key)
{
	return LoRamDot::NetworkKey(0, key);
}

// Enables or disables AES encryption of payload data.
boolean LoRamDot::AESEncryption(boolean mode)
{
	return SendCommand("AT+ENC=" + (mode) ? "1" : "0");
}

/////////////////////////////////////////////
// Manual Activation
/////////////////////////////////////////////

// Sets network address in MANUAL join mode, the server will assign an address in OTA modes.
// address is 4 bytes of hex data (e.g. 01FAB01C)
boolean LoRamDot::NetworkAddress(String address)
{
	// Check if the address is within the valid range
	if (address.length() == 11)
		return SendCommand("AT+NA=" + address);
	
	_lastCommandStatus = false;
	_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
	_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

	return false;
}

// Sets network session key in MANUAL join mode, will be automatically set in OTA modes.
// 16 bytes of hex data and may be using a colon(:) to separate each byte from the next byte. (e.g. 001A2F33445566778899102030405060 or 00:1A:2F:33:44:55:66:77:88:99:10:20:30:40:50:60)
boolean LoRamDot::NetworkSessionKey(String key)
{
	// Check if the key is within the valid range
	if ((key.length() == 32) || (key.length() == 47))
		return SendCommand("AT+NSK=" + key);
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Sets data session key in MANUAL join mode, will be automatically set in OTA modes. Used for AES-128 encryption of transferred data.
// 16 bytes of hex data and may be using a colon(:) to separate each byte from the next byte. (e.g. 001A2F33445566778899102030405060 or 00:1A:2F:33:44:55:66:77:88:99:10:20:30:40:50:60)
boolean LoRamDot::DataSessionKey(String key)
{
	// Check if the key is within the valid range
	if (key.length() == 32 || key.length() == 47)
		return SendCommand("AT+DSK=" + key);

	_lastCommandStatus = false;
	_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
	_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

	return false;
}

// A device using MANUAL join mode a network server may reject uplink packets, if they do not have the correct counter value.
// This setting is available for an application to manage this session parameter.Otherwise, use AT + SS and AT + RS to save this setting to flash in any join mode.
// 0-4294967295 (Default is 1)
boolean LoRamDot::UplinkCounter(unsigned long counter)
{
	return SendCommand("AT+ULC=" + counter);
}

// A device using MANUAL join mode, it may reject downlink packets if they do not have the correct counter value.
// This setting is available for an application to manage this session parameter.Otherwise, use AT + SS and AT + RS to save this setting to flash in any join mode.
// 0-4294967295 (Default is 1)
boolean LoRamDot::DownlinkCounter(unsigned long counter)
{
	return SendCommand("AT+DLC=" + counter);
}

/////////////////////////////////////////////
// Network Joining
/////////////////////////////////////////////

// Displays the last known network join state, which helps determine if communication has been lost.
// Retruns NETWORK_JOINED: true or NETWORK_NOT_JOINED: false) 
boolean LoRamDot::NetworkJoinStatus()
{
	String l_joinStatus = "";

	if (SendCommand("AT+NJS", &l_joinStatus))
		return (l_joinStatus == "1") ? true : false;
	else
		return false;

}

// Sends a ping to the gateway. The gateway responds with a pong containing RSSI and SNR, which the end device
// displays.RSSI ranges from - 140dB to –0dB and SNR ranges from - 20dBm to 20dBm
String LoRamDot::Ping()
{
	String l_pingStatus = "";

	if (SendCommand("AT+PING", &l_pingStatus))
		return l_pingStatus;
	else
		return "";
}

// The maximum number of times the end device tries to retransmit an unacknowledged packet.
// Options are from 0 (default) not required or 1 to 8 maximum number of attempts without an acknowledgment.
boolean LoRamDot::RequireAcknowledgment(byte attempts)
{
	// Check if the mode is within the valid range
	if (attempts >= 0 && attempts <= 8)
		return SendCommand("AT+ACK=" + String(attempts));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Performs a network link check. The first number in the response is the dBm level above the demodulation floor
// (not be confused with the noise floor).This value is from the perspective of the signal sent from the end device
// and received by the gateway.The second number is the number of gateways in the end device's range.
String LoRamDot::NetworkLinkCheck()
{
	String l_nlcStatus = "";

	if (SendCommand("AT+PING", &l_nlcStatus))
		return l_nlcStatus;
	else
		return "";
}

// Performs periodic connectivity checking. This feature is an alternative to enabling ACK for all packets in order to
// detect when the network is not available or the session information has been reset on the server. 
// 0: Disabled (Default); 1 - 255: Number of packets sent before a link check is performed.Link checks are not be sent if ACKs are enabled.
boolean LoRamDot::LinkCheckCount(byte count)
{
	// Check if the mode is within the valid range
	if (count >= 0 && count <= 255)
		return SendCommand("AT+LCC=" + String(count));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}


/////////////////////////////////////////////
// Preserving, Saving, and Restoring Sessions
/////////////////////////////////////////////

// Saves the network session information (join) over resets allowing for a session restore (AT+RS) without requiring a
// join.This command should be issued after the Dot has joined.See AT + PS if using auto join mode.
boolean LoRamDot::SaveNetworkSession()
{
	return SendCommand("AT+SS");
}

// Restores the network session information (join) that was saved with the AT+SS command.
boolean LoRamDot::RestoreNetworkSession()
{
	return SendCommand("AT+RS");
}

// (false: Off [Default]; true: On) Preserves the network session information over resets when using auto join mode (AT+NJM). If not using auto join mode, use with the save session command(AT + SS).
boolean LoRamDot::PreserveSession(boolean preserve)
{
	return SendCommand("AT+PS=" + (preserve) ? "1" : "0");
}

/////////////////////////////////////////////
// Sending and Receiving Packets
/////////////////////////////////////////////

// For reference, use the +TXCH command to display channels used with frequency hopping.
String LoRamDot::TransmitChannel()
{
	String l_transmitChannel = "";

	if (SendCommand("AT+TXCH", &l_transmitChannel))
		return l_transmitChannel;
	else
		return "";
}

// Returns the time, in milliseconds, until the next free channel is available to transmit data. The time can range from 0 - 2793000 milliseconds.
unsigned long LoRamDot::TransmitNext()
{
	String l_transmitNext = ""; // Millisecons (text)

	if (SendCommand("AT+TXN", &l_transmitNext))
	{
		l_transmitNext.trim();

		unsigned long result = 0;

		for (unsigned int i = 0; i < l_transmitNext.length(); i++)
		{
			char c = l_transmitNext.charAt(i);
			if (c < '0' || c > '9') break;
			result *= 10;
			result += (c - '0');
		}

		return result;
	}
	else
		return 0;
}

// Displays the amount of on air time, in milliseconds, required to transmit the number of bytes specified at the current data rate.
// bytes: 0-242 The number of bytes used to calculate the time on air.
unsigned long LoRamDot::TimeOnAir(byte bytes)
{
	// Check if the mode is within the valid range
	if (bytes >= 0 && bytes <= 242)
	{
		String l_onAirTime = ""; // Millisecons (text)

		if (SendCommand("AT+TXN", &l_onAirTime))
		{
			l_onAirTime.trim();

			if (l_onAirTime.equalsIgnoreCase("ERROR"))
			{
				unsigned long result = 0;

				for (unsigned int i = 0; i < l_onAirTime.length(); i++)
				{
					char c = l_onAirTime.charAt(i);
					if (c < '0' || c > '9') break;
					result *= 10;
					result += (c - '0');
				}

				return result;
			}
			else
			{
				_lastCommandStatus = false;
				_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
				_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

				return 0;
			}

		}
	}
	
	_lastCommandStatus = false;
	_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
	_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

	return 0;
}

/////////////////////////////////////////////
// Configuring
/////////////////////////////////////////////

// Displays device settings and status in a tabular format.
String LoRamDot::SettingsAndStatus()
{
	String l_settingsStatus = "";

	if (SendCommand("AT&V", &l_settingsStatus))
		return l_settingsStatus;
	else
		return "";
}

// Sets the device class. The LoRaWAN 1.0 specification defines the three device classes, Class A, B and C. Note : Currently only Class A is supported.
boolean LoRamDot::DeviceClass(String deviceClass)
{
	// Check if the mode is within the valid range
	if (deviceClass == DEVICE_CLASS_A || deviceClass == DEVICE_CLASS_B || deviceClass == DEVICE_CLASS_C)
		return SendCommand("AT+DC=" + deviceClass);
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Sets the port used for application data. Each LoRaWAN packet containing data has an associated port value. 
// Port 0 is reserved for MAC commands, ports 1 - 223 are available for application use, and port 233 - 255 are reserved for future LoRaWAN use.
boolean LoRamDot::ApplicationPort(byte applicationPort)
{
	// Check if the mode is within the valid range
	if (applicationPort >= 1 && applicationPort <= 223)
		return SendCommand("AT+AP=" + String(applicationPort));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Configures the output power of the radio in dBm, before antenna gain. The mac layer will attempt to reach this output level but limit any transmission to the local regulations for the chosen frequency.
// transmitPower: 0-20 dB. (Default is 11).
boolean LoRamDot::TransmitPower(byte transmitPower)
{
	// Check if the mode is within the valid range
	if (transmitPower >= 0 && transmitPower <= 20)
		return SendCommand("AT=TXP" + String(transmitPower));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Sets TX signal inverted. inverted: false = Not Inverted (default), 1 = Inverted 
// Note: Transmitted signals are inverted so motes/gateways do not see other mote/gateway packets.
boolean LoRamDot::TransmitInverted(boolean inverted)
{
	return SendCommand("AT+TXI=" + (inverted) ? "1" : "0");
}

// Sets RX signal inverted. inverted: false = Not Inverted (default), 1 = Inverted 
// Note: Transmitted signals are inverted so motes/gateways do not see other mote/gateway packets.
boolean LoRamDot::ReceiveSignalInverted(boolean inverted)
{
	return SendCommand("AT+RXI=" + (inverted) ? "1" : "0");
}

// Allows the dot to use non-default rx windows, if required by the network it is attempting to communicate with.
// Opens receive window to listen for a response when sending packets with one of the + SEND commands.
// Note: Setting must match that of network server, in case of OTA join the value sent in Join Accept message overwrites this setting
// delay: 1-15 seconds (Default)
boolean LoRamDot::ReceiveDelay(byte delay)
{
	// Check if the mode is within the valid range
	if (delay >= 1 && delay <= 15)
		return SendCommand("AT=TXP" + String(delay));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Sends redundant data to compensate for unreliable communication with the goal of reducing the need to retransmit data.Increasing redundancy increases time - on - air, LoRaWAN specifies a setting of 1 (4 / 5).
// redundancy:	
//		FORWARD_ERROR_CORRECTION_REDUNDANCY_5_BITS = Sends 5 bits to represent 4 bits.
//		FORWARD_ERROR_CORRECTION_REDUNDANCY_6_BITS = Sends 6 bits to represent 4 bits.
//		FORWARD_ERROR_CORRECTION_REDUNDANCY_7_BITS = Sends 7 bits to represent 4 bits.
//		FORWARD_ERROR_CORRECTION_REDUNDANCY_8_BITS = Sends 8 bits to represent 4 bits.
boolean LoRamDot::ForwardErrorCorrection(byte redundancy)
{
	// Check if the mode is within the valid range
	if (redundancy == FORWARD_ERROR_CORRECTION_REDUNDANCY_5_BITS
		|| redundancy == FORWARD_ERROR_CORRECTION_REDUNDANCY_6_BITS
		|| redundancy == FORWARD_ERROR_CORRECTION_REDUNDANCY_7_BITS
		|| redundancy == FORWARD_ERROR_CORRECTION_REDUNDANCY_8_BITS)
		return SendCommand("AT=FEC" + String(redundancy));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Enable or disable Cyclical Redundancy Check(CRC) for uplink and downlink packets. Must be enabled to be compliant with LoRaWAN.Packets received with a bad CRC are discarded.
// enabled: false = CRC disabled, true = CRC enabled(Default)
boolean LoRamDot::CyclicalRedundancyCheck(boolean enabled)
{
	return SendCommand("AT+CRC=" + (enabled) ? "1" : "0");
}

// Enable or disable adaptive data rate for your device. For more information on Adpative Data Rate, refer to your device's Developer Guide.
// enabled: false = ADR disabled (Default), true = ADR enabled
boolean LoRamDot::AdaptiveDataRate(boolean enabled)
{
	return SendCommand("AT+ADR=" + (enabled) ? "1" : "0");
}

// Sets the current data rate to use, DR0-DR15 can be entered as input in addition to (7-12) or (SF_7-SF_12).
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
boolean LoRamDot::TXDataRate(String dataRate)
{
	// Check if the mode is within the valid range
	if ((dataRate.startsWith("DR") && isDigit(dataRate.charAt(2)))
		|| (dataRate.startsWith("SF_") && isDigit(dataRate.substring(3).charAt(3)))
		|| isDigit(dataRate.charAt(0)))
		return SendCommand("AT+TXDR=" + dataRate);
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Display the current data rate the LoRaMAC layer is using. It can be changed by the network server if ADR is enabled.
String LoRamDot::SessionDataRate()
{
	String l_sessionDataRate = "";

	if (SendCommand("AT+SDR", &l_sessionDataRate))
		return l_sessionDataRate;
	else
		return "";
}

// Repeats each frame as many times as indicated or until downlink from network server is received. This setting
//		increases redundancy to increase change of packet to be received by the gateway at the expense of increasing
//		network congestion.When enabled, debug output shows multiple packets being sent.On the Conduit, an MQTT
//		client can listen to the 'packet_recv' topic to see that duplicate packets are received, but not forwarded to the up topic.
//	repeats: Number of send attempts. (Default)
boolean LoRamDot::RepeatPacket(byte repeats)
{
	// Check if the mode is within the valid range
	if (repeats >= 0 && repeats <= 15)
		return SendCommand("AT=REP" + String(repeats));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

/////////////////////////////////////////////
// Sending Packets
/////////////////////////////////////////////

// Sends supplied data and opens a receive window to receive data from the gateway/network server. If a data
//		packet is received, it is output following AT + SEND.To configure the receive data format, use the AT + RXO
//		command.Although parameter1 can be up to 242 bytes, it is limited by the payload size as determined by + TXDR setting
// data: Up to 242 bytes of data or the maximum payload size based on spreading factor (See AT+TXDR)
boolean LoRamDot::Send(String data)
{
	// Check if the data length is within the valid range
	if (data.length() <= 242)
		return SendCommand("AT+SEND=" + data);
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;
	
		return false;
	}
}

// Functions as the +SEND command, but sends hexadecimal data.
// data: String of up to 242 eight bit hexadecimal values. Each value may range from 00 to FF.
boolean LoRamDot::SendBinary(String data)
{
	////////////////////////////////////////////////////////////////////////
	///// TODO: Add check against the length for reduced payload sizes. ////
	////////////////////////////////////////////////////////////////////////
	// Check if the data length is within the valid range
	if (data.length() <= 242)
		return SendCommand("AT+SENDB=" + data);
	
	_lastCommandStatus = false;
	_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
	_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

	return false;
}

// Functions as the +SEND command, but sends hexadecimal data.
// data: String of up to 242 eight bit hexadecimal values. Each value may range from 00 to FF.
boolean LoRamDot::SendBinary(byte data[])
{
	// Convert to HEX String
	String hexData = "";

	for (unsigned long count = 0; count < size_t(data); count++)
	{
		hexData += String(data[count], HEX);
	}

	// Send
	return SendBinary(hexData);
}

/////////////////////////////////////////////
// Receiving Packets
/////////////////////////////////////////////

// Displays the last payload received. It does not initiate reception of new data. Use +SEND to initiate receiving data from the network server.
String LoRamDot::ReceiveOnce()
{
	String l_receivedData = "";

	if (SendCommand("AT+RECV", &l_receivedData))
		return l_receivedData;
	else
		return "";
}

// Formats the receive data output. Data is either processed into hexadecimal data or left unprocessed/raw.
// Hexadecimal outputs the byte values in the response.Raw / Unprocessed outputs the actual bytes on the serial interface.
// format: DATA_FORMAT_HEX = 0, DATA_FORMAT_RAW = 1. 
boolean LoRamDot::ReceiveOutput(byte format)
{
	// Check if the format is within the valid range
	if (format == 0 || format == 1)
		return SendCommand("AT+RXO=" + String(format));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Indicates there is at least one packet pending on the gateway for this end device. This indication is communicated
// to the end device in any packet coming from the server.Each packet contains a data pending bit.
boolean LoRamDot::DataPending()
{
	String l_dataPending = "";

	if (SendCommand("AT+DP", &l_dataPending))
		return (l_dataPending == "1") ? true : false;
	else
		return "";
}

// Enables or disables waiting for RX windows to expire after sending.
// wait: false (0) = Do not wait. Not recommended. true (1) Wait(Default)
boolean LoRamDot::TransmitWait(boolean wait)
{
	// Check if the wait is within the valid range
	if (wait == 0 || wait == 1)
		return SendCommand("AT+TXW=" + String(wait));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

/////////////////////////////////////////////
// Statistics
/////////////////////////////////////////////

// Resets device statistics displayed with the Statistics (AT&S) command.
boolean LoRamDot::ResetStatistics()
{
	return SendCommand("AT&R");
}

// Displays device statistics including join attempts, join failures, packets sent, packets received and missed acks. Use AT&R to reset / clear the statistics.
String LoRamDot::Statistics()
{
	String l_statistics = "";

	if (SendCommand("AT&S", &l_statistics))
		return l_statistics;
	else
		return "";
}

// Displays device statistics including join attempts, join failures, packets sent, packets received and missed acks. Use AT&R to reset / clear the statistics.
String LoRamDot::SignalStrength()
{
	String l_signalStrength = "";

	if (SendCommand("AT+RSSI", &l_signalStrength))
		return l_signalStrength;
	else
		return "";
}

// Displays signal to noise ratio for all packets received from the gateway since the last reset. There are four signal to
//   noise ratio values, which, in order, are: last packet SNR, minimum SNR, maximum SNR and average SNR.Values range from - 20dBm to 20dBm.
String LoRamDot::SignalToNoiseRatio()
{
	String l_snr = "";

	if (SendCommand("AT+SNR", &l_snr))
		return l_snr;
	else
		return "";
}

/////////////////////////////////////////////
// Serial Data Mode
/////////////////////////////////////////////

// Reads serial data, sends packets, and then sleeps using wake settings.The escape sequence is++ + .
//		- When++ + is received to escape serial data mode all buffer data will be discarded.
//		- CTS is handled by the serial driver and is relative to its buffer size.When flow control is enabled, see AT&K.
//		- mDot firmware serial buffer size is 512 bytes.
boolean LoRamDot::SerialDataMode()
{
	return SendCommand("AT+SD");
}

// Configures which operation mode the end device powers up in, either AT command mode or serial data mode.
//		- AT Command mode : The end device powers up or resets in command mode.AT commands are used to send and receive data.
//		- Serial data mode : Allows the end device to send and receive data without entering AT commands.Data
//			is sent and received based on wake command settings.This mode requires network join mode to be set
//			for either auto join or peer - to - peer mode. (AT + NJM = 2 or 3).
// dataMode: ATA_MODE_AT = 0 (AT command mode (Default)) or DATA_MODE_SERIAL = 1 (Serial data mode)
boolean LoRamDot::StartupMode(byte dataMode)
{
	// Check if the dataMode is within the valid range
	if (dataMode == 0 || dataMode == 1)
		return SendCommand("AT+SMODE=" + String(dataMode));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Sets the device to either keep or discard data in the serial buffer when an error occurs.
// discardBuffer: false(0) Data that cannot be sent remains in the serial buffer for later transmission true (1) Data that cannot be sent is discarded.
boolean LoRamDot::SerialDataClearOnError(boolean discardBuffer)
{
	// Check if the discardBuffer is within the valid range
	if (discardBuffer == 0 || discardBuffer == 1)
		return SendCommand("AT+SDCE=" + String(discardBuffer));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

/////////////////////////////////////////////
// Power Management
/////////////////////////////////////////////

// Puts the end device in sleep mode. The end device wakes on interrupt or interval based on AT+WM setting. Once awakened, use AT + SLEEP again to return to sleep mode.
// mode: (0) Deep sleep (ST Micro standby mode) or (1) Sleep (ST Micro stop mode)
boolean LoRamDot::SleepMode(byte sleepMode)
{
	// Check if the sleepMode is within the valid range
	if (sleepMode == 0 || sleepMode == 1)
		return SendCommand("AT+SLEEP=" + String(sleepMode));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// 0 Deep sleep (ST Micro standby mode), 1 Sleep(ST Micro stop mode)
boolean LoRamDot::WakeMode(byte wakeMode)
{
	// Check if the wakeMode is within the valid range
	if (wakeMode == 0 || wakeMode == 1)
		return SendCommand("AT+WM=" + String(wakeMode));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// When using wake mode set to interval, use this command to configure the number of seconds the end device
//	sleeps when in sleep mode.Upon waking, it waits + WD amount of time for an initial character then + WTO amount of time for each additional character.
// interval: 2-2147483647 seconds (Default is 2)
boolean LoRamDot::WakeInterval(unsigned long interval)
{
	// Check if the interval is within the valid range
	if (interval >= 2 && interval <= 2147483647)
		return SendCommand("AT+WI=" + String(interval));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Configures the maximum amount of time to wait for data when the device wakes up from sleep mode. If this timer
//	expires, the device goes back to sleep.If the device received at least one character before this timer expires, the
//	device continues to read input until either the payload is reached or the + WTO timer expires at which time it sends
//	the collected data and goes to sleep.
// delay: 2-2147483647 seconds (Default is 100)
boolean LoRamDot::WakeDelay(unsigned long delay)
{
	// Check if the delay is within the valid range
	if (delay >= 2 && delay <= 2147483647)
		return SendCommand("AT+WD=" + String(delay));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// Configures the amount of time that the device waits for subsequent characters following the first character
//	received upon waking.Once this timer expires, the collected data is sent and the end device goes back to sleep.
// timeout: 0-65000 milliseconds (Default is 20)
boolean LoRamDot::WakeTimeout(unsigned long timeout)
{
	// Check if the timeout is within the valid range
	if (timeout >= 2 && timeout <= 65000)
		return SendCommand("AT+WTO=" + String(timeout));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}

// 	Allows a non-default antenna to be used while still adhering to transmit power regulations.
// gain: -128 to 127 (Default is 3)			
boolean LoRamDot::AntennaGain(int gain)
{
	// Check if the gain is within the valid range
	if (gain >= -128 && gain <= 127)
		return SendCommand("AT+ANT=" + String(gain));
	else
	{
		_lastCommandStatus = false;
		_lastCommandStatusMessage = "INPUT-OUT-OF-RANGE";
		_lastCommandStatusId = COMMAND_STATUS_INPUT_OUT_OF_RANGE;

		return false;
	}
}


