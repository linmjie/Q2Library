#pragma once

#include "Types.hpp"
#include <variant>

/**
 * @brief A wrapper struct that enables easy explicit error making for Result
 */
template <typename ErrType = String>
struct Err_t {
    explicit Err_t(const ErrType &value);
    explicit Err_t(ErrType &&value);

    template <typename... Args>
    explicit Err_t(Args &&...args);

    ErrType m_err;
};

template <typename ErrType>
[[nodiscard]] constexpr inline Err_t<ErrType> Err(const ErrType &value) {
    return Err_t(value);
}

template <typename ErrType>
[[nodiscard]] constexpr inline Err_t<ErrType> Err(ErrType &&value) {
    return Err_t(std::forward<ErrType>(value));
}

template <typename ErrType, typename... Args>
[[nodiscard]] constexpr inline Err_t<ErrType> Err(Args &&...args) {
    return Err_t<ErrType>(std::forward<ErrType>(args)...);
}

[[nodiscard]] inline Err_t<String> Err(const char *str_literal) {
    return Err_t(String(str_literal));
}

/**
 * @brief A wrapper struct that enables easy explicit good type making for
 * Result
 */
template <typename SomeType>
struct Ok_t {
    explicit Ok_t(const SomeType &value);
    explicit Ok_t(SomeType &&value);

    template <typename... Args>
    explicit Ok_t(Args &&...args);

    SomeType m_ok;
};

template <typename SomeType>
[[nodiscard]] constexpr inline Ok_t<SomeType> Ok(const SomeType &value) {
    return Ok_t(value);
}

template <typename SomeType>
[[nodiscard]] constexpr inline Ok_t<SomeType> Ok(SomeType &&value) {
    return Ok_t(std::forward<SomeType>(value));
}

template <typename SomeType, typename... Args>
[[nodiscard]] constexpr inline Ok_t<SomeType> Ok(Args &&...args) {
    return Ok_t<SomeType>(std::forward<Args>(args)...);
}

/**
 * @brief A lightweight, specialized version of c++23's std::expected
 */
template <typename SomeType, typename ErrType = String> class Result {
public:
    explicit Result(const SomeType &value);
    explicit Result(SomeType &&value);

    Result(const Err_t<ErrType> &err);
    Result(Err_t<ErrType> &&err);

    Result(const Ok_t<SomeType> &err);
    Result(Ok_t<SomeType> &&err);
    /**
     * @brief
     */
    [[nodiscard]] bool has_value() const noexcept;
    /**
     * @brief
     */
    [[nodiscard]] SomeType &value();
    /**
     * @brief
     */
    [[nodiscard]] SomeType &operator*();
    /**
     * @brief
     */
    [[nodiscard]] ErrType &error();

    // I don't feel like implementing the monadic operations
private:
    Result(const ErrType &err);
    Result(ErrType &&err);
    std::variant<SomeType, ErrType> inner;

    friend Err_t<ErrType>;
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
