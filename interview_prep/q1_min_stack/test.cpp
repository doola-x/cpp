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
    std::cout << "=== Q1: Min Stack ===\n";

    // Problem example
    {
        MinStack s;
        s.push(-2); s.push(0); s.push(-3);
        CHECK_EQ("example getMin", s.getMin(), -3);
        s.pop();
        CHECK_EQ("example top after pop", s.top(), 0);
        CHECK_EQ("example getMin after pop", s.getMin(), -2);
    }

    // Single element
    {
        MinStack s;
        s.push(42);
        CHECK_EQ("single top", s.top(), 42);
        CHECK_EQ("single getMin", s.getMin(), 42);
    }

    // Min updates correctly as stack grows
    {
        MinStack s;
        s.push(5); s.push(3); s.push(7); s.push(1);
        CHECK_EQ("growing min", s.getMin(), 1);
    }

    // Min restores correctly after popping the minimum element
    {
        MinStack s;
        s.push(5); s.push(3); s.push(1);
        CHECK_EQ("min before any pop", s.getMin(), 1);
        s.pop();
        CHECK_EQ("min after pop 1", s.getMin(), 3);
        s.pop();
        CHECK_EQ("min after pop 3", s.getMin(), 5);
    }

    // Duplicate values
    {
        MinStack s;
        s.push(2); s.push(2); s.push(2);
        CHECK_EQ("dup getMin", s.getMin(), 2);
        s.pop();
        CHECK_EQ("dup getMin after one pop", s.getMin(), 2);
    }

    // Duplicate minimums — min must persist until both copies are gone
    {
        MinStack s;
        s.push(3); s.push(1); s.push(1);
        CHECK_EQ("dup min getMin", s.getMin(), 1);
        s.pop();
        CHECK_EQ("dup min after pop first copy", s.getMin(), 1);
        s.pop();
        CHECK_EQ("dup min after pop second copy", s.getMin(), 3);
    }

    // Negative values
    {
        MinStack s;
        s.push(-1); s.push(-5); s.push(0);
        CHECK_EQ("neg getMin", s.getMin(), -5);
        s.pop(); // pop 0
        CHECK_EQ("neg getMin after pop 0", s.getMin(), -5);
        s.pop(); // pop -5
        CHECK_EQ("neg getMin after pop -5", s.getMin(), -1);
    }

    // top() is independent of getMin()
    {
        MinStack s;
        s.push(10); s.push(1); s.push(5);
        CHECK_EQ("top is most recent, not min", s.top(), 5);
        CHECK_EQ("min is not top", s.getMin(), 1);
    }

    std::cout << "\nResult: " << _passes << " passed, " << _failures << " failed\n";
    return _failures > 0 ? 1 : 0;
}
