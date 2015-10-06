#pragma once

// Defines a "strong" type alias for `mUnderlying`, called `mName`.
#define ECS_STRONG_TYPEDEF(mUnderlying, mName)                                  \
    class mName final                                                           \
    {                                                                           \
    private:                                                                    \
        mUnderlying value;                                                      \
                                                                                \
    public:                                                                     \
        inline mName() = default;                                               \
        inline mName(const mName& mX) = default;                                \
        inline mName(mName&& mX) = default;                                     \
        inline mName& operator=(const mName& rhs) = default;                    \
        inline mName& operator=(mName&& rhs) = default;                         \
        inline constexpr explicit mName(mUnderlying mX) noexcept : value{mX} {} \
        inline constexpr mName& operator=(mUnderlying rhs) noexcept             \
        {                                                                       \
            value = rhs;                                                        \
            return *this;                                                       \
        }                                                                       \
        inline constexpr operator const mUnderlying&() const noexcept           \
        {                                                                       \
            return value;                                                       \
        }                                                                       \
        inline constexpr operator mUnderlying&() noexcept { return value; }     \
        inline constexpr decltype(auto) operator==(const mName& rhs) noexcept   \
        {                                                                       \
            return value == rhs.value;                                          \
        }                                                                       \
        inline constexpr decltype(auto) operator!=(const mName& rhs) noexcept   \
        {                                                                       \
            return value != rhs.value;                                          \
        }                                                                       \
        inline constexpr decltype(auto) operator<(const mName& rhs) noexcept    \
        {                                                                       \
            return value < rhs.value;                                           \
        }                                                                       \
        inline constexpr decltype(auto) operator>(const mName& rhs) noexcept    \
        {                                                                       \
            return value > rhs.value;                                           \
        }                                                                       \
        inline constexpr decltype(auto) operator<=(const mName& rhs) noexcept   \
        {                                                                       \
            return value <= rhs.value;                                          \
        }                                                                       \
        inline constexpr decltype(auto) operator>=(const mName& rhs) noexcept   \
        {                                                                       \
            return value >= rhs.value;                                          \
        }                                                                       \
    };                                                                          \
                                                                                \
    static_assert(std::is_literal_type<mName>{}, "")
