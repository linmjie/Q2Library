#pragma once

#include "Types.hpp"

/**
 * @brief Represents a Patron object that can be read in from a patron csv
 */
class Patron {
public:
    /**
     * @brief Constructs a Patron object
     *
     * @param patron_id Id represnting the patron - should be between 1 and 100
     * @param name A string representation of the patron's name (makes copy)
     * @param email A string representation of the patron's email (makes copy)
     * @param checked_out_count The number of checked out isbn's; should be less
     * than 3
     * @param current A vector of all checked out isbn's; length should be less
     * than 3 (makes copy)
     * @param fines_owned The amount of money owed for overdue books
     *
     * @note arguments are passed in the order defined for the patrons.csv but
     * for effiency the members will not be constructed in the order of the
     * arguments
     */
    Patron(int patron_id, const String &name, const String &email,
           int checked_out_count, const Vec<u64> &current, double fines_owned);

    /** @brief Returns a read-only view of the checked out books */
    [[nodiscard]] const Vec<u64> &get_current() const;

    /** @brief Returns a read-only view of the patron's name */
    [[nodiscard]] const String &get_name() const;

    /** @brief Returns a read-only view of the patron's email */
    [[nodiscard]] const String &get_email() const;

    /** @brief Returns a copy of the fines owned by the patron */
    [[nodiscard]] double get_fines_owned() const;

    /** @brief Returns a copy of the patron's id */
    [[nodiscard]] int get_patron_id() const;

    /** @brief Returns a copy of the patron's number of checked out books */
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
std::ostream &operator<<(std::ostream &stream, const Patron &patron);
