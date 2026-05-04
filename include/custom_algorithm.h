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
      if (comp(*(it + 1), *it)) {
        std::swap(*(it + 1), *it);
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

} // namespace my_algo