#include <algorithm>
#include <memory>
#include <vector>

#include "test_runner.h"

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    // If less than two elements remained - stop
    if (range_end - range_begin < 2)
        return;
    // Create vector, containing current range
    vector<typename RandomIt::value_type> tmp = {make_move_iterator(range_begin),
                                                 make_move_iterator(range_end)};
    // Split range in three and sort. Saving delimiters in int, not iterators
    auto firstDelimiter = (end(tmp) - begin(tmp)) / 3,
            secondDelimiter = 2 * (end(tmp) - begin(tmp)) / 3;
    // Call function on parts iterators
    MergeSort(move(begin(tmp)), move(begin(tmp) + firstDelimiter));
    MergeSort(move(begin(tmp) + firstDelimiter), move(begin(tmp) + secondDelimiter));
    MergeSort(move(begin(tmp) + secondDelimiter), move(end(tmp)));
    // Then merge result into buffet and then into provided range iterators
    vector<typename RandomIt::value_type> tmpToo;
    merge(make_move_iterator(begin(tmp)), make_move_iterator(begin(tmp) + firstDelimiter),
          make_move_iterator(begin(tmp) + firstDelimiter), make_move_iterator(begin(tmp) + secondDelimiter),
          back_inserter(tmpToo));
    merge(make_move_iterator(begin(tmpToo)), make_move_iterator(end(tmpToo)),
          make_move_iterator(begin(tmp) + secondDelimiter), make_move_iterator(end(tmp)),
          range_begin);
}
// Tests, provided by authors
void TestIntVector() {
    vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
    MergeSort(begin(numbers), end(numbers));
    ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);
    return 0;
}
