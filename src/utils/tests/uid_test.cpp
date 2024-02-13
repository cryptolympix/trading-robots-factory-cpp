#include <gtest/gtest.h>
#include "../uid.hpp"

TEST(GenerateUIDTest, GeneratesCorrectSize)
{
    // Test with different sizes
    for (int size = 1; size <= 10; ++size)
    {
        std::string uid = generate_uid(size);
        ASSERT_EQ(uid.size(), size);
    }
}

TEST(GenerateUIDTest, GeneratesUniqueIDs)
{
    // Generate multiple IDs and ensure they are unique
    std::vector<std::string> uids;
    const int num_ids = 1000;
    for (int i = 0; i < num_ids; ++i)
    {
        std::string uid = generate_uid(10); // Assuming fixed size of 10
        // Check if the generated UID is already present
        ASSERT_EQ(std::count(uids.begin(), uids.end(), uid), 0);
        uids.push_back(uid);
    }
}

TEST(GenerateUIDTest, GeneratesAlphanumericIDs)
{
    // Test if generated IDs are alphanumeric
    std::string uid = generate_uid(10); // Assuming fixed size of 10
    ASSERT_TRUE(std::all_of(uid.begin(), uid.end(), [](char c)
                            { return std::isalnum(c); }));
}
