#include "imageLoader.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Original source: https://raw.githubusercontent.com/lvandeve/lodepng/master/examples/example_decode.cpp
Image loadImageFile(std::string fileName)
{
	std::vector<unsigned char> png;
    unsigned char* pixels; //the raw pixels
	int width, height;
	int comp;

	//load and decode
	unsigned char* load = stbi_load(fileName.c_str(), &width, &height, &comp, 0);

	//if there's an error, display it
	if(!load) printf("Error loading image");

	Image image;
	image.width = width;
	image.height = height;
	image.pixels = load;

	return image;

}