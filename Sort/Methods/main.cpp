#include <numeric>
#include <list>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "selection_sort.h"
#include "insertion_sort.h"
#include "shell_sort.h"
#include "merge_sort.h"


namespace {
  constexpr int SIZE = {8096};
}


TEST(TestSelectionSort, SortedStdArray) {
  using Array = std::array<int, SIZE>;
  Array expected;
  std::iota(expected.begin(), expected.end(), 0);
  Array src = expected;

  algo::sort::selection_sort(src.begin(), src.end());

  ASSERT_EQ(src, expected);
}

TEST(TestSelectionSort, UnSortedMultipleVectors) {
  for(std::size_t size = {1}; size < SIZE; size *= 2) {
    std::vector<int> expected(size);
    std::iota(expected.begin(), expected.end(), 0);
    std::vector<int> src(expected);
    std::random_shuffle(src.begin(), src.end());

    algo::sort::selection_sort(src.begin(), src.end());

    ASSERT_EQ(src, expected);
  }
}


TEST(TestInsertionSort, UnSortedMultipleVectors) {
  for(std::size_t size = {1}; size < SIZE; size *= 2) {
    std::vector<int> expected(size);
    std::iota(expected.begin(), expected.end(), 0);
    std::vector<int> src(expected);
    std::random_shuffle(src.begin(), src.end());

    algo::sort::insertion_sort(src.begin(), src.end());

    ASSERT_EQ(src, expected);
  }
}


TEST(TestShellSort, UnSortedMultipleVectors) {
  for(std::size_t size = {1}; size < SIZE; size *= 2) {
    std::vector<int> expected(size);
    std::iota(expected.begin(), expected.end(), 0);
    std::vector<int> src(expected);
    std::random_shuffle(src.begin(), src.end());

    algo::sort::shell_sort(src.begin(), src.end());

    ASSERT_EQ(src, expected);
  }
}


TEST(TestMergeSort, UnSortedMultipleVectors) {
  for(std::size_t size = {1}; size < SIZE; size *= 2) {
    std::vector<int> expected(size);
    std::iota(expected.begin(), expected.end(), 0);
    std::vector<int> src(expected);
    std::random_shuffle(src.begin(), src.end());

    algo::sort::merge_sort(src.begin(), src.end());

    ASSERT_EQ(src, expected);
  }
}

TEST(TestMergeSort, UnSortedMultipleList) {
  for(std::size_t size = {1}; size < SIZE; size *= 2) {
    std::list<int> expected(size);
    std::iota(expected.begin(), expected.end(), 0);
    std::list<int> src(expected);
    std::iota(src.begin(), src.end(), 0);


    algo::sort::merge_sort(src.begin(), src.end());

    ASSERT_EQ(src, expected);
  }
}


int main(int argc, char** argv) {

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}