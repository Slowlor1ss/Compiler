.text
.globl main
main:
		pushq	%rbp		#[Prologue] Save the old base pointer
		movq	%rsp, %rbp		#[Prologue] Set the base pointer to the current stack pointer
		movl	-4(%rbp), %eax		#[PlusEqual] move x into EAX
		movl	-8(%rbp), %edx		#[PlusEqual] move y into EDX
		addl	%edx, %eax		#[PlusEqual] add values together
		movl	%eax, -4(%rbp)		#[PlusEqual] save result into x
		movq	%rbp, %rsp		#[Return] Move stack pointer back to where it was before the function
		popq	%rbp		#[Return] Retrieve base pointer
		ret		#[Return]
