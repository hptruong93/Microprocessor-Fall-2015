#include <stdio.h>
#include "arm_math.h"
#include "viterbi.h"
#include "utils.h"

int main()
{
    //printf("The quick brown fox jumps over the lazy dog.\n");

#ifdef VITERBI_ASM
    printf("Using ViterbiUpdate_asm\n");
#else
    printf("Using ViterbiUpdate_C\n");
#endif

#ifdef TEST_1
    test1();
#endif

#ifdef TEST_2
    test2();
#endif

#ifdef TEST_3
    test3();
#endif

#ifdef TEST_4
    test4();
#endif

#ifdef TEST_5
    test5();
#endif
        
    return 0;
}
