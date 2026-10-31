#pragma once

#include "Types.hpp"

class Patron {
public:
    Patron(int patron_id, const String &name, const String &email,
           int checked_out_count, const Vec<u64> &current, double fines_owned);

    [[nodiscard]] const Vec<u64> &get_current() const;
    [[nodiscard]] const String &get_name() const;
    [[nodiscard]] const String &get_email() const;
    [[nodiscard]] double get_fines_owned() const;
    [[nodiscard]] int get_patron_id() const;
    [[nodiscard]] int get_checked_out_count() const;

private:
    Vec<u64> current;
    String name;
    String email;
    double fines_owned;
    int patron_id;
    int checked_out_count;
};

/**
 * @brief Serializes a patrons.csv into a list of patrons
 */
Opt<Vec<Patron>> load_patrons(const Path &path);
Opt<Patron> parse_patron_line(const String &line);
