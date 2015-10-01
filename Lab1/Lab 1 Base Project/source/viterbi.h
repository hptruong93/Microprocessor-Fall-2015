#ifndef VITERBI_H
#define VITERBI_H

#define S_VALUE  2
#define V_VALUE  3


typedef struct{
    int S; //number of states
    int V; //number of observation types
    float transition[S_VALUE][S_VALUE]; //S x S transition probability matrix
    float emission[S_VALUE][V_VALUE]; //S x V emission probability matrixd
    float* prior; //S x 1 prior probability matrix
} hmm_desc;

extern int ViterbiUpdate_asm(float* InputArray, float* OutputArray, int Observation, hmm_desc* hmm);
int ViterbiUpdate_C(float* InputArray, float* OutputArray, hmm_desc* hmm, int Observation);
int Viterbi_C(int* Observations, int Nobs, int* EstimatedStates, hmm_desc* hmm);

#define VITERBI_ASM

#endif
