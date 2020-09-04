#include <algorithm>

#include "airline_ticket.h"
// Unit test framework code provided in repo root
#include "test_runner.h"

using namespace std;
// Reducing difficulty of macro to minimal - it will just insert field name in lambda
#define SORT_BY(field)                                          \
    [](const AirlineTicket& lhs, const AirlineTicket& rhs) {    \
        return lhs.field < rhs.field;                           \
    }
// Providing required by lambda compare operators for structures
bool operator <(const Date& lhs, const Date& rhs) {
    return vector<int>{lhs.year, lhs.month, lhs.day} <
           vector<int>{rhs.year, rhs.month, rhs.day};
}
// To simplify them, using vector comparing. Also we can cast whole structure to smallest part of it (day or minute)
// But it seems to be slower. Of course I may be wrong
bool operator <(const Time& lhs, const Time& rhs) {
    return vector<int>{lhs.hours, lhs.minutes} <
           vector<int>{rhs.hours, rhs.minutes};
}
// This to operators required by test framework, they are not involved in real solution
bool operator ==(const Date& lhs, const Date& rhs) {
    return vector<int>{lhs.year, lhs.month, lhs.day} ==
           vector<int>{rhs.year, rhs.month, rhs.day};
}

ostream& operator <<(ostream& os, const Date& date) {
    os << date.year << "-" << date.month << "-" << date.day;

    return os;
}
// Tests, provided by authors
void TestSortBy() {
    vector<AirlineTicket> tickets = {
            {"VKO", "AER", "Utair",     {2018, 2, 28}, {17, 40}, {2018, 2, 28}, {20,  0}, 1200},
            {"AER", "VKO", "Utair",     {2018, 3,  5}, {14, 15}, {2018, 3,  5}, {16, 30}, 1700},
            {"AER", "SVO", "Aeroflot",  {2018, 3,  5}, {18, 30}, {2018, 3,  5}, {20, 30}, 2300},
            {"PMI", "DME", "Iberia",    {2018, 2,  8}, {23, 00}, {2018, 2,  9}, { 3, 30}, 9000},
            {"CDG", "SVO", "AirFrance", {2018, 3,  1}, {13, 00}, {2018, 3,  1}, {17, 30}, 8000},
    };

    sort(begin(tickets), end(tickets), SORT_BY(price));
    ASSERT_EQUAL(tickets.front().price, 1200);
    ASSERT_EQUAL(tickets.back().price, 9000);

    sort(begin(tickets), end(tickets), SORT_BY(from));
    ASSERT_EQUAL(tickets.front().from, "AER");
    ASSERT_EQUAL(tickets.back().from, "VKO");

    sort(begin(tickets), end(tickets), SORT_BY(arrival_date));
    ASSERT_EQUAL(tickets.front().arrival_date, (Date{2018, 2, 9}));
    ASSERT_EQUAL(tickets.back().arrival_date, (Date{2018, 3, 5}));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSortBy);
}
