#ifndef TYPES_H
#define TYPES_H

#include <concepts>

template <typename T>
concept Number = (std::integral<T> || std::floating_point<T>)
                  && !std::same_as<T, bool> && !std::same_as<T, char> 
                  && !std::same_as<T, unsigned char> && !std::same_as<T, char8_t> 
                  && !std::same_as<T, char16_t> && !std::same_as<T, char32_t> 
                  && !std::same_as<T, wchar_t>;

#endif    // TYPES_H