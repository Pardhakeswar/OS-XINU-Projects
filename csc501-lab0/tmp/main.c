/* user.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>
/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */

int main()
{	
	/*
	kprintf("\n\nHello World, Xinu lives\n\n");
	long x = 0xaabbccdd;
	long prb1 = zfunction(x);
	kprintf("Return value after zfunction is 0x%lx \n",prb1);
	*/

	//kprintf("Addr etext : %8X\t Addr edata: %8X\t Adrr end :%8X\n",&etext, &edata, &end);
	//printsegaddress();
	
	
	/* Task 3

	int pid = getpid();	
	stacktrace(pid);
	
	*/
	//int i=10;	
//	asm("movl %esp, esp" );
//	asm("movl %ebp, ebp");
//	kprintf("\n Before entering printtos function");
	//kprintf("\nsp : %08X ,fp : %08x",esp ,ebp);
	//kprintf("\nvalue : %08X ,fp : %08x",*esp ,*ebp);
	//kprintf("\n After entering printtos function");
	//printtos();
	//printsegaddress();
	//printprocstks(10);
	/*
	long old =ctr1000;
	kprintf("\n counter value before start = %d %s",ctr1000);
	sleep1000(1);
	long new =ctr1000;
	kprintf("\n counter value after sleep =%d",new );
	*/
	kprintf("\ntask1 value :0x%08x ",zfunction(0xaabbccdd));
        printsegaddress();
	printtos();
	printprocstks(1);	
	syscallsummary_start();
	getpid();
	getpid();
	sleep(1);
	recvclr();
	//receive();
	//sleep(4);
	syscallsummary_stop();
	printsyscallsummary();	
	
	syscallsummary_start();
	getpid();
	syscallsummary_stop();
	printsyscallsummary();		
return 0;
	
}
