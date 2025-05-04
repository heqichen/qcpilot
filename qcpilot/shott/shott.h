#ifndef __QCPILOT_SHOTT_SHOTT_H__
#define __QCPILOT_SHOTT_SHOTT_H__

#include <memory>
#include "cereal/messaging/messaging.h"
#include "openpilot/common/ratekeeper.h"

namespace qcpilot {
namespace shott {

class Shott {
  public:
    Shott();
    void loop();

  private:
    RateKeeper rateKeeper_;
    bool isMyselfNotLagging_ {false};
    SubMaster sm_;
    std::unique_ptr<SubMaster> subMasterPtr_;

    void step();
};


}    // namespace shott

}    // namespace qcpilot

#endif
