/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

/* This header only provides the BSP-compatible names! */

#ifndef _VARIANT_MPS2_AN386_
#define _VARIANT_MPS2_AN386_

/*
 * Arm MPS2 with the AN386 (Cortex-M4F) FPGA image, as emulated by the
 * QEMU mps2-an386 machine. It runs the Arduino core without hardware.
 *
 *   D0..D15   CMSDK GPIO0 pins 0..15
 *   D16, D17  FPGAIO USERLED0, USERLED1
 *   D18, D19  FPGAIO USERPB0, USERPB1 (input only)
 *
 *   UART0 ('Serial'):  zephyr,console, the first QEMU -serial port
 *   UART1 ('Serial1')
 *   SBCON ('Wire'):    I2C shield 0
 *
 * The sketch partition is simulated flash at 0x20380000 (480 KB), which
 * QEMU preloads with the sketch. There is no ADC, DAC, PWM or SPI.
 */

/*
 * Pin count
 */
#define PINS_COUNT         (20u)
#define NUM_DIGITAL_PINS   (20u)
#define NUM_ANALOG_INPUTS  (0u)
#define NUM_ANALOG_OUTPUTS (0u)

/*
 * LEDs (active high)
 */
#define PIN_LED1 D16 /* USERLED0 */
#define PIN_LED2 D17 /* USERLED1 */

#define LED_BUILTIN PIN_LED1

#define LED_STATE_ON 1

/*
 * Buttons
 */
#define PIN_BUTTON1 D18 /* USERPB0 */
#define PIN_BUTTON2 D19 /* USERPB1 */

/*
 * SPI and Wire interfaces
 */
#define SPI_INTERFACES_COUNT  0
#define WIRE_INTERFACES_COUNT 1

extern uint32_t SystemCoreClock;

#endif /* _VARIANT_MPS2_AN386_ */
