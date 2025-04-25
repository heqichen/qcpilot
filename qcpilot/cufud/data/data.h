#ifndef __OPENPILOT_QCPILOT_CUFUD_DATA_DATA_H__
#define __OPENPILOT_QCPILOT_CUFUD_DATA_DATA_H__


namespace qcpilot {
namespace data {

constexpr float KPH_TO_MPS {1000.0F / 3600.0F};


struct MotionState {
    float speed {0.0F};    // m/s
};

struct AdasState {
    bool isCruiseAvailable {false};
    bool isCruiseActive {false};
    bool isAccActive {false};
};

struct ButtonState {
    bool cruiseButton {false};
    bool lkasButton {false};
};

struct VehicleState {
    struct MotionState motionState {};
    struct AdasState adasState {};
    struct ButtonState buttonState {};
};

template<typename T>
class QualifiedData {
  public:
    void disqualify() {
        isQualified_ = false;
    }
    void qualify() {
        isQualified_ = true;
    }
    bool isQualified() const {
        return isQualified_;
    }
    T &data() {
        return data_;
    }
    const T &data() const {
        return data_;
    }

  private:
    T data_ {};
    bool isQualified_ {false};
};

}    // namespace data
}    // namespace qcpilot

#endif
