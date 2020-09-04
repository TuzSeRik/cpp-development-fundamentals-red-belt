#include <sstream>
#include <iterator>
#include <algorithm>
#include <numeric>

#include "search_server.h"
#include "iterator_range.h"

vector<string> SplitIntoWords(const string& line) {
    istringstream words_input(line);
    return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
    // Do not forget to reset storage while updating it
    parsedDocuments.clear();
    numberOfDocuments = 0;
    // For every document
    for (string document; getline(document_input, document); numberOfDocuments++) {
        auto words = SplitIntoWords(document);
        // For every word
        for (auto& word : words) {
            auto& documents = parsedDocuments[word];
            // Checking if it's already added and increment it or add it if not
            if (!documents.empty() && documents.back().first == numberOfDocuments)
                documents.back().second++;
            else
                documents.emplace_back(numberOfDocuments, 1);
        }
    }
}
// Main "search" function
void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
    // For every query
    for (string query; getline(query_input, query);) {
        // Storing number of words, located both in document and query
        vector<uint> summedUpCount(numberOfDocuments, 0);
        // Storing indexes of previous sums
        vector<size_t> idOfSummedUp(numberOfDocuments);
        iota(begin(idOfSummedUp), end(idOfSummedUp), 0);
        auto words = SplitIntoWords(query);
        // For every word
        for (auto& word : words)
            // Adding number of word entry in documents to a related variables in vector
            for (auto& [id, count] :
            (parsedDocuments.find(word) != end(parsedDocuments) ?
            parsedDocuments.find(word)->second : vector<pair<size_t, uint>>()))
                summedUpCount[id] += count;
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
