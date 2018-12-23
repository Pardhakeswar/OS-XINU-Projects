/* pfint.c - pfint */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

sc_queue *sc_q;
ag_q *age_q;

extern page_replace_policy;

/*-------------------------------------------------------------------------
 * pfint - paging fault ISR
 *-------------------------------------------------------------------------
 */
SYSCALL pfint()
{	
	// kprintf("pfinit: starting for pid: %d \n",currpid);
 	 STATWORD ps;
	 disable(ps);
	
	int store, pageth;	 
	unsigned long fault_addr = read_cr2(); /* getting the faulted adress */

	int pd_offset = (fault_addr >>22) & (0x000003ff);
	int pt_offset = (fault_addr & 0x003ff000) >> 12; 
	pd_t* pd = proctab[currpid].pdbr + pd_offset*(sizeof(pd_t));
	
	//kprintf("fault adress 0x%08lx\n",fault_addr);
	//kprintf("pfinit: pd_offset:%d pt_offset :%d \n", pd_offset,pt_offset);
	//kprintf("pt_offset:%d\n",pt_offset);

	if(bsm_lookup(currpid, fault_addr, &store, &pageth) == SYSERR ){ /* mapping not present in backing store */
		
		//kprintf("pfinit:Not a legal adress\n");
		kill(currpid);
		restore(ps);
		return SYSERR;
	}
	//kprintf("pfinit: bs_id :%d, pageth :%d\n",store, pageth);
	if(pd->pd_pres == 0){ /*page table is not present*/
		
		//kprintf("Inside pagetable creation\n");	
		int pt_frame = create_pagetable(currpid);
		
		page_dir_entries_modify(pd, 1, 1 ,0 ,0, 0, 0, 0, 0 ,0, 0, pt_frame+FRAME0);	/* modifying the pd_entries*/	
	}	
	
	/*pagetable is present and page is not present, get a frame for page*/
	
	int page_frame;
	if(get_frm(&page_frame) == SYSERR){
		
	kprintf("pfinit:Not able to get free frame for a page frame \n");
		restore(ps);
		return SYSERR;
	}
	
	modify_frame_store(page_frame, FRM_MAPPED, fault_addr/NBPG, 1, FR_PAGE, currpid, FR_CLEAN); 
		/*frame, status, vpno, refcnt, fr_type, fr_pid, dirty*/


	/* reading from backing store to the page_frame*/

	read_bs((page_frame + FRAME0) * NBPG, store, pageth);

	
	if (page_replace_policy == SC){
		sc_insert(sc_q, page_frame);
	}
	else if(page_replace_policy == AGING){
		//kprintf("inside aging\n");
		aging_insert(age_q, page_frame, page_frame+128);
	}

	  /*look after the page replacemnet policies */
	
	pt_t* pt = (pd->pd_base)*NBPG + (pt_offset)*(sizeof(pt_t));

	page_table_entries_modify(pt, 1, 1 ,0 ,0, 0, 1, CLEAN, 0 ,0, 0, page_frame+FRAME0); /* updating the corresponding pagetable entries*/

	frm_tab[pd->pd_base -FRAME0].fr_refcnt++;
	
	write_cr3(proctab[currpid].pdbr);
	
	//kprintf("pfinit: exiting for pid:%d\n",currpid);
	restore(ps);
	return OK;
}



