/* kill.c - kill */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <io.h>
#include <q.h>
#include <stdio.h>
#include <paging.h>

/*------------------------------------------------------------------------
 * kill  --  kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
SYSCALL kill(int pid)
{
	STATWORD ps;    
	struct	pentry	*pptr;		/* points to proc. table for pid*/
	int	dev;

	disable(ps);
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate==PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	if (--numproc == 0)
		xdone();

	dev = pptr->pdevs[0];
	if (! isbaddev(dev) )
		close(dev);
	dev = pptr->pdevs[1];
	if (! isbaddev(dev) )
		close(dev);
	dev = pptr->ppagedev;
	if (! isbaddev(dev) )
		close(dev);
	
	/* freeing the backing store and frames current process have */	
	int i,j;
	
	for(i=0 ;i < NUM_BACK_STORES; i++){
			
		if(bsm_tab[i].bs_pid[pid] != BADPID){
			
			if(bsm_unmap(pid, bsm_tab[i].bs_vpno[pid], 1)==SYSERR){ /* look after the flag bit */
				restore(ps);
				return SYSERR;
			}
		}
	}
	
	//free_frm((proctab[pid].pdbr >> 12) -FRAME0 );
	for (i=4; i<NFRAMES;i++){
		if(frm_tab[i].fr_type == FR_TBL && frm_tab[i].fr_status == FRM_MAPPED && frm_tab[i].fr_pid == pid){
			free_frm(i);
		}
	}
	free_frm((proctab[pid].pdbr/NBPG) -FRAME0);
	proctab[pid].pdbr=0;
	send(pptr->pnxtkin, pid);

	freestk(pptr->pbase, pptr->pstklen);
	switch (pptr->pstate) {

	case PRCURR:	pptr->pstate = PRFREE;	/* suicide */
			resched();

	case PRWAIT:	semaph[pptr->psem].semcnt++;

	case PRREADY:	dequeue(pid);
			pptr->pstate = PRFREE;
			break;

	case PRSLEEP:
	case PRTRECV:	unsleep(pid);
						/* fall through	*/
	default:	pptr->pstate = PRFREE;
	}
	restore(ps);
	return(OK);
}
