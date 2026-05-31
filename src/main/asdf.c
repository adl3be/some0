#include <unistd.h>
#include <easyconnect.h>
#include <sys/socket.h>



int main (int argc, char* argv[], char* env[])
{
	int							sd;
	int							cl;


	if ( !argv[1] )																												return 1;
	sd = easyconnect_on(argv[1], 16);																							if (!~sd) return 2;
	cl = accept(sd, NULL, NULL);																								if (!~cl) return 3;
	write(cl, "Hi, dude!\n", 10);


	return 0;
}
