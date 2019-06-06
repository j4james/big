// BIG Befunge Integer Generator
// Copyright (c) 2019 James Holderness
// Distributed under the MIT License

#pragma once

#include "big.arithmetic.h"
#include "big.literals.h"
#include "big.negation.h"
#include "big.operations.h"
#include "big.powers.h"

#include <string>

namespace big {

    class generator {
    public:
        // Construct a generator with the given options. The options determine
        // the available strategies that may be used when generating a number.
        generator(const options& opts) : opts{opts}
        {
            add_strategies();
        }

        // Generate the optimal representations for the given target, passing
        // the resulting strings to the callback function. If the all_variants
        // option has been set, it will continue providing as many results as
        // it can find, until the callback returns true.
        void generate(int target, expression_callback callback)
        {
            if (opts.all_variants)
                generate_all_variants(target, callback);
            else
                callback(generate(target));
        }

        // Return the first optimal representation for the given target. If
        // the fast_results option has been set, this will be the first result
        // of minimal length. Otherwise the expression will also be optimised
        // to prioritise shorter string literals.
        std::string generate(int target)
        {
            auto best_length = std::numeric_limits<int>::max();
            auto best_result = std::string{};
            generate_all_variants(target, [&](auto result) {
                auto length = length_of_string_literals(result);
                if (length < best_length) {
                    best_length = length;
                    best_result = result;
                }
                return opts.fast_results || length == 0;
            });
            return best_result;
        }

    private:
        void generate_all_variants(int target, expression_callback callback)
        {
            const auto buffer_length = proxy.minimum_length(target);
            auto buffer = std::string(buffer_length, ' ');
            proxy.build_expression(target, expression(buffer, [&](auto result) {
                // In Befunge-98, multiple spaces are collapsed into one, so
                // any sequence of more than one space is not going to work.
                if (opts.befunge98 && result.find("  ") != std::string::npos)
                    return false;
                return callback(result);
            }));
        }

        static constexpr int length_of_string_literals(std::string_view result) noexcept
        {
            auto length = size_t{0};
            const auto full_length = result.length();
            if (full_length > 0 && result[0] == '"')
                length = result.find('"', 1) + 1;
            if (full_length > length && result[length] == '\'')
                length += 1;
            return static_cast<int>(length);
        }

        void add_strategies()
        {
            add_literal_strategy<decimals>();
            add_literal_strategy<hexadecimals>();
            add_literal_strategy<characters>();
            add_literal_strategy<safe_characters>();
            add_literal_strategy<strings>();
            add_literal_strategy<safe_strings>();

            add_operation_strategy<negation, decimals>();
            add_operation_strategy<negation, hexadecimals>();
            add_operation_strategy<negation, strings>();
            add_operation_strategy<negation, safe_strings>();

            add_operation_strategy<power4_with_div<3>, hexadecimals>();
            add_operation_strategy<power4_with_div<3>, decimals>();
            add_operation_strategy<power4_with_div<1>, hexadecimals>();
            add_operation_strategy<power4_with_div<1>, decimals>();

            add_operation_strategy<cube>();
            add_operation_strategy<cube_with_div<3>, hexadecimals>();
            add_operation_strategy<cube_with_div<3>, decimals>();
            add_operation_strategy<cube_with_div<2>, hexadecimals>();
            add_operation_strategy<cube_with_div<2>, decimals>();
            add_operation_strategy<cube_with_div<1>, hexadecimals>();
            add_operation_strategy<cube_with_div<1>, decimals>();

            add_operation_strategy<square>();
            add_operation_strategy<square_with_div<2>, hexadecimals>();
            add_operation_strategy<square_with_div<2>, decimals>();
            add_operation_strategy<square_with_div<1>, hexadecimals>();
            add_operation_strategy<square_with_div<1>, decimals>();

            add_operation_strategy<multiplication, strings>();
            add_operation_strategy<multiplication, safe_strings>();
            add_operation_strategy<multiplication, hexadecimals>();
            add_operation_strategy<multiplication, decimals>();

            add_operation_strategy<addition, strings>();
            add_operation_strategy<addition, safe_strings>();
            add_operation_strategy<addition, hexadecimals>();
            add_operation_strategy<addition, decimals>();

            add_operation_strategy<subtraction, decimals>();
            add_operation_strategy<subtraction, hexadecimals>();
            add_operation_strategy<reverse_subtraction, strings>();
            add_operation_strategy<reverse_subtraction, safe_strings>();
        }

        template <typename range>
        void add_literal_strategy()
        {
            if (range::is_active(opts))
                proxy.add_strategy<literal_strategy<range>>();
        }

        template <typename operation, typename range = literal_range<1, 1>>
        void add_operation_strategy()
        {
            if (range::is_active(opts))
                proxy.add_strategy<operation_strategy<operation, operand_range<range>>>();
        }

        options opts;
        strategy_proxy proxy;
    };

}  // namespace big
