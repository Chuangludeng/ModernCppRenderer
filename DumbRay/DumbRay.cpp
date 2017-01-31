// DumbRay.cpp : Defines the entry point for the console application.
//

#include "pixel.h"
#include "renderer.h"
#include "model.h"

vector_t mesh[8] = {
	{ 1, -1,  1, 1 },
	{ -1, -1,  1, 1 },
	{ -1,  1,  1, 1 },
	{ 1,  1,  1, 1 },
	{ 1, -1, -1, 1 },
	{ -1, -1, -1, 1 },
	{ -1,  1, -1, 1 },
	{ 1,  1, -1, 1 }
};

void draw_plane(Renderer<Pixel_RGBAU> &tga_renderer, int a, int b, int c, int d, Pixel_RGBAU color) {
	vector_t p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];
	tga_renderer.draw_triangle(p1, p2, p3,color);
	tga_renderer.draw_triangle(p3, p4, p1,color);
}

void draw_box(float alpha, Renderer<Pixel_RGBAU> &tga_renderer, Pixel_RGBAU color)
{
	matrix_t m;
	m.set_rotate(-1, -0.5, 1, alpha);
	tga_renderer.world = m;
	tga_renderer.transform_update();
	draw_plane(tga_renderer, 0, 1, 2, 3, color);
	draw_plane(tga_renderer, 4, 5, 6, 7, color);
	draw_plane(tga_renderer, 0, 4, 5, 1, color);
	draw_plane(tga_renderer, 1, 5, 6, 2, color);
	draw_plane(tga_renderer, 2, 6, 7, 3, color);
	draw_plane(tga_renderer, 3, 7, 4, 0, color);
}

//TODO: this is the basic usage of renderer
/*
you can be more creative, feel free to add more features.
*/
int main()
{
    //resource
    Model unicorn;
    ModelLoader loader;
    loader.load("assets/unicorn.obj", unicorn);

    //setup renderer
    Renderer<Pixel_RGBAU> tga_renderer;
	Pixel_RGBAU clearColor(0,0,255,255);
    tga_renderer.clear(clearColor);
    Pixel_RGBAU Red(255, 0, 0, 255);

	vector_t eye(3, 0, 0, 1);
	vector_t at(0, 0, 0, 1);
	vector_t up(0, 0, 1, 1);

	tga_renderer.setCamera(eye,at,up);

	float alpha = 8.78f;

	draw_box(alpha, tga_renderer, Red);


	//draw_box(1);
  //  for (size_t i = 0; i < unicorn.indices.size();)
  //  {
  //      //face
		//vector_t v1 = unicorn.vertices[i++];
		//vector_t v2 = unicorn.vertices[i++];
		//vector_t v3 = unicorn.vertices[i++];
  //      tga_renderer.draw_triangle(v1, v2, v3, Red);//color should be more "colorful"
  //  }

    tga_renderer.present();
    return 0;
}

