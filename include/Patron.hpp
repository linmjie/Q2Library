#pragma once

#include "Types.hpp"
#include <string>

class Patron {
public:
    Vec<int> current;
    std::string name;
    std::string email;
    double fines_owned;
    int patron_id;
    int checked_out_count;

    Patron(int patron_id, Str name, Str email, int checked_out_count,
           const Vec<int> &current, double fines_owned);

    static Vec<Patron> patron_registry;

    static void test(void);

private:
};
