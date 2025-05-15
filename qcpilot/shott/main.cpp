#include <cassert>
#include "openpilot/common/ratekeeper.h"
#include "openpilot/qcpilot/shott/shott.h"
#include "system/hardware/hw.h"

int main(int argc, char *argv[], const char *envs[]) {
    std::ignore = argc;
    std::ignore = argv;
    std::ignore = envs;
    if (!Hardware::PC()) {
        int ret;
        ret = util::set_realtime_priority(53);
        assert(ret == 0);
        ret = util::set_core_affinity({4});
        assert(ret == 0);
    }
    RateKeeper rateKeeper {"shott", 50};
    qcpilot::Shott shott {};
    bool isMyselfNotLagging {false};
    while (true) {
        isMyselfNotLagging = !rateKeeper.keepTime();
        std::ignore = isMyselfNotLagging;
        shott.step();
    }
    return 0;
}