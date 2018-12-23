#include<stdio.h>
#include<kernel.h>
#include<proc.h>
static unsigned long *esp;
void printprocstks(int priority)
{	
	int i=0;
	asm("movl %esp, esp ");
	for (i=0;i< NPROC; i++){
	if(proctab[i].pprio >= priority){	
	kprintf("\n\n void printprocstks(int priority)"
		"\nProcess :[%s]"
		"\n 	pid : %d"
		"\n 	priority :%d "
		"\n	base :0x%08x"
		"\n        limit :0x%08x"
		"\n	len: %d"
		"\n 	pointer :0x%08x", 
			proctab[i].pname, i,
			proctab[i].pprio,
			proctab[i].pbase,
			proctab[i].plimit,
			proctab[i].pstklen,
			(i == currpid)? esp: (unsigned long*) proctab[i].pesp);
		}
	}	
}
