#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

int main(int argc,char** argv){
  msg titleMessage,titleAckMessage;

  frame titleFrame;
  memset(&titleFrame, 0, sizeof(frame));

  init(HOST,PORT);

  if (recv_message(&titleMessage) < 0) {
    perror("Receive message");
    return -1;
  }
  memset(&titleFrame, 0, sizeof(frame));
  memcpy(&titleFrame, titleMessage.payload, titleMessage.len);

  sprintf(titleAckMessage.payload,"%d", (char) titleFrame.seq_no + 1);

  printf("Got message with data: %s \n", titleFrame.data);
  titleAckMessage.len = strlen(titleAckMessage.payload) + 1;
  send_message(&titleAckMessage);
  return 0;
}
