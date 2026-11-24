#pragma once

#include "Types.hpp"
#include <variant>

/**
 * @brief A wrapper struct that enables easy explicit error making for Result
 */
template <typename ErrType = String>
struct Err_t {
    /** @brief Constructs an Err_t by copying an object */
    explicit Err_t(const ErrType &value);
    /** @brief Constructs an Err_t by moving an object */
    explicit Err_t(ErrType &&value);

    /**
     * @brief Constructs an Err_t by forwarding variadic args to the constructor
     */
    template <typename... Args>
    explicit Err_t(Args &&...args);

    ErrType m_err;
};

/** @brief Constructs an error value by copying an object */
template <typename ErrType>
[[nodiscard]] constexpr inline Err_t<ErrType> Err(const ErrType &value) {
    return Err_t(value);
}

/** @brief Constructs an error value by moving an object */
template <typename ErrType>
[[nodiscard]] constexpr inline Err_t<ErrType> Err(ErrType &&value) {
    return Err_t(std::forward<ErrType>(value));
}

/**
 * @brief Constructs an error value by forwarding variadic args to the
 * constructor
 */
template <typename ErrType, typename... Args>
[[nodiscard]] constexpr inline Err_t<ErrType> Err(Args &&...args) {
    return Err_t<ErrType>(std::forward<ErrType>(args)...);
}

/** @brief Constructs a string error with a string literal */
[[nodiscard]] inline Err_t<String> Err(const char *str_literal) {
    return Err_t(String(str_literal));
}

/**
 * @brief A wrapper struct that enables easy explicit ok type making for
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

/** @brief Constructs an ok value by copying an object */
template <typename SomeType>
[[nodiscard]] constexpr inline Ok_t<SomeType> Ok(const SomeType &value) {
    return Ok_t(value);
}

/** @brief Constructs an ok value by moving an object */
template <typename SomeType>
[[nodiscard]] constexpr inline Ok_t<SomeType> Ok(SomeType &&value) {
    return Ok_t(std::forward<SomeType>(value));
}

/**
 * @brief Constructs an ok value by forwarding variadic args to the constructor
 */
template <typename SomeType, typename... Args>
[[nodiscard]] constexpr inline Ok_t<SomeType> Ok(Args &&...args) {
    return Ok_t<SomeType>(std::forward<Args>(args)...);
}

/**
 * @brief A copy of std::expected from C++23 or the Result type from Rust: a
 * Result object contains either a regular value or an error value
 *
 * @tparam SomeType The type of a non-erroneous value
 * @tparam ErrType The type of the error value: a std::string by default
 */
template <typename SomeType, typename ErrType = String> class Result {
public:
    /** @brief Constructs a Result using a copied Err_t made using Err */
    Result(const Err_t<ErrType> &err);
    /** @brief Constructs a Result using an moved Err_t made using Err */
    Result(Err_t<ErrType> &&err);

    /** @brief Constructs a Result using a copied Ok_t made using Ok */
    Result(const Ok_t<SomeType> &ok);
    /** @brief Constructs a Result using an moved Ok_t made using Ok */
    Result(Ok_t<SomeType> &&ok);

    /** @brief Returns whether there is a non-error value in the Result */
    [[nodiscard]] bool has_value() const noexcept;

    /**
     * @brief Returns the non-error value or throws if there is an error value
     */
    [[nodiscard]] SomeType &value();

    /**
     * @brief Returns the non-error value or throws if there is an error value
     */
    [[nodiscard]] SomeType &operator*();

    /**
     * @brief Returns the error value or throws if there is a non-error value
     */
    [[nodiscard]] ErrType &error();

    // I don't feel like implementing the monadic operations
    // I don't feel like implementing rule of 5 either
private:
    /** @brief Constructs a Result by copying an ok value */
    explicit Result(const SomeType &value);
    /** @brief Constructs a Result by moving an ok value */
    explicit Result(SomeType &&value);

    /** @brief Constructs a Result by copying an error value */
    Result(const ErrType &err);
    /** @brief Constructs a Result by moving an error value */
    Result(ErrType &&err);

private:
    std::variant<SomeType, ErrType> inner;

    friend Err_t<ErrType>;
    friend Ok_t<SomeType>;
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
