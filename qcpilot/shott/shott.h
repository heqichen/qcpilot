#ifndef __OPENPILOT_QCPILOT_SHOTT_H__
#define __OPENPILOT_QCPILOT_SHOTT_H__

#include <QObject>
#include <QtDBus>
#include <vector>
#include "network_def.h"

namespace qcpilot {


template<typename T = QDBusMessage, typename... Args>
T call(const QString &path, const QString &interface, const QString &method, Args &&...args) {
    QDBusInterface nm(NM_DBUS_SERVICE, path, interface, QDBusConnection::systemBus());
    nm.setTimeout(DBUS_TIMEOUT);

    QDBusMessage response = nm.call(method, std::forward<Args>(args)...);
    if (response.type() == QDBusMessage::ErrorMessage) {
        qCritical() << "DBus call error:" << response.errorMessage();
        return T();
    }

    if constexpr (std::is_same_v<T, QDBusMessage>) {
        return response;
    } else if (response.arguments().count() >= 1) {
        QVariant vFirst = response.arguments().at(0).value<QDBusVariant>().variant();
        if (vFirst.canConvert<T>()) {
            return vFirst.value<T>();
        }
        QDebug critical = qCritical();
        critical << "Variant unpacking failure :" << method << ',';
        (critical << ... << args);
    }
    return T();
}

class Shott : public QObject {
    Q_OBJECT
  public:
    Shott(QObject *parent = nullptr);
    virtual ~Shott() {}
    void step();

  private:
    std::vector<QString> adapters_;

    std::vector<QString> getAdapters();
  private slots:
    void deviceAdded(const QDBusObjectPath &path);
    void deviceRemoved(const QDBusObjectPath &path);

};    // classShott:public QObject
}    // namespace qcpilot
#endif
