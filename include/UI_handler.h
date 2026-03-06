#ifndef UI_HANDLER
#define UI_HANDLER

#include "UI_canvas.h"

namespace UI {
    class Handler {
    private:
        UI_State current_state;
        Canvas *canvas[static_cast<int>(UI_State::COUNT)] = {nullptr};

    public:
        /**
         * @brief Construct a new Hanlder object
         * 
         */
        Handler();

        /**
         * @brief run the visualizer
         * 
         */
        void run();
    };
}

#endif