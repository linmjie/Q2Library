#include "Book.hpp"
#include <charconv>
#include <fstream>
#include <iostream>
#include <sstream>

Opt<CheckoutStatus> str_to_checkout_status(const String &str) {
    if (str == "Available") {
        return CheckoutStatus::AVAILABLE;
    }
    if (str == "CheckedOut") {
        return CheckoutStatus::CHECKED_OUT;
    }
    return std::nullopt;
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

Opt<Vec<Book>> load_books(const Path &path) {
    Vec<Book> output{};
    std::ifstream csv(path);
    if (!csv.is_open()) {
        return std::nullopt;
    }

    String head;
    std::getline(csv, head);

    String line;
    while (std::getline(csv, line)) {
        Opt<Book> book = parse_book_line(line);
        if (!book.has_value()) {
            return std::nullopt;
        }
        output.push_back(std::move(*book));
    }

    return output;
}

Opt<Book> parse_book_line(const String &line) {
    // could be done character by character or using find but that's annoying..
    std::stringstream ss(line);

    String title;
    String author;
    u64 isbn{};

    String buf;
    // essentially boils down to:
    // fscanf(ss, "%s;%s;%d;%d", title, author, &isbn, &status);

    // it'd be really nice to have a scanf that can use the string class
    // and return error values if the pattern didn't match up

    if (!std::getline(ss, title, ';')) {
        return std::nullopt;
    }

    if (!std::getline(ss, author, ';')) {
        return std::nullopt;
    }

    if (!std::getline(ss, buf, ';')) {
        return std::nullopt;
    }
    // Source: https://en.cppreference.com/cpp/utility/from_chars
    // Used to learn non-exception based safe atoi conversion
    auto [ptr, err] =
        std::from_chars(buf.data(), buf.data() + buf.size(), isbn);
    if (err != std::errc()) {
        return std::nullopt;
    }

    if (!std::getline(ss, buf, ';')) {
        return std::nullopt;
    }
    Opt<CheckoutStatus> status = str_to_checkout_status(buf);
    if (!status.has_value()) {
        return std::nullopt;
    }

    return Book(title, author, isbn, *status);
}
