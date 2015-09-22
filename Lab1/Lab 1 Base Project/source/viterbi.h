#ifndef VITERBI_H
#define VITERBI_H

typedef struct{
    int S; //number of states
    int V; //number of observation types
    float* transition; //S x S transition probability matrix
    float* emission; //S x V emission probability matrixd
    float* prior; //S x 1 prior probability matrix
} hmm_desc;

int ViterbiUpdate_C(float* InputArray, float* OutputArray, hmm_desc* hmm, int Observation);
int Viterbi_C(int* Observations, int Nobs, int* EstimatedStates, hmm_desc* hmm);

#endif
