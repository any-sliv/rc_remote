#include <gtest/gtest.h>

/*
    All tests are defined in {test_lib}/Modules - per module organization
 */

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}