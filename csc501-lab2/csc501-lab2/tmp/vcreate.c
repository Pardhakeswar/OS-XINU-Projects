/* vcreate.c - vcreate */
    
#include <conf.h>
#include <i386.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <io.h>
#include <paging.h>

/*
static unsigned long esp;
*/

LOCAL	newpid();
/*------------------------------------------------------------------------
 *  create  -  create a process to start running a procedure
 *------------------------------------------------------------------------
 */
SYSCALL vcreate(procaddr,ssize,hsize,priority,name,nargs,args)
	int	*procaddr;		/* procedure address		*/
	int	ssize;			/* stack size in words		*/
	int	hsize;			/* virtual heap size in pages	*/
	int	priority;		/* process priority > 0		*/
	char	*name;			/* name (for debugging)		*/
	int	nargs;			/* number of args that follow	*/
	long	args;			/* arguments (treated like an	*/
					/* array in the code)		*/
{	

	STATWORD ps;
	disable(ps);
	bsd_t bs;
	
	//kprintf("inside vcreate\n");
	if(get_bsm(&bs) == SYSERR){
		//kprintf("vcreate: unable to get a frame\n");
		restore(ps);
		return SYSERR;
	}	
	
	int pid = create(procaddr, ssize, priority, name, nargs, args);
	
	if(isbadpid(pid)){
		
		restore(ps);
		return SYSERR;
	}	
	
	
	if(bsm_map(pid, 4096, bs, hsize) == SYSERR){
		
		//kprintf("vcreate:bsm_map failed\n");
		restore(ps);
		return SYSERR;
	}
		
		bsm_tab[bs].bs_private_heap = PRIVATEHEAP; 
		proctab[pid].store = bs;
		proctab[pid].vhpno = 4096;
		proctab[pid].vhpnpages = hsize;
		proctab[pid].vmemlist->mnext = (struct mblock *)(4096*NBPG); 
		proctab[pid].vmemlist->mlen = (int)truncew(hsize*NBPG);
	
	
		restore(ps);
		return pid;		
	
	
}

/*------------------------------------------------------------------------
 * newpid  --  obtain a new (free) process id
 *------------------------------------------------------------------------
 */
LOCAL	newpid()
{
	int	pid;			/* process id to return		*/
	int	i;

	for (i=0 ; i<NPROC ; i++) {	/* check all NPROC slots	*/
		if ( (pid=nextproc--) <= 0)
			nextproc = NPROC-1;
		if (proctab[pid].pstate == PRFREE)
			return(pid);
	}
	return(SYSERR);
}
