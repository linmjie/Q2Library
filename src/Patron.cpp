#include "Patron.hpp"
#include <cstdio>
#include <iomanip>
#include <ios>

Patron::Patron(int patron_id, const String &name, const String &email,
               int checked_out_count, const Vec<u64> &current,
               double fines_owned)
    : current(current), name(name), email(email), fines_owned(fines_owned),
      patron_id(patron_id), checked_out_count(checked_out_count) {}

const Vec<u64> &Patron::get_current() const { return this->current; }
const String &Patron::get_name() const { return this->name; }
const String &Patron::get_email() const { return this->email; }
double Patron::get_fines_owned() const { return this->fines_owned; }
int Patron::get_patron_id() const { return this->patron_id; }
int Patron::get_checked_out_count() const { return this->checked_out_count; }

std::ostream &operator<<(std::ostream &stream, const Patron &patron) {
    char patron_id[4];
    patron_id[3] = '\0';
    std::snprintf(patron_id, 3, "%d", patron.get_patron_id());

    stream << patron_id << ';' << patron.get_name() << ';' 
           << patron.get_email() << ';' << patron.get_checked_out_count() << ';';

    const Vec<u64> &current = patron.get_current();
    if (current.size() == 0) {
        stream << ';';
    }
    else if (current.size() == 1) {
        stream << current[0] << ';';
    } else {
        for (auto it = current.begin(); it != current.end() - 1; ++it) {
            stream << *it << '|';
        }
        stream << current.back() << ';';
    }
    // Source: https://arstechnica.com/civis/threads/resetting-stream-formatting-options-in-c.533692/
    // Used to save and reset the flags of the ostream
    auto flags = stream.flags();
    stream << std::fixed << std::setprecision(2);
    stream << patron.get_fines_owned();
    stream.flags(flags);
    return stream;
}
