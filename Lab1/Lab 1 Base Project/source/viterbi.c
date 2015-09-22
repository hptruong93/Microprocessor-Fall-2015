#include "utils.h"
#include "viterbi.h"


// typedef struct{
//     int S; //number of states
//     int V; //number of observation types
//     float** transition; //S x S transition probability matrix
//     float** emission; //S x V emission probability matrixd
//     float* prior; //S x 1 prior probability matrix
// } hmm_desc;

void initialize(float** viterbi, hmm_desc* hmm, int Nobs, int initial_observation) {
    for (int i = 1; i < Nobs; i++) {
        for (int j = 0; j < 2 * hmm->S; j++) {
            viterbi[i][j] = 0;
        }
    }

    /* Use float multiplication lib to optimize */
    float* initial_state = viterbi[0];
    float sum = 0;

    /*void  arm_mult_f32 (float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst, uint32_t blockSize)*/
    for (int j = 0; j < hmm->S; j++) {
        initial_state[j] = hmm->prior[j] * hmm->emission[j][initial_observation];
        sum += initial_state[j];
    }


    /*void  arm_scale_f32 (float32_t *pSrc, float32_t scale, float32_t *pDst, uint32_t blockSize)*/
    for (int j = 0; j < hmm->S; j++) {
        initial_state[j] = initial_state[j] / sum;
    }
}

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
                OutputArray[number_of_states + k] = x;
            }
        }

        if (max < 0) {
            error = 1;
        }
        OutputArray[k] = max * p_observation_given_k;
        sum += OutputArray[k];
    }

    /*void  arm_scale_f32 (float32_t *pSrc, float32_t scale, float32_t *pDst, uint32_t blockSize)*/
    for (int k = 0; k < number_of_states; k++) {
        OutputArray[k] = OutputArray[k] / sum;
    }

    return error;
}

int Viterbi_C(int* Observations, int Nobs, int* EstimatedStates, hmm_desc* hmm) {
    u8 error = 0;

    const float PSI_SHIFT = sizeof(float) * hmm->S;

    float viterbi[Nobs][2 * hmm->S];
    initialize(viterbi, hmm, Nobs, Observations[0]);


    //Forward propagation
    for (int i = 1; i < Nobs; i++) {
        int return_value = ViterbiUpdate_C(viterbi[i - 1], viterbi[i], hmm, Observations[i]);

        if (return_value != 0) {
            return return_value;
        }
    }

    //Backtrack
    /*void  arm_max_f32 (float32_t *pSrc, uint32_t blockSize, float32_t *pResult, uint32_t *pIndex)*/
    EstimatedStates[Nobs - 1] = argmax(viterbi[Nobs - 1] + PSI_SHIFT, hmm->S);
    for (int i = Nobs - 1; i > 0; i--) {
        EstimatedStates[i - 1] = ((float*) (viterbi[i] + PSI_SHIFT))[EstimatedStates[i]];
    }

    return 0;
}
