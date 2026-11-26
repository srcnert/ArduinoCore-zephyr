/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/llext/symbol.h>

#define FORCE_EXPORT_SYM(name)                                                                     \
	extern void name(void);                                                                        \
	EXPORT_SYMBOL(name);

FORCE_EXPORT_SYM(bt_enable);
FORCE_EXPORT_SYM(bt_set_name);
FORCE_EXPORT_SYM(bt_id_get);
FORCE_EXPORT_SYM(bt_le_adv_start);
FORCE_EXPORT_SYM(bt_conn_cb_register);
FORCE_EXPORT_SYM(bt_conn_get_dst);
FORCE_EXPORT_SYM(bt_gatt_service_register);
FORCE_EXPORT_SYM(bt_gatt_attr_read);
FORCE_EXPORT_SYM(bt_gatt_attr_read_service);
FORCE_EXPORT_SYM(bt_gatt_attr_read_chrc);
