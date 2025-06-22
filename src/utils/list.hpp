#pragma once
#include <format>
#include <ostream>
#include <vector>
#include <utility>

#define SafeList ListNode __attribute__((__cleanup__(clean_up_list)))

/**
 * Helper struct for working with LeetCode's list questions.
 * The struct definition is exactly the same as official one, with some helper function to make life easier.
 *
 * The "SafeList" macro will auto release memory. Make sure only the head of the list is declared using the macro.
 * Other nodes should either be created using "make_list", or "operator new".
 *
 * We can't use smart pointer here because the solution then will not be able to be submitted (yikes).
 *
 * Usage:
 * SafeList* list = make_list({1, 2, 3});
 * EXPECT_EQ(*list, *list);
 * EXPECT_EQ(list->val , list->val);
 *
 * SafeList* null_list = make_list({});
 * EXPECT_EQ(null_list, nullptr);
 *
 * SafeList* list2 = make_list({1});
 * list2->next = new ListNode(2);
 * EXPECT_EQ("[1, 2]", std::format("{}", *list2));
 *
 * Alternatively use a owned version.
 * This way all nodes are stored in a vector (as), so no manual memory management is needed.
 * auto [a, as] = make_list_owned({1, 2, 3});
 * EXPECT_EQ(1, a->val);
 * EXPECT_EQ(2, a->next->val);
 */
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

static inline std::pair<ListNode*, std::vector<ListNode>> make_list_owned(std::initializer_list<int> values) {
  auto storage = std::vector<ListNode>{};
  if (values.size() == 0) {
    return {nullptr, std::move(storage)};
  }

  storage.reserve(values.size());
  for (int val : values) {
    storage.emplace_back(val);
  }
  for (size_t i = 1; i < storage.size(); i++) {
    storage[i - 1].next = &storage[i];
  }

  return {&storage.front(), std::move(storage)};
}

static inline ListNode* make_list(std::initializer_list<int> list) {
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

static inline void delete_list_nodes(ListNode* node) {
  if (node == nullptr) {
    return;
  }
  if (node->next != nullptr) {
    delete_list_nodes(node->next);
  }
  delete node;
}

static inline void clean_up_list(ListNode** node) {
  if (node == nullptr) {
    return;
  }
  delete_list_nodes(*node);
}

template<>
struct std::formatter<ListNode> : std::formatter<std::vector<int>> {
public:
  template<typename Context>
  constexpr auto format(ListNode const& node, Context& ctx) const {
    auto  vec = std::vector<int>{node.val};
    auto* ptr = node.next;
    while (ptr != nullptr) {
      vec.push_back(ptr->val);
      ptr = ptr->next;
    }
    return std::format_to(ctx.out(), "{}", vec);
  }
};

static inline std::ostream& operator<<(std::ostream& s, ListNode const& node) {
  s << "[";
  s << node.val;
  for (auto* ptr = node.next; ptr != nullptr; ptr = ptr->next) {
    s << ", ";
    s << ptr->val;
  }
    s << "]";
  return s;
}
