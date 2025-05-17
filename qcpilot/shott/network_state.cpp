#include "openpilot/qcpilot/shott/network_state.h"
#include <dbus/dbus.h>
#include <cstdint>
#include <cstdio>

namespace cooboc {
namespace shott {

NetworkState::NetworkState() : dbusSystemConn_ {nullptr}, isDirty_ {false} {
    DBusError error;
    dbus_error_init(&error);

    dbusSystemConn_ = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
    if (nullptr == dbusSystemConn_) {
        std::fprintf(stderr, "Cannot get dbus: %s\r\n", error.message);
        dbus_error_free(&error);
    } else {
        // Register signal of device state changed
        dbus_error_init(&error);
        dbus_bus_add_match(dbusSystemConn_, "type='signal',interface='org.freedesktop.NetworkManager.Device'", &error);
        dbus_connection_flush(dbusSystemConn_);
        if (dbus_error_is_set(&error)) {
            std::fprintf(stderr, "Match Error: %s\r\n", error.message);
            dbus_error_free(&error);
        }
    }
}

void NetworkState::step() {
    DBusError error;
    dbus_error_init(&error);

    // non blocking read of the next available message
    dbus_connection_read_write(dbusSystemConn_, 0);

    DBusMessage* msg {nullptr};
    while (msg = dbus_connection_pop_message(dbusSystemConn_), msg != nullptr) {
        // Check if the message is a signal
        if (dbus_message_is_signal(msg, "org.freedesktop.NetworkManager.Device", "StateChanged")) {
            // read the parameters
            DBusMessageIter argsIter;
            if (dbus_message_iter_init(msg, &argsIter)) {
                if (dbus_message_iter_get_arg_type(&argsIter) == 'u') {
                    std::uint32_t currentState {0U};
                    dbus_message_iter_get_basic(&argsIter, &currentState);
                    if (currentState == 100U) {
                        isDirty_ = true;
                    }
                }
            }

            isDirty_ = true;
        }
        dbus_message_unref(msg);
    }
}


}    // namespace shott
}    // namespace cooboc