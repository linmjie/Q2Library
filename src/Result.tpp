#pragma once

#include <Result.hpp>
#include <variant>

Err::Err(const String &err) : m_err(err) {}

template <typename Some>
Result<Some>::Result(const Some &value) : inner(value) {}

template <typename Some>
Result<Some>::Result(const Err &err) : inner(err.m_err) {}

template <typename Some>
Result<Some>::Result(const String &err): inner(err) {}

template <typename Some> bool Result<Some>::has_value() const noexcept {
    return std::holds_alternative<Some>(this->inner);
}

template <typename Some> Some &Result<Some>::value() {
    if (this->has_value()) {
        return std::get<Some>(this->inner);
    }
    throw "There is not a value in the result";
}

template <typename Some> Some &Result<Some>::operator*() {
    return this->value();
}

template <typename Some> String &Result<Some>::error() {
    if (!this->has_value()) {
        return std::get<String>(this->inner);
    }
    throw "There is not an error in the result";
}
