#ifndef __QCPILOT_CUFUD_EVALUATORS_CAN_VALID_EVALUATOR_H__
#define __QCPILOT_CUFUD_EVALUATORS_CAN_VALID_EVALUATOR_H__

#include <optional>
#include "cereal/messaging/messaging.h"
#include "qcpilot/cufud/evaluators/evaluator.h"


namespace qcpilot {
namespace cufu {
namespace evaluators {

class CanValidEvaluator : public Evaluator {
  public:
    CanValidEvaluator(const std::optional<cereal::CarState::Reader> &carStateReaderOpt) :
        carStateReaderOpt_ {carStateReaderOpt} {}

    inline virtual void update() override {
        if (carStateReaderOpt_.has_value()) {
            isSatisfied_ = carStateReaderOpt_->getCanValid();
        } else {
            isSatisfied_ = false;
        }
    }


  private:
    const std::optional<cereal::CarState::Reader> &carStateReaderOpt_;
};
}    // namespace evaluators
}    // namespace cufu
}    // namespace qcpilot

#endif
