#include "Book.hpp"
#include "Csv.hpp"
#include "Patron.hpp"
#include "Types.hpp"
#include <filesystem>
#include <iostream>

void output_error(const String &err) {
    std::cerr << "[ERROR] " << err << '\n';
}

int main(int argc, char *argv[]) {
    Path data = std::filesystem::current_path() / "data";
    Path lib_path = data / "library.csv";
    Path patron_path = data / "patrons.csv";
    auto lib = csv::load_books(lib_path);
    if (!lib.has_value()) {
        for (const auto &error : lib.error()) {
            output_error(error);
        }
    } else {
        for (const Book &book : *lib) {
            std::cout << book << '\n';
        }
    }
    std::cout << '\n';
    auto patrons = csv::load_patrons(patron_path);
    if (!patrons.has_value()) {
        for (const auto &error : patrons.error()) {
            output_error(error);
        }
    } else {
        for (const Patron &patron : *patrons) {
            std::cout << patron << '\n';
        }
    }

    return 0;
}
