#include "Book.hpp"

Opt<CheckoutStatus> str_to_checkout_status(const String &str) {
    if (str == "Available") {
        return CheckoutStatus::AVAILABLE;
    }
    if (str == "CheckedOut") {
        return CheckoutStatus::CHECKED_OUT;
    }
    return std::nullopt;
}

String checkout_status_to_string(CheckoutStatus status) {
    return status == CheckoutStatus::AVAILABLE ? "Available" : "CheckedOut";
}

Book::Book(const String &title, const String &author, u64 isbn,
           CheckoutStatus checkout_status)
    : title(title), author(author), isbn(isbn),
      checkout_status(checkout_status) {}

const String &Book::get_title() const { return title; }
const String &Book::get_author() const { return author; }
u64 Book::get_isbn() const { return isbn; }
CheckoutStatus Book::get_checkout_status() const { return checkout_status; }
int Book::get_checkout_day() const { return this->checkout_day; };

std::ostream &operator<<(std::ostream &stream, const Book &book) {
    stream << book.get_title() << ';' << book.get_author() 
           << ';' << book.get_isbn() 
           << ';' << checkout_status_to_string(book.get_checkout_status());
    return stream;
}
