/******************************
__DEBUG_FUNCTION<LRUCache>__
Question 146: LRU Cache
Difficulty: Med.

Design a data structure that follows the constraints of a Least Recently Used (LRU) cache.
Implement the LRUCache class:

  LRUCache(int capacity) Initialize the LRU cache with positive size capacity.
  int get(int key) Return the value of the key if the key exists, otherwise return -1.
  void put(int key, int value) Update the value of the key if the key exists. Otherwise, add the key-value pair to the cache. If the number of keys exceeds the capacity from this operation, evict the least recently used key.

The functions get and put must each run in O(1) average time complexity.

Example 1:

Input
["LRUCache", "put", "put", "get", "put", "get", "put", "get", "get", "get"]
[[2], [1, 1], [2, 2], [1], [3, 3], [2], [4, 4], [1], [3], [4]]
Output
[null, null, null, 1, null, -1, null, -1, 3, 4]
Explanation
LRUCache lRUCache = new LRUCache(2);
lRUCache.put(1, 1); // cache is {1=1}
lRUCache.put(2, 2); // cache is {1=1, 2=2}
lRUCache.get(1);    // return 1
lRUCache.put(3, 3); // LRU key was 2, evicts key 2, cache is {1=1, 3=3}
lRUCache.get(2);    // returns -1 (not found)
lRUCache.put(4, 4); // LRU key was 1, evicts key 1, cache is {4=4, 3=3}
lRUCache.get(1);    // return -1 (not found)
lRUCache.get(3);    // return 3
lRUCache.get(4);    // return 4


Constraints:

  1 <= capacity <= 3000
  0 <= key <= 10^4
  0 <= value <= 10^5
  At most 2 * 10^5 calls will be made to get and put.

URL: https://leetcode.com/problems/lru-cache
******************************/

#include <list>
#include <unordered_map>
#include <utility>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

class LRUCache {
  size_t capacity;
  list<pair<int, int>> order{};
  unordered_map<int, decltype(order.end())> location{};

public:
  LRUCache(size_t capacity) : capacity(capacity) {}

  int get(int key) {
    auto it = location.find(key);
    if (it == location.end()) {
      return -1;
    }
    order.splice(order.begin(), order, it->second);
    return it->second->second;
  }

  void put(int key, int value) {
    // update existing value
    auto it = location.find(key);
    if (it != location.end()) {
      it->second->second = value;
      order.splice(order.begin(), order, it->second);
      return;
    }

    // evict last value
    if (order.size() == capacity) {
      int old = order.back().first;
      location.erase(old);
      order.pop_back();
    }

    // put new value to front
    order.emplace_front(key, value);
    location[key] = order.begin();
  }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */

//==============================================================================

TEST(Test, s0146_lru_cache) {
  LRUCache lRUCache(2);
  lRUCache.put(1, 1);
  lRUCache.put(2, 2);
  EXPECT_EQ(1, lRUCache.get(1));
  lRUCache.put(3, 3);
  EXPECT_EQ(-1, lRUCache.get(2));
  lRUCache.put(4, 4);
  EXPECT_EQ(-1, lRUCache.get(1));
  EXPECT_EQ(3, lRUCache.get(3));
  EXPECT_EQ(4, lRUCache.get(4));
}

