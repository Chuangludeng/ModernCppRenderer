#pragma once
#include "render_device.h"

//TODO: implement TGA_Device
//
// example usage:
// TGA_Device<Pixel_RGBA> device;
// device.set_pixel(100, 100, RGBA{123, 0, 0, 255});
// auto pixel = device.get_pixel(100, 100);

template<typename Pixel>//rgba, rgb, grayscale
class TGA_Device : public RenderDevice<Pixel>
{
public:

    TGA_Device((Viewport viewport) : RenderDevice<Pixel>(Viewport)
    {}

    virtual void set_pixel(int x, int y, PixelType p) = 0;

    virtual PixelType get_pixel(int x, int y) = 0;

    virtual void present() = 0; //you can simply write content to file
};