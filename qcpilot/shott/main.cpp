#include <sys/resource.h>
#include <unistd.h>
#include <cassert>
#include "cstring"
#include "openpilot/common/ratekeeper.h"
#include "openpilot/qcpilot/shott/dispenser.h"
#include "openpilot/qcpilot/shott/network_state.h"
#include "system/hardware/hw.h"

struct Args {
    bool noDbus {false};
};

Args parseArgs(int argc, char *argv[]) {
    Args ret;

    for (size_t i {1U}; i < argc; ++i) {
        if (std::strcmp(argv[i], "--no-dbus") == 0) {
            ret.noDbus = true;
        }
    }
    return ret;
}

int main(int argc, char *argv[], const char *envs[]) {
    std::ignore = argc;
    std::ignore = argv;
    std::ignore = envs;
    setpriority(PRIO_PROCESS, 0, -19);

    Args args = parseArgs(argc, argv);

    // Wait for one second for the system
    usleep(1000000ULL);    // sleep 1s

    // Initialize
    RateKeeper rateKeeper {"shott", 50};
    cooboc::shott::NetworkState networkState {args.noDbus};
    cooboc::shott::Dispenser dispenser {};

    bool isMyselfNotLagging {false};
    while (true) {
        isMyselfNotLagging = !rateKeeper.keepTime();
        std::ignore = isMyselfNotLagging;
        networkState.step();
        if (networkState.isDirty()) {
            networkState.clear();
            std::printf("need rebind network\r\n");
            dispenser.rebindNetwork();
        }
        dispenser.step();
    }
    return 0;
}