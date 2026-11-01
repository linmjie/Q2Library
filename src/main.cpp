#include "Book.hpp"
#include "Csv.hpp"
#include "Patron.hpp"
#include "Types.hpp"
#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) {
    Path lib_path =
        std::filesystem::current_path().parent_path() / "data" / "library.csv";
    Path patron_path =
        std::filesystem::current_path().parent_path() / "data" / "library.csv";
    auto lib = csv::load_books(lib_path);
    if (!lib.has_value()) {
        std::cerr << "library.csv: uh oh\n";
    } else {
        for (const Book &book : *lib) {
            std::cout << book << '\n';
        }
    }
    auto patrons = csv::load_patrons(patron_path);
    if (!patrons.has_value()) {
        std::cerr << "patrons.csv: uh oh\n";
    } else {
        for (const Patron &patron : *patrons) {
            std::cout << patron << '\n';
        }
    }

    return 0;
}
