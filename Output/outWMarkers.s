	movl	$111, %ebx		# Start marker bytes
	.byte	0x64, 0x67, 0x90		# Start marker bytes

.text
.globl fibonacci_i
fibonacci_i:
		pushq	%rbp		#[Prologue] Save the old base pointer
		movq	%rsp, %rbp		#[Prologue] Set the base pointer to the current stack pointer
		subq	$16, %rsp		#[Prologue] Make room on the stack for local variables
		movl	%edi, -4(%rbp)		#[ReadParam] move %edi into param:n
		movl	-4(%rbp), %eax		#[Equal] move n into EAX
		movl	$0, %edx		#[Equal] move [Temp_0] into EDX
		cmpl	%edx, %eax		#[Equal] compaire values
		sete	%al		#[Equal] set flag Equal
		cmpb	$0, %al		#[ConditionalJump] check if condition is true or false
		je	.B2		#[ConditionalJump] (jump equal) jump is prev statement is false (we compare to 0)
.B1:
		movl	$0, %eax		#[Return] save return value in the result adress
		movq	%rbp, %rsp		#[Return] Move stack pointer back to where it was before the function
		popq	%rbp		#[Return] Retrieve base pointer
		ret		#[Return]
		jmp	.B3		#[UnconditionalJump]
.B2:
		movl	-4(%rbp), %eax		#[Equal] move n into EAX
		movl	$1, %edx		#[Equal] move [Temp_2] into EDX
		cmpl	%edx, %eax		#[Equal] compaire values
		sete	%al		#[Equal] set flag Equal
		cmpb	$0, %al		#[ConditionalJump] check if condition is true or false
		je	.B5		#[ConditionalJump] (jump equal) jump is prev statement is false (we compare to 0)
		jmp	.B4		#[ConditionalJump] jump inside body
.B3:
		movl	$0, %eax		#[Return] save return value in the result adress
		movq	%rbp, %rsp		#[Return] Move stack pointer back to where it was before the function
		popq	%rbp		#[Return] Retrieve base pointer
		ret		#[Return]
.B4:
		movl	$1, %eax		#[Return] save return value in the result adress
		movq	%rbp, %rsp		#[Return] Move stack pointer back to where it was before the function
		popq	%rbp		#[Return] Retrieve base pointer
		ret		#[Return]
		jmp	.B6		#[UnconditionalJump]
.B5:
		movl	-4(%rbp), %eax		#[Minus] move n into EAX
		movl	$1, %edx		#[Minus] move [Temp_4] into EDX
		subl	%edx, %eax		#[Minus] subtract values
		movl	%eax, %edi		#[WriteParam] move param:%eax into %edi
		call	fibonacci_i		#[Call]
		movl	%eax, -8(%rbp)		# spilling nr1
		movl	-4(%rbp), %eax		#[Minus] move n into EAX
		movl	$2, %edx		#[Minus] move [Temp_7] into EDX
		subl	%edx, %eax		#[Minus] subtract values
		movl	%eax, %edi		#[WriteParam] move param:%eax into %edi
		call	fibonacci_i		#[Call]
		movl	%eax, -12(%rbp)		# spilling nr2
		movl	-8(%rbp), %eax		# restore spill nr1
		addl	-12(%rbp), %eax		# restore spill nr2
		movq	%rbp, %rsp		#[Return] Move stack pointer back to where it was before the function
		popq	%rbp		#[Return] Retrieve base pointer
		ret		#[Return]
.B6:
		jmp	.B3		#[UnconditionalJump]
.globl main
main:
		pushq	%rbp		#[Prologue] Save the old base pointer
		movq	%rsp, %rbp		#[Prologue] Set the base pointer to the current stack pointer
		subq	$16, %rsp		#[Prologue] Make room on the stack for local variables
		movl	$0, -4(%rbp)		#[WriteConst] move 0 into i
.B8:
		movl	-4(%rbp), %eax		#[LessThan] move i into EAX
		movl	$10, %edx		#[LessThan] move [Temp_15] into EDX
		cmpl	%edx, %eax		#[LessThan] compaire values
		setl	%al		#[LessThan] set flag LessThan
		cmpb	$0, %al		#[ConditionalJump] check if condition is true or false
		je	.B10		#[ConditionalJump] (jump equal) jump is prev statement is false (we compare to 0)
.B9:
		movl	-4(%rbp), %edi		#[WriteParam] move param:i into %edi
		call	fibonacci_i		#[Call]
		movl	%eax, -8(%rbp)		#[Assign] move accumulator in j
		movl	-4(%rbp), %eax		#[PlusEqual] move i into EAX
		movl	$1, %edx		#[PlusEqual] move [Temp_18] into EDX
		addl	%edx, %eax		#[PlusEqual] add values together
		movl	%eax, -4(%rbp)		#[PlusEqual] save result into i
		jmp	.B8		#[UnconditionalJump]
.B10:
		movl	-8(%rbp), %eax		#[Return] save j in the result adress
		movq	%rbp, %rsp		#[Return] Move stack pointer back to where it was before the function
		popq	%rbp		#[Return] Retrieve base pointer
		ret		#[Return]


	movl	$222, %ebx		# End marker bytes
	.byte	0x64, 0x67, 0x90		# End marker bytes

