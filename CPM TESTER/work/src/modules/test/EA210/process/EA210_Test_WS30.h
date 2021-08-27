 #if !defined(__EA210_TEST_WS30_H__)
#define __EA210_TEST_WS30_H__

#include <test/test_panel.h>

#if defined(__cplusplus)
    extern "C" {
#endif
void string2hexString(char* input, char* output);
DEFINE_TESTSEQUENCE_FUNCTIONS(STATION_30);
DEFINE_TEST_FUNCTIONS(EA210, 30_0_0);
DEFINE_TEST_FUNCTIONS(EA210, 30_0_1);
DEFINE_TEST_FUNCTIONS(EA210, 30_0_2);
//DEFINE_TEST_FUNCTIONS(EA210, 30_0_3);

DEFINE_TEST_FUNCTIONS(EA210, 30_1_1);
DEFINE_TEST_FUNCTIONS(EA210, 30_1_2);

DEFINE_TEST_FUNCTIONS(EA210, 30_2_0);
DEFINE_TEST_FUNCTIONS(EA210, 30_2_1);
DEFINE_TEST_FUNCTIONS(EA210, 30_2_2);
DEFINE_TEST_FUNCTIONS(EA210, 30_2_3);
DEFINE_TEST_FUNCTIONS(EA210, 30_2_4);
DEFINE_TEST_FUNCTIONS(EA210, 30_2_5);

DEFINE_TEST_FUNCTIONS(EA210, 30_3_1);
DEFINE_TEST_FUNCTIONS(EA210, 30_3_2);
DEFINE_TEST_FUNCTIONS(EA210, 30_3_3);
DEFINE_TEST_FUNCTIONS(EA210, 30_3_4);

DEFINE_TEST_FUNCTIONS(EA210, 30_4_1);
DEFINE_TEST_FUNCTIONS(EA210, 30_4_2);
DEFINE_TEST_FUNCTIONS(EA210, 30_4_3);
DEFINE_TEST_FUNCTIONS(EA210, 30_4_4);
DEFINE_TEST_FUNCTIONS(EA210, 30_4_5);
DEFINE_TEST_FUNCTIONS(EA210, 30_4_6);
DEFINE_TEST_FUNCTIONS(EA210, 30_4_7);
DEFINE_TEST_FUNCTIONS(EA210, 30_4_8);
DEFINE_TEST_FUNCTIONS(EA210, 30_4_9);
DEFINE_TEST_FUNCTIONS(EA210, 30_4_10);
DEFINE_TEST_FUNCTIONS(EA210, 30_4_11);
DEFINE_TEST_FUNCTIONS(EA210, 30_4_12);
DEFINE_TEST_FUNCTIONS(EA210, 30_4_13);
DEFINE_TEST_FUNCTIONS(EA210, 30_4_14);


DEFINE_TEST_FUNCTIONS(EA210, 30_5_1);

DEFINE_TEST_FUNCTIONS(EA210, 30_6_1);
DEFINE_TEST_FUNCTIONS(EA210, 30_6_2);
DEFINE_TEST_FUNCTIONS(EA210, 30_6_3);
DEFINE_TEST_FUNCTIONS(EA210, 30_6_4);
DEFINE_TEST_FUNCTIONS(EA210, 30_6_5);
DEFINE_TEST_FUNCTIONS(EA210, 30_6_6);
DEFINE_TEST_FUNCTIONS(EA210, 30_6_7);
#if defined(__cplusplus)
    } /* extern "C" */
#endif

#endif /* __EA210_TEST_WS30_H__ */
