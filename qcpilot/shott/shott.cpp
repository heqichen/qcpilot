#include "shott.h"
#include <QtDBus>
#include <cstdio>
#include "network_def.h"

namespace qcpilot {

Shott::Shott(QObject *parent) : QObject(parent), adapters_ {} {
    adapters_ = getAdapters();
}

std::vector<QString> Shott::getAdapters() {
    std::vector<QString> adapters {};
    QDBusReply<QList<QDBusObjectPath>> response = call(NM_DBUS_PATH, NM_DBUS_INTERFACE, "GetDevices");
    for (const QDBusObjectPath &path : response.value()) {
        adapters.push_back(path.path());
    }
    return adapters;
}


void Shott::step() {
    std::printf("=======================\r\n");
    for (const auto &a : adapters_) {
        std::printf("%s\r\n", a.toStdString().c_str());
    }
    std::printf("=======================\r\n\r\n\r\n");
    // Implement the logic for each step of the Shott process here.
    // This could involve processing data, updating state, etc.
    // For example:
    // qDebug() << "Shott step executed";
}

}    // namespace qcpilot