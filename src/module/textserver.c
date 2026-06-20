#include <textserver.h>
#include <easyconnect.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>




/* kinda private functions */
struct pollfd					pfd[TEXTSERVER_POLLQ] = {};
int								pfd_curr = 0;
int								pfd_done = 0;

void pfd_add (int);
void pfd_del (int);
void pfd_check ();

void talk (int);
void procedure_accept ();
void procedure_request (int, char*, int);
void procedure_close (int);

/* default hooks */






char*							textserver_host = "127.0.0.1:54321";
int								textserver_max_client = 128;
int								textserver_running = 0;

char*							(*textserver_hook_on_accept) (int);
char*							(*textserver_hook_on_request) (int, char*, int);
char*							(*textserver_hook_on_close) (int);
void							(*textserver_hook_after_accept) (int, char*);
void							(*textserver_hook_after_request) (int, char*);
void							(*textserver_hook_after_close) (int, char*);






int textserver_run ()
{
	int							listener = easyconnect_on(textserver_host, textserver_max_client);								if (!~listener) return -1;


	pfd_curr = 0;
	for (int i = 0; i < TEXTSERVER_POLLQ; i++) {
		pfd[i].fd		= 0;
		pfd[i].events	= 0;
		pfd[i].revents	= 0;
	}
	pfd_add(listener);

	textserver_running = 1;
	while ( textserver_running ) {
		pfd_done = poll(pfd, pfd_curr, -1);																						if (pfd_done < 0) return -1;
		pfd_check();
	}


	return 0;
}




void pfd_add (int fd)
{
	int							i = pfd_curr++;

	pfd[i].fd		= fd;
	pfd[i].events	= POLLIN;
	pfd[i].revents	= 0;
}

void pfd_del (int i)
{
	int							n = --pfd_curr;

	pfd[i].fd		= pfd[n].fd;
	pfd[i].events	= pfd[n].events;
	pfd[i].revents	= pfd[n].revents;
}

void pfd_check ()
{
	if ( pfd[0].revents & POLLIN ) procedure_accept();

	for (int i = 1; pfd_done && i < pfd_curr; i++) {
		if ( !pfd[i].revents )					continue;

		if ( pfd[i].revents & POLLIN ) {
			talk(i);
			pfd_done--;
		} else {
			textserver_running = 0;
		}
	}
}



void talk (int i)
{
	unsigned char				buffer[65536];
	int							len;


	len = read(pfd[i].fd, buffer, 65536);
	if ( !len ) {
		procedure_close(i);
	} else if ( len < 0 ) {
		textserver_running = 0;
	} else {
		procedure_request(i, buffer, len);
	}
}





void procedure_accept ()
{
	int							rookie = accept(pfd[0].fd, NULL, NULL);

	pfd_add(rookie);
	write(rookie, "Hello!\n", 7);
}


void procedure_request (int i, char* str, int len)
{
	write(pfd[i].fd, str, len);
}


void procedure_close (int i )
{
	write(pfd[i].fd, "Bye!\n", 5);
	pfd_del(i);
	close(pfd[i].fd);
}
