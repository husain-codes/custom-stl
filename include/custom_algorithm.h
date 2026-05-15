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
  vector() { data_ = new T[capacity_]; }

  T &operator[](const size_t i) { return data_[i]; }

  void push_back(const T &val) {
    if (size_ < capacity_ )
      data_[size_++] = val;
    else {
      capacity_ *= 2;
      T *new_data = new T[capacity_];
      for (size_t i = 0; i < size_; i++) {
        new_data[i] = std::move(data_[i]);
      }
      delete[] data_;
      data_ = new_data;
      data_[size_++] = val;
    }
  }

  void push_back(T &&val) {
    if (size_ < capacity_)
      data_[size_++] = std::move(val);
    else {
      capacity_ *= 2;
      T *new_data = new T[capacity_];
      for (size_t i = 0; i < size_; i++) {
        new_data[i] = std::move(data_[i]);
      }
      delete[] data_;
      data_ = new_data;
      data_[size_++] = std::move(val);
    }
  }

  size_t size() const { return size_; }
  ~vector() { delete[] data_; }
private:
  size_t size_ = 0;
  size_t capacity_ = 10;
  T *data_ = nullptr;
};
} // namespace cv
