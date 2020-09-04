#pragma once

#include <array>

using namespace std;
// This simple wrapper just prohibit to access "non-initialised" fields of array and provide some methods,
// so it's look like vector with hard-coded capacity
template <typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0) {
        if (a_size > N)
            throw invalid_argument("Size cannot be more than a capacity");
        else
            currentSize = a_size;
    }

    T& operator[](size_t index) {
        if (index >= currentSize)
            throw out_of_range("Index out of bounds");
        else return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= currentSize)
            throw out_of_range("Index out of bounds");
        else return data[index];
    }

    typename array< T, N>::iterator begin() {
        return data.begin();
    }

    typename array<T, N>::iterator end() {
        return data.begin() + currentSize;
    }

    [[nodiscard]] typename array<T, N>::const_iterator begin() const {
        return data.begin();
    }

    [[nodiscard]] typename array<T, N>::const_iterator end() const {
        return data.begin() + currentSize;
    }

    [[nodiscard]] size_t Size() const {
        return currentSize;
    }

    [[nodiscard]] size_t Capacity() const {
        return N;
    }

    void PushBack(const T& value) {
        if (currentSize == N)
            throw overflow_error("Container used all of provided memory");
        else
            data[currentSize++] = value;
    }

    T PopBack() {
        if (currentSize == 0)
            throw underflow_error("Container has no more elements");
        else
            return data[--currentSize];
    }

private:
    size_t currentSize;
    array<T, N> data;
};
