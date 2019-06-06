// BIG Befunge Integer Generator
// Copyright (c) 2019 James Holderness
// Distributed under the MIT License

#pragma once

#include "big.expression.h"
#include "big.operands.h"

#include <limits>
#include <optional>

namespace big {

    struct negation {
        static constexpr auto length = 2;
        static constexpr auto min_target = std::numeric_limits<int>::min();
        static constexpr auto min_rhs = 0;
        static constexpr auto operand_order = smallest_first;

        static std::optional<int> calculate_lhs(int target, int rhs) noexcept
        {
            // Note that lhs and rhs are reversed for a negation op (target = rhs - lhs).
            if (rhs > target)
                return rhs - target;
            else
                return std::nullopt;
        }

        static void build_expression(char rhs_symbol, bool rhs_quoted, expression& container) noexcept
        {
            container.insert_suffix('-');
            if (rhs_quoted)
                container.insert_into_string(rhs_symbol);
            else
                container.insert_prefix(rhs_symbol);
        }
    };

}  // namespace big
