#ifndef __QCPILOT_CUFUD_EVALUATORS_OVERHEAT_EVALUATOR_H__
#define __QCPILOT_CUFUD_EVALUATORS_OVERHEAT_EVALUATOR_H__

#include <optional>
#include "cereal/gen/cpp/log.capnp.h"
#include "cereal/messaging/messaging.h"
#include "openpilot/qcpilot/cufud/evaluators/evaluator.h"

namespace qcpilot {
namespace cufu {
namespace evaluators {

class ResourceEvaluator : public Evaluator {
  public:
    ResourceEvaluator(const std::optional<cereal::DeviceState::Reader> &deviceStateReaderOpt) :
        deviceStateReaderOpt_ {deviceStateReaderOpt},
        frameLostCount_ {0U},
        isLastThermalGood_ {false},
        isLastSpaceGood_ {false},
        isLastMemoryGood_ {false} {}

    inline virtual void update() override {
        if (deviceStateReaderOpt_.has_value()) {
            frameLostCount_ = 0U;
            isLastThermalGood_ =
              (deviceStateReaderOpt_->getThermalStatus() < cereal::DeviceState::ThermalStatus::RED);
            isLastSpaceGood_ = deviceStateReaderOpt_->getFreeSpacePercent() >= 7.0F;
            isLastMemoryGood_ = deviceStateReaderOpt_->getMemoryUsagePercent() <= 90U;
        } else {
            frameLostCount_++;
        }

        if (frameLostCount_ > 60U) {
            isSatisfied_ = false;
        } else {
            isSatisfied_ = isLastThermalGood_ && isLastSpaceGood_ && isLastMemoryGood_;
        }
    }


  private:
    const std::optional<cereal::DeviceState::Reader> &deviceStateReaderOpt_;
    std::size_t frameLostCount_ = 0U;
    bool isLastThermalGood_ = false;
    bool isLastSpaceGood_ = false;
    bool isLastMemoryGood_ = false;
};
}    // namespace evaluators
}    // namespace cufu
}    // namespace qcpilot

#endif
