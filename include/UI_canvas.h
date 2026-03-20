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
    const int MAX_INPUT_INT_CHAR = 100;
    const Color main_background_color = {255, 255, 212, 255};
    const float node_radius = 30.0f;
    const float time_between_steps = 0.4f;
    
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
         * @brief Run the current UI state
         * 
         * The default does not add anything, need to be overrided
         * 
         */
        virtual void run();

        /**
         * @brief update animation for each frame
         * 
         * The default does not do anything, need to be overred
         * 
         */
        virtual void update_animation();
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
        Rectangle prev_button;
        Rectangle next_button;
        Rectangle clear_button;
        Rectangle mode_button;
        Rectangle file_button;
        Rectangle exit_button;
        Rectangle random_button;
        Rectangle skip_button;
        Rectangle speed_button;

        Data_Structure::AVL_Tree tree;

        //Input text field
        char text_string[MAX_INPUT_INT_CHAR + 1];
        int letter_count;
        int frames_counter;

        //Animation
        
        int current_step;
        float ani_speed;
        int speed_multiplier; //Instant mode = 5
        float pause_timer; //Pause time between each step
        bool is_playing;

        //Code highlight
        OPERATION cur_operation;
        Code_Highlight insert_highlight;
        
        public:

        void setup() override;
        void run() override;
        void update_animation() override;

        /**
         * @brief Update the current position of all nodes in the tree rooted at `cur`
         * 
         * @param cur 
         * @param parent_pos 
         * @return true 
         * @return false 
         */
        bool update_node_position(Data_Structure::AVL_Tree::Node* cur);

        /**
         * @brief Sync nodes position between two contiguous histories.
         * 
         * @param old_root 
         * @param new_root 
         */
        void sync_position(Data_Structure::AVL_Tree::Node* new_root, Data_Structure::AVL_Tree::Node* new_root_parent, Data_Structure::AVL_Tree::Node* &old_root);

        /**
         * @brief Draw the tree with root `cur`
         * 
         * @param cur 
         */
        void draw_tree(Data_Structure::AVL_Tree::Node* cur);

        /**
         * @brief Handle insert operation
         * 
         * By default insert values from the input text field
         * 
         */
        void insert();

        /**
         * @brief Handle erase operation
         * 
         */
        void erase();

        /**
         * @brief Handle clear operation
         * 
         */
        void clear();
    };
}

#endif