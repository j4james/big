// BIG Befunge Integer Generator
// Copyright (c) 2019 James Holderness
// Distributed under the MIT License

#pragma once

#include "big.expression.h"
#include "big.operands.h"
#include "big.utils.h"

#include <array>
#include <cmath>
#include <optional>

namespace big {

    namespace detail {

        template <int exponent, typename arg_t>
        static constexpr auto nth_root(arg_t radicand) noexcept
        {
            if constexpr (exponent == 3)
                return std::cbrt(static_cast<double>(radicand));
            else if constexpr (exponent == 2)
                return std::sqrt(static_cast<double>(radicand));
            else if constexpr (exponent == 1)
                return radicand;
            else
                return std::pow(static_cast<double>(radicand), 1.0 / exponent);
        }

        template <int exponent, int div_pos = 0>
        static constexpr int estimate_root(int radicand, int divisor = 1) noexcept
        {
            const auto root = nth_root<exponent>(int64_t(radicand) * divisor);
            if constexpr (div_pos == 1)
                return int(nth_root<exponent - 1>(radicand / int(root / divisor)));
            else
                return int(std::ceil(root));
        }

        template <int exponent, int div_pos = 0>
        static constexpr int calculate_power(int base, int divisor = 1) noexcept
        {
            if constexpr (exponent == 1 && div_pos == 0)
                return base;
            else if constexpr (exponent == div_pos)
                return calculate_power<exponent, 0>(base, divisor) / divisor;
            else
                return calculate_power<exponent - 1, div_pos>(base, divisor) * base;
        }

    }  // namespace detail


    template <typename traits, int div_pos = 0>
    struct power_operation {
        static constexpr auto length = utils::symbol_length(traits::symbol) + (div_pos ? 2 : 0);
        static constexpr auto min_target = traits::min_targets[div_pos];
        static constexpr auto min_rhs = div_pos ? 2 : 1;
        static constexpr auto operand_order = smallest_first;

        static std::optional<int> calculate_lhs(int target, int rhs) noexcept
        {
            auto lhs = detail::estimate_root<traits::exponent, div_pos>(target, rhs);
            if (detail::calculate_power<traits::exponent, div_pos>(lhs, rhs) == target) return lhs;
            return {};
        }

        static void build_expression(char rhs_symbol, bool rhs_quoted, expression& container) noexcept
        {
            if constexpr (div_pos == 0)
                container.insert_suffix(traits::symbol);
            else {
                const auto symbol = std::string_view{traits::symbol};
                const auto div_offset = div_pos + traits::exponent - 2;
                container.insert_suffix(symbol.substr(div_offset));
                container.insert_suffix('/');
                container.insert_suffix(rhs_symbol);
                container.insert_suffix(symbol.substr(0, div_offset));
            }
        }
    };


    struct square_traits {
        static constexpr auto symbol = ":*";
        static constexpr auto exponent = 2;
        static constexpr auto min_targets = std::array{16, 506, 253};
    };

    template <int div_pos>
    using square_with_div = power_operation<square_traits, div_pos>;
    using square = power_operation<square_traits>;


    struct cube_traits {
        static constexpr auto symbol = "::**";
        static constexpr auto exponent = 3;
        static constexpr auto min_targets = std::array{216, 19652, 19652, 15901};
    };

    template <int div_pos>
    using cube_with_div = power_operation<cube_traits, div_pos>;
    using cube = power_operation<cube_traits>;


    struct power4_traits {
        static constexpr auto symbol = ":::***";
        static constexpr auto exponent = 4;
        static constexpr auto min_targets = std::array{0, 1935401, 0, 1257382, 0};
    };

    template <int div_pos>
    using power4_with_div = power_operation<power4_traits, div_pos>;
    using power4 = power_operation<power4_traits>;

}  // namespace big
