#include <iostream>
#include "solution.h"

static int _passes = 0, _failures = 0;

#define CHECK_EQ(label, a, b) do { \
    auto _a = (a); auto _b = (b); \
    if (_a == _b) { \
        std::cout << "  PASS [" << (label) << "]\n"; \
        _passes++; \
    } else { \
        std::cerr << "  FAIL [" << (label) << "] line " << __LINE__ \
                  << ": expected " << _b << ", got " << _a << "\n"; \
        _failures++; \
    } \
} while(0)

int main() {
    std::cout << "=== Q2: LRU Cache ===\n";

    // Problem example
    {
        LRUCache cache(2);
        cache.put(1, 1);
        cache.put(2, 2);
        CHECK_EQ("get existing key 1", cache.get(1), 1);   // promotes 1 to MRU
        cache.put(3, 3);                                    // evicts 2 (LRU)
        CHECK_EQ("evicted key returns -1", cache.get(2), -1);
        cache.put(4, 4);                                    // evicts 1 (LRU)
        CHECK_EQ("evicted key 1 returns -1", cache.get(1), -1);
        CHECK_EQ("get 3", cache.get(3), 3);
        CHECK_EQ("get 4", cache.get(4), 4);
    }

    // Capacity of 1
    {
        LRUCache cache(1);
        cache.put(1, 10);
        CHECK_EQ("cap1 get 1", cache.get(1), 10);
        cache.put(2, 20);                       // evicts 1
        CHECK_EQ("cap1 evicted 1", cache.get(1), -1);
        CHECK_EQ("cap1 get 2", cache.get(2), 20);
    }

    // Updating an existing key does not count as a new insertion
    {
        LRUCache cache(2);
        cache.put(1, 1);
        cache.put(2, 2);
        cache.put(1, 100); // update — 1 becomes MRU, LRU is now 2
        cache.put(3, 3);   // should evict 2, not 1
        CHECK_EQ("updated key not evicted", cache.get(1), 100);
        CHECK_EQ("non-updated key evicted", cache.get(2), -1);
        CHECK_EQ("new key present", cache.get(3), 3);
    }

    // get() promotes a key so it is not the next eviction target
    {
        LRUCache cache(2);
        cache.put(1, 1);
        cache.put(2, 2);
        cache.get(1);       // promote 1 — LRU is now 2
        cache.put(3, 3);    // evicts 2
        CHECK_EQ("promoted key survives eviction", cache.get(1), 1);
        CHECK_EQ("demoted key was evicted", cache.get(2), -1);
        CHECK_EQ("new key present after eviction", cache.get(3), 3);
    }

    // Miss on empty cache
    {
        LRUCache cache(3);
        CHECK_EQ("miss on empty", cache.get(99), -1);
    }

    // No false evictions while under capacity
    {
        LRUCache cache(3);
        cache.put(1, 1); cache.put(2, 2); cache.put(3, 3);
        CHECK_EQ("under cap get 1", cache.get(1), 1);
        CHECK_EQ("under cap get 2", cache.get(2), 2);
        CHECK_EQ("under cap get 3", cache.get(3), 3);
    }

    // Value is updated correctly (not just recency)
    {
        LRUCache cache(2);
        cache.put(1, 1);
        cache.put(1, 99);
        CHECK_EQ("value updated on put", cache.get(1), 99);
    }

    std::cout << "\nResult: " << _passes << " passed, " << _failures << " failed\n";
    return _failures > 0 ? 1 : 0;
}
