#include <easyconnect.h>
#include <sys/socket.h>
#include <string.h>



/* kinda private functions */
char* itoa_serial (char*, unsigned int*);







int easyconnect_to (char* to)
{
	struct sockaddr				sa = easyconnect_str_to_sa(to);
	int							sd;


	sd = socket(AF_INET, SOCK_STREAM, 0);																						if (!~sd) return -1;
	if ( connect(sd, &sa, 16) )																									return -1;


	return sd;
}



int easyconnect_on (char* on, int count)
{
	struct sockaddr				sa = easyconnect_str_to_sa(on);
	int							sd;


	sd = socket(AF_INET, SOCK_STREAM, 0);																						if (!~sd) return -1;
	if ( bind(sd, &sa, 16) )																									return -1;
	if ( listen(sd, count) )																									return -1;


	return sd;
}







struct sockaddr easyconnect_str_to_sa (char* str)
{
	struct sockaddr				sa;
	unsigned int				ipv4;
	unsigned int				port;
	char*						delimiter = " :-";


	str = strtok(str, delimiter);
	ipv4 = easyconnect_str_to_ipv4(str);
	str = strtok(NULL, delimiter);
	port = easyconnect_str_to_port(str);

	sa.sa_family		= AF_INET;
	sa.sa_data[0]		= port >> 8		& 0xff;
	sa.sa_data[1]		= port			& 0xff;
	sa.sa_data[2]		= ipv4 >> 24	& 0xff;
	sa.sa_data[3]		= ipv4 >> 16	& 0xff;
	sa.sa_data[4]		= ipv4 >> 8		& 0xff;
	sa.sa_data[5]		= ipv4			& 0xff;
	sa.sa_data[6]		= 0;


	return sa;
}







unsigned int easyconnect_str_to_ipv4 (char* str)
{
	unsigned int				res = 0;
	unsigned int				tmp = 0;

	str = itoa_serial(str, &tmp) + 1;
		res |= tmp;
		res <<= 8;
	str = itoa_serial(str, &tmp) + 1;
		res |= tmp;
		res <<= 8;
	str = itoa_serial(str, &tmp) + 1;
		res |= tmp;
		res <<= 8;
	str = itoa_serial(str, &tmp);
		res |= tmp;

	return res;
}


unsigned int easyconnect_str_to_port (char* str)
{
	unsigned int				res = 0;

	itoa_serial(str, &res);

	return res;
}


char* itoa_serial (char* str, unsigned int* num)
{
	*num = 0;

	while ( (0x30 <= *str) && (*str <= 0x39) ) {
		*num *= 10;
		*num += *str++ & 0xf;
	}

	return str;
}
