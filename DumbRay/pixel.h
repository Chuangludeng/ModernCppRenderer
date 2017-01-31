#pragma once
#include "bitfield.h"
#include "util.h"

#include <stdint.h>
#include <tuple>
#include <utility>


//TODO: this class is over-complicated, you don't like too much template meta-programming which strikes compiler too hard. 
//please redesign it with union or other effecient types
//eg: possible implementation (RangeValue is for bounds checking, you can implement whatever you want but defensive.
//union { struct Data{ T value; }; Data data; 
//RangeValue<T, 0, 8> R;
//RangeValue<T, 8, 8> G;
//RangeValue<T, 16, 8> B;
//RangeValue<T, 24, 8> A;
//}
union Pixel_RGBAU
{
	unsigned int  color;
	struct { unsigned char R; unsigned char G; unsigned char B; unsigned char A; };
	Pixel_RGBAU(unsigned int  c) : color(c) {}
	Pixel_RGBAU(int r, int g, int b, int a) : A(a), R(r), G(g), B(b) {}
};
