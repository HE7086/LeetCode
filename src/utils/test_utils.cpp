#include "list.hpp"
#include "tree.hpp"

#include <format>
#include <initializer_list>
#include <sstream>
#include <gtest/gtest.h>

TEST(Test_utils, list) {
  {
    SafeList* a = make_list({});
    EXPECT_EQ(a, nullptr);
  }
  {
    SafeList* a = make_list({1});
    SafeList* b = make_list({2});
    EXPECT_NE(*a, *b);
  }
  {
    SafeList* a = make_list({1, 2, 3});
    SafeList* b = make_list({1, 2});

    EXPECT_NE(*a, *b);

    ListNode* c   = make_list({3});
    b->next->next = c;

    EXPECT_EQ(*a, *b);

    auto s = std::stringstream{};
    s << *a;
    EXPECT_EQ("[1, 2, 3]", s.str());

#if __cpp_lib_format_ranges >= 202207L
    EXPECT_EQ("[1, 2, 3]", std::format("{}", *a));
#endif
  }
}
TEST(Test_utils, list_owned) {
  {
    auto [a, as] = make_list_owned({});
    EXPECT_EQ(a, nullptr);
  }
  {
    auto [a, as] = make_list_owned({1});
    auto [b, bs] = make_list_owned({2});
    EXPECT_NE(*a, *b);
  }
  {
    auto [a, as] = make_list_owned({1, 2, 3});
    auto [b, bs] = make_list_owned({1, 2});

    EXPECT_NE(*a, *b);

    auto [c, cs]  = make_list_owned({3});
    b->next->next = c;

    EXPECT_EQ(*a, *b);

    auto s = std::stringstream{};
    s << *a;
    EXPECT_EQ("[1, 2, 3]", s.str());

#if __cpp_lib_format_ranges >= 202207L
    EXPECT_EQ("[1, 2, 3]", std::format("{}", *a));
#endif
  }
}

TEST(Test_utils, tree) {
  {
    SafeTree* a = new TreeNode();
    SafeTree* b = new TreeNode();
    EXPECT_EQ(*a, *b);

    b->val = 1;
    EXPECT_NE(*a, *b);
    EXPECT_EQ(0, a->height());
  }
  {
    SafeTree* a = new TreeNode(1);
    SafeTree* b = new TreeNode(2);
    a->left     = b;

    SafeTree* c = new TreeNode(1);
    SafeTree* d = new TreeNode(2);
    c->left     = d;

    EXPECT_EQ(*a, *c);
    EXPECT_EQ(1, a->height());
  }
  {
    SafeTree* c = new TreeNode(1);
    SafeTree* a = new TreeNode(1, c, nullptr);
    SafeTree* b = new TreeNode(1, c, nullptr);

    EXPECT_EQ(*a, *b);
  }
  {
    SafeTree* c = new TreeNode(1);
    SafeTree* a = new TreeNode(1, c, nullptr);
    SafeTree* b = new TreeNode(1, nullptr, c);

    EXPECT_NE(*a, *b);
  }
}
