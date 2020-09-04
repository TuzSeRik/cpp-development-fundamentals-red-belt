#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <unordered_set>

#include "test_runner.h"

using namespace std;

template <class T>
class ObjectPool {
public:
    // If there are elements in freed queue, take element from it and insert in into used set
    T* Allocate() {
        T* tmp;
        if (!freed.empty()) {
            tmp = freed.front();
            freed.pop();
            used.insert(tmp);
            return tmp;
        }
        else {
            tmp = new T;
            used.insert(tmp);
            return tmp;
        }
    }
    // Same as usual allocate, but with exception in else bracket
    T* TryAllocate() {
        T* tmp;
        if (!freed.empty()) {
            tmp = freed.front();
            freed.pop();
            used.insert(tmp);
            return tmp;
        }
        else {
            return nullptr;
        }
    }

    void Deallocate(T* object) {
        auto tmp = used.find(object);
        if (tmp != end(used)) {
            freed.push(*tmp);
            used.erase(tmp);
        }
        else
            throw invalid_argument("");
    }
    // We store pointers in normal containers, so we iterate trough containers to delete stored pointers
    ~ObjectPool() {
        for (auto tmp : used) {
            delete tmp;
        }

        while (!freed.empty()) {
            delete freed.front();
            freed.pop();
        }
    }
// Deallocated object should support FIFO principle
private:
    unordered_set<T*> used;
    queue<T*> freed;
};
// Tests, provided by authors
void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second")

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third")
    ASSERT_EQUAL(*pool.Allocate(), "first")

    pool.Deallocate(p1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}
