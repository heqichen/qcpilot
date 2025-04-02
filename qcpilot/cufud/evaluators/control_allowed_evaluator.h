#ifndef __QCPILOT_CUFUD_EVALUATORS_CONTROL_ALLOWED_EVALUATOR_H__
#define __QCPILOT_CUFUD_EVALUATORS_CONTROL_ALLOWED_EVALUATOR_H__

#include <optional>
#include "cereal/messaging/messaging.h"
#include "openpilot/qcpilot/cufud/evaluators/evaluator.h"


namespace qcpilot {
namespace cufu {
namespace evaluators {

class ControlAllowedEvaluator : public Evaluator {
  public:
    ControlAllowedEvaluator(
      const bool &isControlling,
      const std::optional<capnp::List<cereal::PandaState, capnp::Kind::STRUCT>::Reader>
        &pandaStatesReaderOpt) :
        isControlling_ {isControlling},
        pandaStatesReaderOpt_ {pandaStatesReaderOpt},
        controlDisallowCount_ {0U},
        signalLostCount_ {0U} {}

    inline virtual void update() override {
        // 1. When controlling enabled, allow 2 frame controls allow failed
        // 2. frame cannot missing
        if (pandaStatesReaderOpt_.has_value()) {
            signalLostCount_ = 0U;
            const bool arePandasAllowControl = allPandaControlAllowed();
            if (isControlling_) {
                if (arePandasAllowControl) {
                    isSatisfied_ = true;
                    controlDisallowCount_ = 0U;
                } else {
                    controlDisallowCount_++;
                    if (controlDisallowCount_ >= 2U) {
                        isSatisfied_ = false;
                    }
                }
            } else {
                // If not controlling, allowsControl always false
                isSatisfied_ = arePandasAllowControl;
                controlDisallowCount_ = 0U;
            }
        } else {
            signalLostCount_++;
        }

        if (signalLostCount_ > 20U) {    // PandaStates runs at 10Hz
            isSatisfied_ = false;
        }
    }

  private:
    const bool &isControlling_;
    const std::optional<capnp::List<cereal::PandaState, capnp::Kind::STRUCT>::Reader>
      &pandaStatesReaderOpt_;

    std::size_t controlDisallowCount_ {0U};
    std::size_t signalLostCount_ {0U};

    inline bool allPandaControlAllowed() {
        bool result = true;
        for (std::size_t i {0U}; i < pandaStatesReaderOpt_->size(); ++i) {
            result &= pandaStatesReaderOpt_.value()[i].getControlsAllowed();
        }
        return result;
    }
};
}    // namespace evaluators
}    // namespace cufu
}    // namespace qcpilot

#endif
