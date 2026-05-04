#include <custom_algorithm.h>
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <iterator> // for std::prev
#include <list>

TEST(FindTest, FindsFirstElement) {
  std::vector<int> v = {10, 20, 30};
  auto it = my_algo::find(v.begin(), v.end(), 10);
  EXPECT_EQ(it, v.begin());
}

TEST(FindTest, FindsLastElement) {
  std::vector<int> v = {10, 20, 30};
  auto it = my_algo::find(v.begin(), v.end(), 30);
  EXPECT_EQ(it, std::prev(v.end()));
}

TEST(FindTest, ReturnsFirstOccurrence) {
  std::vector<int> v = {1, 2, 3, 2, 1};
  auto it = my_algo::find(v.begin(), v.end(), 2);
  EXPECT_EQ(it, v.begin() + 1);
}

TEST(FindTest, FindsExistingElement) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  auto it = my_algo::find(v.begin(), v.end(), 3);
  ASSERT_NE(it, v.end());
  EXPECT_EQ(*it, 3);
}

TEST(FindTest, SingleElementFound) {
  std::vector<int> v = {42};
  auto it = my_algo::find(v.begin(), v.end(), 42);
  EXPECT_EQ(it, v.begin());
}

TEST(FindTest, SingleElementNotFound) {
  std::vector<int> v = {42};
  auto it = my_algo::find(v.begin(), v.end(), 10);
  EXPECT_EQ(it, v.end());
}

TEST(FindTest, ReturnsEndForNonExistingElement) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  auto it = my_algo::find(v.begin(), v.end(), 6);
  EXPECT_EQ(it, v.end());
}

TEST(FindTest, HandlesEmptyRange) {
  std::vector<int> v;
  auto it = my_algo::find(v.begin(), v.end(), 1);
  EXPECT_EQ(it, v.end());
}

TEST(FindTest, WorksWithStrings) {
  std::vector<std::string> v = {"apple", "banana", "cherry"};
  auto it = my_algo::find(v.begin(), v.end(), "banana");
  EXPECT_EQ(*it, "banana");
}

TEST(FindTest, WorksWithConstIterators) {
  const std::vector<int> v = {1, 2, 3};
  auto it = my_algo::find(v.begin(), v.end(), 2);
  EXPECT_EQ(*it, 2);
}

TEST(FindTest, WorksWithList) {
  std::list<int> v = {1, 2, 3};
  auto it = my_algo::find(v.begin(), v.end(), 2);
  EXPECT_EQ(*it, 2);
}
