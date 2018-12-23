/* sdelete.c - sdelete */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>
#include <lab0.h>
/*------------------------------------------------------------------------
 * sdelete  --  delete a semaphore by releasing its table entry
 *------------------------------------------------------------------------
 */
SYSCALL sdelete(int sem)
{
	STATWORD ps;    
	int	pid;
	struct	sentry	*sptr;
	enum system_calls _sdelete = sdelete_;
        unsigned long start_time = ctr1000;

	disable(ps);
	if (isbadsem(sem) || semaph[sem].sstate==SFREE) {
		restore(ps);
		if(start_tracer){
        Tracer[currpid][_sdelete].count++;
        Tracer[currpid][_sdelete].exec_time += ctr1000 - start_time;
      }
		return(SYSERR);
	}
	sptr = &semaph[sem];
	sptr->sstate = SFREE;
	if (nonempty(sptr->sqhead)) {
		while( (pid=getfirst(sptr->sqhead)) != EMPTY)
		  {
		    proctab[pid].pwaitret = DELETED;
		    ready(pid,RESCHNO);
		  }
		resched();
	}
	restore(ps);
	 if(start_tracer){
        Tracer[currpid][_sdelete].count++;
        Tracer[currpid][_sdelete].exec_time += ctr1000 - start_time;
      }
	
	return(OK);
}
