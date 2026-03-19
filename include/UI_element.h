#ifndef UI_ELEMENT
#define UI_ELEMENT

#include "UI_config.h"

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
    void draw_button(Rectangle &button, const char *text, Color background_color, Color text_color, int font_size = 20);

    void draw_node(int pos_x, int pos_y, float radius, bool highlight, const char *text, int font_size = 20);

    /**
     * @brief Get the random integer in range [l..r]
     * 
     * @param l 
     * @param r 
     * @return int 
     */
    int get_random_int(int l, int r);
};

#endif