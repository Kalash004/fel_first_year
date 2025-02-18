#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define HELLO_SERVICE_PORT 55555

#define BUFFER_SIZE 1000

int main(int argc, char *argv[])
{
  int sockfd;
  int rv;
  int yes=1;
  char buffer[BUFFER_SIZE];
  struct sockaddr_in bindaddr;
  struct sockaddr_in braddr;
  struct sockaddr_in peeraddr;
  socklen_t peeraddr_len;

  memset(&bindaddr, 0, sizeof(bindaddr));
  bindaddr.sin_family = AF_INET;
  bindaddr.sin_port = htons(HELLO_SERVICE_PORT);
  bindaddr.sin_addr.s_addr = INADDR_ANY;

  memset(&braddr, 0, sizeof(braddr));
  braddr.sin_family = AF_INET;
  braddr.sin_port = htons(HELLO_SERVICE_PORT);
  braddr.sin_addr.s_addr = INADDR_BROADCAST;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(yes)) == -1) {
    perror("setsockopt (SO_REUSEADDR)");
    exit(1);
  }

  if (bind(sockfd, (struct sockaddr *)&bindaddr, sizeof(bindaddr)) == -1) {
    perror("bind");
    exit(1);
  }

  {
    int broadcast = 1;

    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast,
                   sizeof broadcast) == -1) {
      perror("setsockopt (SO_BROADCAST)");
      exit(1);
    }
  }

  strncpy(buffer, "Hello world", sizeof(buffer));

  if (sendto(sockfd, buffer, sizeof(buffer), 0,
             (struct sockaddr *)&braddr, sizeof(braddr)) == -1) {
    perror("sendto");
    exit(1);
  }


  do {
    int nfds = sockfd + 1;
    fd_set readfds;
    struct timeval timeout;
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    timeout.tv_sec = 4;
    timeout.tv_usec = 0;

    rv = select(nfds, &readfds, NULL, NULL, &timeout);

    if (rv == -1) {
      perror("select");
      exit(1);
    }

    if (rv == 0) {
      printf("No packet arrived - timeout\n");
      break;
    }

    if (!FD_ISSET(sockfd, &readfds)) {
      printf("Spurious select return %d\n", rv);
    }

    peeraddr_len = sizeof(peeraddr);
    rv = recvfrom(sockfd, buffer, sizeof(buffer), 0,
        (struct sockaddr *)&peeraddr, &peeraddr_len);
    if (rv == -1) {
      perror("recvfrom");
      exit(1);
    }

    if (rv < sizeof(buffer))
      buffer[rv] = 0;
    else
      buffer[sizeof(buffer) - 1] = 0;

    printf("Received packet length %d, content: %s\n", rv, buffer);

  } while(1);

  close(sockfd); /* closesocket(sockfd) on Windows */

  return 0;
}