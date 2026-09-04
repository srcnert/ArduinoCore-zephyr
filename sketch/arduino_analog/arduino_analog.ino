/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * This example read WB_A1 pin in analog signal.
 */

#include "Arduino.h"

// Set pin number
#if defined(WISBLOCK_BASE_19007)
uint8_t analogPin = WB_A1;
#else
#warning Please set the right pin refer to the documentation
uint8_t analogPin = 0xFF; // analog Pins
#endif

/*
 * Every ADC channel in rak4631_nrf52840.overlay uses ADC_GAIN_1_6
 * with ADC_REF_INTERNAL (0.6 V), so a reading saturates at 0.6 V * 6 = 3.6 V.
 */
const float adcReference = 3.6f;

void setup() {
	// initialize serial communication at 115200 bits per second
	Serial.begin(115200);
	delay(2000);
	Serial.println("RAKwireless Arduino Analog Example");
	Serial.println("------------------------------------------------------");

	// Assume RAK5811 is plugged, so turn on RAK5811 first.
	pinMode(WB_IO1, OUTPUT);
	digitalWrite(WB_IO1, HIGH);

	analogReadResolution(14);
}

void loop() {
	/*
	 * analogRead() rescales its result to the resolution asked for in
	 * setup(), so the full-scale count is 2^analogReadResolution().
	 */
	float max_count = (float)(1UL << analogReadResolution());

	int adc_value = analogRead(analogPin);
	Serial.printf("ADC pin value = %d\r\n", adc_value); // print analogPin adc value
	// This is the formula to get the input voltage of RAK5811:
	Serial.printf("Voltage value = %f V\r\n",
				  (double)(adcReference * (((float)adc_value) / max_count) * (5.0f) / (3.0f)));
	delay(1000);
}
