#include <algorithm>
#include <iostream>
#include <utility>
#include <deque>

#include "test_runner.h"

using namespace std;

template <typename T>
class PriorityCollection {
public:
    // Use number as id, because it is the simplest option
    using Id = size_t;
    // We'll use set<pair<Priority, Id> to have auto-sorted pointer to most prioritised latest added object
    // and vector to have constant access to any element by id
    Id Add(T object) {
        sortedIndexes.emplace(0, nextId);
        collectionData.emplace_back(move(object), 0);
        return Id(nextId++);
    }

    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
        while (range_begin != range_end)
            *ids_begin++ = Add(move(*range_begin++));
    }
    // Because we can't delete elements and not change order of id's, we'll just nullify object and set it priority to -1
    [[nodiscard]] bool IsValid(Id id) const {
        try {
            return collectionData[id].second >= 0;
        }
            // But if there wasn't any element at all, we'll just get an exception
        catch (out_of_range&) {
            return false;
        }
    }

    [[nodiscard]] const T& Get(Id id) const {
        return collectionData[id].first;
    }
    // Set key is immutable, so we need to re-insert updated pair to a set and just update values in vector
    void Promote(Id id) {
        auto newPriority = collectionData[id].second++, oldPriority = newPriority++;
        sortedIndexes.erase({oldPriority, id});
        sortedIndexes.insert({newPriority, id});
    }
    // We need element with biggest priority and id -> latest element in set
    [[nodiscard]] pair<const T&, int> GetMax() const {
        return collectionData[prev(end(sortedIndexes))->second];
    }

    pair<T, int> PopMax() {
        auto id = prev(end(sortedIndexes))->second;
        auto result = move(collectionData[id]);
        sortedIndexes.erase({result.second, id});
        collectionData[id] = {T(), -1};
        return result;
    }
// Theoretically, we could increase efficiency of program by using map<T, int> instead of vector
// and vector with map's iterators instead of set, so we could make Promote complexity constant instead of 2logN,
// but GetMax and PopMax will become N complexity instead of constant and logN, so it could be useful if
// there are much more Promote calls, that GetMax and PopMax, but this realisation is more universal.
private:
    uint nextId = 0;
    set<pair<int, Id>> sortedIndexes;
    vector<pair<T, int>> collectionData;
};
// Tests, provided by authors
class StringNonCopyable : public string {
public:
    using string::string;
    StringNonCopyable(const StringNonCopyable&) = delete;
    StringNonCopyable(StringNonCopyable&&) = default;
    StringNonCopyable& operator=(const StringNonCopyable&) = delete;
    StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
    PriorityCollection<StringNonCopyable> strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");

    strings.Promote(yellow_id);
    for (int i = 0; i < 2; ++i) {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red")
        ASSERT_EQUAL(item.second, 2)
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow")
        ASSERT_EQUAL(item.second, 2)
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white")
        ASSERT_EQUAL(item.second, 0)
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    return 0;
}
