#pragma once

#include "Types.hpp"
#include <variant>

/**
 * @brief A wrapper class that enables easy explicit error making for Result
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
 *       require refactoring the wrapper Err class in a way I don't know
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

#include "../src/Result.tpp"
