#include "colormap.hpp"

#include "err.hpp"

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

fmt::color str_to_color(char const *str) {
    constexpr auto hash = fnv_1a<unsigned long long>;
    switch (hash(str)) {
        case hash("alice_blue"): return fmt::color::alice_blue;                            // rgb(240,248,255)
        case hash("antique_white"): return fmt::color::antique_white;                      // rgb(250,235,215)
        case hash("aqua"): return fmt::color::aqua;                                        // rgb(0,255,255)
        case hash("aquamarine"): return fmt::color::aquamarine;                            // rgb(127,255,212)
        case hash("azure"): return fmt::color::azure;                                      // rgb(240,255,255)
        case hash("beige"): return fmt::color::beige;                                      // rgb(245,245,220)
        case hash("bisque"): return fmt::color::bisque;                                    // rgb(255,228,196)
        case hash("black"): return fmt::color::black;                                      // rgb(0,0,0)
        case hash("blanched_almond"): return fmt::color::blanched_almond;                  // rgb(255,235,205)
        case hash("blue"): return fmt::color::blue;                                        // rgb(0,0,255)
        case hash("blue_violet"): return fmt::color::blue_violet;                          // rgb(138,43,226)
        case hash("brown"): return fmt::color::brown;                                      // rgb(165,42,42)
        case hash("burly_wood"): return fmt::color::burly_wood;                            // rgb(222,184,135)
        case hash("cadet_blue"): return fmt::color::cadet_blue;                            // rgb(95,158,160)
        case hash("chartreuse"): return fmt::color::chartreuse;                            // rgb(127,255,0)
        case hash("chocolate"): return fmt::color::chocolate;                              // rgb(210,105,30)
        case hash("coral"): return fmt::color::coral;                                      // rgb(255,127,80)
        case hash("cornflower_blue"): return fmt::color::cornflower_blue;                  // rgb(100,149,237)
        case hash("cornsilk"): return fmt::color::cornsilk;                                // rgb(255,248,220)
        case hash("crimson"): return fmt::color::crimson;                                  // rgb(220,20,60)
        case hash("cyan"): return fmt::color::cyan;                                        // rgb(0,255,255)
        case hash("dark_blue"): return fmt::color::dark_blue;                              // rgb(0,0,139)
        case hash("dark_cyan"): return fmt::color::dark_cyan;                              // rgb(0,139,139)
        case hash("dark_golden_rod"): return fmt::color::dark_golden_rod;                  // rgb(184,134,11)
        case hash("dark_gray"): return fmt::color::dark_gray;                              // rgb(169,169,169)
        case hash("dark_green"): return fmt::color::dark_green;                            // rgb(0,100,0)
        case hash("dark_khaki"): return fmt::color::dark_khaki;                            // rgb(189,183,107)
        case hash("dark_magenta"): return fmt::color::dark_magenta;                        // rgb(139,0,139)
        case hash("dark_olive_green"): return fmt::color::dark_olive_green;                // rgb(85,107,47)
        case hash("dark_orange"): return fmt::color::dark_orange;                          // rgb(255,140,0)
        case hash("dark_orchid"): return fmt::color::dark_orchid;                          // rgb(153,50,204)
        case hash("dark_red"): return fmt::color::dark_red;                                // rgb(139,0,0)
        case hash("dark_salmon"): return fmt::color::dark_salmon;                          // rgb(233,150,122)
        case hash("dark_sea_green"): return fmt::color::dark_sea_green;                    // rgb(143,188,143)
        case hash("dark_slate_blue"): return fmt::color::dark_slate_blue;                  // rgb(72,61,139)
        case hash("dark_slate_gray"): return fmt::color::dark_slate_gray;                  // rgb(47,79,79)
        case hash("dark_turquoise"): return fmt::color::dark_turquoise;                    // rgb(0,206,209)
        case hash("dark_violet"): return fmt::color::dark_violet;                          // rgb(148,0,211)
        case hash("deep_pink"): return fmt::color::deep_pink;                              // rgb(255,20,147)
        case hash("deep_sky_blue"): return fmt::color::deep_sky_blue;                      // rgb(0,191,255)
        case hash("dim_gray"): return fmt::color::dim_gray;                                // rgb(105,105,105)
        case hash("dodger_blue"): return fmt::color::dodger_blue;                          // rgb(30,144,255)
        case hash("fire_brick"): return fmt::color::fire_brick;                            // rgb(178,34,34)
        case hash("floral_white"): return fmt::color::floral_white;                        // rgb(255,250,240)
        case hash("forest_green"): return fmt::color::forest_green;                        // rgb(34,139,34)
        case hash("fuchsia"): return fmt::color::fuchsia;                                  // rgb(255,0,255)
        case hash("gainsboro"): return fmt::color::gainsboro;                              // rgb(220,220,220)
        case hash("ghost_white"): return fmt::color::ghost_white;                          // rgb(248,248,255)
        case hash("gold"): return fmt::color::gold;                                        // rgb(255,215,0)
        case hash("golden_rod"): return fmt::color::golden_rod;                            // rgb(218,165,32)
        case hash("gray"): return fmt::color::gray;                                        // rgb(128,128,128)
        case hash("green"): return fmt::color::green;                                      // rgb(0,128,0)
        case hash("green_yellow"): return fmt::color::green_yellow;                        // rgb(173,255,47)
        case hash("honey_dew"): return fmt::color::honey_dew;                              // rgb(240,255,240)
        case hash("hot_pink"): return fmt::color::hot_pink;                                // rgb(255,105,180)
        case hash("indian_red"): return fmt::color::indian_red;                            // rgb(205,92,92)
        case hash("indigo"): return fmt::color::indigo;                                    // rgb(75,0,130)
        case hash("ivory"): return fmt::color::ivory;                                      // rgb(255,255,240)
        case hash("khaki"): return fmt::color::khaki;                                      // rgb(240,230,140)
        case hash("lavender"): return fmt::color::lavender;                                // rgb(230,230,250)
        case hash("lavender_blush"): return fmt::color::lavender_blush;                    // rgb(255,240,245)
        case hash("lawn_green"): return fmt::color::lawn_green;                            // rgb(124,252,0)
        case hash("lemon_chiffon"): return fmt::color::lemon_chiffon;                      // rgb(255,250,205)
        case hash("light_blue"): return fmt::color::light_blue;                            // rgb(173,216,230)
        case hash("light_coral"): return fmt::color::light_coral;                          // rgb(240,128,128)
        case hash("light_cyan"): return fmt::color::light_cyan;                            // rgb(224,255,255)
        case hash("light_golden_rod_yellow"): return fmt::color::light_golden_rod_yellow;  // rgb(250,250,210)
        case hash("light_gray"): return fmt::color::light_gray;                            // rgb(211,211,211)
        case hash("light_green"): return fmt::color::light_green;                          // rgb(144,238,144)
        case hash("light_pink"): return fmt::color::light_pink;                            // rgb(255,182,193)
        case hash("light_salmon"): return fmt::color::light_salmon;                        // rgb(255,160,122)
        case hash("light_sea_green"): return fmt::color::light_sea_green;                  // rgb(32,178,170)
        case hash("light_sky_blue"): return fmt::color::light_sky_blue;                    // rgb(135,206,250)
        case hash("light_slate_gray"): return fmt::color::light_slate_gray;                // rgb(119,136,153)
        case hash("light_steel_blue"): return fmt::color::light_steel_blue;                // rgb(176,196,222)
        case hash("light_yellow"): return fmt::color::light_yellow;                        // rgb(255,255,224)
        case hash("lime"): return fmt::color::lime;                                        // rgb(0,255,0)
        case hash("lime_green"): return fmt::color::lime_green;                            // rgb(50,205,50)
        case hash("linen"): return fmt::color::linen;                                      // rgb(250,240,230)
        case hash("magenta"): return fmt::color::magenta;                                  // rgb(255,0,255)
        case hash("maroon"): return fmt::color::maroon;                                    // rgb(128,0,0)
        case hash("medium_aquamarine"): return fmt::color::medium_aquamarine;              // rgb(102,205,170)
        case hash("medium_blue"): return fmt::color::medium_blue;                          // rgb(0,0,205)
        case hash("medium_orchid"): return fmt::color::medium_orchid;                      // rgb(186,85,211)
        case hash("medium_purple"): return fmt::color::medium_purple;                      // rgb(147,112,219)
        case hash("medium_sea_green"): return fmt::color::medium_sea_green;                // rgb(60,179,113)
        case hash("medium_slate_blue"): return fmt::color::medium_slate_blue;              // rgb(123,104,238)
        case hash("medium_spring_green"): return fmt::color::medium_spring_green;          // rgb(0,250,154)
        case hash("medium_turquoise"): return fmt::color::medium_turquoise;                // rgb(72,209,204)
        case hash("medium_violet_red"): return fmt::color::medium_violet_red;              // rgb(199,21,133)
        case hash("midnight_blue"): return fmt::color::midnight_blue;                      // rgb(25,25,112)
        case hash("mint_cream"): return fmt::color::mint_cream;                            // rgb(245,255,250)
        case hash("misty_rose"): return fmt::color::misty_rose;                            // rgb(255,228,225)
        case hash("moccasin"): return fmt::color::moccasin;                                // rgb(255,228,181)
        case hash("navajo_white"): return fmt::color::navajo_white;                        // rgb(255,222,173)
        case hash("navy"): return fmt::color::navy;                                        // rgb(0,0,128)
        case hash("old_lace"): return fmt::color::old_lace;                                // rgb(253,245,230)
        case hash("olive"): return fmt::color::olive;                                      // rgb(128,128,0)
        case hash("olive_drab"): return fmt::color::olive_drab;                            // rgb(107,142,35)
        case hash("orange"): return fmt::color::orange;                                    // rgb(255,165,0)
        case hash("orange_red"): return fmt::color::orange_red;                            // rgb(255,69,0)
        case hash("orchid"): return fmt::color::orchid;                                    // rgb(218,112,214)
        case hash("pale_golden_rod"): return fmt::color::pale_golden_rod;                  // rgb(238,232,170)
        case hash("pale_green"): return fmt::color::pale_green;                            // rgb(152,251,152)
        case hash("pale_turquoise"): return fmt::color::pale_turquoise;                    // rgb(175,238,238)
        case hash("pale_violet_red"): return fmt::color::pale_violet_red;                  // rgb(219,112,147)
        case hash("papaya_whip"): return fmt::color::papaya_whip;                          // rgb(255,239,213)
        case hash("peach_puff"): return fmt::color::peach_puff;                            // rgb(255,218,185)
        case hash("peru"): return fmt::color::peru;                                        // rgb(205,133,63)
        case hash("pink"): return fmt::color::pink;                                        // rgb(255,192,203)
        case hash("plum"): return fmt::color::plum;                                        // rgb(221,160,221)
        case hash("powder_blue"): return fmt::color::powder_blue;                          // rgb(176,224,230)
        case hash("purple"): return fmt::color::purple;                                    // rgb(128,0,128)
        case hash("rebecca_purple"): return fmt::color::rebecca_purple;                    // rgb(102,51,153)
        case hash("red"): return fmt::color::red;                                          // rgb(255,0,0)
        case hash("rosy_brown"): return fmt::color::rosy_brown;                            // rgb(188,143,143)
        case hash("royal_blue"): return fmt::color::royal_blue;                            // rgb(65,105,225)
        case hash("saddle_brown"): return fmt::color::saddle_brown;                        // rgb(139,69,19)
        case hash("salmon"): return fmt::color::salmon;                                    // rgb(250,128,114)
        case hash("sandy_brown"): return fmt::color::sandy_brown;                          // rgb(244,164,96)
        case hash("sea_green"): return fmt::color::sea_green;                              // rgb(46,139,87)
        case hash("sea_shell"): return fmt::color::sea_shell;                              // rgb(255,245,238)
        case hash("sienna"): return fmt::color::sienna;                                    // rgb(160,82,45)
        case hash("silver"): return fmt::color::silver;                                    // rgb(192,192,192)
        case hash("sky_blue"): return fmt::color::sky_blue;                                // rgb(135,206,235)
        case hash("slate_blue"): return fmt::color::slate_blue;                            // rgb(106,90,205)
        case hash("slate_gray"): return fmt::color::slate_gray;                            // rgb(112,128,144)
        case hash("snow"): return fmt::color::snow;                                        // rgb(255,250,250)
        case hash("spring_green"): return fmt::color::spring_green;                        // rgb(0,255,127)
        case hash("steel_blue"): return fmt::color::steel_blue;                            // rgb(70,130,180)
        case hash("tan"): return fmt::color::tan;                                          // rgb(210,180,140)
        case hash("teal"): return fmt::color::teal;                                        // rgb(0,128,128)
        case hash("thistle"): return fmt::color::thistle;                                  // rgb(216,191,216)
        case hash("tomato"): return fmt::color::tomato;                                    // rgb(255,99,71)
        case hash("turquoise"): return fmt::color::turquoise;                              // rgb(64,224,208)
        case hash("violet"): return fmt::color::violet;                                    // rgb(238,130,238)
        case hash("wheat"): return fmt::color::wheat;                                      // rgb(245,222,179)
        case hash("white"): return fmt::color::white;                                      // rgb(255,255,255)
        case hash("white_smoke"): return fmt::color::white_smoke;                          // rgb(245,245,245)
        case hash("yellow"): return fmt::color::yellow;                                    // rgb(255,255,0)
        case hash("yellow_green"): return fmt::color::yellow_green;                        // rgb(154,205,50)
        default: error<Err::ARGUMENT>("Unknown color `{}`", str);
    }
}

void print_full_color_list() {
    using enum fmt::color;
#define PRINT_COLOR(COLOR) fmt::print(fg(COLOR), "\t{:<25} ({:#08x})\n", #COLOR, static_cast<size_t>(COLOR))
    PRINT_COLOR(alice_blue);
    PRINT_COLOR(antique_white);
    PRINT_COLOR(aqua);
    PRINT_COLOR(aquamarine);
    PRINT_COLOR(azure);
    PRINT_COLOR(beige);
    PRINT_COLOR(bisque);
    PRINT_COLOR(black);
    PRINT_COLOR(blanched_almond);
    PRINT_COLOR(blue);
    PRINT_COLOR(blue_violet);
    PRINT_COLOR(brown);
    PRINT_COLOR(burly_wood);
    PRINT_COLOR(cadet_blue);
    PRINT_COLOR(chartreuse);
    PRINT_COLOR(chocolate);
    PRINT_COLOR(coral);
    PRINT_COLOR(cornflower_blue);
    PRINT_COLOR(cornsilk);
    PRINT_COLOR(crimson);
    PRINT_COLOR(cyan);
    PRINT_COLOR(dark_blue);
    PRINT_COLOR(dark_cyan);
    PRINT_COLOR(dark_golden_rod);
    PRINT_COLOR(dark_gray);
    PRINT_COLOR(dark_green);
    PRINT_COLOR(dark_khaki);
    PRINT_COLOR(dark_magenta);
    PRINT_COLOR(dark_olive_green);
    PRINT_COLOR(dark_orange);
    PRINT_COLOR(dark_orchid);
    PRINT_COLOR(dark_red);
    PRINT_COLOR(dark_salmon);
    PRINT_COLOR(dark_sea_green);
    PRINT_COLOR(dark_slate_blue);
    PRINT_COLOR(dark_slate_gray);
    PRINT_COLOR(dark_turquoise);
    PRINT_COLOR(dark_violet);
    PRINT_COLOR(deep_pink);
    PRINT_COLOR(deep_sky_blue);
    PRINT_COLOR(dim_gray);
    PRINT_COLOR(dodger_blue);
    PRINT_COLOR(fire_brick);
    PRINT_COLOR(floral_white);
    PRINT_COLOR(forest_green);
    PRINT_COLOR(fuchsia);
    PRINT_COLOR(gainsboro);
    PRINT_COLOR(ghost_white);
    PRINT_COLOR(gold);
    PRINT_COLOR(golden_rod);
    PRINT_COLOR(gray);
    PRINT_COLOR(green);
    PRINT_COLOR(green_yellow);
    PRINT_COLOR(honey_dew);
    PRINT_COLOR(hot_pink);
    PRINT_COLOR(indian_red);
    PRINT_COLOR(indigo);
    PRINT_COLOR(ivory);
    PRINT_COLOR(khaki);
    PRINT_COLOR(lavender);
    PRINT_COLOR(lavender_blush);
    PRINT_COLOR(lawn_green);
    PRINT_COLOR(lemon_chiffon);
    PRINT_COLOR(light_blue);
    PRINT_COLOR(light_coral);
    PRINT_COLOR(light_cyan);
    PRINT_COLOR(light_golden_rod_yellow);
    PRINT_COLOR(light_gray);
    PRINT_COLOR(light_green);
    PRINT_COLOR(light_pink);
    PRINT_COLOR(light_salmon);
    PRINT_COLOR(light_sea_green);
    PRINT_COLOR(light_sky_blue);
    PRINT_COLOR(light_slate_gray);
    PRINT_COLOR(light_steel_blue);
    PRINT_COLOR(light_yellow);
    PRINT_COLOR(lime);
    PRINT_COLOR(lime_green);
    PRINT_COLOR(linen);
    PRINT_COLOR(magenta);
    PRINT_COLOR(maroon);
    PRINT_COLOR(medium_aquamarine);
    PRINT_COLOR(medium_blue);
    PRINT_COLOR(medium_orchid);
    PRINT_COLOR(medium_purple);
    PRINT_COLOR(medium_sea_green);
    PRINT_COLOR(medium_slate_blue);
    PRINT_COLOR(medium_spring_green);
    PRINT_COLOR(medium_turquoise);
    PRINT_COLOR(medium_violet_red);
    PRINT_COLOR(midnight_blue);
    PRINT_COLOR(mint_cream);
    PRINT_COLOR(misty_rose);
    PRINT_COLOR(moccasin);
    PRINT_COLOR(navajo_white);
    PRINT_COLOR(navy);
    PRINT_COLOR(old_lace);
    PRINT_COLOR(olive);
    PRINT_COLOR(olive_drab);
    PRINT_COLOR(orange);
    PRINT_COLOR(orange_red);
    PRINT_COLOR(orchid);
    PRINT_COLOR(pale_golden_rod);
    PRINT_COLOR(pale_green);
    PRINT_COLOR(pale_turquoise);
    PRINT_COLOR(pale_violet_red);
    PRINT_COLOR(papaya_whip);
    PRINT_COLOR(peach_puff);
    PRINT_COLOR(peru);
    PRINT_COLOR(pink);
    PRINT_COLOR(plum);
    PRINT_COLOR(powder_blue);
    PRINT_COLOR(purple);
    PRINT_COLOR(rebecca_purple);
    PRINT_COLOR(red);
    PRINT_COLOR(rosy_brown);
    PRINT_COLOR(royal_blue);
    PRINT_COLOR(saddle_brown);
    PRINT_COLOR(salmon);
    PRINT_COLOR(sandy_brown);
    PRINT_COLOR(sea_green);
    PRINT_COLOR(sea_shell);
    PRINT_COLOR(sienna);
    PRINT_COLOR(silver);
    PRINT_COLOR(sky_blue);
    PRINT_COLOR(slate_blue);
    PRINT_COLOR(slate_gray);
    PRINT_COLOR(snow);
    PRINT_COLOR(spring_green);
    PRINT_COLOR(steel_blue);
    PRINT_COLOR(tan);
    PRINT_COLOR(teal);
    PRINT_COLOR(thistle);
    PRINT_COLOR(tomato);
    PRINT_COLOR(turquoise);
    PRINT_COLOR(violet);
    PRINT_COLOR(wheat);
    PRINT_COLOR(white);
    PRINT_COLOR(white_smoke);
    PRINT_COLOR(yellow);
    PRINT_COLOR(yellow_green);
#undef PRINT_COLOR
}
