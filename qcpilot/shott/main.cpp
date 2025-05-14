#include <QCoreApplication>
#include "network.h"
#include "openpilot/common/ratekeeper.h"
#include "openpilot/qcpilot/shott/shott.h"

int main(int argc, char *argv[], const char *envs[]) {
    std::ignore = envs;
    RateKeeper rateKeeper {"shott", 50};
    QCoreApplication app(argc, argv);
    qcpilot::Network network {};
    qcpilot::Shott shott {};
    bool isMyselfNotLagging {false};
    while (true) {
        isMyselfNotLagging = !rateKeeper.keepTime();
        std::ignore = isMyselfNotLagging;
        app.processEvents();
        network.step();
        if (network.isDirty()) {
            network.clear();
            shott.rebindNetwork();
        }
        shott.step();

        // printf("%d\r\n", isMyselfNotLagging);
    }
    return 0;
}