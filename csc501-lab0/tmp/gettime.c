/* gettime.c - gettime */

#include <conf.h>
#include <kernel.h>
#include <date.h>
#include <lab0.h>
extern int getutim(unsigned long *);

/*------------------------------------------------------------------------
 *  gettime  -  get local time in seconds past Jan 1, 1970
 *------------------------------------------------------------------------
 */
SYSCALL	gettime(long *timvar)
{
    /* long	now; */

	/* FIXME -- no getutim */
	enum system_calls _gettime = gettime_;
        unsigned long start_time = ctr1000;
	if(start_tracer){
        Tracer[currpid][_gettime].count++;
        Tracer[currpid][_gettime].exec_time += ctr1000 - start_time;
      }



    return OK;
}
