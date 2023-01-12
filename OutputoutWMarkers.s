	movl	$111, %ebx		# Start marker bytes
	.byte	0x64, 0x67, 0x90		# Start marker bytes



	movl	$222, %ebx		# End marker bytes
	.byte	0x64, 0x67, 0x90		# End marker bytes

