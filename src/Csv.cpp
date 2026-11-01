#include "Csv.hpp"
#include "Book.hpp"
#include "Patron.hpp"
#include "Types.hpp"
#include <charconv>
#include <fstream>
#include <regex>
#include <sstream>

Opt<Vec<Book>> csv::load_books(const Path &path) {
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

Opt<Book> csv::parse_book_line(const String &line) {
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

Opt<Vec<Patron>> csv::load_patrons(const Path &path) {
    Vec<Patron> output{};
    std::ifstream csv(path);
    if (!csv.is_open()) {
        return std::nullopt;
    }

    String head;
    std::getline(csv, head);

    String line;
    while (std::getline(csv, line)) {
        Opt<Patron> book = parse_patron_line(line);
        if (!book.has_value()) {
            return std::nullopt;
        }
        output.push_back(std::move(*book));
    }

    return output;
}

Opt<Patron> csv::parse_patron_line(const String &line) {
    // could be done character by character or using find but that's annoying..
    std::stringstream ss(line);

    // id format is PXXX
    int patron_id{};
    String name;
    String email;
    int checked_out_count{};
    // Vec<u64> current;
    double fines_owned{};

    String buf;

    if (!std::getline(ss, buf)) {
        return std::nullopt;
    }
    // match PXXX where X is digit
    std::regex re(R"(P\d{3})");
    bool matches = std::regex_match(buf, re);
    if (!matches) {
        return std::nullopt;
    }
    auto [ptr, err] =
        std::from_chars(buf.data() + 1, buf.data() + buf.size(), patron_id);
    if (err != std::errc()) {
        return std::nullopt;
    }

    if (!std::getline(ss, name)) {
        return std::nullopt;
    }

    if (!std::getline(ss, email)) {
        return std::nullopt;
    }

    if (!std::getline(ss, buf)) {
        return std::nullopt;
    }
    auto [ptr2, err2] = std::from_chars(buf.data(), buf.data() + buf.size(),
                                      checked_out_count);
    if (err2 != std::errc()) {
        return std::nullopt;
    }

    if (!std::getline(ss, buf)) {
        return std::nullopt;
    }
    Opt<Vec<u64>> current = (csv::vector_from_chars(buf, '|'));
    if (!current.has_value()) {
        return std::nullopt;
    }

    if (!std::getline(ss, buf)) {
        return std::nullopt;
    }
    auto [ptr3, err3] =
        std::from_chars(buf.data(), buf.data() + buf.size(), fines_owned);
    if (err3 != std::errc()) {
        return std::nullopt;
    }
    checked_out_count = current->size();

    return Patron(patron_id, name, email, checked_out_count, *current, fines_owned);
}

// could be generalized with template but templates with .h & .cpp is annoying
Opt<Vec<u64>> csv::vector_from_chars(const String &str, char delimiter) {
    Vec<u64> ret;
    std::stringstream ss(str);

    String buf;
    int num{};
    while (!std::getline(ss, buf, delimiter)) {
        auto [ptr, err] =
            std::from_chars(buf.data(), buf.data() + buf.size(), num);
        if (err != std::errc()) {
            return std::nullopt;
        }
        ret.push_back(num);
    }
    return ret;
}
