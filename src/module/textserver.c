#include <textserver.h>
#include <easyconnect.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>




/* kinda private data and functions */
struct pollfd					pfd[TEXTSERVER_POLLQ] = {};
int								pfd_curr = 0;
int								pfd_done = 0;

/* routins for polling */
void pfd_add (int);
void pfd_del (int);
void pfd_check ();

/* base process routins */
void talk (int);
void procedure_accept ();
void procedure_request (int, char*, int);
void procedure_close (int);

/* default hooks */
char* default_on_accept (int);
char* default_on_request (int, char*, int);
char* default_on_close (int);
void default_after_accept (int, char*);
void default_after_request (int, char*);
void default_after_close (int, char*);
/**/







char*							textserver_host = "127.0.0.1:54321";
int								textserver_max_client = 128;
int								textserver_running = 0;

char*							(*textserver_hook_on_accept) (int) = default_on_accept;
char*							(*textserver_hook_on_request) (int, char*, int) = default_on_request;
char*							(*textserver_hook_on_close) (int) = default_on_close;
void							(*textserver_hook_after_accept) (int, char*) = default_after_accept;
void							(*textserver_hook_after_request) (int, char*) = default_after_request;
void							(*textserver_hook_after_close) (int, char*) = default_after_close;






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














/* routins for polling */
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






/* base process routins */
void talk (int i)
{
	unsigned char				buffer[TEXTSERVER_INPUT_BUFFER];
	int							len;


	for (int i = 0; i < TEXTSERVER_INPUT_BUFFER; i++) buffer[i] = 0;

	len = read(pfd[i].fd, buffer, TEXTSERVER_INPUT_BUFFER);
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
	char*						answer;
	int							a_len = 0;

	pfd_add(rookie);

	answer = textserver_hook_on_accept(rookie);
	for (int i = 0; *(answer + i); i++, a_len++);
	write(rookie, answer, a_len);

	textserver_hook_after_accept(rookie, answer);
}
void procedure_request (int i, char* str, int len)
{
	char*						answer;
	int							a_len = 0;

	answer = textserver_hook_on_request(pfd[i].fd, str, len);
	for (int i = 0; *(answer + i); i++, a_len++);
	write(pfd[i].fd, answer, a_len);

	textserver_hook_after_request(pfd[i].fd, answer);
}
void procedure_close (int i)
{
	char*						answer;
	int							a_len = 0;

	answer = textserver_hook_on_close(pfd[i].fd);
	for (int i = 0; *(answer + i); i++, a_len++);
	write(pfd[i].fd, answer, a_len);

	pfd_del(i);
	close(pfd[i].fd);

	textserver_hook_after_close(pfd[i].fd, answer);
}












/* default hooks */
char* default_on_accept (int fd)
{
	return "Hi!\n";
}
char* default_on_request (int fd, char* str, int len)
{
	return "<not implemented>\n";
}
char* default_on_close (int fd)
{
	return "Bye!\n";
}
void default_after_accept (int fd, char* str)
{
}
void default_after_request (int fd, char* str)
{
}
void default_after_close (int fd, char* str)
{
}
