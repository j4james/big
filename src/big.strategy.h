// BIG Befunge Integer Generator
// Copyright (c) 2019 James Holderness
// Distributed under the MIT License

#pragma once

#include "big.cache.h"
#include "big.expression.h"

#include <algorithm>
#include <memory>
#include <type_traits>
#include <vector>

namespace big {

    constexpr auto maximum_length = 127;

    class strategy {
    public:
        strategy() = default;
        virtual ~strategy() = default;
        strategy(const strategy&) = delete;
        strategy& operator=(const strategy&) = delete;
        strategy(const strategy&&) = delete;
        strategy&& operator=(const strategy&&) = delete;

        virtual bool can_represent(int target) noexcept = 0;
        virtual int minimum_length(int target, bool using_quotes, int space) = 0;
        virtual bool build_expression(int target, const expression& container) = 0;
    };


    class strategy_proxy {
    public:
        int minimum_length(int target, bool using_quotes = false, int space = maximum_length)
        {
            auto cached_length = cached_lengths.lookup(target, using_quotes, space);
            if (cached_length.is_valid) return cached_length.value;
            auto length = maximum_length;
            for (auto& strategy : strategies) {
                if (strategy->can_represent(target)) {
                    const auto candidate_length = strategy->minimum_length(target, using_quotes, space);
                    length = std::min(length, candidate_length);
                    space = std::min(space, candidate_length);
                    if (candidate_length <= 3) break;
                }
            }
            cached_length.value = length;
            return length;
        }

        bool build_expression(int target, const expression& container)
        {
            const auto space = container.space_available();
            const auto using_quotes = container.is_using_quotes();
            for (auto& strategy : strategies)
                if (strategy->can_represent(target))
                    if (strategy->minimum_length(target, using_quotes, space) == space)
                        if (strategy->build_expression(target, container))
                            return true;
            return false;
        }

        template <typename strategy_type>
        void add_strategy()
        {
            if constexpr (std::is_constructible<strategy_type>::value)
                strategies.push_back(std::make_unique<strategy_type>());
            else
                strategies.push_back(std::make_unique<strategy_type>(*this));
        }

    private:
        using strategy_list = std::vector<std::unique_ptr<strategy>>;

        strategy_list strategies;
        cache cached_lengths;
    };

}  // namespace big
