# Q5: Time-Based Key-Value Store

Design a key-value store that stores values **at specific timestamps** and
can retrieve the value that was current at any given point in time.

## Class Interface

```cpp
class TimeMap {
public:
    TimeMap();
    void set(const std::string& key, const std::string& value, int timestamp);
    std::string get(const std::string& key, int timestamp);
};
```

## Behavior

- `set(key, value, timestamp)` — store `key → value` recorded at `timestamp`.
  For any given key, timestamps passed to `set` are strictly increasing.
- `get(key, timestamp)` — return the value associated with key at the **largest
  stored timestamp ≤ the given timestamp**. Return `""` if no such value exists
  (key doesn't exist, or all stored timestamps for this key are greater).

## Constraints

- `1 <= key.length, value.length <= 100`
- `1 <= timestamp <= 10^7`
- Timestamps passed to `set()` for the same key are strictly increasing
- At most `2 * 10^5` calls total

## Example

```
TimeMap tm;
tm.set("foo", "bar", 1);
tm.get("foo", 1);  // "bar"   (exact match)
tm.get("foo", 3);  // "bar"   (largest ts ≤ 3 is ts=1)
tm.set("foo", "baz", 4);
tm.get("foo", 4);  // "baz"   (exact match)
tm.get("foo", 5);  // "baz"   (largest ts ≤ 5 is ts=4)
tm.get("foo", 0);  // ""      (no ts ≤ 0 exists)
```

---
*Hint (only read if stuck): Because timestamps for a given key are strictly
increasing, each key's history is a sorted list. Use `std::upper_bound` to
binary-search for the first entry with timestamp > query, then step back one.*
