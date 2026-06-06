#include <cstdio>
#include <functional>
#include <utility>

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
  vector(vector &&obj) noexcept : size_(0), capacity_(0), data_(nullptr) {
    swap(*this, obj);
  }

  friend void swap(vector &a, vector &b) noexcept {
    using std::swap;
    swap(a.size_, b.size_);
    swap(a.capacity_, b.capacity_);
    swap(a.data_, b.data_);
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
  //      This instantly steals 'v2''s pointers in O(1) speed and sets 'v2' to
  //      null.
  //    - 'swap(*this, obj)' passes our old data over to 'obj' and takes the
  //    stolen data.
  //    - Upon function exit, 'obj' dies and frees our old data seamlessly.
  //
  // EXCEPTION SAFETY: If any allocation fails while constructing the value
  // parameter 'obj', it happens BEFORE entering this function. Your current
  // vector's state is never corrupted, providing Strong Exception Safety.
  // ==========================================
  vector &operator=(vector obj) {
    swap(*this, obj);
    return *this;
  }

  T &operator[](const size_t i) { return data_[i]; }
  const T &operator[](const size_t i) const { return data_[i]; }

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
  void pop_back() {
    if (!empty())
      data_[--size_].~T();
  }
  void reserve(const size_t val) {
    if (val > capacity_) {
      reallocate(val);
    }
  }

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
    // 1. Store the value on the local stack frame FIRST.
    // If new_val belonged to data_, it is safely copied/moved out right here.
    T local_val(std::forward<Arg>(new_val));
    // 2. Now it is completely safe to allocate and delete the old data_ block!
    reallocate(new_capacity);
    // 3. Construct the brand-new incoming element FIRST into the expanded
    // chunk.
    new (&data_[size_++]) T(std::move(local_val));
  }

  // Helper to handle reallocation cleanly without duplication
  void reallocate(size_t new_capacity) {
    T *new_data = static_cast<T *>(::operator new(new_capacity * sizeof(T)));
    for (size_t i = 0; i < size_; i++) {
      new (&new_data[i])
          T(std::move(data_[i])); // Move-construct into new space
      data_[i].~T();              // Explicitly destroy old object
    }
    ::operator delete(data_);
    data_ = new_data;
    capacity_ = new_capacity;
  }
};
} // namespace cv
