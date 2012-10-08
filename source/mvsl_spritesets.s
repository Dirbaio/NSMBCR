@Replace CalcFileID
nsub_02009394:
	mov r0, r0, LSL#16
	mov r0, r0, LSR#16
	
 	cmp r0, #0x8000
 	bxcs lr
 	
 	ldr r1, =0x02085D04
 	ldrsh r1, [r1]
 	add r0, r0, r1
	mov r0, r0, LSL#16
	mov r0, r0, LSR#16
 	bx lr


nsub_0215CC9C_ov_35:
	ldr r1, =0x0208B720
	ldr r1, [r1]
	ldr r0, =0x0215CCE0
	str r1, [r0]
	mov r0, r1
	bx lr

nsub_0215CC70_ov_35:
	bx lr

/*
repl_02009694:
	bx lr
	
nsub_0200B7D0:
	bx lr

hook_02009568:
	cmp r0, #53
	bxne lr
	ldr r1, =0xDEADBEEF
	bl OS_Panic
	bx lr
*/

