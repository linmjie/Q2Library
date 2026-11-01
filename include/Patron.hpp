#pragma once

#include "Types.hpp"

class Patron {
public:
    /**
     * @brief
     */
    Patron(int patron_id, const String &name, const String &email,
           int checked_out_count, const Vec<u64> &current, double fines_owned);

    /**
     * @brief
     */
    [[nodiscard]] const Vec<u64> &get_current() const;
    /**
     * @brief
     */
    [[nodiscard]] const String &get_name() const;
    /**
     * @brief
     */
    [[nodiscard]] const String &get_email() const;
    /**
     * @brief
     */
    [[nodiscard]] double get_fines_owned() const;
    /**
     * @brief
     */
    [[nodiscard]] int get_patron_id() const;
    /**
     * @brief
     */
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
 * @brief insertion operator overload to allow printing the patron to an ostream
 *
 * @param stream The ostream to print to
 * @param patron The book to print
 *
 * @return The stream back out to use chain
 */
std::ostream& operator<<(std::ostream& stream, const Patron &patron);
