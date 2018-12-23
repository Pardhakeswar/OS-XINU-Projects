#include<proc.h>

#define N_SYSCALL 27

		/* global variables declaration*/

unsigned long start_tracer;
unsigned long ctr1000;

		/*strutures declaration*/

typedef struct trace_data_{
	unsigned int count;
	unsigned long exec_time;
}trace_data;

trace_data Tracer[NPROC][N_SYSCALL];

		/*system calls enum*/

enum system_calls
{
	freemem_,
	chprio_,
	getpid_,
	getprio_,
	gettime_,
	kill_,
	receive_,
	recvclr_,
	recvtim_,
	resume_,
	scount_,
	sdelete_,
	send_,
	setdev_,
	setnok_,
	screate_,
	signal_,
	signaln_,
	sleep_,
	sleep10_,
	sleep100_,
	sleep1000_,
	sreset_,
	stacktrace_,
	suspend_,
	unsleep_,
	wait_
	

};

		/* function prototypes*/

extern long zfunction(long);
void printsegaddress();
void printtos();
void printprocstks(int);
//
void syscallsummary_start();
void syscallsummary_stop();
void printsyscallsummary();
