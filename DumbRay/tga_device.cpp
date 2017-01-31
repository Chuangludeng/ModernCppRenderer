#include "tga_device.h"

template TGA_Device<Pixel_RGBAU>::TGA_Device(Viewport viewport);
template TGA_Device<Pixel_RGBAU>::~TGA_Device();
template void TGA_Device<Pixel_RGBAU>::set_pixel(int x, int y, PixelType p);
template Pixel_RGBAU TGA_Device<Pixel_RGBAU>::get_pixel(int x, int y);
template void TGA_Device<Pixel_RGBAU>::flush();

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
	FreeImage_Initialise();
	FreeImage_SetOutputMessage(FreeImageErrorHandler);
	bitmap = FreeImage_Allocate(viewport_.w, viewport_.h, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);
	pitch = FreeImage_GetPitch(bitmap);
	pixels = FreeImage_GetBits(bitmap);
}

template<typename Pixel>
TGA_Device<Pixel>::~TGA_Device()
{
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
}

template<typename Pixel>
void TGA_Device<Pixel>::set_pixel(int x, int y, PixelType p)
{
	BYTE* pixel = pixels + y*pitch + x*4;

	pixel[FI_RGBA_RED] = p.R;
	pixel[FI_RGBA_GREEN] = p.G;
	pixel[FI_RGBA_BLUE] = p.B;
	pixel[FI_RGBA_ALPHA] = p.A;
}

template<typename Pixel>
Pixel TGA_Device<Pixel>::get_pixel(int x, int y)
{
	BYTE* pixel = pixels + y*pitch + x * 4;
	return Pixel(pixel[FI_RGBA_RED], pixel[FI_RGBA_GREEN], pixel[FI_RGBA_BLUE], pixel[FI_RGBA_ALPHA]);
}

template<typename Pixel>
void TGA_Device<Pixel>::flush()
{
	FreeImage_Save(FIF_TARGA, bitmap, "output.tga");
}

