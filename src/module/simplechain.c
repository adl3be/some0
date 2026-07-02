#include <simplechain.h>
#include <stdlib.h>





int simplechain_add (simplechain_p* head, void* data)
{
	simplechain_p				rookie = malloc(sizeof(simplechain_t));															if (!rookie) return 1;

	rookie->data	= data;
	rookie->next	= *head;
	*head			= rookie;

	return 0;
}



void simplechain_del (simplechain_p* this)
{
	void*						next = (*this)->next;

	free(*this);
	*this = next;
}



void simplechain_break (simplechain_p* this)
{
	while (*this)	simplechain_del(this);
}





simplechain_p simplechain_select (simplechain_p this, int (*selector)(void*))
{
	simplechain_p				res = NULL;

	while ( this ) {
		if ( selector(this->data) )		simplechain_add(&res, this->data);
		this = this->next;
	}

	return res;
}





void simplechain_dump (simplechain_p head, void (*printer)(void*))
{
	while (head) {
		printer(head->data);
		head = head->next;
	}
}
