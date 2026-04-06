#include "../include/UI_element.h"

#include <random>
#include <chrono>

namespace UI {
    bool is_clicked(Rectangle button) {
        return CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    }

    void draw_button(Rectangle button, const char *text, Color background_color, Color text_color, int font_size) {
        DrawRectangle(button.x, button.y, button.width, button.height, background_color);
        DrawRectangleLinesEx(button, 1, BLACK);
        Vector2 text_size = MeasureTextEx(GetFontDefault(), text, font_size, 2);
        DrawTextEx(GetFontDefault(), text, (Vector2){button.x + (button.width - text_size.x) / 2, button.y + (button.height - text_size.y) / 2}, font_size, 2, text_color);
    }

    void draw_node(int pos_x, int pos_y, float radius, int highlight, const char *text, int font_size, Color node_color) {
        DrawCircle(pos_x, pos_y, radius, node_color);
        DrawCircleLines(pos_x, pos_y, radius, highlight == 1 ? RED : highlight == 2 ? ORANGE : BLACK);
        Vector2 text_size = MeasureTextEx(main_font, text, font_size, 2);
        DrawRing((Vector2){(float)pos_x, (float)pos_y}, radius, radius + 4, 0, 360, 36, highlight == 1 ? RED : highlight == 2 ? ORANGE : BLACK);
        DrawTextEx(main_font, text, {pos_x - text_size.x / 2, pos_y - text_size.y / 2}, font_size, 2, BLACK);
    }

    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    int get_random_int(int l, int r) {
        return std::uniform_int_distribution<int>(l, r)(rng);
    }

    void Code_Highlight::set_start_pos(Vector2 start_pos) {
        this->start_pos = start_pos;
    }

    void Code_Highlight::set_code_name(const char *code_name) {
        this->code_name = code_name;
    }

    void Code_Highlight::set_highlighted_line(int index) {
        this->highlighted_line = index;
    }

    void Code_Highlight::add(const std::string &new_line) {
        Vector2 text_size = MeasureTextEx(code_font, new_line.c_str(), font_size, 0.5f);
        height = std::max(height, text_size.y + 2 * v_padding);
        width = std::max(width, h_left_padding + text_size.x + h_right_padding);
        source_code.push_back(new_line);
    }

    void Code_Highlight::draw_code() {
        int code_name_width = MeasureText(code_name, 20);
        draw_button({start_pos.x - code_name_width - 10 * 2, start_pos.y - 40, code_name_width + 10.0f * 2, 40}, code_name, Fade(YELLOW, 0.8f), BLACK);
        DrawRectangle(start_pos.x - width, start_pos.y, width, height * source_code.size(), (Color){218, 235, 255, 255});
        DrawRectangleLinesEx({start_pos.x - width - 2.0f, start_pos.y - 2.0f, width + 2.0f, height * source_code.size() + 4.0f}, 3.0f, DARKPURPLE);

        for (int i = 0; i < (int)source_code.size(); i++) {
            auto &cur_line = source_code[i];
            Vector2 pos = {start_pos.x - width, start_pos.y + i * height};

            if (i == highlighted_line) {
                DrawRectangle(pos.x, pos.y, width, height, Fade(BLUE, 0.7f));
            }

            // DrawRectangleLinesEx({pos.x, pos.y, width, height}, 1.0f, BLACK);
            
            DrawTextEx(code_font, cur_line.c_str(), {pos.x + h_left_padding, pos.y + v_padding}, font_size, 0.75f, i == highlighted_line ? WHITE : BLACK);
        }
    }
}