#include "openpilot/qcpilot/shott/shott.h"
#include <cstdio>
#include <cstring>
#include <sstream>
#include <vector>
#include "cereal/archives/portable_binary.hpp"
#include "openpilot/qcpilot/shott/data_def.h"

namespace qcpilot {
namespace shott {

const std::vector<const char *> kSignals {"qcMazdaState", "carState", "deviceState"};
#define MCAST_PORT 1900
#define MCAST_ADDR "239.238.237.236"

Shott::Shott() : rateKeeper_ {"shott", 50}, isMyselfNotLagging_ {false}, sm_ {kSignals} {
    // Create socket
    socketFd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFd_ == -1) {
        std::fprintf(stderr, "Cannot create socket\r\n");
        std::exit(-1);
    }
    // Setup target address
    std::memset(&mcastAddr_, 0x00, sizeof(mcastAddr_));
    mcastAddr_.sin_family = AF_INET;
    mcastAddr_.sin_addr.s_addr = inet_addr(MCAST_ADDR);
    mcastAddr_.sin_port = htons(MCAST_PORT);
}

void Shott::loop() {
    while (true) {
        step();
        isMyselfNotLagging_ = !rateKeeper_.keepTime();
    }
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

    const ssize_t sentSize = sendto(socketFd_,
                                    oss.str().data(),
                                    oss.str().size(),
                                    0,
                                    (struct sockaddr *)&mcastAddr_,
                                    sizeof(mcastAddr_));
    if (sentSize < 0) {
        std::fprintf(stderr, "Failed to send data\r\n");
        std::exit(-2);
    }
}

}    // namespace shott
}    // namespace qcpilot
