#include <sstream>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <mutex>

#include "search_server.h"
#include "iterator_range.h"

vector<string> SplitIntoWords(const string& line) {
    istringstream words_input(line);
    return {make_move_iterator(istream_iterator<string>(words_input)),
            make_move_iterator(istream_iterator<string>())};
}

SearchServer::SearchServer(istream& document_input) {
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
    // Creating new dataset from given input
    unordered_map<string, vector<pair<size_t, uint>>> newParsedDocuments;
    size_t newNumberOfDocuments = 0;
    // For every document
    for (string document; getline(document_input, document); newNumberOfDocuments++) {
        // For every word
        for (auto& word : SplitIntoWords(document)) {
            auto& documents = newParsedDocuments[move(word)];
            // Checking if it's already added and increment it or add it if not
            if (!documents.empty() && documents.back().first == newNumberOfDocuments)
                documents.back().second++;
            else
                documents.emplace_back(newNumberOfDocuments, 1);
        }
    }
    // Locking access to main dataset and swapping it with newly created new one
    mutex m;
    lock_guard guard(m);
    swap(parsedDocuments, newParsedDocuments);
    swap(numberOfDocuments, newNumberOfDocuments);
}
// Main "search" function - single-thread solution
void AddQueriesStreamSingleThread(istream& query_input, ostream& search_results_output,
                                  unordered_map<string, vector<pair<size_t, uint>>>& parsedDocuments,
                                  size_t numberOfDocuments) {
    // For every query
    for (string query; getline(query_input, query);) {
        // Storing number of words, located both in document and query
        vector<uint> summedUpCount(numberOfDocuments, 0);
        // Storing indexes of previous sums
        vector<size_t> idOfSummedUp(numberOfDocuments);
        iota(begin(idOfSummedUp), end(idOfSummedUp), 0);
        auto words = SplitIntoWords(query);
        // For every word
        for (auto& word : words) {
            // Locking access to dataset for this loop
            mutex m;
            lock_guard guard(m);
            // Adding number of word entry in documents to a related variables in vector
            auto position = parsedDocuments.find(word);
            for (auto&[id, count] :
                    (position != end(parsedDocuments) ?
                     position->second : vector<pair<size_t, uint>>()))
                summedUpCount[id] += count;
        }
        // Then partially sort them, to find five with biggest number (of words both in query and document)
        partial_sort(begin(idOfSummedUp), end(Head(idOfSummedUp, 5)), end(idOfSummedUp),
                     [&summedUpCount](int64_t lhs, int64_t rhs) {
                         return pair(summedUpCount[lhs], -lhs) > pair(summedUpCount[rhs], -rhs);
                     });
        // Formatting result to a output stream including that five biggest numbers and id of related documents
        search_results_output << query << ':';
        for (size_t id : Head(idOfSummedUp, 5)) {
            const uint count = summedUpCount[id];

            if (count == 0) {
                break;
            }

            search_results_output << " {" << "docid: " << id << ", " << "hitcount: " << count << '}';
        }
        search_results_output << '\n';
    }
}
// Main "search" function - multi-thread solution
void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
    futures.push_back(async(AddQueriesStreamSingleThread, ref(query_input), ref(search_results_output),
                            ref(parsedDocuments), numberOfDocuments));
}
