/* printsyscallsummary */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lab0.h>
unsigned int sys_start=0, sys_stop=0;

char* print_syscall[] = {       "freemem",
                                "chprio",
                                "getpid",
                                "getprio",
                                "gettime",
                                "kill",
                                "receive",
                                "recvclr",
                                "recvtim",
                                "resume",
                                "scount",
                                "sdelete",
                                "send",
                                "setdev",
                                "setnok",
                                "screate",
                                "signal",
                                "signaln",
                                "sleep",
                                "sleep10",
                                "sleep100",
                                "sleep1000",
                                "reset",
                                "stacktrace",
                                "suspend",
                                "unsleep",
                                "wait"
                        };



void syscallsummary_start(){
	
        start_tracer =1;
	sys_start=1;
        int i,j;
        for(i=0; i < NPROC; i++){

                for(j=0; j < N_SYSCALL; j++){
                        Tracer[i][j].count = 0;
                        Tracer[i][j].exec_time = 0;
                }
        }
}

void syscallsummary_stop (){
        start_tracer =0;
	sys_stop=1;
	
}

void  printsyscallsummary()
{


	if(sys_start && sys_stop){
	
	kprintf("\n\n void printsyscallsummary()"
		"\nProcess [pid:%d]",currpid);
	int i,j;
	int exec_time;
	for (j=0; j<N_SYSCALL;j++){
		
		if(Tracer[currpid][j].count){
		//	kprintf("\n Tracer[%d][%d]].count  :%d",i,j,Tracer[i][j].count);	
			exec_time = (Tracer[currpid][j].exec_time)/(Tracer[currpid][j].count);
			kprintf("\n        syscall: %s, "
					   "count :%d , "
					   "average execution time: %lu (ms)",
					   print_syscall[j],
					   Tracer[currpid][j].count,
					   exec_time);
					
				}
		
		}
	}
	else{
	kprintf("\n System monitoring is Either not on  or not stopped");
	}
	sys_start =0;
	sys_stop=0;
}




