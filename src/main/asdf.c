#include <textserver.h>




char* echo (int, char*, int);




int main (int argc, char* argv[], char* env[])
{
	textserver_hook_on_request = echo;
	textserver_run();
}






char* echo (int fd, char* i_str, int i_len)
{
	return i_str;
}
