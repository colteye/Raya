#include <iostream>
#include <fstream>

#include "Bitmap.h"

struct BitmapHeader {
	int FileSize;
	int Reserved;
	int StartAddress;
	int HeaderSize;
	short Width;
	short Height;
	short ColorPlanes;
	short BitsPerPixel;
};

bool Bitmap::SaveBitmap(std::string file_path) {

	BitmapHeader header;
	header.FileSize=sizeof(BitmapHeader) + 2 + x_resolution * y_resolution * sizeof(int);
	header.Reserved = 0;
	header.StartAddress = sizeof(BitmapHeader) + 2;

	header.HeaderSize = 12;
	header.Width = x_resolution;
	header.Height = y_resolution;
	header.ColorPlanes = 1;
	header.BitsPerPixel = 32;

	std::ofstream bitmap;
	bitmap.open(file_path, std::ios::binary);
	if (!bitmap.is_open()) return false;

	// Write magic.
	short magic = 19778;
	bitmap.write(reinterpret_cast<const char*>(&magic), sizeof(short));

	// Write header.
	bitmap.write(reinterpret_cast<const char*>(&header), sizeof(BitmapHeader));

	// Write 32 bit color data.
	bitmap.write(reinterpret_cast<const char*>(pixels.data()), sizeof(int) * x_resolution * y_resolution);

	bitmap.close();
	return true;
}