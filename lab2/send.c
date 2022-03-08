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

int main(int argc,char** argv){
  init(HOST,PORT);
  msg sentMessage;

  frame sentFrame;
  int seq_no_cnt = 0;

  memset(&sentFrame, 0, sizeof(frame));
  sentFrame.type = FILE_NAME;
  sentFrame.seq_no = seq_no_cnt;
  strcpy(sentFrame.data, argv[1]);

  memcpy(sentMessage.payload, &sentFrame, 2 + strlen(argv[1]));

  sentMessage.len = 2 + strlen(sentFrame.data);

  send_message(&sentMessage);

  if (recv_message(&sentMessage) < 0){
    perror("receive error");
  }
  else {
    printf("[%s] Got reply with payload: %s\n",argv[0],sentMessage.payload);
  }

  

  return 0;
}
