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
#include <regex.h>

#define MAX_DOMAIN_SIZE 253

int validate_port(int port){
  if (port < 0 || port > 65535){
    return 1;
  }
  return 0;
}

int validate_domain(char *domain){
  if (sizeof(domain) > MAX_DOMAIN_SIZE){
    fprintf(stderr, "Invalid domain name!\n");
    return 1;
  }
  regex_t regDomain;
  if (regcomp(&regDomain, "^[a-zA-Z0-9][a-zA-Z0-9.-]*$", 0) != 0){
    fprintf(stderr, "ERROR: failed to compile regDomain\n");
    return 1;
  }
  if (regexec(&regDomain, domain, 0, NULL, 0) != 0){
    regfree(&regDomain);
    return 1;
  }
  regfree(&regDomain);
  return 0;
}

int validate_ip(char *ip){
  regex_t regIPV4;
  if (regcomp(&regIPV4, "^[:digit:]{1,3}[.][:digit:]{1,3}[.][:digit:]{1,3}[.][:digit:]{1,3}", 0) != 0){
    fprintf(stderr, "ERROR: failed to compile regIPV4\n");
    return 1;
  }
  if (regexec(&regIPV4, ip, 0, NULL, 0) != 0){
    regfree(&regIPV4);
    return 1;
  }
  regfree(&regIPV4);
  return 0;
}

char *i2cPort(int port){
  char *cPort;
  if (asprintf(&cPort, "%d", port) < 0){
    return NULL;
  } else{
    return cPort;
  }
}

int http_send_domain(char * domain, int port, char *data){
  if (validate_domain(domain) != 0){
    return 1;
  }
  int sockfd, connectfd;
  struct addrinfo hints, *servinfo, *p;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if (getaddrinfo(domain, i2cPort(port), &hints, &servinfo) != 0){
    freeaddrinfo(servinfo);
    return 1;
  }
  for(p = servinfo; p != NULL; p = p->ai_next){
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd < 0){
      continue;
    }
    connectfd = connect(sockfd, p->ai_addr, p->ai_addrlen);
    if (connectfd < 0){
      close(sockfd);
      continue;
    }
    if (send(sockfd, data, strlen(data), 0) < 0){
      continue;
    } else{
      close(sockfd);
      freeaddrinfo(servinfo);
      return 0;
    }
  }
  freeaddrinfo(servinfo);
  return 1;
}


int main(){
  int result = validate_ip("8.8.8.8");
  printf("%d\n", result);
  return 0;
}
