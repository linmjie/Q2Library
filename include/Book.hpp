#pragma once

#include "Types.hpp"

enum class CheckoutStatus : bool { Available, CheckedOut };

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
