#include <textserver.h>



textserver_msg_t on_init (int);
textserver_msg_t on_request (int, char*, int);
textserver_msg_t on_close (int);



int main (int argc, char* argv[], char* env[])
{
	return textserver_run(
			"127.0.0.1:54321\0", 
			on_init,
			on_request,
			on_close
	);
}




textserver_msg_t on_init (int d)
{
	textserver_msg_t			res = { .len = 7, .data = "Hello!\n", .must_be_freed = 0 };
	return res;
}



textserver_msg_t on_request (int d, char* str, int len)
{
	textserver_msg_t			res = { .len = 7, .data = "Hello!\n", .must_be_freed = 0 };
	return res;
}



textserver_msg_t on_close (int d)
{
	textserver_msg_t			res = { .len = 7, .data = "Hello!\n", .must_be_freed = 0 };
	return res;
}
