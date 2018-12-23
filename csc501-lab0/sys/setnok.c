/* setnok.c - setnok */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>
/*------------------------------------------------------------------------
 *  setnok  -  set next-of-kin (notified at death) for a given process
 *------------------------------------------------------------------------
 */
SYSCALL	setnok(int nok, int pid)
{
	STATWORD ps;    
	struct	pentry	*pptr;
	enum system_calls _setnok = setnok_;
        unsigned long start_time = ctr1000;

	disable(ps);
	if (isbadpid(pid)) {
		restore(ps);
		if(start_tracer){
       			 Tracer[currpid][_setnok].count++;
        		Tracer[currpid][_setnok].exec_time += ctr1000 - start_time;
      		}

		return(SYSERR);
	}
	pptr = &proctab[pid];
	pptr->pnxtkin = nok;
	restore(ps);
	if(start_tracer){
        Tracer[currpid][_setnok].count++;
        Tracer[currpid][_setnok].exec_time += ctr1000 - start_time;
      }

	return(OK);
}
