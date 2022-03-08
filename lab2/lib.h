#ifndef LIB
#define LIB

#define DIM_MSG_MAX 3
#define FILE_NAME 0
#define FILE_DATA 1
#define FILE_END 2
#define ACK 3
#define NACK 4

typedef struct {
  // int type;
  char len; // 2 + cat punem in zona de date
  char payload[1400];
} msg;

typedef struct {
    char type;
    char seq_no;
    char data[1400 - 2 * sizeof(char)];
} frame;

void init(char* remote,int remote_port);
void set_local_port(int port);
void set_remote(char* ip, int port);
int send_message(const msg* m);
int recv_message(msg* r);
//msg* receive_message_timeout(int timeout);

#endif
