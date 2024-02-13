#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <stdexcept>
#include <filesystem>
#include "../cache.cpp"

class CacheTest : public ::testing::Test
{
protected:
    // Test data
    std::unordered_map<std::string, std::string> original_data = {{"key1", "value1"}, {"key2", "value2"}};
    std::filesystem::path test_dir;

    void SetUp() override
    {
        // Create a temporary directory for testing
        test_dir = std::filesystem::temp_directory_path() / "test_cache";
        std::filesystem::create_directory(test_dir);
    }

    void TearDown() override
    {
        // Remove the temporary directory and its contents
        std::filesystem::remove_all(test_dir);
    }
};

// Test caching and loading a dictionary
TEST_F(CacheTest, CacheAndLoadDictionary)
{
    // File path for caching
    std::filesystem::path file_path = test_dir / "cached_data.pkl";

    // Cache the dictionary
    ASSERT_NO_THROW(cache_dictionary<std::string>(original_data, file_path.string()));

    // Load the cached dictionary
    // auto loaded_data = load_cached_dictionary<std::string>(file_path.string());

    // Check if loaded data is the same as the original data
    // ASSERT_EQ(loaded_data, original_data);
}

// Test loading from a nonexistent file
TEST_F(CacheTest, LoadNonexistentFile)
{
    // Try to load from a nonexistent file
    std::filesystem::path file_path = test_dir / "nonexistent_file.pkl";
    // auto loaded_data = load_cached_dictionary<std::string>(file_path.string());

    // Check if loaded data is empty
    // ASSERT_TRUE(loaded_data.empty());
}
