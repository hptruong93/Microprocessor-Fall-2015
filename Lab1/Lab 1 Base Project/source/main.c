#include <stdio.h>
#include "arm_math.h"
#include "viterbi.h"
#include "utils.h"

extern int ViterbiUpdate_asm(float* InputArray, float* OutputArray, hmm_desc* hmm, int Observation);

int main()
{
    printf("AAA\n");
    hmm_desc hmm;
    hmm.S = 2;
    hmm.V = 3;

    float prior[2] = {0.6, 0.4};
    hmm.prior = prior;

    int observation[3] = {0, 1, 2};
    float transition[2][2] = {{0.7, 0.3}, {0.4, 0.6}};
    float emission[2][3] = {{0.5, 0.4, 0.1}, {0.1, 0.3, 0.6}};

    hmm.transition = (float*) transition;
    hmm.emission = (float*) emission;
    
    int Viterbi_C(int* Observations, int Nobs, int* EstimatedStates, hmm_desc* hmm);
    int EstimatedStates[3] = {2, 2, 2};
    Viterbi_C(observation, 3, EstimatedStates, &hmm);

    for (int i = 0; i < 3; i++) {
        printf(" --> %d", EstimatedStates[i]);
    }
    printf("\n");

    return 0;
}
