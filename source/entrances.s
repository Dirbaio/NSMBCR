
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

@ Replace entrance func.
nsub_0201E744:
	PUSH	{LR}
	SUB		SP, SP, #0xC
	
	LDR		R0, =entranceDestArea
	LDR		R0, [R0]
	CMP		R0, #0xFE
	
	LDRNE 	R12, =0x0201E74C
	BXNE	R12
	
	LDR		R1, =entranceDestEntrance
	LDR		R1, [R1]
	MOV		R0, R1, LSR#4
	AND		R1, R1, #0xF
	print 	"Load %r0% %r1%"
	BL		_Z11myLoadLevelii
	
	ADD		SP, SP, #0xC
	POP 	{PC}


