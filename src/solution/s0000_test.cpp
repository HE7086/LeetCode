/* __DEBUG_FUNCTION<main>__ */

#include <print>
#include <utils/list.hpp>

int main() {
  auto [a, _] = ListNode::make_list_owned({1, 2, 3});
  auto* b = a->next;
  auto* c = b->next;
  c->next = a;
  ListNode* d = nullptr;

  std::println("{}", a->val);
}
