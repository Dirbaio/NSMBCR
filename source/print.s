
@=================================
@ Function for printing stuff from C/C++

.global nocashPrint
.global nocashPrint1
.global nocashPrint2
.global nocashPrint3

nocashPrint:
nocashPrint1:
nocashPrint2:
nocashPrint3:
	push {r4-r9}
	ldr r4, =msgData
	mov r5, #0
	
  loop:
		ldrb r6, [r0]
		cmp r6, #0
		beq printMsg
		cmp r5, #120
		beq printMsg
		strb r6, [r4]
		
		add r0, r0, #0x1
		add r4, r4, #0x1
		add r5, r5, #0x1
		b loop
		
  printMsg:
	mov r0, r1
	mov r1, r2
	mov r2, r3
	
	mov  r12,r12
	b    continue83
	.word  0x6464
  msgData:
	.fill 120
	.byte  0                @ending zero (normally not required, see below)
	.align 4               @align following code (use align 2 in thumb mode)
  continue83:

	pop {r4-r9}
bx lr


@=================================
@ Macro for printing stuff from ASM code

.macro print txt
mov r12,r12
b 1f
.word 0x6464
.ascii "\txt"
.byte 0
.align 4
1:
.endm

@ You can print out stuff like this to output console.
@ For example the code below will log every time OpenFileFast is called.

@hook_0206A480:
@	print "OpenFileFast %r0% %r1% %r2%"
@	bx lr

hook_02006970:
	@Complicated printing from stack in here :(
	@I hope I got it right nao.
	print "LoadLevel %r0% %r1%"
	print "    World %r2%";
	print "    Level %r3%";
	add sp, sp, #14*4
	ldr r3, [sp, #0x20]
	print "    Area %r3%";
	ldr r3, [sp, #0x24]
	print "    PlayerNum %r3%";
	ldr r3, [sp, #0x28]
	print "    PlayerMask %r3%";
	ldr r3, [sp, #0x2C]
	print "    PlayerChar1 %r3%";
	ldr r3, [sp, #0x30]
	print "    PlayerChar2 %r3%";
	ldr r3, [sp, #0x34]
	print "    unk1 %r3%";
	ldr r3, [sp, #0x38]
	print "    unk2 %r3%";
	ldr r3, [sp, #0x3C]
	print "    unk3 %r3%";
	sub sp, sp, #14*4

	bx lr
	
afdansafdub_020D08DC_ov_08:
	bx lr

hook_020A2EF8:
	print "StageScene::onDelete %r0%"
	bx lr

hook_020A3318:
	print "StageScene::onCreate %r0%"
	bx lr

/*
hook_02044D78:
	print "free %r0% %r1%"
	bx lr
	
hook_02044FE8:
	print "malloc %r0% %r1% %r2%"
	bx lr
hook_0204500C:
	print "       %r0%"
	bx lr
	*/
	
hook_02009D34:
	print "Free File Cache %r0% %r1%"
	bx lr

rssepl_02006ADC:
	mov r7, #2
	bx lr
	
/*
repl_0215E3DC:
	bx lr
	*/

hook_020A18A4:
	print "ExitLevel %r0%"
	ldr r1, =exitType
	str r0, [r1]
	bx lr