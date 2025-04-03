#ifndef __QCPILOT_CUFUD_EVALUATORS_RADAR_STATE_EVALUATOR_H__
#define __QCPILOT_CUFUD_EVALUATORS_RADAR_STATE_EVALUATOR_H__

#include <optional>
#include "cereal/messaging/messaging.h"
#include "openpilot/qcpilot/cufud/evaluators/evaluator.h"


namespace qcpilot {
namespace cufu {
namespace evaluators {

class RadarStateEvaluator : public Evaluator {
  public:
    RadarStateEvaluator(const std::optional<cereal::RadarState::Reader> &radarStateReaderOpt) :
        radarStateReaderOpt_ {radarStateReaderOpt} {}

    inline virtual void update() override {
        if (radarStateReaderOpt_.has_value()) {
            const bool hasError =
              radarStateReaderOpt_->getRadarErrors().getRadarUnavailableTemporary() ||
              radarStateReaderOpt_->getRadarErrors().getCanError() ||
              radarStateReaderOpt_->getRadarErrors().getRadarFault();
            isSatisfied_ = !hasError;
        }
    }

  private:
    const std::optional<cereal::RadarState::Reader> &radarStateReaderOpt_;
};
}    // namespace evaluators
}    // namespace cufu
}    // namespace qcpilot

#endif
