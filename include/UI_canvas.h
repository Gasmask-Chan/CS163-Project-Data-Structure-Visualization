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
        Camera2D *camera;
        UI_State *current_state;
    
    public:
        /**
         * @brief Set the `current_state` object
         * 
         * @param current_state 
         */
        void set_current_state(UI_State *current_state);

        /**
         * @brief Set the `camera` object
         * 
         * @param camera 
         */
        void set_camera(Camera2D *camera);

        /**
         * @brief Initialize all basic components
         * 
         * Need to be overrided since the default does not do anything
         * 
         */
        virtual void setup();

        /**
         * @brief Run the current UI state.
         * 
         * The default does not add anything.
         * 
         * Need to override
         * 
         */
        virtual void run();
    };

    class Menu_Canvas : public Canvas {
    private:
        //Buttons
        Rectangle avl_tree_button;
        Rectangle linked_list_button;
        Rectangle trie_button ;
        Rectangle heap_button;
        Rectangle mst_button;
        Rectangle sp_button;

        const Color button_color = {255, 255, 136, 255};

    public:
        void setup() override;

        void run() override;
    };

    class AVL_Canvas : public Canvas {
    private:
        //Buttons
        Rectangle input_text_field;
        Rectangle insert_button;
        Rectangle erase_button;
        Rectangle find_button;
        Rectangle undo_button;
        Rectangle redo_button;
        Rectangle clear_button;
        Rectangle mode_button;
        Rectangle file_button;
        Rectangle exit_button;
        Rectangle random_button;

        Data_Structure::AVL_Tree tree;

        //Input text field
        char text_string[MAX_INPUT_INT_CHAR + 1];
        int letter_count;
        int frames_counter;

    public:
        void setup() override;

        void run() override;
    };
}

#endif