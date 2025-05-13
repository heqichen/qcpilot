#include "shott.h"
#include <QtDBus>
#include <cstdio>
#include "network_def.h"

namespace qcpilot {

Shott::Shott(QObject *parent) : QObject(parent), adapters_ {}, isDirty_ {false} {
    adapters_ = getAdapters();
    QDBusConnection::systemBus().connect(
      NM_DBUS_SERVICE, NM_DBUS_PATH, NM_DBUS_INTERFACE, "DeviceAdded", this, SLOT(deviceAdded(QDBusObjectPath)));
    // QDBusConnection::systemBus().connect(
    //   NM_DBUS_SERVICE, NM_DBUS_PATH, NM_DBUS_INTERFACE, "DeviceRemoved", this, SLOT(deviceRemoved(QDBusObjectPath)));
}

std::vector<QString> Shott::getAdapters() {
    std::vector<QString> adapters {};
    QDBusReply<QList<QDBusObjectPath>> response = call(NM_DBUS_PATH, NM_DBUS_INTERFACE, "GetDevices");
    for (const QDBusObjectPath &path : response.value()) {
        adapters.push_back(path.path());
    }
    return adapters;
}

void Shott::deviceAdded(const QDBusObjectPath &path) {
    adapters_.push_back(path.path());
    // std::printf("device added: %s\r\n", path.path().toStdString().c_str());
}
void Shott::deviceRemoved(const QDBusObjectPath &path) {
    // std::printf("device removed: %s\r\n", path.path().toStdString().c_str());
}

void Shott::stateChange(unsigned int new_state, unsigned int previous_state, unsigned int change_reason) {
    isDirty_ = true;
}

void Shott::step() {
    registerStateChange();
}

void Shott::registerStateChange() {
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