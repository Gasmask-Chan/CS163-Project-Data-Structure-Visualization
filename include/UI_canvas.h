#ifndef UI_CANVAS
#define UI_CANVAS

#include "UI_config.h"
#include "UI_element.h"

#include "avl_tree.h"
#include "singly_linked_list.h"
#include "heap.h"
#include "mst.h"
#include "trie.h"
#include "shortest_path.h"

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
    const int MAX_INPUT_INT_CHAR = 4;
    const int MAX_INPUT_CHAR = 10;
    const Color main_background_color = {255, 255, 212, 255};
    
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

    class AVL_Canvas : public Canvas {
    public:
        void run() override;
    };
}

#endif