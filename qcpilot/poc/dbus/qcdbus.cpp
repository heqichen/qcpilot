#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <dbus/dbus.h>
#include <iostream>
#include <tuple>
#include <unistd.h>

int main(int, const char *[], const char *[]) {
  DBusMessage *msg;
  DBusMessageIter args;
  DBusConnection *conn;
  DBusError err;
  int ret;
  // char *sigvalue;

  printf("Listening for signals\n");

  // initialise the errors
  dbus_error_init(&err);

  // connect to the bus and check for errors
  conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
  if (dbus_error_is_set(&err)) {
    fprintf(stderr, "Connection Error (%s)\n", err.message);
    dbus_error_free(&err);
  }
  if (NULL == conn) {
    exit(1);
  }

  // request our name on the bus and check for errors
  ret = dbus_bus_request_name(conn, "org.freedesktop.NetworkManager",
                              DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
  if (dbus_error_is_set(&err)) {
    fprintf(stderr, "Name Error (%s)\n", err.message);
    dbus_error_free(&err);
  }
  if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
    exit(1);
  }

  // add a rule for which messages we want to see
  // see signals from the given interface
  dbus_bus_add_match(
      conn, "type='signal',interface='org.freedesktop.NetworkManager'", &err);
  dbus_connection_flush(conn);
  if (dbus_error_is_set(&err)) {
    fprintf(stderr, "Match Error (%s)\n", err.message);
    exit(1);
  }
  printf("Match rule sent\n");

  // loop listening for signals being emmitted
  while (true) {

    // non blocking read of the next available message
    dbus_connection_read_write(conn, 0);
    msg = dbus_connection_pop_message(conn);

    // loop again if we haven't read a message
    if (NULL == msg) {
      usleep(1000000);
      continue;
    }

    // check if the message is a signal from the correct interface and with the
    // correct name
    std::uint32_t value{0U};
    if (dbus_message_is_signal(msg, "org.freedesktop.NetworkManager",
                               "StateChanged")) {
      // read the parameters
      if (!dbus_message_iter_init(msg, &args))
        fprintf(stderr, "Message Has No Parameters\n");
      else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args))
        fprintf(stderr, "Argument is not string!\n");
      else
        dbus_message_iter_get_basic(&args, &value);

      printf("Got Signal with value %u\n", value);
    }

    // free the message
    dbus_message_unref(msg);
  }
  return 0;
}