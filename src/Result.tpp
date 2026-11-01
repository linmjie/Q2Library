#include <Result.hpp>
#include <variant>

/*
 * All functions in this file need to be inlined because #include's in multiple
 * .cpp files causes one-definition-rule (ODR) violations. The old and intuitive
 * use of inlining - telling the compiler to copy paste function code instead of
 * making function calls - is defunt in modern compilers. The purpose (and
 * modern usage) of inline here is to allow multiple definitions of these
 * functions. Since traditional template definitions in .cpp files and
 * prototypes in .hpp/.h files cannot be separated, the .hpp includes this
 * (.tpp) file at the end to smash the definitions into the header. Though this
 * fixes the issue of templates not compiling with specified types, it also
 * means the same definitions are present in multiple files whenever the
 * respective .hpp file is #include'd. This is the reason why different
 * definitions must be conjoined into one via the inline keyword.
 * Source: https://en.cppreference.com/cpp/language/inline
 */

inline Err::Err(const String &err) : m_err(err) {}

template <typename Some>
inline Result<Some>::Result(const Some &value) : inner(value) {}

template <typename Some>
inline Result<Some>::Result(const Err &err) : inner(err.m_err) {}

template <typename Some>
inline Result<Some>::Result(const String &err) : inner(err) {}

template <typename Some> inline bool Result<Some>::has_value() const noexcept {
    return std::holds_alternative<Some>(this->inner);
}

template <typename Some> inline Some &Result<Some>::value() {
    if (this->has_value()) {
        return std::get<Some>(this->inner);
    }
    throw "There is not a value in the result";
}

template <typename Some> inline Some &Result<Some>::operator*() {
    return this->value();
}

template <typename Some> inline String &Result<Some>::error() {
    if (!this->has_value()) {
        return std::get<String>(this->inner);
    }
    throw "There is not an error in the result";
}
