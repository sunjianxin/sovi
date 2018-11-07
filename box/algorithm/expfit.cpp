#include <iostream>
#include <fstream>
#include <vector>
#include <eigen3/Eigen/Dense>
#include <ctime>
#include <chrono>
#include <math.h>
#include <stdlib.h>

using Eigen::MatrixXf;
using Eigen::VectorXf;
using namespace Eigen;
#define VEC_SIZE 50
#define VEC_SIZE_FP (VEC_SIZE*1.0f)
int main(int argc, char * argv[])
{
	std::string dataline;
	std::ifstream infile("1meg_ringdown.txt");
	
	//std::cout << "got here\n" ;
	int trial = atoi(argv[1]);
	int i = 0;
	while(std::getline(infile,dataline)){
		i++;
		std::cout << "Iteration: " << i << std::endl;

		std::vector<float> data;
		std::stringstream thisline(dataline);

		// read and convert line of data to floating point array
		float thisdata;
		while(thisline >> thisdata){
			data.push_back(thisdata);
			//cout << thisdata << " ";
		}
		int dsize = data.size();
		std::cout << dsize << std::endl;
		
		// now convert to Eigen matrix for manipulation
		MatrixXf X(VEC_SIZE,3);
		VectorXf input(VEC_SIZE);

		
		std::cout << "loading arrays" << std::endl;
		// copy data and construct matrices
		
		float value = 0.0;
		for(int j=0;j<VEC_SIZE;j++){
			static float sum = 0.0;
			
			//value = exp(-(j/VEC_SIZE_FP)*14.0);
			value = data[j+42199];
			sum += value;
			input(j) = value;
			X(j,0) = 1.0f;
			X(j,1) = sum;
			X(j,2) = j;
			
			std::cout << value << " ";
		}
		std::cout << std::endl << "computing SVD. . ." << std::endl;
		VectorXf x(3);

		std::chrono::time_point<std::chrono::system_clock> start,end;
		volatile float decay,offset,amplitude,decay_freq;

		std::cout << "times tried: " << trial << std::endl;

		start = std::chrono::system_clock::now();

		for(int k=0;k<trial;k++){
		//for(int k = 0; k < 1; k++){
			x = X.jacobiSvd(ComputeThinU | ComputeThinV).solve(input);
			decay_freq = log(1-x(1));
			decay = 1.0f/decay_freq;
			offset = -1.0f*x(2)/x(1);
			amplitude = exp(-1.0f*decay_freq)*x(0) - offset;
		}
		std::cout << "decay: " << decay << std::endl;
		std::cout << "offset: " << offset << std::endl;
		std::cout << "amplitude " << amplitude << std::endl;
		//residual = exp(-1.0f*decay_freq*X(j,2))*amplitude+offset - invect(i);

		end = std::chrono::system_clock::now();
		
		std::chrono::duration<double> elapsed_seconds = end - start;
		
		std::cout << std::scientific;	
		std::cout << "solution: " << x << std::endl;
		std::cout << "elapsed seconds: " << elapsed_seconds.count() << "s\n" ;
		
		
		std::cout << std::endl;
		break;
		//std::string junk;
		//std::cin >> junk;
	}
//	MatrixXd m(2,2);
//	m(0,0) = 3;
//	m(1,0) = 2.5;
//	m(0,1) = -1;
//	m(1,1) = m(1,0) + m(0,1);
//	std::cout << m << std::endl;
}
