#ifndef UI_CANVAS
#define UI_CANVAS

#include "UI_config.h"

enum UI_State {
    MENU,
    AVL,
    HEAP,
    MST,
    SP,
    TRIE,
    LINKED_LIST,
    COUNT
};

namespace UI {
    class Canvas {
    protected:
        UI_State *current_state;
    
    public:
        /**
         * @brief Set the `current_state` object
         * 
         * @param cur_state 
         */
        void set_current_state(UI_State *cur_state);

        /**
         * @brief Initial all basic components and run the current UI state.
         * 
         * The default does not add anything.
         * 
         * Need to override
         * 
         */
        virtual void run();
    };

    class Menu_Canvas : public Canvas {
    public:
        void run() override;
    };
}

#endif