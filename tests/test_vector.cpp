#include <gtest/gtest.h>
#include <memory> // Required for std::unique_ptr
#include <string>
#include <vector.hpp>
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
  EXPECT_GE(v.capacity(), 20);
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

// Verify copy constructor
TEST(VectorTestConstructor, CopyConstructor) {
  cv::vector<int> v1;
  for (int i = 0; i < 5; i++)
    v1.push_back(i);

  // Act
  cv::vector<int> v2{v1};

  // Assert metadata
  EXPECT_EQ(v1.size(), v2.size());
  EXPECT_EQ(v2.capacity(), v1.capacity());

  // Assert elements match
  for (int i = 0; i < 5; i++)
    EXPECT_EQ(v1[i], v2[i]);

  // Deep copy check (isolation test)
  for (int i = 0; i < 5; i++) {
    v1[i] = i * i;
    EXPECT_EQ(i, v2[i]);
  }
}

// Critical Edge Case: Empty Vector
TEST(VectorTestConstructor, CopyConstructorEmpty) {
  cv::vector<int> v1;

  // Act
  cv::vector<int> v2{v1};

  // Assert
  EXPECT_EQ(v2.size(), 0);
  EXPECT_EQ(v2.capacity(), v1.capacity());
}

// Object Lifetime Case: Check for memory leaks / object copies
TEST(VectorTestConstructor, CopyConstructorObjectLifetimes) {
  // Use std::string to ensure heap-allocated elements copy correctly
  cv::vector<std::string> v1;
  v1.push_back("hello");
  v1.push_back("world");

  // Act
  cv::vector<std::string> v2{v1};

  // Assert
  EXPECT_EQ(v2[0], "hello");
  EXPECT_EQ(v2[1], "world");

  // Change v1 to ensure strings are fully duplicated copies
  v1[0] = "changed";
  EXPECT_EQ(v2[0], "hello");
}
TEST(VectorTestAssignment, CopyAssignmentDeepCopy) {
  cv::vector<std::string> source;
  source.push_back("Hello");
  source.push_back("World");

  cv::vector<std::string> target;
  target.push_back("dummy");

  target = source;

  EXPECT_EQ(target.size(), source.size());
  EXPECT_EQ(target.capacity(), source.capacity());
  EXPECT_EQ(target[0], "Hello");
  EXPECT_EQ(target[1], "World");

  source[1] = "Changed";
  EXPECT_EQ(target[1], "World");
}

TEST(VectorTestConstructor, CopyConstructorDeepCopyAfterResize) {
  cv::vector<std::string> original;
  for (int i = 0; i < 15; ++i) {
    original.push_back("item" + std::to_string(i));
  }

  cv::vector<std::string> copy{original};

  EXPECT_EQ(copy.size(), original.size());
  EXPECT_EQ(copy.capacity(), original.capacity());
  for (int i = 0; i < 15; ++i) {
    EXPECT_EQ(copy[i], original[i]);
  }

  original[5] = "modified";
  EXPECT_EQ(copy[5], "item5");
}

// Test using a Move-Only Type (Ensures NO copies happen)
TEST(VectorTestConstructor, MoveConstructorMoveOnlyTypes) {
  cv::vector<std::unique_ptr<int>> source;
  source.push_back(std::make_unique<int>(42));
  source.push_back(std::make_unique<int>(100));

  // Act - This will refuse to compile if your vector tries to copy elements!
  cv::vector<std::unique_ptr<int>> moved{std::move(source)};

  // Assert target has stolen ownership
  EXPECT_EQ(moved.size(), 2);
  EXPECT_EQ(moved.capacity(), 10);
  EXPECT_EQ(*moved[0], 42);
  EXPECT_EQ(*moved[1], 100);

  // Assert source was cleanly emptied out by your initializer list/swap setup
  EXPECT_EQ(source.size(), 0);
}

// Track Custom Object Copies to confirm absolute O(1) performance
struct CopyCounter {
  static int copy_count;
  CopyCounter() = default;
  CopyCounter(const CopyCounter &) { copy_count++; } // Track copies
  CopyCounter(CopyCounter &&) noexcept {} // Move constructor does nothing
};
int CopyCounter::copy_count = 0;

TEST(VectorTestConstructor, MoveConstructorDoesNotCopyElements) {
  cv::vector<CopyCounter> source;
  source.push_back(CopyCounter());
  source.push_back(CopyCounter());

  CopyCounter::copy_count = 0; // Reset counter right before move definition

  // Act
  cv::vector<CopyCounter> moved{std::move(source)};

  // Assert: Absolutely 0 copies must have taken place during the vector move!
  EXPECT_EQ(CopyCounter::copy_count, 0)
      << "Error: Elements were copied during vector move constructor!";
  EXPECT_EQ(source.size(), 0);
  EXPECT_EQ(moved.size(), 2);
}

// Verify that move assignment safely transfers data and clears the source
TEST(VectorTestAssignment, MoveAssignmentTransfersDataAndClearsSource) {
  cv::vector<std::string> source;
  source.push_back("Move");
  source.push_back("Assignment");

  cv::vector<std::string> destination;
  destination.push_back("Old Data Stuff"); // Existing data to be overwritten

  // Act - Triggers move assignment via pass-by-value + move constructor route
  destination = std::move(source);

  // Assert destination successfully stole the contents
  EXPECT_EQ(destination.size(), 2);
  EXPECT_EQ(destination.capacity(),
            10); // Original capacity should remain intact
  EXPECT_EQ(destination[0], "Move");
  EXPECT_EQ(destination[1], "Assignment");

  // Assert source was safely grounded to an empty state by your inner swap
  EXPECT_EQ(source.size(), 0);
}

// Verify that move assignment does not trigger expensive element deep copies
TEST(VectorTestAssignment, MoveAssignmentDoesNotCopyElements) {
  cv::vector<CopyCounter> source;
  source.push_back(CopyCounter());
  source.push_back(CopyCounter());

  cv::vector<CopyCounter> destination;
  destination.push_back(CopyCounter());

  // Reset your existing tracking counter right before assignment operation
  CopyCounter::copy_count = 0;

  // Act
  destination = std::move(source);

  // Assert: Absolutely NO copy constructors should run for the elements
  EXPECT_EQ(CopyCounter::copy_count, 0)
      << "Error: Elements were copied during move assignment!";
  EXPECT_EQ(source.size(), 0);
  EXPECT_EQ(destination.size(), 2);
  EXPECT_EQ(destination.capacity(), 10); // Capacity should remain unchanged
}

// Helper function to simulate passing your vector by const reference
void VerifyConstAccess(const cv::vector<std::string> &const_v) {
  // 1. Verify const method empty() compiles and works here
  EXPECT_FALSE(const_v.empty());

  // 2. Verify const operator[] compiles and allows reading
  EXPECT_EQ(const_v[0], "Const");
  EXPECT_EQ(const_v[1], "Safe");

  // Note: Writing const_v[0] = "Change"; here would trigger a compilation
  // error, which is exactly the read-only security safety net we want!
}

// Test empty() states
TEST(VectorTestCapacity, EmptyMethodBehavior) {
  cv::vector<int> v;

  // A freshly created vector must be empty
  EXPECT_TRUE(v.empty());

  v.push_back(42);

  // After adding an element, it must not be empty
  EXPECT_FALSE(v.empty());
}

// 2. Test const bracket operator routing
TEST(VectorTestAccess, ConstBracketOperator) {
  cv::vector<std::string> v;
  v.push_back("Const");
  v.push_back("Safe");

  // Pass it into our const reference helper to validate the const routes
  VerifyConstAccess(v);
}

// 1. Core Test: Verify basic size tracking decrements
TEST(VectorTestModifiers, PopBackDecrementsSize) {
  cv::vector<int> v;
  v.push_back(10);
  v.push_back(20);
  v.push_back(30);

  EXPECT_EQ(v.size(), 3u);

  // Pop the trailing item (30)
  v.pop_back();

  EXPECT_EQ(v.size(), 2u);
  EXPECT_EQ(v[0], 10);
  EXPECT_EQ(v[1], 20);
}

// 2. Edge Case: Verify popping an empty vector does not crash
TEST(VectorTestModifiers, PopBackOnEmptyVectorIsSafe) {
  cv::vector<int> v;

  // Calling pop_back on a vector with size 0 should do nothing safely
  EXPECT_NO_THROW(v.pop_back());
  EXPECT_EQ(v.size(), 0u);
}

// 3. Lifetime Test: Verify object destructors are explicitly triggered
struct DestructorCounter {
  static int destroy_count;
  DestructorCounter() = default;
  ~DestructorCounter() { destroy_count++; }
};
int DestructorCounter::destroy_count = 0;

TEST(VectorTestModifiers, PopBackTriggersElementDestructor) {
  cv::vector<DestructorCounter> v;
  v.push_back(DestructorCounter());
  v.push_back(DestructorCounter());

  // Reset tracking counter after push_back copies/moves are finished
  DestructorCounter::destroy_count = 0;

  // Act
  v.pop_back();

  // Assert: Exactly one element should have been explicitly destroyed
  EXPECT_EQ(DestructorCounter::destroy_count, 1);
  EXPECT_EQ(v.size(), 1u);
}

// Core Test: Verify capacity expands but size and data remain identical
TEST(VectorTestCapacity, ReserveExpandsCapacityLeavesSizeUntouched) {
  cv::vector<int> v;
  v.push_back(10);
  v.push_back(20);

  size_t original_size = v.size();

  // Act - Manually bump capacity to 50
  v.reserve(50);

  // Assert capacity updated but size stayed exactly the same
  EXPECT_EQ(v.capacity(), 50u);
  EXPECT_EQ(v.size(), original_size);

  // Assert actual element values safely survived the migration
  EXPECT_EQ(v[0], 10);
  EXPECT_EQ(v[1], 20);
}

// Boundary Test: Verify requesting smaller capacity does nothing
TEST(VectorTestCapacity, ReserveSmallerCapacityIsIgnored) {
  cv::vector<int> v;
  v.push_back(10); // Initial capacity is 10

  size_t original_capacity = v.capacity();

  // Act - Request a shrink down to 5
  v.reserve(5);

  // Assert capacity did not change or shrink
  EXPECT_EQ(v.capacity(), original_capacity);
}

// Regression Test: Verify self-insertion works perfectly during reallocation
TEST(VectorTestCapacity,
     PushBackSelfReferentialElementTriggersSafeReallocation) {
  cv::vector<std::string> v;

  // Fill the vector completely up to its initial default capacity of 10
  for (int i = 0; i < 10; ++i) {
    v.push_back("item_" + std::to_string(i));
  }

  // Sanity check: Vector must be perfectly full
  ASSERT_EQ(v.size(), v.capacity());

  // Act - Push the first element (index 0) into itself.
  // This triggers a full capacity reallocation while passing a reference to its
  // own block!
  v.push_back(v[0]);

  // Assert size incremented to 11
  EXPECT_EQ(v.size(), 11u);

  // Assert the 11th element is a perfect duplicate copy of the 1st element
  EXPECT_EQ(v[10], "item_0");
}

// Test the Growth Branch (Constructing new elements)
TEST(VectorTestCapacity, ResizeGrowsAndDefaultConstructs) {
  cv::vector<int> v;
  v.push_back(10);
  v.push_back(20);

  // Act - Resize to a larger dimensions (uses default int value of 0)
  v.resize(5);

  // Assert size updated correctly
  EXPECT_EQ(v.size(), 5u);
  EXPECT_GE(v.capacity(), 10u);

  // Assert original elements are untouched and new ones are default-initialized
  EXPECT_EQ(v[0], 10);
  EXPECT_EQ(v[1], 20);
  EXPECT_EQ(v[2], 0);
  EXPECT_EQ(v[3], 0);
  EXPECT_EQ(v[4], 0);
}

// Test the Growth Branch with a Custom Padding Value
TEST(VectorTestCapacity, ResizeGrowsWithCustomValue) {
  cv::vector<std::string> v;
  v.push_back("A");

  // Act - Resize up using "B" as the pad value
  v.resize(3, "B");

  // Assert
  EXPECT_EQ(v.size(), 3u);
  EXPECT_EQ(v[0], "A");
  EXPECT_EQ(v[1], "B");
  EXPECT_EQ(v[2], "B");
}

// Test the Shrink Branch (Ensuring elements are destroyed but capacity remains)
struct ResizeDestructorCounter {
  static int destroy_count;
  ResizeDestructorCounter() = default;
  ~ResizeDestructorCounter() { destroy_count++; }
};
int ResizeDestructorCounter::destroy_count = 0;

TEST(VectorTestCapacity, ResizeShrinksAndDestroysElements) {
  cv::vector<ResizeDestructorCounter> v;
  v.push_back(ResizeDestructorCounter());
  v.push_back(ResizeDestructorCounter());
  v.push_back(ResizeDestructorCounter());
  EXPECT_EQ(v.size(), 3u);
  size_t original_capacity = v.capacity();
  ResizeDestructorCounter pad_val;            // Keep alive on local frame
  ResizeDestructorCounter::destroy_count = 0; // Reset tracking flag

  // Act - Shrink size from 3 down to 1
  v.resize(1, pad_val);

  // Assert size shrank but capacity did not drop
  EXPECT_EQ(v.size(), 1u);
  EXPECT_EQ(v.capacity(), original_capacity);

  // Assert exactly 2 elements were explicitly destroyed
  EXPECT_EQ(ResizeDestructorCounter::destroy_count, 2);
}

// Helper function to force a const reference environment
void VerifyConstAccessors(const cv::vector<int> &const_v) {
  // Verify read-only const front() and back() compile and match
  EXPECT_EQ(const_v.front(), 10);
  EXPECT_EQ(const_v.back(), 30);
}

TEST(VectorTestAccess, FrontAndBackAccessors) {
  cv::vector<int> v;
  v.push_back(10);
  v.push_back(20);
  v.push_back(30);

  // 1. Test basic element lookup
  EXPECT_EQ(v.front(), 10);
  EXPECT_EQ(v.back(), 30);

  // 2. Test mutable editing capabilities via references
  v.front() = 99;
  v.back() = 88;

  EXPECT_EQ(v[0], 99);
  EXPECT_EQ(v[2], 88);

  // 3. Test const access routing safety
  cv::vector<int> const_tester;
  const_tester.push_back(10);
  const_tester.push_back(20);
  const_tester.push_back(30);
  VerifyConstAccessors(const_tester);
}
