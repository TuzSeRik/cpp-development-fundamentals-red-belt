#pragma once

#include <chrono>
#include <iostream>
#include <string>
// This is a profiler, created during the reb belt course
using namespace std;
using namespace std::chrono;

class LogDuration {
public:
    explicit LogDuration(const string& msg = "")
            : message(msg + ": ")
            , start(steady_clock::now())
    {
    }

    ~LogDuration() {
        auto finish = steady_clock::now();
        auto dur = finish - start;
        cerr << message
             << duration_cast<milliseconds>(dur).count()
             << " ms" << endl;
    }
private:
    string message;
    steady_clock::time_point start;
};

#define UNIQ_ID_IMPL(counterno) _a_local_var_##counterno
#define UNIQ_ID(counterno) UNIQ_ID_IMPL(counterno)

#define LOG_DURATION(message) \
  LogDuration UNIQ_ID(__COUNTER__){message};
