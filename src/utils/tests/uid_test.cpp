#include <gtest/gtest.h>
#include "../uid.hpp"

TEST(GenerateUIDTest, GenerateUID)
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

    for (int i = 0; i < uids.size(); ++i)
    {
        // Assert the size of uids is correct
        ASSERT_EQ(uids[i].size(), 10);
    }

    // Test if generated IDs are alphanumeric
    std::string uid = generate_uid(10); // Assuming fixed size of 10
    ASSERT_TRUE(std::all_of(uid.begin(), uid.end(), [](char c)
                            { return std::isalnum(c); }));
}

TEST(GenerateUIDTest, GenerateDateUID)
{
    int size = 14; // Size of the generated ID
    std::string uid = generate_date_uid();
    ASSERT_EQ(uid.size(), size); // Check if the size matches the expected size

    for (char c : uid)
    {
        ASSERT_TRUE(std::isdigit(c)) << "Generated ID contains non-digit characters";
    }
}
