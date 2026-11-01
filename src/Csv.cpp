#include "Csv.hpp"
#include "Book.hpp"
#include "Patron.hpp"
#include "Types.hpp"
#include <charconv>
#include <fstream>
#include <regex>
#include <sstream>

Result<Vec<Book>> csv::load_books(const Path &path) {
    Vec<Book> output{};
    std::ifstream csv(path);
    if (!csv.is_open()) {
        return Err{""};
    }

    String head;
    std::getline(csv, head);

    String line;
    while (std::getline(csv, line)) {
        Result<Book> book = parse_book_line(line);
        if (!book.has_value()) {
            return Err{""};
        }
        output.push_back(std::move(*book));
    }

    return output;
}

Result<Book> csv::parse_book_line(const String &line) {
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
        return Err{""};
    }

    if (!std::getline(ss, author, ';')) {
        return Err{""};
    }

    if (!std::getline(ss, buf, ';')) {
        return Err{""};
    }
    // Source: https://en.cppreference.com/cpp/utility/from_chars
    // Used to learn non-exception based safe atoi conversion
    auto [ptr, err] =
        std::from_chars(buf.data(), buf.data() + buf.size(), isbn);
    if (err == std::errc::invalid_argument) {
        return Err{""};
    } else if (err == std::errc::result_out_of_range) {
        return Err{""};
    }

    if (!std::getline(ss, buf)) {
        return Err{""};
    }
    Opt<CheckoutStatus> status = str_to_checkout_status(buf);
    if (!status.has_value()) {
        return Err{""};
    }

    return Book(title, author, isbn, *status);
}

Result<Vec<Patron>> csv::load_patrons(const Path &path) {
    Vec<Patron> output{};
    std::ifstream csv(path);
    if (!csv.is_open()) {
        return Err{""};
    }

    String head;
    std::getline(csv, head);

    String line;
    for (int running_id = 0; std::getline(csv, line); running_id++) {
        Result<Patron> book = parse_patron_line(line, running_id);
        if (!book.has_value()) {
            return Err{""};
        }
        output.push_back(std::move(*book));
    }

    return output;
}

Result<Patron> csv::parse_patron_line(const String &line, int required_id) {
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

    if (!std::getline(ss, buf, ';')) {
        return Err{""};
    }
    // match PXXX where X is digit
    std::regex re(R"(P\d{3})");
    bool matches = std::regex_match(buf, re);
    if (!matches) {
        return Err{""};
    }
    auto [ptr, err] =
        std::from_chars(buf.data() + 1, buf.data() + buf.size(), patron_id);
    if (err == std::errc{}) {
        return Err{""};
    }
    if (err == std::errc::result_out_of_range || patron_id > 999) {
        return Err{""};
    }
    if (patron_id != required_id) {
        return Err{""};
    }

    if (!std::getline(ss, name, ';')) {
        return Err{""};
    }

    if (!std::getline(ss, email, ';')) {
        return Err{""};
    }

    if (!std::getline(ss, buf, ';')) {
        return Err{""};
    }
    auto [ptr2, err2] =
        std::from_chars(buf.data(), buf.data() + buf.size(), checked_out_count);
    if (err2 == std::errc::invalid_argument) {
        return Err{""};
    } else if (err2 == std::errc::result_out_of_range) {
        return Err{""};
    }

    if (!std::getline(ss, buf, ';')) {
        return Err{""};
    }
    
    Result<Vec<u64>> current = csv::vector_from_chars(buf, '|');
    if (!current.has_value()) {
        return Err{""};
    }

    if (!std::getline(ss, buf)) {
        return Err{""};
    }
    auto [ptr3, err3] =
        std::from_chars(buf.data(), buf.data() + buf.size(), fines_owned);
    if (err3 == std::errc::invalid_argument) {
        return Err{""};
    } else if (err3 == std::errc::result_out_of_range) {
        return Err{""};
    }
    // I'm not overloading -> for this one line
    checked_out_count = (*current).size();

    return Patron(patron_id, name, email, checked_out_count, *current,
                  fines_owned);
}

// could be generalized with template but templates with .h & .cpp is annoying
Result<Vec<u64>> csv::vector_from_chars(const String &str, char delimiter) {
    Vec<u64> ret;
    std::stringstream ss(str);

    String buf;
    int num{};
    while (!std::getline(ss, buf, delimiter)) {
        auto [ptr, err] =
            std::from_chars(buf.data(), buf.data() + buf.size(), num);
        if (err == std::errc{}) {
            return Err{""};
        }
        ret.push_back(num);
    }
    return ret;
}
