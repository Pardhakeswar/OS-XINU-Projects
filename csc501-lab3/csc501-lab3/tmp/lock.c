#include<conf.h>
#include<kernel.h>
#include<proc.h>
#include<q.h>
#include<lock.h>

extern unsigned long ctr1000;

struct lentry locks[NLOCKS];


/*printing the values in the queue*/
void print_q(int ldes )
{
	struct lentry *lptr = &locks[ldes];
        if(isempty(lptr->lqhead))
        	kprintf("queue is emptyt\n");

        int next;
        next = q[lptr->lqhead].qnext;
        while(next != lptr->lqtail){
                if(currpid)
                kprintf("----%s--- ", proctab[next].pname);

                next = q[next].qnext;
        }
        printf("\n\n");
        return;
}

/* caluclating the max prio in the lock wait queue*/

int max_wq_prio(int ldes)
{
	struct lentry *lptr = &locks[ldes];
	int max_prio = -1;
	if(isempty(lptr->lqhead)){
		//kprintf("max_wq_prio:lqueue is empty\n");
		return max_prio;
	}
	int next = q[lptr->lqhead].qnext;
	
	while(next != lptr->lqtail){
		if(max_prio <= proctab[next].pprio)
			max_prio = proctab[next].pprio;
		
		next =q[next].qnext;
	}
		
	return max_prio;
}

/*priority updates for the processes*/

void prio_update(int ldes)
{	
	//kprintf("prio_update: inside prio_update for %d \n", ldes);
	struct lentry *lptr = &locks[ldes];
	struct pentry *pptr;
	int max_prio_wq = max_wq_prio(ldes);
	int i;
	for(i=0; i< NPROC;i++){

		if(lptr->lheld_proc[i]){
			if((pptr = &proctab[i])->pinh < max_prio_wq){
				pptr->pprio = max_prio_wq;
				if(pptr->plock !=-1)
					prio_update(pptr->plock);
			
			}
			else{
				pptr->pprio = pptr->pinh;
				if(pptr->plock !=-1)
                                        prio_update(pptr->plock);
				
			}
		}	
	}

	return ;

}

/*getting the max prio of all write process*/

int get_hprio_wrt_proc(int ldes)
{	
	struct lentry *lptr = &locks[ldes];
	
	int high_prio_wait = -1;
	
	
	if(isempty(lptr->lqhead)){
		//kprintf("lqueue is empty\n");
		return high_prio_wait;
	}
	
	int next =q[lptr->lqhead].qnext;

	while(next !=lptr->lqtail){
		if(proctab[next].ltype == WRITE){
			if(high_prio_wait <= q[next].qkey)
				high_prio_wait =q[next].qkey;
				
		}
		next =q[next].qnext;
	}
	//kprintf("returning from hprio method\n");

	return high_prio_wait;
}


int lock(int ldes, int type, int priority)
{	
	struct lentry *lptr = &locks[ldes];
	struct pentry *pptr = &proctab[currpid];
	int i;


	if(isbadlock(ldes))
		return -1;
	
	//kprintf("intial lock type :%d \n", lptr->ltype);
	if(lptr->ltype == NONE &&  lptr->lstate != LFREE ){
		//kprintf("Inside intial case\n");
		lptr->ltype = type;
		pptr->locks[ldes] =1;
		lptr->lheld_proc[currpid] =1;
		pptr->plockret =OK;
		if(type == READ)
			++lptr->readcnt;

		return pptr->plockret;
	}
	else if(type == WRITE || (type == READ && lptr->ltype == WRITE)){
		
		//kprintf("lock: inside either type is write case\n ");		
		insert(currpid, lptr->lqhead, priority);
		pptr->plock = ldes;
		pptr->ltype = type;
		pptr->plockret = OK;
		pptr->pstate =PRWAIT;
		pptr->lq_enter_time = ctr1000;
		for(i=0; i<NPROC;i++){
			
			if(lptr->lheld_proc[i]){
				if(proctab[i].pprio <= proctab[currpid].pprio)
				prio_update(ldes);	
				break;
			}
		}
		resched();
		if(type == READ)
			++lptr->readcnt;
	
		pptr->locks[ldes]=1;
		lptr->lheld_proc[currpid]=1;
		lptr->ltype = type;
		pptr->plock = -1;
		return pptr->plockret;

	}
	else if(type == READ || lptr->ltype == READ){
		
		int prio_wt_proc = get_hprio_wrt_proc(ldes);	
		//kprintf("high_prio :%d\n", prio_wt_proc);
		if(priority <  prio_wt_proc){						  
			//kprintf("lock: prio less than waiting process prio\n");
			insert(currpid, lptr->lqhead, priority);        	
			pptr->ltype =type;
			pptr->pstate = PRWAIT;
			pptr->plockret =OK;
			pptr->plock = ldes;
			pptr->lq_enter_time = ctr1000;
			//prio_update(ldes);
			for(i=0; i<NPROC;i++){

                        	if(lptr->lheld_proc[i]){
                                	if(proctab[i].pprio <= proctab[currpid].pprio)
                                	prio_update(ldes);
                               	        break;
                        	}
                	}

	
			resched();
			++lptr->readcnt;
			lptr->ltype = type;
			pptr->plock = -1;
			pptr->locks[ldes]=1;
	                lptr->lheld_proc[currpid]=1;

			return pptr->plockret;
		}
		else{	
			//kprintf("lock() :priority greater than wait queue\n");
			++lptr->readcnt;
			pptr->locks[ldes] =1;
			lptr->lheld_proc[currpid] =1;

			return OK;
						
		   }

	 }
                	 
		
			 	
	
}
