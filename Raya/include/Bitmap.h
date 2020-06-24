#ifndef BITMAP_H
#define BITMAP_H

#include <vector>

class Bitmap 
{
public:

	Bitmap(int x, int y) 
	{ 
		x_resolution = x; 
		y_resolution = y; 
		pixels = std::vector<int>(x_resolution * y_resolution); 
	}
	
	bool SaveBitmap(std::string file_path);

	int GetXResolution() const						
	{
		return x_resolution;
	}

	int GetYResolution() const						
	{
		return y_resolution;
	}

	int &GetPixel(int x, int y)				
	{
		return pixels[y * x_resolution + x];
	}
	
	void SetPixel(int x, int y, int pix)		
	{
		pixels[y * x_resolution + x]=pix;
	}

private:
	int x_resolution, y_resolution;
	std::vector<int> pixels;
};

#endif // BITMAP_H
