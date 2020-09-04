#include <algorithm>
#include <string>
#include <vector>
#include <set>

using namespace std;

class Learner {
private:
    set<string> dict;

public:
    int Learn(const vector<string>& words) {
        int newWords = 0;
        // Changing check from find algorithm to every word to set, which will sort and keep uniqueness of words
        for (const auto& word : words) {
            if (dict.insert(word).second)
                newWords++;
        }

        return newWords;
    }
    // Now just returning vector made from set
    vector<string> KnownWords() {
        return {begin(dict), end(dict)};
    }
};
