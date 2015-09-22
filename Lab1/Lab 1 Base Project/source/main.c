#include <stdio.h>
#include "arm_math.h"
#include "viterbi.h"
#include "utils.h"


int main()
{
    printf("AAA\n");

    // S = 2;
    // V = 3;
    // transition = [[ 0.7, 0.3 ],
    //            [ 0.4, 0.6 ]];

    // emission = [ [0.5, 0.4, 0.1 ],
    //           [0.1, 0.3, 0.6] ];

    // prior = [0.6, 0.4];
    // obs = [0, 1, 2];

    // result = [0,0,1];

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
    // for (int i = 0; i < 2; i++) {
    //     for (int j = 0; j < 3; ++j) {
    //         printf("%f\n", get_index(hmm.emission, 3, i, j));
    //     }
    // }

    return 0;
}
