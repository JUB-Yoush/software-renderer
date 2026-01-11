#pragma once

#include "../helper/jtypes.h"
#include "raylib.h"

struct JTexture {
    i32 width;
    i32 height;
    Color *pixels;
};

JTexture j_load_image_from_file(char *filename) {
    Image image = LoadImage(filename);
    auto texture =
            JTexture{image.width, image.height, LoadImageColors(image)};
    UnloadImage(image);
    return texture;
}
