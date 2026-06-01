#ifndef EASYCONNECT_H
#define EASYCONNECT_H



int easyconnect_to (char*);
int easyconnect_on (char*, int);

struct sockaddr easyconnect_str_to_sa (char*);
unsigned int easyconnect_str_to_ipv4 (char*);
unsigned int easyconnect_str_to_port (char*);









#endif
