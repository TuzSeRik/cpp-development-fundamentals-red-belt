#include <sstream>
#include <string>
// Unit test framework code provided in repo root
#include "test_runner.h"

using namespace std;

class Logger {
public:
    explicit Logger(ostream& output_stream) : os(output_stream) {}

    void SetLogLine(bool value) { log_line = value; }

    void SetLogFile(bool value) { log_file= value; }
    // I tried to store this code in macro, but failed - this feature of language seems to be overcomplicated, so
    // it breaks even with such simple code
    void Log(const string& message) {
        if (log_file)
            (log_line) ?
            (os << file + ":" + line + " " + message) : (os << file + " " + message);
        else
            (log_line) ?
            (os << "Line " + line + " " + message) : ( os << message);

        os << endl;
    }
    // Adding setters and variables to store file and line parameters
    void setFile(const string& arg) {
        file = arg;
    }

    void setLine(const string& arg) {
        line = arg;
    }

private:
    ostream& os;
    bool log_line = false, log_file = false;
    string file, line;
};
// Because of all of the macros problems,
// reducing all their logic to minimal - just provide __FILE__ and __LINE__ to logger
#define LOG(logger, message)            \
    logger.setFile(__FILE__);           \
    logger.setLine(to_string(__LINE__));\
    logger.Log(message);
// Tests, provided by authors
void TestLog() {
#line 1 "logger.cpp"

    ostringstream logs;
    Logger l(logs);
    LOG(l, "hello")

    l.SetLogFile(true);
    LOG(l, "hello")

    l.SetLogLine(true);
    LOG(l, "hello")

    l.SetLogFile(false);
    LOG(l, "hello")

    string expected = "hello\n";
    expected += "logger.cpp hello\n";
    expected += "logger.cpp:10 hello\n";
    expected += "Line 13 hello\n";
    ASSERT_EQUAL(logs.str(), expected)
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLog);
}
