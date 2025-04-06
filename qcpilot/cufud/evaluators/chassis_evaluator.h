#ifndef __QCPILOT_CUFUD_EVALUATORS_CHASSIS_EVALUATOR_H__
#define __QCPILOT_CUFUD_EVALUATORS_CHASSIS_EVALUATOR_H__

#include <optional>
#include "cereal/messaging/messaging.h"
#include "openpilot/qcpilot/cufud/evaluators/evaluator.h"


namespace qcpilot {
namespace cufu {
namespace evaluators {

class ChassisEvaluator : public Evaluator {
  public:
    ChassisEvaluator(const std::optional<cereal::CarState::Reader> &carStateReaderOpt) :
        carStateReaderOpt_ {carStateReaderOpt} {}

    inline virtual void update() override {
        if (carStateReaderOpt_.has_value()) {
            bool isCruiseAvailable = false;
            if (carStateReaderOpt_->hasCruiseState()) {
                // CRZ_AVAILABLE
                isCruiseAvailable = carStateReaderOpt_->getCruiseState().getAvailable();
            }
            const bool isLkasBlocked = carStateReaderOpt_->getGenericToggle();
            isSatisfied_ = isCruiseAvailable && (!isLkasBlocked);
        }
    }

  private:
    const std::optional<cereal::CarState::Reader> &carStateReaderOpt_;
};
}    // namespace evaluators
}    // namespace cufu
}    // namespace qcpilot

#endif
