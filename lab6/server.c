/*
*  	Protocoale de comunicatii:
*  	Laborator 6: UDP
*	mini-server de backup fisiere
*/
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include "helpers.h"
void usage(char*file)
{
	fprintf(stderr,"Usage: %s server_port file\n",file);
	exit(0);
}

/*
*	Utilizare: ./server server_port nume_fisier
*/
int main(int argc,char**argv)
{
	int fd;

	if (argc!=3)
		usage(argv[0]);

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket failed");
        return 1;
    }

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(50037);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ( bind(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
        perror("bind failed");
        return 1;
    }

    char buffer[200];
    for (int i = 0; i < 4; i++) {
        int len = recvfrom(fd, buffer, sizeof(buffer) - 1, 0, NULL, 0);
        if (len < 0) {
            perror("recvfrom failed");
            break;
        }
        buffer[len] = '\0';
        printf("%d bytes : %s \n", len, buffer);
    }

    close(fd);

	return 0;
}
