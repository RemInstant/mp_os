#include <string>
#include <sstream>
#include <iomanip>

#include <not_implemented.h>

#include "../include/allocator_global_heap.h"

allocator_global_heap::allocator_global_heap(
    logger *logger):
    _logger(logger)
{
    trace_with_guard("allocator_global_heap::allocator_global_heap(logger *) was called");
    
    trace_with_guard("allocator_global_heap::allocator_global_heap(logger *) successfully finished its work");
}

allocator_global_heap::~allocator_global_heap()
{
    trace_with_guard("allocator_global_heap::~allocator_global_heap() was called");
    
    trace_with_guard("allocator_global_heap::~allocator_global_heap() successfully finished its work");
}

allocator_global_heap::allocator_global_heap(
    allocator_global_heap &&other) noexcept:
    _logger(other._logger)
{
    trace_with_guard("allocator_global_heap::allocator_global_heap(allocator_global_heap &&) was called");
    
    trace_with_guard("allocator_global_heap::allocator_global_heap(allocator_global_heap &&) successfully finished its work");
}

allocator_global_heap &allocator_global_heap::operator=(
    allocator_global_heap &&other) noexcept
{
    trace_with_guard("allocator_global_heap::operator=(allocator_global_heap &&) was called");
    
    if (this != &other)
    {
        _logger = other._logger;
        other._logger = nullptr;
    }
    
    trace_with_guard("allocator_global_heap::operator=(allocator_global_heap &&) successfully finished its work");
    
    return *this;
}

[[nodiscard]] void *allocator_global_heap::allocate(
    size_t value_size,
    size_t values_count)
{
    trace_with_guard("allocator_global_heap::allocate(size_t, size_t) was called (value size = " +
            std::to_string(value_size) + "; count = " + std::to_string(values_count) + ")");
    debug_with_guard("allocator_global_heap::allocate(size_t, size_t) was called (value size = " +
            std::to_string(value_size) + "; count = " + std::to_string(values_count) + ")");
    
    unsigned char *ptr = nullptr;
    size_t size = value_size * values_count;
    
    try
    {
        ptr = reinterpret_cast<unsigned char*>(
                ::operator new(size + sizeof(allocator_global_heap*) + sizeof(size_t)));
    }
    catch(const std::bad_alloc& ba)
    {
        error_with_guard("allocator_global_heap::allocate(size_t, size_t): Bad alloc was occured (size = "
                + std::to_string(size) + ")");
        throw;
    }
    
    *reinterpret_cast<allocator_global_heap**>(ptr) = this;
    *reinterpret_cast<size_t*>(ptr + sizeof(allocator_global_heap*)) = size;
    
    trace_with_guard("allocator_global_heap::allocate(size_t, size_t) successfully finished its work");
    debug_with_guard("allocator_global_heap::allocate(size_t, size_t) successfully finished its work");
    
    return reinterpret_cast<void*>(ptr + sizeof(allocator_global_heap*) + sizeof(size_t));
}

void allocator_global_heap::deallocate(
    void *at)
{
    trace_with_guard("allocator_global_heap::deallocate(void *) was called");
    debug_with_guard("allocator_global_heap::deallocate(void *) was called");
    
    auto ptr = reinterpret_cast<unsigned char*>(at) - sizeof(allocator_global_heap*) - sizeof(size_t);
    
    if (*reinterpret_cast<allocator_global_heap**>(ptr) != this)
    {
        error_with_guard("allocator_global_heap::deallocate(void *): allocator tried to deallocate wrong memory");
        throw std::logic_error("allocator tried to deallocate wrong memory");
    }
    
    size_t size = *reinterpret_cast<size_t*>(ptr + sizeof(allocator_global_heap*));
    
    std::ostringstream str_stream;
    
    if (size > 0)
    {
        str_stream << " with data: ";
    }
    
    str_stream << std::hex << std::setfill('0');
    
    for (size_t i = 0; i < size; ++i)
    {
        str_stream << "0x" << std::setw(2) << static_cast<unsigned int>
                (*(ptr + sizeof(allocator_global_heap*) + sizeof(size_t*) + i)) << ' ';
    }
    
    debug_with_guard("allocator_global_heap::deallocate(void *): deallocated " +
            std::to_string(size) + " bytes" + str_stream.str());
    
    ::operator delete(ptr);
    
    trace_with_guard("allocator_global_heap::deallocate(void *) successfully finished its work");
    debug_with_guard("allocator_global_heap::deallocate(void *) successfully finished its work");
}

inline logger *allocator_global_heap::get_logger() const
{
    return _logger;
}

inline std::string allocator_global_heap::get_typename() const noexcept
{
    return "allocator_global_heap";
}
