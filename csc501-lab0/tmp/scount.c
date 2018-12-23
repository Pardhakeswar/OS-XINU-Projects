/* scount.c - scount */

#include <conf.h>
#include <kernel.h>
#include <sem.h>
#include <lab0.h>
/*------------------------------------------------------------------------
 *  scount  --  return a semaphore count
 *------------------------------------------------------------------------
 */
SYSCALL scount(int sem)
{
extern	struct	sentry	semaph[];
	enum system_calls _scount = scount_;
        unsigned long start_time = ctr1000;
	if (isbadsem(sem) || semaph[sem].sstate==SFREE){
		if(start_tracer){
        Tracer[currpid][_scount].count++;
        Tracer[currpid][_scount].exec_time += ctr1000 - start_time;
      }	
		return(SYSERR);
	}
	if(start_tracer){
        Tracer[currpid][_scount].count++;
        Tracer[currpid][_scount].exec_time += ctr1000 - start_time;
	}

	return(semaph[sem].semcnt);
}
