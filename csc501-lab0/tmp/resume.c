/* resume.c - resume */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>
/*------------------------------------------------------------------------
 * resume  --  unsuspend a process, making it ready; return the priority
 *------------------------------------------------------------------------
 */
SYSCALL resume(int pid)
{
	STATWORD ps;    
	struct	pentry	*pptr;		/* pointer to proc. tab. entry	*/
	int	prio;			/* priority to return		*/
	enum system_calls _resume = resume_;
        unsigned long start_time = ctr1000;

	disable(ps);
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate!=PRSUSP) {
		restore(ps);
		if(start_tracer){
        Tracer[currpid][_resume].count++;
        Tracer[currpid][_resume].exec_time += ctr1000 - start_time;
      }
		return(SYSERR);
	}
	prio = pptr->pprio;
	ready(pid, RESCHYES);
	restore(ps);
	         if(start_tracer){
        Tracer[currpid][_resume].count++;
        Tracer[currpid][_resume].exec_time += ctr1000 - start_time;
      }
		
	return(prio);
}
