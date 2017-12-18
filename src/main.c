#define _GNU_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>

#define MAX_DOMAIN_SIZE 253

int validate_ip(char *ip){
  struct sockaddr_in sa;
  int result = inet_pton(AF_INET, ip, &(sa.sin_addr));
  return result;
}

int validate_port(int port){
  // Validate Port
  if (port < 0 || port > 65535){
    return -1;
  } else{
    return 1;
  }
}

int validate_domain(char *domain){
  // Validate Domain
  if (sizeof(domain) > MAX_DOMAIN_SIZE){
    fprintf(stderr, "Invalid domain name!\n");
    return -1;
  } else{
    return 1;
  }
}

char *i2cPort(int port){
  char *cPort;
  if (asprintf(&cPort, "%d", port) < 0){
    return NULL;
  } else{
    return cPort;
  }
}

int http_send(char *domain, int port, char *data){
  int sockfd, connectfd;
  struct addrinfo hints, *servinfo, *p;
  if (validate_port(port) < 0){ return -1; }
  if (validate_domain(domain) < 0){ return -2; }
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  int result = getaddrinfo(domain, i2cPort(port), &hints, &servinfo);
  if (result != 0){
    fprintf(stderr, "Error obtaining address info.");
    return -3;
  } else{
    printf("Successfully collected address info!\n");
  }
  for(p = servinfo; p != NULL; p = p->ai_next){
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd < 0){
      fprintf(stderr, "Error opening socket\n");
      continue;
    } else{
      printf("Opened socket successfully!\n");
    }
    connectfd = connect(sockfd, p->ai_addr, p->ai_addrlen);
    if (connectfd < 0){
      fprintf(stderr, "Failed to connect to socket\n");
      close(sockfd);
      continue;
    } else{
      printf("Successfully connected to socket!\n");
      result = send(sockfd, data, strlen(data), 0);
      if (result < 0){
        fprintf(stderr, "Failed to send data!\n");
        return -4;
      } else{
        printf("Successfully sent data!\n");
        close(sockfd);
      }
      break;
    }
  }
  freeaddrinfo(servinfo);
  return 0;
}
