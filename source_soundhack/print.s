
@=================================

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


.macro print txt
mov r12,r12
b 1f
.word 0x6464
.ascii "\txt"
.byte 0
.align 4
1:
.endm

hook_0206A480:
	print "OpenFileFast %r0% %r1% %r2%"
	bx lr
	
@patch NNS_SndArcPlayerStartSeq to nullsub :D
func_02060244:
func_020601BC:
func_0205D7C4:
func_0204E924:
	print "derp"
	bx lr
	
func_020E6524:
	bx lr
	
hook_0212B748:
	print "MarioActor_doPlayerNumberStuff %r0% %r1%"
