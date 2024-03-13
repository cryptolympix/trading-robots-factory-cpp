#include <gtest/gtest.h>

int neat_main(int argc, char **argv)
{
    // Run all the tests
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}