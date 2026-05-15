#include <custom_algorithm.h>
#include <gtest/gtest.h>
#include <vector>

// Verify Rvalue push_back and move semantics
TEST(VectorTestRvalue, PushBackAndAccess) {
  cv::vector<int> v;
  v.push_back(10);
  v.push_back(20);
  v.push_back(30);

  EXPECT_EQ(v[0], 10);
  EXPECT_EQ(v[1], 20);
  EXPECT_EQ(v[2], 30);
}

// Verify lvalue push_back and copy semantics
TEST(VectorTestLvalue, PushBackLvalue) {
  cv::vector<std::string> v;
  std::string str1 = "Hello";
  std::string str2 = "World";

  v.push_back(str1);
  v.push_back(str2);

  EXPECT_EQ(v[0], "Hello");
  EXPECT_EQ(v[1], "World");
}

// Verify that vector resizes correctly
TEST(VectorTestResize, PushBackBeyondInitialCapacity) {
  cv::vector<int> v;
  for (int i = 0; i < 20; i++) {
    v.push_back(i);
  }

  for (int i = 0; i < 20; i++) {
    EXPECT_EQ(v[i], i);
  }

  EXPECT_EQ(v.size(), 20);
}

// Verify that vector can handle different types
TEST(VectorTestTypes, PushBackDifferentTypes) {
  cv::vector<double> v_double;
  v_double.push_back(3.14);
  double val = 2.718;
  v_double.push_back(val);

  EXPECT_DOUBLE_EQ(v_double[0], 3.14);
  EXPECT_DOUBLE_EQ(v_double[1], 2.718);

  cv::vector<std::string> v_string;
  v_string.push_back("foo");
  std::string str = "bar";
  v_string.push_back(str);

  EXPECT_EQ(v_string[0], "foo");
  EXPECT_EQ(v_string[1], "bar");
}
