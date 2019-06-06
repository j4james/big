// BIG Befunge Integer Generator
// Copyright (c) 2019 James Holderness
// Distributed under the MIT License

#include "big.h"

#include <iomanip>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace std;

using range_list = vector<pair<int, int>>;

[[noreturn]] void display_help()
{
    cout << "Befunge Integer Generator" << endl;
    cout << endl;
    cout << "Usage:" << endl;
    cout << "  big [options...] <number>" << endl;
    cout << "  big [options...] <number>..<number>" << endl;
    cout << endl;
    cout << "Options:" << endl;
    cout << "  -h, --help      Display this help" << endl;
    cout << "  -f, --fast      Prefer faster results over shorter string literals" << endl;
    cout << "  -n, --numeric   Only allow numeric expressions (no strings)" << endl;
    cout << "  -s, --safe      Disallow unsafe characters: SPACE, ';' and '@'" << endl;
    cout << "  -a, --all       Show all variants matching the minimal length" << endl;
    cout << "      --93        Befunge-93 compatible expressions (default)" << endl;
    cout << "      --98        Befunge-98 compatible expressions" << endl;
    exit(0);
}

[[noreturn]] void error(const string& msg)
{
    cout << "big: " << msg << endl;
    cout << "big: try 'big --help' for more information" << endl;
    exit(-1);
}

auto parse_number(const string& arg)
{
    try {
        auto used_length = size_t{0};
        const auto is_hex = arg[arg.length() - 1] == 'h';
        const auto value = stoi(arg, &used_length, is_hex ? 16 : 10);
        const auto expected_length = arg.length() - is_hex;
        if (used_length != expected_length) throw invalid_argument("");
        return value;
    } catch (...) {
        error(arg + " is not a valid number");
    }
}

void parse_arg(const string& arg, range_list& ranges, big::options& opts)
{
    static const auto option_pattern = regex{R"((?:[-/]([a-z])|(?:--|/)([a-z0-9]+)))"};
    auto option_match = smatch{};
    if (regex_match(arg, option_match, option_pattern)) {
        const auto opt = option_match.length(1) ? option_match[1] : option_match[2];
        if (opt == "help" || opt == "h")
            display_help();
        else if (opt == "fast" || opt == "f")
            opts.fast_results = true;
        else if (opt == "numeric" || opt == "n")
            opts.numeric_only = true;
        else if (opt == "safe" || opt == "s")
            opts.safe_only = true;
        else if (opt == "all" || opt == "a")
            opts.all_variants = true;
        else if (opt == "93")
            opts.befunge98 = false;
        else if (opt == "98")
            opts.befunge98 = true;
        else
            error(arg + " is not a valid option");
    } else {
        static const auto range_pattern = regex{R"(([^\.]+)(?:\.\.([^\.]+))?)"};
        auto range_match = smatch{};
        if (!regex_match(arg, range_match, range_pattern))
            error(arg + " is not a valid number");
        else {
            auto first = parse_number(range_match[1]);
            auto last = range_match.length(2) ? parse_number(range_match[2]) : first;
            ranges.emplace_back(first, last);
        }
    }
}

int main(int argc, const char* const argv[])
{
    auto ranges = range_list{};
    auto opts = big::options{};
    for (auto i = 1; i < argc; i++)
        parse_arg(argv[i], ranges, opts);

    if (argc <= 1)
        display_help();
    if (!ranges.size())
        error("no number specified");

    auto generator = big::generator{opts};
    const auto multiple_targets = ranges.size() > 1 || ranges[0].first != ranges[0].second;
    for (auto range : ranges) {
        const auto inc = range.first > range.second ? -1 : 1;
        for (auto target = range.first; target != range.second + inc; target += inc) {
            generator.generate(target, [=](auto result) {
                if (multiple_targets)
                    cout << left << setw(16) << target;
                cout << result << endl;
                return false;
            });
        }
    }

    return 0;
}
