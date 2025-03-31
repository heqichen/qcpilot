#ifndef __QCPILOT_CUFUD_EVALUATORS_PANDA_SAFETY_CONFIG_EVALUATOR_H__
#define __QCPILOT_CUFUD_EVALUATORS_PANDA_SAFETY_CONFIG_EVALUATOR_H__

#include <cstdint>
#include <optional>
#include "cereal/messaging/messaging.h"
#include "openpilot/qcpilot/cufud/evaluators/evaluator.h"


namespace qcpilot {
namespace cufu {
namespace evaluators {

class PandaSafetyConfigEvaluator : public Evaluator {
  public:
    PandaSafetyConfigEvaluator(
      const cereal::CarParams::Reader &carParams,
      const std::optional<capnp::List<cereal::PandaState, capnp::Kind::STRUCT>::Reader>
        &pandaStatesReaderOpt) :
        carParams_ {carParams},
        pandaStatesReaderOpt_ {pandaStatesReaderOpt},
        safetyConfigMatched_ {false},
        safetyConfigMismatchCount_ {0U},
        signalLostCount_ {0U} {}

    inline virtual void update() override {
        if (pandaStatesReaderOpt_.has_value()) {
            signalLostCount_ = 0U;
            std::size_t pandaSefetyConfigLen = pandaStatesReaderOpt_->size();
            std::size_t carParamConfigLen = carParams_.getSafetyConfigs().size();

            safetyConfigMatched_ = true;
            if (pandaSefetyConfigLen < carParamConfigLen) {
                safetyConfigMatched_ = false;
            } else {
                for (std::size_t i {0U}; i < pandaSefetyConfigLen; ++i) {
                    // const auto & (pandaStatesReaderOpt_.value())[i].
                    safetyConfigMatched_ &= pandaStatesReaderOpt_.value()[i].getSafetyModel() ==
                                            carParams_.getSafetyConfigs()[i].getSafetyModel();
                    safetyConfigMatched_ &= pandaStatesReaderOpt_.value()[i].getSafetyParam() ==
                                            carParams_.getSafetyConfigs()[i].getSafetyParam();
                    safetyConfigMatched_ &=
                      pandaStatesReaderOpt_.value()[i].getAlternativeExperience() ==
                      carParams_.getAlternativeExperience();
                }
                for (std::size_t i {pandaSefetyConfigLen}; i < carParamConfigLen; ++i) {
                    const cereal::CarParams::SafetyModel &model =
                      pandaStatesReaderOpt_.value()[i].getSafetyModel();
                    safetyConfigMatched_ &= (model == cereal::CarParams::SafetyModel::NO_OUTPUT) ||
                                            (model == cereal::CarParams::SafetyModel::SILENT);
                }
            }

            if (safetyConfigMatched_) {
                safetyConfigMismatchCount_ = 0U;
            }
        }
        safetyConfigMismatchCount_++;
        signalLostCount_++;

        if (!safetyConfigMatched_ && (safetyConfigMismatchCount_ >= 1000U)) {    // 10 Seconds
            isSatisfied_ = false;
        } else {
            isSatisfied_ = true;
        }
        if (signalLostCount_ > 200U) {
            isSatisfied_ = false;
        }
    }

  private:
    const cereal::CarParams::Reader &carParams_;
    const std::optional<capnp::List<cereal::PandaState, capnp::Kind::STRUCT>::Reader>
      &pandaStatesReaderOpt_;

    bool safetyConfigMatched_ = false;
    std::size_t safetyConfigMismatchCount_ = 0U;
    std::size_t signalLostCount_ = 0U;
};
}    // namespace evaluators
}    // namespace cufu
}    // namespace qcpilot

#endif
