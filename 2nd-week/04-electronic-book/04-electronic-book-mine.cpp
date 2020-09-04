#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

class ReadingManager {
public:
    // This task is simplified by knowledge of number of pages and people, this system should handle
    // We can use vectors with constant access time and "sort" them by ourselves
    ReadingManager()
            : usersToPages(MAX_USER_COUNT_ + 1, 0),
              pagesToUsers(MAX_PAGES_COUNT_ + 1, 0) {}
    // This vectors work like maps. One stores number of pages by user_id, second one stores number of users by page
    void Read(const int& user_id, const int& page_count) {
        if (usersToPages[user_id] == 0) {
            usersToPages[user_id] = page_count;
            pagesToUsers[page_count]++;
            totalUsers++;
        }
        else {
            pagesToUsers[usersToPages[user_id]]--;
            usersToPages[user_id] = page_count;
            pagesToUsers[page_count]++;
        }
    }

    [[nodiscard]] double Cheer(const int& user_id) const {
        if (usersToPages[user_id] == 0) {
            return 0;
        }

        if (totalUsers == 1) {
            return 1;
        }
        // Constant access time gives us opportunity to just iterate through vector linearly
        int user_count = 0;
        const int page_number = usersToPages[user_id];
        for (size_t i = 0; i < page_number; ++i) {
            user_count += pagesToUsers[i];
        }
        // We used totalUsers to avoid problem with empty nodes in vector -
        // we just count "size" of library by our hands, as we did with "sorting"
        return (user_count * 1.0) / (totalUsers - 1);
    }

private:
    static const int MAX_USER_COUNT_ = 100'000;
    static const int MAX_PAGES_COUNT_ = 1000;

    vector<int> usersToPages;
    vector<int> pagesToUsers;
    int totalUsers = 0;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        }
        else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }

    return 0;
}
