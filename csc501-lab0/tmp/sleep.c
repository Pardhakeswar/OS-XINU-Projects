/* sleep.c - sleep */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>
#include <lab0.h>
/*------------------------------------------------------------------------
 * sleep  --  delay the calling process n seconds
 *------------------------------------------------------------------------
 */
SYSCALL	sleep(int n)
{
	STATWORD ps;
	unsigned long start_time = ctr1000;
	enum system_calls _sleep_ = sleep_ ;    
	if (n<0 || clkruns==0){
		if(start_tracer){
        	Tracer[currpid][_sleep_].count++;
        	Tracer[currpid][_sleep_].exec_time += ctr1000 - start_time;
     	    }
		return(SYSERR);
	}
	if (n == 0) {
	        disable(ps);
		resched();
		restore(ps);
		if(start_tracer){
                Tracer[currpid][_sleep_].count++;
                Tracer[currpid][_sleep_].exec_time += ctr1000 - start_time;
            }

		return(OK);
	}
	while (n >= 1000) {
		sleep10(10000);
		n -= 1000;
	}
	if (n > 0)
		sleep10(10*n);
		if(start_tracer){
                Tracer[currpid][_sleep_].count++;
                Tracer[currpid][_sleep_].exec_time += ctr1000 - start_time;
            }

	return(OK);
}
