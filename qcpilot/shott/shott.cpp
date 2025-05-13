#include "shott.h"
#include <QtDBus>
#include <cstdio>
#include "network_def.h"

namespace qcpilot {

Shott::Shott(QObject *parent) : QObject(parent), adapters_ {} {
    adapters_ = getAdapters();
    QDBusConnection::systemBus().connect(
      NM_DBUS_SERVICE, NM_DBUS_PATH, NM_DBUS_INTERFACE, "DeviceAdded", this, SLOT(deviceAdded(QDBusObjectPath)));
    QDBusConnection::systemBus().connect(
      NM_DBUS_SERVICE, NM_DBUS_PATH, NM_DBUS_INTERFACE, "DeviceRemoved", this, SLOT(deviceRemoved(QDBusObjectPath)));
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
    std::printf("device added: %s\r\n", path.path().toStdString().c_str());
}
void Shott::deviceRemoved(const QDBusObjectPath &path) {
    std::printf("device removed: %s\r\n", path.path().toStdString().c_str());
}


void Shott::step() {
    // std::printf("=======================\r\n");
    // for (const auto &a : adapters_) {
    //     std::printf("%s\r\n", a.toStdString().c_str());
    // }
    // std::printf("=======================\r\n\r\n\r\n");
    // Implement the logic for each step of the Shott process here.
    // This could involve processing data, updating state, etc.
    // For example:
    // qDebug() << "Shott step executed";
}

}    // namespace qcpilot