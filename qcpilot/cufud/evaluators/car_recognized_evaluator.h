#ifndef __QCPILOT_CUFUD_EVALUATORS_CAR_RECOGNIZED_EVALUATOR_H__
#define __QCPILOT_CUFUD_EVALUATORS_CAR_RECOGNIZED_EVALUATOR_H__

#include "cereal/messaging/messaging.h"
#include "openpilot/qcpilot/cufud/evaluators/evaluator.h"

namespace qcpilot {
namespace cufu {
namespace evaluators {

class CarRecognizedEvaluator : public Evaluator {
  public:
    CarRecognizedEvaluator(const cereal::CarParams::Reader &carParams) {
        isSatisfied_ = carParams.getBrand() != "mock";
    }
    inline virtual void update() override {}
};
}    // namespace evaluators
}    // namespace cufu
}    // namespace qcpilot

#endif
