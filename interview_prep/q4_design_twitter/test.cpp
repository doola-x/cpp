#include <iostream>
#include <vector>
#include "solution.h"

static int _passes = 0, _failures = 0;

static void check_feed(const std::string& label, std::vector<int> got, std::vector<int> expected) {
    if (got == expected) {
        std::cout << "  PASS [" << label << "]\n";
        _passes++;
        return;
    }
    std::cerr << "  FAIL [" << label << "]: expected [";
    for (int i = 0; i < (int)expected.size(); i++) { if (i) std::cerr << ","; std::cerr << expected[i]; }
    std::cerr << "], got [";
    for (int i = 0; i < (int)got.size(); i++) { if (i) std::cerr << ","; std::cerr << got[i]; }
    std::cerr << "]\n";
    _failures++;
}

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
    std::cout << "=== Q4: Design Twitter ===\n";

    // Problem example
    {
        Twitter t;
        t.postTweet(1, 5);
        check_feed("own tweet in feed", t.getNewsFeed(1), {5});
        t.follow(1, 2);
        t.postTweet(2, 6);
        check_feed("followed tweet appears most recent first", t.getNewsFeed(1), {6, 5});
        t.unfollow(1, 2);
        check_feed("unfollowed tweet disappears", t.getNewsFeed(1), {5});
    }

    // User's own tweets appear without following themselves
    {
        Twitter t;
        t.postTweet(1, 1);
        t.postTweet(1, 2);
        check_feed("own tweets most recent first", t.getNewsFeed(1), {2, 1});
    }

    // Feed is capped at 10 entries, most recent first
    {
        Twitter t;
        for (int i = 1; i <= 12; i++) t.postTweet(1, i);
        auto feed = t.getNewsFeed(1);
        CHECK_EQ("feed capped at 10", (int)feed.size(), 10);
        check_feed("feed contains 10 most recent", feed, {12,11,10,9,8,7,6,5,4,3});
    }

    // Empty feed for a user with no tweets and no follows
    {
        Twitter t;
        check_feed("brand new user has empty feed", t.getNewsFeed(99), {});
    }

    // Interleaved tweets from multiple users are ordered by recency
    {
        Twitter t;
        t.follow(1, 2);
        t.postTweet(1, 1); // time 0
        t.postTweet(2, 2); // time 1
        t.postTweet(1, 3); // time 2
        t.postTweet(2, 4); // time 3
        check_feed("interleaved tweets by global recency", t.getNewsFeed(1), {4, 3, 2, 1});
    }

    // Unfollow: previously visible tweets are removed from feed
    {
        Twitter t;
        t.postTweet(2, 10);
        t.follow(1, 2);
        check_feed("can see followed user tweet", t.getNewsFeed(1), {10});
        t.unfollow(1, 2);
        check_feed("tweet gone after unfollow", t.getNewsFeed(1), {});
    }

    // User always sees their own tweets even without following anyone
    {
        Twitter t;
        t.postTweet(5, 7);
        t.postTweet(5, 8);
        check_feed("own tweets without any follows", t.getNewsFeed(5), {8, 7});
    }

    // Following a non-existent user (no tweets) should not crash
    {
        Twitter t;
        t.follow(1, 999);
        check_feed("follow user with no tweets", t.getNewsFeed(1), {});
        t.postTweet(1, 42);
        check_feed("own tweet still visible", t.getNewsFeed(1), {42});
    }

    // Feed from multiple followed users picks the 10 most recent overall
    {
        Twitter t;
        t.follow(1, 2);
        t.follow(1, 3);
        // User 2: tweets 1,2,3,4,5 (oldest first)
        for (int i = 1; i <= 5; i++) t.postTweet(2, i);
        // User 3: tweets 6,7,8,9,10,11
        for (int i = 6; i <= 11; i++) t.postTweet(3, i);
        // User 3's tweets were all posted after user 2's, so top 10 = 11..3 (skipping 1,2)
        auto feed = t.getNewsFeed(1);
        CHECK_EQ("multi-user feed size", (int)feed.size(), 10);
        check_feed("multi-user feed top 10", feed, {11,10,9,8,7,6,5,4,3,2});
    }

    std::cout << "\nResult: " << _passes << " passed, " << _failures << " failed\n";
    return _failures > 0 ? 1 : 0;
}
