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
#include "openpilot/qcpilot/cufud/evaluators/control_allowed_evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/echo_evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/hardware_evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/panda_safety_config_evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/resource_evaluator.h"

namespace qcpilot {
namespace cufu {


const std::vector<const char *> kSignals = {
  "deviceState",
  "peripheralState",
  "liveCalibration",
  "pandaStates",

  "modelV2",
  "controlsState",
  "radarState",
  "carParams",
  "driverMonitoringState",
  "carState",
  "driverStateV2",
  "wideRoadCameraState",
  "managerState",
  "selfdriveState",
  "longitudinalPlan",
};

CuFuD::CuFuD(const cereal::CarParams::Reader &carParams) :
    // carParams_ {carParams},
    isControllingEnabled_ {false},
    isSignalHealthy_ {false},
    contextPtr_ {Context::create()},
    carStateSockPtr_ {SubSocket::create(contextPtr_.get(), "carState")},
    subMasterPtr_ {std::make_unique<SubMaster>(kSignals)},
    carRecognizedEvaluator_ {carParams.getBrand() != "mock"},
    onCarEvaluator_ {!carParams.getNotCar()},
    carSpeedEvaluator_ {carStateReaderOpt_},
    canValidEvaluator_ {carStateReaderOpt_},
    resourceEvaluator_ {deviceStateReaderOpt_},
    hardwareEvaluator_ {peripheralStateReaderOpt_, deviceStateReaderOpt_},
    calibratedEvaluator_ {liveCalibrationReaderOpt_},
    pandaSafetyConfigEvaluator_ {carParams, pandaStatesReaderOpt_},
    controlAllowedEvaluator_ {isControllingEnabled_, pandaStatesReaderOpt_},
    signalHealthyEvaluator_ {isSignalHealthy_},
    evaluators_ {&carRecognizedEvaluator_,
                 &onCarEvaluator_,
                 &carSpeedEvaluator_,
                 &canValidEvaluator_,
                 &resourceEvaluator_,
                 &hardwareEvaluator_,
                 &calibratedEvaluator_,
                 &pandaSafetyConfigEvaluator_,
                 &controlAllowedEvaluator_,
                 &signalHealthyEvaluator_} {
    assert(carStateSockPtr_ != nullptr);
    carStateSockPtr_->setTimeout(20);    // CarState runs at 100Hz
    assert(subMasterPtr_ != nullptr);
    carStateReaderOpt_.reset();
    deviceStateReaderOpt_.reset();
    peripheralStateReaderOpt_.reset();
    liveCalibrationReaderOpt_.reset();
    pandaStatesReaderOpt_.reset();
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
    peripheralStateReaderOpt_.reset();
    liveCalibrationReaderOpt_.reset();
    pandaStatesReaderOpt_.reset();

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
            peripheralStateReaderOpt_ = (*subMasterPtr_)["peripheralState"].getPeripheralState();
        }
        if (subMasterPtr_->updated("liveCalibration")) {
            liveCalibrationReaderOpt_ = (*subMasterPtr_)["liveCalibration"].getLiveCalibration();
        }
        if (subMasterPtr_->updated("pandaStates")) {
            pandaStatesReaderOpt_ = (*subMasterPtr_)["pandaStates"].getPandaStates();
        }
    }
}

void CuFuD::updateEvaluators() {
    isSignalHealthy_ = subMasterPtr_->allAliveAndValid();
    if (!isSignalHealthy_) {
        for (const char *signalName : kSignals) {
            if (!subMasterPtr_->alive(signalName)) {
                std::printf("%s not alive\r\n", signalName);
            }
            if (!subMasterPtr_->valid(signalName)) {
                std::printf("%s not valid\r\n", signalName);
            }
        }
    }
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

    isControllingEnabled_ = longitudinalEnabled;
}


}    // namespace cufu
}    // namespace qcpilot