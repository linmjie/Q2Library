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

template <typename ErrType>
inline Err_t<ErrType>::Err_t(const ErrType &err) : m_err(err) {}

template <typename ErrType>
inline Err_t<ErrType>::Err_t(ErrType &&err)
    : m_err(std::forward<ErrType>(err)) {}

template <typename ErrType>
template <typename... Args>
inline Err_t<ErrType>::Err_t(Args &&...args)
    : m_err(std::forward<Args>(args)...) {}

template <typename SomeType>
inline Ok_t<SomeType>::Ok_t(const SomeType &ok) : m_ok(ok) {}

template <typename SomeType>
inline Ok_t<SomeType>::Ok_t(SomeType &&ok) : m_ok(std::forward<SomeType>(ok)) {}

template <typename SomeType>
template <typename... Args>
inline Ok_t<SomeType>::Ok_t(Args &&...args)
    : m_ok(std::forward<Args>(args)...) {}

template <typename SomeType, typename ErrType>
inline Result<SomeType, ErrType>::Result(const SomeType &value)
    : inner(value) {}

template <typename SomeType, typename ErrType>
inline Result<SomeType, ErrType>::Result(SomeType &&value)
    : inner(std::forward<SomeType>(value)) {}

template <typename SomeType, typename ErrType>
inline Result<SomeType, ErrType>::Result(const Err_t<ErrType> &err)
    : inner(err.m_err) {}

template <typename SomeType, typename ErrType>
inline Result<SomeType, ErrType>::Result(Err_t<ErrType> &&err)
    : inner(std::forward<ErrType>(err.m_err)) {}

template <typename SomeType, typename ErrType>
inline Result<SomeType, ErrType>::Result(const Ok_t<SomeType> &ok)
    : inner(ok.m_ok) {}

template <typename SomeType, typename ErrType>
inline Result<SomeType, ErrType>::Result(Ok_t<SomeType> &&ok)
    : inner(std::forward<SomeType>(ok.m_ok)) {}

template <typename SomeType, typename ErrType>
inline Result<SomeType, ErrType>::Result(const ErrType &err) : inner(err) {}

template <typename SomeType, typename ErrType>
inline Result<SomeType, ErrType>::Result(ErrType &&err)
    : inner(std::forward<ErrType>(err)) {}

template <typename SomeType, typename ErrType>
inline bool Result<SomeType, ErrType>::has_value() const noexcept {
    return std::holds_alternative<SomeType>(this->inner);
}

template <typename SomeType, typename ErrType>
inline SomeType &Result<SomeType, ErrType>::value() {
    if (this->has_value()) {
        return std::get<SomeType>(this->inner);
    }
    throw "There is not a value in the result";
}

template <typename SomeType, typename ErrType>
inline SomeType &Result<SomeType, ErrType>::operator*() {
    return this->value();
}

template <typename SomeType, typename ErrType>
inline ErrType &Result<SomeType, ErrType>::error() {
    if (!this->has_value()) {
        return std::get<ErrType>(this->inner);
    }
    throw "There is not an error in the result";
}
