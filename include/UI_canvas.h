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
    const int MAX_INPUT_INT_CHAR = 101;
    const Color main_background_color = {255, 255, 212, 255};
    const float node_radius = 30.0f;
    const float time_between_steps = 1.0f;
    const float ani_speed = 0.05f;
    
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
        Rectangle input_text_field_1; //For Insert, Erase and Find
        Rectangle input_text_field_2; //For update old_val
        Rectangle input_text_field_3; //For update new_val
        Rectangle insert_button;
        Rectangle erase_button;
        Rectangle find_button;
        Rectangle prev_button;
        Rectangle next_button;
        Rectangle clear_button;
        Rectangle file_button;
        Rectangle exit_button;
        Rectangle random_button;
        Rectangle skip_button;
        Rectangle speed_button;
        Rectangle update_button;

        Data_Structure::AVL_Tree tree;

        //Input text field
        char text_string_1[MAX_INPUT_INT_CHAR + 1];
        int letter_count_1;
        int frames_counter_1;

        char text_string_2[MAX_INPUT_INT_CHAR + 1];
        int letter_count_2;
        int frames_counter_2;

        char text_string_3[MAX_INPUT_INT_CHAR + 1];
        int letter_count_3;
        int frames_counter_3;

        //Animation
        
        int current_step;
        int speed_multiplier; //Instant mode = 5
        float pause_timer; //Pause time between each step
        bool is_playing;

        //Code highlight
        OPERATION current_operation;
        Code_Highlight highlighter[OPERATION::NONE];
        
        public:

        void setup() override;
        void run() override;
        void update_animation() override;

        /**
         * @brief Update the current position of all nodes in the tree rooted at `cur`
         * 
         * @param cur 
         * @return true 
         * @return false 
         */
        bool update_node_position(Data_Structure::AVL_Tree::Node* cur);

        /**
         * @brief Sync nodes position between two contiguous histories.
         * 
         * @param new_root
         * @param new_root_parent 
         * @param old_root
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
         * @brief Handle next operation
         * 
         */
        void next();
    
        /**
         * @brief Handle back operation
         * 
         */
        void prev();

        /**
         * @brief Handle clear operation
         * 
         */
        void clear();

        /**
         * @brief Handle skip operation
         * 
         */
        void skip();

        /**
         * @brief Handle update operation
         * 
         * Status: unfinished
         */
        void update();

        /**
         * @brief Handle find operation
         * 
         */
        void find();

        /**
         * @brief Handle open file operation
         * 
         */
        void open_file();
    };

    class Heap_Canvas : public Canvas {
    private:
        //Buttons
        Rectangle input_text_field;
        Rectangle insert_button;
        Rectangle erase_button;
        Rectangle find_button;
        Rectangle prev_button;
        Rectangle next_button;
        Rectangle clear_button;
        Rectangle file_button;
        Rectangle exit_button;
        Rectangle random_button;
        Rectangle skip_button;
        Rectangle speed_button;
        Rectangle update_button;

        Data_Structure::Heap heap;

        //Input text field
        char text_string[MAX_INPUT_INT_CHAR + 1];
        int letter_count;
        int frames_counter;

        //Animation
        
        int current_step;
        int speed_multiplier; //Instant mode = 5
        float pause_timer; //Pause time between each step
        bool is_playing;

        //Code highlight
        OPERATION current_operation;
        Code_Highlight highlighter[OPERATION::NONE];


    public:
        void setup() override;
        void run() override;
        void update_animation() override;

        /**
         * @brief Update the current position of all nodes in the tree rooted at `cur`
         * 
         * @param array
         * @return true 
         * @return false 
         */
        bool update_node_position(std::vector<Data_Structure::Heap::Node> &array);

        /**
         * @brief Sync nodes position between two contiguous histories.
         * 
         * @param new_array
         * @param old_array
         */
        void sync_position(std::vector<Data_Structure::Heap::Node> &new_array, std::vector<Data_Structure::Heap::Node> &old_array);

        /**
         * @brief Draw the tree with root `cur`
         * 
         * @param cur 
         */
        void draw_tree(const std::vector<Data_Structure::Heap::Node> &array);

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
         * @brief Handle next operation
         * 
         */
        void next();
    
        /**
         * @brief Handle back operation
         * 
         */
        void prev();

        /**
         * @brief Handle clear operation
         * 
         */
        void clear();

        /**
         * @brief Handle skip operation
         * 
         */
        void skip();

        /**
         * @brief Handle update operation
         * 
         * Status: unfinished
         */
        void update();

        /**
         * @brief Handle find operation
         * 
         */
        void find();

        /**
         * @brief Handle open file operation
         * 
         */
        void open_file();
    };

    class LinkedList_Canvas : public Canvas {
    private:
        //Buttons
        Rectangle input_text_field;
        Rectangle insert_button;
        Rectangle erase_button;
        Rectangle find_button;
        Rectangle prev_button;
        Rectangle next_button;
        Rectangle clear_button;
        Rectangle file_button;
        Rectangle exit_button;
        Rectangle random_button;
        Rectangle skip_button;
        Rectangle speed_button;
        Rectangle update_button;
        Rectangle head_button;
        Rectangle tail_button;

        Data_Structure::Singly_Linked_List linked_list;

        //Input text field
        char text_string[MAX_INPUT_INT_CHAR + 1];
        int letter_count;
        int frames_counter;

        //Animation
        
        int current_step;
        int speed_multiplier; //Instant mode = 5
        float pause_timer; //Pause time between each step
        bool is_playing;

        //Code highlight
        OPERATION current_operation;
        Code_Highlight highlighter[OPERATION::NONE];

        //Pop up
        bool is_popup_open;

    public:
        void setup() override;
        void run() override;
        void update_animation() override;

        /**
         * @brief Update the current position of all nodes in the Linked List
         * 
         * @param pHead
         * @return true 
         * @return false 
         */
        bool update_node_position(Data_Structure::Singly_Linked_List::Node* &pHead);

        /**
         * @brief Sync nodes position between two contiguous histories.
         * 
         * @param new_pHead
         * @param old_pHead
         */
        void sync_position(Data_Structure::Singly_Linked_List::Node* &new_pHead, Data_Structure::Singly_Linked_List::Node* &old_pHead);

        /**
         * @brief Draw the tree with root `cur`
         * 
         * @param cur 
         */
        void draw_tree(Data_Structure::Singly_Linked_List::Node* pHead);

        /**
         * @brief Handle insert operation
         * 
         * By default insert values from the input text field
         * 
         * @param insert_at_the_end
         * 
         */
        void insert(bool insert_at_the_end);

        /**
         * @brief Handle erase operation
         * 
         */
        void erase();

        /**
         * @brief Handle next operation
         * 
         */
        void next();
    
        /**
         * @brief Handle back operation
         * 
         */
        void prev();

        /**
         * @brief Handle clear operation
         * 
         */
        void clear();

        /**
         * @brief Handle skip operation
         * 
         */
        void skip();

        /**
         * @brief Handle update operation
         * 
         * Status: unfinished
         */
        void update();

        /**
         * @brief Handle find operation
         * 
         */
        void find();

        /**
         * @brief Handle open file operation
         * 
         */
        void open_file();
    };
}

#endif