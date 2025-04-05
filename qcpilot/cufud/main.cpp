#include <qcpilot/cufud/cufud.h>
#include <cassert>
#include <string>
#include "common/params.h"
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
    Params params_;
    std::string carParamCap = params_.get("CarParams", true);
    assert(carParamCap.size() > 0);
    AlignedBuffer aligned_buf;
    capnp::FlatArrayMessageReader msg(aligned_buf.align(carParamCap.data(), carParamCap.size()));
    cereal::CarParams::Reader carParamsReader = msg.getRoot<cereal::CarParams>();

    qcpilot::cufu::CuFuD cuFuD(carParamsReader);

    cuFuD.loop();

    return 0;
}