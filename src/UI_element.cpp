#include "../include/UI_element.h"

#include <random>

namespace UI {
    bool is_clicked(Rectangle button) {
        return CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    }

    void draw_button(Rectangle &button, const char *text, Color background_color, Color text_color, int font_size) {
        DrawRectangle(button.x, button.y, button.width, button.height, background_color);
        DrawRectangleLinesEx(button, 1, BLACK);
        Vector2 text_size = MeasureTextEx(GetFontDefault(), text, font_size, 2);
        DrawTextEx(GetFontDefault(), text, (Vector2){button.x + (button.width - text_size.x) / 2, button.y + (button.height - text_size.y) / 2}, font_size, 2, text_color);
    }

    void draw_node(int pos_x, int pos_y, float radius, bool highlight, const char *text, int font_size) {
        DrawCircle(pos_x, pos_y, radius, WHITE);
        DrawCircleLines(pos_x, pos_y, radius, highlight ? RED : BLACK);
        Vector2 text_size = MeasureTextEx(main_font, text, font_size, 2);
        DrawRing((Vector2){pos_x, pos_y}, radius, radius + 4, 0, 360, 36, highlight ? RED : BLACK);
        DrawTextEx(main_font, text, {pos_x - text_size.x / 2, pos_y - text_size.y / 2}, font_size, 2, BLACK);
    }

    std::mt19937 rng(727);
    int get_random_int(int l, int r) {
        return std::uniform_int_distribution<int>(l, r)(rng);
    }
}