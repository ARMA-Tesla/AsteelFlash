 #if !defined(__EA210_TEST_WS20_H__)
#define __EA210_TEST_WS20_H__

#include <test/test_panel.h>

#if defined(__cplusplus)
    extern "C" {
#endif

DEFINE_TESTSEQUENCE_FUNCTIONS(STATION_20);

DEFINE_TEST_FUNCTIONS(EA210, 20_0_0);
DEFINE_TEST_FUNCTIONS(EA210, 20_0_1);
DEFINE_TEST_FUNCTIONS(EA210, 20_1_1);
DEFINE_TEST_FUNCTIONS(EA210, 20_1_2);
DEFINE_TEST_FUNCTIONS(EA210, 20_1_3);

#if defined(__cplusplus)
    } /* extern "C" */
#endif

#endif /* __EA210_TEST_WS20_H__ */
