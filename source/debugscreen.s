repl_02005B58:
	@Force DBG screen to show (No key combination required)
	mov r5, #5

	@Stop all DMAs
	@This avoids garbled graphics on debugscreen because of HDMA
	ldr r12, =0x040000B8
	str r5, [r12]
	str r5, [r12, #0xC]
	str r5, [r12, #0x18]
	str r5, [r12, #0x24]

	bx lr

@ Remove intro scene before levels.
repl_02152868_ov_36:
	bx lr

