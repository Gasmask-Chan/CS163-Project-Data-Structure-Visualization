#include "../include/UI_config.h"

namespace UI {
    Font main_font = {0};

    void load_font() {
        main_font = LoadFontEx("font/Montserrat-Bold.ttf", 64, 0, 0);
        SetTextureFilter(main_font.texture, TEXTURE_FILTER_BILINEAR);

        if (!main_font.texture.id) {
            std::cout << "Failed to load font!" << std::endl;
        }
    }

    void unload_font() {
        UnloadFont(main_font);
    }
}