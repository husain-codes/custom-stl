#include <cstdio>
#include <functional>
#include <utility>

namespace my_algo {
// Requires Random Access Iterators
template <typename RandomIt, typename Compare = std::less<>>
void bubble_sort(RandomIt first, RandomIt last, Compare comp = Compare{}) {
  if (first == last)
    return;
  for (RandomIt end = last; end != first; --end) {
    bool swapped = false;
    for (RandomIt it = first; it != end - 1; ++it) {
      if (comp(*std::next(it), *it)) {
        std::swap(*std::next(it), *it);
        swapped = true;
      }
    }
    if (!swapped)
      break;
  }
}

template <typename RandomIt, typename Compare = std::less<>>
void selection_sort(RandomIt first, RandomIt last, Compare comp = Compare{}) {
  if (first == last)
    return;

  for (RandomIt it = first; it != last; it++) {
    RandomIt swapIt = it;
    for (RandomIt j = std::next(it); j != last; j++) {
      if (comp(*j, *swapIt)) {
        swapIt = j;
      }
    }
    if (*swapIt != *it)
      std::swap(*swapIt, *it);
  }
}

template <typename BidirectionalIt, typename Compare = std::less<>>
void insertion_sort(BidirectionalIt first, BidirectionalIt last,
                    Compare comp = Compare{}) {
  if (first == last || std::next(first) == last)
    return;
  for (BidirectionalIt key = std::next(first); key != last; ++key) {
    auto value = *key;
    auto j = key;
    while (j != first && comp(value, *std::prev(j))) {
      *j = *std::prev(j);
      --j;
    }
    *j = value;
  }
}

template <typename InputIt, typename T>
[[nodiscard]] constexpr InputIt
find(InputIt first, InputIt last,
     const T &value) noexcept(noexcept(*first == value)) {
  for (; first != last; ++first) {
    if (*first == value) {
      return first; // Found
    }
  }
  return last; // Not found
}

template <typename BidirectionalIt>
void reverse(BidirectionalIt first, BidirectionalIt last) {
  if (first == last)
    return;

  while (first != last && first != --last) {
    std::swap(*first, *last);
    ++first;
  }
}

template <typename BidirectionalIt>
constexpr void rotate(BidirectionalIt first, BidirectionalIt middle,
                      BidirectionalIt last) {
  if (first == middle || middle == last)
    return;
  my_algo::reverse(first, middle);
  my_algo::reverse(middle, last);
  my_algo::reverse(first, last);
}
} // namespace my_algo

namespace cv {

template <typename T> class vector {
public:
  vector() { data_ = static_cast<T *>(::operator new(capacity_ * sizeof(T))); }

  vector(const vector &obj) {
    size_ = obj.size_;
    capacity_ = obj.capacity_;
    data_ = static_cast<T *>(::operator new(capacity_ * sizeof(T)));
    for (size_t i = 0; i < size_; i++) {
      new (&data_[i]) T(obj.data_[i]);
    }
  }

  // ==========================================
  // MOVE CONSTRUCTOR (Rule of 5 - 4/5)
  // ==========================================
  // Performs a lightning-fast O(1) resource transfer. 
  // It initializes this container to a clean empty state first, 
  // then swaps tracking fields to steal the source object's heap pointers.
  vector(vector &&obj) noexcept:size_(0) , capacity_(0), data_(nullptr) {
    swap(*this , obj);
  }

  friend void swap(vector& a , vector& b) noexcept {
    using std::swap;
    swap(a.size_ , b.size_);
    swap(a.capacity_ , b.capacity_);
    swap(a.data_ , b.data_);
  }

  // ==========================================
  // UNIFIED ASSIGNMENT OPERATOR (Rule of 5 - 5/5)
  // ==========================================
  // This single function utilizes the "Pass-by-Value Copy-and-Swap" idiom.
  // It handles BOTH Copy Assignment and Move Assignment optimally depending
  // on whether the incoming argument is an Lvalue or an Rvalue:
  //
  // 1. CASE A: COPY ASSIGNMENT (v1 = v2;)
  //    - Because 'obj' is passed by value, the compiler automatically invokes
  //      your COPY CONSTRUCTOR to create the parameter 'obj' on the stack.
  //      This creates a completely isolated Deep Copy of the data.
  //    - 'swap(*this, obj)' replaces our current contents with the new data.
  //    - When the function exits, 'obj' goes out of scope and its destructor
  //      automatically destroys our OLD data.
  //
  // 2. CASE B: MOVE ASSIGNMENT (v1 = std::move(v2);)
  //    - Because the argument is cast to an rvalue, the compiler optimizes
  //      initialization by invoking your MOVE CONSTRUCTOR to build 'obj'.
  //      This instantly steals 'v2''s pointers in O(1) speed and sets 'v2' to null.
  //    - 'swap(*this, obj)' passes our old data over to 'obj' and takes the stolen data.
  //    - Upon function exit, 'obj' dies and frees our old data seamlessly.
  //
  // EXCEPTION SAFETY: If any allocation fails while constructing the value 
  // parameter 'obj', it happens BEFORE entering this function. Your current 
  // vector's state is never corrupted, providing Strong Exception Safety.
  // ==========================================
  vector &operator=(vector obj) {
    swap(*this , obj);
    return *this;
  }

  T &operator[](const size_t i) { return data_[i]; }
  const T &operator[](const size_t i) const {return data_[i];}
  void push_back(const T &val) {
    if (size_ >= capacity_) {
      reallocate_and_insert(capacity_ * 2, val);
    } else {
      new (&data_[size_++]) T(val);
    }
  }

  void push_back(T &&val) {
    if (size_ >= capacity_) {
      reallocate_and_insert(capacity_ * 2, std::move(val));
    } else {
      new (&data_[size_++]) T(std::move(val));
    }
  }

  size_t size() const { return size_; }
  size_t capacity() const { return capacity_; }
  bool empty() const { return size_ == 0; }

  ~vector() {
    clear();
    ::operator delete(data_); // Free raw memory
  }

  void clear() {
    for (size_t i = 0; i < size_; i++) {
      data_[i].~T(); // Call explicit destructor
    }
    size_ = 0;
  }

private:
  size_t size_ = 0;
  size_t capacity_ = 10;
  T *data_ = nullptr;
  // Helper helper to handle resizing cleanly without duplication
  template <typename Arg>
  void reallocate_and_insert(size_t new_capacity, Arg &&new_val) {
    capacity_ = new_capacity;
    T *new_data = static_cast<T *>(::operator new(capacity_ * sizeof(T)));
    // Construct the brand-new incoming element FIRST into the expanded chunk.
    new (&new_data[size_]) T(std::forward<Arg>(new_val));
    for (size_t i = 0; i < size_; i++) {
      new (&new_data[i])
          T(std::move(data_[i])); // Move-construct into new space
      data_[i].~T();              // Explicitly destroy old object
    }
    ::operator delete(data_); // Free old raw chunk
    data_ = new_data;
    size_++;
  }
};
} // namespace cv
