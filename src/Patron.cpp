#include "Patron.hpp"
#include <iostream>

static void register_patron(const Patron *patron, int id) {
    // write later lol
}

Patron::Patron(int patron_id, Str name, Str email, int checked_out_count,
               const Vec<int> &current, double fines_owned)
    : current(current), name(name), email(email), fines_owned(fines_owned),
      patron_id(patron_id), checked_out_count(checked_out_count) 
{
    register_patron(this, this->patron_id);
}

void Patron::test(void) { std::cout << "Patron test\n"; }
