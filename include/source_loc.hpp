#pragma once 
#include <filesystem>
// INFO: For whatever reason, gcc16.20 libstdc++23 std::source_location::current() is bugged on my M1
//
struct source_loc{
    constexpr source_loc(
        char const* a_file = __builtin_FILE(),
        int a_line = __builtin_LINE(),
        char const* a_func = __builtin_FUNCTION()
    )
        :file(a_file)
        ,line(a_line)
        ,func(a_func)
    {}

    char const* file;
    int line;
    char const* func;
    constexpr auto filename() const -> std::string{
        return std::filesystem::path(file).filename();
    }
};
