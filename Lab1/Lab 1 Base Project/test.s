	AREA text, CODE, READONLY
	EXPORT example1

example1
	MOV R1, #16
	MOV R2, #8
	ADD R1, R2, R3
	BX LR ;
	END