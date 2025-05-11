#include <QCoreApplication>
#include <QDBusInterface>
#include <QDebug>

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);

    if (!QDBusConnection::systemBus().isConnected()) {
        qWarning().noquote() << "complexping"
                             << "Cannot connect to the D-Bus session bus.\n"
                                "To start it, run:\n"
                                "\teval `dbus-launch --auto-syntax`";
        return 1;
    } else {
        qDebug() << "success";
    }

    // QDBusServiceWatcher serviceWatcher(
    //   SERVICE_NAME, QDBusConnection::systemBus(), QDBusServiceWatcher::WatchForRegistration);

    // Ping ping;
    // QObject::connect(&serviceWatcher, &QDBusServiceWatcher::serviceRegistered, &ping, &Ping::start);

    // QProcess pong;
    // pong.start("./complexpong");

    app.exec();
}