repl_02005B58:
	@Force DBG screen to show
	mov r5, #5
	bx lr

	@Stop all DMAs
	@This avoids garbled graphics because of HDMA
	ldr r12, =0x040000B8
	str r5, [r12]
	str r5, [r12, #0xC]
	str r5, [r12, #0x18]
	str r5, [r12, #0x24]

	bx lr


@Kill BG HDMA
nsub_01FF8420:
nsub_01FFFC20:
	bx lr
	

@Spawn 2 players
repl_020069B4:
	mov r2, #3
	str r2, [r13, #0x28]
	bx lr
	
@ Force 2nd player to use player 1's view data
@ (Or something like that...?)
hook_02108E14:
	mov r3, #0
	strb r3, [r1, #0x1E]
	bx lr

