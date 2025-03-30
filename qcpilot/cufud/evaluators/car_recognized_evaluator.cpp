#include "qcpilot/cufud/evaluators/car_recognized_evaluator.h"
#include "cereal/messaging/messaging.h"

namespace qcpilot {
namespace cufu {
namespace evaluators {

CarRecognizedEvaluator::CarRecognizedEvaluator(const cereal::CarParams::Reader &carParams) {
    isSatisfied_ = carParams.getBrand() != "mock";
}

}    // namespace evaluators
}    // namespace cufu
}    // namespace qcpilot