#include <custom_algorithm.h>
#include <gtest/gtest.h>
#include <list>
#include <string>
#include <vector>

TEST(RotateTest, RotateSingleElement) {
  std::vector<int> v = {10};
  my_algo::rotate(v.begin(), v.begin(), v.end());
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v[0], 10);
}

TEST(RotateTest, RotateMultipleElements) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  my_algo::rotate(v.begin(), v.begin() + 2, v.end());
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v[0], 3);
  EXPECT_EQ(v[1], 4);
  EXPECT_EQ(v[2], 5);
  EXPECT_EQ(v[3], 1);
  EXPECT_EQ(v[4], 2);
}

TEST(RotateTest, RotateWithZeroDistance) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  my_algo::rotate(v.begin(), v.begin(), v.end());
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[3], 4);
  EXPECT_EQ(v[4], 5);
}

TEST(RotateTest, RotateWithFullDistance) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  my_algo::rotate(v.begin(), v.end(), v.end());
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[3], 4);
  EXPECT_EQ(v[4], 5);
}

TEST(RotateTest, RotateEmptyVector) {
  std::vector<int> v;
  my_algo::rotate(v.begin(), v.begin(), v.end());
  EXPECT_TRUE(v.empty());
}

TEST(RotateTest, RotateString) {
  std::string s = "Hello, World!";
  my_algo::rotate(s.begin(), s.begin() + 4, s.end());
  EXPECT_EQ(s, "o, World!Hell");
}

TEST(RotateTest, RotateList) {
  std::list<char> l = {'a', 'b', 'c', 'd', 'e'};
  auto middle = std::next(l.begin(), 2);
  my_algo::rotate(l.begin(), middle, l.end());
  std::vector<char> expected = {'c', 'd', 'e', 'a', 'b'};
  EXPECT_EQ(std::vector<char>(l.begin(), l.end()), expected);
}
