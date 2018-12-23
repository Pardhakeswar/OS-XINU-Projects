/* setdev.c - setdev */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lab0.h>
/*------------------------------------------------------------------------
 *  setdev  -  set the two device entries in the process table entry
 *------------------------------------------------------------------------
 */
SYSCALL	setdev(int pid, int dev1, int dev2)
{
	short	*nxtdev;
	enum system_calls _setdev = setdev_;
        unsigned long start_time = ctr1000;

	if (isbadpid(pid)){
		if(start_tracer){
        Tracer[currpid][_setdev].count++;
        Tracer[currpid][_setdev].exec_time += ctr1000 - start_time;
      }
		return(SYSERR);
	}
	nxtdev = (short *) proctab[pid].pdevs;
	*nxtdev++ = dev1;
	*nxtdev = dev2;
	 if(start_tracer){
        Tracer[currpid][_setdev].count++;
        Tracer[currpid][_setdev].exec_time += ctr1000 - start_time;
      }

	return(OK);
}
