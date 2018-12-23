#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <math.h>
#include <sched.h>

extern int a_call,b_call,c_call,aaa,bbb,ccc;

void setschedclass (int sched_class){
        sched_class_ = sched_class;
}

int getschedclass(){
        return sched_class_;
}

/*
void print_q(int head )
{
        if(isempty(head))
        kprintf("redyqueue is emptyt\n");

        int next;
        next = q[head].qnext;
        while(next != rdytail){
                if(currpid)
                kprintf("----%s--- ", proctab[next].pname);

                next = q[next].qnext;
        }
        printf("\n\n");
        return;
}

*/					/*calculating the exponential function based on distribution function */

int get_exp_proc(int rval){
	
		
	//if(firstkey(rdyhead)==lastkey(rdytail))
	//	return NULLPROC;
	
	int next =q[rdyhead].qnext;	
	while(q[next].qkey <= rval &&  next < NPROC){
		next =q[next].qnext;
	}
	if( next < NPROC){
	
		while(q[next].qkey == q[q[next].qnext].qkey && next < NPROC)         //handling the round robin case by taking last element
			next =q[next].qnext;	
		return next;
	}
	else
	 	return q[next].qprev;
}


int expdist_sched(){

	 register struct pentry  *optr;  /* pointer to old process entry */
         register struct pentry  *nptr;  /* pointer to new process entry */
	
	if(isempty(rdyhead))
		return(OK);
	
	optr = &proctab[currpid];
	//if(currpid)
 	//print_q(rdyhead);		
	if(optr->pstate == PRCURR){
		optr->pstate =PRREADY;
		insert(currpid, rdyhead, optr->pprio);
	}

	int random_value =(int) expdev(0.1);
	int exp_proc = get_exp_proc(random_value);
	//kprintf("random_value :%d exp_proc :%d\n",random_value, exp_proc);	
	nptr =&proctab[currpid =dequeue(exp_proc)];
	nptr->pstate =PRCURR;
#ifdef RTCLOCK
	preempt = QUANTUM;
#endif

	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
                return(OK);	
}


	
int max_goodness(int head){

        int next = q[head].qnext;
        int max_goodness =0;
        int max_good_pid = next;
        while(next != rdytail){

                if(max_goodness < proctab[next].goodness)
                        {
                                max_goodness = proctab[next].goodness;
                                max_good_pid = next;
                        }
                next =q[next].qnext;
        }
        return max_good_pid;
}



int linux_sched(){
	
	register struct pentry  *optr;  /* pointer to old process entry */
        register struct pentry  *nptr;  /* pointer to new process entry */

	optr = &proctab[currpid];
	int i, oldpprio, epoch =0;
	
	
	if(preempt <= 0 ){                     //if time quantun is used up goodness value becomes zero and gets executed in next epoch
		optr->goodness =0;
		optr->remain =0;
	}
	else{
		oldpprio =optr->goodness -optr->remain;   //any priority changes should take place in new epoch;
		optr->goodness = oldpprio + preempt;   //updating the goodness value when reschedule gets called
		optr->remain = preempt;
	}
	
	
	//if(currpid)
	//kprintf("\n currpid :%d preempt :%d - goodnes %d -remian %d \n", currpid,preempt, optr->goodness,optr->remain); 
	
	if(optr->pstate ==PRCURR){
		optr->pstate =PRREADY;
		insert(currpid, rdyhead, optr->pprio);	
	}
	

	int good_max = max_goodness(rdyhead);
	//if(currpid )
	//kprintf("good_max ;%d", good_max);

	if( (proctab[good_max].goodness == 0) && 
		(((optr->remain ==0 ))|| optr->pstate != PRCURR)){
			
		if(epoch == 0){
			//if(currpid)
			//kprintf("\n  inside new epoch \n");
		
			for (i=0; i< NPROC ;i++){
				
				if(proctab[i].pstate ==PRFREE)
					continue;
				else{
					if(proctab[i].remain == proctab[i].quantum || proctab[i].remain ==0)
						proctab[i].quantum = proctab[i].pprio;
					else
						proctab[i].quantum = proctab[i].pprio + (int)(proctab[i].remain)/2;
					
					proctab[i].remain = proctab[i].quantum;
					proctab[i].goodness = proctab[i].pprio +proctab[i].remain;
					
				    }		
			}
			epoch =1;
		

		good_max = max_goodness(rdyhead); // calculating the max goodness after epoch creation
			
	      }
								
	 if((proctab[good_max].goodness == 0 ) && epoch ==1){   //Null process handling      
			if(currpid == NULLPROC)
				return(OK);
			}	
	}	
	//if(currpid)
	//print_q_linux(rdyhead);
	//if(currpid)
	//kprintf("epoch %d\n",good_max_epoch);
	currpid = good_max;
	//if(currpid)
	//kprintf("currpid :%d, max epoch :%d\n",currpid, good_max_epoch);
	nptr =&proctab[dequeue(good_max)];
	nptr->pstate =PRCURR;
	
	if(currpid == NULLPROC){
	#ifdef  RTCLOCK
        preempt = QUANTUM;
        #endif
	}
	else
	preempt = nptr->remain; 

	ctxsw((int) &optr->pesp, (int) optr->pirmask, (int) &nptr->pesp, (int) nptr->pirmask);
	return(OK);
		

}
