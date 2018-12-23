/* chprio.c - chprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>
#include <lab0.h>
/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
SYSCALL chprio(int pid, int newprio)
{
	STATWORD ps;    
	struct	pentry	*pptr;
	enum system_calls _chprio_ = chprio_;
	unsigned long start_time = ctr1000;
	
	disable(ps);
	if (isbadpid(pid) || newprio<=0 ||
	    (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
	if(start_tracer){
		Tracer[currpid][_chprio_].count++;
 		Tracer[currpid][_chprio_].exec_time += ctr1000 -start_time;
		}
		return(SYSERR);
	}
	pptr->pprio = newprio;
	restore(ps);
	if(start_tracer){
	Tracer[currpid][_chprio_].count++;
        Tracer[currpid][_chprio_].exec_time += ctr1000 - start_time;
	}	
	return(newprio);
}
