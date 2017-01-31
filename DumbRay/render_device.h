#pragma once
#include "pixel.h"

struct Viewport {
	uint32_t x;
	uint32_t y;
	uint32_t w;
	uint32_t h;
};

template<typename Pixel>
class RenderDevice {
public:

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