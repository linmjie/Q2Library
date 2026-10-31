#include "Book.hpp"

Book::Book(const String &title, const String &author, u64 isbn,
           CheckoutStatus checkout_status)
    : title(title), author(author), isbn(isbn),
      checkout_status(checkout_status) {}

const String &Book::get_title() const { return title; }
const String &Book::get_author() const { return author; }
u64 Book::get_isbn() const { return isbn; }
CheckoutStatus Book::get_checkout_status() const { return checkout_status; }
int Book::get_checkout_day() const { return this->checkout_day; };
