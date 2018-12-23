/* ldelete.c - ldelete */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>


SYSCALL ldelete(int ldes)
{
	STATWORD ps;
        int     pid;
        struct  lentry  *lptr;

        disable(ps);
        if (isbadlock(ldes) || locks[ldes].lstate == LFREE) {
                restore(ps);
                return(SYSERR);
        }
        lptr = &locks[ldes];
        lptr->lstate = LFREE;
        if (nonempty(lptr->lqhead)) {
                while( (pid=getfirst(lptr->lqhead)) != EMPTY)
                  {
                    proctab[pid].plockret = DELETED;
                    ready(pid,RESCHNO);
                  }
                resched();
        }
        restore(ps);
        return(OK);

	
}


