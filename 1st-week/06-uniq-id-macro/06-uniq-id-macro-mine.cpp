#include <string>
#include <vector>

using namespace std;
// One-word guide to macros: no
#define UNIQ_ID a(__COUNTER__)
#define a(b) z(b)
#define z(b) var##b
// Tests, provided by authors
int main() {
    int UNIQ_ID = 0;
    string UNIQ_ID = "hello";
    vector<string> UNIQ_ID = {"hello", "world"};
    vector<int> UNIQ_ID = {1, 2, 3, 4};
}
