/******************************
Question 4: Median of Two Sorted Arrays

Given two sorted arrays nums1 and nums2 of size m and n respectively, return the median of the two sorted arrays.
The overall run time complexity should be O(log (m+n)).

Example 1:

Input: nums1 = [1,3], nums2 = [2]
Output: 2.00000
Explanation: merged array = [1,2,3] and median is 2.

Example 2:

Input: nums1 = [1,2], nums2 = [3,4]
Output: 2.50000
Explanation: merged array = [1,2,3,4] and median is (2 + 3) / 2 = 2.5.


Constraints:

    nums1.length == m
    nums2.length == n
    0 <= m <= 1000
    0 <= n <= 1000
    1 <= m + n <= 2000
    -10^6 <= nums1[i], nums2[i] <= 10^6

URL: https://leetcode.com/problems/median-of-two-sorted-arrays
******************************/

#include <algorithm>
#include <limits>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* O(nlog(n)) time; O(n) space
 * Most straight forward solution, just copy both vectors and sort them.
 * This ignores the fact that the inputs are sorted, so it works on unsorted inputs too.
 * Minor optimization: branch elimination at the return instead of checking for parity.
 */
/* class Solution {
 * public:
 *   double findMedianSortedArrays(vector<int> const& nums1, vector<int> const& nums2) {
 *     if (nums1.size() == 0 && nums2.size() == 0) {
 *       return 0.0;
 *     }
 *
 *     auto vec = vector<int>{};
 *     vec.reserve(nums1.size() + nums2.size());
 *     copy(nums1.begin(), nums1.end(), back_inserter(vec));
 *     copy(nums2.begin(), nums2.end(), back_inserter(vec));
 *     sort(vec.begin(), vec.end());
 *
 *     // branch elimination
 *     return (vec[(vec.size() - 1) / 2] + vec[vec.size() / 2]) / 2.0;
 *   }
 * };
 */

/* O(n) time; O(1) space
 * Iterating both vectors simultaneously starting from the front.
 * When reached middle point, calculate the median based on parity of input.
 * Despite being O(n) this is already able to beat 100% somehow.
 */
// class Solution {
// public:
//   double findMedianSortedArrays(vector<int> const& nums1, vector<int> const& nums2) {
//     if (nums1.empty() && nums2.empty()) {
//       return 0.0;
//     }
//     if (nums1.empty()) {
//       return (nums2[(nums2.size() - 1) / 2] + nums2[nums2.size() / 2]) / 2.0;
//     }
//     if (nums2.empty()) {
//       return (nums1[(nums1.size() - 1) / 2] + nums1[nums1.size() / 2]) / 2.0;
//     }
//
//     auto const size = nums1.size() + nums2.size();
//     auto const mid = size / 2;
//     auto it1 = nums1.begin();
//     auto it2 = nums2.begin();
//     size_t count = 0;
//     int temp = 0;
//     while (it1 != nums1.end() && it2 != nums2.end() && count < mid) {
//       count++;
//       if (*it1 < *it2) {
//         temp = *it1;
//         it1++;
//       } else {
//         temp = *it2;
//         it2++;
//       }
//     }
//     while (it1 != nums1.end() && count < mid) {
//       temp = *it1;
//       count++;
//       it1++;
//     }
//     while (it2 != nums2.end() && count < mid) {
//       temp = *it2;
//       count++;
//       it2++;
//     }
//     if (it1 == nums1.end()) {
//       if (size % 2 == 0) {
//         return (temp + *it2) / 2.0;
//       } else {
//         return *it2;
//       }
//     }
//     if (it2 == nums2.end()) {
//       if (size % 2 == 0) {
//         return (temp + *it1) / 2.0;
//       } else {
//         return *it1;
//       }
//     }
//     if (size % 2 == 0) {
//       if (*it1 < *it2) {
//         return (temp + *it1) / 2.0;
//       } else {
//         return (temp + *it2) / 2.0;
//       }
//     } else {
//       if (*it1 < *it2) {
//         return *it1;
//       } else {
//         return *it2;
//       }
//     }
//   }
// };

/* O(log(n)) time; O(1) space
 * Instead of linear search now binary search is used.
 * For further optimization, we can partition both vectors and compare their values on the edge.
 */
class Solution {
public:
  double findMedianSortedArrays(vector<int> const& nums1, vector<int> const& nums2) {
    if (nums1.size() > nums2.size()) {
      return findMedianSortedArrays(nums2, nums1);
    }

    int m = nums1.size(), n = nums2.size();
    int left = 0, right = m;

    while (left <= right) {
      int partA = (left + right) / 2;
      int partB = (m + n + 1) / 2 - partA;

      int maxLeftA  = (partA == 0) ? numeric_limits<int>::min() : nums1[partA - 1];
      int minRightA = (partA == m) ? numeric_limits<int>::max() : nums1[partA];
      int maxLeftB  = (partB == 0) ? numeric_limits<int>::min() : nums2[partB - 1];
      int minRightB = (partB == n) ? numeric_limits<int>::max() : nums2[partB];

      if (maxLeftA <= minRightB && maxLeftB <= minRightA) {
        if ((m + n) % 2 == 0) {
          return (max(maxLeftA, maxLeftB) + min(minRightA, minRightB)) / 2.0;
        } else {
          return max(maxLeftA, maxLeftB);
        }
      } else if (maxLeftA > minRightB) {
        // maxLeftA is too large to be in the smaller half.
        // search in the larger part
        right = partA - 1;
      } else {
        // search in the smaller part
        left = partA + 1;
      }
    }

    return 0.0;
  }
};

//==============================================================================

TEST(Test, s0004_median_of_two_sorted_arrays) {
  auto s = Solution{};
  // EXPECT_EQ(0.0, s.findMedianSortedArrays({}, {}));
  EXPECT_EQ(1.0, s.findMedianSortedArrays({1}, {}));
  EXPECT_EQ(1.5, s.findMedianSortedArrays({1, 2}, {}));
  EXPECT_EQ(1.5, s.findMedianSortedArrays({1}, {2}));
  EXPECT_EQ(2.0, s.findMedianSortedArrays({1, 3}, {2}));
  EXPECT_EQ(2.5, s.findMedianSortedArrays({1, 2}, {3, 4}));
  EXPECT_EQ(3.0, s.findMedianSortedArrays({1, 2, 3, 4, 5}, {1, 2, 3, 4, 5}));
  EXPECT_EQ(3.0, s.findMedianSortedArrays({1, 2, 3, 4, 5}, {1, 2, 4, 5}));
  EXPECT_EQ(3.0, s.findMedianSortedArrays({1, 2, 3}, {4, 5}));
  EXPECT_EQ(3.5, s.findMedianSortedArrays({1, 2, 3}, {4, 5, 6}));
  EXPECT_EQ(9.0, s.findMedianSortedArrays({1, 2, 3, 4, 5}, {6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}));
}
