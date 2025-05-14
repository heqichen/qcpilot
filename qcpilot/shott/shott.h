#ifndef __OPENPILOT_QCPILOT_SHOTT_SHOTT_H__
#define __OPENPILOT_QCPILOT_SHOTT_SHOTT_H__

#include <arpa/inet.h>
#include <net/if.h>

namespace qcpilot {

class Shott {
  public:
    Shott();
    void rebindNetwork();
    void step();

  private:
    int socketFd_ {-1};
    struct sockaddr_in mcastAddr_ {};
};

}    // namespace qcpilot

#endif
