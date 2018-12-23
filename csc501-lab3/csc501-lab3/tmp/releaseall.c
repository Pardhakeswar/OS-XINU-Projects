	#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

/* traversing from the back*/

int release_lqueue_proc_(int ldes)
{

	struct lentry *lptr = &locks[ldes];
	struct pentry *pptr;
	
	//kprintf("release all :release lqueue function\n");	
	if(isempty(lptr->lqhead)){
                //kprintf("lqueue is empty\n");
                return -1;
        }
	
	//int next =q[lptr->lqhead].qnext;
	int hiprio_pid =q[lptr->lqtail].qprev;
	
	int max_wr_prio = get_hprio_wrt_proc(ldes);
	//kprintf("release_all(): max_wr_prio %d\n", max_wr_prio);

	if(proctab[hiprio_pid].ltype == WRITE){
		
		//kprintf("relese queue : inside high_prio write type\n");
		dequeue(hiprio_pid);
		prio_update(ldes);
		ready(hiprio_pid, RESCHYES);
	
		return OK;
	}
	int read_gr_wr;
	int prev  =q[hiprio_pid].qprev;
	while(prev!= lptr->lqhead){
		
		/*more than one same highest priority process */
		

		if(q[prev].qkey == q[hiprio_pid].qkey){
				
			if(proctab[prev].ltype == WRITE){
				
					
				if(proctab[prev].lq_enter_time - proctab[hiprio_pid].lq_enter_time < 1000){
                                //kprintf("release_queue: Inside less than 1sec waiting time and other is write \n");		
					dequeue(prev);
					prio_update(ldes);
			                ready(prev, RESCHYES);

                			return OK;

				}
			}	 
		}
		/* no more than one process with same priority but highest is read 
 		 then need to release all process which are in greater than max write*/
	
		if(proctab[prev].ltype == READ){
			if(q[prev].qkey >= max_wr_prio){
				
				//kprintf("release all (): greater than max prio and should be released \n");	
				read_gr_wr = prev;
				prev =q[prev].qprev;
				dequeue(read_gr_wr);
				ready(read_gr_wr, RESCHNO);
				continue;
				
			}
		}		
		prev = q[prev].qprev;		
	}

	/*dequeeing the highest prio proc if it is read and inserting it to ready queue*/

	dequeue(hiprio_pid);
	prio_update(ldes);
	ready(hiprio_pid, RESCHYES);
	
	return OK;

}

/*end */

int releaseall(int numlocks, int ldes)
{
	//STATWORD ps;
	
	//kprintf("releaseall(): Intilaisation\n");
	unsigned long *l;
	int i,rc;
	int rel_lock[numlocks];
	int is_valid_lock[numlocks];
	struct pentry *pptr = &proctab[currpid];
	struct lentry *lptr;
	
	l =  (unsigned long *)(&ldes) + (numlocks-1);
	for(i=0;i< numlocks; i++){
		rel_lock[(numlocks-1) -i] = *l--;
		is_valid_lock[numlocks]=0;
	}
		
	
	/*checking if all the locks are beiing held by the currprocess*/

	for (i=0;i<numlocks;i++){
		
		//lptr = &locks[rel_lock[i]];
		if(!pptr->locks[(rel_lock[i])] || isbadlock(rel_lock[i])
			|| (lptr = &locks[rel_lock[i]])->lstate == LFREE){
			//kprintf("syserror: lock not being held by curr process\n");
			is_valid_lock[i] =0;
		
		}
		else
			is_valid_lock[i] =1;

		if(is_valid_lock[i]){
			/*updating the lock and pptr data structures while releasing*/
			lptr->lheld_proc[currpid] =0;
			pptr->locks[rel_lock[i]] =0;
			pptr->ltype =NONE;

			if(lptr->ltype == READ){
				//kprintf("release all :inside read type\n ");
				--lptr->readcnt;
				//kprintf("readcnt :%d\n", lptr->readcnt);
				if(lptr->readcnt != 0){
					continue;
				}
				else {
					//kprintf("release all :inside readcnt =0 \n");
					//lptr->lheld_proc[currpid]=0;
					if((rc =release_lqueue_proc_(rel_lock[i])) == -1){
                                        	//kprintf("release_all_error :Not able to find the next process\n");
                                		lptr->ltype = NONE;
					//kprintf("After the release the lock type is %d\n", locks[rel_lock[i]].ltype);
					}
			   	}
				
			}
			else{
				//kprintf("release all: inside type write condition\n");
				//lptr->lheld_proc[currpid] =0;
				if((rc = release_lqueue_proc_(rel_lock[i])) == -1 ){
					//kprintf("release_all_error :Not able to find the next process\n");
					lptr->ltype =NONE;
					
			    }
			}	
					
		}
			
	}
	/*checking if locks to be released are acquired by the current process */	
	for(i=0; i<numlocks;i++){
		if(!is_valid_lock[i]){

			return SYSERR;
		}
	}

	return OK;
}
