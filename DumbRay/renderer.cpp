#include "renderer.h"

#include "tga_device.h"

template Renderer<Pixel_RGBAU>::Renderer();
template Renderer<Pixel_RGBAU>::~Renderer();
template void Renderer<Pixel_RGBAU>::draw_triangle(vector_t &t0, vector_t &t1, vector_t &t2, Pixel_RGBAU color);
template void Renderer<Pixel_RGBAU>::draw_line(vector_t &from, vector_t &to, Pixel_RGBAU color);
template void Renderer<Pixel_RGBAU>::clear(Pixel_RGBAU clear_color);
template void Renderer<Pixel_RGBAU>::present();

template<typename Pixel>
Renderer<Pixel>::Renderer()
{
	RenderDevice<Pixel>::Viewport viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.w = 640;
	viewport.h = 480;

	device = new TGA_Device<Pixel>(viewport);
}

template<typename Pixel>
Renderer<Pixel>::~Renderer()
{
	delete device;
}

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
	device->flush();
}
