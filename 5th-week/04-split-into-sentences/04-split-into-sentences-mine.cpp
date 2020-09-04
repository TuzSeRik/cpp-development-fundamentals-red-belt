#include <vector>
#include <algorithm>

#include "test_runner.h"

using namespace std;

template <typename Token>
using Sentence = vector<Token>;

template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token>&& tokens) {
    vector<Sentence<Token>> result;
    // Emplacing empty vector to prevent copying
    result.emplace_back();
    // Cursor to position we fill in result vector. It would be better
    // if for cycle could initialise variables of different types, but we have what we have
    size_t i = 0;
    for (auto previousToken = begin(tokens), token = next(begin(tokens));
         token != end(tokens);
         previousToken++, token++) {
        result[i].push_back(move(*previousToken));
        // If sentence is over, changing to next sentence to fill
        if (result[i].back().IsEndSentencePunctuation() && !token->IsEndSentencePunctuation()) {
            i++;
            result.emplace_back();
        }
    }
    // Very last letter in source vector. It should be added to last sentence,
    // but not with back(), because const variables are not movable
    result[i].push_back(move(*prev(end(tokens))));

    return result;
}
// Tests, provided by authors
struct TestToken {
    string data;
    bool is_end_sentence_punctuation = false;

    [[nodiscard]] bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }
    bool operator==(const TestToken& other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
    return stream << token.data;
}

void TestSplitting() {
    ASSERT_EQUAL(
            SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!"}})),
            vector<Sentence<TestToken>>({
                                                {{"Split"}, {"into"}, {"sentences"}, {"!"}}
                                        })
    )

    ASSERT_EQUAL(
            SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}})),
            vector<Sentence<TestToken>>({
                                                {{"Split"}, {"into"}, {"sentences"}, {"!", true}}
                                        })
    )

    ASSERT_EQUAL(
            SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true}})),
            vector<Sentence<TestToken>>({
                                                {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
                                                {{"Without"}, {"copies"}, {".", true}},
                                        })
    )
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSplitting);
    return 0;
}
