/* Initalisation and creation of page tables and page directories */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>

int globalpagetable[4] ;

/*
void print_frametable(int frame_no)
{
	kprintf("\n----------frame status ----\n");
	kprintf("frm_tab[%d].fr_status: %d\n",frame_no, frm_tab[frame_no].fr_status);
}
*/

int create_pagetable(int pid)
{	
	
	int i,pt_frame;
	//int pt_frame=0;
	if(get_frm(&pt_frame) == SYSERR){
		//kprintf("create_page_table: failed to get a frame\n");
		return -1;
	}

	//kprintf("create_page_table: pt_frame obtained %d\n", pt_frame);

	modify_frame_store(pt_frame, FRM_MAPPED, -1, 0, FR_TBL, pid, FR_CLEAN); /* doubt in frame count*/
        /*frame, status, vpno, refcnt, fr_type, fr_pid, dirty*/
	
	
	//print_frametable(pt_frame);
	int page_table_base_frame = FRAME0+ pt_frame;
	
	
	for(i=0; i< PTE_ENTRIES;i++){
		pt_t *pt = (page_table_base_frame)*NBPG + i*(sizeof(pt_t));
		
		/*initilaising the page table entry*/
		
		page_table_entries_modify(pt, 0, 1 ,0 ,0, 0, 0, CLEAN, 0 ,0, 0, 0);
			
	}

	return pt_frame;
}


int create_pagedirectory(int pid)
{	
	//kprintf("create_pagedirectory: starting for pid:%d\n",pid);
	int pd_frame,i;
	if(get_frm(&pd_frame) == SYSERR){
		//kprintf("create_page_dir: failed to get a frame\n");
		return -1;
	}
	
	
	modify_frame_store(pd_frame, FRM_MAPPED, -1, 0, FR_DIR, pid, FR_CLEAN);
	/*frame, status, vpno, refcnt, fr_type, fr_pid, dirty*/

	int page_dir_base_adress = (FRAME0+ pd_frame)*NBPG;
	//kprintf("page_dir_base_adress : %d\n",page_dir_base_adress);
	proctab[pid].pdbr = page_dir_base_adress;
	
	for(i=0; i< PDE_ENTRIES; i++){
		
		pd_t *pd = (proctab[pid].pdbr) + i*(sizeof(pd_t));
                /*initilaising the page table entry*/
	
		if(i<4){
		//page_dir_entries_modify(pd, 1, 1 ,0 ,0, 0, 0, 0, 0 ,0, 0, globalpagetable[i]);
		
		page_dir_entries_modify(pd, 1, 1 ,0 ,0, 0, 0, 0, 0 ,0, 0, FRAME0+i);
		//kprintf("page_directory :pd->pd_base %d\n", pd->pd_base);
		frm_tab[pd_frame].fr_refcnt++;
	   }
		else
		page_dir_entries_modify(pd, 0, 1 ,0 ,0, 0, 0, 0, 0 ,0, 0, 0);	
	}	
	
	//kprintf("create_pagedirectory: exiting for pid %d\n",pid);
	return pd_frame;
}



int global_page_tables()
{	
	//kprintf("global_page_tables: Starting for pid %d\n", currpid);
	int i,j;
	int frame;
	for(i=0; i<4; i++){
		
		frame = create_pagetable(NULLPROC);
		globalpagetable[i] = FRAME0 + frame;
		//kprintf("gloablpagetable_value -- :%d\n",globalpagetable[i]);
	
		for(j=0; j< PTE_ENTRIES; j++){
			
		pt_t *pt = (globalpagetable[i])*NBPG + (j*(sizeof(pt_t)));
                /*initilaising the page table entry*/
			
		page_table_entries_modify(pt, 1, 1 ,0 ,0, 0, 0, CLEAN, 0 ,0, 0, j+i*FRAME0);	  
		frm_tab[frame].fr_refcnt ++;
	     }
		
				
	}
	
	//kprintf("global_page_tables: Exiting for pid\n",currpid);
	return OK;
}


void page_table_entries_modify(pt_t *pt, int pres, int write, int user, int pwt,
                                int pcd, int acc, int dirty, int mbz,
                                        int global, int avail, int base)

{
		pt->pt_pres  = pres;
                pt->pt_write = write;
                pt->pt_user  = user;
                pt->pt_pwt   = pwt;
                pt->pt_pcd   = pcd;
                pt->pt_acc   = acc;
                pt->pt_dirty = dirty;    
                pt->pt_mbz   = mbz;
                pt->pt_global= global;
                pt->pt_avail = avail;
                pt->pt_base  = base;
	
	return;
}


void page_dir_entries_modify(pd_t *pd, int pres, int write, int user, int pwt,
				int pcd, int acc, int fmb, int mbz,
                                   	int global, int avail, int base)
{	
	
		pd->pd_pres  = pres;
                pd->pd_write = write;
                pd->pd_user  = user;
                pd->pd_pwt   = pwt;
                pd->pd_pcd   = pcd;
                pd->pd_acc   = acc;
                pd->pd_fmb   = fmb;    
                pd->pd_mbz   = mbz;
                pd->pd_global= global;
                pd->pd_avail = avail;
                pd->pd_base  = base;
	
	return;

}


