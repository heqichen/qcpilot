#include <arpa/inet.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <net/if.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MCAST_ADDR "239.238.237.236"
#define MCAST_PORT 1900

class Toohs {
public:
  Toohs() {
    socketFd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFd_ < 0) {
      std::fprintf(stderr, "Cannot create socket\r\n");
      std::exit(-1);
    }

    std::memset(&localAddr_, 0, sizeof(localAddr_));
    localAddr_.sin_family = AF_INET;
    localAddr_.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr_.sin_port = htons(MCAST_PORT);

    int err = 0;

    // Allow multiple receivers
    int yes = 1;
    err = setsockopt(socketFd_, SOL_SOCKET, SO_REUSEADDR, (void *)&yes,
                     sizeof(yes));
    if (err < 0) {
      std::fprintf(stderr, "Reuse Address failed\r\n");
      std::exit(-2);
    }

    err = bind(socketFd_, (struct sockaddr *)&localAddr_, sizeof(localAddr_));
    if (err < 0) {
      std::fprintf(stderr, "Cannot bind socket\r\n");
      std::exit(-2);
    }

    // enable loopback for testing
    int loop = 1;
    err = setsockopt(socketFd_, IPPROTO_IP, IP_MULTICAST_LOOP, &loop,
                     sizeof(loop));
    if (err < 0) {
      std::fprintf(stderr, "Cannot set IP_MULTICAST_LOOP\r\n");
      std::exit(-3);
    }

    struct ip_mreq mreq {};
    std::memset(&mreq, 0, sizeof(mreq));
    mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    err = setsockopt(socketFd_, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq,
                     sizeof(mreq));
    if (err < 0) {
      std::fprintf(stderr, "Register multicast address\r\n");
      std::exit(-4);
    }
  }

  void receive() {
    socklen_t socketAddrLen = sizeof(localAddr_);
    std::uint8_t buffer[256];
    std::memset(buffer, 0x00, sizeof(buffer));

    const ssize_t receiveByte =
        recvfrom(socketFd_, buffer, 256, 0, (struct sockaddr *)&localAddr_,
                 &socketAddrLen);
    if (receiveByte < 0) {
      std::fprintf(stderr, "Receive Error!\r\n");
      std::exit(-5);
    }
    if (receiveByte > 0) {
      std::printf("received: %d[%s]\r\n", receiveByte, buffer);
    } else {
      std::printf("no data \r\n");
    }
  }

private:
  int socketFd_{-1};
  struct sockaddr_in localAddr_ {};
};

int main(int argc, char *argv[], const char *envs[]) {
  Toohs t;
  while (true) {
    // sleep();
    t.receive();
  }
  return 0;
}
