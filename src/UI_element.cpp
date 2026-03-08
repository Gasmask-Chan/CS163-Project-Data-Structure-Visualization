#include "../include/UI_element.h"

namespace UI {
    bool is_clicked(Rectangle button) {
        return CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    }

    void draw_button(Rectangle &button, const char *text, Color background_color, Color text_color, int font_size) {
        DrawRectangle(button.x, button.y, button.width, button.height, background_color);
        DrawRectangleLinesEx(button, 1, BLACK);
        Vector2 text_size = MeasureTextEx(GetFontDefault(), text, font_size, 1);
        DrawText(text, button.x + (button.width - text_size.x) / 2, button.y + (button.height - text_size.y) / 2, font_size, text_color);
    }
}