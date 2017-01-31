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

	Renderer();
	~Renderer();

    void draw_triangle(vector_t &t0, vector_t &t1, vector_t &t2, Pixel color);
    
    void draw_line(vector_t &from, vector_t &to, Pixel color);

    void clear(Pixel clear_color);

    void present();

	void transform_update();

	vector_t transform_apply(const vector_t& x);

	int transform_check_cvv(const vector_t& v);

	vector_t transform_homogenize(const vector_t& x);

	void setCamera(vector_t& eye, vector_t& at, vector_t& up );

	matrix_t world;         // 世界坐标变换
private:
    RenderDevice<Pixel> *device; // TODO: you know how to make it exception safe.
	Viewport viewport;

	matrix_t view;          // 摄影机坐标变换
	matrix_t projection;    // 投影变换
	matrix_t transformMatrix;     // transform = world * view * projection
};