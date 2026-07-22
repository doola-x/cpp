# Q1: Min Stack

Design a stack that supports push, pop, top, and retrieving the minimum
element — all in **O(1) time**.

## Class Interface

```cpp
class MinStack {
public:
    MinStack();
    void push(int val);
    void pop();
    int top();
    int getMin();
};
```

## Constraints

- `pop()`, `top()`, and `getMin()` will only be called on a non-empty stack
- All four operations must run in O(1) time
- `-2^31 <= val <= 2^31 - 1`

## Example

```
MinStack s;
s.push(-2);
s.push(0);
s.push(-3);
s.getMin(); // returns -3
s.pop();
s.top();    // returns 0
s.getMin(); // returns -2
```

---
*Hint (only read if stuck): Think about what extra information you need to
track alongside each element so getMin() never has to scan. What happens
to the minimum when you pop an element that was the minimum?*
