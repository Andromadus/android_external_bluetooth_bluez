/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2004-2006  Marcel Holtmann <marcel@holtmann.org>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef __H_BLUEZ_DBUS_H__
#define __H_BLUEZ_DBUS_H__

#include <stdint.h>
#include <dbus/dbus.h>

#define __END_SIG__ DBUS_TYPE_INVALID_AS_STRING

#define BASE_PATH		"/org/bluez"
#define BASE_INTERFACE		"org.bluez"

#define ADAPTER_PATH		BASE_PATH "/Adapter"
#define ADAPTER_INTERFACE	BASE_INTERFACE ".Adapter"

#define MANAGER_PATH		BASE_PATH "/Manager"
#define MANAGER_INTERFACE	BASE_INTERFACE ".Manager"

#define ERROR_INTERFACE		BASE_INTERFACE ".Error"

#define SECURITY_INTERFACE	BASE_INTERFACE ".Security"

#define MANAGER_PATH_MASK	(1 << 15)
#define ADAPTER_PATH_MASK	(1 << 14)

/* /org/bluez/Manager */
#define MANAGER_ROOT_ID		MANAGER_PATH_MASK

/* /org/bluez/Adapter */
#define ADAPTER_ROOT_ID		ADAPTER_PATH_MASK

/* E.g. /org/bluez/Adapter/hci0 */
#define ADAPTER_PATH_ID		(ADAPTER_PATH_MASK | 0x0001)

#define INVALID_PATH_ID		0xFFFF
#define INVALID_DEV_ID		0xFFFF

#define MAX_PATH_LENGTH		64

typedef DBusHandlerResult (*service_handler_func_t) (DBusConnection *conn,
							DBusMessage *msg,
							void *user_data);

struct service_data {
	const char		*name;
	service_handler_func_t	handler_func;
};

typedef int (timeout_handler_func_t) (void *data);

struct hci_dbus_data {
	uint16_t dev_id;
	uint16_t path_id;
	uint32_t discoverable_timeout;
	uint32_t timeout_hits;
	timeout_handler_func_t *timeout_handler;
	uint8_t mode;
	char *requestor_name;
};

struct passkey_agent {
	char *addr;
	char *name;
	char *path;
};

typedef int register_function_t(DBusConnection *conn, uint16_t id);
typedef int unregister_function_t(DBusConnection *conn, uint16_t id);

DBusHandlerResult msg_func_device(DBusConnection *conn, DBusMessage *msg, void *data);
DBusHandlerResult msg_func_manager(DBusConnection *conn, DBusMessage *msg, void *data);

DBusHandlerResult bluez_new_failure_msg(DBusConnection *conn, DBusMessage *msg, const uint32_t ecode);

DBusMessage *dev_signal_factory(const int devid, const char *prop_name, const int first, ...);

DBusConnection *get_dbus_connection(void);

int get_default_dev_id(void);

DBusHandlerResult error_failed(DBusConnection *conn, DBusMessage *msg, int err);
DBusHandlerResult error_invalid_arguments(DBusConnection *conn, DBusMessage *msg);
DBusHandlerResult error_not_authorized(DBusConnection *conn, DBusMessage *msg);
DBusHandlerResult error_out_of_memory(DBusConnection *conn, DBusMessage *msg);
DBusHandlerResult error_no_such_adapter(DBusConnection *conn, DBusMessage *msg);
DBusHandlerResult error_unknown_address(DBusConnection *conn, DBusMessage *msg);
DBusHandlerResult error_not_available(DBusConnection *conn, DBusMessage *msg);
DBusHandlerResult error_not_connected(DBusConnection *conn, DBusMessage *msg);
DBusHandlerResult error_unsupported_major_class(DBusConnection *conn, DBusMessage *msg);
DBusHandlerResult error_bonding_already_exists(DBusConnection *conn, DBusMessage *msg);
DBusHandlerResult error_bonding_does_not_exist(DBusConnection *conn, DBusMessage *msg);
DBusHandlerResult error_bonding_in_progress(DBusConnection *conn, DBusMessage *msg);
DBusHandlerResult error_discover_in_progress(DBusConnection *conn, DBusMessage *msg);
DBusHandlerResult error_passkey_agent_already_exists(DBusConnection *conn, DBusMessage *msg);
DBusHandlerResult error_passkey_agent_does_not_exist(DBusConnection *conn, DBusMessage *msg);

typedef void (*name_cb_t)(const char *name, void *user_data);

int name_listener_add(DBusConnection *connection, const char *name,
				name_cb_t func, void *user_data);
int name_listener_remove(DBusConnection *connection, const char *name,
				name_cb_t func, void *user_data);

DBusHandlerResult handle_security_method(DBusConnection *conn, DBusMessage *msg, void *data);

service_handler_func_t find_service_handler(struct service_data *services, DBusMessage *msg);

static inline DBusHandlerResult send_reply_and_unref(DBusConnection *conn, DBusMessage *msg)
{
	return DBUS_HANDLER_RESULT_HANDLED;
}

/*======================================================================== 
    BlueZ D-Bus Manager service definitions "/org/bluez/Manager"
 *========================================================================*/

#define MGR_LIST_ADAPTERS	"ListAdapters"
#define MGR_DEFAULT_ADAPTER	"DefaultAdapter"

/* Signals sent in the Manager path */
#define BLUEZ_MGR_DEV_ADDED		"AdapterAdded"
#define BLUEZ_MGR_DEV_REMOVED		"AdapterRemoved"

/* Manager Signatures */
#define MGR_LIST_ADAPTERS_SIGNATURE		__END_SIG__
#define MGR_DEFAULT_ADAPTER_SIGNATURE		__END_SIG__

#define MGR_REPLY_ADAPTER_LIST_SIGNATURE	DBUS_TYPE_ARRAY_AS_STRING \
						__END_SIG__

/*======================================================================== 
    BlueZ D-Bus Adapter path definitions "/org/bluez/Adapter"
 *========================================================================*/
#define DEV_GET_ADDRESS			"GetAddress"
#define DEV_GET_VERSION			"GetVersion"
#define DEV_GET_REVISION		"GetRevision"
#define DEV_GET_MANUFACTURER		"GetManufacturer"
#define DEV_GET_COMPANY			"GetCompany"
#define DEV_GET_FEATURES		"GetFeatures"
#define DEV_GET_MODE			"GetMode"
#define DEV_LIST_MINOR_CLASSES		"ListAvailableMinorClasses"
#define DEV_SET_MODE			"SetMode"
#define DEV_GET_DISCOVERABLE_TO		"GetDiscoverableTimeout"
#define DEV_SET_DISCOVERABLE_TO		"SetDiscoverableTimeout"
#define DEV_IS_CONNECTABLE		"IsConnectable"
#define DEV_IS_DISCOVERABLE		"IsDiscoverable"
#define DEV_GET_MAJOR_CLASS		"GetMajorClass"
#define DEV_GET_MINOR_CLASS		"GetMinorClass"
#define DEV_SET_MINOR_CLASS		"SetMinorClass"
#define DEV_GET_SERVICE_CLASSES		"GetServiceClasses"
#define DEV_GET_NAME			"GetName"
#define DEV_SET_NAME			"SetName"
#define DEV_GET_REMOTE_VERSION		"GetRemoteVersion"
#define DEV_GET_REMOTE_REVISION		"GetRemoteRevision"
#define DEV_GET_REMOTE_MANUFACTURER	"GetRemoteManufacturer"
#define DEV_GET_REMOTE_COMPANY		"GetRemoteCompany"
#define DEV_GET_REMOTE_NAME		"GetRemoteName"
#define DEV_GET_REMOTE_ALIAS		"GetRemoteAlias"
#define DEV_SET_REMOTE_ALIAS		"SetRemoteAlias"
#define DEV_LAST_SEEN			"LastSeen"
#define DEV_LAST_USED			"LastUsed"
#define DEV_CREATE_BONDING		"CreateBonding"
#define DEV_REMOVE_BONDING		"RemoveBonding"
#define DEV_HAS_BONDING_NAME		"HasBonding"
#define DEV_LIST_BONDINGS		"ListBondings"
#define DEV_GET_PIN_CODE_LENGTH		"GetPinCodeLength"
#define DEV_GET_ENCRYPTION_KEY_SIZE	"GetEncryptionKeySize"
#define DEV_DISCOVER_DEVICES		"DiscoverDevices"
#define DEV_CANCEL_DISCOVERY		"CancelDiscovery"
#define DEV_DISCOVER_CACHE		"DiscoverCache"
#define DEV_DISCOVER_SERVICE		"DiscoverService"

/*FIXME: maybe this section can be moved to a internal header file */
/* Adapter service signature */
#define DEV_GET_ADDRESS_SIGNATURE		__END_SIG__
#define DEV_GET_VERSION_SIGNATURE		__END_SIG__
#define DEV_GET_REVISION_SIGNATURE		__END_SIG__
#define DEV_GET_MANUFACTURER_SIGNATURE		__END_SIG__
#define DEV_GET_COMPANY_SIGNATURE		__END_SIG__
#define DEV_GET_FEATURES_SIGNATURE		__END_SIG__
#define DEV_GET_MODE_SIGNATURE			__END_SIG__
#define DEV_LIST_MINOR_CLASSES_SIGNATURE	__END_SIG__
#define DEV_SET_MODE_SIGNATURE			DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__
#define DEV_GET_DISCOVERABLE_TO_SIGNATURE	__END_SIG__
#define DEV_SET_DISCOVERABLE_TO_SIGNATURE	DBUS_TYPE_UINT32_AS_STRING \
						__END_SIG__
#define DEV_IS_CONNECTABLE_SIGNATURE		__END_SIG__
#define DEV_IS_DISCOVERABLE_SIGNATURE		__END_SIG__
#define DEV_GET_MAJOR_CLASS_SIGNATURE		__END_SIG__
#define DEV_GET_MINOR_CLASS_SIGNATURE		__END_SIG__
#define DEV_SET_MINOR_CLASS_SIGNATURE		DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__
#define DEV_GET_SERVICE_CLASSES_SIGNATURE	__END_SIG__
#define DEV_GET_NAME_SIGNATURE			__END_SIG__
#define DEV_SET_NAME_SIGNATURE			DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__
#define DEV_GET_REMOTE_VERSION_SIGNATURE	DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__
#define DEV_GET_REMOTE_REVISION_SIGNATURE	DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__
#define DEV_GET_REMOTE_MANUFACTURER_SIGNATURE	DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__
#define DEV_GET_REMOTE_COMPANY_SIGNATURE	DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__
#define DEV_GET_REMOTE_NAME_SIGNATURE		DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__
#define DEV_GET_REMOTE_ALIAS_SIGNATURE		DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__
#define DEV_SET_REMOTE_ALIAS_SIGNATURE		DBUS_TYPE_STRING_AS_STRING \
						DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__
#define DEV_LAST_SEEN_SIGNATURE			DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__
#define DEV_LAST_USED_SIGNATURE			DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__
#define DEV_CREATE_BONDING_SIGNATURE		DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__
#define DEV_REMOVE_BONDING_SIGNATURE		DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__
#define DEV_HAS_BONDING_SIGNATURE		DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__
#define DEV_LIST_BONDINGS_SIGNATURE		__END_SIG__
#define DEV_GET_PIN_CODE_LENGTH_SIGNATURE	DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__
#define DEV_GET_ENCRYPTION_KEY_SIZE_SIGNATURE	DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__
#define DEV_DISCOVER_DEVICES_SIGNATURE		__END_SIG__
#define DEV_CANCEL_DISCOVERY_SIGNATURE		__END_SIG__
#define DEV_DISCOVER_CACHE_SIGNATURE		__END_SIG__
#define DEV_DISCOVER_SERVICE_SIGNATURE		DBUS_TYPE_STRING_AS_STRING \
						__END_SIG__

/* Signals sent in the adapter based path /org/bluez/Adapter/{hci0, hci1, ...} */
#define DEV_SIG_MODE_CHANGED		"ModeChanged"
#define DEV_SIG_NAME_CHANGED		"NameChanged"
#define DEV_SIG_MINOR_CLASS_CHANGED	"MinorClassChanged"
#define DEV_SIG_REMOTE_NAME_UPDATED	"RemoteNameUpdated"
#define DEV_SIG_REMOTE_ALIAS_CHANGED	"RemoteAliasChanged"
#define DEV_SIG_BONDING_CREATED		"BondingCreated"
#define DEV_SIG_BONDING_FAILED		"BondingFailed"
#define DEV_SIG_BONDING_REMOVED		"BondingRemoved"
#define DEV_SIG_DISCOVER_START		"DiscoveryStart"
#define DEV_SIG_DISCOVER_COMPLETE	"DiscoveryCompleted"
#define DEV_SIG_REMOTE_DEVICE_FOUND	"RemoteDeviceFound"

/*
 * Scanning modes, used by DEV_SET_MODE
 * off: remote devices are not allowed to find or connect to this device
 * connectable: remote devices are allowed to connect, but they are not
 *              allowed to find it.
 * discoverable: remote devices are allowed to connect and find this device
 * unknown: reserved to not allowed/future modes
 */
#define MODE_OFF		"off"
#define MODE_CONNECTABLE	"connectable"
#define MODE_DISCOVERABLE	"discoverable"
#define MODE_UNKNOWN		"unknown"

#define DFT_DISCOVERABLE_TIMEOUT	180	/* 3 seconds */
#define DISCOVERABLE_TIMEOUT_OFF	0

/* BLUEZ_DBUS_ERROR 
 * EFailed error messages signature is : su
 * Where the first argument is a string(error message description),
 * the last  is a uint32 that contains the error class(system, dbus or hci). */

/* Error code offsets */
#define BLUEZ_EBT_OFFSET		(0x00000000) /* see Bluetooth error code */
#define BLUEZ_EBT_EXT_OFFSET		(0x00000100)
#define BLUEZ_EDBUS_OFFSET		(0x00010000)
#define BLUEZ_ESYSTEM_OFFSET		(0x00020000)
#define BLUEZ_EFUTURE_OFFSET		(0x00040000)

/* D-Bus error code, class BLUEZ_EDBUS_OFFSET */
#define BLUEZ_EDBUS_UNKNOWN_METHOD	(0x01 | BLUEZ_EDBUS_OFFSET)
#define BLUEZ_EDBUS_WRONG_SIGNATURE	(0x02 | BLUEZ_EDBUS_OFFSET)
#define BLUEZ_EDBUS_WRONG_PARAM		(0x03 | BLUEZ_EDBUS_OFFSET)
#define BLUEZ_EDBUS_RECORD_NOT_FOUND	(0x04 | BLUEZ_EDBUS_OFFSET)
#define BLUEZ_EDBUS_NO_MEM   		(0x05 | BLUEZ_EDBUS_OFFSET)
#define BLUEZ_EDBUS_CONN_NOT_FOUND	(0x06 | BLUEZ_EDBUS_OFFSET)
#define BLUEZ_EDBUS_UNKNOWN_PATH	(0x07 | BLUEZ_EDBUS_OFFSET)
#define BLUEZ_EDBUS_NOT_IMPLEMENTED	(0x08 | BLUEZ_EDBUS_OFFSET)

#endif /* __H_BLUEZ_DBUS_H__ */
