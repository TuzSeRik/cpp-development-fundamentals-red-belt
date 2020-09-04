#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;
// Interface for our main class
vector<string> SplitIntoWords(const string& line);

class SearchServer {
public:
    SearchServer() = default;

    explicit SearchServer(istream& document_input);

    void UpdateDocumentBase(istream& document_input);

    void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
    size_t numberOfDocuments = 0;
    unordered_map<string, vector<pair<size_t, uint>>> parsedDocuments;
};
