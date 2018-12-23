/* signaln.c - signaln */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>
#include <lab0.h>
/*------------------------------------------------------------------------
 *  signaln -- signal a semaphore n times
 *------------------------------------------------------------------------
 */
SYSCALL signaln(int sem, int count)
{
	STATWORD ps;    
	struct	sentry	*sptr;
	enum system_calls _signaln = signaln_;
        unsigned long start_time = ctr1000;

	disable(ps);
	if (isbadsem(sem) || semaph[sem].sstate==SFREE || count<=0) {
		restore(ps);
		if(start_tracer){
        Tracer[currpid][_signaln].count++;
        Tracer[currpid][_signaln].exec_time += ctr1000 - start_time;
      }
		return(SYSERR);
	}
	sptr = &semaph[sem];
	for (; count > 0  ; count--)
		if ((sptr->semcnt++) < 0)
			ready(getfirst(sptr->sqhead), RESCHNO);
	resched();
	restore(ps);
	if(start_tracer){
        Tracer[currpid][_signaln].count++;
        Tracer[currpid][_signaln].exec_time += ctr1000 - start_time;
      }
	return(OK);
}
