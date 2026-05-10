#include <custom_algorithm.h>
#include <gtest/gtest.h>
#include <list>
#include <string>
#include <vector>

TEST(ReverseTest, ReverseSingleElement) {
  std::vector<int> v = {10};
  my_algo::reverse(v.begin(), v.end());
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v[0], 10);
}

TEST(ReverseTest, ReverseMultipleElements) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  my_algo::reverse(v.begin(), v.end());
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v[0], 5);
  EXPECT_EQ(v[1], 4);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[3], 2);
  EXPECT_EQ(v[4], 1);
}

TEST(ReverseTest, ReverseEmptyVector) {
  std::vector<int> v;
  my_algo::reverse(v.begin(), v.end());
  EXPECT_TRUE(v.empty());
}

TEST(ReverseTest, ReverseString) {
  std::string s = "Hello, World!";
  my_algo::reverse(s.begin(), s.end());
  EXPECT_EQ(s, "!dlroW ,olleH");
}

TEST(ReverseTest, ReverseList) {
  std::list<char> l = {'a', 'b', 'c', 'd', 'e'};
  my_algo::reverse(l.begin(), l.end());
  std::vector<char> expected = {'e', 'd', 'c', 'b', 'a'};
  EXPECT_EQ(std::vector<char>(l.begin(), l.end()), expected);
}
