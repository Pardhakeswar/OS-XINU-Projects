#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <stdio.h>



/*--------------------------------Test to check the implementaions of semaphore and locks for priority inheritance --------------------------------*/

char output[10];
int count1,count2;

void writer_lock (char msg, int lck, int lprio)
{
        kprintf ("  %c: to acquire lock\n", msg);
        lock (lck, WRITE, lprio);
	output[count1++]=msg;
        kprintf ("  %c: acquired lock \n", msg);
        //sleep (2);
        kprintf ("  %c: to release lock\n", msg);
	output[count1++]=msg;
        releaseall (1, lck);
}

void mid_prio_proc_locks( char msg)
{	
	output[count1++]=msg;
	kprintf ("%c is not able to interrupt due to priority inheritance \n", msg);
	output[count1++]=msg;
	kprintf("%c is done.\n",msg);
}

void mid_prio_proc_sem( char msg)
{	
	output[count2++]=msg;
        kprintf ("%c is able to interrupt \n", msg);
	output[count2++]=msg;
        kprintf("%c is done.\n",msg);
}


void writer_sem (char msg, int sid)
{
	kprintf ("%c: Requesting the semaphore, \n", msg);
	wait(sid);
	output[count2++]=msg;
	kprintf ("%c: acquired the (semaphore), \n", msg);
	sleep(1);
	output[count2++]=msg;
	kprintf ("%c: to release semaphore\n", msg);
	signal(sid);
}

void inversion_locks ()
{
	int lck;
	int count1 =0;
	//int count2 =0;
	int p1, p2, p3;

	kprintf("\n Testing inversion using locks \n");
	lck  = lcreate ();

	p1 = create(writer_lock, 2000, 25, "Process A", 3, 'A', lck, 20);
	p2 = create(mid_prio_proc_locks, 2000, 30, "Process B", 1, 'B');
	p3 = create(writer_lock, 2000, 35, "Process C", 3, 'C', lck, 20);

	resume(p1);
	resume(p3);
	resume(p2);
	
	sleep (5);
	ldelete (lck);

	kill(p1);
	kill(p3);
	kill(p2);
	kprintf("output=%s\n", output);

	kprintf("Implementation using Locks completed \n");



}

void inversion_sem()
{	
	int sem;
	int count2 =0;
	int p1, p2, p3;

	kprintf("\n Inveriosn Using semaphores  \n");
	
	sem= screate(1);

        p1 = create(writer_sem, 2000, 25, "Process A", 2, 'A', sem);
        p2 = create(mid_prio_proc_sem, 2000, 30, "Process B", 1, 'B');
        p3 = create(writer_sem, 2000, 35, "Process C", 2, 'C', sem);


        resume(p1);
        resume(p3);
        resume(p2);
        sleep (5);
        sdelete (sem);

        kill(p1);
        kill(p2);
        kill(p3);

        kprintf("output=%s\n", output);
        kprintf("Implementation using semaphores completed \n");
}
