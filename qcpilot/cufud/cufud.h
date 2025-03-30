#ifndef __QCPILOT_CUFUD_CUFUD_H__
#define __QCPILOT_CUFUD_CUFUD_H__

#include <memory>
#include <optional>
#include "cereal/messaging/messaging.h"

namespace qcpilot {

class CuFuD {
  public:
    CuFuD();
    void step();
    void foo();

  private:
    void updateInput();

    std::unique_ptr<Context> contextPtr_;
    std::unique_ptr<SubSocket> carStateSockPtr_;
    AlignedBuffer carStateBuf_;
    std::unique_ptr<SubMaster> subMasterPtr_;

    std::optional<cereal::CarState::Reader> carStateReaderOpt_;
};

}    // namespace qcpilot

#endif