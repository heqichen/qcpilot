#include <qcpilot/cufud/cufud.h>
#include <cassert>
#include "common/util.h"
#include "system/hardware/hw.h"

int main(int argc, char *argv[], char *envs[]) {
    if (!Hardware::PC()) {
        int ret;
        ret = util::set_realtime_priority(53);
        assert(ret == 0);
        ret = util::set_core_affinity({4});
        assert(ret == 0);
    }
    qcpilot::CuFuD cuFuD;

    while (true) {
        cuFuD.step();
    }
    return 0;
}