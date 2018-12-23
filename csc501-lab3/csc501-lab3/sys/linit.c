#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<lock.h>
#include <q.h>


struct lentry locks[NLOCKS];

void linit(){

	struct lentry *lptr;
	int i;

	for (i=0 ; i<NLOCKS ; i++) {      /* initialize locks */
                (lptr = &locks[i])->lstate = LFREE;
		lptr->ltype =NONE;
		lptr->readcnt =0;
                lptr->lqtail = 1 + (lptr->lqhead = newqueue());
		
		int k;
		for (k =0; k< 50 ;k++)
                	lptr->lheld_proc[k] =0;

        }


	return;

}
