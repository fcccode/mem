/*
    Copyright 2018 Brick

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge, publish, distribute,
    sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or
    substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
    BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#if !defined(MEM_BRICK_H)
#define MEM_BRICK_H

#include "defines.h"

#include <type_traits>

#include <cstring>

namespace mem
{
    class any_pointer;

    class pointer
    {
    private:
        uintptr_t value_ {0};

    public:
        MEM_CONSTEXPR pointer() noexcept = default;

        MEM_CONSTEXPR pointer(nullptr_t) noexcept;
        MEM_CONSTEXPR pointer(uintptr_t address) noexcept;

        template <typename T>
        pointer(T* address) noexcept;

        template <typename T, typename C>
        pointer(T C::* address) noexcept;

        MEM_CONSTEXPR pointer add(size_t count) const noexcept;
        MEM_CONSTEXPR pointer sub(size_t count) const noexcept;

        MEM_CONSTEXPR pointer offset(ptrdiff_t count) const noexcept;

        MEM_CONSTEXPR pointer shift(pointer from, pointer to) const noexcept;

        MEM_CONSTEXPR pointer align_up(size_t align) const noexcept;
        MEM_CONSTEXPR pointer align_down(size_t align) const noexcept;

#if defined(MEM_ARCH_X86_64)
        pointer rip(size_t offset) const noexcept;
#endif // MEM_ARCH_X86_64

        pointer& deref() const noexcept;

        MEM_CONSTEXPR pointer operator+(size_t count) const noexcept;
        MEM_CONSTEXPR pointer operator-(size_t count) const noexcept;

        MEM_CONSTEXPR ptrdiff_t operator-(pointer rhs) const noexcept;

        MEM_CONSTEXPR_14 pointer& operator+=(size_t count) noexcept;
        MEM_CONSTEXPR_14 pointer& operator-=(size_t count) noexcept;

        MEM_CONSTEXPR_14 pointer& operator++() noexcept;
        MEM_CONSTEXPR_14 pointer& operator--() noexcept;

        MEM_CONSTEXPR_14 pointer operator++(int) noexcept;
        MEM_CONSTEXPR_14 pointer operator--(int) noexcept;

        MEM_CONSTEXPR bool operator==(pointer rhs) const noexcept;
        MEM_CONSTEXPR bool operator!=(pointer rhs) const noexcept;

        MEM_CONSTEXPR bool operator<(pointer rhs) const noexcept;
        MEM_CONSTEXPR bool operator>(pointer rhs) const noexcept;

        MEM_CONSTEXPR bool operator<=(pointer rhs) const noexcept;
        MEM_CONSTEXPR bool operator>=(pointer rhs) const noexcept;

        MEM_CONSTEXPR bool operator!() const noexcept;

        MEM_CONSTEXPR explicit operator bool() const noexcept;

        template <typename T = pointer>
        typename std::add_lvalue_reference<T>::type at(size_t offset) const noexcept;

        template <typename T>
        MEM_CONSTEXPR typename std::enable_if<std::is_integral<T>::value, T>::type as() const noexcept;

        template <typename T>
        typename std::enable_if<std::is_pointer<T>::value, T>::type as() const noexcept;

        template <typename T>
        typename std::enable_if<std::is_member_pointer<T>::value, T>::type as() const noexcept;

        template <typename T>
        typename std::enable_if<std::is_lvalue_reference<T>::value, T>::type as() const noexcept;

        template <typename T>
        typename std::enable_if<std::is_array<T>::value, typename std::add_lvalue_reference<T>::type>::type as() const noexcept;

        MEM_CONSTEXPR any_pointer any() const noexcept;
    };

    class any_pointer
    {
    private:
        uintptr_t value_ {0};

    public:
        MEM_CONSTEXPR any_pointer(pointer value) noexcept;

        MEM_CONSTEXPR operator uintptr_t() const noexcept;

        template <typename T>
        operator T*() const noexcept;
    };

    class region
    {
    public:
        pointer start {nullptr};
        size_t size {0};

        MEM_CONSTEXPR region() noexcept = default;

        MEM_CONSTEXPR region(pointer start, size_t size) noexcept;

        MEM_CONSTEXPR bool contains(region rhs) const noexcept;

        MEM_CONSTEXPR bool contains(pointer address) const noexcept;
        MEM_CONSTEXPR bool contains(pointer start, size_t size) const noexcept;

        template <typename T>
        MEM_CONSTEXPR bool contains(pointer address) const noexcept;

        MEM_CONSTEXPR bool operator==(region rhs) const noexcept;
        MEM_CONSTEXPR bool operator!=(region rhs) const noexcept;

        void copy(pointer source) const noexcept;
        void fill(byte value) const noexcept;

        MEM_CONSTEXPR region sub_region(pointer address) const noexcept;
    };

    MEM_CONSTEXPR MEM_STRONG_INLINE pointer::pointer(nullptr_t) noexcept
        : value_(0)
    { }

    MEM_CONSTEXPR MEM_STRONG_INLINE pointer::pointer(uintptr_t address) noexcept
        : value_(address)
    { }

    template <typename T>
    MEM_STRONG_INLINE pointer::pointer(T* address) noexcept
        : value_(reinterpret_cast<uintptr_t>(address))
    { }

    template <typename T, typename C>
    MEM_STRONG_INLINE pointer::pointer(T C::* address) noexcept
        : value_(reinterpret_cast<const uintptr_t&>(address))
    {
        static_assert(sizeof(address) == sizeof(uintptr_t), "That's no pointer. It's a space station.");
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE pointer pointer::add(size_t count) const noexcept
    {
        return value_ + count;
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE pointer pointer::sub(size_t count) const noexcept
    {
        return value_ - count;
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE pointer pointer::offset(ptrdiff_t count) const noexcept
    {
        return static_cast<uintptr_t>(static_cast<intptr_t>(value_) + count);
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE pointer pointer::shift(pointer from, pointer to) const noexcept
    {
        return (value_ - from.value_) + to.value_;
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE pointer pointer::align_up(size_t align) const noexcept
    {
        return (value_ + align - 1) / align * align;
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE pointer pointer::align_down(size_t align) const noexcept
    {
        return value_ - (value_ % align);
    }

#if defined(MEM_ARCH_X86_64)
    MEM_STRONG_INLINE pointer pointer::rip(size_t offset) const noexcept
    {
        return static_cast<uintptr_t>(static_cast<intptr_t>(value_ + offset) + *reinterpret_cast<const int32_t*>(value_));
    }
#endif // MEM_ARCH_X86_64

    MEM_STRONG_INLINE pointer& pointer::deref() const noexcept
    {
        return *reinterpret_cast<pointer*>(value_);
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE pointer pointer::operator+(size_t count) const noexcept
    {
        return value_ + count;
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE pointer pointer::operator-(size_t count) const noexcept
    {
        return value_ - count;
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE ptrdiff_t pointer::operator-(pointer rhs) const noexcept
    {
        return static_cast<ptrdiff_t>(static_cast<intptr_t>(value_) - static_cast<intptr_t>(rhs.value_));
    }

    MEM_CONSTEXPR_14 MEM_STRONG_INLINE pointer& pointer::operator+=(size_t count) noexcept
    {
        value_ += count;

        return *this;
    }

    MEM_CONSTEXPR_14 MEM_STRONG_INLINE pointer& pointer::operator-=(size_t count) noexcept
    {
        value_ -= count;

        return *this;
    }

    MEM_CONSTEXPR_14 MEM_STRONG_INLINE pointer& pointer::operator++() noexcept
    {
        ++value_;

        return *this;
    }

    MEM_CONSTEXPR_14 MEM_STRONG_INLINE pointer& pointer::operator--() noexcept
    {
        --value_;

        return *this;
    }

    MEM_CONSTEXPR_14 MEM_STRONG_INLINE pointer pointer::operator++(int) noexcept
    {
        pointer result = *this;

        ++value_;

        return result;
    }

    MEM_CONSTEXPR_14 MEM_STRONG_INLINE pointer pointer::operator--(int) noexcept
    {
        pointer result = *this;

        --value_;

        return result;
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE bool pointer::operator==(pointer rhs) const noexcept
    {
        return value_ == rhs.value_;
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE bool pointer::operator!=(pointer rhs) const noexcept
    {
        return value_ != rhs.value_;
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE bool pointer::operator<(pointer rhs) const noexcept
    {
        return value_ < rhs.value_;
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE bool pointer::operator>(pointer rhs) const noexcept
    {
        return value_ > rhs.value_;
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE bool pointer::operator<=(pointer rhs) const noexcept
    {
        return value_ <= rhs.value_;
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE bool pointer::operator>=(pointer rhs) const noexcept
    {
        return value_ >= rhs.value_;
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE bool pointer::operator!() const noexcept
    {
        return !value_;
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE pointer::operator bool() const noexcept
    {
        return value_;
    }

    template <typename T>
    MEM_CONSTEXPR MEM_STRONG_INLINE typename std::enable_if<std::is_integral<T>::value, T>::type pointer::as() const noexcept
    {
        static_assert(std::is_same<typename std::make_unsigned<T>::type, uintptr_t>::value, "Invalid Integer Type");

        return static_cast<T>(value_);
    }

    template <typename T>
    MEM_STRONG_INLINE typename std::add_lvalue_reference<T>::type pointer::at(size_t offset) const noexcept
    {
        return *reinterpret_cast<typename std::add_pointer<T>::type>(value_ + offset);
    }

    template <typename T>
    MEM_STRONG_INLINE typename std::enable_if<std::is_pointer<T>::value, T>::type pointer::as() const noexcept
    {
        return reinterpret_cast<T>(value_);
    }

    template <typename T>
    MEM_STRONG_INLINE typename std::enable_if<std::is_member_pointer<T>::value, T>::type pointer::as() const noexcept
    {
        static_assert(sizeof(T) == sizeof(value_), "That's no pointer. It's a space station.");

        return reinterpret_cast<const T&>(value_);
    }

    template <typename T>
    MEM_STRONG_INLINE typename std::enable_if<std::is_lvalue_reference<T>::value, T>::type pointer::as() const noexcept
    {
        return *reinterpret_cast<typename std::add_pointer<T>::type>(value_);
    }

    template <typename T>
    MEM_STRONG_INLINE typename std::enable_if<std::is_array<T>::value, typename std::add_lvalue_reference<T>::type>::type pointer::as() const noexcept
    {
        return *reinterpret_cast<typename std::add_pointer<T>::type>(value_);
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE any_pointer pointer::any() const noexcept
    {
        return any_pointer(*this);
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE any_pointer::any_pointer(pointer value) noexcept
        : value_(value.as<uintptr_t>())
    { }

    MEM_CONSTEXPR MEM_STRONG_INLINE any_pointer::operator uintptr_t() const noexcept
    {
        return value_;
    }

    template <typename T>
    MEM_STRONG_INLINE any_pointer::operator T*() const noexcept
    {
        return reinterpret_cast<T*>(value_);
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE region::region(pointer start_, size_t size_) noexcept
        : start(start_)
        , size(size_)
    { }

    MEM_CONSTEXPR MEM_STRONG_INLINE bool region::contains(region rhs) const noexcept
    {
        return (rhs.start >= start) && ((rhs.start + rhs.size) <= (start + size));
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE bool region::contains(pointer address) const noexcept
    {
        return (address >= start) && (address < (start + size));
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE bool region::contains(pointer start_, size_t size_) const noexcept
    {
        return (start_ >= start) && ((start_ + size_) <= (start + size));
    }

    template <typename T>
    MEM_CONSTEXPR MEM_STRONG_INLINE bool region::contains(pointer address) const noexcept
    {
        return (address >= start) && ((address + sizeof(T)) <= (start + size));
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE bool region::operator==(region rhs) const noexcept
    {
        return (start == rhs.start) && (size == rhs.size);
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE bool region::operator!=(region rhs) const noexcept
    {
        return (start != rhs.start) || (size != rhs.size);
    }

    MEM_STRONG_INLINE void region::copy(pointer source) const noexcept
    {
        std::memcpy(start.as<void*>(), source.as<const void*>(), size);
    }

    MEM_STRONG_INLINE void region::fill(byte value) const noexcept
    {
        std::memset(start.as<void*>(), value, size);
    }

    MEM_CONSTEXPR MEM_STRONG_INLINE region region::sub_region(pointer address) const noexcept
    {
        return region(address, size - static_cast<size_t>(address - start));
    }
}
#endif // !MEM_BRICK_H
