#include <map>
#include <string>
#include <future>
#include <functional>
#include <thread>

#include "test_runner.h"
#include "profile.h"

using namespace std;
// This structure needed to simplify summarising of thread's results
struct Stats {
    map<string, int> word_frequences;

    void operator += (const Stats& other) {
        for(const auto& word : other.word_frequences)
            this->word_frequences[word.first] += word.second;
    }
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
    string_view lineView = line;
    Stats middleResult;
    // Trimming line
    auto pos = lineView.find_first_not_of(" ");
    lineView.remove_prefix(pos);

    while (pos != string_view::npos) {
        // Getting word from line
        pos = lineView.find_first_of(" ");
        auto key = string(lineView.substr(0, pos));
        // Looking for word in keywords and incrementing counter if found
        if (key_words.count(key) > 0)
            middleResult.word_frequences[move(key)]++;
        // If line ended - break, else delete founded word with prefix space
        if (pos == std::string::npos)
            break;
        else
            lineView.remove_prefix(pos + 1);
    }

    return middleResult;
}
// Middleware function, splitting given stream into lines
Stats ExploreKeyWordsSingleThread(
        const set<string>& key_words, istream& input
) {
    Stats result;
    for (string line; getline(input, line); ) {
        result += ExploreLine(key_words, line);
    }
    return result;
}
// Main function
Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    auto numberOfThreads = thread::hardware_concurrency();
    vector<stringstream> threadsData(numberOfThreads);
    vector<future<Stats>> threads;
    Stats result;
    // Trying to parallelize incoming data equally to all hardware threads
    int counter = 0;
    string tmp;
    while (getline(input, tmp))
        threadsData[counter++ % numberOfThreads] << tmp << "\n";
    // Starting threads with provided data parts
    threads.reserve(numberOfThreads);
    for (auto& part : threadsData) {
        threads.push_back(async([&] {
            return ExploreKeyWordsSingleThread(key_words, part);
        }));
    }
    // Sum-upping results;
    for (auto& future : threads)
        result += future.get();

    return result;
}
// Tests, provided by authors
void TestBasic() {
    const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    const auto stats = ExploreKeyWords(key_words, ss);
    const map<string, int> expected = {
            {"yangle", 6},
            {"rocks", 2},
            {"sucks", 1}
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestBasic);
}
