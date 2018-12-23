/* vgetmem.c - vgetmem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <proc.h>
#include <paging.h>

extern struct pentry proctab[];
/*------------------------------------------------------------------------
 * vgetmem  --  allocate virtual heap storage, returning lowest WORD address
 *------------------------------------------------------------------------
 */

/* Taking the reference from getmem and just changing the inital mblck head*/

WORD	*vgetmem(nbytes)
	unsigned nbytes;
{	
	STATWORD ps;    
	disable(ps);

	struct  mblock  *prev, *next, *leftover;
	
	struct mblock *vmlist;
	vmlist = proctab[currpid].vmemlist;
	
	if (nbytes==0 || vmlist->mnext== (struct mblock *) NULL) {
		restore(ps);
		return( (WORD *)SYSERR);
	}

	nbytes = (unsigned int) roundmb(nbytes);

	for (prev= vmlist,next=vmlist->mnext ;
     		next != (struct mblock *) NULL ;
     		prev=next,next=next->mnext)
		if ( prev->mlen == nbytes) {
                        prev->mnext = (struct mblock*)((unsigned)next + nbytes);
                        prev->mlen =0;
                        restore(ps);
                        return( (WORD *)next );
                } 
		else if ( prev->mlen > nbytes ) {
                        leftover = (struct mblock *)( (unsigned)next + nbytes );
                        prev->mnext = leftover;
                        prev->mlen = prev->mlen - nbytes;
						restore(ps);
                        return( (WORD *)next );
                }

	restore(ps);
	return OK;
}


