#pragma once

#include <string_view>
#include <map>
#include <deque>

#include "http_request.h"

using namespace std;

class Stats {
public:
    void AddMethod(string_view method);
    void AddUri(string_view uri);
    [[nodiscard]] const map<string_view, int>& GetMethodStats() const;
    [[nodiscard]] const map<string_view, int>& GetUriStats() const;
// Pre-initialising our containers
private:
    map<string_view, int> methodStats = {{"GET", 0}, {"POST", 0},
                                                   {"PUT", 0}, {"DELETE", 0},
                                                   {"UNKNOWN", 0}},
                                    uriStats = {{"/", 0}, {"/order", 0},
                                                {"/product", 0}, {"/basket", 0},
                                                {"/help", 0}, {"unknown", 0}};
};

HttpRequest ParseRequest(string_view line);
