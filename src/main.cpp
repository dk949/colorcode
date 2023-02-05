#include "args.hpp"
#include "err.hpp"

#include <fmt/color.h>
#include <pcre2.h>

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <span>
#include <string_view>
#include <utility>
#include <vector>

constexpr auto utf8_mask = 0xc0;
constexpr auto utf8_continue = 0x80;

using Code = std::unique_ptr<pcre2_code, decltype(pcre2_code_free) *>;
using MatchData = std::unique_ptr<pcre2_match_data, decltype(pcre2_match_data_free) *>;
using BeginEnd = std::pair<PCRE2_SIZE, PCRE2_SIZE>;
using Ovec = std::span<BeginEnd>;

Code make_code(char const *pat, std::uint32_t opts = 0) {
    int err;
    PCRE2_SIZE offs;
    auto *out = pcre2_compile(reinterpret_cast<PCRE2_SPTR>(pat), PCRE2_ZERO_TERMINATED, opts, &err, &offs, nullptr);
    if (!out) [[unlikely]] {
        // TODO: handle error
        error<Err::PATTERN>("Error in the pattern");
    }
    return Code {out, pcre2_code_free};
}

MatchData make_match_data(pcre2_code const *code) {
    return MatchData {pcre2_match_data_create_from_pattern(code, nullptr), pcre2_match_data_free};
}

int find_match(
    pcre2_code *code, pcre2_match_data *data, std::string_view subj, PCRE2_SIZE offs = 0, std::uint32_t opts = 0) {

    auto out = pcre2_match(code,                    //
        reinterpret_cast<PCRE2_SPTR>(subj.data()),  //
        subj.size(),                                //
        offs,                                       //
        opts,                                       //
        data,                                       //
        nullptr                                     //
    );
    if (out < 0 && out != PCRE2_ERROR_NOMATCH) [[unlikely]]
        error<Err::MATCH>("Match error: {}", out);

    return out;
}

Ovec get_ovec(pcre2_match_data *data) {
    auto *ptr = reinterpret_cast<BeginEnd *>(pcre2_get_ovector_pointer(data));
    auto cnt = pcre2_get_ovector_count(data);
    return Ovec {ptr, cnt};
}

BeginEnd *get_ovec_no_cap(pcre2_match_data *data) {
    return reinterpret_cast<BeginEnd *>(pcre2_get_ovector_pointer(data));
}

std::uint32_t get_code_opt(pcre2_code const *code, std::uint32_t what = PCRE2_INFO_ALLOPTIONS) {
    std::uint32_t option_bits;
    switch (what) {
        case PCRE2_INFO_FIRSTBITMAP:
        case PCRE2_INFO_JITSIZE:
        case PCRE2_INFO_NAMETABLE:
        case PCRE2_INFO_SIZE:  //
            error<Err::INTERNAL>("Unsupported `what` parameter: {:#x}", what);
    }
    switch (pcre2_pattern_info(code, what, &option_bits)) {
        case PCRE2_ERROR_NULL: error<Err::INTERNAL>("the argument code is NULL");
        case PCRE2_ERROR_BADMAGIC: error<Err::INTERNAL>("the \"magic number\" was not found");
        case PCRE2_ERROR_BADOPTION: error<Err::INTERNAL>("the value of what is invalid");
        case PCRE2_ERROR_BADMODE: error<Err::INTERNAL>("the pattern was compiled in the wrong mode");
        case PCRE2_ERROR_UNSET: error<Err::INTERNAL>("the requested information is not set");
    }
    return option_bits;
}

bool is_code_opt_set(pcre2_code *code, std::uint32_t opt, std::uint32_t where = PCRE2_INFO_ALLOPTIONS) {
    return (get_code_opt(code, where) & opt) != 0;
}

struct Matcher {
private:
    pcre2_code *m_code;
    pcre2_match_data *m_data;
    std::string_view m_subj;
    BeginEnd *m_ovec = nullptr;
    bool m_is_utf;
    bool m_crlf;
    bool m_first_match = true;

public:
    Matcher(pcre2_code *code, pcre2_match_data *data, std::string_view subj)
            : m_code(code)
            , m_data(data)
            , m_subj(subj) {
        if (find_match(m_code, m_data, m_subj) == PCRE2_ERROR_NOMATCH) return;
        m_ovec = get_ovec_no_cap(m_data);
        m_is_utf = is_code_opt_set(m_code, PCRE2_UTF);
        m_crlf = [&, nl = get_code_opt(m_code, PCRE2_INFO_NEWLINE)] {
            return nl == PCRE2_NEWLINE_ANY || nl == PCRE2_NEWLINE_CRLF || nl == PCRE2_NEWLINE_ANYCRLF;
        }();
        if (m_ovec->first > m_ovec->second) error<Err::PATTERN>("\\K was used to set the match start after the end");
    }

    BeginEnd *find_next() {
        if (m_first_match) [[unlikely]] {
            m_first_match = false;
            return m_ovec;
        }
        assert(m_ovec);
start:
        std::uint32_t opts = 0;
        auto offs = m_ovec->second;

        if (m_ovec->first == m_ovec->second) {  // Is match empty?
            if (m_ovec->first == m_subj.size()) {
                return nullptr;
            }
            opts = PCRE2_NOTEMPTY_ATSTART | PCRE2_ANCHORED;  // Try again but do not match empty string
        } else {
            auto start = pcre2_get_startchar(m_data);
            if (offs <= start) {
                if (offs >= m_subj.size()) {  // end of input
                    return nullptr;
                }
                offs = start + 1;
                if (m_is_utf) {
                    for (; offs < m_subj.size(); offs++)  // Skip untel the end of teh utf8 character
                        if ((m_subj[offs] & utf8_mask) != utf8_continue) break;
                }
            }
        }
        auto const next_match = find_match(m_code, m_data, m_subj, offs, opts);
        if (next_match == PCRE2_ERROR_NOMATCH) {
            if (opts == 0)  // All matches found
                return nullptr;
            m_ovec->second = offs + 1;
            if (m_crlf                          // If using crld
                && offs < m_subj.size() - 1     // If at CRLF
                && m_subj.data()[offs] == '\r'  //
                && m_subj.data()[offs + 1] == '\n') {
                m_ovec->second++;
            } else if (m_is_utf) {
                for (; m_ovec->second < m_subj.size(); m_ovec->second++) {
                    if ((m_subj.data()[m_ovec->second] & utf8_mask) != utf8_continue) break;
                    m_ovec->second++;
                }
            }
            goto start;  // FIXME: AAAAAAAAA
        }
        // Successful match
        if (m_ovec->first > m_ovec->second) error<Err::PATTERN>("\\K was used to set the match start after the end");

        return m_ovec;
    }
};

int main(int argc, char **argv) {
    auto const args = Args(argc, argv);
    std::istream *file = nullptr;
    if (args.input_file) {
        file = new std::ifstream(*args.input_file);
        if (!file->good()) error<Err::IO>("Could not open file `{}`", *args.input_file);
    } else {
        file = &std::cin;
    }

    // Make codes from regex strings
    std::vector<Code> codes;
    for (auto const &re : args.res)
        codes.emplace_back(make_code(re));

    // Reserve space for match data
    std::vector<MatchData> data;
    for (auto const &code : codes)
        data.emplace_back(make_match_data(code.get()));

    std::string line;
    while (std::getline(*file, line)) {
        // Create matchers for each code
        std::vector<Matcher> matchers;
        for (size_t i = 0; i < codes.size(); i++)
            matchers.emplace_back(codes[i].get(), data[i].get(), line);

        std::vector<BeginEnd *> next_matches;
        for (auto &matcher : matchers)
            next_matches.push_back(matcher.find_next());

        auto const done = [&] {
            for (auto const *p : next_matches) {
                if (p) return false;
            }
            return true;
        };

        char const *prev_end = line.data();
        while (!done()) {
            auto next_match_ = std::min_element(std::begin(next_matches),
                std::end(next_matches),
                [](BeginEnd *a, BeginEnd *b) { return a && b ? a->first < b->first : bool(a); });

            size_t const match_idx = static_cast<size_t>(next_match_ - next_matches.begin());
            auto *next_match = *next_match_;

            size_t begin = next_match->first;
            size_t end = next_match->second;
            bool need_next_match = true;
            for (size_t i = 0; auto *m : next_matches) {
                if (i != match_idx && m) {
                    if (m->first < end) {
                        end = m->first;
                        next_match->first = m->second;
                        need_next_match = false;
                    }
                }
                i++;
            }

            auto strstart = line.data() + begin;
            auto len = end - begin;
            assert(prev_end <= strstart);
            std::string_view nomatch {prev_end, strstart};
            std::string_view match {strstart, len};
            fmt::print("{}", nomatch);
            fmt::print(fg(args.cols[match_idx]), "{}", match);
            prev_end = strstart + len;
            if (need_next_match) next_matches[match_idx] = matchers[match_idx].find_next();
        }
        std::string_view rest {prev_end, line.end().base()};
        fmt::print("{}\n", rest);
    }
    if (file != &std::cin) delete file;
}
