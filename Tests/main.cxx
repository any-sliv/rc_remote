#include <gtest/gtest.h>

/*
    All tests are defined in {test_lib}/Modules - per module organization
    Also there you will find HAL mocks. Tests links only mocked,
    does not link any of real HAL headers.
 */

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}