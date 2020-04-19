#pragma once

#include "stb_image.h"
#include <vector>
#include <string>

typedef struct Image {
	unsigned int width, height;
	unsigned char* pixels;
} Image;

Image loadImageFile(std::string fileName);