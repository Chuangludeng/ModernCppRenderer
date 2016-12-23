#pragma once
#include <limits>
#include <cassert>

template<typename T, size_t Offset, size_t Bits>
struct Bitfield {
    T value;

    static_assert(Offset + Bits <= sizeof(T) * CHAR_BIT, "bit field out of range");

    static const T Max = (T(1) << Bits) - 1;
    static const T Mask = Max << Offset;

    T maximum() const { return Max; }

    Bitfield& operator=(T v)
    {
        assert(v <= Max);
        value = (value &~Mask) | (v << Offset);//clear and set
        return *this;
    }

    operator T() const
    {
        return (value >> Offset) & Max;
    }

    T get_value() const
    {
        return value;
    }

    void set_value(T v)
    {
        value = (value &~Mask) | v;
    }

    Bitfield& operator+=(T v)
    {
        assert(T(*this) + v <= Max);
        value += v << Offset;

        return *this;
    }

    Bitfield& operator-=(T v)
    {
        assert(T(*this) >= v);
        value -= v;

        return *this;
    }

    Bitfield& operator++() { return *this += 1; }
    Bitfield operator++(int)        // postfix form
    {
        Bitfield tmp(*this);
        operator++();
        return tmp;
    }
    Bitfield& operator--() { return *this -= 1; }
    Bitfield operator--(int)       // postfix form
    {
        Bitfield tmp(*this);
        operator--();
        return tmp;
    }
};