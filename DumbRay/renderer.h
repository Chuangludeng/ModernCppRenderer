#pragma once
#include "render_device.h"
#include "math.h"

//TODO: implement Renderer
/*
Triangle drawing is the fundamental of computer graphics, it's the most concise, precisely defined, 
least ambiguous primitive possible for describing a surface in 3D space.

You should make good use of search engines, it's not that hard.
*/

template<typename Pixel> //old school renderer, coupled with Pixel format.
class Renderer {
public:
    void draw_triangle(vector_t &t0, vector_t &t1, vector_t &t2, Pixel color);
    
    void draw_line(vector_t &from, vector_t &to, Pixel color);

    void clear(Pixel clear_color);

    void present();
private:
    RenderDevice<Pixel> *device_; // TODO: you know how to make it exception safe.
};
