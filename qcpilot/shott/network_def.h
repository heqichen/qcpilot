#ifndef __OPENPILOT_QCPILOT_SHOTT_NETWORK_DEF_H__
#define __OPENPILOT_QCPILOT_SHOTT_NETWORK_DEF_H__

#include <QString>

namespace qcpilot {

const QString NM_DBUS_PATH = "/org/freedesktop/NetworkManager";
const QString NM_DBUS_PATH_SETTINGS = "/org/freedesktop/NetworkManager/Settings";

const QString NM_DBUS_INTERFACE = "org.freedesktop.NetworkManager";
const QString NM_DBUS_INTERFACE_PROPERTIES = "org.freedesktop.DBus.Properties";
const QString NM_DBUS_INTERFACE_SETTINGS = "org.freedesktop.NetworkManager.Settings";
const QString NM_DBUS_INTERFACE_SETTINGS_CONNECTION = "org.freedesktop.NetworkManager.Settings.Connection";
const QString NM_DBUS_INTERFACE_DEVICE = "org.freedesktop.NetworkManager.Device";
const QString NM_DBUS_INTERFACE_DEVICE_WIRELESS = "org.freedesktop.NetworkManager.Device.Wireless";
const QString NM_DBUS_INTERFACE_ACCESS_POINT = "org.freedesktop.NetworkManager.AccessPoint";
const QString NM_DBUS_INTERFACE_ACTIVE_CONNECTION = "org.freedesktop.NetworkManager.Connection.Active";
const QString NM_DBUS_INTERFACE_IP4_CONFIG = "org.freedesktop.NetworkManager.IP4Config";

const QString NM_DBUS_SERVICE = "org.freedesktop.NetworkManager";

const int NM_DEVICE_STATE_UNKNOWN = 0;
const int NM_DEVICE_STATE_ACTIVATED = 100;
const int NM_DEVICE_STATE_NEED_AUTH = 60;
const int NM_DEVICE_TYPE_WIFI = 2;
const int NM_DEVICE_TYPE_MODEM = 8;
const int NM_DEVICE_STATE_REASON_SUPPLICANT_DISCONNECT = 8;
const int DBUS_TIMEOUT = 100;

// https://developer-old.gnome.org/NetworkManager/1.26/nm-dbus-types.html#NMMetered
const int NM_METERED_UNKNOWN = 0;
const int NM_METERED_YES = 1;
const int NM_METERED_NO = 2;
const int NM_METERED_GUESS_YES = 3;
const int NM_METERED_GUESS_NO = 4;


}    // namespace qcpilot

#endif