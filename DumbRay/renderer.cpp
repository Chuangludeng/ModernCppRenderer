#include "renderer.h"

#include "tga_device.h"

template Renderer<Pixel_RGBAU>::Renderer();
template Renderer<Pixel_RGBAU>::~Renderer();
template void Renderer<Pixel_RGBAU>::transform_update();
template vector_t Renderer<Pixel_RGBAU>::transform_apply(const vector_t& x);
template int Renderer<Pixel_RGBAU>::transform_check_cvv(const vector_t& v);
template vector_t Renderer<Pixel_RGBAU>::transform_homogenize(const vector_t& x);
template void Renderer<Pixel_RGBAU>::setCamera(vector_t& eye, vector_t& at, vector_t& up);
template void Renderer<Pixel_RGBAU>::draw_triangle(vector_t &t0, vector_t &t1, vector_t &t2, Pixel_RGBAU color);
template void Renderer<Pixel_RGBAU>::draw_line(vector_t &from, vector_t &to, Pixel_RGBAU color);
template void Renderer<Pixel_RGBAU>::clear(Pixel_RGBAU clear_color);
template void Renderer<Pixel_RGBAU>::present();

template<typename Pixel>
Renderer<Pixel>::Renderer()
{
	viewport.x = 0;
	viewport.y = 0;
	viewport.w = 640;
	viewport.h = 480;

	device = new TGA_Device<Pixel>(viewport);

	// 初始化，设置屏幕长宽
	float aspect = (float)viewport.w / ((float)viewport.h);
	world.set_identity();
	view.set_identity();
	projection.matrix_set_perspective(3.1415926f * 0.5f, aspect, 1.0f, 500.0f);
	transform_update();
}

template<typename Pixel>
Renderer<Pixel>::~Renderer()
{
	delete device;
}

// 矩阵更新，计算 transform = world * view * projection
template<typename Pixel>
void Renderer<Pixel>::transform_update() {
	matrix_t m;
	m = world * view;
	transformMatrix = m * projection;
}

// 将矢量 x 进行 project 
template<typename Pixel>
vector_t Renderer<Pixel>::transform_apply(const vector_t& x) {
	return transformMatrix.apply(x);
}

// 检查齐次坐标同 cvv 的边界用于视锥裁剪
template<typename Pixel>
int Renderer<Pixel>::transform_check_cvv(const vector_t& v) {
	float w = v.w;
	int check = 0;
	if (v.z < 0.0f) check |= 1;
	if (v.z > w) check |= 2;
	if (v.x < -w) check |= 4;
	if (v.x > w) check |= 8;
	if (v.y < -w) check |= 16;
	if (v.y > w) check |= 32;
	return check;
}

// 归一化，得到屏幕坐标
template<typename Pixel>
vector_t Renderer<Pixel>::transform_homogenize(const vector_t& x) {
	vector_t y;
	float rhw = 1.0f / x.w;
	y.x = (x.x * rhw + 1.0f) * viewport.w * 0.5f;
	y.y = (1.0f - x.y * rhw) * viewport.h * 0.5f;
	y.z = x.z * rhw;
	y.w = 1.0f;
	return y;
}

template<typename Pixel>
void Renderer<Pixel>::draw_triangle(vector_t &t0, vector_t &t1, vector_t &t2, Pixel color)
{
	point_t p1, p2, p3, c1, c2, c3;

	// 按照 Transform 变化
	c1 = transform_apply(t0);
	c2 = transform_apply(t1);
	c3 = transform_apply(t2);

	// 裁剪，注意此处可以完善为具体判断几个点在 cvv内以及同cvv相交平面的坐标比例
	// 进行进一步精细裁剪，将一个分解为几个完全处在 cvv内的三角形
	if (transform_check_cvv(c1) != 0) return;
	if (transform_check_cvv(c2) != 0) return;
	if (transform_check_cvv(c3) != 0) return;

	// 归一化
	p1 = transform_homogenize(c1);
	p2 = transform_homogenize(c2);
	p3 = transform_homogenize(c3);

// 线框绘制
	draw_line(p1,p2, color);
	draw_line(p1,p3, color);
	draw_line(p3,p2, color);
}

template<typename Pixel>
void Renderer<Pixel>::setCamera(vector_t& eye, vector_t& at, vector_t& up)
{
	view.matrix_set_lookat(eye, at, up);
	transform_update();
}

template<typename Pixel>
void Renderer<Pixel>::draw_line(vector_t &from, vector_t &to, Pixel color)
{
	int x, y, rem = 0;
	if (from == to) {
		device->set_pixel(from.x, from.y,color);
	}
	else if (from.x == to.x) {
		int inc = (from.y <= to.y) ? 1 : -1;
		for (y = from.y; y != to.y; y += inc) device->set_pixel(from.x, y,color);
		device->set_pixel(to.x, to.y,color);
	}
	else if (from.y == to.y) {
		int inc = (from.x <= to.x) ? 1 : -1;
		for (x = from.x; x != to.x; x += inc) device->set_pixel(x, from.y,color);
		device->set_pixel(to.x, to.y,color);
	}
	else {
		int dx = (from.x < to.x) ? to.x - from.x : from.x - to.x;
		int dy = (from.y < to.y) ? to.y - from.y : from.y - to.y;
		if (dx >= dy) {
			if (to.x < from.x) x = from.x, y = from.y, from.x = to.x, from.y = to.y, to.x = x, to.y = y;
			for (x = from.x, y = from.y; x <= to.x; x++) {
				device->set_pixel(x, y,color);
				rem += dy;
				if (rem >= dx) {
					rem -= dx;
					y += (to.y >= from.y) ? 1 : -1;
					device->set_pixel(x, y,color);
				}
			}
			device->set_pixel(to.x, to.y,color);
		}
		else {
			if (to.y < from.y) x = from.x, y = from.y, from.x = to.x, from.y = to.y, to.x = x, to.y = y;
			for (x = from.x, y = from.y; y <= to.y; y++) {
				device->set_pixel(x, y,color);
				rem += dx;
				if (rem >= dy) {
					rem -= dy;
					x += (to.x >= from.x) ? 1 : -1;
					device->set_pixel(x, y,color);
				}
			}
			device->set_pixel(to.x, to.y,color);
		}
	}
}

template<typename Pixel>
void Renderer<Pixel>::clear(Pixel clear_color)
{
	for (uint32_t x = 0 ; x <viewport.w ; x++)
	{
		for (uint32_t y = 0; y < viewport.h; y++)
		{
			device->set_pixel(x, y, clear_color);
		}
	}
}

template<typename Pixel>
void Renderer<Pixel>::present()
{
	device->flush();
}
