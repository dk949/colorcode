#ifndef ARGS_HPP
#define ARGS_HPP
#include "colormap.hpp"
#include "err.hpp"

#include <fmt/color.h>
#include <fmt/core.h>
#include <project/config.hpp>

#include <optional>
#include <vector>

template<typename Size>
constexpr Size fnv_1a(char const *data) {
    Size hash = sizeof(Size) == 4 ? 0x811c9dc5 : sizeof(Size) == 8 ? 0xcbf29ce484222325 : 0;
    constexpr Size prime = sizeof(Size) == 4 ? 0x01000193 : sizeof(Size) == 8 ? 0x00000100000001B3 : 0;

    for (; *data != '\0'; data++) {
        hash ^= static_cast<Size>(*data);
        hash *= prime;
    }
    return hash;
}

struct Args {
public:
    std::vector<char const *> res;
    std::vector<fmt::color> cols;
    std::optional<char const *> input_file;

private:
    [[noreturn]] static void help() {
        (void)colorcode::project::dirs::src;
        fmt::print(
            R"({0}

Usage:
    {1} [OPTIONS] -color:regex [-color:regex...] [PATH]

    Input file can be supplied as a path, or if no path is given, stdin will be used.
    If the path begins with a dash (-) put a single dash as the preceding argument e.g.:
        {1} -red:'apple' - -path/that/starts/with/a/dash

    -color:regex        when input matches `regex`, color it with `color`
                        see `--list-colors` for a list of supported colors.

    --list-colors       print the list of supported colors and exit

    --help              print this message and exit
    --version           print version info and exit

)",
            colorcode::project::description,
            colorcode::project::name);
        std::exit(0);
    }

    [[noreturn]] static void version() {
        fmt::print("{} v{}\n", colorcode::project::name, colorcode::project::version::full);
        exit(0);
    }

    [[noreturn]] static void print_colors() {
        print_full_color_list();
        exit(0);
    }


public:
    Args(int argc, char **argv) {
        bool skip_dash = false;

        for (int i = 1; i < argc; i++) {
            switch (argv[i][0]) {
                case '-':
                    if (!skip_dash) {
                        if (argv[i][1] == 0) {
                            skip_dash = true;
                        } else if (argv[i][1] == '-') {
                            if (std::strcmp("--help", argv[i]) == 0) {
                                help();
                            } else if (std::strcmp("--version", argv[i]) == 0) {
                                version();
                            } else if (std::strcmp("--list-colors", argv[i]) == 0) {
                                print_colors();
                            } else {
                                error<Err::ARGUMENT>("Unsupported flag {}", argv[i]);
                            }
                        } else {
                            int col_pos = 0;
                            argv[i]++;
                            for (int j = 0;; j++) {
                                if (!argv[i][j])
                                    error<Err::ARGUMENT>("Expected agrument of the form `-color:regex`. See `--help`");
                                if (argv[i][j] == ':') {
                                    argv[i][j] = 0;
                                    col_pos = j + 1;
                                    break;
                                }
                            }
                            fmt::color col = str_to_color(argv[i]);
                            res.push_back(argv[i] + col_pos);
                            cols.push_back(col);
                        }
                        break;
                    }
                    [[fallthrough]];
                default:
                    if (!input_file)
                        input_file.emplace(argv[i]);
                    else
                        error<Err::ARGUMENT>("Expected only 1 input file");
            }
        }
        if (cols.size() != res.size()) error<Err::INTERNAL>("Number of colors does not match the number of REs");
        if (cols.empty()) error<Err::ARGUMENT>("Specify at least one color:regex pattern, see `--help`");
    }
};

#endif  // ARGS_HPP
