#include <iostream>
#include "soil.hpp"

Soil::Soil()
{
	std::cout << "Soil Constructing" << std::endl;
	a = 1;
}

Soil::~Soil()
{
	std::cout << "Soil destructing" << std::endl;
}
