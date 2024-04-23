#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H

#include <binary_search_tree.h>

template<
    typename tkey,
    typename tvalue>
class red_black_tree final:
    public binary_search_tree<tkey, tvalue>
{

public:
    
    enum class node_color
    {
        RED,
        BLACK
    };

private:
    
    struct node final:
        binary_search_tree<tkey, tvalue>::node
    {
        
    public:
    
        node_color color;
    
    public:
    
        explicit node(
            tkey const &key,
            tvalue const &value);
                
    
        explicit node(
            tkey const &key,
            tvalue &&value);
        
    };

public:
    
    struct iterator_data final:
        public binary_search_tree<tkey, tvalue>::iterator_data
    {
    
        friend void red_black_tree<tkey, tvalue>::inject_additional_data(
           typename binary_search_tree<tkey, tvalue>::iterator_data *,
           typename binary_search_tree<tkey, tvalue>::node *) const;
    
    private:
        
        node_color _color;
    
    public:
    
        node_color get_color() const;
    
    public:
        
        iterator_data();
    
        explicit iterator_data(
            unsigned int depth,
            tkey const &key,
            tvalue const &value,
            node_color color);
    
        explicit iterator_data(
            unsigned int depth,
            node **src_node);
    
        iterator_data(
            iterator_data const &other);
        
        iterator_data(
            iterator_data &&other) noexcept;
        
        iterator_data &operator=(
            iterator_data const &other);
        
        iterator_data &operator=(
            iterator_data &&other) noexcept;
        
        ~iterator_data() noexcept final;
    
    };

private:
    
    class insertion_template_method final:
        public binary_search_tree<tkey, tvalue>::insertion_template_method
    {
    
    public:
        
        explicit insertion_template_method(
            red_black_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);
    
    private:
        
        void balance(
            std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path,
            typename binary_search_tree<tkey, tvalue>::node* node_to_dispose = nullptr) override;
    
    private:
    
        inline std::string get_typename() const noexcept override;
    
    };
    
    class disposal_template_method final:
        public binary_search_tree<tkey, tvalue>::disposal_template_method
    {
    
    public:
        
        explicit disposal_template_method(
            red_black_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);
        
    private:
    
        void balance(
            std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path,
            typename binary_search_tree<tkey, tvalue>::node* node_to_dispose) override;
        
        void call_small_rotation(
            typename binary_search_tree<tkey, tvalue>::node** node_to_rotate,
            void *root,
            bool is_right) const;
        
        void call_big_rotation(
            typename binary_search_tree<tkey, tvalue>::node** node_to_rotate,
            void *root,
            bool is_right) const;
    
    private:
    
        inline std::string get_typename() const noexcept override;
    
    };

public:
    
    explicit red_black_tree(
        std::function<int(tkey const &, tkey const &)> comparer = associative_container<tkey, tvalue>::default_key_comparer(),
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy =
                binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy =
                binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:
    
    ~red_black_tree() noexcept final;
    
    red_black_tree(
        red_black_tree<tkey, tvalue> const &other);
    
    red_black_tree<tkey, tvalue> &operator=(
        red_black_tree<tkey, tvalue> const &other);
    
    red_black_tree(
        red_black_tree<tkey, tvalue> &&other) noexcept;
    
    red_black_tree<tkey, tvalue> &operator=(
        red_black_tree<tkey, tvalue> &&other) noexcept;

private:

    inline size_t get_node_size() const noexcept override;

    inline void call_node_constructor(
        typename binary_search_tree<tkey,tvalue>::node *raw_space,
        tkey const &key,
        tvalue const &value) override;
    
    inline void call_node_constructor(
        typename binary_search_tree<tkey,tvalue>::node *raw_space,
        tkey const &key,
        tvalue &&value) override;

    void inject_additional_data(
        typename binary_search_tree<tkey,tvalue>::iterator_data *destination,
        typename binary_search_tree<tkey,tvalue>::node *source) const override;

    typename binary_search_tree<tkey,tvalue>::iterator_data *create_iterator_data() const override;

    typename binary_search_tree<tkey,tvalue>::iterator_data *create_iterator_data(
        unsigned int depth,
        typename binary_search_tree<tkey,tvalue>::node *&src_node) const override;

    static inline node_color get_color(
        typename binary_search_tree<tkey,tvalue>::node* node) noexcept;
    
    static inline void set_color(
        typename binary_search_tree<tkey,tvalue>::node* node,
        node_color color) noexcept;

private:

    inline std::string get_typename() const noexcept override;

};


#pragma region node implementation

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue const &value):
        binary_search_tree<tkey, tvalue>::node(key, value),
        color(node_color::RED)
{ }

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue &&value):
        binary_search_tree<tkey, tvalue>::node(key, std::move(value)),
        color(node_color::RED)
{ }

#pragma endregion node implementation

#pragma region iterator_data implementation

template<
    typename tkey,
    typename tvalue>
typename red_black_tree<tkey, tvalue>::node_color red_black_tree<tkey, tvalue>::iterator_data::get_color() const
{
    if (this->_is_state_initialized)
    {
        return _color;
    }
    
    throw std::logic_error("Tried to read from uninitialized iterator data");
}


template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data():
    binary_search_tree<tkey, tvalue>::iterator_data(),
    _color(node_color::RED)
{ }

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value,
    node_color color):
        binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value),
        _color(color)
{ }

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    node **src_node):
        binary_search_tree<tkey, tvalue>::iterator_data(depth, reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node**>(src_node))
{
    if (*src_node != nullptr)
    {
        _color = (*src_node)->color;
    }
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(
    iterator_data const &other):
        binary_search_tree<tkey, tvalue>::iterator_data(other),
        _color(other._color)
{ }

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(
    iterator_data &&other) noexcept:
        binary_search_tree<tkey, tvalue>::iterator_data(std::move(other)),
        _color(std::move(other._color))
{ }

template<
    typename tkey,
    typename tvalue>
typename red_black_tree<tkey, tvalue>::iterator_data &red_black_tree<tkey, tvalue>::iterator_data::operator=(
    iterator_data const &other)
{
    if (this != &other)
    {
        binary_search_tree<tkey, tvalue>::iterator_data::operator=(other);
        _color = other._color;
    }
    
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename red_black_tree<tkey, tvalue>::iterator_data &red_black_tree<tkey, tvalue>::iterator_data::operator=(
    iterator_data &&other) noexcept
{
    if (this != &other)
    {
        binary_search_tree<tkey, tvalue>::iterator_data::operator=(std::move(other));
        _color = std::move(other._color);
    }
    
    return *this;
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::~iterator_data() noexcept
{ }

#pragma endregion iterator_data implementation

#pragma region template methods implementation

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    red_black_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
        binary_search_tree<tkey, tvalue>::insertion_template_method(tree, insertion_strategy)
{ }

template<
    typename tkey,
    typename tvalue>
void red_black_tree<tkey, tvalue>::insertion_template_method::balance(
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path,
    typename binary_search_tree<tkey, tvalue>::node* node_to_dispose)
{
    this->trace_with_guard(get_typename() + "::insertion_template_method::balance(std::stack<node**>) : called.")
        ->debug_with_guard(get_typename() + "::insertion_template_method::balance(std::stack<node**>) : called.");
    
    while(path.size() >= 3)
    {
        typename binary_search_tree<tkey, tvalue>::node **current = path.top();
        path.pop();
        typename binary_search_tree<tkey, tvalue>::node **parent = path.top();
        path.pop();
        typename binary_search_tree<tkey, tvalue>::node **grandparent = path.top();
        typename binary_search_tree<tkey, tvalue>::node **uncle = (*grandparent)->left_subtree == *parent
                ? &((*grandparent)->right_subtree)
                : &((*grandparent)->left_subtree);
        
        if (get_color(*current) == node_color::BLACK || get_color(*parent) == node_color::BLACK)
        {
            break;
        }
        
        if (get_color(*uncle) == node_color::RED)
        {
            set_color(*grandparent, node_color::RED);
            set_color(*parent, node_color::BLACK);
            set_color(*uncle, node_color::BLACK);
        }
        else
        {
            set_color(*grandparent, node_color::RED);
            
            if ((*grandparent)->left_subtree == *parent)
            {
                if ((*parent)->left_subtree == *current)
                {
                    set_color(*parent, node_color::BLACK);
                    dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->small_right_rotation(*grandparent);
                }
                else
                {
                    set_color(*current, node_color::BLACK);
                    dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->big_right_rotation(*grandparent);
                }
            }
            else
            {
                if ((*parent)->right_subtree == *current)
                {
                    set_color(*parent, node_color::BLACK);
                    dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->small_left_rotation(*grandparent);
                }
                else
                {
                    set_color(*current, node_color::BLACK);
                    dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->big_left_rotation(*grandparent);
                }
            }
        }
    }
    
    if (path.size() == 1)
    {
        auto *node = reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(*(path.top()));
        reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(*(path.top()))->color = node_color::BLACK;
    }
    
    this->trace_with_guard(get_typename() + "::insertion_template_method::balance(std::stack<node**>) : successfully finished.")
        ->debug_with_guard(get_typename() + "::insertion_template_method::balance(std::stack<node**>) : successfully finished.");
}

template<
    typename tkey,
    typename tvalue>
inline std::string red_black_tree<tkey, tvalue>::insertion_template_method::get_typename() const noexcept
{
    return "red_black_tree<tkey, tvalue>::insertion_template_method";
}


template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    red_black_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
        binary_search_tree<tkey, tvalue>::disposal_template_method(tree, disposal_strategy)
{ }

template<
    typename tkey,
    typename tvalue>
void red_black_tree<tkey, tvalue>::disposal_template_method::balance(
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path,
    typename binary_search_tree<tkey, tvalue>::node* node_to_dispose)
{
    this->trace_with_guard(get_typename() + "::disposal_template_method::balance(std::stack<node**>) : called.")
        ->debug_with_guard(get_typename() + "::disposal_template_method::balance(std::stack<node**>) : called.");
    
    if (get_color(node_to_dispose) == node_color::RED)
    {
        this->trace_with_guard(get_typename() + "::disposal_template_method::balance(std::stack<node**>) : successfully finished.")
            ->debug_with_guard(get_typename() + "::disposal_template_method::balance(std::stack<node**>) : successfully finished.");
        
        return;
    }
    
    if (node_to_dispose->left_subtree != nullptr || node_to_dispose->right_subtree != nullptr)
    {
        dynamic_cast<red_black_tree<tkey, tvalue>::node*>(*(path.top()))->color = node_color::BLACK;
    }
    else
    {
        // BLACK LEAF...
        bool is_disbalanced = true;
        
        while (path.size() >= 2 && is_disbalanced)
        {
            is_disbalanced = false;
            
            typename binary_search_tree<tkey, tvalue>::node** current = path.top();
            path.pop();
            typename binary_search_tree<tkey, tvalue>::node** parent = path.top();
            
            bool is_right = (*parent)->right_subtree == *current;
            
            typename binary_search_tree<tkey, tvalue>::node** brother = is_right // always exists
                    ? &(*parent)->left_subtree
                    : &(*parent)->right_subtree;
            
            typename binary_search_tree<tkey, tvalue>::node** nephew = is_right
                    ? &(*brother)->right_subtree
                    : &(*brother)->left_subtree;
            
            typename binary_search_tree<tkey, tvalue>::node** opposite_nephew = is_right
                    ? &(*brother)->left_subtree
                    : &(*brother)->right_subtree;
            
            if (get_color(*parent) == node_color::RED)
            {
                // RED PARENT, BLACK BROTHER WITH BLACK CHILDREN
                if (get_color(*nephew) == node_color::BLACK && get_color(*opposite_nephew) == node_color::BLACK)
                {
                    set_color(*parent, node_color::BLACK);
                    set_color(*brother, node_color::RED);
                }
                else
                {
                    // RED PARENT, BLACK BROTHER, RED OPPOSITE NEPHEW
                    if (get_color(*opposite_nephew) == node_color::RED)
                    {
                        set_color(*brother, node_color::RED);
                        set_color(*parent, node_color::BLACK);
                        set_color(*opposite_nephew, node_color::BLACK);
                        call_small_rotation(parent, this->_tree, is_right);
                    }
                    // RED PARENT, BLACK BROTHER, BLACK OPPOSITE NEPHEW <-- CHECKED BY TEST 12
                    else
                    {
                        // "make" opposite nephew red, then follow the previous case
                        set_color(*parent, node_color::BLACK);
                        call_big_rotation(parent, this->_tree, is_right);
                    }
                }
            }
            else
            {
                if (get_color(*brother) == node_color::RED)
                {
                    // brother is red => nephew exists
                    typename binary_search_tree<tkey, tvalue>::node** grandnephew = is_right
                            ? &(*nephew)->right_subtree
                            : &(*nephew)->left_subtree;
                    
                    typename binary_search_tree<tkey, tvalue>::node** opposite_grandnephew = is_right
                            ? &(*nephew)->left_subtree
                            : &(*nephew)->right_subtree;
                    
                    // BLACK PARENT, RED BROTHER, BLACK CO-DIRECTIONAL NEPHEW WITH BLACK CHILDREN
                    if (get_color(*grandnephew) == node_color::BLACK && get_color(*opposite_grandnephew) == node_color::BLACK)
                    {
                        set_color(*brother, node_color::BLACK);
                        set_color(*nephew, node_color::RED);
                        call_small_rotation(parent, this->_tree, is_right);
                    }
                    else
                    {
                        // BLACK PARENT, RED BROTHER, BLACK CO-DIRECTIONAL NEPHEW WITH RED OPPOSITE CHILD
                        if (get_color(*opposite_grandnephew) == node_color::RED)
                        {
                            set_color(*opposite_grandnephew, node_color::BLACK);
                            call_big_rotation(parent, this->_tree, is_right);
                        }
                        // BLACK PARENT, RED BROTHER, BLACK CO-DIRECTIONAL NEPHEW WITH RED CO-DIRECTIONAL CHILD
                        else
                        {
                            call_big_rotation(parent, this->_tree, is_right);
                        }
                    }
                    
                }
                else
                {
                    // BLACK PARENT, BLACK BROTHER WITH BLACK CHILDREN
                    if (get_color(*nephew) == node_color::BLACK && get_color(*opposite_nephew) == node_color::BLACK)
                    {
                        set_color(*brother, node_color::RED);
                        is_disbalanced = true;
                    }
                    else
                    {
                        // BLACK PARENT, BLACK BROTHER WITH RED CO-DIRECTIONAL CHILDREN
                        if (get_color(*nephew) == node_color::RED)
                        {
                            set_color(*nephew, node_color::BLACK);
                            call_big_rotation(parent, this->_tree, is_right);
                        }
                        // BLACK PARENT, BLACK BROTHER WITH RED OPPOSITE CHILDREN
                        else
                        {
                            set_color(*opposite_nephew, node_color::BLACK);
                            call_small_rotation(parent, this->_tree, is_right);
                        }
                    }
                }
            }
        } // while end
    }
    
    this->trace_with_guard(get_typename() + "::disposal_template_method::balance(std::stack<node**>) : successfully finished.")
        ->debug_with_guard(get_typename() + "::disposal_template_method::balance(std::stack<node**>) : successfully finished.");
}

template<
    typename tkey,
    typename tvalue>
void red_black_tree<tkey, tvalue>::disposal_template_method::call_small_rotation(
    typename binary_search_tree<tkey, tvalue>::node** node_to_rotate,
    void *root,
    bool is_right) const
{
    if (is_right)
    {
        dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->small_right_rotation(*node_to_rotate);
    }
    else
    {
        dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->small_left_rotation(*node_to_rotate);
    }
}

template<
    typename tkey,
    typename tvalue>
void red_black_tree<tkey, tvalue>::disposal_template_method::call_big_rotation(
    typename binary_search_tree<tkey, tvalue>::node** node_to_rotate,
    void *root,
    bool is_right) const
{
    if (is_right)
    {
        dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->big_right_rotation(*node_to_rotate);
    }
    else
    {
        dynamic_cast<red_black_tree<tkey, tvalue>*>(this->_tree)->big_left_rotation(*node_to_rotate);
    }
}

template<
    typename tkey,
    typename tvalue>
inline std::string red_black_tree<tkey, tvalue>::disposal_template_method::get_typename() const noexcept
{
    return "red_black_tree<tkey, tvalue>::disposal_template_method";
}

#pragma endregion template methods implementation

#pragma region rbt construction, assignment, destruction implementation

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::red_black_tree(
    std::function<int(tkey const &, tkey const &)> comparer,
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
        binary_search_tree<tkey, tvalue>(
            new(std::nothrow) red_black_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy),
            new(std::nothrow) typename binary_search_tree<tkey, tvalue>::obtaining_template_method(this),
            new(std::nothrow) red_black_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy),
            comparer, allocator, logger)
{
    if (this->_insertion_template == nullptr || this->_obtaining_template == nullptr ||
            this->_disposal_template == nullptr)
    {
        delete this->_insertion_template;
        delete this->_obtaining_template;
        delete this->_disposal_template;
        throw std::bad_alloc();
    }
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::red_black_tree(
    red_black_tree<tkey, tvalue> const &other):
        binary_search_tree<tkey, tvalue>(other._keys_comparer, other.get_logger(), other.get_allocator())
{
    try
    {
        this->_root = copy(static_cast<node*>(other._root));
        this->_insertion_template = new red_black_tree<tkey, tvalue>::insertion_template_method(this, other._insertion_template->_insertion_strategy);
        this->_obtaining_template = new typename binary_search_tree<tkey, tvalue>::obtaining_template_method(this);
        this->_disposal_template = new red_black_tree<tkey, tvalue>::disposal_template_method(this, other._disposal_template->_disposal_strategy);
    }
    catch (const std::bad_alloc& ex)
    {
        clear(reinterpret_cast<node**>(&this->_root));
        delete this->_insertion_template;
        delete this->_obtaining_template;
        delete this->_disposal_template;
        throw;
    }
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::red_black_tree(
    red_black_tree<tkey, tvalue> &&other) noexcept:
        binary_search_tree<tkey, tvalue>(std::move(other))
{ }

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue> &red_black_tree<tkey, tvalue>::operator=(
    red_black_tree<tkey, tvalue> const &other)
{
    if (*this != other)
    {
        binary_search_tree<tkey, tvalue>::operator=(other);
    }
    
    return *this;
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue> &red_black_tree<tkey, tvalue>::operator=(
    red_black_tree<tkey, tvalue> &&other) noexcept
{
    if (*this != other)
    {
        binary_search_tree<tkey, tvalue>::operator=(std::move(other));
    }
    
    return *this;
}

template<
    typename tkey,
    typename tvalue>
red_black_tree<tkey, tvalue>::~red_black_tree() noexcept
{ }

#pragma endregion rbt construction, assignment, destruction implementation

#pragma region avl extra functions implementation

template<
    typename tkey,
    typename tvalue>
inline size_t red_black_tree<tkey, tvalue>::get_node_size() const noexcept
{
    return sizeof(typename red_black_tree<tkey, tvalue>::node);
}

template<
    typename tkey,
    typename tvalue>
inline void red_black_tree<tkey, tvalue>::call_node_constructor(
    typename binary_search_tree<tkey,tvalue>::node *raw_space,
    tkey const &key,
    tvalue const &value)
{
    allocator::construct(dynamic_cast<red_black_tree<tkey, tvalue>::node*>(raw_space), key, value);
}

template<
    typename tkey,
    typename tvalue>
inline void red_black_tree<tkey, tvalue>::call_node_constructor(
    typename binary_search_tree<tkey,tvalue>::node *raw_space,
    tkey const &key,
    tvalue &&value)
{
    allocator::construct(reinterpret_cast<red_black_tree<tkey, tvalue>::node*>(raw_space), key, std::move(value));
}

template<
    typename tkey,
    typename tvalue>
void red_black_tree<tkey, tvalue>::inject_additional_data(
    typename binary_search_tree<tkey,tvalue>::iterator_data *destination,
    typename binary_search_tree<tkey,tvalue>::node *source) const
{
    auto *rbt_destination = dynamic_cast<red_black_tree<tkey, tvalue>::iterator_data*>(destination);
    auto *rbt_source = dynamic_cast<red_black_tree<tkey, tvalue>::node*>(source);
    
    rbt_destination->_color = rbt_source->color;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *red_black_tree<tkey, tvalue>::create_iterator_data() const
{
    return new iterator_data;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *red_black_tree<tkey, tvalue>::create_iterator_data(
    unsigned int depth,
    typename binary_search_tree<tkey,tvalue>::node *&src_node) const
{
    return new iterator_data(depth, reinterpret_cast<red_black_tree<tkey, tvalue>::node**>(&src_node));
}

template<
    typename tkey,
    typename tvalue>
typename red_black_tree<tkey, tvalue>::node_color red_black_tree<tkey, tvalue>::get_color(
    typename binary_search_tree<tkey,tvalue>::node* node) noexcept
{
    auto *rbt_node = dynamic_cast<red_black_tree<tkey,tvalue>::node*>(node);
    
    return rbt_node ? rbt_node->color : node_color::BLACK;
}

template<
    typename tkey,
    typename tvalue>
void red_black_tree<tkey, tvalue>::set_color(
    typename binary_search_tree<tkey,tvalue>::node* node,
    node_color color) noexcept
{
    dynamic_cast<red_black_tree<tkey,tvalue>::node*>(node)->color = color;
}

#pragma endregion avl extra functions implementation

template<
    typename tkey,
    typename tvalue>
inline std::string red_black_tree<tkey, tvalue>::get_typename() const noexcept
{
    return "red_black_tree<tkey, tvalue>";
}


#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H
