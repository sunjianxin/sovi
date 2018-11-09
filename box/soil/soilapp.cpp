// Copyright (C) 2018, Vis Lab, CSE
// University of Nebraska-Lincoln
// Author: Jianxin Sun, Hongfeng Yu

#include <iostream>
#include "soil.hpp"

#define CSVFILE "/Users/jianxinsun/research/soilsampling/csp3.spec.dat.csv"

int main(int argc, char *argv[]) {
	Soil soil(CSVFILE);
	std::cout << "Converting to a new .slio file..." << std::endl;
	soil.ReadCSV();
	std::cout << "Done" << std::endl;
	return 0;
}
