#ifndef __QCPILOT_CUFUD_EVALUATORS_CAR_SPEED_EVALUATOR_H__
#define __QCPILOT_CUFUD_EVALUATORS_CAR_SPEED_EVALUATOR_H__

#include <optional>
#include "cereal/messaging/messaging.h"
#include "openpilot/qcpilot/cufud/data/data.h"
#include "openpilot/qcpilot/cufud/evaluators/evaluator.h"

namespace qcpilot {
namespace cufu {
namespace evaluators {

class CarSpeedEvaluator : public Evaluator {
  public:
    CarSpeedEvaluator(const data::QualifiedData<data::MotionState> &motionState) :
        motionState_ {motionState} {}

    inline virtual void update() override {
        if (motionState_.isQualified()) {
            isSatisfied_ = motionState_.data().speed > 0.1F;
        } else {
            isSatisfied_ = false;
        }
    }

  private:
    const data::QualifiedData<data::MotionState> &motionState_;
    ;
};
}    // namespace evaluators
}    // namespace cufu
}    // namespace qcpilot

#endif
