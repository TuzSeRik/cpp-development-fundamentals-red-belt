#include <vector>
#include <stdexcept>

using namespace std;

template<typename T>
struct Deque {
public:
    // Mocking two vectors to one container
    [[nodiscard]] bool Empty() const {
        return front.empty() && back.empty();
    }

    [[nodiscard]] size_t Size() const {
        return front.size() + back.size();
    }

    T& operatorBrackets(size_t index) {
        if (index >= front.size())
            return back[index - front.size()];
        else
            return front[front.size() - index - 1];
    }

    T& operator[](size_t index) {
        return operatorBrackets(index);
    }

    const T& operator[](size_t index) const {
        return operatorBrackets(index);
    }

    void SizeExceptions(size_t index) {
        if (index >= front.size() + back.size())
            throw out_of_range("");
    }

    T& At(size_t index) {
        SizeExceptions(index);

        return operatorBrackets(index);
    }

    const T& At(size_t index) const {
        SizeExceptions(index);

        return operatorBrackets(index);
    }
    // I tried to merge this two methods in one but failed
    T& Peek(vector<T>& firstPart, vector<T>& secondPart) {
        if (!firstPart.empty())
            return firstPart.back();
        else
            return secondPart[0];
    }
    // This code repeat makes me cry, but I can't do anything about it
    const T& Peek(const vector<T>& firstPart, const vector<T>& secondPart) const {
        if (!firstPart.empty())
            return firstPart.back();
        else
            return secondPart.at(0);
    }

    T& Back() {
        return Peek(back, front);
    }

    const T& Back() const {
        return Peek(back, front);
    }

    T& Front() {
        return Peek(front, back);
    }

    const T& Front() const {
        return Peek(front, back);
    }
    // I tried to balance deque after push, but then program fails time test
    void PushBack(const T& arg) {
        back.push_back(arg);
    }
    // It may be result of my lack of skill, but it also may be not needed there
    void PushFront(const T& arg) {
        front.push_back(arg);
    }

private:
    // Storing everything pushed from front in front vector and pushed back in back vector
    vector<T> front, back;
};
