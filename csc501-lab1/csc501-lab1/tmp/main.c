/* user.c - main */
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <math.h>
#include <sched.h>

/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */



#define LOOP    50
int prA, prB, prC;
int proc_a(char), proc_b(char), proc_c(char);
int proc(char);
volatile int a_cnt = 0;
volatile int b_cnt = 0;
volatile int c_cnt = 0;

int i, count=0;
int main()
{	
	/*	
	int a=0,b=0,c=0,i;
        srand(1234);
        for( i=1 ;i<1000 ;i++){
        int result =  expdev(0.1);
        kprintf("dumy: %d\n" ,result);
        if( result < 10){
        a=a+1;
        }
        else if(result < 20 ){
        b=b+1;
        }
        else{
        c=c+1;
        }
        }
	
        kprintf("a count: %d b count:%d c count :%d",a,b,c);	*/
	/*srand(1234);
		
	setschedclass(EXPDISTSCHED);
		prA = create(proc_a, 2000, 10, "proc A", 1, 'A');
		prB = create(proc_b, 2000, 20, "proc B", 1, 'B');
		prC = create(proc_c, 2000, 30, "proc C", 1, 'C');
		resume(prA);
		resume(prB);
		resume(prC);
		sleep(10);
		kill(prA);
		kill(prB);
		kill(prC);
	kprintf("\nTest Result: A = %d, B = %d, C = %d\n", a_cnt, b_cnt, c_cnt);
	*/
		

	setschedclass(LINUXSCHED);
		resume(prA = create(proc, 2000, 20, "proc A", 1, 'A'));
		resume(prB = create(proc, 2000, 20, "proc B", 1, 'B'));
		resume(prC = create(proc, 2000, 20, "proc C", 1, 'C'));

		while (count++ < LOOP) {
			kprintf("M");
			for (i = 0; i < 1000000; i++)
				;
		}
//	shutdown();
//	kprintf("\n\nHello World, Xinu lives\n\n");
	
	return 0;
}

int proc_a(char c) {
	int i;
	kprintf("Start... %c\n", c);
	b_cnt = 0;
	c_cnt = 0;

	while (1) {
		for (i = 0; i < 10000; i++)
			;
		a_cnt++;
	}
	return 0;
}

int proc_b(char c) {
	int i;
	kprintf("Start... %c\n", c);
	a_cnt = 0;
	c_cnt = 0;

	while (1) {
		for (i = 0; i < 10000; i++)
			;
		b_cnt++;
	}
	return 0;
}

int proc_c(char c) {
	int i;
	kprintf("Start... %c\n", c);
	a_cnt = 0;
	b_cnt = 0;

	while (1) {
		for (i = 0; i < 10000; i++)
			;
		c_cnt++;
	}
	return 0;
}

int proc(char c) {
	int i;
	int count = 0;

	while (count++ < LOOP) {
		kprintf("%c", c);
		for (i = 0; i < 1000000; i++)
			;
	}
	return 0;
}
