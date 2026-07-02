#include <simplechain.h>
#include <stdio.h>



int first_is_number (void*);
void printer (void*);


int main (int argc, char* argv[], char* env[])
{
	simplechain_p				guy = NULL;
	simplechain_p				not_name;


	simplechain_add(&guy, "end.");
	simplechain_add(&guy, "5924358");
	simplechain_add(&guy, "experiment");
	simplechain_add(&guy, "0rubick");
	simplechain_add(&guy, "some");
	simplechain_add(&guy, "1234");
	simplechain_add(&guy, "When I see the way you went");
	simplechain_add(&guy, "asdfqwer");

	not_name = simplechain_select(guy, first_is_number);
		if ( not_name )	simplechain_dump(not_name, printer);
		else			printf("<empty>\n");
	simplechain_break(&not_name);


	return 0;
}









int first_is_number (void* name)
{
	char						c = *((char*)name);

	if ( 0x30 <= c && c <= 0x39 )			return 1;
	else									return 0;
}


void printer (void* name)
{
	printf("%s\n", name);
}



