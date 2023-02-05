#ifndef ERR_HPP
#define ERR_HPP

#include <fmt/core.h>

enum struct Err { INTERNAL = -1, ARGUMENT = 1, PATTERN, MATCH, IO };

template<Err err, typename... Args>
[[noreturn]] void error(fmt::format_string<Args...> fmt, Args &&...args) {
    fmt::print(fmt, std::forward<Args>(args)...);
    std::putchar('\n');
    exit(static_cast<int>(err));
}

#endif  // ERR_HPP
