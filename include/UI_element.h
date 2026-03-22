#ifndef UI_ELEMENT
#define UI_ELEMENT

#include "UI_config.h"

#include <vector>

namespace UI {
    /**
     * @brief Check mouse click on the input `Rectangle` button
     * 
     * @param button 
     * @return true 
     * @return false 
     */
    bool is_clicked(Rectangle button);

    /**
     * @brief Draw `Rectangle` button with text
     * 
     * @param button 
     * @param text 
     * @param background_color 
     * @param text_color 
     */
    void draw_button(Rectangle button, const char *text, Color background_color, Color text_color, int font_size = 20);

    void draw_node(int pos_x, int pos_y, float radius, bool highlight, const char *text, int font_size = 20);

    /**
     * @brief Get the random integer in range [l..r]
     * 
     * @param l 
     * @param r 
     * @return int 
     */
    int get_random_int(int l, int r);

    class Code_Highlight {
    private:
        const float h_left_padding = 7.0f;
        const float h_right_padding = 12.f;
        const float v_padding = 7.0f;
        const float font_size = 15.0f;

        Vector2 start_pos;
        float height = 0, width = 0;
        std::vector<std::string> source_code;
        int highlighted_line = -1;

        const char *code_name;

    public:
        /**
         * @brief Set the position of the top right corner of the source code box
         * 
         * @param start_pos 
         */
        void set_start_pos(Vector2 start_pos);

        /**
         * @brief Set the `code_name` object
         * 
         * @param code_name 
         */
        void set_code_name(const char *code_name);

        /**
         * @brief Set the index of the highlighted line
         * 
         * @param index 
         */
        void set_highlighted_line(int index);

        /**
         * @brief Add new line to the source code
         * 
         * @param new_line 
         */
        void add(const std::string &new_line);    

        /**
         * @brief Draw the source code to the screen
         * 
         */
        void draw_code();
    };
};

#endif