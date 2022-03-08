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

  msg t, r;
  frame ft;

  int fileID = open("in.txt", O_RDONLY);

  int c = 0;
  int seq_no = 0;
  while((c = read(fileID, ft.data, DIM_MSG) )> 0) {
      ft.type = FDATA;
      ft.seq_no = seq_no;
      seq_no = (seq_no + 1) % 256;

      ft.parity = ft.seq_no ^ ft.type;
      for (int i = 0; i < c; i++) {
          ft.parity = ft.parity ^ ft.data[i];
      }

      t.len = 3 + c;
      memcpy(t.payload, &ft, t.len);
      send_message(&t);

      while(1) {
          recv_message(&r);
          if (r.payload[0] == ACK) {
              printf("S-a primit corect frame-ul %d\n", seq_no - 1);
              break;
          } else {
              printf("Se retransmite\n");
              send_message(&t);
          }
      }
  }
  ft.type = FEND;
  ft.seq_no = seq_no;
  ft.parity = ft.type ^ ft.seq_no;
  t.len = 3;
  memcpy(t.payload, &ft, t.len);
  send_message(&t);
  while(1) {
      recv_message(&r);
      if (r.payload[0] == ACK) {
          break;
      } else {
          send_message(&t);
      }
  }


  return 0;
}
