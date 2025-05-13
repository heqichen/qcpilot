#include "network.h"
#include <QtDBus>
#include <cstdio>
#include "network_def.h"

namespace qcpilot {


Network::Network(QObject *parent) : QObject(parent), adapters_ {}, isDirty_ {false} {
    adapters_ = getAdapters();
    QDBusConnection::systemBus().connect(
      NM_DBUS_SERVICE, NM_DBUS_PATH, NM_DBUS_INTERFACE, "DeviceAdded", this, SLOT(deviceAdded(QDBusObjectPath)));
    // QDBusConnection::systemBus().connect(
    //   NM_DBUS_SERVICE, NM_DBUS_PATH, NM_DBUS_INTERFACE, "DeviceRemoved", this, SLOT(deviceRemoved(QDBusObjectPath)));
}

std::vector<QString> Network::getAdapters() {
    std::vector<QString> adapters {};
    QDBusReply<QList<QDBusObjectPath>> response = call(NM_DBUS_PATH, NM_DBUS_INTERFACE, "GetDevices");
    for (const QDBusObjectPath &path : response.value()) {
        adapters.push_back(path.path());
    }
    return adapters;
}

void Network::deviceAdded(const QDBusObjectPath &path) {
    adapters_.push_back(path.path());
    // std::printf("device added: %s\r\n", path.path().toStdString().c_str());
}
void Network::deviceRemoved(const QDBusObjectPath &path) {
    std::ignore = path;
    // std::printf("device removed: %s\r\n", path.path().toStdString().c_str());
}

void Network::stateChange(unsigned int new_state, unsigned int previous_state, unsigned int change_reason) {
    std::ignore = new_state;
    std::ignore = previous_state;
    std::ignore = change_reason;
    isDirty_ = true;
}

void Network::step() {
    registerStateChange();
}

void Network::registerStateChange() {
    QDBusConnection bus = QDBusConnection::systemBus();
    for (const QString &devicePath : adapters_) {
        bus.connect(NM_DBUS_SERVICE,
                    devicePath,
                    NM_DBUS_INTERFACE_DEVICE,
                    "StateChanged",
                    this,
                    SLOT(stateChange(unsigned int, unsigned int, unsigned int)));
        std::printf("register state change: %s\r\n", devicePath.toStdString().c_str());
    }
    adapters_.clear();
}

}    // namespace qcpilot