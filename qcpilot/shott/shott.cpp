#include "openpilot/qcpilot/shott/shott.h"
#include <cstdio>
#include <vector>

namespace qcpilot {
namespace shott {

const std::vector<const char *> kSignals {"qcMazdaState", "carState", "deviceState"};

Shott::Shott() :
    rateKeeper_ {"shott", 50},
    isMyselfNotLagging_ {false},
    sm_ {kSignals},
    subMasterPtr_ {std::make_unique<SubMaster>(kSignals)} {}

void Shott::loop() {
    while (true) {
        step();
        isMyselfNotLagging_ = !rateKeeper_.keepTime();
        // std::printf("lagging? %d\r\n", isMyselfNotLagging_);
    }
}

void Shott::step() {
    // update message
    sm_.update(0);
    subMasterPtr_->update(0);
    if (sm_.updated("qcMazdaState")) {
        // process message
        std::printf("got mazda state\r\n");
    } else {
        std::printf("no mazda state\r\n");
    }

    // if (subMasterPtr_->updated("deviceState")) {
    //     std::printf("got mazda state\r\n");
    // } else {
    //     std::printf("no mazda state\r\n");
    // }
}

}    // namespace shott
}    // namespace qcpilot
