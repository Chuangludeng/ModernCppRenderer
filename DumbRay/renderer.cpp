#include "renderer.h"

template void Renderer<Pixel_RGBAU>::draw_triangle(vector_t &t0, vector_t &t1, vector_t &t2, Pixel_RGBAU color);
template void Renderer<Pixel_RGBAU>::draw_line(vector_t &from, vector_t &to, Pixel_RGBAU color);
template void Renderer<Pixel_RGBAU>::clear(Pixel_RGBAU clear_color);
template void Renderer<Pixel_RGBAU>::present();

template<typename Pixel>
void Renderer<Pixel>::draw_triangle(vector_t &t0, vector_t &t1, vector_t &t2, Pixel color)
{

}

template<typename Pixel>
void Renderer<Pixel>::draw_line(vector_t &from, vector_t &to, Pixel color)
{

}

template<typename Pixel>
void Renderer<Pixel>::clear(Pixel clear_color)
{

}

template<typename Pixel>
void Renderer<Pixel>::present()
{

}
