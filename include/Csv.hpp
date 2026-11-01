#pragma once

#include "Book.hpp"
#include "Patron.hpp"
#include "Types.hpp"

namespace csv {
    /**
     * @brief
     */
    Opt<Vec<Book>> load_books(const Path &path);
    /**
     * @brief
     */
    Opt<Book> parse_book_line(const String &line);
    /**
     * @brief
     */
    Opt<Vec<Patron>> load_patrons(const Path &path);
    /**
     * @brief
     */
    Opt<Patron> parse_patron_line(const String &line);
    /**
     * @brief
     */
    Opt<Vec<u64>> vector_from_chars(const String &str, char delimiter);
}
