#include <unordered_map>
#include <string_view>
#include <deque>
#include <string>

#include "test_runner.h"

using namespace std;

class Translator {
public:
    void Add(string_view source, string_view target) {
        // Looking for words in maps
        const auto sourcePosition = oneToTwo.find(source),
                targetPosition = twoToOne.find(target);
        // If no words found, adding words to queue and inserting found entries in maps
        if (sourcePosition == end(oneToTwo) && targetPosition == end(twoToOne)) {
            realStorage.push_front(string(source));
            realStorage.push_front(string(target));

            oneToTwo[*next(begin(realStorage))] = *begin(realStorage);
            twoToOne[*begin(realStorage)] = *next(begin(realStorage));
        }
            // If found only one, save not founded word and update entries in maps
        else if (sourcePosition == end(oneToTwo)) {
            realStorage.push_front(string(source));

            oneToTwo[*begin(realStorage)] = targetPosition->first;
            twoToOne[target] = *begin(realStorage);
        }
        else if (targetPosition == end(twoToOne)) {
            realStorage.push_front(string(target));

            oneToTwo[source] = *begin(realStorage);
            twoToOne[*begin(realStorage)] = sourcePosition->first;
        }
    }
    // Return word or empty string
    [[nodiscard]] string_view TranslateForward(string_view source) const {
        try {
            return oneToTwo.at(source);
        }
        catch (out_of_range&) {
            return "";
        }
    }

    [[nodiscard]] string_view TranslateBackward(string_view target) const {
        try {
            return twoToOne.at(target);
        }
        catch (out_of_range&) {
            return "";
        }
    }

private:
    unordered_map<string_view, string_view> oneToTwo, twoToOne;
    // Saving real string here, to prevent views from invalidating
    deque<string> realStorage;
};
// Tests, provided by authors
void TestSimple() {
    Translator translator;
    translator.Add(string("okno"), string("window"));
    translator.Add(string("stol"), string("table"));

    ASSERT_EQUAL(translator.TranslateForward("okno"), "window")
    ASSERT_EQUAL(translator.TranslateBackward("table"), "stol")
    ASSERT_EQUAL(translator.TranslateBackward("stol"), "")
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSimple);
    return 0;
}
