#include "../include/UI_theme.h"

namespace UI_Theme {
    Color_Theme current_theme = Color_Theme::LIGHT;
    Node_Style current_style = Node_Style::CIRCLE;

    float node_radius = 30.0f;
    float edge_thickness = 3.0f;

    //Default
    Color bg_color = {255, 255, 212, 255};
    Color node_border = BLACK;
    Color node_fill = WHITE;
    Color text_color = BLACK;
    Color edge_color = BLACK;
    Color highlight_color = RED;

    void apply_theme() {
        if (current_theme == Color_Theme::LIGHT) {
            bg_color = (Color){255, 255, 212, 255};
            node_border = BLACK;
            node_fill = WHITE;
            text_color = BLACK;
            edge_color = BLACK;
            highlight_color = RED;
        } 
        else if (current_theme == Color_Theme::DARK) {
            bg_color = (Color){46, 52, 64, 255};
            node_border = (Color){136, 192, 208, 255};
            node_fill = (Color){67, 76, 94, 255};
            text_color = (Color){216, 222, 233, 255};
            edge_color = (Color){76, 86, 106, 255};
            highlight_color = (Color){191, 97, 106, 255};
        }
    }

    void next_theme() {
        int t = static_cast<int>(current_theme);
        current_theme = static_cast<Color_Theme>((t + 1) & 1);
        apply_theme();
    }

    void next_style() {
        if (current_style == Node_Style::CIRCLE) {
            current_style = Node_Style::RECTANGLE;
        }
        else {
            current_style = Node_Style::CIRCLE;
        }
    }
}