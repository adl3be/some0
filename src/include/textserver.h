#ifndef TEXTSERVER_H
#define TEXTSERVER_H


#define TEXTSERVER_POLLQ		1024



extern char*					textserver_host;
extern int						textserver_max_client;
extern int						textserver_running;

extern char*					(*textserver_hook_on_accept) (int);
extern char*					(*textserver_hook_on_request) (int, char*, int);
extern char*					(*textserver_hook_on_close) (int);
extern void						(*textserver_hook_after_accept) (int, char*);
extern void						(*textserver_hook_after_request) (int, char*);
extern void						(*textserver_hook_after_close) (int, char*);


int textserver_run ();








#endif
