/******************************
__DEBUG_FUNCTION<canFinish>__
Question 207: Course Schedule
Difficulty: Med.

There are a total of numCourses courses you have to take, labeled from 0 to numCourses - 1. You are given an array prerequisites where prerequisites[i] = [ai, bi] indicates that you must take course bi first if you want to take course ai.

  For example, the pair [0, 1], indicates that to take course 0 you have to first take course 1.

Return true if you can finish all courses. Otherwise, return false.

Example 1:

Input: numCourses = 2, prerequisites = [[1,0]]
Output: true
Explanation: There are a total of 2 courses to take.
To take course 1 you should have finished course 0. So it is possible.

Example 2:

Input: numCourses = 2, prerequisites = [[1,0],[0,1]]
Output: false
Explanation: There are a total of 2 courses to take.
To take course 1 you should have finished course 0, and to take course 0 you should also have finished course 1. So it is impossible.


Constraints:

  1 <= numCourses <= 2000
  0 <= prerequisites.length <= 5000
  prerequisites[i].length == 2
  0 <= ai, bi < numCourses
  All the pairs prerequisites[i] are unique.

URL: https://leetcode.com/problems/course-schedule
******************************/

#include <queue>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

class Solution {
public:
  bool canFinish(int numCourses, vector<vector<int>> const& prerequisites) {
    auto edges  = vector<vector<int>>(numCourses);
    auto numPre = vector<int>(numCourses, 0);

    for (auto const& e : prerequisites) {
      int course = e[0];
      int prereq = e[1];
      edges[prereq].push_back(course);
      ++numPre[course];
    }

    // courses without prerequisites
    auto q = queue<int>{};
    for (int i = 0; i < numCourses; ++i) {
      if (numPre[i] == 0) {
        q.push(i);
      }
    }

    int courseTaken = 0;
    while (!q.empty()) {
      int current = q.front();
      q.pop();
      courseTaken++;
      for (int dependent : edges[current]) {
        numPre[dependent]--;
        if (numPre[dependent] == 0) {
          q.push(dependent);
        }
      }
    }

    return courseTaken == numCourses;
  }
};

//==============================================================================

TEST(Test, s0207_course_schedule) {
  auto s = Solution{};
  EXPECT_TRUE(s.canFinish(2, {{1, 0}}));
  EXPECT_FALSE(s.canFinish(2, {{1, 0}, {0, 1}}));
}

