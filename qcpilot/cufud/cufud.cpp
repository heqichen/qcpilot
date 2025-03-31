#include "openpilot/qcpilot/cufud/cufud.h"
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <optional>
#include "cereal/messaging/messaging.h"
#include "openpilot/qcpilot/cufud/evaluators/calibrated_evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/can_valid_evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/car_recognized_evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/car_speed_evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/const_evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/hardware_evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/resource_evaluator.h"


namespace qcpilot {
namespace cufu {

CuFuD::CuFuD(const cereal::CarParams::Reader &carParams) :
    // carParams_ {carParams},
    contextPtr_ {Context::create()},
    carStateSockPtr_ {SubSocket::create(contextPtr_.get(), "carState")},
    subMasterPtr_ {std::make_unique<SubMaster>(std::vector<const char *> {
      "deviceState",
      "peripheralState",
      "liveCalibration",

      "modelV2",
      "controlsState",
      "radarState",
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
    carRecognizedEvaluator_ {carParams.getBrand() != "mock"},
    onCarEvaluator_ {!carParams.getNotCar()},
    carSpeedEvaluator_ {carStateReaderOpt_},
    canValidEvaluator_ {carStateReaderOpt_},
    resourceEvaluator_ {deviceStateReaderOpt_},
    hardwareEvaluator_ {peripheralStateOpt_, deviceStateReaderOpt_},
    calibratedEvaluator_ {liveCalibrationOpt_},
    evaluators_ {&carRecognizedEvaluator_,
                 &onCarEvaluator_,
                 &carSpeedEvaluator_,
                 &canValidEvaluator_,
                 &resourceEvaluator_,
                 &hardwareEvaluator_,
                 &calibratedEvaluator_} {
    assert(carStateSockPtr_ != nullptr);
    carStateSockPtr_->setTimeout(20);
    assert(subMasterPtr_ != nullptr);
    carStateReaderOpt_.reset();
    deviceStateReaderOpt_.reset();
    peripheralStateOpt_.reset();
    liveCalibrationOpt_.reset();
}

void CuFuD::step() {
    updateInput();
    updateEvaluators();
    consolidateResult();
}


void CuFuD::updateInput() {
    // Clear previous input
    carStateReaderOpt_.reset();
    deviceStateReaderOpt_.reset();
    peripheralStateOpt_.reset();
    liveCalibrationOpt_.reset();

    // Wait/Block for carState
    std::unique_ptr<Message> msg {carStateSockPtr_->receive(false)};
    subMasterPtr_->update(0);
    if (msg) {
        capnp::FlatArrayMessageReader msgReader(carStateBuf_.align(msg.get()));
        cereal::Event::Reader event = msgReader.getRoot<cereal::Event>();
        carStateReaderOpt_ = event.getCarState();

        if (subMasterPtr_->updated("deviceState")) {
            deviceStateReaderOpt_ = (*subMasterPtr_)["deviceState"].getDeviceState();
        }
        if (subMasterPtr_->updated("peripheralState")) {
            peripheralStateOpt_ = (*subMasterPtr_)["peripheralState"].getPeripheralState();
        }
        if (subMasterPtr_->updated("liveCalibration")) {
            liveCalibrationOpt_ = (*subMasterPtr_)["liveCalibration"].getLiveCalibration();
        }
    }
}

void CuFuD::updateEvaluators() {
    for (auto &evaluator : evaluators_) {
        evaluator->update();
    }
}

void CuFuD::consolidateResult() {
    bool longitudinalEnabled = true;
    for (const auto &evaluator : evaluators_) {
        longitudinalEnabled &= evaluator->isSatisfied();
    }

    std::printf("long: %d  ", longitudinalEnabled);
    std::vector<bool> evaresult;
    for (auto &evaluator : evaluators_) {
        evaresult.push_back(evaluator->isSatisfied());
    }
    for (const bool b : evaresult) {
        std::printf("%d ", b);
    }
    std::printf("\r");
}


}    // namespace cufu
}    // namespace qcpilot