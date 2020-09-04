#include <vector>
// Unit test framework code provided in repo root
#include "test_runner.h"

using namespace std;

template<typename T>
class Table {
public:
    Table(const size_t& a, const size_t& b) {
        table.resize(a, vector<T>(b));
    }
    // Second bracket operator is already defined for vector
    vector<T>& operator [](const size_t& a) {
        return table[a];
    }

    vector<T>& operator [](const size_t& a) const {
        return table[a];
    }

    void Resize(const size_t& a, const size_t& b) {
        table.resize(a);
        // For needed to resize already created vectors
        for (auto& row : table) {
            row.resize(b);
        }
    }
    // To prevent segfault, check outer vector first
    [[nodiscard]] pair<size_t, size_t> Size() const {
        if (table.empty() || table.back().empty())
            return make_pair(0, 0);

        return make_pair(table.size(), table.back().size());
    }
// Storing table as vector in vector
private:
    vector<vector<T>> table;
};
// Tests, provided by authors
void TestTable() {
    Table<int> t(1, 1);
    ASSERT_EQUAL(t.Size().first, 1u)
    ASSERT_EQUAL(t.Size().second, 1u)
    t[0][0] = 42;
    ASSERT_EQUAL(t[0][0], 42)
    t.Resize(3, 4);
    ASSERT_EQUAL(t.Size().first, 3u)
    ASSERT_EQUAL(t.Size().second, 4u)
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestTable);
    return 0;
}
