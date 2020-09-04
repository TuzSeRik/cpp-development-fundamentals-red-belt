#include "stats.h"

using namespace std;
// Checking if argument is in map
void Stats::AddMethod(string_view method) {
    if (methodStats.find(method) != end(methodStats))
        methodStats.at(method)++;
    else
        methodStats.at("UNKNOWN")++;
}
// If not, increasing 'unknown' field
void Stats::AddUri(string_view uri) {
    if (uriStats.find(uri) != end(uriStats))
        uriStats.at(uri)++;
    else
        uriStats.at("unknown")++;
}

const map<string_view, int>& Stats::GetMethodStats() const {
    return methodStats;
}

const map<string_view, int>& Stats::GetUriStats() const {
    return uriStats;
}

HttpRequest ParseRequest(string_view line) {
    string_view method, uri, protocol;
    // If line starts with a space
    line.remove_prefix(line.find_first_not_of(" "));

    method = line.substr(0, line.find(" "));
    line.remove_prefix(min(line.find(" ") + 1, line.size()));

    uri = line.substr(0, line.find(" "));
    line.remove_prefix(min(line.find(" ") + 1, line.size()));

    protocol = line.substr(0, line.find(" "));

    return {method, uri, protocol};
}
