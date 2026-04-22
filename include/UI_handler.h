#ifndef UI_HANDLER
#define UI_HANDLER

#include "UI_canvas.h"

namespace UI {
    class Handler {
    private:
        UI_State current_state;
        Camera2D camera;
        Canvas *canvas[static_cast<int>(UI_State::COUNT)] = {nullptr};

    public:
        /**
         * @brief Destroy the Handler object
         * 
         */
        ~Handler();

        /**
         * @brief Construct a new Hanlder object
         * 
         */
        Handler();

        /**
         * @brief Handle camera update event
         * 
         */
        void update_camera();

        /**
         * @brief run the visualizer
         * 
         */
        void run();
    };
}

#endif