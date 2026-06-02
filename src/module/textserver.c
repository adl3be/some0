#include <textserver.h>
#include <easyconnect.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>




/* kinda private functions */
int talk (int, textserver_msg_t (*) (int, char*, int), void (*) (int));
int procedure_accept (int, textserver_msg_t (*) (int));
int procedure_request (int, char*, int, textserver_msg_t (*) (int, char*, int));
int procedure_close (int, void (*) (int));




int								textserver_running = 0;
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
		void (*on_close) (int)
)
{
	int							listener = easyconnect_on(address, textserver_max_client);										if (!~listener) return 1;
	int							client = accept(listener, NULL, NULL);															if (!~client) return 2;
	struct pollfd				fds[] = { { .fd = client, .events = POLLIN } };


	textserver_running = 1;
	while ( textserver_running ) {
		poll(fds, 1, -1);
		talk(client, on_request, on_close);
	}


	return 0;
}





int talk (int d, textserver_msg_t (*on_talk) (int, char*, int), void (*on_leave) (int))
{
	unsigned char				buffer[65536];
	int							len;
	
	len = read(d, buffer, 65536);																								if (!~len) return 1;
	if ( len )		return procedure_request(d, buffer, len, on_talk);
	else			return procedure_close(d, on_leave);
}



int procedure_accept (int d, textserver_msg_t (*handler) (int))
{
	textserver_msg_t			answer;
	
	answer = handler(d);
	if ( write(d, answer.data, answer.len) != answer.len )																		return 1;
	if ( answer.must_be_freed )					free(answer.data);

	return 0;
}



int procedure_request (int d, char* ask, int len, textserver_msg_t (*handler) (int, char*, int))
{
	textserver_msg_t			answer;
	
	answer = handler(d, ask, len);
	if ( write(d, answer.data, answer.len) != answer.len )																		return 1;
	if ( answer.must_be_freed )					free(answer.data);
}



int procedure_close (int d, void (*handler) (int))
{
	handler(d);
	if ( write(d, textserver_bye_msg.data, textserver_bye_msg.len) != textserver_bye_msg.len )									return 1;
	if ( textserver_bye_msg.must_be_freed )		free(textserver_bye_msg.data);
	close(d);
}
