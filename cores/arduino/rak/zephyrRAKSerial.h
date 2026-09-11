/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file zephyrRAKSerial.h
 * @brief RAK extensions to the Arduino serial classes.
 *
 * arduino::ZephyrSerial derives from rak::ZephyrRAKSerial, so the RUI
 * overloads reach every Serial object.
 */

#ifndef __ZEPHYR_RAK_SERIAL_H__
#define __ZEPHYR_RAK_SERIAL_H__

#include <stddef.h>
#include <stdint.h>

#include <zephyr/toolchain.h>

#include <api/String.h>

/* Zephyr device handle. Only ever used as a pointer here, so a declaration
 * keeps <zephyr/device.h> out of every sketch.
 */
struct device;

/**
 * @defgroup rak_serial RAK Serial
 * @brief RUI-compatible serial API.
 * @{
 */

namespace rak {

/**
 * @brief Selects which firmware service owns a serial port.
 *
 * @note This core has no AT command, protocol or pass-through service, so every
 *       port behaves as RAK_CUSTOM_MODE.
 */
enum class RAK_SERIAL_MODE : uint8_t {
	RAK_AT_MODE,      /**< AT command mode. Not implemented; ignored. */
	RAK_API_MODE,     /**< API mode. Not implemented; ignored. */
	RAK_PASS_MODE,    /**< LoRa pass-through mode. Not implemented; ignored. */
	RAK_CUSTOM_MODE,  /**< Sketch owns the port. The only behaviour here. */
	RAK_DEFAULT_MODE, /**< Platform default. */
};

/**
 * @name RUI spellings
 * @{
 */
constexpr RAK_SERIAL_MODE RAK_AT_MODE = RAK_SERIAL_MODE::RAK_AT_MODE;
constexpr RAK_SERIAL_MODE RAK_API_MODE = RAK_SERIAL_MODE::RAK_API_MODE;
constexpr RAK_SERIAL_MODE RAK_PASS_MODE = RAK_SERIAL_MODE::RAK_PASS_MODE;
constexpr RAK_SERIAL_MODE RAK_CUSTOM_MODE = RAK_SERIAL_MODE::RAK_CUSTOM_MODE;
constexpr RAK_SERIAL_MODE RAK_DEFAULT_MODE = RAK_SERIAL_MODE::RAK_DEFAULT_MODE;

/** @} */

/**
 * @brief RAK additions inherited by arduino::ZephyrSerial.
 */
class ZephyrRAKSerial {
public:
	/**
	 * @brief Configures the port. Implemented by the deriving class.
	 *
	 * @param baudrate Bits per second.
	 * @param config   Frame format, for example @c SERIAL_8N1.
	 */
	virtual void begin(unsigned long baudrate, uint16_t config) = 0;

	/**
	 * @brief Writes a byte sequence. Implemented by the deriving class.
	 *
	 * @param buffer Bytes to send.
	 * @param size   Number of bytes.
	 *
	 * @return Number of bytes written.
	 */
	virtual size_t write(const uint8_t *buffer, size_t size) = 0;

	/**
	 * @brief Configures the port and records the RUI mode.
	 *
	 * @param baudrate Bits per second.
	 * @param config   Frame format, for example @c SERIAL_8N1.
	 * @param mode     Recorded for getMode().
	 */
	void begin(unsigned long baudrate, uint16_t config, RAK_SERIAL_MODE mode);

	/**
	 * @brief Configures the port as 8N1 and records the RUI mode.
	 *
	 * @param baudrate Bits per second.
	 * @param mode     Recorded for getMode().
	 */
	void begin(unsigned long baudrate, RAK_SERIAL_MODE mode);

	/**
	 * @brief Returns the mode last passed to begin().
	 *
	 * @return RAK_DEFAULT_MODE if begin() was never called with a mode.
	 */
	RAK_SERIAL_MODE getMode() const;

	/**
	 * @brief Formats text and writes it to the port, like @c printf().
	 *
	 * @param fmt printf-style format string, followed by its arguments.
	 *
	 * @return Number of bytes written.
	 */
	size_t printf(const char *fmt, ...) __printf_like(2, 3);

	/**
	 * @brief Locks or unlocks the AT command console.
	 *
	 * @param locked Ignored.
	 */
	void lock(bool locked);

	/**
	 * @brief Sets the AT console password.
	 *
	 * @param new_passwd Ignored.
	 * @param len        Ignored.
	 *
	 * @return Always @c false, because there is no AT service to protect.
	 */
	bool password(const char *new_passwd, size_t len);

	/**
	 * @brief Sets the AT console password.
	 *
	 * @param new_passwd Ignored.
	 *
	 * @return Always @c false, because there is no AT service to protect.
	 */
	bool password(const ::arduino::String &new_passwd);

	/**
	 * @brief Returns the baudrate the port is running at.
	 *
	 * @return Bits per second, or 0 if the port cannot report it.
	 *
	 * @note A USB CDC ACM port reports the rate the host opened it with, so
	 *       the answer comes from the terminal rather than from begin().
	 */
	uint32_t getBaudrate();

protected:
	virtual const struct device *getUartDevice() const = 0;

	RAK_SERIAL_MODE serial_mode_ = RAK_DEFAULT_MODE; /**< Mode last passed to begin(). */
};

} // namespace rak

/** @} */

#endif /* __ZEPHYR_RAK_SERIAL_H__ */
