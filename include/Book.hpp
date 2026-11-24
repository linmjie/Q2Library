#pragma once

#include "Types.hpp"
#include <ostream>

enum class CheckoutStatus : bool { AVAILABLE, CHECKED_OUT };
// kinda coupled but whatever
constexpr Str AVAILABLE_STR = "Available";
constexpr Str CHECKED_OUT_STR = "CheckedOut";

/**
 * @brief Maps strings to a CheckoutStatus enum or none if it's an invalid map
 */
[[nodiscard]] Opt<CheckoutStatus> str_to_checkout_status(const String &str);

/**
 * @brief Represents a book object that can be read in from a library csv
 */
class Book {
public:
    /**
     * @brief Constructs a valid Book object
     *
     * @param title A string representation of a title (makes copy)
     * @param author A string representation of the author (makes copy)
     * @param isbn A number denoting the isbn - must be less than the max 64-bit
     * @param checkout_status An enum of AVAILABLE and CHECKED_OUT
     */
    Book(const String &title, const String &author, u64 isbn,
         CheckoutStatus checkout_status);

    /** @brief Returns a read-only view of the book's title */
    [[nodiscard]] const String &get_title() const;

    /** @brief Returns a read-only view of the book's title */
    [[nodiscard]] const String &get_author() const;

    /** @brief Returns a copy of the book's isbn */
    [[nodiscard]] u64 get_isbn() const;

    /** @brief Returns a copy of the book's checkout_status */
    [[nodiscard]] CheckoutStatus get_checkout_status() const;

    /** @brief Returns the book's checkout_day */
    [[nodiscard]] int get_checkout_day() const;

private:
    String title;
    String author;
    u64 isbn;
    CheckoutStatus checkout_status;
    int checkout_day{};
};

/**
 * @brief insertion operator overload to allow printing the book to an ostream
 *
 * @param stream The ostream to print to
 * @param book The book to print
 *
 * @return The stream back out to use chain
 */
std::ostream &operator<<(std::ostream &stream, const Book &book);
