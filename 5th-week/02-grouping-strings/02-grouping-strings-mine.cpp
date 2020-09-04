#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <set>

#include "test_runner.h"

using namespace std;

template <typename String>
using Group = vector<String>;

template <typename String>
using Char = typename String::value_type;

template <typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
    map<set<Char<String>>, Group<String>> groups;
    vector<Group<String>> result;

    for (auto& str : strings)
        groups[{begin(str), end(str)}].push_back(move(str));

    result.reserve(groups.size());
    for (auto& [key, group] : groups)
        result.push_back(move(group));

    return result;
}
// Tests, provided by authors
void TestGroupingABC() {
    vector<string> strings = {"caab", "abc", "cccc", "bacc", "c"};
    auto groups = GroupHeavyStrings(strings);
    ASSERT_EQUAL(groups.size(), 2)
    sort(begin(groups), end(groups));
    ASSERT_EQUAL(groups[0], vector<string>({"caab", "abc", "bacc"}))
    ASSERT_EQUAL(groups[1], vector<string>({"cccc", "c"}))
}

void TestGroupingReal() {
    vector<string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
    auto groups = GroupHeavyStrings(strings);
    ASSERT_EQUAL(groups.size(), 4)
    sort(begin(groups), end(groups));
    ASSERT_EQUAL(groups[0], vector<string>({"laptop", "paloalto"}))
    ASSERT_EQUAL(groups[1], vector<string>({"law", "wall", "awl"}))
    ASSERT_EQUAL(groups[2], vector<string>({"port"}))
    ASSERT_EQUAL(groups[3], vector<string>({"top", "pot"}))
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestGroupingABC);
    RUN_TEST(tr, TestGroupingReal);
    return 0;
}
