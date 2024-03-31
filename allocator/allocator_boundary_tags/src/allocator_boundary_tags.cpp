#include <mutex>

#include <not_implemented.h>

#include "../include/allocator_boundary_tags.h"

allocator_boundary_tags::~allocator_boundary_tags()
{
    get_mutex().~mutex();
    get_allocator()->deallocate(_trusted_memory);
}

allocator_boundary_tags::allocator_boundary_tags(
    allocator_boundary_tags const &other)
{
    throw not_implemented("allocator_boundary_tags::allocator_boundary_tags(allocator_boundary_tags const &)", "your code should be here...");
}

allocator_boundary_tags &allocator_boundary_tags::operator=(
    allocator_boundary_tags const &other)
{
    throw not_implemented("allocator_boundary_tags &allocator_boundary_tags::operator=(allocator_boundary_tags const &)", "your code should be here...");
}

allocator_boundary_tags::allocator_boundary_tags(
    allocator_boundary_tags &&other) noexcept
{
    throw not_implemented("allocator_boundary_tags::allocator_boundary_tags(allocator_boundary_tags &&) noexcept", "your code should be here...");
}

allocator_boundary_tags &allocator_boundary_tags::operator=(
    allocator_boundary_tags &&other) noexcept
{
    throw not_implemented("allocator_boundary_tags &allocator_boundary_tags::operator=(allocator_boundary_tags &&) noexcept", "your code should be here...");
}

allocator_boundary_tags::allocator_boundary_tags(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    // указатель на родительский аллокатор
    // указатель на логгер
    // указатель на мьютекс
    // фит мод
    // кол-во данных
    // указатель на ... свободный / занятый?
    
    if (space_size < get_block_meta_size())
    {
        if (logger != nullptr)
        {
            // log
        }
        throw std::logic_error("cannot initialize allocator instance");
    }
    
    try
    {
        _trusted_memory = parent_allocator == nullptr
                ? ::operator new(space_size + get_meta_size())
                : parent_allocator->allocate(space_size + get_meta_size(), 1);
    }
    catch (std::bad_alloc const &ex)
    {
        // log
        throw;
    }
    
    unsigned char *ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
    
    *reinterpret_cast<allocator**>(ptr) = parent_allocator;
    ptr += sizeof(allocator*);
    
    *reinterpret_cast<class logger**>(ptr) = logger;
    ptr += sizeof(class logger*);
    
    new(reinterpret_cast<std::mutex*>(ptr)) std::mutex();
    ptr += sizeof(std::mutex*);
    
    *reinterpret_cast<fit_mode*>(ptr) = allocate_fit_mode;
    ptr += sizeof(fit_mode);
    
    *reinterpret_cast<block_size_t*>(ptr) = space_size;
    ptr += sizeof(block_size_t);
    
    *reinterpret_cast<block_pointer_t*>(ptr) = nullptr;
}

[[nodiscard]] void *allocator_boundary_tags::allocate(
    size_t value_size,
    size_t values_count)
{
    block_size_t req_size = value_size * values_count;
    
    // if (requested_size < get_block_min_size())
    // {
    //     requested_size = get_block_min_size();
    //     // log;
    // }
    
    block_pointer_t target_prev = nullptr;
    block_pointer_t target_block = nullptr;
    block_size_t target_size = 0;
    
    block_pointer_t cur_block = get_memory_list_head();
    block_pointer_t next_block = nullptr;
    
    if (cur_block == nullptr)
    {
        if (req_size > get_memory_size())
        {
            // log
            // throw
        }
        
        target_block = _trusted_memory + get_meta_size();
    }
    else
    {
        block_size_t size = reinterpret_cast<unsigned char*>(cur_block) -
            (reinterpret_cast<unsigned char*>(_trusted_memory) + get_meta_size());
        
        if (size >= req_size)
        {
            target_block = _trusted_memory + get_meta_size();
            target_size = size;
        }
        
        while (cur_block != nullptr)
        {
            if (get_fit_mode() == fit_mode::first_fit && target_block)
            {
                break;
            }
            
            next_block = get_next_block(cur_block);
            
            unsigned char *r_border = reinterpret_cast<unsigned char*>(next_block);
            unsigned char *l_border = reinterpret_cast<unsigned char*>(cur_block) + 
                    get_block_meta_size() + get_block_data_size(cur_block);
            
            if (r_border == nullptr)
            {
                r_border = reinterpret_cast<unsigned char*>(_trusted_memory) +
                        get_meta_size() + get_memory_size();
            }
            
            size = r_border - l_border;
            
            if ((size >= req_size) && (get_fit_mode() == fit_mode::first_fit ||
                (get_fit_mode() == fit_mode::the_best_fit && (size - req_size < target_size - req_size)) ||
                (get_fit_mode() == fit_mode::the_worst_fit && (size - req_size > target_size - req_size))))
            {
                target_prev = cur_block;
                target_block = reinterpret_cast<block_pointer_t>(l_border);
                target_size = size;
            }
            
            cur_block = next_block;
        }
    }
    
    unsigned char *ptr = reinterpret_cast<unsigned char*>(target_block);
    
    *reinterpret_cast<block_size_t*>(ptr) = target_size;
    ptr += sizeof(block_size_t);
    
    *reinterpret_cast<block_pointer_t*>(ptr) = target_prev;
    ptr += sizeof(block_pointer_t);
    
    *reinterpret_cast<block_pointer_t*>(ptr) = nullptr;
    
    if (target_prev != nullptr)
    {
        block_pointer_t next_block = get_next_block(target_prev);
        
        if (next_block != nullptr)
        {
            *reinterpret_cast<block_pointer_t*>(ptr) = get_next_block(next_block);
            
            *reinterpret_cast<block_pointer_t*>(reinterpret_cast<unsigned char*>(next_block) +
                    sizeof(block_size_t)) = target_block;
        }
        
        *reinterpret_cast<block_pointer_t*>(reinterpret_cast<unsigned char*>(target_prev) +
                sizeof(block_size_t) + sizeof(block_pointer_t)) = target_block;
    }
    
    return target_block;
}

void allocator_boundary_tags::deallocate(
    void *at)
{
    throw not_implemented("void allocator_boundary_tags::deallocate(void *)", "your code should be here...");
}

inline void allocator_boundary_tags::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    throw not_implemented("inline void allocator_boundary_tags::set_fit_mode(allocator_with_fit_mode::fit_mode)", "your code should be here...");
}

std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const noexcept
{
    throw not_implemented("std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const noexcept", "your code should be here...");
}

inline allocator *allocator_boundary_tags::get_allocator() const
{
    unsigned char* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
    
    return *reinterpret_cast<allocator**>(ptr);
}

inline logger *allocator_boundary_tags::get_logger() const
{
    unsigned char* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
    
    ptr += sizeof(allocator*);
    
    return *reinterpret_cast<logger**>(ptr);
}

inline std::mutex &allocator_boundary_tags::get_mutex() const
{
    unsigned char* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
    
    ptr += sizeof(allocator*) + sizeof(logger*);
    
    return *reinterpret_cast<std::mutex*>(ptr);
}

inline allocator_with_fit_mode::fit_mode allocator_boundary_tags::get_fit_mode() const
{
    unsigned char* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
    
    ptr += sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex);
    
    return *reinterpret_cast<fit_mode*>(ptr);
}

inline allocator::block_size_t allocator_boundary_tags::get_memory_size() const
{
    unsigned char* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
    
    ptr += sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex) + sizeof(fit_mode);
    
    return *reinterpret_cast<block_size_t*>(ptr);
}

inline allocator::block_pointer_t allocator_boundary_tags::get_memory_list_head() const
{
    unsigned char* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
    
    ptr += sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex) +
             sizeof(fit_mode) + sizeof(block_size_t);
    
    return *reinterpret_cast<block_pointer_t**>(ptr);
}

inline size_t allocator_boundary_tags::get_meta_size() const
{
    return sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex) + sizeof(fit_mode) +
            sizeof(block_size_t) + sizeof(block_pointer_t);
}

inline allocator::block_size_t allocator_boundary_tags::get_block_meta_size() const
{
    return sizeof(block_size_t) + 2 * sizeof(block_pointer_t);
}

inline allocator::block_size_t allocator_boundary_tags::get_block_data_size(
    block_pointer_t block) const
{
    return *reinterpret_cast<block_size_t*>(block);
}

inline allocator::block_pointer_t allocator_boundary_tags::get_prev_block(
    block_pointer_t block) const
{
    return *reinterpret_cast<block_pointer_t*>(
            reinterpret_cast<unsigned char*>(block) + sizeof(block_size_t));
}

inline allocator::block_pointer_t allocator_boundary_tags::get_next_block(
    block_pointer_t block) const
{
    return *reinterpret_cast<block_pointer_t*>(
            reinterpret_cast<unsigned char*>(block) + sizeof(block_size_t) + sizeof(block_pointer_t));
}

inline std::string allocator_boundary_tags::get_typename() const noexcept
{
    return "allocator_boundary_tags";
}