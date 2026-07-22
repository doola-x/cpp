# Q3: Logger Rate Limiter

Design a logger that receives a stream of timestamped messages and decides
whether each message should be printed, enforcing a **10-second cooldown**
per unique message.

## Class Interface

```cpp
class Logger {
public:
    Logger();
    bool shouldPrintMessage(int timestamp, const std::string& message);
};
```

## Behavior

- `shouldPrintMessage(timestamp, message)`:
  Return `true` if **at least 10 seconds** have elapsed since this message
  last printed (or it has never printed). Return `false` otherwise.
  When `true` is returned, record this as the message's new last-print time.
- Timestamps in calls are non-decreasing.

## Constraints

- `0 <= timestamp <= 10^9`
- At most `10^4` calls

## Example

```
Logger logger;
logger.shouldPrintMessage(1,  "foo"); // true  (first time)
logger.shouldPrintMessage(2,  "bar"); // true  (first time)
logger.shouldPrintMessage(3,  "foo"); // false (3 - 1 = 2 < 10)
logger.shouldPrintMessage(8,  "bar"); // false (8 - 2 = 6 < 10)
logger.shouldPrintMessage(10, "foo"); // false (10 - 1 = 9 < 10)
logger.shouldPrintMessage(11, "foo"); // true  (11 - 1 = 10 >= 10)
```

---
*Hint (only read if stuck): A single hash map from message → last_print_timestamp
is all you need. The check is: `timestamp - last_seen >= 10`.*
