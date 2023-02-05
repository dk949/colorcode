#include "args.hpp"
#include "err.hpp"

#include <fmt/color.h>
#include <pcre2.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <span>
#include <string_view>
#include <utility>
#include <vector>

constexpr auto utf8_mask = 0xc0;
constexpr auto utf8_continue = 0x80;

char const *test_string =
    R"(lemon ipsum dolor sit amet, consectetur apple elit. Integer malesuada lorem at aliquet convallis. Ut porta convallis tincidunt. Aenean lemon fermentum arcu, in molestie orange Mauris purus massa, lobortis sit amet apple apple cursus a purus. Maecenas consequat orci apple leo hendrerit lemon Sed interdum libero sed nulla sodales tincidunt. lemon Apple nulla erat, a lobortis sapien sollicitudin orange Apple Vestibulum gravida aliquam ullamcorper. Proin porta magna non ex Orange cursus porta magna tristique. Donec porttitor mi vel ex Orange eget aliquet massa condimentum. Fusce a Orange vitae tellus Orange lemon sed eu ipsum. Lemon et orange at dolor venenatis posuere. Aliquam posuere orange justo sed elementum. Etiam orange augue, malesuada eu Orange a, suscipit in lemon lemon orange sed ex a faucibus."

Aliquam viverra lacus eget apple aliquam. Vestibulum finibus dignissim diam nec interdum. apple vulputate in Apple id consequat. Proin Orange dolor eget consequat consectetur, metus elit convallis ex, et porta leo mi orange mi. Phasellus at ante orange Cras vulputate, quam sed viverra lobortis, lemon libero consequat ligula, eu imperdiet quam augue vel Apple Vivamus placerat gravida lorem sit amet tincidunt. Proin vulputate bibendum nulla et vestibulum. Proin aliquet Lemon a vulputate iaculis.

Etiam dictum tortor ut Lemon congue, in Apple felis Orange orange pulvinar orange nulla nec congue. Suspendisse ut orange lemon ante facilisis pretium at apple tortor. Ut sit amet purus ornare, pretium justo Orange faucibus enim. Donec lectus urna, dapibus vel rhoncus Apple Orange Apple Orange lemon ut Lemon purus. apple volutpat venenatis semper. In hac habitasse platea dictumst. Phasellus orange ante et Orange porta, purus sem hendrerit Lemon eu tincidunt elit orci Apple urna. apple condimentum efficitur consequat. Etiam consectetur turpis vel interdum eleifend. lemon vulputate libero apple auctor facilisis. In vel lorem et ante faucibus fringilla. lemon sodales in arcu Apple tincidunt. Duis semper, tortor ac venenatis Orange Orange ante placerat Lemon ut rhoncus purus orci non ante.

Cras euismod apple Lemon purus Orange suscipit. Phasellus nunc arcu, commodo a libero Orange lemon Lemon est. Suspendisse sed magna leo. Morbi aliquam viverra metus id pharetra. Interdum et malesuada Orange lemon ante ipsum primis in faucibus. In lacinia, neque ut orange ornare, nibh lemon laoreet Lemon eget venenatis sem nunc Lemon quam. Nulla faucibus justo Apple Apple pulvinar varius. Donec accumsan orange Apple laoreet euismod. Donec fringilla ornare apple Orange gravida odio vitae Orange aliquet. Praesent laoreet, erat nec finibus lemon elit erat Orange dui, et lemon nisl urna a orci. Apple Orange nibh sit lemon Lemon posuere, ipsum Lemon ullamcorper leo, eu dapibus augue felis ac tellus. Vestibulum in orci at magna scelerisque commodo.
Apple
Ut lemon nibh ante. Mauris efficitur diam Lemon urna tristique pellentesque quis sit amet lemon Sed in Lemon orange Curabitur Orange nibh posuere, consequat erat sed, Orange ante. Morbi consectetur magna a Lemon lemon sed aliquet Apple Lemon Suspendisse sit amet quam Lemon lemon turpis ut, luctus diam. Integer orange Apple et diam Lemon aliquam ac Orange mi. Apple eu auctor tortor. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Lemon maximus Lemon vitae purus luctus sodales. Donec vel tristique diam. apple nisl lectus, blandit nec pretium non, lemon sit amet lacus.
)";

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

struct Mathcher {
private:
    pcre2_code *m_code;
    pcre2_match_data *m_data;
    std::string_view m_subj;
    BeginEnd *m_ovec;
    bool m_is_utf;
    bool m_crlf;
    bool m_first_match = true;

public:
    Mathcher(pcre2_code *code, pcre2_match_data *data, std::string_view subj)
            : m_code(code)
            , m_data(data)
            , m_subj(subj) {
        if (find_match(m_code, m_data, m_subj) == PCRE2_ERROR_NOMATCH) fmt::print("Not handled\n");
        // error<Err::INTERNAL>("Not handled");
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
        if (m_ovec->first > m_ovec->second)
            error<Err::PATTERN>("\\K was used len, to set the match start after the end");

        return m_ovec;
    }
};

int main(int argc, char **argv) {
    auto const args = Args(argc, argv);
    auto const test_string_sv = std::string_view {test_string};
    fmt::print("res = {}\n", fmt::join(args.res, ", "));

    // Make codes from regex strings
    std::vector<Code> codes;
    for (auto const &re : args.res)
        codes.emplace_back(make_code(re));

    // Reserve space for match data
    std::vector<MatchData> data;
    for (auto const &code : codes)
        data.emplace_back(make_match_data(code.get()));


    // Create matchers for each code
    std::vector<Mathcher> matchers;
    for (size_t i = 0; i < codes.size(); i++)
        matchers.emplace_back(codes[i].get(), data[i].get(), test_string_sv);

    std::vector<BeginEnd *> next_matches;
    for (auto &matcher : matchers)
        next_matches.push_back(matcher.find_next());

    auto const done = [&] {
        for (auto const *p : next_matches) {
            if (p) return false;
        }
        return true;
    };

    char const *prev_end = test_string_sv.data();
    while (!done()) {
        auto const next_match_ = std::min_element(std::begin(next_matches),
            std::end(next_matches),
            [](BeginEnd *a, BeginEnd *b) { return a && b ? a->first < b->first : bool(a); });
        size_t const match_idx = static_cast<size_t>(next_match_ - next_matches.begin());
        auto const next_match = *next_match_;

        auto strstart = test_string_sv.data() + next_match->first;
        auto len = next_match->second - next_match->first;
        std::string_view nomatch {prev_end, strstart};
        std::string_view match {strstart, len};
        fmt::print("{}", nomatch);
        fmt::print(fg(args.cols[match_idx]), "{}", match);
        prev_end = strstart + len;
        next_matches[match_idx] = matchers[match_idx].find_next();
    }
    std::string_view rest {prev_end, test_string_sv.end()};
    fmt::print("{}", rest);
}
