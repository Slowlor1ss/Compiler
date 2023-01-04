.text
.globl test_arithmetic
.type test_arithmetic, @function
test_arithmetic:
		pushq	%rbp		#[Prologue] Save the old base pointer
		movq	%rsp, %rbp		#[Prologue] Set the base pointer to the current stack pointer
		movl	$24, %eax		#[Return] save return value in the result adress
		movq	%rbp, %rsp		#[Return] Move stack pointer back to where it was before the function
		popq	%rbp		#[Return] Retrieve base pointer
		ret		#[Return]
.globl test_if_ii
.type test_if_ii, @function
test_if_ii:
		pushq	%rbp		#[Prologue] Save the old base pointer
		movq	%rsp, %rbp		#[Prologue] Set the base pointer to the current stack pointer
		movl	%esi, -8(%rbp)		#[ReadParam] move %esi into param:y
		movl	%edi, -4(%rbp)		#[ReadParam] move %edi into param:x
		movl	-4(%rbp), %eax		#[GreaterThan] move x into EAX
		movl	$4, %edx		#[GreaterThan] move [Temp_7] into EDX
		cmpl	%edx, %eax		#[GreaterThan] compaire values
		setg	%al		#[GreaterThan] set flag GreaterThan
		movb	%al, -17(%rbp)		#[GreaterThan] save result into [Temp_8]
		cmpb	$0, -17(%rbp)		#[ConditionalJump] check if condition is true or false
		je	.B3		#[ConditionalJump] (jump equal) jump is prev statement is false (we compare to 0)
		jmp	.B2		#[ConditionalJump] jump inside body
.B2:
		movl	$1, -12(%rbp)		#[WriteConst] move 1 into a
		jmp	.B4		#[UnconditionalJump]
.B3:
.B4:
		movl	-8(%rbp), %eax		#[LessThan] move y into EAX
		movl	$10, %edx		#[LessThan] move [Temp_11] into EDX
		cmpl	%edx, %eax		#[LessThan] compaire values
		setl	%al		#[LessThan] set flag LessThan
		movb	%al, -26(%rbp)		#[LessThan] save result into [Temp_12]
		cmpb	$0, -26(%rbp)		#[ConditionalJump] check if condition is true or false
		je	.B6		#[ConditionalJump] (jump equal) jump is prev statement is false (we compare to 0)
		jmp	.B5		#[ConditionalJump] jump inside body
.B5:
		movl	$1, -21(%rbp)		#[WriteConst] move 1 into b
		jmp	.B7		#[UnconditionalJump]
.B6:
		movl	$0, -21(%rbp)		#[WriteConst] move 0 into b
.B7:
		movl	-12(%rbp), %eax		#[Plus] move a into EAX
		movl	-21(%rbp), %edx		#[Plus] move b into EDX
		addl	%edx, %eax		#[Plus] add values together
		movl	%eax, -30(%rbp)		#[Plus] save result into [Temp_15]
		movl	-30(%rbp), %eax		#[Return] save return value in the result adress
		movq	%rbp, %rsp		#[Return] Move stack pointer back to where it was before the function
		popq	%rbp		#[Return] Retrieve base pointer
		ret		#[Return]
.globl test_while
.type test_while, @function
test_while:
		pushq	%rbp		#[Prologue] Save the old base pointer
		movq	%rsp, %rbp		#[Prologue] Set the base pointer to the current stack pointer
		movl	$0, -4(%rbp)		#[WriteConst] move 0 into i
		jmp	.B9		#[UnconditionalJump]
.B9:
		movl	-4(%rbp), %eax		#[LessThan] move i into EAX
		movl	$5, %edx		#[LessThan] move [Temp_17] into EDX
		cmpl	%edx, %eax		#[LessThan] compaire values
		setl	%al		#[LessThan] set flag LessThan
		movb	%al, -9(%rbp)		#[LessThan] save result into [Temp_18]
		cmpb	$0, -9(%rbp)		#[ConditionalJump] check if condition is true or false
		je	.B11		#[ConditionalJump] (jump equal) jump is prev statement is false (we compare to 0)
		jmp	.B10		#[ConditionalJump] jump inside body
.B10:
		movl	-4(%rbp), %eax		#[Plus] move i into EAX
		movl	$1, %edx		#[Plus] move [Temp_19] into EDX
		addl	%edx, %eax		#[Plus] add values together
		movl	%eax, -17(%rbp)		#[Plus] save result into [Temp_20]
		movl	-17(%rbp), %eax		#[Assign] move [Temp_20] in accumulator
		movl	%eax, -4(%rbp)		#[Assign] move accumulator in i
		jmp	.B9		#[UnconditionalJump]
.B11:
		movl	$0, -13(%rbp)		#[WriteConst] move 0 into j
		jmp	.B12		#[UnconditionalJump]
.B12:
		movl	-13(%rbp), %eax		#[LessThan] move j into EAX
		movl	$10, %edx		#[LessThan] move [Temp_22] into EDX
		cmpl	%edx, %eax		#[LessThan] compaire values
		setl	%al		#[LessThan] set flag LessThan
		movb	%al, -18(%rbp)		#[LessThan] save result into [Temp_23]
		cmpb	$0, -18(%rbp)		#[ConditionalJump] check if condition is true or false
		je	.B14		#[ConditionalJump] (jump equal) jump is prev statement is false (we compare to 0)
		jmp	.B13		#[ConditionalJump] jump inside body
.B13:
		movl	-13(%rbp), %eax		#[Plus] move j into EAX
		movl	$1, %edx		#[Plus] move [Temp_24] into EDX
		addl	%edx, %eax		#[Plus] add values together
		movl	%eax, -26(%rbp)		#[Plus] save result into [Temp_25]
		movl	-26(%rbp), %eax		#[Assign] move [Temp_25] in accumulator
		movl	%eax, -13(%rbp)		#[Assign] move accumulator in j
		movl	-13(%rbp), %eax		#[Equal] move j into EAX
		movl	$5, %edx		#[Equal] move [Temp_26] into EDX
		cmpl	%edx, %eax		#[Equal] compaire values
		sete	%al		#[Equal] set flag Equal
		movb	%al, -23(%rbp)		#[Equal] save result into [Temp_27]
		cmpb	$0, -23(%rbp)		#[ConditionalJump] check if condition is true or false
		je	.B16		#[ConditionalJump] (jump equal) jump is prev statement is false (we compare to 0)
		jmp	.B15		#[ConditionalJump] jump inside body
		jmp	.B15		#[UnconditionalJump]
.B14:
		movl	-4(%rbp), %eax		#[Plus] move i into EAX
		movl	-13(%rbp), %edx		#[Plus] move j into EDX
		addl	%edx, %eax		#[Plus] add values together
		movl	%eax, -22(%rbp)		#[Plus] save result into [Temp_29]
		movl	-22(%rbp), %eax		#[Return] save return value in the result adress
		movq	%rbp, %rsp		#[Return] Move stack pointer back to where it was before the function
		popq	%rbp		#[Return] Retrieve base pointer
		ret		#[Return]
.B15:
		movl	-4(%rbp), %eax		#[Plus] move i into EAX
		movl	-13(%rbp), %edx		#[Plus] move j into EDX
		addl	%edx, %eax		#[Plus] add values together
		movl	%eax, -27(%rbp)		#[Plus] save result into [Temp_28]
		movl	-27(%rbp), %eax		#[Return] save return value in the result adress
		movq	%rbp, %rsp		#[Return] Move stack pointer back to where it was before the function
		popq	%rbp		#[Return] Retrieve base pointer
		ret		#[Return]
		jmp	.B16		#[UnconditionalJump]
.B16:
.globl test_function_calls
.type test_function_calls, @function
test_function_calls:
		pushq	%rbp		#[Prologue] Save the old base pointer
		movq	%rsp, %rbp		#[Prologue] Set the base pointer to the current stack pointer
		subq	$48, %rsp		#[Prologue] Make room on the stack for local variables
		call	test_arithmetic		#[Call]
		movl	%eax, -8(%rbp)		#[Call] Save Func Return value into [Temp_30]
		movl	-8(%rbp), %eax		#[Assign] move [Temp_30] in accumulator
		movl	%eax, -4(%rbp)		#[Assign] move accumulator in k
		movl	$6, %esi		#[WriteParam] move param:[Temp_32] into %esi
		movl	$8, %edi		#[WriteParam] move param:x into %edi
		call	test_if_ii		#[Call]
		movl	%eax, -16(%rbp)		#[Call] Save Func Return value into [Temp_33]
		movl	-16(%rbp), %eax		#[Assign] move [Temp_33] in accumulator
		movl	%eax, -12(%rbp)		#[Assign] move accumulator in l
		call	test_while		#[Call]
		movl	%eax, -20(%rbp)		#[Call] Save Func Return value into [Temp_34]
		movl	-20(%rbp), %eax		#[Assign] move [Temp_34] in accumulator
		movl	%eax, -16(%rbp)		#[Assign] move accumulator in m
		movl	-4(%rbp), %eax		#[Plus] move k into EAX
		movl	-12(%rbp), %edx		#[Plus] move l into EDX
		addl	%edx, %eax		#[Plus] add values together
		movl	%eax, -20(%rbp)		#[Plus] save result into [Temp_35]
		movl	-20(%rbp), %eax		#[Plus] move [Temp_35] into EAX
		movl	-16(%rbp), %edx		#[Plus] move m into EDX
		addl	%edx, %eax		#[Plus] add values together
		movl	%eax, -24(%rbp)		#[Plus] save result into [Temp_36]
		movl	-24(%rbp), %eax		#[Return] save return value in the result adress
		movq	%rbp, %rsp		#[Return] Move stack pointer back to where it was before the function
		popq	%rbp		#[Return] Retrieve base pointer
		ret		#[Return]
.globl main
.type main, @function
main:
		pushq	%rbp		#[Prologue] Save the old base pointer
		movq	%rsp, %rbp		#[Prologue] Set the base pointer to the current stack pointer
		subq	$16, %rsp		#[Prologue] Make room on the stack for local variables
		call	test_function_calls		#[Call]
		movl	%eax, -8(%rbp)		#[Call] Save Func Return value into [Temp_37]
		movl	-8(%rbp), %eax		#[Assign] move [Temp_37] in accumulator
		movl	%eax, -4(%rbp)		#[Assign] move accumulator in result
		movl	-4(%rbp), %eax		#[Return] save return value in the result adress
		movq	%rbp, %rsp		#[Return] Move stack pointer back to where it was before the function
		popq	%rbp		#[Return] Retrieve base pointer
		ret		#[Return]
