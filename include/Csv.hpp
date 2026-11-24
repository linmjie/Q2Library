#pragma once

#include "Book.hpp"
#include "Patron.hpp"
#include "Result.hpp"
#include "Types.hpp"

namespace csv {
constexpr Str JOINED_BOOK_HEADER = "title;author;isbn;status";
constexpr Str JOINED_PATRON_HEADER =
    "patron_id;name;email;checked_out_count;current;fines_owed";

// I don't feel like making full structs for the errors but Vec<String> is
// rather ambiguous
using Errors = Vec<String>;

/**
 * @brief Loads a library csv into a vector of books if completely successful.
 * If there are errors parsing the file, then a list of errors are returned
 *
 * @param path A Path object denoting the csv path
 *
 * @return A Result object that contains a vector of books if successful, if any
 * parsing errors are present, the Result object will instead contain a vector
 * of errors as strings
 */
[[nodiscard]] Result<Vec<Book>, Errors> load_books(const Path &path);

/**
 * @brief Parses a single line into a Book object or a string error if there are
 * parsing errors. Lines follow the csv line format denoted in the assignment.
 *
 * @param line The line string to parse
 *
 * @return A Result object that contains a Book object if successful or a string
 * error denoting the first error on the line
 */
[[nodiscard]] Result<Book> parse_book_line(const String &line);

/**
 * @brief Loads a patron csv into a vector of patrons if completely successful.
 * If there are errors parsing the file, then a list of errors are returned
 *
 * @param path A Path object denoting the csv path
 *
 * @return A Result object that contains a vector of patrons if successful, if
 * any parsing errors are present, the Result object will instead contain a
 * vector of errors as strings
 */
[[nodiscard]] Result<Vec<Patron>, Errors> load_patrons(const Path &path);

/**
 * @brief Parses a single line into a Patron object or a string error if there
 * are parsing errors. Lines follow the csv format denoted in the assignment.
 *
 * @param line The line string to parse
 * @param prev_id The previous id, the patron parsed in this line must then be
 * prev_id + 1
 *
 * @return A Result object that contains a Patron object if successful or a
 * string error denoting the first error on the line
 */
[[nodiscard]] Result<Patron> parse_patron_line(const String &line, int prev_id);

/**
 * @brief Parses a line of numbers separated by a user-specified character
 * delimiter or a string error if parsing fails
 *
 * @param str The string to parse
 * @param delimiter The character delimiter: will attempt to parse numbers on
 * either side of the delimiter
 *
 * @return A Result object of either the vector of numbers or a string error if
 * there were parsing errors
 *
 * @note May get swapped out with a templated version if needed
 */
[[nodiscard]] Result<Vec<u64>> vector_from_chars(const String &str,
                                                 char delimiter);
} // namespace csv
