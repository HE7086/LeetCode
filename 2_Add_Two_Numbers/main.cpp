#include <fmt/core.h>
#include <fmt/ranges.h>
#include <vector>

#define SafeList ListNode __attribute__((__cleanup__(clean_up)))

using namespace std;

// why leetcode why
struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

static ListNode *make_nodes(std::initializer_list<int> list) {
  if (list.size() == 0) {
    return nullptr;
  }
  auto it = list.begin();
  ListNode *head = new ListNode(*it++);
  ListNode *ptr = head;
  for (; it != list.end(); ++it) {
    ptr->next = new ListNode(*it);
    ptr = ptr->next;
  }
  return head;
}

static void delete_nodes(ListNode *node) {
  if (node->next != nullptr) {
    delete_nodes(node->next);
  }
  delete node;
}

static void clean_up(ListNode **node) { delete_nodes(*node); }

template <> class fmt::formatter<ListNode> {
public:
  constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
  template <typename Context>
  constexpr auto format(const ListNode &node, Context &ctx) const {
    std::vector<int> vec{node.val};
    ListNode *p = node.next;
    while (p != nullptr) {
      vec.push_back(p->val);
      p = p->next;
    }
    return fmt::format_to(ctx.out(), "{}", vec);
  }
};

//==============================================================================

class Solution {
public:
  ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
    int carry = 0;
    ListNode *l3 = new ListNode();
    ListNode *ptr = l3;

    while (l1 != nullptr || l2 != nullptr) {
      int sum = carry;
      if (l1) {
        sum += l1->val;
        l1 = l1->next;
      }
      if (l2) {
        sum += l2->val;
        l2 = l2->next;
      }

      carry = sum / 10;
      ptr->val = sum % 10;

      if (l1 || l2) {
        ptr->next = new ListNode();
        ptr = ptr->next;
      }
    }

    if (carry != 0) {
      ptr->next = new ListNode(carry);
    }

    return l3;
  }
};

//==============================================================================

int main() {
  auto solution = Solution{};
  {
    SafeList *a = make_nodes({2, 4, 3});
    SafeList *b = make_nodes({5, 6, 4});
    SafeList *c = make_nodes({7, 0, 8});
    SafeList *d = solution.addTwoNumbers(a, b);
    fmt::println("{} == {}", *d, *c);
  }
  {
    SafeList *a = make_nodes({0});
    SafeList *b = make_nodes({0});
    SafeList *c = make_nodes({0});
    SafeList *d = solution.addTwoNumbers(a, b);
    fmt::println("{} == {}", *d, *c);
  }
  {
    SafeList *a = make_nodes({9, 9, 9, 9, 9, 9, 9});
    SafeList *b = make_nodes({9, 9, 9, 9});
    SafeList *c = make_nodes({8, 9, 9, 9, 0, 0, 0, 1});
    SafeList *d = solution.addTwoNumbers(a, b);
    fmt::println("{} == {}", *d, *c);
  }
}
