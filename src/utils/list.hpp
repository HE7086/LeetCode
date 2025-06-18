#pragma once
#include <format>
#include <vector>

#define SafeList ListNode __attribute__((__cleanup__(clean_up)))

struct ListNode {
  int       val;
  ListNode* next;
  ListNode()
      : val(0), next(nullptr) {}
  ListNode(int x)
      : val(x), next(nullptr) {}
  ListNode(int x, ListNode* next)
      : val(x), next(next) {}

  bool operator==(ListNode const& other) const {
    if (val != other.val || (next != nullptr && other.next == nullptr) || (next == nullptr && other.next != nullptr)) {
      return false;
    }
    if (next != nullptr) {
      return next->operator==(*other.next);
    }
    return true;
  }
};

static inline ListNode* make_nodes(std::initializer_list<int> list) {
  if (list.size() == 0) {
    return nullptr;
  }
  auto  it   = list.begin();
  auto* head = new ListNode(*it++);
  auto* ptr  = head;
  for (; it != list.end(); ++it) {
    ptr->next = new ListNode(*it);
    ptr       = ptr->next;
  }
  return head;
}

static inline void delete_nodes(ListNode* node) {
  if (node->next != nullptr) {
    delete_nodes(node->next);
  }
  delete node;
}

static inline void clean_up(ListNode** node) {
  delete_nodes(*node);
}

template<>
class std::formatter<ListNode> {
public:
  constexpr auto parse(std::format_parse_context& ctx) {
    return ctx.begin();
  }
  template<typename Context>
  constexpr auto format(ListNode const& node, Context& ctx) const {
    auto  vec = std::vector<int>{node.val};
    auto* p   = node.next;
    while (p != nullptr) {
      vec.push_back(p->val);
      p = p->next;
    }
    return std::format_to(ctx.out(), "{}", vec);
  }
};
