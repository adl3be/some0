#ifndef TEXTSERVER_H
#define TEXTSERVER_H



typedef struct textserver_msg_s {
	unsigned int				len;
	unsigned char*				data;
	int							must_be_freed;
} textserver_msg_t, *textserver_msg_p;


extern int						textserver_max_client;
extern textserver_msg_t			textserver_bye_msg;
extern int						textserver_running;



int textserver_run (
		char* address,
		textserver_msg_t (*on_accept) (int),
		textserver_msg_t (*on_request) (int, char*, int),
		void (*on_close) (int)
);








#endif
