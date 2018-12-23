/* xm.c = xmmap xmunmap */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <paging.h>


/*-------------------------------------------------------------------------
 * xmmap - xmmap
 *-------------------------------------------------------------------------
 */
SYSCALL xmmap(int virtpage, bsd_t source, int npages)
{	
	STATWORD ps;
	disable(ps);
	
	//kprintf("Inside xmmap system call \n")	;
	if(virtpage < 4096 || is_bsid_invalid(source)
			     ||  npages > 256 || npages <1){	
  		
		restore(ps);
		return SYSERR;
	}

	if(bsm_tab[source].bs_status == BSM_UNMAPPED || bsm_tab[source].bs_private_heap == PRIVATEHEAP ){
                
                restore(ps);
                return SYSERR;
        }
	
	if(bsm_map(currpid, virtpage, source, npages) == SYSERR){
		
		restore(ps);
		return SYSERR;
	}
	//kprintf("xmmap:succesfully executed xmmap\n");
	restore(ps);
	return OK;

}

/*-------------------------------------------------------------------------
 * xmunmap - xmunmap
 *-------------------------------------------------------------------------
 */
SYSCALL xmunmap(int virtpage)
{	
	STATWORD ps;
	disable(ps);

	if(virtpage < 4096){
		
		restore(ps);
                return SYSERR;
	}

	if(bsm_unmap(currpid, virtpage, 0) == SYSERR){
					
		restore(ps);
		return SYSERR;
	}
	
	//write_cr3(proctab[currpid].pdbr);   /*doubtful check once again*/
	
	restore(ps);
  	return OK;
}
