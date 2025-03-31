#ifndef __QCPILOT_CUFUD_EVALUATORS_EVALUATOR_H__
#define __QCPILOT_CUFUD_EVALUATORS_EVALUATOR_H__

#include <optional>

namespace qcpilot {
namespace cufu {
namespace evaluators {


class Evaluator {
  protected:
    Evaluator() : isSatisfied_ {false} {}
    bool isSatisfied_ = false;

  public:
    virtual ~Evaluator() = default;

    inline virtual void update() = 0;
    inline bool isSatisfied() const {
        return isSatisfied_;
    }
};


}    // namespace evaluators
}    // namespace cufu
}    // namespace qcpilot

#endif