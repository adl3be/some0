#ifndef SIMPLECHAIN_H
#define SIMPLECHAIN_H


typedef struct simplechain_s {
	void*						data;
	struct simplechain_s*		next;
} simplechain_t, *simplechain_p;



int simplechain_add (simplechain_p*, void*);
void simplechain_del (simplechain_p*);
void simplechain_break (simplechain_p*);

simplechain_p simplechain_select (simplechain_p, int (*)(void*));

void simplechain_dump (simplechain_p, void (*)(void*));




#endif
