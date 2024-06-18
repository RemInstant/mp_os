#include <iostream>
#include <functional>
#include <stack>

template<
    typename tkey>
int default_comparer(
    tkey const &lhs,
    tkey const &rhs)
{
    return lhs < rhs;
}

template<>
int default_comparer<int>(
    int const &lhs,
    int const &rhs)
{
    return lhs - rhs;
}

template<
    typename tkey,
    typename tvalue>
class B_plus_tree final
{

private:

    struct key_value_ptr_pair
    {
    
        tkey key;
        tvalue *value;
        
        key_value_ptr_pair():
            value(nullptr)
        {}
        
        key_value_ptr_pair(
            tkey key,
            tvalue &&value):
                key(key)
        {
            this->value = new tvalue(std::move(value));
        }
        
        // ~key_value_ptr_pair()
        // {
        //     delete value;
        // }
    
    };

    struct node
    {
    
        size_t virtual_size;
        key_value_ptr_pair *kvp_array;
        node **subtrees;
        
        node(size_t t):
            virtual_size(0)
        {
            try
            {
                kvp_array = new key_value_ptr_pair[2 * t];
                subtrees = new node * [2 * t + 1];
            }
            catch (std::bad_alloc const &)
            {
                delete[] kvp_array;
            }
            
            for (size_t i = 0; i < 2 * t + 1; ++i)
            {
                subtrees[i] = nullptr;
            }
        }
    
    };

private:

    size_t _t;
    node *_root;
    std::function<int(tkey const &, tkey const &)> _comparer;


public:

    B_plus_tree(
        size_t t,
        std::function<int(tkey const &, tkey const &)> comparer = default_comparer<tkey>):
            _t(t),
            _root(nullptr),
            _comparer(comparer)
    { }
    
    // rule of five

public:

    bool insert(
        tkey const &key,
        tvalue &&value)
    {
        auto path = find_path(key);
        
        if (path.empty())
        {
            _root = new node(_t);
            _root->kvp_array[0] = key_value_ptr_pair(key, std::move(value));
            _root->virtual_size++;
            return true;
        }
        
        auto **cur = path.top().first;
        auto index = path.top().second;
        
        if (index >= 0)
        {
            return false;
        }
        
        index = -index - 1;
        (*cur)->kvp_array[(*cur)->virtual_size] = key_value_ptr_pair(key, std::move(value));
        
        for (size_t i = (*cur)->virtual_size; i > index; --i)
        {
            std::swap((*cur)->kvp_array[i], (*cur)->kvp_array[i-1]);
        }
        
        (*cur)->virtual_size++;
        
        while ((*cur)->virtual_size == 2 * _t)
        {
            auto [kvp, right] = split(*cur);
            path.pop();
            
            if (path.empty())
            {
                node* tmp = *cur;
                
                _root = new node(_t);
                _root->kvp_array[0] = std::move(kvp);
                _root->virtual_size++;
                
                _root->subtrees[0] = tmp;
                _root->subtrees[1] = right;
                
                return true;
            }
            
            cur = path.top().first;
            index = -path.top().second - 1;
            
            (*cur)->kvp_array[(*cur)->virtual_size] = std::move(kvp);
            (*cur)->subtrees[(*cur)->virtual_size + 1] = right;
            
            for (size_t i = (*cur)->virtual_size; i > index; --i)
            {
                std::swap((*cur)->kvp_array[i], (*cur)->kvp_array[i-1]);
                std::swap((*cur)->subtrees[i+1], (*cur)->subtrees[i]);
            }
            
            (*cur)->virtual_size++;
        }
        
        return true;
    }

    bool dispose(
        tkey const &key)
    {
        auto path = find_path(key);
        
        if (path.empty() || path.top().second < 0)
        {
            return false;
        }
        
        node *cur = *path.top().first;
        size_t index = path.top().second;
        
        bool need_restructure = index == 0;
        tkey disposed_key = cur->kvp_array[index].key;
        
        for (size_t i = index + 1; i < cur->virtual_size; ++i)
        {
            std::swap(cur->kvp_array[i - 1], cur->kvp_array[i]);
        }
        // destruct
        
        cur->virtual_size--;
        
        while (cur->virtual_size < _t - 1)
        {
            path.pop();
            
            if (path.empty())
            {
                if (cur->virtual_size == 0)
                {
                    _root = cur->subtrees[0];
                    delete cur;
                }
                
                return true;
            }
            
            node *parent = *path.top().first;
            int sub_index = path.top().second;
            size_t index = -sub_index - 1;
            
            bool is_leaf = cur->subtrees[0] == nullptr;
            
            bool is_left_brother_exists = index > 0;
            bool is_right_brother_exists = index < parent->virtual_size;
            
            bool can_take_from_left = is_left_brother_exists && (parent->subtrees[index - 1]->virtual_size > _t - 1);
            bool can_take_from_right = is_right_brother_exists && (parent->subtrees[index + 1]->virtual_size > _t - 1);
            
            if (can_take_from_left)
            {
                node *left_brother = parent->subtrees[index - 1];
                
                if (is_leaf)
                {
                    for (size_t i = cur->virtual_size; i > 1; ++i)
                    {
                        std::swap(cur->kvp_array[i], cur->kvp_array[i-1]);
                    }
                    
                    std::swap(cur->kvp_array[0], left_brother->kvp_array[left_brother->virtual_size - 1]);
                    // destruct node?
                }
                else
                {
                    for (size_t i = cur->virtual_size; i > 1; ++i)
                    {
                        std::swap(cur->kvp_array[i], cur->kvp_array[i-1]);
                        std::swap(cur->subtrees[i+1], cur->subtrees[i]);
                    }
                    std::swap(cur->subtrees[0], cur->subtrees[1]);
                    
                    std::swap(cur->kvp_array[0], parent->kvp_array[index - 1]);
                    std::swap(parent->kvp_array[index - 1], left_brother->kvp_array[left_brother->virtual_size - 1]);
                    std::swap(cur->subtrees[0], left_brother->subtrees[left_brother->virtual_size]);
                    
                    // destruct
                }
                
                left_brother->virtual_size--;
                cur->virtual_size++;
            }
            else if (can_take_from_right)
            {
                node *right_brother = parent->subtrees[index + 1];
                
                if (is_leaf)
                {
                    std::swap(cur->kvp_array[cur->virtual_size], right_brother->kvp_array[0]);
                    
                    for (size_t i = 1; i < right_brother->virtual_size; ++i)
                    {
                        std::swap(right_brother->kvp_array[i-1], right_brother->kvp_array[i]);
                    }
                    
                    parent->kvp_array[index] = right_brother->kvp_array[0];
                }
                else
                {
                    std::swap(cur->kvp_array[cur->virtual_size], parent->kvp_array[index]);
                    std::swap(parent->kvp_array[index], right_brother->kvp_array[0]);
                    std::swap(cur->subtrees[cur->virtual_size + 1], right_brother->subtrees[0]);
                    
                    for (size_t i = 1; i < right_brother->virtual_size; ++i)
                    {
                        std::swap(right_brother->kvp_array[i-1], right_brother->kvp_array[i]);
                        std::swap(right_brother->subtrees[i-1], right_brother->subtrees[i]);
                    }
                    std::swap(cur->subtrees[right_brother->virtual_size - 1], cur->subtrees[right_brother->virtual_size]);
                }
                
                right_brother->virtual_size--;
                cur->virtual_size++;
            }
            else
            {
                if (is_left_brother_exists)
                {
                    merge(parent, index - 1, is_leaf);
                }
                else
                {
                    merge(parent, index, is_leaf);
                }
            }
            
            if (is_leaf && need_restructure)
            {
                update_structure(cur, path, disposed_key);
                need_restructure = false;
            }
            
            if (can_take_from_left || can_take_from_right)
            {
                return true;
            }
            
            cur = parent;
        }
        
        if (need_restructure)
        {
            update_structure(cur, path, disposed_key);
        }
        
        return true;
    }

    tvalue obtain(
        tkey const &key)
    {
        int index = -1;
        int subtree_index;
        node *cur = _root;
        
        if (_root == nullptr)
        {
            throw std::logic_error("Tree is empty");
        }
        
        while (cur != nullptr)
        {
            index = find_node_path(cur, key);
            
            subtree_index = -index - 1;
            index = -index - 2;
            
            if (index < cur->virtual_size && cur->kvp_array[index].key == key)
            {
                return *cur->kvp_array[index].value;
            }
            
            cur = cur->subtrees[subtree_index];
        }
        
        throw std::logic_error("Attempt to obtain non-existent key");
    }
    
    std::vector<std::pair<tkey, tvalue>>
    obtain_between(
        tkey const &key_left,
        tkey const &key_right,
        bool left_bound_inclusive,
        bool right_bound_inclusive)
    {
        std::vector<std::pair<tkey, tvalue>> range;
        auto path = find_path(key_left);
        
        node *cur = *path.top().first;
        int index = path.top().second;
        
        if (index < 0)
        {
            index = -index - 1;
        }
        
        if (index == cur->virtual_size)
        {
            index--;
        }
        
        if (_comparer(cur->kvp_array[index].key, key_left) == 0 && !left_bound_inclusive)
        {
            index++;
            
            if (index == cur->virtual_size)
            {
                cur = cur->subtrees[2 * _t];
                index = 0;
            }
        }
        
        while (cur != nullptr && _comparer(cur->kvp_array[index].key, key_right) <= (right_bound_inclusive ? 0 : -1))
        {
            range.emplace_back(cur->kvp_array[index].key, *cur->kvp_array[index].value);
            ++index;

            if (index == cur->virtual_size)
            {
                cur = cur->subtrees[2 * _t];
                index = 0;
            }
        } 
        
        return range;
    }

private:

    int find_node_path(
        node *node,
        tkey key)
    {
        size_t left = 0;
        size_t right = node->virtual_size - 1;
        
        while (true)
        {
            size_t i = (left + right) / 2;
            
            int res = _comparer(key, node->kvp_array[i].key);
            
            if (res == 0)
            {
                return -i - 2;
            }
            
            if (left == right)
            {
                return res < 0 ? (-i - 1) : (-i - 2);
            }
            
            if (res < 0)
            {
                right = i;
            }
            else
            {
                left = i + 1;
            }
        }
    }
    
    std::stack<std::pair<node**, int>> find_path(
        tkey key)
    {
        std::stack<std::pair<node**, int>> path;
        int index = -1;
        node **cur = &_root;
        
        if (_root == nullptr)
        {
            return path;
        }
        
        while (*cur != nullptr)
        {
            index = find_node_path(*cur, key);
            path.emplace(cur, index);
            cur = &((*cur)->subtrees[-index - 1]);
        }
        
        int tmp = -path.top().second - 2;
        
        if ((*path.top().first)->kvp_array[tmp].key == key)
        {
            path.top().second = tmp;
        }
        
        return path;
    }

    std::pair<key_value_ptr_pair, node*> split(
        node *cur)
    {
        size_t mediant = cur->virtual_size / 2;
        
        key_value_ptr_pair kvp = cur->kvp_array[mediant];
        node *new_node = new node(_t);
        
        if (cur->subtrees[0] == nullptr)
        {
            for (size_t i = 0; i < mediant; ++i)
            {
                std::swap(cur->kvp_array[mediant + i], new_node->kvp_array[i]);
            }
            new_node->virtual_size = mediant;
            new_node->subtrees[2 * _t] = cur->subtrees[2 * _t];
            cur->subtrees[2 * _t] = new_node;
        }
        else
        {
            for (size_t i = 0; i < mediant - 1; ++i)
            {
                std::swap(cur->kvp_array[mediant + i + 1], new_node->kvp_array[i]);
                std::swap(cur->subtrees[mediant + i + 1], new_node->subtrees[i]);
            }
            std::swap(cur->subtrees[cur->virtual_size], new_node->subtrees[mediant - 1]);
            new_node->virtual_size = mediant - 1;
        }
        
        cur->virtual_size = mediant;
        
        return std::make_pair(kvp, new_node);
    }

    void merge(
        node* parent,
        size_t index,
        bool is_leaf)
    {
        node *left_child = parent->subtrees[index];
        node *right_child = parent->subtrees[index + 1];
        
        if (is_leaf)
        {
            for (size_t i = 0; i < right_child->virtual_size; ++i)
            {
                left_child->kvp_array[left_child->virtual_size + i] = std::move(right_child->kvp_array[i]);
            }
            
            left_child->virtual_size += right_child->virtual_size;
            left_child->subtrees[2 * _t] = right_child->subtrees[2 * _t];
            delete right_child;
            
            for (size_t i = index + 1; i < parent->virtual_size; ++i)
            {
                std::swap(parent->kvp_array[i - 1], parent->kvp_array[i]);
                parent->subtrees[i] = parent->subtrees[i + 1];
            }
            
            //destruct;
            parent->virtual_size--;
        }
        else
        {
            left_child->kvp_array[left_child->virtual_size] = std::move(parent->kvp_array[index]);
            
            for (size_t i = 0; i < right_child->virtual_size; ++i)
            {
                left_child->kvp_array[left_child->virtual_size + i + 1] = std::move(right_child->kvp_array[i]);
                left_child->subtrees[left_child->virtual_size + i + 1] = std::move(right_child->subtrees[i]);
            }
            left_child->subtrees[2 * _t - 2] = std::move(right_child->subtrees[right_child->virtual_size]);
            
            left_child->virtual_size += right_child->virtual_size + 1;
            delete right_child;
            
            for (size_t i = index + 1; i < parent->virtual_size; ++i)
            {
                std::swap(parent->kvp_array[i - 1], parent->kvp_array[i]);
                parent->subtrees[i] = parent->subtrees[i + 1];
            }
            
            parent->virtual_size--;
        }
    }
    
    void update_structure(
        node *leaf,
        std::stack<std::pair<node**, int>> const &path,
        tkey const &disposed_key)
    {
        auto path_copy = path;
        
        while (!path_copy.empty())
        {
            auto [cur, index] = path_copy.top();
            path_copy.pop();
            
            if (index < 0)
            {
                index = -index - 2;
            }
            
            if ((*cur)->kvp_array[index].key == disposed_key)
            {
                (*cur)->kvp_array[index] = leaf->kvp_array[0];
                return;
            }
        }
    }
};



int main(int argc, char** argv, char** env)
{
    B_plus_tree<int, int> tree(2);
    
    tree.insert(5, 5);
    tree.insert(15, 15);
    tree.insert(8, 8);
    tree.insert(11, 11);
    
    tree.insert(6, 6);
    tree.insert(7, 7);
    
    tree.insert(12, 12);
    tree.insert(17, 17);
    
    tree.insert(13, 13);
    tree.insert(14, 14);
    
    std::cout << tree.obtain(13) << std::endl;
    std::cout << tree.obtain(7) << std::endl;
    std::cout << tree.obtain(8) << std::endl;
    std::cout << tree.obtain(12) << std::endl;
    std::cout << tree.obtain(11) << std::endl;
    std::cout << tree.obtain(15) << std::endl;
    std::cout << tree.obtain(14) << std::endl;
    
    auto range = tree.obtain_between(5, 17, 1, 1);
    for (auto elem : range)
    {
        std::cout << elem.first << ' ';
    }
    std::cout << std::endl;
    
    tree.dispose(7);
    tree.dispose(8);
    tree.dispose(14);
    tree.dispose(13);
    tree.dispose(15);
    tree.dispose(12);
    tree.dispose(11);
    
    std::cout << tree.obtain(5) << std::endl;
    std::cout << tree.obtain(6) << std::endl;
    std::cout << tree.obtain(17) << std::endl;
    
    std::cout << "timofey" << std::endl;
    
}