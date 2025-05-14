#include <QCoreApplication>
#include "network.h"
#include "openpilot/common/ratekeeper.h"

int main(int argc, char *argv[], const char *envs[]) {
    std::ignore = envs;
    RateKeeper rateKeeper {"shott", 1};
    QCoreApplication app(argc, argv);
    qcpilot::Network network {};
    bool isMyselfNotLagging {false};
    while (true) {
        isMyselfNotLagging = !rateKeeper.keepTime();
        std::ignore = isMyselfNotLagging;
        app.processEvents();
        network.step();
        printf("%d\r\n", isMyselfNotLagging);
        if (network.isDirty()) {
            printf("network changed\r\n");
            network.clear();
        }
    }
    return 0;
}