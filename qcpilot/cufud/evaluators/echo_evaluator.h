#ifndef __QCPILOT_CUFUD_EVALUATORS_ECHO_EVALUATOR_H__
#define __QCPILOT_CUFUD_EVALUATORS_ECHO_EVALUATOR_H__

#include <optional>
#include "openpilot/qcpilot/cufud/evaluators/evaluator.h"


namespace qcpilot {
namespace cufu {
namespace evaluators {

class EchoEvaluator : public Evaluator {
  public:
    EchoEvaluator(const bool &value) : value_ {value} {}

    inline virtual void update() override {
        isSatisfied_ = value_;
    }

  private:
    const bool &value_;
};
}    // namespace evaluators
}    // namespace cufu
}    // namespace qcpilot

#endif
