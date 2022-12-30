.text
.globl main
.type main, @function
main:
		pushq	%rbp		#[Prologue] Save the old base pointer
		movq	%rsp, %rbp		#[Prologue] Set the base pointer to the current stack pointer
		movl	$2, -4(%rbp)		#[WriteConst] move 2 into x
		movl	$10, -12(%rbp)		#[WriteConst] move 10 into z
.B1:
		movl	$8, -4(%rbp)		#[WriteConst] move 8 into x
.B3:
		movl	$8, %eax		#[Return] save return value in the result adress
		movq	%rbp, %rsp		#[Return] Move stack pointer back to where it was before the function
		popq	%rbp		#[Return] Retrieve base pointer
		ret		#[Return]
