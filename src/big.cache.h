// BIG Befunge Integer Generator
// Copyright (c) 2019 James Holderness
// Distributed under the MIT License

#pragma once

#include <unordered_map>

namespace big {

    class cache {
    public:
        using size_type = int16_t;

        struct reference {
            size_type& value;
            bool is_valid;
        };

        reference lookup(int target, bool using_quotes, int space)
        {
            auto& entries = using_quotes ? quoted_entries : unquoted_entries;
            auto& entry = entries[target];
            const auto entry_valid = entry.value != 0 && entry.space >= space;
            if (!entry_valid) entry.space = space;
            return {entry.value, entry_valid};
        }

    private:
        struct map_entry {
            size_type value;
            size_type space;
        };
        using map = std::unordered_map<int, map_entry>;

        map quoted_entries;
        map unquoted_entries;
    };

}  // namespace big
