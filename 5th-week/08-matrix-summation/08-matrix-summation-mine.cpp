#include <vector>
#include <future>

#include "test_runner.h"

using namespace std;
// Using paginator template with deleted unused methods from previous task.
template <typename Iterator >
class IteratorRange {
public:
    // Simple container to represent pack of Iterators as one object
    IteratorRange<Iterator>(const Iterator& first, const Iterator& last) : first(first), last(last) {}

    [[nodiscard]] Iterator begin() const {
        return first;
    }

    [[nodiscard]] Iterator end() const {
        return last;
    }

private:
    Iterator first, last;
};

template <typename Iterator>
class Paginator {
public:
    // Main functionality are in constructor - wee need to pack pageSize iterators to one IteratorRange
    // and then store it in storage
    Paginator(Iterator first, Iterator last, size_t pageSize) {
        while (first != last) {
            auto tmp = next(first, min(pageSize, static_cast<size_t>(last - first)));
            storage.push_back({first, tmp});
            first = tmp;
        }
    }

    [[nodiscard]] auto begin() const {
        return storage.begin();
    }

    [[nodiscard]] auto end() const {
        return storage.end();
    }
    // Perfect solution will consist of adding to IteratorRange insert functionality
    // and storing here IteratorRange of IteratorRange<Iterator>
private:
    vector<IteratorRange<Iterator>> storage;
};
// To eliminate unnecessary type printing, use auto and simply return Paginator
template <typename C>
auto Paginate(C& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}
// Task code
int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
    // I would prefer dynamic pageSize, but it cause MemoryLimitExceeded, so for the sake of this task
    // I'll simplify it to 9000 (max row count) \ 4
    size_t pageSize = 2250;
    int64_t results = 0;
    // Storing futures to prevent their deletion
    vector<future<int64_t>> tokens;
    // Push future on every page calculation
    for (auto page : Paginate(matrix, pageSize))
        tokens.push_back(async([=] {
            int64_t middleResult = 0;

            for (auto& row : page)
                for (auto& item : row)
                    middleResult += item;

            return middleResult;
        }));
    // Sum up results
    for (auto& token : tokens)
        results += token.get();

    return results;
}
// Tests, provided by authors
void TestCalculateMatrixSum() {
    const vector<vector<int>> matrix = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 16}
    };
    ASSERT_EQUAL(CalculateMatrixSum(matrix), 136)
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestCalculateMatrixSum);
}
