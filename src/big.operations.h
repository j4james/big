// BIG Befunge Integer Generator
// Copyright (c) 2019 James Holderness
// Distributed under the MIT License

#pragma once

#include "big.strategy.h"

#include <algorithm>

namespace big {

    template <typename operation, typename operands>
    class operation_strategy : public strategy {
    public:
        operation_strategy(strategy_proxy& proxy) noexcept : proxy{proxy}
        {
        }

        bool can_represent(int target) noexcept override
        {
            return target >= operation::min_target;
        }

        int minimum_length(int target, bool using_quotes, int space) override
        {
            if (space <= operation::length) return maximum_length;
            space -= operation::length;
            using_quotes |= operands::need_quotes;
            auto length = maximum_length;
            for (const auto rhs : operands(operation::operand_order, operation::min_rhs)) {
                if (operands::can_represent(rhs)) {
                    if (const auto lhs = operation::calculate_lhs(target, rhs)) {
                        const auto lhs_length = proxy.minimum_length(*lhs, using_quotes, space);
                        length = std::min(length, lhs_length + operation::length);
                        space = std::min(space, lhs_length);
                        if (lhs_length <= 3) break;
                    }
                }
            }
            return length;
        }

        bool build_expression(int target, const expression& container) override
        {
            const auto space = container.space_available() - operation::length;
            const auto using_quotes = container.is_using_quotes() | operands::need_quotes;
            for (const auto rhs : operands(operation::operand_order, operation::min_rhs)) {
                if (operands::can_represent(rhs)) {
                    if (const auto lhs = operation::calculate_lhs(target, rhs)) {
                        if (proxy.minimum_length(*lhs, using_quotes, space) == space) {
                            const auto rhs_quoted = operands::need_quotes;
                            const auto rhs_symbol = operands::symbol_for(rhs);
                            auto rhs_expression = expression{container};
                            operation::build_expression(rhs_symbol, rhs_quoted, rhs_expression);
                            if (proxy.build_expression(*lhs, rhs_expression)) return true;
                        }
                    }
                }
            }
            return false;
        }

    private:
        strategy_proxy& proxy;
    };

}  // namespace big
