#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H

#include <binary_search_tree.h>
#include <extra_utility.h>

template<
    typename tkey,
    typename tvalue>
class AVL_tree final:
    public binary_search_tree<tkey, tvalue>
{

private:
    
    struct node final:
        binary_search_tree<tkey, tvalue>::node
    {

    public:
    
        unsigned int subtree_height;
    
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
    
        friend void AVL_tree<tkey, tvalue>::inject_additional_data(
           typename binary_search_tree<tkey, tvalue>::iterator_data *,
           typename binary_search_tree<tkey, tvalue>::node *) const;
    
    private:
        
        unsigned int _subtree_height;
    
    public:
    
        unsigned int get_subtree_height() const;

    public:
    
        iterator_data();
    
        explicit iterator_data(
            unsigned int depth,
            tkey const &key,
            tvalue const &value,
            unsigned int subtree_height);
    
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
    
    void balance(
        std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path);
    
    
    class insertion_template_method final:
        public binary_search_tree<tkey, tvalue>::insertion_template_method
    {
    
    public:
        
        explicit insertion_template_method(
            AVL_tree<tkey, tvalue> *tree,
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
            AVL_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);
        
    private:
    
        void balance(
            std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path,
            typename binary_search_tree<tkey, tvalue>::node* node_to_dispose = nullptr) override;
    
    private:
    
        inline std::string get_typename() const noexcept override;
    
    };

public:
    
    explicit AVL_tree(
        std::function<int(tkey const &, tkey const &)> comparer = associative_container<tkey, tvalue>::default_key_comparer(),
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy =
                binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy =
                binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:
    
    ~AVL_tree() noexcept final;
    
    AVL_tree(
        AVL_tree<tkey, tvalue> const &other);
    
    AVL_tree<tkey, tvalue> &operator=(
        AVL_tree<tkey, tvalue> const &other);
    
    AVL_tree(
        AVL_tree<tkey, tvalue> &&other) noexcept;
    
    AVL_tree<tkey, tvalue> &operator=(
        AVL_tree<tkey, tvalue> &&other) noexcept;

private:

    inline size_t get_node_size() const noexcept override;

    inline void update_node_data(
        typename binary_search_tree<tkey,tvalue>::node *node) const noexcept override;

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

    static inline unsigned int get_subtree_height(
        typename binary_search_tree<tkey,tvalue>::node* node) noexcept;

private:

    inline std::string get_typename() const noexcept override;

};


#pragma region node implementation

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue const &value):
        binary_search_tree<tkey, tvalue>::node(key, value),
        subtree_height(1)
{ }

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue &&value):
        binary_search_tree<tkey, tvalue>::node(key, std::move(value)),
        subtree_height(1)
{ }

#pragma endregion node implementation

#pragma region iterator_data implementation

template<
    typename tkey,
    typename tvalue>
unsigned int AVL_tree<tkey, tvalue>::iterator_data::get_subtree_height() const
{
    if (this->_is_state_initialized)
    {
        return _subtree_height;
    }
    
    throw std::logic_error("Tried to read from uninitialized iterator data");
}


template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data():
    binary_search_tree<tkey, tvalue>::iterator_data(),
    _subtree_height(0)
{ }

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value,
    unsigned int subtree_height):
        binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value),
        _subtree_height(subtree_height)
{ }

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    node **src_node):
        binary_search_tree<tkey, tvalue>::iterator_data(depth, reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node**>(src_node))
{
    if (*src_node != nullptr)
    {
        _subtree_height = (*src_node)->subtree_height;
    }
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
    iterator_data const &other):
        binary_search_tree<tkey, tvalue>::iterator_data(other),
        _subtree_height(other._subtree_height)
{ }

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
    iterator_data &&other) noexcept:
        binary_search_tree<tkey, tvalue>::iterator_data(std::move(other)),
        _subtree_height(std::move(other._subtree_height))
{ }

template<
    typename tkey,
    typename tvalue>
typename AVL_tree<tkey, tvalue>::iterator_data &AVL_tree<tkey, tvalue>::iterator_data::operator=(
    iterator_data const &other)
{
    if (this != &other)
    {
        binary_search_tree<tkey, tvalue>::iterator_data::operator=(other);
        _subtree_height = other._subtree_height;
    }
    
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename AVL_tree<tkey, tvalue>::iterator_data &AVL_tree<tkey, tvalue>::iterator_data::operator=(
    iterator_data &&other) noexcept
{
    if (this != &other)
    {
        binary_search_tree<tkey, tvalue>::iterator_data::operator=(std::move(other));
        _subtree_height = std::move(other._subtree_height);
    }
    
    return *this;
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::~iterator_data() noexcept
{ }

#pragma endregion iterator_data implementation

#pragma region avl balance implementation

template<
    typename tkey,
    typename tvalue>
void AVL_tree<tkey, tvalue>::balance(
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path)
{
    this->trace_with_guard(get_typename() + "::balance(std::stack<node**>) : called.")
        ->debug_with_guard(get_typename() + "::balance(std::stack<node**>) : called.");
    
    while (!path.empty())
    {
        typename binary_search_tree<tkey, tvalue>::node **current = path.top();
        typename AVL_tree<tkey, tvalue>::node *avl_current = dynamic_cast<AVL_tree<tkey, tvalue>::node*>(*current);
        path.pop();
        
        if (*current == nullptr)
        {
            continue;
        }
        
        update_node_data(*current);
        
        unsigned int left_subtree_height = get_subtree_height((*current)->left_subtree);
        unsigned int right_subtree_height = get_subtree_height((*current)->right_subtree);
        
        this->debug_with_guard(get_typename() + "::balance(std::stack<node**>) : checking node with key \"" +
                extra_utility::make_string((*current)->key) + "\". Height of left subtree is " + std::to_string(left_subtree_height) +
                ". Height of right subtree is " + std::to_string(right_subtree_height));
        
        if (right_subtree_height - left_subtree_height == 2)
        {
            typename binary_search_tree<tkey, tvalue>::node *right_subtree = (*current)->right_subtree;
            
            if (get_subtree_height(right_subtree->left_subtree) > get_subtree_height(right_subtree->right_subtree))
            {
                this->big_left_rotation(*current);
            }
            else
            {
                this->small_left_rotation(*current);
            }
        }
        else if (right_subtree_height - left_subtree_height == -2)
        {
            typename binary_search_tree<tkey, tvalue>::node *left_subtree = (*current)->left_subtree;
            
            if (get_subtree_height(left_subtree->right_subtree) > get_subtree_height(left_subtree->left_subtree))
            {
                this->big_right_rotation(*current);
            }
            else
            {
                this->small_right_rotation(*current);
            }
        }
    }
    
    this->trace_with_guard(get_typename() + "::balance(std::stack<node**>) : successfully finished.")
        ->debug_with_guard(get_typename() + "::balance(std::stack<node**>) : successfully finished.");
}

#pragma endregion avl balance implementation

#pragma region template methods implementation

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    AVL_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
        binary_search_tree<tkey, tvalue>::insertion_template_method(tree, insertion_strategy)
{ }

template<
    typename tkey,
    typename tvalue>
void AVL_tree<tkey, tvalue>::insertion_template_method::balance(
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path,
    typename binary_search_tree<tkey, tvalue>::node* node_to_dispose)
{
    dynamic_cast<AVL_tree<tkey, tvalue>*>(this->_tree)->balance(path);
}

template<
    typename tkey,
    typename tvalue>
inline std::string AVL_tree<tkey, tvalue>::insertion_template_method::get_typename() const noexcept
{
    return "binary_search_tree<tkey, tvalue>::insertion_template_method";
}


template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    AVL_tree<tkey, tvalue> *tree,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
        binary_search_tree<tkey, tvalue>::disposal_template_method(tree, disposal_strategy)
{ }

template<
    typename tkey,
    typename tvalue>
void AVL_tree<tkey, tvalue>::disposal_template_method::balance(
    std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path,
    typename binary_search_tree<tkey, tvalue>::node* node_to_dispose)
{
    dynamic_cast<AVL_tree<tkey, tvalue>*>(this->_tree)->balance(path);
}

template<
    typename tkey,
    typename tvalue>
inline std::string AVL_tree<tkey, tvalue>::disposal_template_method::get_typename() const noexcept
{
    return "binary_search_tree<tkey, tvalue>::disposal_template_method";
}

#pragma endregion template methods implementation

#pragma region avl construction, assignment, destruction implementation

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
    std::function<int(tkey const &, tkey const &)> comparer,
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
        binary_search_tree<tkey, tvalue>(
            new(std::nothrow) AVL_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy),
            new(std::nothrow) typename binary_search_tree<tkey, tvalue>::obtaining_template_method(this),
            new(std::nothrow) AVL_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy),
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
AVL_tree<tkey, tvalue>::AVL_tree(
    AVL_tree<tkey, tvalue> const &other):
        binary_search_tree<tkey, tvalue>(other._keys_comparer, other.get_logger(), other.get_allocator())
{
    try
    {
        this->_root = copy(static_cast<node*>(other._root));
        this->_insertion_template = new AVL_tree<tkey, tvalue>::insertion_template_method(this, other._insertion_template->_insertion_strategy);
        this->_obtaining_template = new typename binary_search_tree<tkey, tvalue>::obtaining_template_method(this);
        this->_disposal_template = new AVL_tree<tkey, tvalue>::disposal_template_method(this, other._disposal_template->_disposal_strategy);
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
AVL_tree<tkey, tvalue>::AVL_tree(
    AVL_tree<tkey, tvalue> &&other) noexcept:
        binary_search_tree<tkey, tvalue>(std::move(other))
{ }

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(
    AVL_tree<tkey, tvalue> const &other)
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
AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(
    AVL_tree<tkey, tvalue> &&other) noexcept
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
AVL_tree<tkey, tvalue>::~AVL_tree() noexcept
{ }

#pragma endregion avl construction, assignment, destruction implementation

#pragma region avl extra functions implementation

template<
    typename tkey,
    typename tvalue>
inline size_t AVL_tree<tkey, tvalue>::get_node_size() const noexcept
{
    return sizeof(typename AVL_tree<tkey, tvalue>::node);
}

template<
    typename tkey,
    typename tvalue>
inline void AVL_tree<tkey, tvalue>::update_node_data(
    typename binary_search_tree<tkey,tvalue>::node *node) const noexcept
{
    AVL_tree<tkey, tvalue>::node *avl_node = dynamic_cast<AVL_tree<tkey, tvalue>::node*>(node);
    
    unsigned int left_subtree_height = get_subtree_height(avl_node->left_subtree);
    unsigned int right_subtree_height = get_subtree_height(avl_node->right_subtree);
    
    avl_node->subtree_height = std::max(left_subtree_height, right_subtree_height) + 1;
}

template<
    typename tkey,
    typename tvalue>
inline void AVL_tree<tkey, tvalue>::call_node_constructor(
    typename binary_search_tree<tkey,tvalue>::node *raw_space,
    tkey const &key,
    tvalue const &value)
{
    allocator::construct(dynamic_cast<AVL_tree<tkey, tvalue>::node*>(raw_space), key, value);
}

template<
    typename tkey,
    typename tvalue>
inline void AVL_tree<tkey, tvalue>::call_node_constructor(
    typename binary_search_tree<tkey,tvalue>::node *raw_space,
    tkey const &key,
    tvalue &&value)
{
    allocator::construct(reinterpret_cast<AVL_tree<tkey, tvalue>::node*>(raw_space), key, std::move(value));
}

template<
    typename tkey,
    typename tvalue>
void AVL_tree<tkey, tvalue>::inject_additional_data(
    typename binary_search_tree<tkey,tvalue>::iterator_data *destination,
    typename binary_search_tree<tkey,tvalue>::node *source) const
{
    auto *avl_destination = dynamic_cast<AVL_tree<tkey, tvalue>::iterator_data*>(destination);
    auto *avl_source = dynamic_cast<AVL_tree<tkey, tvalue>::node*>(source);
    
    avl_destination->_subtree_height = avl_source->subtree_height;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *AVL_tree<tkey, tvalue>::create_iterator_data() const
{
    return new iterator_data;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *AVL_tree<tkey, tvalue>::create_iterator_data(
    unsigned int depth,
    typename binary_search_tree<tkey,tvalue>::node *&src_node) const
{
    return new iterator_data(depth, reinterpret_cast<AVL_tree<tkey, tvalue>::node**>(&src_node));
}

template<
    typename tkey,
    typename tvalue>
inline unsigned int AVL_tree<tkey, tvalue>::get_subtree_height(
    typename binary_search_tree<tkey,tvalue>::node* node) noexcept
{
    auto *avl_node = dynamic_cast<AVL_tree<tkey,tvalue>::node*>(node);
    
    return avl_node ? avl_node->subtree_height : 0;
}

#pragma endregion avl extra functions implementation

template<
    typename tkey,
    typename tvalue>
inline std::string AVL_tree<tkey, tvalue>::get_typename() const noexcept
{
    return "AVL_tree<tkey, tvalue>";
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H