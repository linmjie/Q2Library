#pragma once

#include "Book.hpp"
#include "Patron.hpp"
#include "Result.hpp"
#include "Types.hpp"

namespace csv {
constexpr Str JOINED_BOOK_HEADER = "title;author;isbn;status";
constexpr Str JOINED_PATRON_HEADER =
    "patron_id;name;email;checked_out_count;current;fines_owed";

/**
 * @brief
 */
[[nodiscard]] Result<Vec<Book>> load_books(const Path &path);
/**
 * @brief
 */
[[nodiscard]] Result<Book> parse_book_line(const String &line);
/**
 * @brief
 */
[[nodiscard]] Result<Vec<Patron>> load_patrons(const Path &path);
/**
 * @brief
 */
[[nodiscard]] Result<Patron> parse_patron_line(const String &line, int prev_id);
/**
 * @brief
 * @note May get swapped out with a templated version if needed
 */
[[nodiscard]] Result<Vec<u64>> vector_from_chars(const String &str,
                                                 char delimiter);
} // namespace csv
