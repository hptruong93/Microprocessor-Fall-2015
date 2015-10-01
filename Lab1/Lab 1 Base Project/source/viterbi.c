#include "utils.h"
#include "viterbi.h"
#include "math.h"

float viterbi[NOBS][2 * S_DEF];

int ViterbiUpdate_C(float* InputArray, float* OutputArray, hmm_desc* hmm, int Observation) {
    int number_of_states = hmm->S;
    u8 error = 0;
    float sum = 0;

    printf("#############################################\n");
    for (int k = 0; k < number_of_states; k++) {
        float p_observation_given_k = get_index(&((hmm->emission)[0][0]), hmm->V, k, Observation);
        printf("Considering state %d with p_observation_given_k (%d, %d) is %f\n", k, k, Observation, p_observation_given_k);

                float temporary[number_of_states];
        for (int x = 0; x < number_of_states; x++) {
            float value = InputArray[x] * get_index(&((hmm->transition)[0][0]), number_of_states, x, k);
            printf("###### Evaluating InputArray[%d] * transition[%d][%d] = %f * %f = %f\n", x, x, k, InputArray[x], get_index(&((hmm->transition)[0][0]), number_of_states, x, k), value);
            temporary[x] = value;
        }

        float hard_max = 3;
        float* max = &hard_max;
        int max_index = argmax(temporary, hmm->S, max);
        OutputArray[number_of_states + k] = max_index;

        if (*max < 0) {
            error = 1;
        }
        
        if (fabs(*max) > ZERO_THRESHOLD) {
            OutputArray[k] = *max * p_observation_given_k;
            printf("Max value shall be %f multiply by %f gives %f\n", *max, p_observation_given_k, OutputArray[k]);
            sum += OutputArray[k];
        } else {
            printf("Max = 0 \n");
            OutputArray[k] = 0;
            printf("Max value shall be 0 multiply by %f gives 0\n", *max, p_observation_given_k, OutputArray[k]);
        }
    }

    printf("Sum would be %f\n", sum);
    if (fabs(sum) > ZERO_THRESHOLD) {
        for (int k = 0; k < number_of_states; k++) {
            OutputArray[k] = OutputArray[k] / sum;
        }
    } else {
        printf("Not gonna divide by 0... Skip this division\n");
    }
    printf("Conclude for this iteration\n");
    print_array(OutputArray, number_of_states);

    return error;
}

int Viterbi_C(int* Observations, int Nobs, int* EstimatedStates, hmm_desc* hmm) {

    for (int i = 1; i < Nobs; i++) {
        for (int j = 0; j < 2 * hmm->S; j++) {
            viterbi[i][j] = 0;
        }
    }
        
    float sum = 0;

    printf("Observation 0 is %d\n", Observations[0]);
    for (int j = 0; j < hmm->S; j++) {
        viterbi[0][j] = hmm->prior[j] * get_index(&((hmm->emission)[0][0]), hmm->V, j, Observations[0]);
        sum += viterbi[0][j];
    }

    if (fabs(sum) > ZERO_THRESHOLD) {
        for (int j = 0; j < hmm->S; j++) {
            viterbi[0][j] = viterbi[0][j] / sum;
        }
    }
        
    for (int i = 1; i < Nobs; i++) {
        printf("-----> At iteration number %d with Observation %d\n", i, Observations[i]);
        print_array(viterbi[i - 1], hmm->S);

        printf("Array output address before is %x\n", viterbi[i]);
#ifdef VITERBI_ASM
        int return_value = ViterbiUpdate_asm(viterbi[i - 1], viterbi[i], Observations[i], hmm);   
#else
        int return_value = ViterbiUpdate_C(viterbi[i - 1], viterbi[i], hmm, Observations[i]);
#endif
        printf("Array output address after is %x\n", viterbi[i]);
        // printf("vitpsi is --> ");
        print_array(viterbi[i], hmm->S);

        if (return_value != 0) {
            return return_value;
        }
    }

    float* max_value;
    EstimatedStates[Nobs - 1] = argmax(viterbi[Nobs - 1], hmm->S, max_value);
    for (int i = Nobs - 1; i > 0; i--) {
        EstimatedStates[i - 1] = viterbi[i][hmm->S + EstimatedStates[i]];
    }

    return 0;
}
