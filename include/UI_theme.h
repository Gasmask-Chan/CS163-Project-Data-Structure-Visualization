#ifndef THEME
#define THEME

#include "UI_config.h"

namespace UI_Theme {
    enum class Color_Theme {
        LIGHT, 
        DARK
    };
    enum class Node_Style {
        CIRCLE,
        RECTANGLE
    };

    extern Color_Theme current_theme;
    extern Node_Style current_style;

    extern float node_radius;
    extern float edge_thickness;
    extern int font_size;

    extern Color bg_color;
    extern Color node_border;
    extern Color node_fill;
    extern Color text_color;
    extern Color edge_color;
    extern Color highlight_color;

    void apply_theme();

    void next_theme();

    void next_style(); 
}

#endif