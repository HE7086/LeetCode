/******************************
__DEBUG_FUNCTION<findSubstring>__
Question 30: Substring with Concatenation of All Words
Difficulty: Hard

You are given a string s and an array of strings words. All the strings of words are of the same length.
A concatenated string is a string that exactly contains all the strings of any permutation of words concatenated.

  For example, if words = ["ab","cd","ef"], then "abcdef", "abefcd", "cdabef", "cdefab", "efabcd", and "efcdab" are all concatenated strings. "acdbef" is not a concatenated string because it is not the concatenation of any permutation of words.

Return an array of the starting indices of all the concatenated substrings in s. You can return the answer in any order.

Example 1:

Input: s = "barfoothefoobarman", words = ["foo","bar"]
Output: [0,9]
Explanation:
The substring starting at 0 is "barfoo". It is the concatenation of ["bar","foo"] which is a permutation of words.
The substring starting at 9 is "foobar". It is the concatenation of ["foo","bar"] which is a permutation of words.

Example 2:

Input: s = "wordgoodgoodgoodbestword", words = ["word","good","best","word"]
Output: []
Explanation:
There is no concatenated substring.

Example 3:

Input: s = "barfoofoobarthefoobarman", words = ["bar","foo","the"]
Output: [6,9,12]
Explanation:
The substring starting at 6 is "foobarthe". It is the concatenation of ["foo","bar","the"].
The substring starting at 9 is "barthefoo". It is the concatenation of ["bar","the","foo"].
The substring starting at 12 is "thefoobar". It is the concatenation of ["the","foo","bar"].


Constraints:

  1 <= s.length <= 10^4
  1 <= words.length <= 5000
  1 <= words[i].length <= 30
  s and words[i] consist of lowercase English letters.

URL: https://leetcode.com/problems/substring-with-concatenation-of-all-words
******************************/

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

//==============================================================================

/* N: words.size()
 * M: s.length()
 * L: words[i].length()
 *
 * O(N! * N * M * L) time; O(1) space
 * Trivial solution without any optimization.
 * Terrible performance because of the permutation.
 */
/* class Solution {
 * public:
 *   vector<int> findSubstring(string const s, vector<string>& words) {
 *     auto ans = vector<int>{};
 *
 *     size_t length = ranges::fold_left(words | views::transform(&string::length), 0ul, plus<size_t>{});
 *     if (s.length() < length) {
 *       return ans;
 *     }
 *
 *     sort(words.begin(), words.end());
 *     do {
 *       auto w = views::join(words) | ranges::to<string>();
 *       for (size_t i = 0; i < s.length() - length + 1; i++) {
 *         if (equal(w.begin(), w.end(), s.begin() + i)) {
 *           ans.push_back(i);
 *         }
 *       }
 *     } while (next_permutation(words.begin(), words.end()));
 *     return ans;
 *   }
 * };
 */

/* O(N^2 * M * L) time; O(N) space
 * Slightly optimized version.
 * We only need to count the occurances of each word to determine whether the string is a valid permutation.
 * Without iterating through all the possible permutations this is way faster than the previous version.
 */
/* class Solution {
 *   size_t occurances(string_view sv, string_view word) {
 *     size_t len = word.length();
 *     size_t ans = 0;
 *     for (size_t i = 0; i < sv.length(); i += len) {
 *       if (sv.substr(i, len) == word) {
 *         ans++;
 *       }
 *     }
 *     return ans;
 *   }
 *
 * public:
 *   vector<int> findSubstring(string_view s, vector<string>& words) {
 *     vector<int> ans;
 *
 *     size_t len = words.front().length() * words.size();
 *     if (s.size() < len) {
 *       return ans;
 *     }
 *
 *     auto word_freq = map<string_view, size_t>{};
 *     for (string_view word : words) {
 *       word_freq[word]++;
 *     }
 *
 *     for (size_t i = 0; i < s.length() - len + 1; i++) {
 *       auto sv = s.substr(i, len);
 *       for (auto [word, f] : word_freq) {
 *         if (f != occurances(sv, word)) {
 *           goto skip;
 *         }
 *       }
 *       ans.push_back(i);
 * skip:
 *     }
 *
 *     return ans;
 *   }
 * };
 */

/* O(M * L) time, O(N) space
 */
class Solution {
public:
  vector<int> findSubstring(string_view s, vector<string> const& words) {
    auto   ans    = vector<int>{};
    size_t length = words.front().length();

    if (s.size() < words.size() * length) {
      return ans;
    }

    // build target reference map
    auto map = unordered_map<string_view, size_t>{};
    for (auto const& word : words) {
      map[word]++;
    }

    // matched words may have offset
    // because all words have same length, check all possible offsets [0, length)
    for (size_t offset = 0; offset < length; ++offset) {
      size_t matched = 0;
      auto   seen    = unordered_map<string_view, size_t>{};

      for (size_t i = offset; i < s.size() - length + 1; i += length) {
        auto next = s.substr(i, length);

        if (!map.contains(next)) {
          // next word is not a valid word =>
          // all sliding windows containing such word cannot be possibly valid
          seen.clear();
          matched = 0;
          continue;
        }
        seen[next]++;
        matched++;

        while (seen[next] > map[next]) {
          // if there are too many matches for a word,
          // the front of the sliding window is removed until the window is potentially valid again.
          auto last = s.substr(i - (matched - 1) * length, length);
          seen[last]--;
          matched--;
        }

        // found a valid match
        if (matched == words.size()) {
          ans.push_back(i - (matched - 1) * length);
        }
      }
    }

    return ans;
  }
};

//==============================================================================

TEST(Test, s0030_substring_with_concatenation_of_all_words) {
  auto check = [](vector<int> expected, string input, vector<string> words) {
    auto ans = Solution{}.findSubstring(input, words);

    sort(expected.begin(), expected.end());
    sort(ans.begin(), ans.end());

    EXPECT_EQ(expected, ans);
  };
  check({0, 9}, "barfoothefoobarman", {"foo", "bar"});
  check({}, "b", {"bbb", "bbb"});
  check({}, "wordgoodgoodgoodbestword", {"word", "good", "best", "word"});
  check({6, 9, 12}, "barfoofoobarthefoobarman", {"bar", "foo", "the"});
  check({8}, "wordgoodgoodgoodbestword", {"word", "good", "best", "good"});
  check({0}, "ababababab", {"ababa", "babab"});
  check({1}, "ababaab", {"ab", "ba", "ba"});
  check(
      {935},
      "pjzkrkevzztxductzzxmxsvwjkxpvukmfjywwetvfnujhweiybwvvsrfequzkhossmootkmyxgjgfordrpapjuunmqnxxdrqrfgkrsjqbszgiqlc"
      "fnrpjlcwdrvbumtotzylshdvccdmsqoadfrpsvnwpizlwszrtyclhgilklydbmfhuywotjmktnwrfvizvnmfvvqfiokkdprznnnjycttprkxpuyk"
      "hmpchiksyucbmtabiqkisgbhxngmhezrrqvayfsxauampdpxtafniiwfvdufhtwajrbkxtjzqjnfocdhekumttuqwovfjrgulhekcpjszyynadxh"
      "nttgmnxkduqmmyhzfnjhducesctufqbumxbamalqudeibljgbspeotkgvddcwgxidaiqcvgwykhbysjzlzfbupkqunuqtraxrlptivshhbihtsig"
      "tpipguhbhctcvubnhqipncyxfjebdnjyetnlnvmuxhzsdahkrscewabejifmxombiamxvauuitoltyymsarqcuuoezcbqpdaprxmsrickwpgwpso"
      "plhugbikbkotzrtqkscekkgwjycfnvwfgdzogjzjvpcvixnsqsxacfwndzvrwrycwxrcismdhqapoojegggkocyrdtkzmiekhxoppctytvphjynr"
      "htcvxcobxbcjjivtfjiwmduhzjokkbctweqtigwfhzorjlkpuuliaipbtfldinyetoybvugevwvhhhweejogrghllsouipabfafcxnhukcbtmxzs"
      "hoyyufjhzadhrelweszbfgwpkzlwxkogyogutscvuhcllphshivnoteztpxsaoaacgxyaztuixhunrowzljqfqrahosheukhahhbiaxqzfmmwcjx"
      "ountkevsvpbzjnilwpoermxrtlfroqoclexxisrdhvfsindffslyekrzwzqkpeocilatftymodgztjgybtyheqgcpwogdcjlnlesefgvimwbxcbz"
      "vaibspdjnrpqtyeilkcspknyylbwndvkffmzuriilxagyerjptbgeqgebiaqnvdubrtxibhvakcyotkfonmseszhczapxdlauexehhaireihxspl"
      "gdgmxfvaevrbadbwjbdrkfbbjjkgcztkcbwagtcnrtqryuqixtzhaakjlurnumzyovawrcjiwabuwretmdamfkxrgqgcdgbrdbnugzecbgyxxdqm"
      "isaqcyjkqrntxqmdrczxbebemcblftxplafnyoxqimkhcykwamvdsxjezkpgdpvopddptdfbprjustquhlazkjfluxrzopqdstulybnqvyknrchb"
      "phcarknnhhovweaqawdyxsqsqahkepluypwrzjegqtdoxfgzdkydeoxvrfhxusrujnmjzqrrlxglcmkiykldbiasnhrjbjekystzilrwkzhontwm"
      "ehrfsrzfaqrbbxncphbzuuxeteshyrveamjsfiaharkcqxefghgceeixkdgkuboupxnwhnfigpkwnqdvzlydpidcljmflbccarbiegsmweklwngv"
      "ygbqpescpeichmfidgsjmkvkofvkuehsmkkbocgejoiqcnafvuokelwuqsgkyoekaroptuvekfvmtxtqshcwsztkrzwrpabqrrhnlerxjojemcxe"
      "l",
      {"dhvf", "sind", "ffsl", "yekr", "zwzq", "kpeo", "cila", "tfty", "modg", "ztjg", "ybty", "heqg", "cpwo", "gdcj",
       "lnle", "sefg", "vimw", "bxcb"}
  );
}
