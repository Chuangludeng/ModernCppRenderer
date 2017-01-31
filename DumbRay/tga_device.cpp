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
	bytespp = FreeImage_GetLine(bitmap) / FreeImage_GetWidth(bitmap);
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
	BYTE* bits = FreeImage_GetScanLine(bitmap, y);
	bits += x * bytespp;
	bits[FI_RGBA_RED] = p.R;
	bits[FI_RGBA_GREEN] = p.G;
	bits[FI_RGBA_BLUE] = p.B;
	bits[FI_RGBA_ALPHA] = p.A;
}

template<typename Pixel>
Pixel TGA_Device<Pixel>::get_pixel(int x, int y)
{
	BYTE* bits = FreeImage_GetScanLine(bitmap, y);
	bits += x * bytespp;
	return Pixel(bits[FI_RGBA_RED], bits[FI_RGBA_GREEN], bits[FI_RGBA_BLUE], bits[FI_RGBA_ALPHA]);
}

template<typename Pixel>
void TGA_Device<Pixel>::flush()
{
	FreeImage_Save(FIF_TARGA, bitmap, "output.tga");
}

