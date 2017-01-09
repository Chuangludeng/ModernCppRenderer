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

    TGA_Device(Viewport viewport) : RenderDevice<Pixel>(viewport)
    {}

    void set_pixel(int x, int y, PixelType p) override;

    PixelType get_pixel(int x, int y) override;

    void present() override; //you can simply write content to file
};