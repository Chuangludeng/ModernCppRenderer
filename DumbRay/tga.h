#pragma once
//https://en.wikipedia.org/wiki/Truevision_TGA
#include <stdint.h>

//little endian
#pragma pack(push,1) //pragma pack supproted by clang and gcc as well
struct TGA_Header {
    uint8_t id_length;
    uint8_t color_map_type;
    uint8_t img_map_type;
    struct TGA_color_map_spec {
        uint16_t first_entry_index;
        uint16_t length;
        uint8_t entry_size;
    } color_map_spec;
    struct TGA_img_spec {
        uint16_t x_orig;
        uint16_t y_orig;
        uint16_t width;
        uint16_t height;
        uint8_t pixel_depth;//bits per pixel
        uint8_t img_desc;
    } img_spec;
};

struct TGA_Footer {
    //TODO: declare footer
};

struct TGA_Extension {
    //TODO: declare extension
};

#pragma pack(pop)

