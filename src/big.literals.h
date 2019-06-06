// BIG Befunge Integer Generator
// Copyright (c) 2019 James Holderness
// Distributed under the MIT License

#pragma once

#include "big.options.h"
#include "big.strategy.h"
#include "big.utils.h"

#include <string>

namespace big {

    template <typename range>
    class literal_strategy : public strategy {
    public:
        bool can_represent(int target) noexcept override
        {
            const auto in_range = target >= range::min_value && target <= range::max_value;
            return in_range && range::disallowed_set::contains(target) == false;
        }

        int minimum_length(int target, bool using_quotes, int space) noexcept override
        {
            if (not_double_quoted && using_quotes)
                return base_length + 2;
            else
                return base_length;
        }

        bool build_expression(int target, const expression& container) override
        {
            auto value = std::string{};
            if (not_double_quoted && container.is_using_quotes()) value += "\"\"";
            if constexpr (range::prefix > 0) value += range::prefix;
            value += range::symbol_for(target);
            if constexpr (range::suffix > 0) value += range::suffix;
            return container.set_final_value(value);
        }

    private:
        static constexpr auto not_double_quoted = range::prefix != '"';
        static constexpr auto base_length = (range::prefix > 0) + 1 + (range::suffix > 0);
    };


    template <int min, int max, char base_character = '0' + min>
    struct literal_range {
        using disallowed_set = utils::const_set<>;
        static constexpr auto min_value = min;
        static constexpr auto max_value = max;
        static constexpr auto prefix = '\0';
        static constexpr auto suffix = '\0';

        static constexpr char symbol_for(int value) noexcept
        {
            return value - min_value + base_character;
        }

        static bool is_active(const options& opts) noexcept
        {
            return true;
        }
    };


    struct decimals : literal_range<0, 9> {
        static bool is_active(const options& opts) noexcept
        {
            return true;
        }
    };

    struct hexadecimals : literal_range<10, 15, 'a'> {
        static bool is_active(const options& opts) noexcept
        {
            return opts.befunge98;
        }
    };

    struct characters : literal_range<32, 126, ' '> {
        static constexpr auto prefix = '\'';
        static bool is_active(const options& opts) noexcept
        {
            return opts.befunge98 && !opts.numeric_only && !opts.safe_only;
        }
    };

    struct safe_characters : characters {
        using disallowed_set = utils::const_set<' ', ';', '@'>;
        static bool is_active(const options& opts) noexcept
        {
            return opts.befunge98 && !opts.numeric_only && opts.safe_only;
        }
    };

    struct strings : literal_range<32, 126, ' '> {
        using disallowed_set = utils::const_set<'"'>;
        static constexpr auto prefix = '"';
        static constexpr auto suffix = '"';
        static bool is_active(const options& opts) noexcept
        {
            return !opts.numeric_only && !opts.safe_only;
        }
    };

    struct safe_strings : strings {
        using disallowed_set = utils::const_set<'"', ' ', ';', '@'>;
        static bool is_active(const options& opts) noexcept
        {
            return !opts.numeric_only && opts.safe_only;
        }
    };

}  // namespace big
