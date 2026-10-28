/*
 * Copyright (c) 2022 Mike Szczys
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

/* This header only provides the BSP-compatible names! */

#ifndef _VARIANT_NRF52840DK_
#define _VARIANT_NRF52840DK_

/*
 * Board Pins
 *
 * nRF52840 DK signals that are not routed to the Arduino header and
 * have no Arduino pin number, so a sketch cannot drive them through
 * the Arduino API.
 *
 *   P0.00  XL1       32.768 kHz crystal
 *   P0.01  XL2       32.768 kHz crystal
 *   P0.05  RTS       UART0 (J-Link VCOM)
 *   P0.06  TXD       UART0 (J-Link VCOM)
 *   P0.07  CTS       UART0 (J-Link VCOM)
 *   P0.08  RXD       UART0 (J-Link VCOM)
 *   P0.09  NFC1      NFC antenna
 *   P0.10  NFC2      NFC antenna
 *   P0.17  QSPI_CSN  MX25R64 flash
 *   P0.18  RESET     reset button (UICR 'gpio-as-nreset')
 *   P0.19  QSPI_SCK  MX25R64 flash
 *   P0.20  QSPI_IO0  MX25R64 flash
 *   P0.21  QSPI_IO1  MX25R64 flash
 *   P0.22  QSPI_IO2  MX25R64 flash
 *   P0.23  QSPI_IO3  MX25R64 flash
 *
 * QSPI is disabled and the MX25R64 node is deleted by this variant.
 */

/*
 * Connector Pins
 *
 * Arduino header signals owned by a Zephyr peripheral. The bus pins are
 * set up by devicetree pin control and keep their D/A numbers.
 *
 *   UART1 ('Serial1'): P1.02 TX (D1), P1.01 RX (D0)
 *   UART0 ('Serial2'): J-Link VCOM, see Board Pins
 *   I2C0  ('Wire'):    P0.26 SDA (D14), P0.27 SCL (D15)
 *   SPI3  ('SPI'):     P1.15 SCK (D13), P1.13 MOSI (D11),
 *                      P1.14 MISO (D12), P1.12 CS (D10)
 *   SPI1  (disabled):  P0.31 SCK (A5), P0.30 MOSI (A4), P1.08 MISO (D7)
 *   PWM:               LED1, D3, D5, D6, D9, D10, D11
 *
 * SPI1 is disabled because its upstream pins overlap A4, A5 and D7.
 */

/*
 * Pin count
 */
#define PINS_COUNT         (30u)
#define NUM_DIGITAL_PINS   (30u)
#define NUM_ANALOG_INPUTS  (6u)
#define NUM_ANALOG_OUTPUTS (0u)

/*
 * LEDs (active low)
 *
 * Devicetree marks the LEDs GPIO_ACTIVE_LOW, so writing LED_STATE_ON
 * (HIGH) turns them on. Only LED1 has PWM.
 */
#define PIN_LED1 D22 /* P0.13 */
#define PIN_LED2 D23 /* P0.14 */
#define PIN_LED3 D24 /* P0.15 */
#define PIN_LED4 D25 /* P0.16 */

#define LED_BUILTIN PIN_LED1
#define LED_CONN    PIN_LED2

#define LED_STATE_ON 1

/*
 * Buttons (active low)
 *
 * The buttons have no external pull-up: use INPUT_PULLUP, a pressed
 * button reads LOW.
 */
#define PIN_BUTTON1 D26 /* P0.11 */
#define PIN_BUTTON2 D27 /* P0.12 */
#define PIN_BUTTON3 D28 /* P0.24 */
#define PIN_BUTTON4 D29 /* P0.25 */

/*
 * Analog pins (A0..A5 map to SAADC channels 1, 2, 4, 5, 6, 7)
 */
#define PIN_A0 A0 /* P0.03 */
#define PIN_A1 A1 /* P0.04 */
#define PIN_A2 A2 /* P0.28 */
#define PIN_A3 A3 /* P0.29 */
#define PIN_A4 A4 /* P0.30 */
#define PIN_A5 A5 /* P0.31 */

/*
 * Serial interfaces
 */
#define PIN_SERIAL1_RX D0
#define PIN_SERIAL1_TX D1

/*
 * SPI interfaces
 */
#define SPI_INTERFACES_COUNT 1

#define PIN_SPI_MISO D12
#define PIN_SPI_MOSI D11
#define PIN_SPI_SCK  D13
#define PIN_SPI_CS   D10

#define MISO PIN_SPI_MISO
#define MOSI PIN_SPI_MOSI
#define SCK  PIN_SPI_SCK
#define SS   PIN_SPI_CS

/*
 * Wire interfaces
 */
#define WIRE_INTERFACES_COUNT 1

#define PIN_WIRE_SDA D14
#define PIN_WIRE_SCL D15

#define SDA PIN_WIRE_SDA
#define SCL PIN_WIRE_SCL

#endif /* _VARIANT_NRF52840DK_ */
