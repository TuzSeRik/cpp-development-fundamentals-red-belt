#include <algorithm>

#include "test_runner.h"

using namespace std;
// We can use a little hack to swap values without tmp variable but who cares?
template <typename T>
void Swap(T* first, T* second) {
    auto tmp = *first;
    *first = *second;
    *second = tmp;
}
// Sort by value pointed
template <typename T>
void SortPointers(vector<T*>& pointers) {
    sort(begin(pointers), end(pointers), [](auto lhs, auto rhs) -> bool {
        return *lhs < *rhs;
    });
}
// Here I started to like C++ less. I hope pointers and gamedev do not intersect
template <typename T>
void ReversedCopy(T* source, size_t count, T* destination) {
    // If block do not intersect, using standard algorithm
    if ((destination + count <= source) || (source + count <= destination)) {
        reverse_copy(source, source + count, destination);
    }
    else {
        auto leftBorder = destination, rightBorder = destination + count;
        // If right part of destination block is affected by source block,
        // Copy all unaffected source values in reverse order and set left border of reversing to source start
        if (destination < source) {
            for (size_t i = 0, j = count - 1 ; j >= destination + count - source; ++i, --j) {
                destination[i] = source[j];
            }
            leftBorder = source;
        }
        // If left part of destination block is affected by source block,
        // Copy all unaffected source values in reverse order and set right border of reversing to source end
        if (source < destination) {
            for (size_t i = 0, j = count - 1; i < destination - source; ++i, --j) {
                destination[j] = source[i];
            }
            rightBorder = source + count;
        }
        // Reversing selected block. For equal block default values used
        reverse(leftBorder, rightBorder);
    }
}
// Tests, provided by authors and some by me
void TestSwap() {
    int a = 1;
    int b = 2;
    Swap(&a, &b);
    ASSERT_EQUAL(a, 2)
    ASSERT_EQUAL(b, 1)

    string h = "world";
    string w = "hello";
    Swap(&h, &w);
    ASSERT_EQUAL(h, "hello")
    ASSERT_EQUAL(w, "world")
}

void TestSortPointers() {
    int one = 1;
    int two = 2;
    int three = 3;

    vector<int*> pointers;
    pointers.push_back(&two);
    pointers.push_back(&three);
    pointers.push_back(&one);

    SortPointers(pointers);

    ASSERT_EQUAL(pointers.size(), 3u)
    ASSERT_EQUAL(*pointers[0], 1)
    ASSERT_EQUAL(*pointers[1], 2)
    ASSERT_EQUAL(*pointers[2], 3)
}

void TestReverseCopy() {
    const size_t count = 7;

    auto* source = new size_t[count];
    auto* dest = new size_t[count];
    auto* dest2 = new size_t[count + 1];

    for (size_t i = 0; i < count; ++i) {
        source[i] = i + 1;
        dest2[i] = i + 1;
    }
    dest2[count] = 8;

    ReversedCopy(source, count, dest);
    const vector<int> expected1 = {7, 6, 5, 4, 3, 2, 1};
    ASSERT_EQUAL(vector<int>(dest, dest + count), expected1)

    ReversedCopy(source, count - 1, source + 1);
    const vector<int> expected2 = {1, 6, 5, 4, 3, 2, 1};
    ASSERT_EQUAL(vector<int>(source, source + count), expected2)
    // Testing destination block before source and different size of block simultaneously
    ReversedCopy(dest2 + 1, count, dest2);
    const vector<int> expected3 = {8, 7, 6, 5, 4, 3, 2, 8};
    ASSERT_EQUAL(vector<int>(dest2, dest2 + count + 1), expected3)
    // Test for the same blocks
    ReversedCopy(dest, count, dest);
    const vector<int> expected4 = {1, 2, 3, 4, 5, 6, 7};
    ASSERT_EQUAL(vector<int>(dest, dest + count), expected4)

    delete[] dest;
    delete[] source;
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSwap);
    RUN_TEST(tr, TestSortPointers);
    RUN_TEST(tr, TestReverseCopy);
    return 0;
}
