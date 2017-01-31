#include "tga_device.h"

#include "FreeImage.h"

void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
	printf("\n*** ");
	if (fif != FIF_UNKNOWN) {
		printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
	}
	printf(message);
	printf(" ***\n");
}

template<typename Pixel>
TGA_Device<Pixel>::TGA_Device(Viewport viewport) : RenderDevice<Pixel>(viewport)
{
	framebuffer = new Pixel[viewport.w][viewport.h];

	FreeImage_Initialise();
	FreeImage_SetOutputMessage(FreeImageErrorHandler);
}

template<typename Pixel>
TGA_Device<Pixel>::~TGA_Device()
{
	delete framebuffer[][];
	FreeImage_DeInitialise();
}

template<typename Pixel>
void TGA_Device<Pixel>::set_pixel(int x, int y, PixelType p)
{
	framebuffer[x][y] = p;
}

template<typename Pixel>
Pixel TGA_Device<Pixel>::get_pixel(int x, int y)
{
	return framebuffer[x][y];
}

template<typename Pixel>
void TGA_Device<Pixel>::present()
{
	FIBITMAP* bitmap = FreeImage_Allocate(viewport_.w, viewport_.h, 32,FI_RGBA_RED_MASK,FI_RGBA_GREEN_MASK,FI_RGBA_BLUE_MASK);
	// 计算每像素的字节数(24 位像素为3 字节而32 位像素为4 字节)
	int bytespp = FreeImage_GetLine(bitmap) / FreeImage_GetWidth(bitmap);
	for (unsigned y = 0; y < FreeImage_GetHeight(bitmap); y++)
	{
		BYTE* bits = FreeImage_GetScanLine(bitmap, y);
		for (unsigned x = 0; x < FreeImage_GetWidth(bitmap); x++) {
			// 设置像素点颜色为绿色，透明度为128
			bits[FI_RGBA_RED] = framebuffer[x][y]->R;
			bits[FI_RGBA_GREEN] = framebuffer[x][y]->G;
			bits[FI_RGBA_BLUE] = framebuffer[x][y]->B;
			bits[FI_RGBA_ALPHA] = framebuffer[x][y]->A;
			// 跳转到下一个像素
			bits += bytespp;
		}
	}
	FreeImage_Save(FIF_TARGA, bitmap, "output.tga");
	FreeImage_Unload(bitmap);
}

