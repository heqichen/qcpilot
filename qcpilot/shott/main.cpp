#include <QCoreApplication>
#include "network.h"

int main(int argc, char *argv[], const char *envs[]) {
    std::ignore = envs;
    QCoreApplication app(argc, argv);
    qcpilot::Network network {};

    while (true) {
        app.processEvents();
        network.step();
        if (network.isDirty()) {
            printf("network changed\r\n");
            network.clear();
        }
    }
    return 0;
}