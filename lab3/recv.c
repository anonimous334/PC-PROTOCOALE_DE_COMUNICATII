#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lib.h"
#define HOST "127.0.0.1"
#define PORT 10001

int main(int argc,char** argv){
  init(HOST,PORT);
  msg r,t;
  frame fr;
  int parity = 0;
  int fileID = open("out.txt", O_WRONLY | O_TRUNC);

  while (recv_message(&r)) {
      memcpy(&fr, r.payload, r.len);
      parity = fr.seq_no ^ fr.type;
      for (int i = 0; i < r.len - 3; i++) {
          parity = parity ^ fr.data[i];
      }

      if (fr.parity == parity) {
          if (fr.type == FEND) {
              t.len = 1;
              t.payload[0] = ACK;
              send_message(&t);
              close(fileID);
              break;
          }
          write(fileID, fr.data, r.len - 3);
          t.len = 1;
          t.payload[0] = ACK;
          send_message(&t);
      } else {
          t.len = 1;
          t.payload[0] = NACK;
          send_message(&t);
      }
  }
  return 0;
}
