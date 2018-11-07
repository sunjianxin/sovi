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
#define VEC_SIZE 2000
#define VEC_SIZE_FP (VEC_SIZE*1.0f)

float exp_func(float a, float b, float c, float input){
    return a*exp(-b*input) + c;
}


int main(int argc, char * argv[])
{
	std::string dataline;
	std::ifstream infile("y.txt");
    std::vector<float> data;

    int trial = atoi(argv[1]);

    while(std::getline(infile, dataline)){
       data.push_back(std::stof(dataline)); 
    
    }
	
    // now convert to Eigen matrix for manipulation
    MatrixXf X(VEC_SIZE,3);
    VectorXf input(VEC_SIZE);

    
    std::cout << "loading arrays" << std::endl;
    std::cout << "size: " << data.size() << std::endl;
    // copy data and construct matrices
    
    float value = 0.0;
    for(int j=0;j<VEC_SIZE;j++){
        static float sum = 0.0;
        //std::cout << "data" << j << ":" << data[j] << std::endl;        
        //value = exp(-(j/VEC_SIZE_FP)*14.0);
        value = data.at(j);
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

    float k = -x(1);
    float b = x(2)/k;
    float Y0 = x(0)-b;

    end = std::chrono::system_clock::now();
    
    std::chrono::duration<double> elapsed_seconds = end - start;
    
    std::cout << std::scientific;	
    std::cout << "solution: " << x << std::endl;
    std::cout << "elapsed seconds: " << elapsed_seconds.count() << "s\n" ;
    
    //save points  on fitted curve
    std::ofstream file;
    file.open("y_fitted.txt");
    if (!file.is_open()){
        std::cout << "open save file failed!" << std::endl;
    }else{
        for (int i = 0; i < VEC_SIZE; i++){
            file << exp_func(Y0, k, b, i) << "\n";
        }
    }
    file.close();



}

