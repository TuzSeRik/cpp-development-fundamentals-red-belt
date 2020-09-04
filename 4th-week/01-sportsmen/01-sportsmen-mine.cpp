#include <iostream>
#include <vector>
#include <list>

using namespace std;

int main() {
    // I/O delay time reduction
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // Storing iterators of every sportsmen
    list<int> queue;
    vector<list<int>::iterator> positions(100000, end(queue));
    int q;
    cin >> q;
    // Connecting new sportsman to needed (or end) and storing iterator to him in vector
    int newSportsman, nextSportsman;
    for (int i = 0; i < q; ++i) {
        cin >> newSportsman >> nextSportsman;
        positions[newSportsman] = queue.insert(positions[nextSportsman], newSportsman);
    }
    // Iterating through resulting queue and outputting it
    for (const auto& sportsman : queue)
        cout << sportsman << "\n";

    return 0;
}
