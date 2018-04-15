#include "ColorUtil.h"
#include <map>
#include <sstream>
#include <cstdio>

#include <SDL2/SDL_log.h>

#define RGBA(R, G, B, A) {{{R/255.0f, G/255.0f, B/255.0f, A/255.0f}}}
#define RGB(R, G, B) RGBA(R, G, B, 0xff)

NVGcolor ColorUtil::stringToColor(const std::string &color)
{
    static std::map<std::string, NVGcolor> colorMap = {
        {"transparent", RGBA(0x00, 0x00, 0x00, 0x00) },
        {"black", RGB(0x00, 0x00, 0x00) },
        {"silver", RGB(0xc0, 0xc0, 0xc0) },
        {"gray", RGB(0x80, 0x80, 0x80) },
        {"white", RGB(0xff, 0xff, 0xff) },
        {"maroon", RGB(0x80, 0x00, 0x00) },
        {"red", RGB(0xff, 0x00, 0x00) },
        {"purple", RGB(0x80, 0x00, 0x80) },
        {"fuchsia", RGB(0xff, 0x00, 0xff) },
        {"green", RGB(0x00, 0x80, 0x00) },
        {"lime", RGB(0x00, 0xff, 0x00) },
        {"olive", RGB(0x80, 0x80, 0x00) },
        {"yellow", RGB(0xff, 0xff, 0x00) },
        {"navy", RGB(0x00, 0x00, 0x80) },
        {"blue", RGB(0x00, 0x00, 0xff) },
        {"teal", RGB(0x00, 0x80, 0x80) },
        {"aqua", RGB(0x00, 0xff, 0xff) },
        {"orange", RGB(0xff, 0xa5, 0x00) },
        {"aliceblue", RGB(0xf0, 0xf8, 0xff) },
        {"antiquewhite", RGB(0xfa, 0xeb, 0xd7) },
        {"aquamarine", RGB(0x7f, 0xff, 0xd4) },
        {"azure", RGB(0xf0, 0xff, 0xff) },
        {"beige", RGB(0xf5, 0xf5, 0xdc) },
        {"bisque", RGB(0xff, 0xe4, 0xc4) },
        {"blanchedalmond", RGB(0xff, 0xeb, 0xcd) },
        {"blueviolet", RGB(0x8a, 0x2b, 0xe2) },
        {"brown", RGB(0xa5, 0x2a, 0x2a) },
        {"burlywood", RGB(0xde, 0xb8, 0x87) },
        {"cadetblue", RGB(0x5f, 0x9e, 0xa0) },
        {"chartreuse", RGB(0x7f, 0xff, 0x00) },
        {"chocolate", RGB(0xd2, 0x69, 0x1e) },
        {"coral", RGB(0xff, 0x7f, 0x50) },
        {"cornflowerblue", RGB(0x64, 0x95, 0xed) },
        {"cornsilk", RGB(0xff, 0xf8, 0xdc) },
        {"crimson", RGB(0xdc, 0x14, 0x3c) },
        {"cyan", RGB(0x00, 0xff, 0xff) },
        {"darkblue", RGB(0x00, 0x00, 0x8b) },
        {"darkcyan", RGB(0x00, 0x8b, 0x8b) },
        {"darkgoldenrod", RGB(0xb8, 0x86, 0x0b) },
        {"darkgray", RGB(0xa9, 0xa9, 0xa9) },
        {"darkgreen", RGB(0x00, 0x64, 0x00) },
        {"darkgrey", RGB(0xa9, 0xa9, 0xa9) },
        {"darkkhaki", RGB(0xbd, 0xb7, 0x6b) },
        {"darkmagenta", RGB(0x8b, 0x00, 0x8b) },
        {"darkolivegreen", RGB(0x55, 0x6b, 0x2f) },
        {"darkorange", RGB(0xff, 0x8c, 0x00) },
        {"darkorchid", RGB(0x99, 0x32, 0xcc) },
        {"darkred", RGB(0x8b, 0x00, 0x00) },
        {"darksalmon", RGB(0xe9, 0x96, 0x7a) },
        {"darkseagreen", RGB(0x8f, 0xbc, 0x8f) },
        {"darkslateblue", RGB(0x48, 0x3d, 0x8b) },
        {"darkslategray", RGB(0x2f, 0x4f, 0x4f) },
        {"darkslategrey", RGB(0x2f, 0x4f, 0x4f) },
        {"darkturquoise", RGB(0x00, 0xce, 0xd1) },
        {"darkviolet", RGB(0x94, 0x00, 0xd3) },
        {"deeppink", RGB(0xff, 0x14, 0x93) },
        {"deepskyblue", RGB(0x00, 0xbf, 0xff) },
        {"dimgray", RGB(0x69, 0x69, 0x69) },
        {"dimgrey", RGB(0x69, 0x69, 0x69) },
        {"dodgerblue", RGB(0x1e, 0x90, 0xff) },
        {"firebrick", RGB(0xb2, 0x22, 0x22) },
        {"floralwhite", RGB(0xff, 0xfa, 0xf0) },
        {"forestgreen", RGB(0x22, 0x8b, 0x22) },
        {"gainsboro", RGB(0xdc, 0xdc, 0xdc) },
        {"ghostwhite", RGB(0xf8, 0xf8, 0xff) },
        {"gold", RGB(0xff, 0xd7, 0x00) },
        {"goldenrod", RGB(0xda, 0xa5, 0x20) },
        {"greenyellow", RGB(0xad, 0xff, 0x2f) },
        {"grey", RGB(0x80, 0x80, 0x80) },
        {"honeydew", RGB(0xf0, 0xff, 0xf0) },
        {"hotpink", RGB(0xff, 0x69, 0xb4) },
        {"indianred", RGB(0xcd, 0x5c, 0x5c) },
        {"indigo", RGB(0x4b, 0x00, 0x82) },
        {"ivory", RGB(0xff, 0xff, 0xf0) },
        {"khaki", RGB(0xf0, 0xe6, 0x8c) },
        {"lavender", RGB(0xe6, 0xe6, 0xfa) },
        {"lavenderblush", RGB(0xff, 0xf0, 0xf5) },
        {"lawngreen", RGB(0x7c, 0xfc, 0x00) },
        {"lemonchiffon", RGB(0xff, 0xfa, 0xcd) },
        {"lightblue", RGB(0xad, 0xd8, 0xe6) },
        {"lightcoral", RGB(0xf0, 0x80, 0x80) },
        {"lightcyan", RGB(0xe0, 0xff, 0xff) },
        {"lightgoldenrodyellow", RGB(0xfa, 0xfa, 0xd2) },
        {"lightgray", RGB(0xd3, 0xd3, 0xd3) },
        {"lightgreen", RGB(0x90, 0xee, 0x90) },
        {"lightgrey", RGB(0xd3, 0xd3, 0xd3) },
        {"lightpink", RGB(0xff, 0xb6, 0xc1) },
        {"lightsalmon", RGB(0xff, 0xa0, 0x7a) },
        {"lightseagreen", RGB(0x20, 0xb2, 0xaa) },
        {"lightskyblue", RGB(0x87, 0xce, 0xfa) },
        {"lightslategray", RGB(0x77, 0x88, 0x99) },
        {"lightslategrey", RGB(0x77, 0x88, 0x99) },
        {"lightsteelblue", RGB(0xb0, 0xc4, 0xde) },
        {"lightyellow", RGB(0xff, 0xff, 0xe0) },
        {"limegreen", RGB(0x32, 0xcd, 0x32) },
        {"linen", RGB(0xfa, 0xf0, 0xe6) },
        {"magenta", RGB(0xff, 0x00, 0xff) },
        {"mediumaquamarine", RGB(0x66, 0xcd, 0xaa) },
        {"mediumblue", RGB(0x00, 0x00, 0xcd) },
        {"mediumorchid", RGB(0xba, 0x55, 0xd3) },
        {"mediumpurple", RGB(0x93, 0x70, 0xdb) },
        {"mediumseagreen", RGB(0x3c, 0xb3, 0x71) },
        {"mediumslateblue", RGB(0x7b, 0x68, 0xee) },
        {"mediumspringgreen", RGB(0x00, 0xfa, 0x9a) },
        {"mediumturquoise", RGB(0x48, 0xd1, 0xcc) },
        {"mediumvioletred", RGB(0xc7, 0x15, 0x85) },
        {"midnightblue", RGB(0x19, 0x19, 0x70) },
        {"mintcream", RGB(0xf5, 0xff, 0xfa) },
        {"mistyrose", RGB(0xff, 0xe4, 0xe1) },
        {"moccasin", RGB(0xff, 0xe4, 0xb5) },
        {"navajowhite", RGB(0xff, 0xde, 0xad) },
        {"oldlace", RGB(0xfd, 0xf5, 0xe6) },
        {"olivedrab", RGB(0x6b, 0x8e, 0x23) },
        {"orangered", RGB(0xff, 0x45, 0x00) },
        {"orchid", RGB(0xda, 0x70, 0xd6) },
        {"palegoldenrod", RGB(0xee, 0xe8, 0xaa) },
        {"palegreen", RGB(0x98, 0xfb, 0x98) },
        {"paleturquoise", RGB(0xaf, 0xee, 0xee) },
        {"palevioletred", RGB(0xdb, 0x70, 0x93) },
        {"papayawhip", RGB(0xff, 0xef, 0xd5) },
        {"peachpuff", RGB(0xff, 0xda, 0xb9) },
        {"peru", RGB(0xcd, 0x85, 0x3f) },
        {"pink", RGB(0xff, 0xc0, 0xcb) },
        {"plum", RGB(0xdd, 0xa0, 0xdd) },
        {"powderblue", RGB(0xb0, 0xe0, 0xe6) },
        {"rosybrown", RGB(0xbc, 0x8f, 0x8f) },
        {"royalblue", RGB(0x41, 0x69, 0xe1) },
        {"saddlebrown", RGB(0x8b, 0x45, 0x13) },
        {"salmon", RGB(0xfa, 0x80, 0x72) },
        {"sandybrown", RGB(0xf4, 0xa4, 0x60) },
        {"seagreen", RGB(0x2e, 0x8b, 0x57) },
        {"seashell", RGB(0xff, 0xf5, 0xee) },
        {"sienna", RGB(0xa0, 0x52, 0x2d) },
        {"skyblue", RGB(0x87, 0xce, 0xeb) },
        {"slateblue", RGB(0x6a, 0x5a, 0xcd) },
        {"slategray", RGB(0x70, 0x80, 0x90) },
        {"slategrey", RGB(0x70, 0x80, 0x90) },
        {"snow", RGB(0xff, 0xfa, 0xfa) },
        {"springgreen", RGB(0x00, 0xff, 0x7f) },
        {"steelblue", RGB(0x46, 0x82, 0xb4) },
        {"tan", RGB(0xd2, 0xb4, 0x8c) },
        {"thistle", RGB(0xd8, 0xbf, 0xd8) },
        {"tomato", RGB(0xff, 0x63, 0x47) },
        {"turquoise", RGB(0x40, 0xe0, 0xd0) },
        {"violet", RGB(0xee, 0x82, 0xee) },
        {"wheat", RGB(0xf5, 0xde, 0xb3) },
        {"whitesmoke", RGB(0xf5, 0xf5, 0xf5) },
        {"yellowgreen", RGB(0x9a, 0xcd, 0x32) },
        {"rebeccapurple", RGB(0x66, 0x33, 0x99) },
    };

    NVGcolor result = RGB(0,0,0); // default is black.

    const std::string rgb = "rgb";
    const std::string rgba = "rgba";

    if(color[0] == '#')
    {
        std::string r = "0x";
        std::string g = "0x";
        std::string b = "0x";
        std::string a = "0x";

        switch(color.length())
        {
        case 4: // example: #RGB
            r += color[1];
            g += color[2];
            b += color[3];
            a += "f";
            break;
        case 5: // example: #RGBA
            r += color[1];
            g += color[2];
            b += color[3];
            a += color[4];
            break;
        case 7: // example: #RRGGBB
            r += color[1] + color[2];
            g += color[3] + color[4];
            b += color[5] + color[6];
            a += "ff";
            break;
        case 9: // example: #RRGGBBAA
            r += color[1] + color[2];
            g += color[3] + color[4];
            b += color[5] + color[6];
            a += color[7] + color[8];
            break;
        }

        std::stringstream ss;
        ss << std::hex << r;
        ss >> result.r;
        ss.clear();

        ss << std::hex << g;
        ss >> result.g;
        ss.clear();

        ss << std::hex << b;
        ss >> result.b;
        ss.clear();

        ss << std::hex << a;
        ss >> result.a;
        ss.clear();

        switch(color.length())
        {
        case 4: // example: #RGB
        case 5: // example: #RGBA
            result.r /= 15.0f;
            result.g /= 15.0f;
            result.b /= 15.0f;
            result.a /= 15.0f;
            break;
        case 7: // example: #RRGGBB
        case 9: // example: #RRGGBBAA
            result.r /= 255.0f;
            result.g /= 255.0f;
            result.b /= 255.0f;
            result.a /= 255.0f;
            break;
        }

    }
    else if (color.compare(0, rgb.size(), rgb) == 0 ||
             color.compare(0, rgba.size(), rgba) == 0 )
    {
        // TODO this parsing section really sucks and should be redone because
        // it needs to support all of these:
        //
        //  Functional syntax
        // -------------------
        // rgb(255,0,153)
        // rgb(255, 0, 153)
        // rgb(100%,0%,60%)
        // rgb(100%, 0%, 60%)
        //
        //  Functional syntax with alpha
        // ------------------------------
        // rgb(255, 0, 153, 1)
        // rgb(255, 0, 153, 100%)
        //
        //  Whitespace syntax
        // -------------------
        // rgb(255 0 153 / 1)
        // rgb(255 0 153 / 100%)
        //
        //  Functional syntax with floats value
        // -------------------------------------
        // rgb(255, 0, 153.6, 1)
        // rgb(1e2, .5e1, .5e0, +.25e2%)


        std::string values;
        values = color.substr((color[3] == '(' ? 4 : 5));
        values = values.substr(0, values.length()-1);

        std::istringstream ss(values);
        std::string token;

        size_t index = 0;
        size_t found;
        while(std::getline(ss, token, ',')) {
            found=token.find('.');
            if (found!=std::string::npos)
            {
                result.rgba[index++] = stof(token);
            }
            else
            {
                result.rgba[index++] = (stoi(token) / 255.0f);
            }
        }
    }
    else
    {
        auto it = colorMap.find(color);
        if (it != colorMap.end())
        {
            result = it->second;
        }
    }

    return result;
}
