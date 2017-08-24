#include <stdio.h>      /* for perror() */
#include <stdlib.h>     /* for exit() */
#include <sys/socket.h> /* for socket(), bind(), and constants */
#include <arpa/inet.h>  /* for sockaddr_in, sockaddr, and constants */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#include <string>
#include <iostream>

#define BACKLOG 1

int die(std::string msg)
{
  std::cout << msg << '\n';
  return 1;
}

int main()
{
  struct sockaddr_in serverAddress;
  auto port = 5000;

  auto serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serverSocket < 0)
  {
    return die("Couldnt' create socket");
  }

  /* Ensure address memory is empty, so nothing leaks in */
  memset(&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  serverAddress.sin_port = htons(port);

  if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
  {
    return die("couldn't bind address");
  }

  if (listen(serverSocket, BACKLOG) < 0)
  {
    return die("couldn't listen");
  }

  struct sockaddr_in clientAddress;
  unsigned int clientLen = sizeof(clientAddress);
  auto message = "Hello, from this C server!\n";
  auto msgLen = strlen(message);

  while (1)
  {
    auto clientSocket = accept(
      serverSocket,
      (struct sockaddr *)&clientAddress,
      &clientLen);
    
    if (clientSocket < 0)
    {
      return die("Couln't accept connection");
    }

    printf("Got connection from %s:%d\n",
           inet_ntoa(clientAddress.sin_addr),
           clientAddress.sin_port);

    if (send(clientSocket, message, msgLen, 0) != msgLen)
    {
      return die("Couln't send whole message");
    }
    close(clientSocket);
  }

  close(serverSocket);

  return 0;
}
