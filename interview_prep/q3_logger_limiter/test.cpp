#include <iostream>
#include "solution.h"

static int _passes = 0, _failures = 0;

#define CHECK_EQ(label, a, b) do { \
    bool _a = (a); bool _b = (b); \
    if (_a == _b) { \
        std::cout << "  PASS [" << (label) << "]\n"; \
        _passes++; \
    } else { \
        std::cerr << "  FAIL [" << (label) << "] line " << __LINE__ \
                  << ": expected " << (_b ? "true" : "false") \
                  << ", got " << (_a ? "true" : "false") << "\n"; \
        _failures++; \
    } \
} while(0)

int main() {
    std::cout << "=== Q3: Logger Rate Limiter ===\n";

    // Problem example
    {
        Logger logger;
        CHECK_EQ("t=1 foo first time", logger.shouldPrintMessage(1, "foo"), true);
        CHECK_EQ("t=2 bar first time", logger.shouldPrintMessage(2, "bar"), true);
        CHECK_EQ("t=3 foo only 2s elapsed", logger.shouldPrintMessage(3, "foo"), false);
        CHECK_EQ("t=8 bar only 6s elapsed", logger.shouldPrintMessage(8, "bar"), false);
        CHECK_EQ("t=10 foo only 9s elapsed", logger.shouldPrintMessage(10, "foo"), false);
        CHECK_EQ("t=11 foo 10s elapsed ok", logger.shouldPrintMessage(11, "foo"), true);
    }

    // Boundary: exactly 10 seconds later is allowed
    {
        Logger logger;
        logger.shouldPrintMessage(0, "hello");
        CHECK_EQ("exactly 10s allowed", logger.shouldPrintMessage(10, "hello"), true);
    }

    // Boundary: 9 seconds is not allowed
    {
        Logger logger;
        logger.shouldPrintMessage(0, "hello");
        CHECK_EQ("9s not allowed", logger.shouldPrintMessage(9, "hello"), false);
    }

    // Different messages are completely independent
    {
        Logger logger;
        logger.shouldPrintMessage(0, "a");
        logger.shouldPrintMessage(0, "b");
        CHECK_EQ("a still blocked at t=5", logger.shouldPrintMessage(5, "a"), false);
        CHECK_EQ("b still blocked at t=5", logger.shouldPrintMessage(5, "b"), false);
        CHECK_EQ("a allowed at t=10", logger.shouldPrintMessage(10, "a"), true);
        CHECK_EQ("b allowed at t=10", logger.shouldPrintMessage(10, "b"), true);
    }

    // First occurrence always prints regardless of timestamp value
    {
        Logger logger;
        CHECK_EQ("first occurrence at t=100", logger.shouldPrintMessage(100, "newmsg"), true);
        CHECK_EQ("first occurrence at t=0", logger.shouldPrintMessage(0, "anothernew"), true);
    }

    // Cooldown resets from the last PRINTED time, not first
    {
        Logger logger;
        CHECK_EQ("spam t=0 prints", logger.shouldPrintMessage(0, "spam"), true);
        CHECK_EQ("spam t=1 blocked", logger.shouldPrintMessage(1, "spam"), false);
        CHECK_EQ("spam t=9 still blocked", logger.shouldPrintMessage(9, "spam"), false);
        CHECK_EQ("spam t=10 prints again", logger.shouldPrintMessage(10, "spam"), true);
        CHECK_EQ("spam t=19 blocked", logger.shouldPrintMessage(19, "spam"), false);
        CHECK_EQ("spam t=20 prints again", logger.shouldPrintMessage(20, "spam"), true);
    }

    std::cout << "\nResult: " << _passes << " passed, " << _failures << " failed\n";
    return _failures > 0 ? 1 : 0;
}
