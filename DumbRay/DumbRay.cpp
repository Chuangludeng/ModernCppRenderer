// DumbRay.cpp : Defines the entry point for the console application.
//

#include "pixel.h"
#include "renderer.h"
#include "model.h"

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
    tga_renderer.clear(Pixel_RGBAU({128, 110, 55, 255}));
    Pixel_RGBAU Red({ 255, 0, 0, 255 });

    for (size_t i = 0; i < unicorn.indices.size();)
    {
        //face
        vec3i v1 = unicorn.vertices[i++];
        vec3i v2 = unicorn.vertices[i++];
        vec3i v3 = unicorn.vertices[i++];
        tga_renderer.draw_triangle(v1, v2, v3, Red);//color should be more "colorful"
    }

    tga_renderer.present();
    return 0;
}

