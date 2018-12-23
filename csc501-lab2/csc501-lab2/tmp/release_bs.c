#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

SYSCALL release_bs(bsd_t bs_id) {

  /* release the backing store with ID bs_id */
   	
	STATWORD ps;
	disable(ps);
	if(is_bsid_invalid(bs_id) || bsm_tab[bs_id].bs_status == BSM_UNMAPPED){
		
		restore(ps);
		return SYSERR;
	}
	if(bsm_tab[bs_id].bs_private_heap == PRIVATEHEAP){
		free_bsm(bs_id);
		
		restore(ps);
		return OK;
	}
	else{   /* shared backing store between processes*/

		bsm_tab[bs_id].bs_pid[currpid] = 0;
		bsm_tab[bs_id].bs_vpno[currpid] = 4096;
		bsm_tab[bs_id].bs_npages[currpid] = 0;
		
		if(bsm_tab[bs_id].bs_pid[currpid] != BADPID){

			bsm_tab[bs_id].bs_pid[currpid] = BADPID;
			bsm_tab[bs_id].bs_refcnt--;
		}

		if(bsm_tab[bs_id].bs_refcnt ==0){
			free_bsm(bs_id);
		}
		
		restore(ps);
		return OK;
	}	
	

}

