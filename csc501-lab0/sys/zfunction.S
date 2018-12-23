
.section .text

.global zfunction

zfunction:
	
	pushl %ebp
	movl %esp, %ebp
	pushl %ebx
	pushl %ecx
	push  %edx
	subl $4, %esp
	movl 8(%ebp), %ebx
	movl $0xfffc01ff, %edx
	andl %ebx, %edx
	shrl $8, %edx
	movl $0xff000000, %ecx
	orl  %ecx, %edx
	movl %edx, -16(%ebp)
	movl -16(%ebp),% eax   
	add $4, %esp
	pop %edx
	pop %ecx
	pop %ebx
	movl %ebp, %esp
	popl %ebp	
	#leave
	ret

