// BIG Befunge Integer Generator
// Copyright (c) 2019 James Holderness
// Distributed under the MIT License

#pragma once

#include "big.expression.h"
#include "big.operands.h"
#include "big.utils.h"

#include <optional>

namespace big {

    template <typename traits>
    struct arithmetic_operation {
        static constexpr auto length = utils::symbol_length(traits::symbol) + 1;
        static constexpr auto min_target = 10;
        static constexpr auto min_rhs = traits::min_rhs;
        static constexpr auto operand_order = traits::operand_order;

        static std::optional<int> calculate_lhs(int target, int rhs) noexcept
        {
            return traits::calculate_lhs(target, rhs);
        }

        static void build_expression(char rhs_symbol, bool rhs_quoted, expression& container) noexcept
        {
            container.insert_suffix(traits::symbol);
            if (rhs_quoted)
                container.insert_into_string(rhs_symbol);
            else
                container.insert_suffix(rhs_symbol);
        }
    };


    using multiplication = arithmetic_operation<struct multiplication_traits>;
    struct multiplication_traits {
        static constexpr auto symbol = '*';
        static constexpr auto min_rhs = 2;
        static constexpr auto operand_order = largest_first;
        static std::optional<int> calculate_lhs(int target, int rhs) noexcept
        {
            if (target % rhs == 0)
                return target / rhs;
            else
                return std::nullopt;
        }
    };


    using addition = arithmetic_operation<struct addition_traits>;
    struct addition_traits {
        static constexpr auto symbol = '+';
        static constexpr auto min_rhs = 1;
        static constexpr auto operand_order = largest_first;
        static std::optional<int> calculate_lhs(int target, int rhs) noexcept
        {
            if (target > rhs)
                return target - rhs;
            else
                return std::nullopt;
        }
    };


    using subtraction = arithmetic_operation<struct subtraction_traits>;
    struct subtraction_traits {
        static constexpr auto symbol = '-';
        static constexpr auto min_rhs = 1;
        static constexpr auto operand_order = smallest_first;
        static std::optional<int> calculate_lhs(int target, int rhs) noexcept
        {
            if (target <= 0x7FFFFFFF - rhs)
                return target + rhs;
            else
                return std::nullopt;
        }
    };


    using reverse_subtraction = arithmetic_operation<struct reverse_subtraction_traits>;
    struct reverse_subtraction_traits : subtraction_traits {
        static constexpr auto symbol = "\\-";
    };

}  // namespace big
