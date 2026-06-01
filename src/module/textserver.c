#include <textserver.h>
#include <easyconnect.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>




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
)
{
	int							listener = easyconnect_on(address, textserver_max_client);										if (!~listener) return 1;
	int							client = accept(listener, NULL, NULL);															if (!~client) return 2;
	textserver_msg_t			answer;
	unsigned char				buffer[1024];
	int							len;
	
	
	answer = on_accept(client);
	write(client, answer.data, answer.len);			if ( answer.must_be_freed )		free(answer.data);

	len = read(client, buffer, 1024);
	while ( len > 0 ) {
		answer = on_request(client, buffer, len);
		write(client, answer.data, answer.len);			if ( answer.must_be_freed )		free(answer.data);
		len = read(client, buffer, 1024);
	}																															if ( len < 0 ) return 3;
	answer = on_close(client);
	write(client, textserver_bye_msg.data, textserver_bye_msg.len);			if ( textserver_bye_msg.must_be_freed )		free(textserver_bye_msg.data);


	return 0;
}
