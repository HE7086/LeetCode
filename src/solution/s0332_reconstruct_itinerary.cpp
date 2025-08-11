/******************************
__DEBUG_FUNCTION<findItinerary>__
Question 332: Reconstruct Itinerary
Difficulty: Hard

You are given a list of airline tickets where tickets[i] = [fromi, toi] represent the departure and the arrival airports of one flight. Reconstruct the itinerary in order and return it.
All of the tickets belong to a man who departs from "JFK", thus, the itinerary must begin with "JFK". If there are multiple valid itineraries, you should return the itinerary that has the smallest lexical order when read as a single string.

  For example, the itinerary ["JFK", "LGA"] has a smaller lexical order than ["JFK", "LGB"].

You may assume all tickets form at least one valid itinerary. You must use all the tickets once and only once.

Example 1:

Input: tickets = [["MUC","LHR"],["JFK","MUC"],["SFO","SJC"],["LHR","SFO"]]
Output: ["JFK","MUC","LHR","SFO","SJC"]

Example 2:

Input: tickets = [["JFK","SFO"],["JFK","ATL"],["SFO","ATL"],["ATL","JFK"],["ATL","SFO"]]
Output: ["JFK","ATL","JFK","SFO","ATL","SFO"]
Explanation: Another possible reconstruction is ["JFK","SFO","ATL","JFK","ATL","SFO"] but it is larger in lexical order.


Constraints:

  1 <= tickets.length <= 300
  tickets[i].length == 2
  fromi.length == 3
  toi.length == 3
  fromi and toi consist of uppercase English letters.
  fromi != toi

URL: https://leetcode.com/problems/reconstruct-itinerary
******************************/

#include <functional>
#include <queue>
#include <stack>
#include <string>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* dfs with stack
 */
class Solution {
public:
  vector<string> findItinerary(vector<vector<string>> const& tickets) {
    auto adj_minheap = unordered_map<string, priority_queue<string, vector<string>, greater<string>>>{};
    adj_minheap.reserve(tickets.size() * 2);

    for (auto const& t : tickets) {
      adj_minheap[t[0]].push(t[1]);
    }

    auto itinerary = vector<string>{};
    itinerary.reserve(tickets.size() + 1);

    auto s = stack<string>{};
    s.push("JFK");

    while (!s.empty()) {
      auto& current = s.top();
      auto  it      = adj_minheap.find(current);
      if (it != adj_minheap.end() && !it->second.empty()) {
        auto next = it->second.top();
        it->second.pop();
        s.push(std::move(next));
      } else {
        itinerary.push_back(std::move(current));
        s.pop();
      }
    }

    reverse(itinerary.begin(), itinerary.end());
    return itinerary;
  }
};

//==============================================================================

TEST(Test, s0332_reconstruct_itinerary) {
  // clang-format off
  EXPECT_EQ((vector<string>{"JFK", "SFO", "JFK", "ATL", "AAA", "ATL", "BBB", "ATL", "CCC", "ATL", "DDD", "ATL", "EEE", "ATL", "FFF", "ATL", "GGG", "ATL", "HHH", "ATL", "III", "ATL", "JJJ", "ATL", "KKK", "ATL", "LLL", "ATL", "MMM", "ATL", "NNN", "ATL"}),
      Solution{}.findItinerary({{"JFK","SFO"},{"JFK","ATL"},{"SFO","JFK"},{"ATL","AAA"},{"AAA","ATL"},{"ATL","BBB"},{"BBB","ATL"},{"ATL","CCC"},{"CCC","ATL"},{"ATL","DDD"},{"DDD","ATL"},{"ATL","EEE"},{"EEE","ATL"},{"ATL","FFF"},{"FFF","ATL"},{"ATL","GGG"},{"GGG","ATL"},{"ATL","HHH"},{"HHH","ATL"},{"ATL","III"},{"III","ATL"},{"ATL","JJJ"},{"JJJ","ATL"},{"ATL","KKK"},{"KKK","ATL"},{"ATL","LLL"},{"LLL","ATL"},{"ATL","MMM"},{"MMM","ATL"},{"ATL","NNN"},{"NNN","ATL"}})
  );
  EXPECT_EQ((vector<string>{"JFK", "MUC", "LHR", "SFO", "SJC"}),
      Solution{}.findItinerary({{"MUC", "LHR"}, {"JFK", "MUC"}, {"SFO", "SJC"}, {"LHR", "SFO"}})
  );
  EXPECT_EQ((vector<string>{"JFK", "ATL", "JFK", "SFO", "ATL", "SFO"}),
      Solution{}.findItinerary({{"JFK", "SFO"}, {"JFK", "ATL"}, {"SFO", "ATL"}, {"ATL", "JFK"}, {"ATL", "SFO"}})
  );
  // clang-format on
}
