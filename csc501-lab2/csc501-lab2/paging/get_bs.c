#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

int get_bs(bsd_t bs_id, unsigned int npages) {

  /* requests a new mapping of npages with ID map_id */
	
	
	//kprintf("get_bs: Inside get_bs\n");	
	if(npages > 256 || npages < 1 || is_bsid_invalid(bs_id)){
	
		return SYSERR;
	}
	/* if unmapped map with the currpid*/

	if(bsm_tab[bs_id].bs_status == BSM_UNMAPPED){
	
		if(bsm_map(currpid, 4096, bs_id, npages) == SYSERR){
                        //kprintf("get_bs: Mapping failed\n");
                        return SYSERR;
                }
                else{
			//kprintf("get_bs succesufully executed \n");
                	return npages;
		}	
	}
	
	else{
		if(bsm_tab[bs_id].bs_pid[currpid] != BADPID){   // current process have already mapping in the backing store
		
		  return bsm_tab[bs_id].bs_npages[currpid];
		}
		else{
		
			if(bsm_tab[bs_id].bs_private_heap == PRIVATEHEAP &&  
						bsm_tab[bs_id].bs_pid[currpid] == BADPID){ //currpid has no mapping and is a private backing store for mapped process
				
				return SYSERR;
			}
			else{
				bsm_tab[bs_id].bs_refcnt++;
                 		modify_bs(bs_id, BSM_MAPPED, currpid, 4096, npages, 0, 0);
				return npages;

			}
		   // If other process is not having the private heap 
 			//this case has to be handled and left out  
		}
	}
	
}


