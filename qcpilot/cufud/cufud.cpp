#include "qcpilot/cufud/cufud.h"
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <optional>
#include "cereal/messaging/messaging.h"


namespace qcpilot {

CuFuD::CuFuD() :
    contextPtr_ {Context::create()},
    carStateSockPtr_ {SubSocket::create(contextPtr_.get(), "carState")},
    subMasterPtr_ {std::make_unique<SubMaster>(std::vector<const char *> {
      "modelV2",
      "controlsState",
      "liveCalibration",
      "radarState",
      "deviceState",
      "pandaStates",
      "carParams",
      "driverMonitoringState",
      "carState",
      "driverStateV2",
      "wideRoadCameraState",
      "managerState",
      "selfdriveState",
      "longitudinalPlan",
    })} {
    assert(carStateSockPtr_ != nullptr);
    carStateSockPtr_->setTimeout(20);
    assert(subMasterPtr_ != nullptr);
    carStateReaderOpt_.reset();
}

void CuFuD::step() {
    updateInput();
    foo();
}

void CuFuD::foo() {
    if (carStateReaderOpt_.has_value()) {
        std::printf("v = %f\r\n", carStateReaderOpt_->getVEgo());
    }
}
void CuFuD::updateInput() {
    // Clear previous input
    carStateReaderOpt_.reset();

    // Wait/Block for carState
    std::unique_ptr<Message> msg {carStateSockPtr_->receive(false)};
    subMasterPtr_->update(0);
    if (msg) {
        capnp::FlatArrayMessageReader msgReader(carStateBuf_.align(msg.get()));
        cereal::Event::Reader event = msgReader.getRoot<cereal::Event>();
        carStateReaderOpt_ = event.getCarState();
    }
}

void CuFuD() {
    std::unique_ptr<Context> context(Context::create());
    std::unique_ptr<SubSocket> carStateSock(SubSocket::create(context.get(), "carState"));
    std::unique_ptr<SubMaster> sm {std::make_unique<SubMaster>(std::vector<const char *> {
      "modelV2",
      "controlsState",
      "liveCalibration",
      "radarState",
      "deviceState",
      "pandaStates",
      "carParams",
      "driverMonitoringState",
      "carState",
      "driverStateV2",
      "wideRoadCameraState",
      "managerState",
      "selfdriveState",
      "longitudinalPlan",
    })};

    assert(carStateSock != NULL);
    carStateSock->setTimeout(20);
    AlignedBuffer aligned_buf;    // this is where the actual data live in.

    std::printf("ready to receive data\r\n");
    cereal::CarState::Reader carState;
    cereal::CarState::Reader lastCarState;
    while (true) {
        std::unique_ptr<Message> msg {carStateSock->receive(false)};
        sm->update(0);
        if (msg) {
            capnp::FlatArrayMessageReader msgReader(aligned_buf.align(msg.get()));
            cereal::Event::Reader event = msgReader.getRoot<cereal::Event>();

            carState = event.getCarState();
            // std::optional<cereal::CarState::Reader> rr;

            // rr = carState;

            float v = carState.getVEgo();
            float lastV = lastCarState.getVEgo();
            std::printf("%u, v: %f , last: %f\r\n", carState.totalSize().capCount, v, lastV);
            lastCarState = carState;


            if (sm->updated("pandaStates")) {
                auto pandaStates = (*sm)["pandaStates"].getPandaStates();
                if (pandaStates.size() > 0) {
                    auto isallowed = pandaStates[0].getControlsAllowed();
                    std::printf("is allow control: %d\r\n", isallowed);
                }
            }

        } else {
            // std::printf("nohing?\n");
            // timeout
        }

        // std::printf("%lu\r\n", sm->frame);
    }


    // std::unique_ptr<SubMaster> sm(SubMaster::c)

    // carStateSock->
}

}    // namespace qcpilot