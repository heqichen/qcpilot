#ifndef __QCPILOT_CUFUD_EVALUATORS_HARDWARE_EVALUATOR_H__
#define __QCPILOT_CUFUD_EVALUATORS_HARDWARE_EVALUATOR_H__

#include <optional>
#include "cereal/messaging/messaging.h"
#include "openpilot/qcpilot/cufud/evaluators/evaluator.h"


namespace qcpilot {
namespace cufu {
namespace evaluators {

class HardwareEvaluator : public Evaluator {
  public:
    HardwareEvaluator(
      const std::optional<cereal::PeripheralState::Reader> &peripheralStateReaderOpt,
      const std::optional<cereal::DeviceState::Reader> &deviceStateReaderOpt) :
        peripheralStateReaderOpt_ {peripheralStateReaderOpt},
        deviceStateReaderOpt_ {deviceStateReaderOpt},
        peripheralStateHasData_ {false},
        deviceStateHasData_ {false},
        lostPeripheralStateCount_ {0U},
        lostDeviceStateCount_ {0U},
        fanRpm_ {0U},
        desiredFan_ {0U},
        fanStuckCount_ {0U} {
        isSatisfied_ = false;
    }


    inline virtual void update() override {
        if (peripheralStateReaderOpt_.has_value()) {
            peripheralStateHasData_ = true;
            lostPeripheralStateCount_ = 0U;
            fanRpm_ = peripheralStateReaderOpt_->getFanSpeedRpm();
        } else {
            lostPeripheralStateCount_++;
        }
        if (deviceStateReaderOpt_.has_value()) {
            deviceStateHasData_ = true;
            lostDeviceStateCount_ = 0U;
            desiredFan_ = deviceStateReaderOpt_->getFanSpeedPercentDesired();
        } else {
            lostDeviceStateCount_++;
        }

        if (peripheralStateHasData_ && deviceStateHasData_) {
            if ((lostPeripheralStateCount_ > 100U) || (lostDeviceStateCount_ > 100U)) {
                // Signal is missing
                isSatisfied_ = false;
            } else {
                const cereal::PandaState::PandaType &pandaType {
                  peripheralStateReaderOpt_->getPandaType()};
                if (pandaType == cereal::PandaState::PandaType::UNKNOWN) {
                    // unsupported panda type
                    isSatisfied_ = true;
                } else {
                    bool fanStuck = (desiredFan_ > 50U) && (fanRpm_ < 500U);
                    if (fanStuck) {
                        if (isFanStuck_) {
                            fanStuckCount_++;
                            // already stuck
                            if (fanStuckCount_ > 500U) {    // 5 Seconds
                                isSatisfied_ = false;
                            }
                        } else {
                            // start count, raise fanStuck
                            isFanStuck_ = true;
                            fanStuckCount_ = 1U;
                            // Satified not changed
                        }
                    } else {
                        isFanStuck_ = false;
                        isSatisfied_ = true;
                    }
                }
            }
        }
    }


  private:
    const std::optional<cereal::PeripheralState::Reader> &peripheralStateReaderOpt_;
    const std::optional<cereal::DeviceState::Reader> &deviceStateReaderOpt_;

    bool peripheralStateHasData_ = false;
    bool deviceStateHasData_ = false;
    bool isFanStuck_ = false;
    std::size_t lostPeripheralStateCount_ = 0U;
    std::size_t lostDeviceStateCount_ = 0U;
    std::size_t fanRpm_ = 0U;
    std::size_t desiredFan_ = 0U;
    std::size_t fanStuckCount_ = 0U;
};
}    // namespace evaluators
}    // namespace cufu
}    // namespace qcpilot

#endif
