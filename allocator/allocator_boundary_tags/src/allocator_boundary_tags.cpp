#include <mutex>

#include <not_implemented.h>

#include "../include/allocator_boundary_tags.h"

allocator_boundary_tags::~allocator_boundary_tags()
{
    trace_with_guard(get_typename() + "::~allocator_boundary_tags() was called");
    
    get_mutex().~mutex();
    deallocate_with_guard(_trusted_memory);
    
    trace_with_guard(get_typename() + "::~allocator_boundary_tags() successfully finished its work");
}

allocator_boundary_tags::allocator_boundary_tags(
    allocator_boundary_tags &&other) noexcept:
    _trusted_memory(other._trusted_memory)
{
    trace_with_guard(get_typename() + "::allocator_boundary_tags(allocator_boundary_tags &&) was called");
    
    other._trusted_memory = nullptr;
    
    trace_with_guard(get_typename() + "::allocator_boundary_tags(allocator_boundary_tags &&) successfully finished its work");
}

allocator_boundary_tags &allocator_boundary_tags::operator=(
    allocator_boundary_tags &&other) noexcept
{
    trace_with_guard(get_typename() + "::operator=(allocator_boundary_tags &&) was called");
    
    get_mutex().~mutex();
    deallocate_with_guard(_trusted_memory);
    
    _trusted_memory = other._trusted_memory;
    other._trusted_memory = nullptr;
    
    trace_with_guard(get_typename() + "::operator=(allocator_boundary_tags &&) successfully finished its work");
    
    return *this;
}

allocator_boundary_tags::allocator_boundary_tags(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    trace_with_guard(get_typename() + "::~allocator_boundary_tags() was called");
    
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
                ? ::operator new(space_size + get_allctr_meta_size())
                : parent_allocator->allocate(space_size + get_allctr_meta_size(), 1);
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
    
    *reinterpret_cast<block_size_t*>(ptr) = space_size;
    ptr += sizeof(block_size_t);
    
    *reinterpret_cast<block_pointer_t*>(ptr) = nullptr;
    
    trace_with_guard(get_typename() + "::~allocator_boundary_tags() successfully finished its work");
}

[[nodiscard]] void *allocator_boundary_tags::allocate(
    size_t value_size,
    size_t values_count)
{
    trace_with_guard(get_typename() + "::~allocator_boundary_tags() was called");
    
    block_size_t req_size = value_size * values_count;
    block_size_t cmn_size = req_size + get_block_meta_size();
    
    block_pointer_t target_prev = nullptr;
    block_pointer_t target_block = nullptr;
    block_pointer_t target_next = nullptr;
    block_size_t target_size = 0;
    
    block_pointer_t &head_block = get_head_block();
    block_pointer_t cur_block = head_block;
    block_pointer_t next_block = nullptr;
    block_size_t size = 0;
    
    size = cur_block == nullptr
            ? get_allctr_data_size()
            : (reinterpret_cast<unsigned char*>(cur_block) - get_block_meta_size()) -
                (reinterpret_cast<unsigned char*>(_trusted_memory) + get_allctr_meta_size());
    
    if (size >= cmn_size)
    {
        target_block = reinterpret_cast<unsigned char*>(_trusted_memory) +
                get_allctr_meta_size() + get_block_meta_size();
        target_size = size;
    }
    
    while (cur_block != nullptr)
    {
        if (get_fit_mode() == fit_mode::first_fit && target_block)
        {
            break;
        }
        
        next_block = get_next_block(cur_block);
        
        unsigned char *r_border = next_block == nullptr
                ? reinterpret_cast<unsigned char*>(
                    _trusted_memory) + get_allctr_meta_size() + get_allctr_data_size()
                : reinterpret_cast<unsigned char*>(
                    next_block) - get_block_meta_size();
        
        unsigned char *l_border = reinterpret_cast<unsigned char*>(
                cur_block) + get_block_data_size(cur_block);
               
        size = r_border - l_border;
        
        if ((size >= cmn_size) && (get_fit_mode() == fit_mode::first_fit ||
            (get_fit_mode() == fit_mode::the_best_fit && (size - req_size < target_size - req_size)) ||
            (get_fit_mode() == fit_mode::the_worst_fit && (size - req_size > target_size - req_size))))
        {
            target_prev = cur_block;
            target_block = reinterpret_cast<block_pointer_t>(reinterpret_cast<unsigned char*>(
                    l_border) + get_block_meta_size());
            target_size = size;
        }
        
        cur_block = next_block;
    }
    
    if (target_block == nullptr)
    {
        error_with_guard(get_typename() + "::allocate(size_t, size_t) : no space to allocate requested memory");
        throw std::bad_alloc();
    }
    
    if (target_size - cmn_size < get_block_meta_size())
    {
        req_size += target_size - cmn_size;
        cmn_size = target_size;
        // log from req_size to cmn_size - get_block_meta_size();
    }
    
    
    if (target_prev != nullptr)
    {
        target_next = get_next_block(target_prev);
        get_next_block(target_prev) = target_block;
    }
    else
    {
        target_next = head_block;
    }
    
    if (target_next != nullptr)
    {
        get_prev_block(target_next) = target_block;
    }
    
    get_block_data_size(target_block) = req_size;
    get_prev_block(target_block) = target_prev;
    get_next_block(target_block) = target_next;
    
    if (head_block == nullptr || head_block == get_next_block(target_block))
    {
        get_head_block() = target_block;
    }
    
    get_allctr_avail_size() -= cmn_size;
    
    trace_with_guard(get_typename() + "::~allocator_boundary_tags() successfully finished its work");
    
    return target_block;
}

void allocator_boundary_tags::deallocate(
    void *at)
{
    trace_with_guard(get_typename() + "::~allocator_boundary_tags() was called");
    
    if (at == nullptr)
    {
        return;
    }
    
    unsigned char* mem_begin = reinterpret_cast<unsigned char*>(
            _trusted_memory) + get_allctr_meta_size();
    unsigned char* mem_end = reinterpret_cast<unsigned char*>(
            _trusted_memory) + get_allctr_meta_size() + get_allctr_data_size();
    
    if (at < mem_begin || at >= mem_end)
    {
        error_with_guard(get_typename() + "::deallocate(void *) : tried to deallocate non-related memory");
        throw std::logic_error(get_typename() + "::deallocate(void *) : tried to deallocate non-related memory");
    }
    
    block_pointer_t prev_block = get_prev_block(at);
    block_pointer_t next_block = get_next_block(at);
    
    if (prev_block && (prev_block < mem_begin || prev_block >= mem_end) ||
        next_block && (next_block < mem_begin || next_block >= mem_end))
    {
        error_with_guard(get_typename() + "::deallocate(void *) : pointer does not point on allocated memory");
        throw std::logic_error(get_typename() + "::deallocate(void *) : pointer does not point on allocated memory");
    }
    
    if (prev_block == nullptr)
    {
        get_head_block() = next_block;
    }
    else
    {
        get_next_block(prev_block) = next_block;
    }
    
    if (next_block != nullptr)
    {
        get_prev_block(next_block) = prev_block;
    }
    
    get_allctr_avail_size() += get_block_meta_size() + get_block_data_size(at);
    
    trace_with_guard(get_typename() + "::~allocator_boundary_tags() successfully finished its work");
}

inline void allocator_boundary_tags::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    get_fit_mode() = mode;
}

std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const noexcept
{
    trace_with_guard(get_typename() + "::~allocator_boundary_tags() was called");
    
    std::vector<allocator_test_utils::block_info> blocks_state(0);
    
    block_pointer_t cur_block = get_head_block();
    block_pointer_t next_block = nullptr;
    block_size_t free_size, occupied_size;
    
    free_size = cur_block == nullptr
            ? get_allctr_data_size()
            : (reinterpret_cast<unsigned char*>(cur_block) - get_block_meta_size()) -
                (reinterpret_cast<unsigned char*>(_trusted_memory) + get_allctr_meta_size());
    
    if (free_size)
    {
        blocks_state.push_back( { .block_size = free_size, .is_block_occupied = false } );
    }
    
    while (cur_block != nullptr)
    {
        next_block = get_next_block(cur_block);
        
        unsigned char *r_border = next_block == nullptr
                ? reinterpret_cast<unsigned char*>(
                    _trusted_memory) + get_allctr_meta_size() + get_allctr_data_size()
                : reinterpret_cast<unsigned char*>(
                    next_block) - get_block_meta_size();
        
        unsigned char *l_border = reinterpret_cast<unsigned char*>(
                cur_block) + get_block_data_size(cur_block);
        
        free_size = r_border - l_border;
        occupied_size = get_block_meta_size() + get_block_data_size(cur_block);
        
        
        blocks_state.push_back( { .block_size = occupied_size, .is_block_occupied = true } );
        
        if (free_size)
        {
            blocks_state.push_back( { .block_size = free_size, .is_block_occupied = false } );
        }
        
        cur_block = next_block;
    }
    
    trace_with_guard(get_typename() + "::~allocator_boundary_tags() successfully finished its work");
    
    return blocks_state;
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

inline allocator_with_fit_mode::fit_mode &allocator_boundary_tags::get_fit_mode() const
{
    unsigned char* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
    
    ptr += sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex);
    
    return *reinterpret_cast<fit_mode*>(ptr);
}

inline allocator::block_size_t allocator_boundary_tags::get_allctr_data_size() const
{
    unsigned char* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
    
    ptr += sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex) + sizeof(fit_mode);
    
    return *reinterpret_cast<block_size_t*>(ptr);
}

inline allocator::block_size_t &allocator_boundary_tags::get_allctr_avail_size() const
{
    unsigned char* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
    
    ptr += sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex) +
             sizeof(fit_mode) + sizeof(block_size_t);
    
    return *reinterpret_cast<block_size_t*>(ptr);
}

inline allocator::block_pointer_t &allocator_boundary_tags::get_head_block() const
{
    unsigned char* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
    
    ptr += sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex) +
             sizeof(fit_mode) + 2 * sizeof(block_size_t);
    
    return *reinterpret_cast<block_pointer_t*>(ptr);
}

inline size_t allocator_boundary_tags::get_allctr_meta_size() const
{
    return sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex) + sizeof(fit_mode) +
            2 * sizeof(block_size_t) + sizeof(block_pointer_t);
}

inline allocator::block_size_t allocator_boundary_tags::get_block_meta_size() const
{
    return sizeof(block_size_t) + 2 * sizeof(block_pointer_t);
}

inline allocator::block_size_t &allocator_boundary_tags::get_block_data_size(
    block_pointer_t block) const
{
    return *reinterpret_cast<block_size_t*>(block);
}

inline allocator::block_pointer_t &allocator_boundary_tags::get_prev_block(
    block_pointer_t block) const
{
    return *reinterpret_cast<block_pointer_t*>(
            reinterpret_cast<unsigned char*>(block) + sizeof(block_size_t));
}

inline allocator::block_pointer_t &allocator_boundary_tags::get_next_block(
    block_pointer_t block) const
{
    return *reinterpret_cast<block_pointer_t*>(
            reinterpret_cast<unsigned char*>(block) + sizeof(block_size_t) + sizeof(block_pointer_t));
}

inline std::string allocator_boundary_tags::get_typename() const noexcept
{
    return "allocator_boundary_tags";
}
