#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_BIGINT_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_BIGINT_H

#include <iostream>
#include <map>
#include <vector>
#include <optional>

#include <allocator.h>
#include <allocator_guardant.h>
#include <not_implemented.h>

class big_integer final:
    allocator_guardant
{

public:
    
    enum class multiplication_rule
    {
        trivial,
        Karatsuba,
        SchonhageStrassen
    };

private:
    
    class multiplication
    {
    
    public:
        
        virtual ~multiplication() noexcept = default;
    
    public:
        
        virtual big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const = 0;
        
    };
    
    class trivial_multiplication final:
        public multiplication
    {
    
    public:
        
        big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const override;
        
    };
    
    class Karatsuba_multiplication final:
        public multiplication
    {

    public:
        
        big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const override;
        
    };
    
    class Schonhage_Strassen_multiplication final:
        public multiplication
    {

    public:
        
        big_integer &multiply(
            big_integer &first_multiplier,
            big_integer const &second_multiplier) const override;
        
    };

public:
    
    enum class division_rule
    {
        trivial,
        Newton,
        BurnikelZiegler
    };

private:
    
    class division
    {
    
    public:
        
        virtual ~division() noexcept = default;
    
    public:
        
        virtual big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const = 0;
        
        virtual big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const = 0;
        
    };
    
    class trivial_division final:
        public division
    {
    
    public:
    
        big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
        
        big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
    
    private:
        
        std::pair<std::optional<big_integer>, big_integer> divide_with_remainder(
            big_integer const &dividend,
            big_integer const &divisor,
            bool eval_quotient,
            big_integer::multiplication_rule multiplication_rule) const;
    
    
    };
    
    class Newton_division final:
        public division
    {
    
    public:
        
        big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
        
        big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
        
    };
    
    class Burnikel_Ziegler_division final:
        public division
    {
    
    public:
        
        big_integer &divide(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
        
        big_integer &modulo(
            big_integer &dividend,
            big_integer const &divisor,
            big_integer::multiplication_rule multiplication_rule) const override;
        
    };

private:

    class const_iterator final
    {
        
    private:
        
        big_integer const *_holder;
        int _pos;
        
    public:
    
        const_iterator(
            big_integer const *holder,
            int pos = 0);
        
    public:
    
        bool operator==(
            const_iterator const &other);
        
        bool operator!=(
            const_iterator const &other);
        
        unsigned int operator*();
        
        const_iterator operator++();
        
        const_iterator operator++(
            int unused);
    
    };
    
    class const_reverse_iterator final
    {
        
    private:
        
        big_integer const *_holder;
        int _pos;
        
    public:
    
        const_reverse_iterator(
            big_integer const *holder,
            int pos = 0);
        
    public:
    
        bool operator==(
            const_reverse_iterator const &other);
        
        bool operator!=(
            const_reverse_iterator const &other);
        
        unsigned int operator*();
        
        const_reverse_iterator operator++();
        
        const_reverse_iterator operator++(
            int unused);
    
    };
    
    class half_const_iterator final
    {
        
    private:
        
        big_integer const *_holder;
        int _pos;
        bool _is_oldest;
        
    public:
    
        half_const_iterator(
            big_integer const *holder,
            int pos = 0,
            bool is_oldest = false);
        
    public:
    
        bool operator==(
            half_const_iterator const &other);
        
        bool operator!=(
            half_const_iterator const &other);
        
        unsigned int operator*();
        
        half_const_iterator operator++();
        
        half_const_iterator operator++(
            int unused);
    
    public:
    
        inline bool is_oldest() noexcept;
    
    };

private:

    int _oldest_digit;
    unsigned int *_other_digits;
    allocator *_allocator;

public:
    
    big_integer(
        int digit,
        allocator *allocator = nullptr);
    
    big_integer(
        int const *digits,
        size_t digits_count,
        allocator *allocator = nullptr);

    explicit big_integer(
        std::vector<int> const &digits,
        allocator *allocator = nullptr);

    explicit big_integer(
        std::string const &value_as_string,
        size_t base = 10,
        allocator *allocator = nullptr);
    
    explicit big_integer(
        big_integer const &other,
        allocator *allocator);

public:

    #pragma region rule of five
    
    ~big_integer() noexcept;
    
    big_integer(
        big_integer const &other);

    big_integer &operator=(
        big_integer const &other);
    
    big_integer(
        big_integer &&other) noexcept;
    
    big_integer &operator=(
        big_integer &&other) noexcept;
    
    #pragma endregion rule of five

public:

    #pragma region equivalence relations
    
    bool operator==(
        big_integer const &other) const;

    bool operator!=(
        big_integer const &other) const;
    
    #pragma endregion equivalence relations

public:

    #pragma region order relations
    
    bool operator<(
        big_integer const &other) const;

    bool operator>(
        big_integer const &other) const;

    bool operator<=(
        big_integer const &other) const;

    bool operator>=(
        big_integer const &other) const;
    
    #pragma endregion order relations

public:

    #pragma region standard operations

    big_integer operator-() const;
    
    big_integer &operator+=(
        big_integer const &other);

    big_integer operator+(
        big_integer const &other) const;

    big_integer operator+(
        std::pair<big_integer, allocator *> const &other) const;
    
    big_integer &operator-=(
        big_integer const &other);

    big_integer operator-(
        big_integer const &other) const;

    big_integer operator-(
        std::pair<big_integer, allocator *> const &other) const;
    
    big_integer &operator*=(
        big_integer const &other);

    big_integer operator*(
        big_integer const &other) const;

    big_integer operator*(
        std::pair<big_integer, allocator *> const &other) const;
    
    big_integer &operator/=(
        big_integer const &other);

    big_integer operator/(
        big_integer const &other) const;

    big_integer operator/(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator%=(
        big_integer const &other);

    big_integer operator%(
        big_integer const &other) const;

    big_integer operator%(
        std::pair<big_integer, allocator *> const &other) const;
    
    #pragma endregion standard operations

public:

    #pragma region bitwise operations
    
    big_integer operator~() const;

    big_integer &operator&=(
        big_integer const &other);

    big_integer operator&(
        big_integer const &other) const;

    big_integer operator&(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator|=(
        big_integer const &other);

    big_integer operator|(
        big_integer const &other) const;

    big_integer operator|(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator^=(
        big_integer const &other);

    big_integer operator^(
        big_integer const &other) const;

    big_integer operator^(
        std::pair<big_integer, allocator *> const &other) const;

    big_integer &operator<<=(
        size_t shift);

    big_integer operator<<(
        size_t shift) const;

    big_integer operator<<(
        std::pair<size_t, allocator *> const &shift) const;

    big_integer &operator>>=(
        size_t shift);

    big_integer operator>>(
        size_t shift) const;

    big_integer operator>>(
        std::pair<size_t, allocator *> const &shift) const;
    
    #pragma endregion bitwise operations

public:

    #pragma region custom multiplication and division
    
    static big_integer &multiply(
        big_integer &first_multiplier,
        big_integer const &second_multiplier,
        allocator *allocator = nullptr,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer multiply(
        big_integer const &first_multiplier,
        big_integer const &second_multiplier,
        allocator *allocator = nullptr,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer &divide(
        big_integer &dividend,
        big_integer const &divisor,
        allocator *allocator = nullptr,
        big_integer::division_rule division_rule = big_integer::division_rule::trivial,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer divide(
        big_integer const &dividend,
        big_integer const &divisor,
        allocator *allocator = nullptr,
        big_integer::division_rule division_rule = big_integer::division_rule::trivial,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer &modulo(
        big_integer &dividend,
        big_integer const &divisor,
        allocator *allocator = nullptr,
        big_integer::division_rule division_rule = big_integer::division_rule::trivial,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);

    static big_integer modulo(
        big_integer const &dividend,
        big_integer const &divisor,
        allocator *allocator = nullptr,
        big_integer::division_rule division_rule = big_integer::division_rule::trivial,
        big_integer::multiplication_rule multiplication_rule = big_integer::multiplication_rule::trivial);
    
    #pragma endregion custom multiplication and division

public:

    #pragma region IO
    
    friend std::ostream &operator<<(
        std::ostream &stream,
        big_integer const &value);
    
    friend std::istream &operator>>(
        std::istream &stream,
        big_integer &value);
    
    #pragma endregion IO

public:

    #pragma region common integer functions
    
    int get_digits_count() const noexcept;

    inline int sign() const noexcept;

    inline bool is_equal_to_zero() const noexcept;
    
    inline bool is_equal_to_one() const noexcept;

    inline unsigned int get_digit(
        int position) const noexcept;
    
    #pragma endregion common integer functions

public:

    #pragma region math functions
    
    static big_integer gcd(big_integer const &a, big_integer const &b);
    
    big_integer abs() const;
    
    #pragma endregion math functions

public:

    void dump_value(
        std::ostream &stream) const;
    
    std::string to_string() const;

private:

    #pragma region utility methods
    
    big_integer &clear();
    
    big_integer &copy_from(
        big_integer const &other,
        allocator *allocator);
        
    big_integer &move_from(
        big_integer &&other);
    
    big_integer &initialize_from(
        int const *digits,
        size_t digits_count);

    big_integer &initialize_from(
        std::vector<int> const &digits,
        size_t digits_count);

    big_integer &initialize_from(
        std::string const &value,
        size_t base);
    
    big_integer &change_sign();
    
    static void print_byte(
        std::ostream &stream,
        unsigned char byte_value);

    static void dump_int_value(
        std::ostream &stream,
        int value);
    
    static size_t get_significant_digits_cnt(
        int const *digits,
        size_t digits_count,
        bool forced_to_be_positive = false);
    
    static size_t get_significant_digits_cnt(
        std::vector<int> const &digits,
        bool forced_to_be_positive = false);
    
    static std::pair<std::optional<big_integer>, big_integer> divide_with_remainder(
        big_integer const &dividend,
        big_integer const &divisor,
        bool eval_quotient,
        big_integer::multiplication_rule multiplication_rule = multiplication_rule::trivial);
    
    static unsigned int char_to_int(
        char ch);
    
    #pragma endregion utility methods

private:

    #pragma region iterator requesting

    const_iterator cbegin() const;
    
    const_iterator cend() const;
    
    const_reverse_iterator crbegin() const;
    
    const_reverse_iterator crend() const;
    
    half_const_iterator half_cbegin() const;
    
    half_const_iterator half_cend() const;
    
    #pragma endregion iterator requesting

private:

    [[nodiscard]] allocator *get_allocator() const noexcept override;
    
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BIGINT_H