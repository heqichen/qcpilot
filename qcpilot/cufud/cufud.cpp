#include "qcpilot/cufud/cufud.h"
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <optional>
#include "cereal/messaging/messaging.h"


namespace qcpilot {
namespace cufu {

CuFuD::CuFuD(const cereal::CarParams::Reader &carParams) :
    // carParams_ {carParams},
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
    })},
    carRecognizedEvaluator_ {carParams},
    carSpeedEvaluator_ {carStateReaderOpt_},
    canValidEvaluator_ {carStateReaderOpt_} {
    assert(carStateSockPtr_ != nullptr);
    carStateSockPtr_->setTimeout(20);
    assert(subMasterPtr_ != nullptr);
    carStateReaderOpt_.reset();
}

void CuFuD::step() {
    updateInput();
    updateEvaluators();
    consolicateResult();
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

void CuFuD::updateEvaluators() {
    carRecognizedEvaluator_.update();
    carSpeedEvaluator_.update();
}

void CuFuD::consolicateResult() {
    bool longitudinalEnabled = true;
    longitudinalEnabled = carRecognizedEvaluator_.isSatisfied() &&
                          carSpeedEvaluator_.isSatisfied() && canValidEvaluator_.isSatisfied();

    std::printf("long: %d  ", longitudinalEnabled);
    std::vector<bool> evaresult;
    evaresult.push_back(carRecognizedEvaluator_.isSatisfied());
    evaresult.push_back(carSpeedEvaluator_.isSatisfied());
    evaresult.push_back(canValidEvaluator_.isSatisfied());
    for (const bool b : evaresult) {
        std::printf("%d ", b);
    }
    std::printf("\r\n");
}


}    // namespace cufu
}    // namespace qcpilot