/* bsm.c - manage the backing store mapping*/

#include <conf.h>
#include <kernel.h>
#include <paging.h>
#include <proc.h>
bs_map_t bsm_tab[NUM_BACK_STORES];
sc_queue *sc_q;
ag_q *age_q;
extern page_replace_policy;

/*-------------------------------------------------------------------------
 * init_bsm- initialize bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL init_bsm()
{	
	STATWORD ps;
        disable(ps);

	int i;
        for (i=0;i<NUM_BACK_STORES;i++)
        {	
                	
		modify_bs(i, BSM_UNMAPPED, BADPID, 0, 0, 0, 0);
        }

	restore(ps);
        return OK;
}

/*-------------------------------------------------------------------------
 * get_bsm - get a free entry from bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL get_bsm(int* avail)
{	
	
	STATWORD ps;
        disable(ps);

	int i;
        for(i=0; i< NUM_BACK_STORES ;i++)
        {
                if(bsm_tab[i].bs_status == BSM_UNMAPPED)
                        *avail =i;
             
                restore(ps);
                return OK;
        }
	restore(ps);
	return SYSERR;
}


/*-------------------------------------------------------------------------
 * free_bsm - free an entry from bsm_tab 
 *-------------------------------------------------------------------------
 */
SYSCALL free_bsm(int i)
{	
	STATWORD ps;
	disable(ps);

	if(is_bsid_invalid(i)){
		restore(ps);
		return SYSERR;
	}
	else{
		modify_bs(i, BSM_UNMAPPED, BADPID, 0, 0, 0, 0);
		restore(ps);
		return OK;
	}
	
}


/*-------------------------------------------------------------------------
 * bsm_lookup - lookup bsm_tab and find the corresponding entry
 *-------------------------------------------------------------------------
 */

SYSCALL bsm_lookup(int pid, long vaddr, int* store, int* pageth)
{	
	STATWORD ps;
        disable(ps);
	
	if(isbadpid(pid)){
		restore(ps);
		return SYSERR;
	}
	int i;
	
	//kprintf("Bsm_lookup: Intilasing bsm_lookup\n");
	int vpno = vaddr/NBPG; 
	
	for (i=0 ; i < NUM_BACK_STORES ;i++)
	{
		if(bsm_tab[i].bs_pid[pid] != BADPID && vpno >= bsm_tab[i].bs_vpno[pid])
		{	/*one more condition to include check and include it*/
			*store =i;
			*pageth = vpno -bsm_tab[i].bs_vpno[pid];
			 
			 //kprintf("bsm_lookup: succesfully executed bsm_lookup\n");
			 restore(ps);
			 return OK;
		}
	}
	
	if(is_bsid_invalid(i)){
		restore(ps);
		return SYSERR;
	}
	
}


/*-------------------------------------------------------------------------
 * bsm_map - add an mapping into bsm_tab 
 *-------------------------------------------------------------------------
 */

SYSCALL bsm_map(int pid, int vpno, int source, int npages)
{	
	STATWORD ps;
	disable(ps);
	
	//kprintf("bsm_map: Initialising the bsm_map\n");
	if(npages > 256 || npages <1 ||
			is_bsid_invalid(source)|| isbadpid(pid)){
		
		restore(ps);
		return SYSERR;
		
	}

	 bsm_tab[source].bs_refcnt++;
	 modify_bs(source, BSM_MAPPED, pid, vpno, npages, 0, 0);
	 
	 //kprintf("bsm_map: End of Bsm_map\n");
	 restore(ps);
         return OK;
	
	
}



/*-------------------------------------------------------------------------
 * bsm_unmap - delete an mapping from bsm_tab
 *-------------------------------------------------------------------------
 */
SYSCALL bsm_unmap(int pid, int vpno, int flag)
{	
	STATWORD ps;
        disable(ps);

	if(isbadpid(pid)){
		
		restore(ps);
		return SYSERR;
	}
	int bs_id, pageth;
	node *del_node = sc_q->front;
	
	//kprintf("bsm:unmap Initilasing bsm_unmap\n");
	if(bsm_lookup(pid, vpno*NBPG, &bs_id, &pageth)== SYSERR){
		
		restore(ps);
		return SYSERR;
	}
	

	int vpno_base = bsm_tab[bs_id].bs_vpno[pid];
	int page_no;
	int npages = bsm_tab[bs_id].bs_npages[pid];

	/* travesing and freeing the pages the pid have mapped in bs_store*/
	for(page_no =vpno_base; page_no <= vpno_base + npages; page_no++)
	{
		int virt_adress = page_no*NBPG;

		/* getting the page directory pointer*/

		pd_t* pd = proctab[pid].pdbr + ((virt_adress)>>22)*(sizeof(pd_t));
		int pt_offset = (virt_adress & 0x003ff000)>>12;

		if(pd->pd_pres ==1){
			
			pt_t * pt = pd->pd_base*NBPG + (pt_offset)*(sizeof(pt_t));

			if(pt->pt_pres == 1){
				
				if(frm_tab[pt->pt_base - FRAME0].fr_status == FRM_MAPPED){
					
					write_bs(pt->pt_base * 4096, bs_id, pageth );
					frm_tab[(pt->pt_base) - FRAME0].fr_refcnt--;
			
					if(frm_tab[(pt->pt_base) - FRAME0].fr_refcnt<=0){
						

			/* finding the  node to be deleted in the sc_queue */
			if(page_replace_policy == SC){
	
				if( sc_q->front == sc_q->rear && del_node->data == pt->pt_base - FRAME0)
					sc_delete(sc_q, del_node);

				else{
					
					while( del_node->next != sc_q->front){
						if( del_node->data == pt->pt_base - FRAME0)
							break;
					del_node = del_node->next;
					}
					sc_delete(sc_q, del_node);
				    }
				}
			
			 if(page_replace_policy == AGING){

					aging_delete(age_q, pt->pt_base - FRAME0);
				}
					free_frm(pt->pt_base - FRAME0);        // free the frame 
			   }
				
		      }
		}	
	  }
    }
	
	/* if multiple process are sharing then need to free_bsm if bsm_refcnt becomes zero*/
	
	modify_bs(bs_id, BSM_UNMAPPED, pid, -1, 0, 0, 0);
	bsm_tab[bs_id].bs_refcnt--;
	if(bsm_tab[bs_id].bs_refcnt <=0)   /* if no process is mapping need to free it to zero*/
		free_bsm(bs_id);
	
	//kprintf("bsm_unmap: Exiting\n ");	
	restore(ps);
	return OK;
	
}

void modify_bs(bsd_t bsd, int status, int pid, int vpno, int npages, int sem, int is_private_heap)
{	
	int j;
	bsm_tab[bsd].bs_status = status;

	/* Initialising all the pid, vpno, npages array with */
	if(bsm_tab[bsd].bs_status == BSM_UNMAPPED && pid == BADPID){
		
		for (j=0;j<NPROC;j++){
			bsm_tab[bsd].bs_pid[j] = pid;
			bsm_tab[bsd].bs_vpno[j] =vpno;
			bsm_tab[bsd].bs_npages[j] =npages;
		}
	}
	else if (bsm_tab[bsd].bs_status == BSM_MAPPED && pid != BADPID){ /* updating with corresponding pid entry */
		bsm_tab[bsd].bs_pid[pid] = pid;
                bsm_tab[bsd].bs_vpno[pid] =vpno;
                bsm_tab[bsd].bs_npages[pid] =npages;
	}
	else if (bsm_tab[bsd].bs_status == BSM_UNMAPPED && pid != BADPID){ /* unmapping for only partcilar pid*/
		
		bsm_tab[bsd].bs_pid[pid] = BADPID;
                bsm_tab[bsd].bs_vpno[pid] = -1;
                bsm_tab[bsd].bs_npages[pid] = 0;

	}
	bsm_tab[bsd].bs_sem = sem;
	bsm_tab[bsd].bs_private_heap =is_private_heap;
	
	return ;
}	
