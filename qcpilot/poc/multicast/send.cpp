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
        sendto(socketFd_, str, std::strlen(str) + 1, 0,
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

class TcpShoot {
public:
  TcpShoot() {
    bzero((char *)&servAddr_, sizeof(servAddr_));
    servAddr_.sin_family = AF_INET;
    servAddr_.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr_.sin_port = htons(MCAST_PORT);

    // open stream oriented socket with internet address
    // also keep track of the socket descriptor
    serverFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd_ < 0) {
      std::fprintf(stderr, "Error establishing the server socket\r\n");
      std::exit(-1);
    }

    // bind the socket to its local address
    int err = bind(serverFd_, (struct sockaddr *)&servAddr_, sizeof(servAddr_));
    if (err < 0) {
      std::fprintf(stderr, "Error binding socket to local address\r\n");
      std::exit(-2);
    }

    if ((listen(serverFd_, 5)) != 0) {
      std::fprintf(stderr, "Listen failed...\r\n");
      exit(0);
    }
    std::printf("wait for client\r\n");
  }

  void send(const char *str) {
    // if (clientFd_ < 0) {
    //   // wait for client
    //   acceptClient();
    // }
    sockaddr_in clientSockAddr;
    socklen_t clientSockAddrSize = sizeof(clientSockAddr);
    clientFd_ =
        accept(serverFd_, (sockaddr *)&clientSockAddr, &clientSockAddrSize);
    std::printf("client come\r\n");
    write(clientFd_, str, strlen(str));
    close(clientFd_);
  }

private:
  struct sockaddr_in servAddr_ {};
  int serverFd_{-1};
  int clientFd_{-1};
};

int main(int argc, char *argv[], char **envs) {
  Shoot s{};

  while (true) {
    usleep(10000);
    // std::string str;
    // std::cin >> str;
    // s.send(str.c_str());
    s.send("hdddddddello worlddddddddddddddddd\r\n");
  }

  return 0;
}
