#pragma once
#include "pixel.h"
template<typename Pixel>
class RenderDevice {
public:
    struct Viewport {
        float x;
        float y;
        float w;
        float h;
    };

    using PixelType = Pixel;

    RenderDevice(Viewport viewport) : viewport_(viewport)
    {}

    virtual ~RenderDevice() = default;

    virtual void set_pixel(int x, int y, PixelType pixel) = 0;

    virtual PixelType get_pixel(int x, int y) = 0;
    
    virtual void flush() = 0;

protected:
    Viewport viewport_;

};