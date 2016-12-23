#pragma once
#include "bitfield.h"
#include "util.h"

#include <stdint.h>
#include <tuple>
#include <utility>

//pixel format types, not depth, no compression for now
enum class PixelFormat {
    R1,
    A8,
    R8,
    R8I,
    R8U,
    R8S,
    R16,
    R16I,
    R16U,
    R16F,
    R16S,
    R32I,
    R32U,
    R32F,
    RG8,
    RG8I,
    RG8U,
    RG8S,
    RG16,
    RG16I,
    RG16U,
    RG16F,
    RG16S,
    RG32I,
    RG32U,
    RG32F,
    RGB8,
    RGB8I,
    RGB8U,
    RGB8S,
    RGB9E5F,
    BGRA8,
    RGBA8,
    RGBA8I,
    RGBA8U,
    RGBA8S,
    RGBA16,
    RGBA16I,
    RGBA16U,
    RGBA16F,
    RGBA16S,
    RGBA32I,
    RGBA32U,
    RGBA32F,
    R5G6B5,
    RGBA4,
    RGB5A1,
    RGB10A2,
    R11G11B10F,

    COUNT
};


//TODO: this class is over-complicated, you don't like too much template meta-programming which strikes compiler too hard. 
//please redesign it with union or other effecient types
//eg: possible implementation (RangeValue is for bounds checking, you can implement whatever you want but defensive.
//union { struct Data{ T value; }; Data data; 
//RangeValue<T, 0, 8> R;
//RangeValue<T, 8, 8> G;
//RangeValue<T, 16, 8> B;
//RangeValue<T, 24, 8> A;
//}
template<PixelFormat format, size_t... ComponentSizes>
class Pixel {

    template<size_t... I>
    static auto make_components(std::index_sequence<I...>)
    {
        auto constexpr sizes = std::make_tuple(ComponentSizes...);
        //size = offset + sizeI
        return std::make_tuple(Bitfield<storage_type, partial_sum<I>::value(sizes), std::get<I>(sizes)>{}...);
    }

    static auto make_components()
    {
        return make_components(std::make_index_sequence<sizeof...(ComponentSizes)>());
    }

public:
    static constexpr size_t total_size = sum_of<ComponentSizes...>::value;
    static constexpr size_t num_components = sizeof...(ComponentSizes);
    
    static const PixelFormat format_type = format;

    using storage_type = //detect pixel storage size by size of total components
        std::conditional_t<total_size == 0, void,
        std::conditional_t<total_size <= 8, uint8_t,
        std::conditional_t<total_size <= 16, uint16_t,
        std::conditional_t<total_size <= 32, uint32_t,
        std::conditional_t<total_size <= 64, uint64_t,
        void>>>>>;

    using component_storage_type = decltype(make_components());
    
    Pixel()
        : components_{}
    {}

    //set whole pixel value, eg: packed rgba
    Pixel(storage_type pixel)
    {
        set_value(pixel);
    }

    //set pixel component separately, eg Pixel pixel{r, g, b, a}
    template<size_t N>
    Pixel(const storage_type(&comps)[N]) : components_{}
    {
        static_assert(N == num_components, "invalid size of components");

        set_recursive<N - 1>(comps);
    }

    //get packed pixel value
    storage_type get_value() const
    {
        return get_value_unpacked(std::make_index_sequence<num_components>());
    }

    //set packed pixel value
    void set_value(storage_type pixel)
    {
        set_value_recursive<num_components - 1>(pixel);
    }

    //get pixel component value
    template<size_t index>
    storage_type get() const
    {
        return std::get<index>(components_);
    }

    //set pixel component value
    template<size_t index>
    void set(storage_type val)
    {
        std::get<index>(components_) = val;
    }

private:
    template<size_t... I>
    storage_type get_value_unpacked(std::index_sequence<I...>) const
    {
        storage_type ret = 0;

        unpack((ret |= std::get<I>(components_).get_value())...);

        return ret;
    }

    template<size_t N>
    void set_recursive(const storage_type *comps)
    {
        static_assert(N >= 0, "invalid tuple index");
        set<N>(comps[N]);

        set_recursive<N - 1>(comps);
    }

    template<>
    void set_recursive<0>(const storage_type *comps)
    {
        set<0>(comps[0]);
    }

    template<size_t N>
    void set_value_recursive(storage_type comp)
    {
        static_assert(N >= 0, "invalid tuple index");
        std::get<N>(components_).set_value(comp);

        set_value_recursive<N - 1>(comp);
    }

    template<>
    void set_value_recursive<0>(storage_type comp)
    {
        std::get<0>(components_).set_value(comp);
    }
private:
    component_storage_type components_;
};


//pre-defined pixel formats, you can add more
//only unsigned types supported for now.
using Pixel_RGBAU = Pixel<PixelFormat::RGBA32U, 8, 8, 8, 8>;
using Pixel_BGRAU = Pixel<PixelFormat::BGRA8, 8, 8, 8, 8>;
using Pixel_RGB = Pixel<PixelFormat::RGB8, 8, 8, 8>;
using Pixel_Gray = Pixel<PixelFormat::R8, 8>;
