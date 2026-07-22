#include <iostream>
#include <string>
#include "solution.h"

static int _passes = 0, _failures = 0;

#define CHECK_EQ(label, a, b) do { \
    std::string _a = (a); std::string _b = (b); \
    if (_a == _b) { \
        std::cout << "  PASS [" << (label) << "]\n"; \
        _passes++; \
    } else { \
        std::cerr << "  FAIL [" << (label) << "] line " << __LINE__ \
                  << ": expected \"" << _b << "\", got \"" << _a << "\"\n"; \
        _failures++; \
    } \
} while(0)

int main() {
    std::cout << "=== Q5: Time-Based Key-Value Store ===\n";

    // Problem example
    {
        TimeMap tm;
        tm.set("foo", "bar", 1);
        CHECK_EQ("exact timestamp", tm.get("foo", 1), "bar");
        CHECK_EQ("future timestamp returns last known", tm.get("foo", 3), "bar");
        tm.set("foo", "baz", 4);
        CHECK_EQ("exact updated timestamp", tm.get("foo", 4), "baz");
        CHECK_EQ("future after update", tm.get("foo", 5), "baz");
        CHECK_EQ("before first ts returns empty", tm.get("foo", 0), "");
    }

    // Query before any stored timestamp returns ""
    {
        TimeMap tm;
        tm.set("x", "v", 5);
        CHECK_EQ("query at ts=4 before first set", tm.get("x", 4), "");
        CHECK_EQ("query at ts=1 before first set", tm.get("x", 1), "");
    }

    // Missing key returns ""
    {
        TimeMap tm;
        CHECK_EQ("missing key on empty map", tm.get("nope", 10), "");
        tm.set("other", "val", 1);
        CHECK_EQ("missing key on populated map", tm.get("nope", 10), "");
    }

    // Multiple timestamps for the same key: binary search picks the floor
    {
        TimeMap tm;
        tm.set("k", "a", 1);
        tm.set("k", "b", 3);
        tm.set("k", "c", 5);
        tm.set("k", "d", 7);
        CHECK_EQ("ts=0 before all", tm.get("k", 0), "");
        CHECK_EQ("ts=1 exact first", tm.get("k", 1), "a");
        CHECK_EQ("ts=2 between 1 and 3", tm.get("k", 2), "a");
        CHECK_EQ("ts=3 exact second", tm.get("k", 3), "b");
        CHECK_EQ("ts=4 between 3 and 5", tm.get("k", 4), "b");
        CHECK_EQ("ts=5 exact third", tm.get("k", 5), "c");
        CHECK_EQ("ts=6 between 5 and 7", tm.get("k", 6), "c");
        CHECK_EQ("ts=7 exact last", tm.get("k", 7), "d");
        CHECK_EQ("ts=100 after all", tm.get("k", 100), "d");
    }

    // Multiple keys are independent
    {
        TimeMap tm;
        tm.set("a", "va1", 1);
        tm.set("b", "vb1", 2);
        tm.set("a", "va2", 3);
        CHECK_EQ("key a at ts=2", tm.get("a", 2), "va1");
        CHECK_EQ("key b at ts=3", tm.get("b", 3), "vb1");
        CHECK_EQ("key a at ts=3 updated", tm.get("a", 3), "va2");
        CHECK_EQ("key b ts=1 before first set", tm.get("b", 1), "");
    }

    // Single entry edge cases
    {
        TimeMap tm;
        tm.set("z", "only", 50);
        CHECK_EQ("exactly at ts", tm.get("z", 50), "only");
        CHECK_EQ("ts before only entry", tm.get("z", 49), "");
        CHECK_EQ("ts well after only entry", tm.get("z", 9999), "only");
    }

    std::cout << "\nResult: " << _passes << " passed, " << _failures << " failed\n";
    return _failures > 0 ? 1 : 0;
}
