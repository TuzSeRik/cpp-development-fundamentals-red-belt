#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <queue>

using namespace std;
// Accumulate all relevant information about booking in one structure
struct BookingEntry {
    string hotelName;
    int clientId, roomCount;
    int64_t time;
};

class BookingManager {
private:
    // Minimal time, as it stated in task description
    int64_t currentTime = -1'000'000'000'000'000'000;
    // To reduce time needed, we'll store all bookings entry in queue and all hotels statistics in maps.
    // First stores map of users with number of rooms they booked.
    // Second stores total number of booked rooms
    queue<BookingEntry> hotelQueue;
    unordered_map<string, unordered_map<int, int>> bookingStatsClients;
    unordered_map<string, int> bookingStatsRooms;

    void AdjustTime() {
        // Deleting entries older than one day
        while (!hotelQueue.empty() && abs(currentTime - hotelQueue.front().time) >= 86400) {
            auto &frontEntry = hotelQueue.front();
            // For every entry reducing number of booked rooms of relevant user.
            // If no rooms left, delete user from map
            bookingStatsClients.at(frontEntry.hotelName).at(frontEntry.clientId) -= frontEntry.roomCount;
            if (bookingStatsClients.at(frontEntry.hotelName).at(frontEntry.clientId) == 0)
                bookingStatsClients.at(frontEntry.hotelName).erase(frontEntry.clientId);
            // Reducing total number of booked rooms
            bookingStatsRooms.at(frontEntry.hotelName) -= frontEntry.roomCount;

            hotelQueue.pop();
        }
    }

public:
    void Book(const int64_t& time, const string& hotel_name, const int& client_id, const int& room_count) {
        // Adding entries, clients, rooms and so on
        hotelQueue.push({hotel_name, client_id, room_count, time});
        bookingStatsClients[hotel_name][client_id] += room_count;
        bookingStatsRooms[hotel_name] += room_count;
        // If current time changed, delete old entries
        if (time > currentTime) {
            currentTime = time;
            AdjustTime();
        }
    }
    // If no entry found, both methods should return 0
    int Clients(const string& hotel_name) const {
        try {
            return bookingStatsClients.at(hotel_name).size();
        }
        catch (out_of_range&) {
            return 0;
        }
    }

    int Rooms(const string& hotel_name) const {
        try {
            return bookingStatsRooms.at(hotel_name);
        }
        catch (out_of_range&) {
            return 0;
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    BookingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;

        if (query_type == "BOOK") {
            int64_t time;
            string hotel_name;
            int client_id, room_count;
            cin >> time >> hotel_name >> client_id >> room_count;
            manager.Book(time, hotel_name, client_id, room_count);
        }
        else {
            string hotel_name;
            cin >> hotel_name;

            if (query_type == "CLIENTS") {
                cout << manager.Clients(hotel_name) << "\n";
            }
            else {
                cout << manager.Rooms(hotel_name) << "\n";
            }
        }
    }

    return 0;
}
