#ifndef __QCPILOT_CUFUD_EVALUATORS_CONST_EVALUATOR_H__
#define __QCPILOT_CUFUD_EVALUATORS_CONST_EVALUATOR_H__

#include <optional>
#include "cereal/messaging/messaging.h"
#include "openpilot/qcpilot/cufud/evaluators/evaluator.h"


namespace qcpilot {
namespace cufu {
namespace evaluators {

class ConstEvaluator : public Evaluator {
  public:
    ConstEvaluator(const bool value) {
        isSatisfied_ = value;
    }
    inline virtual void update() override {}

  private:
};
}    // namespace evaluators
}    // namespace cufu
}    // namespace qcpilot

#endif
