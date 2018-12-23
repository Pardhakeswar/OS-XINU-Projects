/* wait.c - wait */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * wait  --  make current process wait on a semaphore
 *------------------------------------------------------------------------
 */
SYSCALL	wait(int sem)
{
	STATWORD ps;    
	struct	sentry	*sptr;
	struct	pentry	*pptr;
	enum system_calls _wait = wait_;
        unsigned long start_time = ctr1000;
	
	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore(ps);
		if(start_tracer){
        	Tracer[currpid][_wait].count++;
        	Tracer[currpid][_wait].exec_time += ctr1000 - start_time;
	      }

		return(SYSERR);
	}
	
	if (--(sptr->semcnt) < 0) {
		(pptr = &proctab[currpid])->pstate = PRWAIT;
		pptr->psem = sem;
		enqueue(currpid,sptr->sqtail);
		pptr->pwaitret = OK;
		resched();
		restore(ps);
		if(start_tracer){
       		 Tracer[currpid][_wait].count++;
        	 Tracer[currpid][_wait].exec_time += ctr1000 - start_time;
      		}

		return pptr->pwaitret;
	}
	restore(ps);
	if(start_tracer){
        Tracer[currpid][_wait].count++;
        Tracer[currpid][_wait].exec_time += ctr1000 - start_time;
      }

	return(OK);
}
