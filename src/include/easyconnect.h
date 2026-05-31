#ifndef EASYCONNECT_H
#define EASYCONNECT_H



int easyconnect_to (unsigned char*);
int easyconnect_on (unsigned char*, int);

struct sockaddr easyconnect_str_to_sa (unsigned char*);
unsigned int easyconnect_str_to_ipv4 (unsigned char*);
unsigned int easyconnect_str_to_port (unsigned char*);









#endif
