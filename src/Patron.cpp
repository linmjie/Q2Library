#include "Patron.hpp"
#include <charconv>
#include <fstream>
#include <optional>
#include <regex>
#include <sstream>
#include <system_error>

Patron::Patron(int patron_id, const String &name, const String &email,
               int checked_out_count, const Vec<u64> &current,
               double fines_owned)
    : current(current), name(name), email(email), fines_owned(fines_owned),
      patron_id(patron_id), checked_out_count(checked_out_count) {}

const Vec<u64> &Patron::get_current() const { return this->current; }
const String &Patron::get_name() const { return this->name; }
const String &Patron::get_email() const { return this->email; }
double Patron::get_fines_owned() const { return this->fines_owned; }
int Patron::get_patron_id() const { return this->patron_id; }
int Patron::get_checked_out_count() const { return this->checked_out_count; }

Opt<Vec<Patron>> load_patrons(const Path &path) {
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

Opt<Patron> parse_patron_line(const String &line) {
    // could be done character by character or using find but that's annoying..
    std::stringstream ss(line);

    // name format is PXXX
    String name;
    String email;
    int checked_out_count{};
    double fines_owned{};

    String buf;

    int patron_id{};
    Vec<u64> current{};

    if (!std::getline(ss, name)) {
        return std::nullopt;
    }
    // std::regex r("P[0-9]{3}");

    if (!std::getline(ss, email)) {
        return std::nullopt;
    }

    if (!std::getline(ss, buf)) {
        return std::nullopt;
    }
    auto [ptr, err] = std::from_chars(buf.data(), buf.data() + buf.size(),
                                      checked_out_count);
    if (err != std::errc()) {
        return std::nullopt;
    }

    if (!std::getline(ss, buf)) {
        return std::nullopt;
    }

    if (!std::getline(ss, buf)) {
        return std::nullopt;
    }

    if (!std::getline(ss, buf)) {
        return std::nullopt;
    }


    return Patron(patron_id, name, email, checked_out_count, current, fines_owned);
}
