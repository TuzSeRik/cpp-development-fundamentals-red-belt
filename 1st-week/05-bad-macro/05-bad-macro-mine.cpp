// Unit test framework code provided in repo root
#include "test_runner.h"

using namespace std;
// Main problem - semicolon between statements, so it should be eliminated
#define PRINT_VALUES(out, x, y) (out) << (x) << endl << (y) << endl
// Tests, provided by authors
int main() {
    TestRunner tr;
    tr.RunTest([] {
        ostringstream output;
        PRINT_VALUES(output, 5, "red belt");
        ASSERT_EQUAL(output.str(), "5\nred belt\n");
    }, "PRINT_VALUES usage example");
}
