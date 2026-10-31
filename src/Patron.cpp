#include "Patron.hpp"

Patron::Patron(int patron_id, const String &name, const String &email, int checked_out_count,
               const Vec<int> &current, double fines_owned)
    : current(current), name(name), email(email),
      fines_owned(fines_owned), patron_id(patron_id),
      checked_out_count(checked_out_count) {}

const Vec<u64> &Patron::get_current() const { return this->current; }
const String &Patron::get_name() const { return this->name; }
const String &Patron::get_email() const { return this->email; }
double Patron::get_fines_owned() const { return this->fines_owned; }
int Patron::get_patron_id() const { return this->patron_id; }
int Patron::get_checked_out_count() const { return this->checked_out_count; }
