	AREA text, CODE, READONLY
		EXPORT ViterbiUpdate_asm

ViterbiUpdate_asm FUNCTION
	;We have 4, 5, 6, 7, 8, 10 and 11
	;Always use 10 & 11 as temp/ calculation var
	;R5 = error and R7 = sum and R10 = k and R11 = number_of_states
	
	;R10 = k and R11 = number_of_states
	MOV R10, #0
	B LOOP1
BACK1

	;S7 = sum
	;R10 = k and R11 = number_of_states
	MOV R10, #0
	B LOOP3
BACK3
	;

	BX LR;


LOOP1 ;R5 = error and R10 = k and R11 = number_of_states
	CMP R10, R11
	BEQ BACK1
	;S12 = emission[k][Observation]
	ADD R4, R3, #8 ;get emission matrix address
	ADD R5, R3, #4 ; R5 = V
	MUL R5, R10, R5 ; R5 = k * V
	ADD R5, R5, R3 ; R5 = k * v + Observation
	VLDR.F32 S12, [R5]; p_observation_given_k = get_index(hmm->emission, hmm->V, k, Observation);

	;#########################Matrix term by term multiplication####################
	B LOOP2
BACK2
	; Now the whole temporary array is in stack
	; Must use it immediately
	;#########################Find max index########################################
	MOV R9, #0; i = 0
	VSUB.F32 S11, S11 ; *max = 0
	B LOOP2_5
BACK2_5
	; Now everything should be removed from stack
	; S6 = max_index; S11 = *max
    
	ADD R4, R10, R11
	PUSH {R11}
	ADD R4, R4, R1
	VSTR.F32 S6, [R4] ; OutputArray[number_of_states + k] = max_index
	POP {R11}


    ;#########################Check for error#######################################
	;S11 = *max and S12 = p_observation_given_k
	VCMP.F32 S11, #0.0 ;if *max < 0
	IT LE ; then
	ADDLE R5, #1 ; error = 1

	;#########################Assign value and accumulate sum ######################
	VMUL.F32 S10, S11, S12 ; temp = max * p_observation_given_k
	PUSH {R11}
	MOV R11, R1
	ADD R11, R10
	VSTR.F32 S10, [R11] ; OutputArray[k] = temp
	POP {R11}
	VADD.F32 S7, S10 ; sum = sum + temp

	ADD R10, #1 ; k++
	B LOOP1

	;#########################Matrix term by term multiplication####################
LOOP2 ; R9 = k and R10 = x and R11 = number_of_states and S5 = InputArray[x] and R4 = hmm->transition
	CMP R10, R11
	BEQ BACK2

	;S6 = value
	MUL R5, R11, R10; R5 = x * number_of_states
	ADD R5, R5, R9 ; R5 = x * number_of_states + k
	ADD R4, R4, R5 ; R4 = address of get_index(hmm->emission, hmm->V, k, Observation)
	VLDR.F32 S6, [R4] ; S6 = get_index(hmm->emission, hmm->V, k, Observation);
	VMUL.F32 S6, S5, S6 ; S6 = InputArray[x] * get_index(hmm->emission, hmm->V, k, Observation);

	VPUSH {S6}
	ADD R10, #1
	B LOOP2


;#########################Find max index########################################
LOOP2_5; R9 = i and R11 = number_of_states
	CMP R9, R11
	BEQ BACK2_5

	;S6 = max_index; S11 = *max
	VPOP {S10} ; S10 = next element in temp array
	VCMP.F32 S11, S10
	ITT LE ; if max < current
	VMOVLE.F32 S11, S10 ; max = current
	VMOVLE.F32 S6, R9; max_index = i

	ADD R9, #1
	B LOOP2_5

	;#########################Divide by sum ########################################
LOOP3 ;R10 = k and R11 = number_of_states and S7 = sum
	CMP R10, R11
	BEQ BACK3
	
	PUSH {R11}
	MOV R11, R1
	ADD R11, R10
	VLDR.F32 S10, [R11] ; temp =  OutputArray[k]
	VDIV.F32 S10, S10, S7 ; temp = temp / sum
	VSTR.F32 S10, [R11] ; OutputArray[k] = temp
	POP {R11}
	ADD R10, #1

	B LOOP3

	ENDFUNC
	END