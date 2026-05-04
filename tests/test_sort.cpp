#include <algorithm> // for std::is_sorted
#include <custom_algorithm.h>
#include <gtest/gtest.h>
#include <vector>

TEST(BubbleSortTest, SortsIntegersAscending) {
  std::vector<int> v = {5, 2, 9, 1, 5, 6};
  my_algo::bubble_sort(v.begin(), v.end());
  EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(BubbleSortTest, HandlesAlreadySorted) {
  std::vector<int> v = {1, 2, 3, 4, 5};

  my_algo::bubble_sort(v.begin(), v.end());

  EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(BubbleSortTest, HandlesReverseSorted) {
  std::vector<int> v = {5, 4, 3, 2, 1};

  my_algo::bubble_sort(v.begin(), v.end());

  EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(BubbleSortTest, SortsWithCustomComparator) {
  std::vector<int> v = {5, 2, 9, 1, 5, 6};
  auto compare = [](int a, int b) { return a > b; };
  my_algo::bubble_sort(v.begin(), v.end(), compare);
  EXPECT_TRUE(std::is_sorted(v.begin(), v.end(), compare));
}

TEST(BubbleSortTest, EmptyRange) {
  std::vector<int> v;
  my_algo::bubble_sort(v.begin(), v.end());
  EXPECT_TRUE(v.empty());
}

TEST(BubbleSortTest, SingleElement) {
  std::vector<int> v = {42};
  my_algo::bubble_sort(v.begin(), v.end());
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v[0], 42);
}

TEST(BubbleSortTest, WorksWithStrings) {
  std::vector<std::string> v = {"banana", "apple", "cherry"};
  my_algo::bubble_sort(v.begin(), v.end());
  EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(BubbleSortTest, HandlesDuplicates) {
  std::vector<int> v = {3, 1, 2, 3, 1};
  my_algo::bubble_sort(v.begin(), v.end());
  EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(BubbleSortTest, HandlesFloats) {
  std::vector<double> v = {3.14, 2.71, 1.41, 1.73};
  my_algo::bubble_sort(v.begin(), v.end());
  EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

// Verify selection sort
TEST(SelectionSortTest, SortsIntegersAscending) {
  std::vector<int> v = {5, 2, 9, 1, 5, 6};
  my_algo::selection_sort(v.begin(), v.end());
  EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(SelectionSortTest, HandlesReverseSorted) {
  std::vector<int> v = {5, 4, 3, 2, 1};

  my_algo::selection_sort(v.begin(), v.end());

  EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(SelectionSortTest, SortsWithCustomComparator) {
  std::vector<int> v = {5, 2, 9, 1, 5, 6};
  auto compare = [](int a, int b) { return a > b; };
  my_algo::selection_sort(v.begin(), v.end(), compare);
  EXPECT_TRUE(std::is_sorted(v.begin(), v.end(), compare));
}

TEST(SelectionSortTest, EmptyRange) {
  std::vector<int> v;
  my_algo::selection_sort(v.begin(), v.end());
  EXPECT_TRUE(v.empty());
}

TEST(SelectionSortTest, SingleElement) {
  std::vector<int> v = {42};
  my_algo::selection_sort(v.begin(), v.end());
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v[0], 42);
}

TEST(SelectionSortTest, WorksWithStrings) {
  std::vector<std::string> v = {"banana", "apple", "cherry"};
  my_algo::selection_sort(v.begin(), v.end());
  EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(SelectionSortTest, HandlesDuplicates) {
  std::vector<int> v = {3, 1, 2, 3, 1};
  my_algo::selection_sort(v.begin(), v.end());
  EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(SelectionSortTest, HandlesFloats) {
  std::vector<double> v = {3.14, 2.71, 1.41, 1.73};
  my_algo::selection_sort(v.begin(), v.end());
  EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));
}

TEST(SelectionSortTest, ExactOutputMatch) {
  std::vector<int> v = {5, 2, 9, 1};
  std::vector<int> expected = {1, 2, 5, 9};

  my_algo::selection_sort(v.begin(), v.end());

  EXPECT_EQ(v, expected);
}