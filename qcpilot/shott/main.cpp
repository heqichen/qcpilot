#include <cassert>
#include "openpilot/qcpilot/shott/shott.h"
#include "system/hardware/hw.h"


int main(int argc, const char *argv[], const char *envs[]) {
    if (!Hardware::PC()) {
        int ret;
        ret = util::set_realtime_priority(53);
        assert(ret == 0);
        ret = util::set_core_affinity({4});
        assert(ret == 0);
    }

    qcpilot::shott::Shott shott;
    shott.loop();
    return 0;
}