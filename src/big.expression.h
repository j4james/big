// BIG Befunge Integer Generator
// Copyright (c) 2019 James Holderness
// Distributed under the MIT License

#pragma once

#include <functional>
#include <string>

namespace big {

    using expression_callback = std::function<bool(std::string_view)>;

    class expression {
    public:
        expression(std::string& buffer, expression_callback callback)
            : start_ptr{buffer.data()},
              prefix_ptr{start_ptr},
              suffix_ptr{start_ptr + buffer.length()},
              string_ptr{start_ptr + 1},
              callback{callback}
        {
        }

        expression(const expression& rhs)
            : start_ptr{rhs.start_ptr},
              prefix_ptr{rhs.prefix_ptr},
              suffix_ptr{rhs.suffix_ptr},
              string_ptr{rhs.string_ptr},
              callback{rhs.callback}
        {
        }

        expression(const expression&&) = delete;
        expression& operator=(const expression&) = delete;
        expression& operator=(const expression&&) = delete;
        ~expression() = default;

        int space_available() const noexcept
        {
            return static_cast<int>(suffix_ptr - string_ptr + 1);
        }

        bool is_using_quotes() const noexcept
        {
            return string_ptr > prefix_ptr + 1;
        }

        void insert_into_string(char ch) noexcept
        {
            *(string_ptr++) = ch;
        }

        void insert_prefix(char ch) noexcept
        {
            *(prefix_ptr++) = ch;
            string_ptr++;
        }

        void insert_suffix(char ch) noexcept
        {
            *(--suffix_ptr) = ch;
        }

        void insert_suffix(std::string_view s) noexcept
        {
            for (auto i = s.length(); i--;)
                insert_suffix(s[i]);
        }

        bool set_final_value(std::string_view s) const
        {
            *prefix_ptr = s[0];
            auto content_ptr = string_ptr;
            for (auto ch : s.substr(1))
                *(content_ptr++) = ch;
            return callback(start_ptr);
        }

    private:
        char* start_ptr;
        char* prefix_ptr;
        char* suffix_ptr;
        char* string_ptr;
        expression_callback callback;
    };

}  // namespace big
