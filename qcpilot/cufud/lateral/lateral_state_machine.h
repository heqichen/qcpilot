#ifndef __OPENPILOT_QCPILOT_LATERAL_LATERAL_STATE_MACHINE_H__
#define __OPENPILOT_QCPILOT_LATERAL_LATERAL_STATE_MACHINE_H__

#include "openpilot/qcpilot/cufud/data/data.h"

namespace qcpilot {
namespace cufu {

class LateralStateMachine {
  public:
    LateralStateMachine(const data::QualifiedData<data::VehicleState> &vehicleState) :
        vehicleState_ {vehicleState} {}
    void step() {
        if (vehicleState_.isQualified()) {
            bool buttonPressed = vehicleState_.data().buttonState.lkasButton;
            if (buttonPressed && !previousButtonPressed_) {
                isLateralActive_ = !isLateralActive_;
            }
            previousButtonPressed_ = buttonPressed;
        }
    }
    bool isLateralActive() const {
        return isLateralActive_;
    }

  private:
    const data::QualifiedData<data::VehicleState> &vehicleState_;
    bool previousButtonPressed_ {false};
    bool isLateralActive_ {false};
};

}    // namespace cufu
}    // namespace qcpilot
#endif
