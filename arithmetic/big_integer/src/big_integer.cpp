#include <cstring>
#include <string>
#include <limits>
#include <algorithm>

#include "../include/big_integer.h"

#pragma region custom multiplication implementation

big_integer &big_integer::trivial_multiplication::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier) const
{
    throw not_implemented("big_integer &big_integer::trivial_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
}

big_integer &big_integer::Karatsuba_multiplication::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier) const
{
    throw not_implemented("big_integer &big_integer::Karatsuba_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
}

big_integer &big_integer::Schonhage_Strassen_multiplication::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier) const
{
    throw not_implemented("big_integer &big_integer::Schonhage_Strassen_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
}

#pragma endregion custom multiplication implementation

#pragma region custom division implementation

big_integer &big_integer::trivial_division::divide(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::trivial_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::trivial_division::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::trivial_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Newton_division::divide(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Newton_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Newton_division::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Newton_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Burnikel_Ziegler_division::divide(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Burnikel_Ziegler_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Burnikel_Ziegler_division::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Burnikel_Ziegler_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

#pragma endregion custom multiplication implementation

#pragma region iterators implementation

    #pragma region const_iterator implementation

big_integer::const_iterator::const_iterator(
    big_integer const *holder,
    int pos):
        _holder(holder),
        _pos(pos)
{ }
    
bool big_integer::const_iterator::operator==(
    const_iterator const &other)
{
    return _holder == other._holder && _pos == other._pos;
}

bool big_integer::const_iterator::operator!=(
    const_iterator const &other)
{
    return !(*this == other);
}

unsigned int big_integer::const_iterator::operator*()
{
    return _holder->get_digit(_pos);
}

big_integer::const_iterator big_integer::const_iterator::operator++()
{
    if (_pos == _holder->get_digits_count())
    {
        return *this;
    }
    
    ++_pos;
    
    return *this;
}

big_integer::const_iterator big_integer::const_iterator::operator++(
    int unused)
{
    const_iterator it = *this;
    ++*this;
    return it;
}

    #pragma endregion const_iterator implementation

    #pragma region const_reverse_iterator implementation

big_integer::const_reverse_iterator::const_reverse_iterator(
    big_integer const *holder,
    int pos):
        _holder(holder),
        _pos(pos)
{ }
    
bool big_integer::const_reverse_iterator::operator==(
    const_reverse_iterator const &other)
{
    return _holder == other._holder && _pos == other._pos;
}

bool big_integer::const_reverse_iterator::operator!=(
    const_reverse_iterator const &other)
{
    return !(*this == other);
}

unsigned int big_integer::const_reverse_iterator::operator*()
{
    return _holder->get_digit(_pos);
}

big_integer::const_reverse_iterator big_integer::const_reverse_iterator::operator++()
{
    if (_pos == -1)
    {
        return *this;
    }
    
    --_pos;
    
    return *this;
}

big_integer::const_reverse_iterator big_integer::const_reverse_iterator::operator++(
    int unused)
{
    const_reverse_iterator it = *this;
    ++*this;
    return it;
}

    #pragma endregion const_reverse_iterator implementation

    #pragma region half_const_iterator implementation

big_integer::half_const_iterator::half_const_iterator(
    big_integer const *holder,
    int pos,
    bool is_oldest):
        _holder(holder),
        _pos(pos),
        _is_oldest(is_oldest)
{ }
    
bool big_integer::half_const_iterator::operator==(
    half_const_iterator const &other)
{
    return _holder == other._holder && _pos == other._pos && _is_oldest == other._is_oldest;
}

bool big_integer::half_const_iterator::operator!=(
    half_const_iterator const &other)
{
    return !(*this == other);
}

unsigned int big_integer::half_const_iterator::operator*()
{
    constexpr int shift = sizeof(unsigned int) << 2;
    constexpr int mask = (1 << shift) - 1;
    
    unsigned int number = _holder->get_digit(_pos);
    
    if (_is_oldest)
    {
        return number >> shift;
    }
    else
    {
        return number & mask;
    }
}

big_integer::half_const_iterator big_integer::half_const_iterator::operator++()
{
    if (_pos == _holder->get_digits_count())
    {
        return *this;
    }
    
    if (!_is_oldest)
    {
        _is_oldest = true;
        return *this;
    }
    
    ++_pos;
    _is_oldest = false;
    
    return *this;
}

big_integer::half_const_iterator big_integer::half_const_iterator::operator++(
    int unused)
{
    half_const_iterator it = *this;
    ++*this;
    return it;
}

bool big_integer::half_const_iterator::is_oldest() noexcept
{
    return _is_oldest;
}

    #pragma endregion half_const_iterator implementation

#pragma endregion iterators implementation

#pragma region general constructors

big_integer::big_integer(
    int digit,
    allocator *allocator):
        _oldest_digit(digit),
        _other_digits(nullptr),
        _allocator(allocator)
{ }

big_integer::big_integer(
    int const *digits,
    size_t digits_count,
    allocator *allocator):
        _allocator(allocator)
{
    initialize_from(digits, get_significant_digits_cnt(digits, digits_count));
}

big_integer::big_integer(
    std::vector<int> const &digits,
    allocator *allocator):
        _allocator(allocator)
{
    initialize_from(digits, get_significant_digits_cnt(digits));
}

big_integer::big_integer(
    std::string const &value_as_string,
    size_t base,
    allocator *allocator):
        _allocator(allocator)
{
    initialize_from(value_as_string, base);
}

#pragma endregion general constructors

#pragma region rule of five implementation

big_integer::~big_integer()
{
    clear();
}

big_integer::big_integer(
    big_integer const &other)
{
    copy_from(other);
}

big_integer &big_integer::operator=(
    big_integer const &other)
{
    if (this != &other)
    {
        clear();
        copy_from(other);
    }
    
    return *this;
}

big_integer::big_integer(
    big_integer &&other) noexcept
{
    move_from(std::move(other));
}

big_integer &big_integer::operator=(
    big_integer &&other) noexcept
{
    if (this != &other)
    {
        clear();
        move_from(std::move(other));
    }
    
    return *this;
}

#pragma endregion rule of five implementation

#pragma region equivalence relations implementation

bool big_integer::operator==(
    big_integer const &other) const
{
    if (get_digits_count() != other.get_digits_count())
    {
        return false;
    }
    
    auto iter = cbegin();
    auto iter_end = cend();
    auto other_iter = other.cbegin();
    auto other_iter_end = other.cend();
    
    while (iter != iter_end)
    {
        if (*iter != *other_iter)
        {
            return false;
        }
        
        ++iter;
        ++other_iter;
    }
    
    return true;
}

bool big_integer::operator!=(
    big_integer const &other) const
{
    return !(*this == other);
}

#pragma endregion equivalence relations implementation

#pragma region order relations implementation

bool big_integer::operator<(
    big_integer const &other) const
{
    if (sign() == -1 ^ other.sign() == -1)
    {
        return sign() == -1;
    }
    
    if (get_digits_count() != other.get_digits_count())
    {
        return get_digits_count() < other.get_digits_count() ^ sign() == -1;
    }
    
    auto iter = crbegin();
    auto iter_end = crend();
    auto other_iter = other.crbegin();
    auto other_iter_end = other.crend();
    
    while (iter != iter_end)
    {
        if (*iter != *other_iter)
        {
            return *iter < *other_iter ^ sign() == -1;
        }
        
        ++iter;
        ++other_iter;
    }
    
    return false;
}

bool big_integer::operator>(
    big_integer const &other) const
{
    if (sign() == -1 ^ other.sign() == -1)
    {
        return sign() == -1;
    }
    
    if (get_digits_count() != other.get_digits_count())
    {
        return get_digits_count() > other.get_digits_count() ^ sign() == -1;
    }
    
    auto iter = crbegin();
    auto iter_end = crend();
    auto other_iter = other.crbegin();
    auto other_iter_end = other.crend();
    
    while (iter != iter_end)
    {
        if (*iter != *other_iter)
        {
            return *iter > *other_iter ^ sign() == -1;
        }
        
        ++iter;
        ++other_iter;
    }
    
    return false;
}

bool big_integer::operator<=(
    big_integer const &other) const
{
    return !(*this > other);
}

bool big_integer::operator>=(
    big_integer const &other) const
{
    return !(*this < other);
}

#pragma endregion order relations implementation

#pragma region standard operations implementation

big_integer big_integer::operator-() const
{
    return big_integer(*this).change_sign();
}

big_integer &big_integer::operator+=(
    big_integer const &other)
{
    if (other.is_equal_to_zero())
    {
        return *this;
    }
    
    if (is_equal_to_zero())
    {
        return *this = other;
    }
    
    if (sign() == -1)
    {
        return change_sign()
                .operator+=(-other)
                .change_sign();
    }
    
    if (other.sign() == -1)
    {
        return *this -= -other;
    }
    
    auto const first_value_digits_count = get_digits_count();
    auto const second_value_digits_count = other.get_digits_count();
    auto const digits_count = std::max(first_value_digits_count, second_value_digits_count);
    
    constexpr int shift = sizeof(unsigned int) << 2;
    constexpr int mask = (1 << shift) - 1;
    
    size_t pos = 0;
    std::vector<int> result_digits(digits_count, 0);
    
    unsigned int operation_result = 0;
    
    auto first_iter = half_cbegin();
    auto first_iter_end = half_cend();
    auto second_iter = other.half_cbegin();
    auto seocnd_iter_end = other.half_cend();
    
    for (; first_iter != first_iter_end || second_iter != seocnd_iter_end; ++first_iter, ++second_iter)
    {
        operation_result += *first_iter + *second_iter;
        result_digits[pos] |= (operation_result & mask) << (first_iter.is_oldest() ? shift : 0);
        
        operation_result >>= shift;
        if (first_iter.is_oldest())
        {
            ++pos;
        }
    }
    
    if (operation_result)
    {
        result_digits.push_back(*reinterpret_cast<int *>(&operation_result));
    }
    
    clear();
    initialize_from(result_digits, get_significant_digits_cnt(result_digits, true));
    
    return *this;
}

big_integer big_integer::operator+(
    big_integer const &other) const
{
    return big_integer(*this) += other;
}

big_integer big_integer::operator+(
    std::pair<big_integer, allocator *> const &other) const
{
    return big_integer(0, other.second) += big_integer(*this) += other.first;
}

big_integer &big_integer::operator-=(
    big_integer const &other)
{
    if (other.is_equal_to_zero())
    {
        return *this;
    }
    
    if (is_equal_to_zero())
    {
        return *this = -other;
    }
    
    if (sign() == -1)
    {
        return change_sign()
                .operator-=(-other)
                .change_sign();
    }
    
    if (other.sign() == -1)
    {
        return *this += -other;
    }
    
    if (*this < other)
    {
        return *this = -(other - *this);
    }
    
    auto const first_value_digits_count = get_digits_count();
    auto const second_value_digits_count = other.get_digits_count();
    auto const digits_count = std::max(first_value_digits_count, second_value_digits_count);
    
    unsigned int borrow_cnt = 0;
    constexpr unsigned int decremented_borrow_value = std::numeric_limits<unsigned int>::max();
    
    size_t pos = 0;
    std::vector<int> result_digits(digits_count, 0);
    
    auto first_iter = cbegin();
    auto first_iter_end = cend();
    auto second_iter = other.cbegin();
    auto seocnd_iter_end = other.cend();
    
    for (; first_iter != first_iter_end || second_iter != seocnd_iter_end; ++first_iter, ++second_iter)
    {
        unsigned int first_value = *first_iter;
        unsigned int second_value = *second_iter;
        
        if ((first_value != 0 && first_value - borrow_cnt >= second_value) ||
                (second_value == 0 && first_value >= borrow_cnt))
        {
            result_digits[pos++] = first_value - second_value - borrow_cnt;
            borrow_cnt = 0;
        }
        else
        {
            result_digits[pos++] = (decremented_borrow_value - second_value - borrow_cnt) + 1 + first_value;
            borrow_cnt = 1;
        }
    }
    
    if (borrow_cnt)
    {
        // todo log logger error
        throw std::logic_error("Borrowing from zero\n");
    }
    
    clear();
    initialize_from(result_digits, get_significant_digits_cnt(result_digits, true));
    
    return *this;
    
}

big_integer big_integer::operator-(
    big_integer const &other) const
{
    return big_integer(*this) -= other;
}

big_integer big_integer::operator-(
    std::pair<big_integer, allocator *> const &other) const
{
    return big_integer(0, other.second) += big_integer(*this) -= other.first;
}

big_integer &big_integer::operator*=(
    big_integer const &other)
{
    if (other.is_equal_to_zero())
    {
        return *this = other;
    }
    
    if (is_equal_to_zero())
    {
        return *this;
    }
    
    if (other.is_equal_to_one())
    {
        return *this;
    }
    
    if (is_equal_to_one())
    {
        return *this = other;
    }
    
    if (sign() == -1)
    {
        return change_sign()
                .operator*=(other)
                .change_sign();
    }
    
    if (other.sign() == -1)
    {
        return operator*=(-other)
                .change_sign();
    }
    
    auto const first_value_digits_count = get_digits_count();
    auto const second_value_digits_count = other.get_digits_count();
    auto const max_digits_count = first_value_digits_count + second_value_digits_count;
    
    constexpr int shift = sizeof(unsigned int) << 2;
    constexpr int mask = (1 << shift) - 1;
    
    std::vector<int> half_digits(2 * max_digits_count, 0);
    size_t pos_shift = 0;
    
    auto first_iter = half_cbegin();
    auto first_iter_end = half_cend();
    
    for (; first_iter != first_iter_end; ++first_iter)
    {
        if (*first_iter == 0)
        {
            ++pos_shift;
            continue;
        }
        
        size_t pos = 0;
        unsigned int operation_result = 0;  
        
        auto second_iter = other.half_cbegin();
        auto second_iter_end = other.half_cend();
        
        for (; second_iter != second_iter_end; ++second_iter)
        {
            operation_result += *first_iter * *second_iter + half_digits[pos_shift + pos];
            half_digits[pos_shift + pos++] = operation_result & mask;
            
            operation_result >>= shift;
        }
        
        for (; operation_result && (pos_shift + pos) < half_digits.size(); ++pos)
        {
            operation_result += half_digits[pos_shift + pos];
            half_digits[pos_shift + pos++] = operation_result & mask;
            
            operation_result >>= shift;
        }
        
        if (operation_result)
        {
            // TODO logger log error
            throw std::logic_error("too much digit carrying while mupltiplicating");
        }
        
        ++pos_shift;
    }
    
    std::vector<int> result_digits(max_digits_count);
    
    for (size_t i = 0; i < max_digits_count; ++i)
    {
        result_digits[i] = (half_digits[2*i + 1] << shift) + half_digits[2*i];
    }
    
    clear();
    initialize_from(result_digits, get_significant_digits_cnt(result_digits, true));
    
    return *this;
}

big_integer big_integer::operator*(
    big_integer const &other) const
{
    return big_integer(*this) *= other;
}

big_integer big_integer::operator*(
    std::pair<big_integer, allocator *> const &other) const
{
    return big_integer(0, other.second) += big_integer(*this) *= other.first;
}

big_integer &big_integer::operator/=(
    big_integer const &other)
{
    auto [quotiend, _] = divide_with_remainder(*this, other, true);
    
    return *this = quotiend.value();
}

big_integer big_integer::operator/(
    big_integer const &other) const
{
    return big_integer(*this) /= other;
}

big_integer big_integer::operator/(
    std::pair<big_integer, allocator *> const &other) const
{
    return big_integer(0, other.second) += big_integer(*this) /= other.first;
}

big_integer &big_integer::operator%=(
    big_integer const &other)
{
    auto [_, remainder] = divide_with_remainder(*this, other, false);
    
    return *this = remainder;
}

big_integer big_integer::operator%(
    big_integer const &other) const
{
    return big_integer(*this) %= other;
}

big_integer big_integer::operator%(
    std::pair<big_integer, allocator *> const &other) const
{
    return big_integer(0, other.second) += big_integer(*this) %= other.first;
}

#pragma endregion standard operations implementation

#pragma region bitwise operations implementation

big_integer big_integer::operator~() const
{
    const_iterator iter = cbegin();
    const_iterator iter_end = cend();
    
    std::vector<int> result_digits;
    result_digits.reserve(get_digits_count());
    
    for (; iter != iter_end; ++iter)
    {
        result_digits.push_back(~*iter);
    }
    
    return big_integer(result_digits);
    
    return *this;
}

big_integer &big_integer::operator&=(
    big_integer const &other)
{
    const_iterator iter = cbegin();
    const_iterator iter_end = cend();
    const_iterator other_iter = other.cbegin();
    const_iterator other_iter_end = other.cend();
    
    std::vector<int> result_digits;
    result_digits.reserve(std::max(get_digits_count(), other.get_digits_count()));
    
    for (; iter != iter_end || other_iter != other_iter_end; ++iter, ++other_iter)
    {
        result_digits.push_back(*iter & *other_iter);
    }
    
    clear();
    initialize_from(result_digits, get_significant_digits_cnt(result_digits));
    
    return *this;
}

big_integer big_integer::operator&(
    big_integer const &other) const
{
    return big_integer(*this) &= other;
}

big_integer big_integer::operator&(
    std::pair<big_integer, allocator *> const &other) const
{
    return big_integer(0, other.second) += big_integer(*this) &= other.first;
}

big_integer &big_integer::operator|=(
    big_integer const &other)
{
    const_iterator iter = cbegin();
    const_iterator iter_end = cend();
    const_iterator other_iter = other.cbegin();
    const_iterator other_iter_end = other.cend();
    
    std::vector<int> result_digits;
    result_digits.reserve(std::max(get_digits_count(), other.get_digits_count()));
    
    for (; iter != iter_end || other_iter != other_iter_end; ++iter, ++other_iter)
    {
        result_digits.push_back(*iter | *other_iter);
    }
    
    clear();
    initialize_from(result_digits, get_significant_digits_cnt(result_digits));
    
    return *this;
}

big_integer big_integer::operator|(
    big_integer const &other) const
{
    return big_integer(*this) |= other;
}

big_integer big_integer::operator|(
    std::pair<big_integer, allocator *> const &other) const
{
    return big_integer(0, other.second) += big_integer(*this) |= other.first;
}

big_integer &big_integer::operator^=(
    big_integer const &other)
{
    const_iterator iter = cbegin();
    const_iterator iter_end = cend();
    const_iterator other_iter = other.cbegin();
    const_iterator other_iter_end = other.cend();
    
    std::vector<int> result_digits;
    result_digits.reserve(std::max(get_digits_count(), other.get_digits_count()));
    
    for (; iter != iter_end || other_iter != other_iter_end; ++iter, ++other_iter)
    {
        result_digits.push_back(*iter ^ *other_iter);
    }
    
    clear();
    initialize_from(result_digits, get_significant_digits_cnt(result_digits));
    
    return *this;
}

big_integer big_integer::operator^(
    big_integer const &other) const
{
    return big_integer(*this) ^= other;
}

big_integer big_integer::operator^(
    std::pair<big_integer, allocator *> const &other) const
{
    return big_integer(0, other.second) += big_integer(*this) ^= other.first;
}

big_integer &big_integer::operator<<=(
    size_t shift)
{
    if (is_equal_to_zero() || shift == 0)
    {
        return *this;
    }
    
    auto value_sign = sign();
    if (value_sign == -1)
    {
        change_sign();
    }
    
    auto const added_young_zeros = shift / (sizeof(unsigned int) << 3);
    shift %= (sizeof(unsigned int) << 3);
    
    constexpr int half_shift = sizeof(unsigned int) << 2;
    constexpr int half_mask = (1 << half_shift) - 1;
    bool is_shift_bigger_than_half = shift >= half_shift;
    shift %= half_shift;
    
    size_t pos = added_young_zeros;
    std::vector<int> result_digits(get_digits_count() + added_young_zeros + 1, 0);
    
    auto iter = half_cbegin();
    auto iter_end = half_cend();
    
    for (; iter != iter_end; ++iter)
    {
        bool is_writing_oldest_firstly = iter.is_oldest() ^ is_shift_bigger_than_half;
        
        unsigned int operation_result = *iter << shift;
        result_digits[pos] |= (operation_result & half_mask) << (is_writing_oldest_firstly ? half_shift : 0);
        
        if (is_writing_oldest_firstly)
        {
            ++pos;
        }
        
        result_digits[pos] |= (operation_result >> half_shift) << (is_writing_oldest_firstly ? 0 : half_shift);
    }
    
    clear();
    initialize_from(result_digits, get_significant_digits_cnt(result_digits, true));
    
    if (value_sign == -1)
    {
        change_sign();
    }
    
    return *this;
}

big_integer big_integer::operator<<(
    size_t shift) const
{
    return big_integer(*this) <<= shift;
}

big_integer big_integer::operator<<(
    std::pair<size_t, allocator *> const &shift) const
{
    return big_integer(0, shift.second) += big_integer(*this) <<= shift.first;
}

big_integer &big_integer::operator>>=(
    size_t shift)
{
    if (is_equal_to_zero() || shift == 0)
    {
        return *this;
    }
    
    auto value_sign = sign();
    if (value_sign == -1)
    {
        change_sign();
    }
    
    auto const erased_digits = shift / (sizeof(unsigned int) << 3);
    shift %= (sizeof(unsigned int) << 3);
    
    if (erased_digits >= get_digits_count())
    {
        *this = big_integer(0);
        if (value_sign == -1)
        {
            change_sign();
        }
        return *this;
    }
    
    constexpr int half_shift = sizeof(unsigned int) << 2;
    constexpr int half_mask = (1 << half_shift) - 1;
    bool is_shift_bigger_than_half = shift >= half_shift;
    shift %= half_shift;
    
    size_t pos = 0;
    std::vector<int> result_digits(get_digits_count() - erased_digits, 0);
    
    auto iter = half_cbegin();
    auto iter_end = half_cend();
    
    for (size_t i = 0; i < 2*erased_digits; ++i)
    {
        ++iter;
    }
    
    for (; iter != iter_end; ++iter)
    {
        bool is_writing_oldest_firstly = iter.is_oldest() == is_shift_bigger_than_half;
        unsigned int operation_result = *iter << half_shift >> shift;
        
        if (pos > 0)
        {
            result_digits[pos - 1] |= (operation_result & half_mask) << (is_writing_oldest_firstly ? half_shift : 0);
        }
        
        if (is_writing_oldest_firstly)
        {
            ++pos;
        }
        
        if (pos > 0)
        {
            result_digits[pos - 1] |= (operation_result >> half_shift) << (is_writing_oldest_firstly ? 0 : half_shift);
        }
    }
    
    clear();
    initialize_from(result_digits, get_significant_digits_cnt(result_digits, true));
    
    if (value_sign == -1)
    {
        change_sign();
    }
    
    return *this;
}

big_integer big_integer::operator>>(
    size_t shift) const
{
    return big_integer(*this) <<= shift;
}

big_integer big_integer::operator>>(
    std::pair<size_t, allocator *> const &shift) const
{
    return big_integer(0, shift.second) += big_integer(*this) <<= shift.first;
}

#pragma endregion bitwise operations implementation

#pragma region custom multiplication and division implementation

big_integer &big_integer::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier,
    allocator *allocator,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer &big_integer::multiply(big_integer &, big_integer const &, allocator *, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer big_integer::multiply(
    big_integer const &first_multiplier,
    big_integer const &second_multiplier,
    allocator *allocator,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer big_integer::multiply(big_integer const &, big_integer const &, allocator *, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::divide(
    big_integer &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer &big_integer::divide(big_integer &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer big_integer::divide(
    big_integer const &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer big_integer::divide(big_integer const &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer &big_integer::modulo(big_integer &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer big_integer::modulo(
    big_integer const &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer big_integer::modulo(big_integer const &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

#pragma endregion custom multiplication and division implementation

#pragma region IO implementation

std::ostream &operator<<(
    std::ostream &stream,
    big_integer const &value)
{
    return stream << value.to_string();
}

std::istream &operator>>(
    std::istream &stream,
    big_integer &value)
{
    std::string value_as_string;
    
    stream >> value_as_string;
    
    value.clear();
    value.initialize_from(value_as_string, 10);
    
    return stream;
}

#pragma endregion IO implementation

#pragma region common integer functions implementation

inline int big_integer::get_digits_count() const noexcept
{
    return static_cast<int>(_other_digits == nullptr
         ? 1
         : *_other_digits);
}

inline int big_integer::sign() const noexcept
{
    if (is_equal_to_zero())
    {
        return 0;
    }

    return 1 - (static_cast<int>((*reinterpret_cast<unsigned int const *>(&_oldest_digit) >> ((sizeof(int) << 3) - 1))) << 1);
}

inline bool big_integer::is_equal_to_zero() const noexcept
{
    return _oldest_digit == 0 && _other_digits == nullptr;
}

inline bool big_integer::is_equal_to_one() const noexcept
{
    return _oldest_digit == 1 && _other_digits == nullptr;
}

inline unsigned int big_integer::get_digit(
    int position) const noexcept
{
    if (_other_digits == nullptr)
    {
        return position == 0 ? _oldest_digit : 0;
    }

    int const digits_count = get_digits_count();
    if (position < digits_count - 1)
    {
        return _other_digits[position + 1];
    }

    if (position == digits_count - 1)
    {
        return _oldest_digit;
    }

    return 0;
}

#pragma endregion common integer functions implementation

void big_integer::dump_value(
    std::ostream &stream) const
{
    for (auto i = 0; i < get_digits_count(); ++i)
    {
        auto digit = get_digit(i);
        dump_int_value(stream, *reinterpret_cast<int *>(&digit));
        stream << ' ';
    }
}

std::string big_integer::to_string() const
{
    std::string str;
    str.reserve(11 * get_digits_count());
    
    big_integer big_number(*this);
    int sign = big_number.sign();
    
    if (sign == -1)
    {
        big_number.change_sign();
    }
    
    size_t const big_modulus_zeros_cnt = 9;
    size_t big_modulus = 1;
    
    for (size_t i = 0; i < big_modulus_zeros_cnt; ++i)
    {
        big_modulus *= 10;
    }
    
    while (!big_number.is_equal_to_zero())
    {
        auto [quotient, remainder] = big_integer::divide_with_remainder(big_number, big_integer(big_modulus), true);
        
        unsigned int tmp = remainder.get_digit(0);
        for (size_t i = 0; i < big_modulus_zeros_cnt && (tmp || !quotient.value().is_equal_to_zero()); ++i)
        {
            str.push_back('0' + tmp % 10);
            tmp /= 10;
        }
        
        big_number = quotient.value();
    }
    
    if (is_equal_to_zero())
    {
        str.push_back('0');
    }
    
    if (sign == -1)
    {
        str.push_back('-');
    }
    
    std::reverse(str.begin(), str.end());
    
    return str;
}

#pragma region utility methods implementation

void big_integer::clear()
{
    deallocate_with_guard(_other_digits);
    
    _oldest_digit = 0;
    _other_digits = nullptr;
    _allocator = nullptr;
}

void big_integer::copy_from(
    big_integer const &other)
{
    _oldest_digit = other._oldest_digit;
    _other_digits = nullptr;
    _allocator = other._allocator;
    
    if (other._other_digits == nullptr)
    {
        return;
    }
    
    try
    {
        _other_digits = reinterpret_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), other.get_digits_count()));
    }
    catch (std::bad_alloc const &)
    {
        clear();
        // TODO log logger error
        throw;
    }
    
    std::memcpy(_other_digits, other._other_digits, sizeof(unsigned int) * other.get_digits_count());
}

void big_integer::move_from(
    big_integer &&other)
{
    _oldest_digit = other._oldest_digit;
    _other_digits = other._other_digits;
    _allocator = other._allocator;
    
    other._oldest_digit = 0;
    other._other_digits = nullptr;
    other._allocator = nullptr;
}

void big_integer::initialize_from(
    int const *digits,
    size_t digits_count)
{
    if (digits == nullptr)
    {
        // TODO log logger error
        throw std::logic_error("pointer to digits array must not be nullptr");
    }

    if (digits_count == 0)
    {
        // TODO log logger error
        throw std::logic_error("digits array length must be greater than 0");
    }
    
    if (digits_count > std::numeric_limits<unsigned char>::max())
    {
        // TODO log logger error
        throw std::logic_error("digits array length must be not greater than unsigned int max value");
    }

    _oldest_digit = digits[digits_count - 1];
    _other_digits = nullptr;
    
    if (digits_count == 1)
    {
        return;
    }
    
    try
    {
        _other_digits = reinterpret_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), digits_count));
    }
    catch (std::bad_alloc const &)
    {
        clear();
        // TODO log logger error
        throw;
    }
    
    *_other_digits = static_cast<unsigned int>(digits_count);
    
    std::memcpy(_other_digits + 1, digits, sizeof(unsigned int) * (digits_count - 1));
}

void big_integer::initialize_from(
    std::vector<int> const &digits,
    size_t digits_count)
{
    if (digits.empty() || digits_count == 0)
    {
        // TODO log logger error
        throw std::logic_error("std::vector<int> of digits should not be empty");
    }
    
    _oldest_digit = digits_count <= digits.size() ? digits[digits_count - 1] : 0;
    _other_digits = nullptr;
    
    if (digits_count == 1)
    {
        return;
    }
    
    try
    {
        _other_digits = reinterpret_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), digits_count));
    }
    catch (std::bad_alloc const &)
    {
        clear();
        // TODO log logger error
        throw;
    }
    
    *_other_digits = static_cast<unsigned int>(digits_count);
    
    for (auto i = 0; i < digits_count - 1; ++i)
    {
        _other_digits[1 + i] = (i < digits.size()
                ? *reinterpret_cast<unsigned int const *>(&digits[i])
                : 0);
    }
}

void big_integer::initialize_from(
    std::string const &value_as_string,
    size_t base)
{
    _oldest_digit = 0;
    _other_digits = nullptr;
    
    bool negative_flag = false;
    size_t pos = 0;
    
    if (value_as_string[pos] == '-')
    {
        negative_flag = true;
        ++pos;
    }
    
    for (; pos < value_as_string.size(); ++pos)
    {
        unsigned int digit = char_to_int(value_as_string[pos]);
        
        if (digit >= base)
        {
            throw std::logic_error("Invalid digit");
        }
        
        *this *= big_integer(base, this->_allocator);
        *this += big_integer(digit, this->_allocator);
    }
    
    if (negative_flag)
    {
        change_sign();
    }
}

big_integer &big_integer::change_sign()
{
    _oldest_digit ^= (1 << ((sizeof(int) << 3) - 1));
    
    // if (_oldest_digit == 0)
    // {
    //     _oldest_digit = _other_digits[get_digits_count() - 1];
    //     --(*_other_digits);      
    // }
    // else if (_oldest_digit == std::numeric_limits<int>::min())
    // {
    //     _other_digits[get_digits_count()] = _oldest_digit;
    //     _oldest_digit = 0;
    //     ++(*_other_digits); 
    // }
    // else
    // {
    //     _oldest_digit = -_oldest_digit;
    // }

    return *this;
}

void big_integer::print_byte(
    std::ostream &stream,
    unsigned char byte_value)
{
    for (int i = 0; i < 8; i++)
    {
        stream << ((byte_value >> (7 - i)) & 1);
    }
}

void big_integer::dump_int_value(
    std::ostream &stream,
    int value)
{
    auto *p = reinterpret_cast<unsigned char *>(&value);
    for (int i = 0; i < sizeof(int); i++)
    {
        print_byte(stream, *p++);
        stream << ' ';
    }
}

size_t big_integer::get_significant_digits_cnt(
    int const *digits,
    size_t digits_count,
    bool forced_to_be_positive)
{
    size_t significant_digits_cnt = digits_count;
    
    while (significant_digits_cnt > 2 && digits[significant_digits_cnt - 1] == 0 && digits[significant_digits_cnt - 2] == 0)
    {
        --significant_digits_cnt;
    }
    
    if (significant_digits_cnt > 1 && digits[significant_digits_cnt - 1] == 0)
    {
        auto maybe_overflowed_digit_ptr = reinterpret_cast<unsigned int const *>(&digits[significant_digits_cnt - 2]);
        if (*maybe_overflowed_digit_ptr >> ((sizeof(unsigned int) << 3) - 1) == 0)
        {
            --significant_digits_cnt;
        }
    }
    
    if (forced_to_be_positive && digits[significant_digits_cnt - 1] != 0)
    {
        auto maybe_overflowed_digit_ptr = reinterpret_cast<unsigned int const *>(&digits[significant_digits_cnt - 1]);
        if (*maybe_overflowed_digit_ptr >> ((sizeof(unsigned int) << 3) - 1) == 1)
        {
            ++significant_digits_cnt;
        }
    }
    
    return significant_digits_cnt;
}

size_t big_integer::get_significant_digits_cnt(
    std::vector<int> const &digits,
    bool forced_to_be_positive)
{
    size_t significant_digits_cnt = digits.size();
    
    while (significant_digits_cnt > 2 && digits[significant_digits_cnt - 1] == 0 && digits[significant_digits_cnt - 2] == 0)
    {
        --significant_digits_cnt;
    }
    
    if (significant_digits_cnt > 1 && digits[significant_digits_cnt - 1] == 0)
    {
        auto maybe_overflowed_digit_ptr = reinterpret_cast<unsigned int const *>(&digits[significant_digits_cnt - 2]);
        if (*maybe_overflowed_digit_ptr >> ((sizeof(unsigned int) << 3) - 1) == 0)
        {
            --significant_digits_cnt;
        }
    }
    
    if (forced_to_be_positive && digits[significant_digits_cnt - 1] != 0)
    {
        auto maybe_overflowed_digit_ptr = reinterpret_cast<unsigned int const *>(&digits[significant_digits_cnt - 1]);
        if (*maybe_overflowed_digit_ptr >> ((sizeof(unsigned int) << 3) - 1) == 1)
        {
            ++significant_digits_cnt;
        }
    }
    
    return significant_digits_cnt;
}

std::pair<std::optional<big_integer>, big_integer> big_integer::divide_with_remainder(
    big_integer const &dividend,
    big_integer const &divisor,
    bool eval_quotient)
{
    if (divisor.is_equal_to_zero())
    {
        // todo log logger log custom error
        throw std::logic_error("attempt to divide by zero");
    }
    
    if (dividend.is_equal_to_zero())
    {
        return std::make_pair(std::optional(big_integer(0)), big_integer(0));
    }
    
    if (divisor.is_equal_to_one())
    {
        return std::make_pair(std::optional(dividend), big_integer(0));
    }
    
    if (dividend.sign() == -1)
    {
        auto [quotient, remainder] = divide_with_remainder(-dividend, divisor, eval_quotient);
        
        if (quotient.has_value())
        {
            return std::make_pair(std::optional(-quotient.value()), -remainder);
        }
        else
        {
            return std::make_pair(std::optional<big_integer>(), -remainder);
        }
    }
    
    if (divisor.sign() == -1)
    {
        auto [quotient, remainder] = divide_with_remainder(dividend, -divisor, eval_quotient);
        
        if (quotient.has_value())
        {
            return std::make_pair(std::optional(-quotient.value()), -remainder);
        }
        else
        {
            return std::make_pair(std::optional<big_integer>(), -remainder);
        }
    }
    
    auto const dividend_digits_count = dividend.get_digits_count();
    
    std::vector<int> result_digits(1, 0);
    big_integer minuend(0);
    
    for (size_t i = 0; i < dividend_digits_count; ++i)
    {
        unsigned int cur_digit = dividend.get_digit(dividend_digits_count - i - 1);
        
        minuend <<= 8 * sizeof(unsigned int);
        minuend += big_integer(std::vector<int>( {*reinterpret_cast<int *>(&cur_digit), 0} ));
        
        if (minuend < divisor)
        {
            result_digits.push_back(0);
        }
        else
        {
            unsigned int digit = 0;
            big_integer subtrahend(0);
            
            for (unsigned int multiplier = 1 << (8*sizeof(unsigned int) - 1); multiplier > 0; multiplier >>= 1)
            {
                int multiplier_int = *reinterpret_cast<int *>(&multiplier);
                big_integer tmp = divisor * big_integer(std::vector<int>( {multiplier_int, 0} ));
                
                if (minuend >= subtrahend + tmp)
                {
                    subtrahend += tmp;
                    digit += multiplier;
                }
            }
            
            minuend -= subtrahend;
            
            if (eval_quotient)
            {
                result_digits.push_back(digit);
            }
        }
    }
    
    if (eval_quotient)
    {
        std::reverse(result_digits.begin(), result_digits.end());
        
        return std::make_pair(std::optional(big_integer(result_digits)), minuend);
    }
    
    return std::make_pair(std::optional<big_integer>(), minuend);
}

unsigned int big_integer::char_to_int(
    char ch)
{
    if ('0' <= ch && ch <= '9')
    {
        return ch - '0';
    }
    if ('A' <= ch && ch <= 'Z')
    {
        return ch - 'A' + 10;
    }
    if ('a' <= ch && ch <= 'z')
    {
        return ch - 'a' + 36;
    }
    
    // todo logger log error
    throw std::logic_error("Invalid digit");
}

static void swap(
    big_integer &lhs,
    big_integer &rhs)
{
    big_integer tmp = std::move(lhs);
    lhs = std::move(rhs);
    rhs = std::move(lhs);
}

#pragma endregion utility methods implementation

#pragma region iterator requesting implementation

big_integer::const_iterator big_integer::cbegin() const
{
    return const_iterator(this);
}

big_integer::const_iterator big_integer::cend() const
{
    return const_iterator(this, get_digits_count());
}

big_integer::const_reverse_iterator big_integer::crbegin() const
{
    return const_reverse_iterator(this, get_digits_count() - 1);
}

big_integer::const_reverse_iterator big_integer::crend() const
{
    return const_reverse_iterator(this, -1);
}

big_integer::half_const_iterator big_integer::half_cbegin() const
{
    return half_const_iterator(this);
}

big_integer::half_const_iterator big_integer::half_cend() const
{
    return half_const_iterator(this, get_digits_count());
}

#pragma endregion iterator requesting implementation

[[nodiscard]] allocator *big_integer::get_allocator() const noexcept
{
    return _allocator;
}