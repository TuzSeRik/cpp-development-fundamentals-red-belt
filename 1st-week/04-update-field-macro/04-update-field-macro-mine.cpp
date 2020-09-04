#include "airline_ticket.h"
// Unit test framework code provided in repo root
#include "test_runner.h"

using namespace std;
// To unify input of all types of variables, read everything from stringstream
#define UPDATE_FIELD(ticket, field, values)    \
      {                                        \
          auto it = values.find(#field);       \
                                               \
          if (it != values.end()) {            \
              istringstream is(it->second);    \
              is >> ticket.field;              \
          }                                    \
      }
// Operators, required by task
istream& operator >>(istream& is, Date& date) {
    is >> date.year;
    is.ignore(1);
    is >> date.month;
    is.ignore(1);
    is >> date.day;

    return is;
}

istream& operator >>(istream& is, Time& time) {
    is >> time.hours;
    is.ignore(1);
    is >> time.minutes;

    return is;
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

bool operator ==(const Time& lhs, const Time& rhs) {
    return vector<int>{lhs.hours, lhs.minutes} ==
            vector<int>{rhs.hours, rhs.minutes};
}

ostream& operator <<(ostream& os, const Time& time) {
    os << time.hours << ":" << time.minutes;

    return os;
}
// Tests, provided by authors
void TestUpdate() {
    AirlineTicket t;
    t.price = 0;

    const map<string, string> updates1 = {
            {"departure_date", "2018-2-28"},
            {"departure_time", "17:40"},
    };
    UPDATE_FIELD(t, departure_date, updates1)
    UPDATE_FIELD(t, departure_time, updates1)
    UPDATE_FIELD(t, price, updates1)

    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}))
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}))
    ASSERT_EQUAL(t.price, 0)

    const map<string, string> updates2 = {
            {"price", "12550"},
            {"arrival_time", "20:33"},
    };
    UPDATE_FIELD(t, departure_date, updates2)
    UPDATE_FIELD(t, departure_time, updates2)
    UPDATE_FIELD(t, arrival_time, updates2)
    UPDATE_FIELD(t, price, updates2)

    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}))
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}))
    ASSERT_EQUAL(t.price, 12550)
    ASSERT_EQUAL(t.arrival_time, (Time{20, 33}))
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestUpdate);
}
