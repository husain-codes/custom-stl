#include <custom_algorithm.h>
#include <iostream>
#include <vector>

template <typename iterator> void print_vector(iterator begin, iterator end) {
  for (auto it = begin; it != end; ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
}

int main() {
  std::cout << "Enter the array elements with space in between and to end "
               "array type q\n";
  double input;
  std::vector<double> v;
  while (std::cin >> input) {
    v.push_back(input);
  }

  auto compare = [](double a, double b) { return a > b; };

  my_algo::bubble_sort(v.begin(), v.end(), compare);
  print_vector(v.begin(), v.end());
  return 0;
}