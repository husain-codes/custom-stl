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
} // namespace my_algo
