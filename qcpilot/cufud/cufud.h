#ifndef __QCPILOT_CUFUD_CUFUD_H__
#define __QCPILOT_CUFUD_CUFUD_H__

#include <memory>
#include <optional>
#include "cereal/messaging/messaging.h"
#include "qcpilot/cufud/evaluators/car_recognized_evaluator.h"

namespace qcpilot {
namespace cufu {

class CuFuD {
  public:
    CuFuD(const cereal::CarParams::Reader &carParams);
    void step();
    void foo();

  private:
    void updateInput();
    void updateEvaluators();

    // const cereal::CarParams::Reader &carParams_;

    std::unique_ptr<Context> contextPtr_;
    std::unique_ptr<SubSocket> carStateSockPtr_;
    AlignedBuffer carStateBuf_;
    std::unique_ptr<SubMaster> subMasterPtr_;

    std::optional<cereal::CarState::Reader> carStateReaderOpt_;

    evaluators::CarRecognizedEvaluator carRecognizedEvaluator_;
};

}    // namespace cufu
}    // namespace qcpilot

#endif