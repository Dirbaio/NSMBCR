
@ Replace entrance func.
nsub_0201E744:
	PUSH	{LR}
	SUB		SP, SP, #0xC
	
	LDR		R0, =entranceDestArea
	LDR		R0, [R0]
	CMP		R0, #0xFE
	
	LDRNE 	R12, =0x0201E74C
	BXNE	R12
	
	MOV		R0, #0
	LDR		R1, =entranceDestEntrance
	LDR		R1, [R1]
	BL		_Z11myLoadLevelii
	
	ADD		SP, SP, #0xC
	POP 	{PC}


