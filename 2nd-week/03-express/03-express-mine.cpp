#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;

class RouteManager {
public:
    // Add operation time for set and vector was almost on the same level for example data
    void AddRoute(const int& start, const int& finish) {
        reachable_lists_[start].insert(finish);
        reachable_lists_[finish].insert(start);
    }
    // Bottleneck of the program
    [[nodiscard]] int FindNearestFinish(const int& start, const int& finish) const {
        int result = abs(start - finish);

        if (reachable_lists_.count(start) < 1) {
            return result;
        }
        // Set and lower_bound eliminates need to linearly iterate through container
        const set<int> &reachable_stations = reachable_lists_.at(start);
        const auto cutPoint = reachable_stations.lower_bound(finish);
        // If lower_bound result is not a border element,
        // assigning result minimal distance between finish and lower_bound and previous element
        if (cutPoint != end(reachable_stations))
            result = min(result, abs(finish - *cutPoint));
        if (cutPoint != begin(reachable_stations))
            result = min(result, abs(finish - *prev(cutPoint)));

        return result;
    }
// Changing vector to set to keep elements sorted
private:
    map<int, set<int>> reachable_lists_;
};

int main() {
    RouteManager routes;

    string query_type;
    int query_count, start, finish;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        cin >> query_type;
        cin >> start >> finish;

        if (query_type == "ADD") {
            routes.AddRoute(start, finish);
        }
        else if (query_type == "GO") {
            cout << routes.FindNearestFinish(start, finish) << "\n";
        }
    }

    return 0;
}
