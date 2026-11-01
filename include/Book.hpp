#pragma once

#include "Types.hpp"
#include <ostream>

enum class CheckoutStatus : bool { AVAILABLE, CHECKED_OUT };

/**
 * @brief
 */
[[nodiscard]] Opt<CheckoutStatus> str_to_checkout_status(const String &str);

class Book {
public:
    /**
     * @brief
     */
    Book(const String &title, const String &author, u64 isbn,
         CheckoutStatus checkout_status);

    /**
     * @brief
     */
    [[nodiscard]] const String &get_title() const;
    /**
     * @brief
     */
    [[nodiscard]] const String &get_author() const;
    /**
     * @brief
     */
    [[nodiscard]] u64 get_isbn() const;
    /**
     * @brief
     */
    [[nodiscard]] CheckoutStatus get_checkout_status() const;
    /**
     * @brief
     */
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
