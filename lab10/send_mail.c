#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define SMTP_PORT 25
#define MAXLEN 500

/**
 * Citeste maxim maxlen octeti din socket-ul sockfd. Intoarce
 * numarul de octeti cititi.
 */
ssize_t Readline(int sockd, void *vptr, size_t maxlen) {
    ssize_t n, rc;
    char    c, *buffer;

    buffer = vptr;

    for ( n = 1; n < maxlen; n++ ) {
	if ( (rc = read(sockd, &c, 1)) == 1 ) {
	    *buffer++ = c;
	    if ( c == '\n' )
		break;
	}
	else if ( rc == 0 ) {
	    if ( n == 1 )
		return 0;
	    else
		break;
	}
	else {
	    if ( errno == EINTR )
		continue;
	    return -1;
	}
    }

    *buffer = 0;
    return n;
}

/**
 * Trimite o comanda SMTP si asteapta raspuns de la server.
 * Comanda trebuie sa fie in buffer-ul sendbuf.
 * Sirul expected contine inceputul raspunsului pe care
 * trebuie sa-l trimita serverul in caz de succes (de ex. codul
 * 250). Daca raspunsul semnaleaza o eroare se iese din program.
 */
void send_command(int sockfd, char sendbuf[], char *expected) {
  char recvbuf[MAXLEN];
  int nbytes;
  char CRLF[3];

  CRLF[0] = 13; CRLF[1] = 10; CRLF[2] = 0;
  strcat(sendbuf, CRLF);
  printf("Trimit: %s", sendbuf);
  write(sockfd, sendbuf, strlen(sendbuf));
  nbytes = Readline(sockfd, recvbuf, MAXLEN - 1);
  recvbuf[nbytes] = 0;
  printf("Am primit: %s", recvbuf);
  if (strstr(recvbuf, expected) != recvbuf) {
    printf("Am primit mesaj de eroare de la server!\n");
    exit(-1);
  }
}

int main(int argc, char **argv) {
  int sockfd;
  int port = SMTP_PORT;
  struct sockaddr_in servaddr;
  char server_ip[10];
  char sendbuf[MAXLEN];
  char recvbuf[MAXLEN];

  if (argc != 2) {
    printf("Utilizare: ./send_msg adresa_server");
    exit(-1);
  }
  strcpy(server_ip, argv[1]);

  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	printf("Eroare la  creare socket.\n");
	exit(-1);
  }

  /* formarea adresei serverului */
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);

  if (inet_aton(server_ip, &servaddr.sin_addr) <= 0 ) {
    printf("Adresa IP invalida.\n");
    exit(-1);
  }

  /*  conectare la server  */
  if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
    printf("Eroare la conectare\n");
    exit(-1);
  }

  Readline(sockfd, recvbuf, MAXLEN - 1);
  printf("Am primit: %s\n", recvbuf);

  sprintf(sendbuf, "HELO razvan");
  send_command(sockfd, sendbuf, "250");

  while (1) {
      Readline(STDIN_FILENO, sendbuf, MAXLEN - 1);
      sendbuf[strlen(sendbuf) - 1] = '\0';

      if (strncmp(sendbuf, "MAIL FROM:", 10) == 0) {
          send_command(sockfd, sendbuf, "250");
          continue;
      }

      if (strncmp(sendbuf, "RCPT TO:", 8) == 0) {
          send_command(sockfd, sendbuf, "250");
          continue;
      }

      if (strncmp(sendbuf, "RSET", 4) == 0) {
          sprintf(sendbuf, "RSET");
          send_command(sockfd, sendbuf, "250");
          continue;
      }

      if (strncmp(sendbuf, "QUIT", 4) == 0) {
          sprintf(sendbuf, "QUIT");
          send_command(sockfd, sendbuf, "221");
          break;
      }

      if (strncmp(sendbuf, "DATA", 4) == 0) {
          sprintf(sendbuf, "DATA");

          send_command(sockfd, sendbuf, "354");

          memset(sendbuf, 0, sizeof(sendbuf));
          memset(recvbuf, 0, sizeof(recvbuf));
          scanf("%s", recvbuf);

          while(strcmp(recvbuf,".") != 0) {
              //Readline(STDIN_FILENO, recvbuf, MAXLEN - 1);
              strcat(recvbuf, "\r\n");
              strcat(sendbuf, recvbuf);
              scanf("%s", recvbuf);
          }
          strcat(sendbuf, ".");
          send_command(sockfd, sendbuf, "250");
      }
  }

  close(sockfd);
}
