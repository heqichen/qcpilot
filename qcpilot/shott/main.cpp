#include <QCoreApplication>
#include "shott.h"

int main(int argc, char *argv[], const char *envs[]) {
    QCoreApplication app(argc, argv);
    qcpilot::Shott shott {};

    while (true) {
        app.processEvents();
        shott.step();
    }
    return 0;
}