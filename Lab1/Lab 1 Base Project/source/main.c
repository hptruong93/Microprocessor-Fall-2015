#include <stdio.h>
#include "arm_math.h"
#include "viterbi.h"
#include "utils.h"

#define TEST_1

int main()
{
    printf("The quick brown fox jumps over the lazy dog.\n");

#ifdef TEST_1
     hmm_desc hmm1;
     hmm1.S = S_VALUE;
     hmm1.V = V_VALUE;

    float prior[2] = {0.6, 0.4};
    hmm1.prior = prior;

    int observation[3] = {0, 1, 2};
    //float transition[2][2] = {{0.7, 0.4}, {0.3, 0.6}};
    //float emission[2][3] = {{0.5, 0.4, 0.1}, {0.1, 0.3, 0.6}};

    hmm1.transition[0][0] = 0.7;
    hmm1.transition[0][1] = 0.4;
    hmm1.transition[1][0] = 0.3;
    hmm1.transition[1][1] = 0.6;
    
    hmm1.emission[0][0] = 0.5;
    hmm1.emission[0][1] = 0.4;
    hmm1.emission[0][2] = 0.1;
    hmm1.emission[1][0] = 0.1;
    hmm1.emission[1][1] = 0.3;
    hmm1.emission[1][2] = 0.6;
    
    int EstimatedStates[3] = {2, 2, 2};
    Viterbi_C(observation, 3, EstimatedStates, &hmm1);

    for (int i = 0; i < 3; i++) {
        printf(" --> %d", EstimatedStates[i]);
    } printf("\n");
#endif

#ifdef TEST_2
    hmm_desc hmm;
    hmm.S = S_VALUE; // 3
    hmm.V = V_VALUE; // 4
    hmm.transition[0][0] = 1;
    hmm.transition[0][1] = 1;
    hmm.transition[0][2] = 2;
    hmm.transition[1][0] = 5;
    hmm.transition[1][1] = 1;
    hmm.transition[1][2] = 2;
    hmm.transition[2][0] = 4;
    hmm.transition[2][1] = 3;
    hmm.transition[2][2] = 1;
    hmm.emission[0][0] = 2;
    hmm.emission[0][1] = 2;
    hmm.emission[0][2] = 3;
    hmm.emission[0][3] = 1;
    hmm.emission[1][0] = 1;
    hmm.emission[1][1] = 1;
    hmm.emission[1][2] = 2;
    hmm.emission[1][3] = 2;
    hmm.emission[2][0] = 2;
    hmm.emission[2][1] = 1;
    hmm.emission[2][2] = 2;
    hmm.emission[2][3] = 1;
    hmm.prior[0] = 0.6;
    hmm.prior[1] = 0.3;
    hmm.prior[2] = 0.1;
    
    float inputArray[] = {1, 2, 2, 2, 0, 1};
    float outputArray[2 * hmm.S];
    int observation = 2;
    
		printf("Before inputAray = %x\n", inputArray);
		printf("Before hmm = %x\n", &hmm);
    int viterbiUpdate = ViterbiUpdate_asm(inputArray, outputArray, observation, &hmm);
    
    //0.3529| 0.2941| 0.3529| 2.0000| 0.0000| 1.0000|
		printf("Return code = %d\n", viterbiUpdate);
		printf("Return inputAray = %x\n", inputArray);
		printf("Return hmm = %x\n", &hmm);
		
    for (int i = 0; i < 2* hmm.S; i++) {
        printf(" --> %f", outputArray[i]);
    }
    printf("\n");
#endif

    return 0;
}
