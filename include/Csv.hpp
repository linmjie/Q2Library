#pragma once

#include "Book.hpp"
#include "Patron.hpp"
#include "Types.hpp"

namespace csv {
/**
 * @brief
 */
[[nodiscard]] Opt<Vec<Book>> load_books(const Path &path);
/**
 * @brief
 */
[[nodiscard]] Opt<Book> parse_book_line(const String &line);
/**
 * @brief
 */
[[nodiscard]] Opt<Vec<Patron>> load_patrons(const Path &path);
/**
 * @brief
 */
[[nodiscard]] Opt<Patron> parse_patron_line(const String &line);
/**
 * @brief 
 * @note May get swapped out with a templated version if needed
 */
[[nodiscard]] Opt<Vec<u64>> vector_from_chars(const String &str,
                                              char delimiter);
} // namespace csv
