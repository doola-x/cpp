# Q4: Design Twitter (Simplified Feed)

Design a simplified Twitter with posting, following, and a merged news feed.

## Class Interface

```cpp
class Twitter {
public:
    Twitter();
    void postTweet(int userId, int tweetId);
    std::vector<int> getNewsFeed(int userId);
    void follow(int followerId, int followeeId);
    void unfollow(int followerId, int followeeId);
};
```

## Behavior

- `postTweet(userId, tweetId)` — user composes a new tweet
- `getNewsFeed(userId)` — return the **10 most recent** tweet IDs from the
  user's own posts and the posts of everyone they follow, **most recent first**
- `follow(followerId, followeeId)` — followerId starts following followeeId
- `unfollow(followerId, followeeId)` — followerId stops following followeeId.
  Users always see their own tweets regardless of follow status.

## Constraints

- `1 <= userId, followerId, followeeId <= 500`
- `0 <= tweetId <= 10^4`, all tweet IDs are unique
- Feed contains at most 10 tweets
- At most `3 * 10^4` calls total

## Example

```
Twitter t;
t.postTweet(1, 5);
t.getNewsFeed(1);          // [5]
t.follow(1, 2);
t.postTweet(2, 6);
t.getNewsFeed(1);          // [6, 5]  ← 6 is more recent
t.unfollow(1, 2);
t.getNewsFeed(1);          // [5]
```

---
*Hints (only read if stuck):*
*— You need a global ordering across tweets. A simple counter incremented
on every postTweet gives each tweet a unique "time" value.*
*— getNewsFeed needs to merge multiple per-user tweet lists. A max-heap
(priority_queue) over the front of each list works in O(k log k) where k
is the number of followed users.*
