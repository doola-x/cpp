# Q2: LRU Cache

Design a data structure that follows the **Least Recently Used (LRU)**
cache eviction policy.

## Class Interface

```cpp
class LRUCache {
public:
    LRUCache(int capacity);
    int get(int key);
    void put(int key, int value);
};
```

## Behavior

- `get(key)` — return the value if key exists, otherwise `-1`.
  Accessing a key marks it as **most recently used**.
- `put(key, value)` — insert or update the key-value pair.
  If inserting would exceed capacity, evict the **least recently used** key first.
  Updating an existing key also marks it as most recently used.

## Constraints

- `1 <= capacity <= 3000`
- `0 <= key, value <= 10^4`
- Both `get` and `put` must run in **O(1)** average time

## Example

```
LRUCache cache(2);
cache.put(1, 1);
cache.put(2, 2);
cache.get(1);    // returns 1     (marks 1 as MRU; order: 2→1)
cache.put(3, 3); // evicts 2      (order: 1→3)
cache.get(2);    // returns -1    (was evicted)
cache.put(4, 4); // evicts 1      (order: 3→4)
cache.get(1);    // returns -1
cache.get(3);    // returns 3
cache.get(4);    // returns 4
```

---
*Hint (only read if stuck): O(1) for both reads and writes means you need
O(1) removal from the middle of your ordering structure. Think about pairing
an `unordered_map` with a `std::list` and storing iterators in the map.*
