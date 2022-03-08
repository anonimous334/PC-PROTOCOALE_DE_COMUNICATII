#ifndef LIB
#define LIB

#define FNAME		1
#define FDATA		2
#define FEND		3
#define TRANSEND		4
#define ACK_T1		"ACK(TYPE1)"
#define ACK_T2		"ACK(TYPE2)"
#define ACK_T3		"ACK(TYPE3)"
#define DIM_MSG     3
#define MSGSIZE		1400
#define PKTSIZE		1396
#define ACK 1
#define NACK 0

typedef struct {
  	int len;
  	char payload[MSGSIZE];
} msg;

typedef struct {
    char type;
    char seq_no;
    char parity;
    char data[1400 - 2 * sizeof(char)];
} frame;

void init(char* remote,int remote_port);
void set_local_port(int port);
void set_remote(char* ip, int port);
int send_message(const msg* m);
int recv_message(msg* r);

#endif
