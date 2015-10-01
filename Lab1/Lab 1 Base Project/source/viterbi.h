#ifndef VITERBI_H
#define VITERBI_H

#define S_DEF  4
#define V_DEF  3
#define NOBS   20

#define ZERO_THRESHOLD 0.0000000001



typedef struct{
    int S; //number of states
    int V; //number of observation types
    float transition[S_DEF][S_DEF]; //S x S transition probability matrix
    float emission[S_DEF][V_DEF]; //S x V emission probability matrixd
    float prior[S_DEF]; //S x 1 prior probability matrix
} hmm_desc;

extern int ViterbiUpdate_asm(float* InputArray, float* OutputArray, int Observation, hmm_desc* hmm);
int ViterbiUpdate_C(float* InputArray, float* OutputArray, hmm_desc* hmm, int Observation);
int Viterbi_C(int* Observations, int Nobs, int* EstimatedStates, hmm_desc* hmm);

#define VITERBI_C

#endif
