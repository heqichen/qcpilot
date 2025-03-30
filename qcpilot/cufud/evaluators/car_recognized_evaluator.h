#ifndef __QCPILOT_CUFUD_EVALUATORS_CAR_RECOGNIZED_EVALUATOR_H__
#define __QCPILOT_CUFUD_EVALUATORS_CAR_RECOGNIZED_EVALUATOR_H__

#include "cereal/messaging/messaging.h"


namespace qcpilot {
namespace cufu {
namespace evaluators {


class CarRecognizedEvaluator {
  public:
    CarRecognizedEvaluator(const cereal::CarParams::Reader &carParams);
    bool isSatisfied() const {
        return isSatisfied_;
    }

  private:
    bool isSatisfied_ = false;
};
}    // namespace evaluators
}    // namespace cufu
}    // namespace qcpilot

#endif
