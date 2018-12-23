/* frame.c - manage physical frames */
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

//defining the frame global sructures

fr_map_t frm_tab[NFRAMES];
extern int page_replace_policy ;

/*-------------------------------------------------------------------------
 * init_frm - initialize frm_tab
 *-------------------------------------------------------------------------
 */

/*
void free_frame_list(){

	int i=0;
	kprintf("\n===========Frame Status==========\n");
	for(i=0; i< 60; i++){
		//kprintf("\n===========Frame Status==========\n");
		kprintf("frm_tab[%d].fr_status: %d frm_vpno : %d\n", i,frm_tab[i].fr_status,frm_tab[i].fr_vpno);	
	}
	return;	

}
*/

SYSCALL init_frm()
{
	STATWORD ps;
	disable(ps);
	int i ;
	for (i=0; i< NFRAMES ;i++)
	{	
		modify_frame_store(i, FRM_UNMAPPED, -1, 0, -1, BADPID, FR_CLEAN);
		/*frame, status, vpno, refcnt, fr_type, fr_pid, dirty*/
	}
	restore(ps);
 	return OK;
}

/*-------------------------------------------------------------------------
 * get_frm - get a free frame according page replacement policy
 *-------------------------------------------------------------------------
 */

SYSCALL get_frm(int* avail)
{
	STATWORD ps;
        disable(ps);

	int i;
	for(i=0; i< NFRAMES;i++)
	{	
		if(frm_tab[i].fr_status == FRM_UNMAPPED){
			//kprintf("frme eneetred %d\n",i);
			*avail = i;
			//sc_flag =1;			
			restore(ps);
			return OK;
		}
	}
	/* Getting a frame using page replacement policies */
			
	if(page_replace_policy == SC){
	
		int frame_sc = frm_replace_SC();
		free_frm(frame_sc); 
		*avail = frame_sc;

		restore(ps);
		return OK;
	}	
	if(page_replace_policy == AGING){
		
		int frame_ag = frm_replace_aging();
		//kprintf("frame obtained was %d\n",frame_ag);
		free_frm(frame_ag);
		*avail = frame_ag;
		
		restore(ps);
		return OK;
		 	
	}
	 
	restore(ps);
	return SYSERR;
}



/*-------------------------------------------------------------------------
 * free_frm - free a frame 
 *-------------------------------------------------------------------------
 */


SYSCALL free_frm(int i)
{
	STATWORD ps;
	disable(ps);
	
	if(frm_tab[i].fr_type == FR_PAGE){
		
		int bs_id, pageth;
		if(bsm_lookup(frm_tab[i].fr_pid, frm_tab[i].fr_vpno * NBPG, &bs_id, &pageth) == SYSERR){
		
		restore(ps);
		return SYSERR;
	    }
	//kprintf("free_ frame enetered for %d frame\n",i, frm_tab[i].fr_vpno);	
	write_bs((i + FRAME0) * 4096, bs_id, pageth);
	int va_addr = frm_tab[i].fr_vpno*NBPG;
	
	int pd_offset = (va_addr >>22) & (0x000003ff);
        int pt_offset = (va_addr & 0x003ff000) >> 12;
	
	pd_t* pd = proctab[frm_tab[i].fr_pid].pdbr + pd_offset*(sizeof(pd_t));
	pt_t* pt = (pd->pd_base)*NBPG + pt_offset*(sizeof(pt_t));
	
		if(!pt->pt_pres){
			
			restore(ps);
			return SYSERR;
	   	}
		else{
			page_table_entries_modify(pt, 0, 1 ,0 ,0, 0, 0, CLEAN, 0 ,0, 0, 0);
		}
		modify_frame_store(i, FRM_UNMAPPED, -1, 0, -1, BADPID, FR_CLEAN);
		
		/* if the ref_cnt of page table becomes zero then need to free page tabele*/

		frm_tab[pd->pd_base - FRAME0].fr_refcnt--;
		if (frm_tab[pd->pd_base - FRAME0].fr_refcnt == 0) {
			free_frm(pd->pd_base - FRAME0);
		}
		restore(ps);
		return OK;

    	}
	else{ 			/* This case is for FR_DIR & FR_TBL */
		if(i >3){
			
			modify_frame_store(i, FRM_UNMAPPED, -1, 0, -1, BADPID, FR_CLEAN);
		}

		restore (ps);
		return OK;
	}
	
	restore(ps);
	return SYSERR;
}


void modify_frame_store(int frame_no, int status, int vpno, int refcnt, 
					int type, int pid, int dirty)
{
	frm_tab[frame_no].fr_status = status;
	frm_tab[frame_no].fr_vpno   = vpno;
	frm_tab[frame_no].fr_refcnt = refcnt;
	frm_tab[frame_no].fr_type   = type;
	frm_tab[frame_no].fr_pid    = pid;
	frm_tab[frame_no].fr_dirty  = dirty;
	
	return;	
}
