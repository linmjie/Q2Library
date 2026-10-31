#pragma once

#include "Types.hpp"

enum class CheckoutStatus : bool { AVAILABLE, CHECKED_OUT };
Opt<CheckoutStatus> str_to_checkout_status(const String &str);

class Book {
public:
    Book(const String &title, const String &author, u64 isbn,
         CheckoutStatus checkout_status);

    [[nodiscard]] const String &get_title() const;
    [[nodiscard]] const String &get_author() const;
    [[nodiscard]] u64 get_isbn() const;
    [[nodiscard]] CheckoutStatus get_checkout_status() const;
    [[nodiscard]] int get_checkout_day() const;

private:
    String title;
    String author;
    u64 isbn;
    CheckoutStatus checkout_status;
    int checkout_day;
};

/**
 * @brief Serializes a library.csv into a list of books
 */
Opt<Vec<Book>> load_books(const Path &path);
Opt<Book> parse_book_line(const String &line);
