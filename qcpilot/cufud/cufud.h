#ifndef __QCPILOT_CUFUD_CUFUD_H__
#define __QCPILOT_CUFUD_CUFUD_H__

#include <array>
#include <memory>
#include <optional>
#include "cereal/messaging/messaging.h"
#include "openpilot/qcpilot/cufud/evaluators/can_valid_evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/car_recognized_evaluator.h"
#include "openpilot/qcpilot/cufud/evaluators/car_speed_evaluator.h"
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

    const bool isCarRecognized_ = false;
    const bool isOnCar_ = false;
    // std::array<std::unique_ptr<evaluators::Evaluator>, 2U> evaluators_;
    // evaluators::CarRecognizedEvaluator carRecognizedEvaluator_;
    evaluators::CarSpeedEvaluator carSpeedEvaluator_;
    evaluators::CanValidEvaluator canValidEvaluator_;
    evaluators::ResourceEvaluator resourceEvaluator_;
};

}    // namespace cufu
}    // namespace qcpilot

#endif