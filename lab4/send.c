#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

int main(int argc, char *argv[])
{
	msg t;
	int i, res;

	int bdp = atoi(argv[1]);

	printf("%d\n", bdp /1400);
	printf("[SENDER] Starting.\n");
	init(HOST, PORT);

	int windowSize = bdp / 1400;
	if (windowSize > COUNT) {
		windowSize = COUNT;
	}

	for (i = 0; i < windowSize; i++) {
		memset(&t, 'r', sizeof(msg));
		t.payload[0] = i;
		t.len = MSGSIZE;

		res = send_message(&t);
		if (res < 0) {
			perror("[SENDER] Send error. Exiting.\n");
			return -1;
		}
		printf("[SENDER] Sending message %d\n", i);

	}

	for(int i = windowSize; i < COUNT; i++) {
		res = recv_message(&t);
		if (res < 0) {
			perror("[SENDER] Send error. Exiting.\n");
			return -1;
		}

		t.len = MSGSIZE;
		memset(&t, 'r', sizeof(msg));
		t.payload[0] = i;

		res = send_message(&t);
		printf("[SENDER] Sending message %d\n", i);
		if (res < 0) {
			perror("[SENDER] Send error. Exiting.\n");
			return -1;
		}
	}


	printf("[SENDER] Job done, all sent.\n");
	return 0;
}
