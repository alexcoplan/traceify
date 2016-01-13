#include <string>

class Pixel {
private:
	int r;
	int g;
	int b;

public:
	std::string getColour();
	Pixel(int red, int green, int blue); 
};
