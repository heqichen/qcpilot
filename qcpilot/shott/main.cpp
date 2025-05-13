#include <QCoreApplication>
#include "network.h"

int main(int argc, char *argv[], const char *envs[]) {
    std::ignore = envs;
    QCoreApplication app(argc, argv);
    qcpilot::Network network {};

    while (true) {
        app.processEvents();
        network.step();
    }
    return 0;
}