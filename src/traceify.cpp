#include <iostream>

#include "pixel.hpp"
#include "msg.cpp"

int main()
{
	Pixel p(100,100,100);
	Msg msg("The pixel has RGB " + p.getColour());
	std::cout << "The message is (maybe): " << msg.getMessage() << std::endl; 

	return 0;
}
