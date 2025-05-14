#include "openpilot/qcpilot/shott/shott.h"
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include "cereal/archives/portable_binary.hpp"
#include "cereal/messaging/messaging.h"
#include "openpilot/qcpilot/shott/data_def.h"

namespace qcpilot {

namespace {

const std::vector<const char *> kSignals {"qcMazdaState", "carState", "deviceState"};
#define MCAST_PORT 1900
#define MCAST_ADDR "239.238.237.236"

}    // namespace


Shott::Shott() : socketFd_ {-1}, mcastAddr_ {}, frame_ {}, sm_ {kSignals} {
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

void Shott::step() {
    // Reset memory
    std::memset(&frame_, 0x00, sizeof(frame_));


    // update message
    sm_.update(0);
    if (sm_.updated("qcMazdaState")) {
        frame_.engineRpm = sm_["qcMazdaState"].getQcMazdaState().getEngineRpm();
        // process message
    }


    // publish frame data
    std::ostringstream oss(std::ios::binary);
    {
        cereal::PortableBinaryOutputArchive archive(oss);
        archive(frame_);
    }
    std::cout << "size: " << oss.str().size() << std::endl;
    errno = 0;
    const ssize_t sentSize =
      sendto(socketFd_, oss.str().data(), oss.str().size(), 0, (struct sockaddr *)&mcastAddr_, sizeof(mcastAddr_));
    if (sentSize < 0) {
        std::fprintf(stderr, "Failed to send data to %d, errno=%d\r\n", socketFd_, errno);
        std::exit(-2);
    }
}
}    // namespace qcpilot