/* getpid.c - getpid */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * getpid  --  get the process id of currently executing process
 *------------------------------------------------------------------------
 */
SYSCALL getpid()
{	
	
	unsigned long start_time = ctr1000;
	enum system_calls _getpid_ = getpid_ ;
	if(start_tracer){
	Tracer[currpid][_getpid_].count++;
 	Tracer[currpid][_getpid_].exec_time += ctr1000-start_time;
	}
	return(currpid);
}
