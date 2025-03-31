#ifndef __QCPILOT_CUFUD_CUFUD_H__
#define __QCPILOT_CUFUD_CUFUD_H__

#include <array>
#include <memory>
#include <optional>
#include <tuple>
#include "cereal/messaging/messaging.h"
#include "openpilot/qcpilot/cufud/evaluators/can_valid_evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/car_recognized_evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/car_speed_evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/const_evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/resource_evaluator.h"

namespace qcpilot {
namespace cufu {

class CuFuD {
  public:
    CuFuD(const cereal::CarParams::Reader &carParams);
    void step();

  private:
    void updateInput();
    void updateEvaluators();
    void consolicateResult();

    // const cereal::CarParams::Reader &carParams_;

    std::unique_ptr<Context> contextPtr_;
    std::unique_ptr<SubSocket> carStateSockPtr_;
    AlignedBuffer carStateBuf_;
    std::unique_ptr<SubMaster> subMasterPtr_;

    std::optional<cereal::CarState::Reader> carStateReaderOpt_;
    std::optional<cereal::DeviceState::Reader> deviceStateReaderOpt_;

    evaluators::ConstEvaluator carRecognizedEvaluator_;
    evaluators::ConstEvaluator onCarEvaluator_;
    evaluators::CarSpeedEvaluator carSpeedEvaluator_;
    evaluators::CanValidEvaluator canValidEvaluator_;
    evaluators::ResourceEvaluator resourceEvaluator_;
    std::array<evaluators::Evaluator *, 5U> evaluators_;
};

}    // namespace cufu
}    // namespace qcpilot

#endif