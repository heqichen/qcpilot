#ifndef __QCPILOT_CUFUD_EVALUATORS_INIT_TIMEOUT_EVALUATOR_H__
#define __QCPILOT_CUFUD_EVALUATORS_INIT_TIMEOUT_EVALUATOR_H__

#include <cstdint>
#include "cereal/messaging/messaging.h"
#include "openpilot/qcpilot/cufud/evaluators/evaluator.h"


namespace qcpilot {
namespace cufu {
namespace evaluators {

class InitTimeoutEvaluator : public Evaluator {
  public:
    InitTimeoutEvaluator(const std::optional<cereal::CarState::Reader>& carStateOpt) :
        carStateOpt_ {carStateOpt},
        count_ {0U} {}

    inline virtual void update() override {
        // Call me at 100hz, wait 6seconds to start
        if (carStateOpt_.has_value()) {
            count_++;
            isSatisfied_ = count_ > 600U;
        }
    }

  private:
    const std::optional<cereal::CarState::Reader>& carStateOpt_;
    std::size_t count_ {0U};
};
}    // namespace evaluators
}    // namespace cufu
}    // namespace qcpilot

#endif
