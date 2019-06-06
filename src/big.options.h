// BIG Befunge Integer Generator
// Copyright (c) 2019 James Holderness
// Distributed under the MIT License

#pragma once

namespace big {

    struct options {
        // Prefer faster results over shorter string literals
        bool fast_results = false;
        // Only allow numeric expressions (no strings)
        bool numeric_only = false;
        // Disallow unsafe characters: SPACE, ';' and '@'
        bool safe_only = false;
        // Return all variants matching the minimal length
        bool all_variants = false;
        // Permit the use of Befunge-98 operations
        bool befunge98 = false;
    };

}  // namespace big
