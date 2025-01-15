#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void launch(struct Server *server) {
  int new_socket;
  int address_length;
  char buffer[10000];
  while (1) {
    printf("Listening on 127.0.0.1\n===== WAITING FOR CONNECTION =====\n");
    address_length = sizeof(server->address);
    new_socket = accept(server->socket, (struct sockaddr *)&server->address,
                        (socklen_t *)&address_length);
    read(new_socket, buffer, 10000);
    printf("%s\n", buffer);
    printf("===== CONNECTION RECIVED =====\n");
    const char hello[] =
        "HTTP/1.1 200 OK\nDate: Sat, 11 Jan 2025 21:05:22 "
        "GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Sat, "
        "11 Jan 2025 21:05:22 GMT\nSat, 11 Jan 2025 21:05:22 "
        "GMT\nContent-Type: text/html\nConnection: "
        "Closed\n\n<html><body><h1>Hello world</h1></body></html>";
    write(new_socket, hello, sizeof(hello) - 1);
    close(new_socket);
  }
}

int main() {
  struct Server server =
      server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, 10, launch);
  server.launch(&server);
  return EXIT_SUCCESS;
}
