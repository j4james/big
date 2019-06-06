// BIG Befunge Integer Generator
// Copyright (c) 2019 James Holderness
// Distributed under the MIT License

#pragma once

#include <algorithm>

namespace big {

    enum operand_order {
        largest_first,
        smallest_first
    };

    class operand_iterator {
    public:
        operand_iterator(int n, int increment) noexcept
            : n{n}, increment{increment}
        {
        }

        bool operator!=(operand_iterator rhs) const noexcept
        {
            return n != rhs.n;
        }

        operand_iterator& operator++() noexcept
        {
            n += increment;
            return *this;
        }

        int operator*() const noexcept { return n; }

    private:
        int n, increment;
    };

    template <typename range>
    class operand_range {
    public:
        operand_range(operand_order order, int min_value) noexcept
        {
            if (order == smallest_first) {
                first = std::max(range::min_value, min_value);
                last = range::max_value + 1;
                increment = 1;
            } else {
                first = range::max_value;
                last = std::max(range::min_value, min_value) - 1;
                increment = -1;
            }
        }

        operand_iterator begin() const noexcept { return {first, increment}; }
        operand_iterator end() const noexcept { return {last, increment}; }

        static constexpr bool can_represent(int n) noexcept
        {
            return range::disallowed_set::contains(n) == false;
        }

        static constexpr char symbol_for(int n) noexcept
        {
            return range::symbol_for(n);
        }

        static constexpr auto need_quotes = (range::prefix == '"' && range::suffix == '"');

    private:
        int first, last, increment;
    };

}  // namespace big
