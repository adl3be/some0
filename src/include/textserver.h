#ifndef TEXTSERVER_H
#define TEXTSERVER_H



typedef struct textserver_msg_s {
	unsigned int				len;
	unsigned char*				data;
	int							must_be_freed;
} textserver_msg_t, *textserver_msg_p;


int								textserver_max_client = 128;
textserver_msg_t				textserver_bye_msg = {
	.len						= 5,
	.data						= "Bye!\n",
	.must_be_freed				= 0
};



int textserver_run (
		char* address,
		textserver_msg_t (*on_accept) (int),
		textserver_msg_t (*on_request) (int, char*, int),
		textserver_msg_t (*on_close) (int)
);








#endif
