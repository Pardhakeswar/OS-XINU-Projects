/* signal.c - signal */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * signal  --  signal a semaphore, releasing one waiting process
 *------------------------------------------------------------------------
 */
SYSCALL signal(int sem)
{
	STATWORD ps;    
	register struct	sentry	*sptr;
	enum system_calls _signal = signal_;
        unsigned long start_time = ctr1000;

	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore(ps);
		if(start_tracer){
        Tracer[currpid][_signal].count++;
        Tracer[currpid][_signal].exec_time += ctr1000 - start_time;
      }
		return(SYSERR);
	}
	if ((sptr->semcnt++) < 0)
		ready(getfirst(sptr->sqhead), RESCHYES);
	restore(ps);
	if(start_tracer){
        Tracer[currpid][_signal].count++;
        Tracer[currpid][_signal].exec_time += ctr1000 - start_time;
      }
	return(OK);
}
