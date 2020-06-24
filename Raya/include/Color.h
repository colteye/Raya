#ifndef COLOR_H
#define COLOR_H

class Color {
public:
	Color()									
	{
		red = green = blue = 1.0f;
	}

	Color(float r, float g, float b)			
	{
		red = r; 
		green = g; 
		blue = b;
	}

	void Set(float r, float g, float b)		
	{
		red = r; 
		green = g; 
		blue = b;
	}

	void Add(const Color c)					
	{
		red += c.red; 
		green += c.green; 
		blue += c.blue;
	}

	void AddScaled(const Color c, float s)	
	{
		red += s*c.red; 
		green += s*c.green; 
		blue += s*c.blue;
	}

	void Scale(float s)						
	{
		red *= s; 
		green *= s; 
		blue *= s;
	}

	void Scale(const Color c, float s)		
	{
		red = s * c.red; 
		green = s * c.green; 
		blue = s * c.blue;
	}

	void Multiply(const Color c)			
	{
		red *= c.red; 
		green *= c.green; 
		blue *= c.blue;
	}

	int ToInt() {
		int r = (red < 0) ? 0 : ((red >= 1.0) ? 255 : int(red * 256.0f));
		int g = (green < 0) ? 0 : ((green >= 1.0) ? 255 : int(green * 256.0f));
		int b = (blue < 0) ? 0 : ((blue >= 1.0) ? 255 : int(blue * 256.0f));
		return (r << 16) | (g << 8) | b;
	}
	void FromInt(int c)						
	{
		float r = float((c >> 16) & 0xff) / 255.0f;
		float g = float((c >> 8) & 0xff) / 255.0f;
		float b = float(c & 0xff) / 255.0f;

		Set(r, g, b);
	}

	static Color WHITE, GREY, BLACK, RED, YELLOW, BLUE, GREEN;

private:
	float red, green, blue;
};

#endif // COLOR_H
