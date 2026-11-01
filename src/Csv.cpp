#include "Csv.hpp"
#include "Book.hpp"
#include "Patron.hpp"
#include "Types.hpp"
#include <charconv>
#include <format>
#include <fstream>
#include <regex>
#include <sstream>

Result<Vec<Book>> csv::load_books(const Path &path) {
    // TODO: add hashset to error on duplicate isbns
    Vec<Book> output;
    std::ifstream csv(path);
    if (!csv.is_open()) {
        return Err{std::format("{} could not be opened", path.string())};
    }

    String head;
    std::getline(csv, head);
    if (head != csv::JOINED_BOOK_HEADER) {
        return Err{std::format("'{}' does not match required header {}", head,
                               csv::JOINED_BOOK_HEADER)};
    }

    String line;
    // line numbers start at 1 for the user
    for (int line_num = 1; std::getline(csv, line); line_num++) {
        Result<Book> book = parse_book_line(line);
        if (!book.has_value()) {
            return Err{std::format("on line {} of {}: {} (line: {})", line_num,
                                   path.string(), book.error(), line)};
        }
        output.push_back(std::move(*book));
    }

    return output;
}

Result<Vec<Patron>> csv::load_patrons(const Path &path) {
    Vec<Patron> output;
    std::ifstream csv(path);
    if (!csv.is_open()) {
        return Err{std::format("{} could not be opened", path.string())};
    }

    String head;
    std::getline(csv, head);
    if (head != csv::JOINED_PATRON_HEADER) {
        return Err{std::format("'{}' does not match required header {}", head,
                               csv::JOINED_PATRON_HEADER)};
    }

    String line;
    // ids start at 001 and line numbers start at 1
    for (int running_id = 1; std::getline(csv, line); running_id++) {
        Result<Patron> book = parse_patron_line(line, running_id);
        if (!book.has_value()) {
            return Err{std::format("on line {} of {}: {} (line: {})",
                                   running_id, path.string(), book.error(),
                                   line)};
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
    u64 isbn;

    String buf;
    // essentially boils down to:
    // fscanf(ss, "%s;%s;%d;%d", title, author, &isbn, &status);

    // it'd be really nice to have a scanf that can use the string class
    // and return error values if the pattern didn't match up

    if (!std::getline(ss, title, ';')) {
        return Err{"no ';' to end title column"};
    }

    if (!std::getline(ss, author, ';')) {
        return Err{"no ';' to end author column"};
    }

    if (!std::getline(ss, buf, ';')) {
        return Err{"no ';' to end isbn column"};
    }
    // Source: https://en.cppreference.com/cpp/utility/from_chars
    // Used to learn non-exception based safe atoi conversion
    auto [ptr, err] =
        std::from_chars(buf.data(), buf.data() + buf.size(), isbn);
    if (err == std::errc::invalid_argument) {
        return Err{std::format(
            "isbn column: '{}' could not be parsed as an integer", buf)};
    } else if (err == std::errc::result_out_of_range) {
        return Err{std::format("isbn column: '{}' out of integer range", buf)};
    }

    if (!std::getline(ss, buf)) {
        return Err{"no checkout status column"};
    }
    Opt<CheckoutStatus> status = str_to_checkout_status(buf);
    if (!status.has_value()) {
        return Err{std::format("could not parse status cell as a checkout "
                               "status - valid statuses: {} and {}",
                               AVAILABLE_STR, CHECKED_OUT_STR)};
    }

    return Book(title, author, isbn, *status);
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
        return Err{"no ';' to end patron_id column"};
    }
    // match PXXX where X is digit
    std::regex re(R"(P\d{3})");
    bool matches = std::regex_match(buf, re);
    if (!matches) {
        return Err{std::format(
            "{} could did not match PXXX format where XXX is some number",
            buf)};
    }
    auto [ptr, err] =
        std::from_chars(buf.data() + 1, buf.data() + buf.size(), patron_id);
    if (err == std::errc::invalid_argument) {
        return Err{std::format(
            "{} could did not match PXXX format where XXX is some number",
            buf)};
    }
    if (err == std::errc::result_out_of_range || patron_id > 999) {
        return Err{
            std::format("{} id is out of range (must be between 1-999)", buf)};
    }
    if (patron_id != required_id) {
        return Err{std::format(
            "{} id does not follow consecutive order; must be P{:03}", buf,
            required_id)};
    }

    if (!std::getline(ss, name, ';')) {
        return Err{"no ';' to end name column"};
    }

    if (!std::getline(ss, email, ';')) {
        return Err{"no ';' to end email column"};
    }
    // add regex to validate email?

    if (!std::getline(ss, buf, ';')) {
        return Err{"no ';' to end checked_out_count column"};
    }
    auto [ptr2, err2] =
        std::from_chars(buf.data(), buf.data() + buf.size(), checked_out_count);
    if (err2 == std::errc::invalid_argument) {
        return Err{std::format("checked_out_count column: '{}' could not "
                               "be parsed as a number",
                               buf)};
    } else if (err2 == std::errc::result_out_of_range) {
        return Err{
            std::format("checked_out_count column: '{}' out of numeric range",
                        checked_out_count)};
    }

    if (!std::getline(ss, buf, ';')) {
        return Err{"no ';' to end current column"};
    }

    Result<Vec<u64>> current = csv::vector_from_chars(buf, '|');
    if (!current.has_value()) {
        return Err{std::format("current column: {}", current.error())};
    }
    // I don't feel like overloading the -> operator for this one line...
    size_t current_size = (*current).size();
    if (current_size > 3) {
        return Err{std::format("current column: '{}' should only have three "
                               "books (isbns) checked out, {} were found",
                               buf, current_size)};
    }
    // Potentially return an error if they don't match up to begin with?
    checked_out_count = current_size;

    if (!std::getline(ss, buf)) {
        return Err{"no fines_owned column"};
    }
    auto [ptr3, err3] =
        std::from_chars(buf.data(), buf.data() + buf.size(), fines_owned);
    if (err3 == std::errc::invalid_argument) {
        return Err{std::format(
            "fines_owned column: '{}' could not be parsed as a number", buf)};
    } else if (err3 == std::errc::result_out_of_range) {
        return Err{
            std::format("fines_owned column: '{}' out of numeric range", buf)};
    }

    return Patron(patron_id, name, email, checked_out_count, *current,
                  fines_owned);
}

// could be generalized with template but templates with .h & .cpp is annoying
Result<Vec<u64>> csv::vector_from_chars(const String &str, char delimiter) {
    Vec<u64> ret;
    // maybe change to check if all whitespace instead of is empty?
    if (str.empty()) {
        return ret;
    }
    std::stringstream ss(str);

    String buf;
    u64 num{};
    while (std::getline(ss, buf, delimiter)) {
        auto [ptr, err] =
            std::from_chars(buf.data(), buf.data() + buf.size(), num);
        if (err == std::errc::invalid_argument) {
            return Err{std::format("{} could not be parsed as a number", buf)};
        } else if (err == std::errc::result_out_of_range) {
            return Err{std::format("'{}' out of numeric range", buf)};
        }
        ret.push_back(num);
    }
    return ret;
}
