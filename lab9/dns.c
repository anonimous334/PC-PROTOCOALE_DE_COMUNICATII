// Protocoale de comunicatii
// Laborator 9 - DNS
// dns.c
// Copyright 2018 Razvan Radoi, first of his name
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int usage(char* name) {
	printf("Usage:\n\t%s -n <NAME>\n\t%s -a <IP>\n", name, name);
	return 1;
}

// Receives a name and prints IP addresses
void get_ip(char* name) {
	int ret;
	struct addrinfo hints, *result, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    ret = getaddrinfo(name, NULL, &hints, &result);
    for (p = result; p!= NULL; p = p->ai_next) {
        printf("%s\n", inet_ntoa(((struct sockaddr_in *) p->ai_addr)->sin_addr));
    }
    freeaddrinfo(result);
}

// Receives an address and prints the associated name and service
void get_name(char* ip) {
	int ret;
	struct sockaddr_in addr;
	char host[1024];
	char service[20];

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    ret = getnameinfo((struct sockaddr *) &addr, sizeof(struct sockaddr),
    host, 1024, NULL, 20, 0);
    printf("%s\n", host);
}

int main(int argc, char **argv) {
	if (argc < 3) {
		return usage(argv[0]);
	}

	if (strncmp(argv[1], "-n", 2) == 0) {
		get_ip(argv[2]);
	} else if (strncmp(argv[1], "-a", 2) == 0) {
		get_name(argv[2]);
	} else {
		return usage(argv[0]);
	}

	return 0;
}
