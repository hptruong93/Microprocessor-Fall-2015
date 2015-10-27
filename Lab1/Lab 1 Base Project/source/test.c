#include "viterbi.h"
#include "test.h"

/** test4 is for AccObs
*   test5 is for 
*/


void test1(void) {
#ifdef TEST_1
    hmm_desc hmm1;
    hmm1.S = S_DEF;
    hmm1.V = V_DEF;

    hmm1.prior[0] = 0.6;
        hmm1.prior[1] = 0.4;

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
}


void test2(void) {
#ifdef TEST_2
    hmm_desc hmm;
    hmm.S = S_DEF; // 3
    hmm.V = V_DEF; // 4
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
}

void test3(void) {
#ifdef TEST_3
    // #define S_DEF 4
    // #define V_DEF 3
    //This is one of the three hmm structs that you will be provided
    hmm_desc hmm1 = {S_DEF, V_DEF,
        {
            {0.09,0.9,0.01,0},
            {0.09,0.01,0.9,0},
            {0.09,0.45,0.01,0.45},
            {0.5,0.5,0,0}
        },

        {
            {1,0,0},
            {0,1,0},
            {0,0,1},
            {0,1,0}
        },

        {0.25,0.25,0.25,0.25}
    };
        
    //This is the array of observations (same for all three hmm structs)
    int vitTestArray[20] = {2,1,1,2,1,2,1,0,1,2,1,1,2,1,2,1,1,2,1,0};

    int output[NOBS];
        

    int error = Viterbi_C(vitTestArray, NOBS, output, &hmm1);
    printf("Error is %d\n", error);
    for (int i = 0; i < NOBS; i++) {
        printf(" --> %d", output[i]);
    } printf("\n");

    //This is the array with which you should compare the EstimatedStates array generated by your method (different for each hmm struct)
    int vit_path[20] = {2,3,1,2,1,2,3,0,1,2,3,1,2,1,2,3,1,2,3,0};
    printf("Expected values\n");
    for (int i = 0; i < NOBS; i++) {
        printf(" --> %d", vit_path[i]);
    } printf("\n");
#endif
}

void test4(void) {
#ifdef TEST_4
    int accT = 45;
    int output[45];
    float accObs[45] =
    {-0.47, 0.51, 0.69, 0.02, 0.54, 0.12, -0.48, 0.42, 3.73,
    6.19, 6.74, 6.49, -5.71, -4.15, -7.35, -7.20, -7.02, -3.11,
    4.37, 2.17, 4.36, 1.86, -0.62, 0.49, 0.08, 0.60, 0.27,
    0.12, 6.67, 7.13, 7.92, 0.80, -1.77, -3.59, -0.87, 4.60,
    4.25, 6.57, 2.44, 0.03, 0.56, 0.10, 0.48, 0.33, 0.12};
    
    //And the threshold values
    float alpha1 = 0.7;
    float alpha2 = 0.8;
    int N1 = 3;
    int N2 = 3;
    int N3 = 3;

    //int accelometer(int accT, float* accObs, float* output, float alpha1, float alpha2, int N1, int N2, int N3)
    int len = accelometer(45, accObs, output, alpha1, alpha2, N1, N2, N3);
    printf("Found length = %d\n", len);
    for (int i = 0; i < len; i++) {
        printf("--> %d ", output[i]);
    }
    printf("\n");
#endif
}

void test5(void) {
#ifdef TEST_5
    hmm_desc hmm1 = {S_DEF, V_DEF,
        {
            {0.09,0.9,0.01,0},
            {0.09,0.01,0.9,0},
            {0.09,0.45,0.01,0.45},
            {0.5,0.5,0,0}
        },

        {
            {1,0,0},
            {0,1,0},
            {0,0,1},
            {0,1,0}
        },

        {0.25,0.25,0.25,0.25}
    };

    int Y[9] = {0,1,2,1,0,1,2,1,0};
    int nobs = 9;

    int output[NOBS];

    int error = Viterbi_C(Y, NOBS, output, &hmm1);

    printf("Error is %d\n", error);
    for (int i = 0; i < NOBS; i++) {
        printf(" --> %d", output[i]);
    } printf("\n");

    //Finally, after verifying that the generated observation sequence is correct
    //it should be used in your Viterbi code along with a previously provided hmm struct
    //to generate an EstimatedStates array that matches the given expected output:
    int X[9] = {0,1,2,3,0,1,2,3,0};
    printf("Expected values\n");
    for (int i = 0; i < NOBS; i++) {
        printf(" --> %d", X[i]);
    } printf("\n");
#endif
}

void test6(void) {
#ifdef TEST_6

#endif
}
