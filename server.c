#include "server.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
struct Server server_constructor(int domain, int service, int protocol,
                                 u_long interface, int port, int backlog,
                                 void (*launch)(struct Server *server)) {
  struct Server server;

  server.domain = domain;
  server.service = service;
  server.protocol = protocol;
  server.interface = interface;
  server.port = port;
  server.backlog = backlog;

  server.address.sin_family = domain;
  server.address.sin_port = htons(port);
  server.address.sin_addr.s_addr = htonl(interface);

  // creats socket, binds it and then starts lisening
  server.socket = socket(domain, service, protocol);

  if (server.port == 0) {
    perror("Failed to connect to socket\n");
    exit(EXIT_FAILURE);
  }

  if ((bind(server.socket, (struct sockaddr *)&server.address,
            sizeof(server.address))) < 0) {
    perror("Failed to bind socket\n");
    exit(EXIT_FAILURE);
  }

  if (listen(server.socket, server.backlog) < 0) {
    perror("Failed to start lisening\n");
    exit(EXIT_FAILURE);
  }

  server.launch = launch;

  return server;
};
