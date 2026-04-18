#include "../include/UI_config.h"

namespace UI {
    Font main_font = {0};
    Font code_font = {0};

    void load_font() {
        main_font = LoadFontEx("font/cmu-serif-bold.ttf", 20, 0, 0);
        SetTextureFilter(main_font.texture, TEXTURE_FILTER_BILINEAR);

        code_font = LoadFontEx("font/CONSOLA.TTF", 15, 0, 0);
        SetTextureFilter(code_font.texture, TEXTURE_FILTER_BILINEAR);

        if (!main_font.texture.id || !code_font.texture.id) {
            std::cout << "Failed to load font!" << std::endl;
        }
    }

    void unload_font() {
        UnloadFont(main_font);
        UnloadFont(code_font);
    }
}