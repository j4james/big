// BIG Befunge Integer Generator
// Copyright (c) 2019 James Holderness
// Distributed under the MIT License

#pragma once

namespace big::utils {

    static constexpr int symbol_length(char c)
    {
        return 1;
    }

    static constexpr int symbol_length(std::string_view s)
    {
        return static_cast<int>(s.length());
    }

    template <int... values>
    struct const_set;

    template <>
    struct const_set<> {
        static constexpr bool contains(int n) { return false; }
    };

    template <int value, int... more_values>
    struct const_set<value, more_values...> {
        static constexpr bool contains(int n)
        {
            return value == n || const_set<more_values...>::contains(n);
        }
    };

}  // namespace big::utils
