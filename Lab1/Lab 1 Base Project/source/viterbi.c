#include "utils.h"
#include "viterbi.h"


// typedef struct{
//     int S; //number of states
//     int V; //number of observation types
//     float** transition; //S x S transition probability matrix
//     float** emission; //S x V emission probability matrixd
//     float* prior; //S x 1 prior probability matrix
// } hmm_desc;

int ViterbiUpdate_C(float* InputArray, float* OutputArray, hmm_desc* hmm, int Observation) {
    int number_of_states = hmm->S;
    u8 error = 0;
    float sum = 0;

    for (int k = 0; k < number_of_states; k++) {
        float p_observation_given_k = hmm->emission[k][Observation];
        float max = -1;

        for (int x = 0; x < number_of_states; x++) {
            float value = hmm->transition[x][k];
            if (value > max) {
                max = value;
            }
        }

        if (max < 0) {
            error = 1;
        }
        OutputArray[k] = max * p_observation_given_k;
        sum += OutputArray[k];
    }

    for (int k = 0; k < number_of_states; k++) {
        OutputArray[k] = OutputArray[k] / sum;
    }

    return error;
}

int Viterbi_C(int* Observations, int Nobs, int* EstimatedStates, hmm_desc* hmm) {
    // u8 error = 0;

    // for (int i = 0; i < Nobs; i++) {
    //     int return_value = ViterbiUpdate_C(current_state, EstimatedStates hmm, Observations[i]);
    // }
    return 0;
}
