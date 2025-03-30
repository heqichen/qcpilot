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
        deviceStateReaderOpt_ {deviceStateReaderOpt} {}

    inline virtual void update() override {
        isSatisfied_ = false;
    }


  private:
    const std::optional<cereal::PeripheralState::Reader> &peripheralStateReaderOpt_;
    const std::optional<cereal::DeviceState::Reader> &deviceStateReaderOpt_;
};
}    // namespace evaluators
}    // namespace cufu
}    // namespace qcpilot

#endif
