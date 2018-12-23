/* getprio.c - getprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>
/*------------------------------------------------------------------------
 * getprio -- return the scheduling priority of a given process
 *------------------------------------------------------------------------
 */
SYSCALL getprio(int pid)
{
	STATWORD ps;    
	struct	pentry	*pptr;
	enum system_calls _getprio_ = getprio_;
	unsigned long start_time =ctr1000;
	disable(ps);
	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
	if(start_tracer){
		Tracer[currpid][_getprio_].count++;
 		Tracer[currpid][_getprio_].exec_time += ctr1000-start_time;
		return(SYSERR);
		}
	}
	restore(ps);
	if(start_tracer){
	 Tracer[currpid][_getprio_].count++;
         Tracer[currpid][_getprio_].exec_time += ctr1000-start_time;
	}
	return(pptr->pprio);
}
