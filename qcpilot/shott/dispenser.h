#ifndef __OPENPILOT_QCPILOT_SHOTT_SHOTT_H__
#define __OPENPILOT_QCPILOT_SHOTT_SHOTT_H__

#include <arpa/inet.h>
#include <net/if.h>
#include "cereal/messaging/messaging.h"
#include "openpilot/qcpilot/shott/data_def.h"

namespace cooboc {
namespace shott {

class Dispenser {
  public:
    Dispenser();
    void rebindNetwork();
    void step();

  private:
    int socketFd_ {-1};
    struct sockaddr_in mcastAddr_ {};
    struct ConsoleFrame frame_;
    SubMaster sm_;
};

}    // namespace shott
}    // namespace cooboc

#endif
