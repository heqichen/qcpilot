#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <net/if.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MCAST_PORT 1900
#define MCAST_ADDR "239.238.237.236"
#define MCAST_DATA "Hello world???" /*多播發送的數據*/
#define MCAST_INTERVAL 5            /*發送間隔時間*/

class Shoot {
public:
  Shoot() {
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
  ~Shoot() { close(socketFd_); }

  void send(const char *str) {
    const ssize_t sentSize =
        sendto(socketFd_, str, std::strlen(str), 0,
               (struct sockaddr *)&mcastAddr_, sizeof(mcastAddr_));
    if (sentSize < 0) {
      std::fprintf(stderr, "Failed to send data\r\n");
      std::exit(-2);
    }
  }

private:
  int socketFd_{-1};
  struct sockaddr_in mcastAddr_ {};
};

int main(int argc, char *argv[], char **envs) {
  Shoot s{};

  while (true) {
    std::string str;
    std::cin >> str;
    s.send(str.c_str());
  }

  return 0;
}
