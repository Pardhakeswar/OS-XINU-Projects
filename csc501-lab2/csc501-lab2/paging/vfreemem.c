/* vfreemem.c - vfreemem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <proc.h>
#include <paging.h>

extern struct pentry proctab[];
/*------------------------------------------------------------------------
 *  vfreemem  --  free a virtual memory block, returning it to vmemlist
 *------------------------------------------------------------------------
 */

/* Taking the reference from freemem and just changing the head pointer 
 * 	to proctab[pid].vmemlist instead of memlist */


SYSCALL	vfreemem(struct mblock *block, unsigned size)
{
	
	
	STATWORD ps;
	disable(ps);
	struct mblock *p, *q;
	
	struct pentry *pptr = &proctab[currpid];
        //struct mblock *vmlist = &(pptr->vmemlist);
	
	if(size ==0 || block < 4096*NBPG){
		restore(ps);
		return SYSERR;
	}
	size = (unsigned int) roundmb(size);
	
	for(p = &(pptr->vmemlist),q = p->mnext; q!= NULL, q<block; p=q, q=q->mnext){
		
		if(((p->mlen + (unsigned)p) > (unsigned)block && p!=&(pptr->vmemlist)) || (q!=NULL && (size+(unsigned)block > (unsigned)q))){
		restore(ps);
		return SYSERR;
		}

		if(p!=&(pptr->vmemlist) &&( p->mlen + (unsigned)p == (unsigned)block))
		p->mlen += size;
		
		else{
			block->mlen = size;
			block->mnext =q;
			p->mnext = block;
			p =block;
		}
		
		if ((unsigned) (p->mlen + (unsigned)p) == (unsigned)q){
			p->mlen +=q->mlen;
			p->mnext = q-> mnext;
		}
	}

		restore(ps);
		return OK;

}
