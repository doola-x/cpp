# Answers

---

## Q1: Min Stack

### Key Insight
You need O(1) `getMin()`, but items leave the stack via `pop()` — so you can't
just store a single global minimum. Instead, track the minimum **at every
level** of the stack. The cleanest way is a second stack that mirrors the main
stack but only holds the running minimum at each depth.

When you push `val`, push `min(val, current_min)` onto the min-stack.  
When you pop, pop both stacks together.

### Complexity
- Time: O(1) all operations
- Space: O(n) — two stacks

### Solution

```cpp
#pragma once
#include <stack>
#include <algorithm>

class MinStack {
    std::stack<int> data;
    std::stack<int> mins;  // mins.top() == minimum at current stack depth

public:
    MinStack() {}

    void push(int val) {
        data.push(val);
        int cur_min = mins.empty() ? val : std::min(val, mins.top());
        mins.push(cur_min);
    }

    void pop() {
        data.pop();
        mins.pop();
    }

    int top()    { return data.top(); }
    int getMin() { return mins.top(); }
};
```

**Alternative**: Use a single stack of `{value, min_at_this_point}` pairs —
same idea, slightly less memory overhead.

---

## Q2: LRU Cache

### Key Insight
`get` and `put` must both be O(1). That means:
- O(1) lookup → `unordered_map`
- O(1) promotion to MRU and O(1) eviction from LRU → `std::list` (doubly-linked)

The map stores iterators into the list. The list is ordered MRU (front) → LRU
(back). On access: splice the node to the front. On eviction: remove from back.

### Complexity
- Time: O(1) average for both get and put
- Space: O(capacity)

### Solution

```cpp
#pragma once
#include <list>
#include <unordered_map>

class LRUCache {
    int cap;
    // list stores (key, value); front = MRU, back = LRU
    std::list<std::pair<int,int>> order;
    std::unordered_map<int, std::list<std::pair<int,int>>::iterator> map;

public:
    LRUCache(int capacity) : cap(capacity) {}

    int get(int key) {
        auto it = map.find(key);
        if (it == map.end()) return -1;
        order.splice(order.begin(), order, it->second); // move to front
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = map.find(key);
        if (it != map.end()) {
            it->second->second = value;
            order.splice(order.begin(), order, it->second);
            return;
        }
        if ((int)order.size() == cap) {
            map.erase(order.back().first);
            order.pop_back();
        }
        order.emplace_front(key, value);
        map[key] = order.begin();
    }
};
```

---

## Q3: Logger Rate Limiter

### Key Insight
Trivially simple once you realize the only state you need is
`unordered_map<string, int>` mapping each message to the last timestamp it
was allowed to print. Check `timestamp - last_seen >= 10`; if true, print and
update the map.

### Complexity
- Time: O(1) average per call
- Space: O(m) where m = number of unique messages seen

### Solution

```cpp
#pragma once
#include <string>
#include <unordered_map>

class Logger {
    std::unordered_map<std::string, int> last_print;

public:
    Logger() {}

    bool shouldPrintMessage(int timestamp, const std::string& message) {
        auto it = last_print.find(message);
        if (it != last_print.end() && timestamp - it->second < 10)
            return false;
        last_print[message] = timestamp;
        return true;
    }
};
```

**Follow-up to think about**: If timestamps could be out-of-order, this simple
approach breaks. You'd need to only update `last_print[message]` when the new
timestamp is actually larger.

---

## Q4: Design Twitter

### Key Insight
Three sub-problems:
1. **Global tweet ordering** — a simple counter incremented on every `postTweet`
   assigns a monotonically increasing "time" to each tweet.
2. **Per-user tweet history** — store each user's tweets as a vector of
   `{time, tweetId}` pairs (newest at the back).
3. **Merging feeds** — to get the 10 most recent tweets from N users, use a
   max-heap seeded with the most recent tweet from each relevant user, then
   pop up to 10 times, re-pushing the next tweet from the same user each time.

### Complexity
- `postTweet`: O(1)
- `getNewsFeed`: O(k log k) where k = number of followed users
- `follow`/`unfollow`: O(1) average

### Solution

```cpp
#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

class Twitter {
    int timer = 0;
    // tweets[userId] = list of {global_time, tweetId}, oldest first
    std::unordered_map<int, std::vector<std::pair<int,int>>> tweets;
    std::unordered_map<int, std::unordered_set<int>> following;

public:
    Twitter() {}

    void postTweet(int userId, int tweetId) {
        tweets[userId].emplace_back(timer++, tweetId);
    }

    std::vector<int> getNewsFeed(int userId) {
        // max-heap: {global_time, tweetId, userId, index_in_user_tweets}
        using T4 = std::tuple<int,int,int,int>;
        std::priority_queue<T4> pq;

        auto seed = [&](int uid) {
            auto& v = tweets[uid];
            if (!v.empty()) {
                int idx = (int)v.size() - 1;
                pq.emplace(v[idx].first, v[idx].second, uid, idx);
            }
        };

        seed(userId);
        for (int f : following[userId]) seed(f);

        std::vector<int> feed;
        while (!pq.empty() && (int)feed.size() < 10) {
            auto [t, tid, uid, idx] = pq.top(); pq.pop();
            feed.push_back(tid);
            if (idx > 0) {
                idx--;
                pq.emplace(tweets[uid][idx].first, tweets[uid][idx].second, uid, idx);
            }
        }
        return feed;
    }

    void follow(int followerId, int followeeId) {
        following[followerId].insert(followeeId);
    }

    void unfollow(int followerId, int followeeId) {
        following[followerId].erase(followeeId);
    }
};
```

---

## Q5: Time-Based Key-Value Store

### Key Insight
Because timestamps for any given key are strictly increasing (guaranteed),
each key's history is a naturally sorted list of `{timestamp, value}` pairs.
A `get(key, ts)` query asks: *what is the last entry with timestamp ≤ ts?*

That's a textbook `std::upper_bound` call on the timestamp dimension — find
the first entry with timestamp > ts, then step back one. If stepping back
falls off the front, return `""`.

### Complexity
- `set`: O(1) amortized (vector append)
- `get`: O(log n) where n = number of set calls for that key

### Solution

```cpp
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

class TimeMap {
    // history[key] = sorted list of {timestamp, value}
    std::unordered_map<std::string, std::vector<std::pair<int,std::string>>> history;

public:
    TimeMap() {}

    void set(const std::string& key, const std::string& value, int timestamp) {
        history[key].emplace_back(timestamp, value);
    }

    std::string get(const std::string& key, int timestamp) {
        auto hit = history.find(key);
        if (hit == history.end()) return "";

        auto& v = hit->second;
        // upper_bound with a custom comparator on the first element of the pair
        auto it = std::upper_bound(v.begin(), v.end(), timestamp,
            [](int ts, const std::pair<int,std::string>& p) { return ts < p.first; });

        if (it == v.begin()) return "";  // all stored timestamps > query
        --it;
        return it->second;
    }
};
```

**Note**: `std::upper_bound` with a mixed-type comparator requires the search
value to be the *left* argument. The lambda `(int ts, pair& p)` returns
`ts < p.first`, meaning "is the target less than this element?", which is the
correct sense for upper_bound.
