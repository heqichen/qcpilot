#include "qcpilot/cufud/cufud.h"
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <memory>
#include "cereal/messaging/messaging.h"
#include "common/util.h"
#include "system/hardware/hw.h"

/*
    self.car_state_sock = messaging.sub_sock('carState', timeout=20)
    self.sm = messaging.SubMaster(['deviceState', 'pandaStates', 'peripheralState', 'modelV2',
   'liveCalibration', 'carOutput', 'driverMonitoringState', 'longitudinalPlan', 'livePose',
                                   'managerState', 'liveParameters', 'radarState',
   'liveTorqueParameters',
                                   'controlsState', 'carControl', 'driverAssistance', 'alertDebug']
   + \ self.camera_packets + self.sensor_packets + self.gps_packets, ignore_alive=ignore,
   ignore_avg_freq=ignore, ignore_valid=ignore, frequency=int(1/DT_CTRL))


*/

class RateKeeper {
  public:
    RateKeeper(std::uint32_t rate) {
        std::unique_ptr<Context> context(Context::create());
        std::unique_ptr<SubSocket> carStateSock(SubSocket::create(context.get(), "carState"));
        assert(carStateSock != NULL);
        carStateSock->setTimeout(20);
        AlignedBuffer aligned_buf;


        std::printf("ready to receive data\r\n");

        while (true) {
            std::unique_ptr<Message> msg {carStateSock->receive(false)};
            if (msg) {
                std::size_t msgSize = msg->getSize();
                std::printf("%ld\r\n", msgSize);

                capnp::FlatArrayMessageReader msgReader(aligned_buf.align(msg.get()));
                cereal::Event::Reader event = msgReader.getRoot<cereal::Event>();
                auto carState = event.getCarState();
                float v = carState.getVEgo();
                std::printf("v: %f\r\n", v);
            } else {
                // std::printf("nohing?");
                // timeout
            }
        }


        // std::unique_ptr<SubMaster> sm(SubMaster::c)

        // carStateSock->
    }
};

int main(int argc, char *argv[], char *envs[]) {
    if (!Hardware::PC()) {
        int ret;
        ret = util::set_realtime_priority(53);
        assert(ret == 0);
        ret = util::set_core_affinity({4});
        assert(ret == 0);
    }
    qcpilot::CufuD cufud;
    RateKeeper rk {100};


    return 0;
}


namespace qcpilot {
CufuD::CufuD() {}
}    // namespace qcpilot
