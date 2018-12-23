#include<stdio.h>
static unsigned long *ebp,*prev_stack;
//static unsigned long *esp;
void printtos()
{	
	int i=1,a=1,b=2,c=3,d =4;
	kprintf("\n\n void printtos()");	
	asm("movl %ebp, ebp");
	asm("leal 8(%ebp), %eax");
	asm("movl %eax, prev_stack");
	//asm("movl %esp, esp");
	kprintf("\nBefore[0x%08x]: 0x%08x ",prev_stack,*(prev_stack));
	//kprintf("\nBefore[0x%08x]: 0x%08x ", ebp+2, *(ebp+2));
	kprintf("\nAfter[0x%08x]: 0x%08x ",ebp,*(ebp));
	for (;i<=4;i++)
	{
		kprintf("\nelement[0x%08x]: 0x%08x",ebp-i,*(ebp-i));
	}
	//kprintf("esp value:0x%08x",esp);
	//int j;
	//for(j=0;j<19;j++)
		//kprintf("\nelement value[0x%08x] =0x%08x",ebp-j,*(ebp-j));
	
}



