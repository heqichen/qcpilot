// receiver.cpp
#include <cstdint>
#include <dbus/dbus.h>
#include <iostream>

int main() {
  DBusError err;
  dbus_error_init(&err);

  DBusConnection *conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
  if (dbus_error_is_set(&err)) {
    std::cerr << "Connection Error : " << err.message << std::endl;
    dbus_error_free(&err);
    return 1;
  }
  if (!conn) {
    std::cerr << "Failed to connect to the bus" << std::endl;
    return 1;
  }

  const char *signal_name = "org.freedesktop.NetworkManager";
  //  const char *signal_path = "/org/freedesktop/NetworkManager";
  const char *signal_interface = "org.freedesktop.NetworkManager";

  dbus_bus_add_match(
      conn,
      "type='signal',interface='org.freedesktop.NetworkManager',member='"
      "StateChanged'",
      &err);
  dbus_connection_flush(conn);
  if (dbus_error_is_set(&err)) {
    std::cerr << "Match Error: " << err.message << std::endl;
    dbus_error_free(&err);
    dbus_connection_unref(conn);
    return 1;
  }

  while (true) {
    dbus_connection_read_write(conn, 0);
    DBusMessage *msg = dbus_connection_pop_message(conn);

    if (msg == nullptr) {
      continue;
    }

    if (dbus_message_is_signal(msg, signal_interface, signal_name)) {
      DBusMessageIter iter;
      dbus_message_iter_init(msg, &iter);
      std::uint32_t data;
      dbus_message_iter_get_basic(&iter, &data);
      std::cout << "Received signal: " << data << std::endl;
    }
    dbus_message_unref(msg);
  }
  dbus_connection_unref(conn);
  return 0;
}