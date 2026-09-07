/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Setup and read values from a SHTC3 temperature and humidity sensor.
 *
 * This example is based on the official WisBlock RAK1901 example, see
 * examples/common/sensors/RAK1901_Temperature_Humidity_SHTC3 in
 * https://github.com/RAKWireless/WisBlock
 *
 * To build this example, you need to install the SHTC3 library with:
 * arduino-cli lib install "SparkFun SHTC3 Humidity and Temperature Sensor Library"
 */

#include <Arduino.h>
#include "SparkFun_SHTC3.h"

SHTC3 g_shtc3; // Declare an instance of the SHTC3 class

// The errorDecoder function prints "SHTC3_Status_TypeDef" results
// in a human-friendly way
void errorDecoder(SHTC3_Status_TypeDef message) {
	switch (message) {
	case SHTC3_Status_Nominal:
		Serial.print("Nominal");
		break;
	case SHTC3_Status_Error:
		Serial.print("Error");
		break;
	case SHTC3_Status_CRC_Fail:
		Serial.print("CRC Fail");
		break;
	default:
		Serial.print("Unknown return code");
		break;
	}
}

void shtc3_read_data(void) {
	float Temperature = 0;
	float Humidity = 0;

	g_shtc3.update();

	// You can also assess the status of the last command by checking the
	// ".lastStatus" member of the object
	if (g_shtc3.lastStatus == SHTC3_Status_Nominal) {

		Temperature = g_shtc3.toDegC(); // Packing LoRa data
		Humidity = g_shtc3.toPercent();

		Serial.print("RH = ");
		Serial.print(g_shtc3.toPercent()); // "toPercent" returns the percent humidity as a floating
										   // point number
		Serial.print("% (checksum: ");

		// Like "passIDcrc" this is true when the RH value is valid from
		// the sensor (but not necessarily up-to-date in terms of time)
		if (g_shtc3.passRHcrc) {
			Serial.print("pass");
		} else {
			Serial.print("fail");
		}

		Serial.print("), T = ");
		Serial.print(g_shtc3.toDegC()); // "toDegF" and "toDegC" return the temperature as a
										// floating point number in deg F and deg C respectively
		Serial.print(" deg C (checksum: ");

		// Like "passIDcrc" this is true when the T value is valid from
		// the sensor (but not necessarily up-to-date in terms of time)
		if (g_shtc3.passTcrc) {
			Serial.print("pass");
		} else {
			Serial.print("fail");
		}
		Serial.println(")");
	} else {
		Serial.print("Update failed, error: ");
		errorDecoder(g_shtc3.lastStatus);
		Serial.println();
	}
}

void setup() {
	time_t timeout = millis();
	Serial.begin(115200);
	while (!Serial) {
		if ((millis() - timeout) < 5000) {
			delay(100);
		} else {
			break;
		}
	}

	Wire.begin();
	Serial.println("shtc3 init");
	Serial.print("Beginning sensor. Result = "); // Most SHTC3 functions return a variable of the
												 // type "SHTC3_Status_TypeDef" to indicate the
												 // status of their execution
	errorDecoder(g_shtc3.begin()); // To start the sensor you must call "begin()", the default
								   // settings use Wire (default Arduino I2C port)
	Wire.setClock(400000);

	Serial.println();

	// Whenever data is received the associated checksum is calculated and
	// verified so you can be sure the data is true
	if (g_shtc3.passIDcrc) { // The checksum pass indicators are: passIDcrc, passRHcrc, and passTcrc
							 // for the ID, RH, and T readings respectively
		Serial.print("ID Passed Checksum. ");
		Serial.print("Device ID: 0b");
		Serial.println(
			g_shtc3.ID,
			BIN); // The 16-bit device ID can be accessed as a member variable of the object
	} else {
		Serial.println("ID Checksum Failed. ");
	}
}

void loop() {
	shtc3_read_data();
	delay(1000);
}
