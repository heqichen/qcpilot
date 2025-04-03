#ifndef __QCPILOT_CUFUD_EVALUATORS_POSENET_EVALUATOR_H__
#define __QCPILOT_CUFUD_EVALUATORS_POSENET_EVALUATOR_H__

#include <optional>
#include "cereal/messaging/messaging.h"
#include "openpilot/qcpilot/cufud/evaluators/evaluator.h"


namespace qcpilot {
namespace cufu {
namespace evaluators {

class PosenetEvaluator : public Evaluator {
  public:
    PosenetEvaluator(const std::optional<cereal::LivePose::Reader> &livePoseOpt) :
        livePoseOpt_ {livePoseOpt} {}

    inline virtual void update() override {
        if (livePoseOpt_.has_value()) {
            isSatisfied_ = livePoseOpt_->getPosenetOK() && livePoseOpt_->getInputsOK();
        }
    }

  private:
    const std::optional<cereal::LivePose::Reader> &livePoseOpt_;
};

}    // namespace evaluators
}    // namespace cufu
}    // namespace qcpilot

#endif
