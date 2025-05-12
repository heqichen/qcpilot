#ifndef __QCPILOT_SHOTT_SHOTT_H__
#define __QCPILOT_SHOTT_SHOTT_H__

#include <arpa/inet.h>
#include <net/if.h>
#include <memory>
#include "cereal/messaging/messaging.h"
#include "openpilot/common/ratekeeper.h"
#include "openpilot/qcpilot/shott/data_def.h"

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

    struct ConsoleFrame frame_;

    int socketFd_ {-1};
    struct sockaddr_in mcastAddr_ {};

    void step();
};


}    // namespace shott

}    // namespace qcpilot

#endif
