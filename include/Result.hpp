#pragma once

#include "Types.hpp"
#include <variant>

/**
 * @brief A wrapper struct that enables easy explicit error making for Result
 */
struct Err {
    explicit Err(const String &value);
    String m_err;
};

/**
 * @brief A lightweight, specialized version of c++23's std::expected
 *        I don't feel like putting any errors other than strings
 *        so it's only for strings lol
 * @note Attemping to make this class take a generic error type would
 *       require refactoring the wrapper Err struct in a way I don't know
 */
template <typename Some> class Result {
public:
    Result(const Some &value);
    Result(const Err &err);
    /**
     * @brief
     */
    [[nodiscard]] bool has_value() const noexcept;
    /**
     * @brief
     */
    [[nodiscard]] Some &value();
    /**
     * @brief
     */
    [[nodiscard]] Some &operator*();
    /**
     * @brief
     */
    [[nodiscard]] String &error();

    // I don't feel like implementing the monadic operations
private:
    Result(const String &err);
    std::variant<Some, String> inner;

    friend Err;
};

/*
 * Tempalate definitions in a .cpp file and template prototypes in this (.hpp)
 * file cannot be separated in a traditional sense. The .cpp file with the
 * template definitions will be compiled without knowledge of the usage of the
 * template and cannot generate the code with the types used in other .cpp
 * files.
 * To keep header and implementation files separated, one must do one of the
 * following:
 *     1. template specializations must be explictly defined (i.e. template
 *        class Result<Vec<Book>>)
 *     2. template definitions need to be directly pasted through #include into
 *        where it is used.
 * #include'ing the template definitions at the end of this .hpp file is a hacky
 * way of using the latter method. Any file that #include's this .hpp file can
 * access the template and the compiler is able to generate templated types
 * because the template definitions are technically in the .hpp file.
 */
#include "../src/Result.tpp"
