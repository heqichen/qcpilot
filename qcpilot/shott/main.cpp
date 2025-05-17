#include <unistd.h>
#include <cassert>
#include "openpilot/common/ratekeeper.h"
#include "openpilot/qcpilot/shott/dispenser.h"
#include "openpilot/qcpilot/shott/network_state.h"
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

    // Wait for one second for the system
    usleep(1000000ULL);    // sleep 1s

    // Initialize
    RateKeeper rateKeeper {"shott", 50};
    cooboc::shott::NetworkState networkState {};
    cooboc::shott::Dispenser dispenser {};

    bool isMyselfNotLagging {false};
    while (true) {
        isMyselfNotLagging = !rateKeeper.keepTime();
        std::ignore = isMyselfNotLagging;
        networkState.step();
        if (networkState.isDirty()) {
            networkState.clear();
            dispenser.rebindNetwork();
            std::printf("need rebind network\r\n");
        }
        dispenser.step();
    }
    return 0;
}