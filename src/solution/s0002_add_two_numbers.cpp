#include <utils/test.hpp>
#include <utils/list.hpp>

using namespace std;

//==============================================================================

static ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
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

//==============================================================================

int main() {
  {
    SafeList *a = make_nodes({2, 4, 3});
    SafeList *b = make_nodes({5, 6, 4});
    SafeList *c = make_nodes({7, 0, 8});
    ASSERT_EQ(*c, *addTwoNumbers(a, b));
  }
  {
    SafeList *a = make_nodes({0});
    SafeList *b = make_nodes({0});
    SafeList *c = make_nodes({0});
    ASSERT_EQ(*c, *addTwoNumbers(a, b));
  }
  {
    SafeList *a = make_nodes({9, 9, 9, 9, 9, 9, 9});
    SafeList *b = make_nodes({9, 9, 9, 9});
    SafeList *c = make_nodes({8, 9, 9, 9, 0, 0, 0, 1});
    ASSERT_EQ(*c, *addTwoNumbers(a, b));
  }
}
