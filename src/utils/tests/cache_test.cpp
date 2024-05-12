#include <gtest/gtest.h>
#include "../cache.hpp"

// Fixture class for Cache tests
class CacheTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        cache = new Cache("cache/test_cache.json");
    }

    void TearDown() override
    {
        delete cache;
        // Remove the file
        std::filesystem::remove("cache/test_cache.json");
    }

    // Pointer to the cache object
    Cache *cache;
};

// Test case to check if cache file exists
TEST_F(CacheTest, FileExist)
{
    EXPECT_FALSE(cache->exist()); // Initially, the file should not exist
    cache->add("key1", CachedData{.base_currency_conversion_rate = 1.0});
    cache->create();
    EXPECT_TRUE(cache->exist()); // After creating the cache, the file should exist
}

// Test case to add and retrieve data from cache
TEST_F(CacheTest, AddAndGet)
{
    cache->add("key1", CachedData{.base_currency_conversion_rate = 1.0});
    cache->create();
    Cache *loadedCache = Cache::load("cache/test_cache.json");
    EXPECT_EQ(1.0, loadedCache->get("key1").base_currency_conversion_rate); // Check if the value retrieved is correct
    delete loadedCache;
}

// Test case to check if exception is thrown for non-existing key
TEST_F(CacheTest, KeyNotFound)
{
    cache->add("key1", CachedData{.base_currency_conversion_rate = 1.0});
    cache->create();
    Cache *loadedCache = Cache::load("cache/test_cache.json");
    EXPECT_THROW(loadedCache->get("non_existing_key"), std::runtime_error);
    delete loadedCache;
}

// Test case to check if cache file is loaded correctly
TEST_F(CacheTest, LoadFile)
{
    cache->add("key1", CachedData{.base_currency_conversion_rate = 1.0});
    cache->create();
    Cache *loadedCache = Cache::load("cache/test_cache.json");
    EXPECT_TRUE(loadedCache->exist());                                      // Check if the loaded cache file exists
    EXPECT_EQ(1.0, loadedCache->get("key1").base_currency_conversion_rate); // Check if the value retrieved is correct
    delete loadedCache;
}
