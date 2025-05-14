#include "openpilot/qcpilot/shott/shott.h"
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace qcpilot {


#define MCAST_PORT 1900
#define MCAST_ADDR "239.238.237.236"

Shott::Shott() : socketFd_ {-1}, mcastAddr_ {} {
    // Create socket
    socketFd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFd_ == -1) {
        std::fprintf(stderr, "Cannot create socket\r\n");
    }

    // Setup target address
    std::memset(&mcastAddr_, 0x00, sizeof(mcastAddr_));
    mcastAddr_.sin_family = AF_INET;
    mcastAddr_.sin_addr.s_addr = inet_addr(MCAST_ADDR);
    mcastAddr_.sin_port = htons(MCAST_PORT);
}

void Shott::rebindNetwork() {
    if (socketFd_ != -1) {
        close(socketFd_);
        socketFd_ = -1;
    }
    socketFd_ = socket(AF_INET, SOCK_DGRAM, 0);
}

void Shott::step() {}
}    // namespace qcpilot