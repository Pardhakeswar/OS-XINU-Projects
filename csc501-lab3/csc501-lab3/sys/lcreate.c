#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<q.h>
#include<lock.h>
#include<stdio.h>

int newlock();

SYSCALL lcreate(void)
{
	STATWORD ps;	
	int lock;
	
	//kprintf("inside lcreate\n");
	disable(ps);
	if((lock =newlock()) == SYSERR){
		//kprintf("lcreate: not able to create lock\n");
		restore(ps);
		return SYSERR;
	}
	restore (ps);	
	return(lock);

}


int newlock()
{

	int lock;
	int i;
	
	for(i=0;i<NLOCKS;i++){
		
		lock =nextlock--;
		if(nextlock< 0)
			nextlock =NLOCKS-1;
		if(locks[lock].lstate == LFREE){
			locks[lock].lstate =LUSED;
			return lock;
		}
		
	}
	return (SYSERR);

}
