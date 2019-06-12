// Copyright (C) 2018, Vis Lab, CSE
// University of Nebraska-Lincoln
// Author: Jianxin Sun, Hongfeng Yu

#ifndef _INCLUDED_SOIL_HPP
#define _INCLUDED_SOIL_HPP

#include <vector>
#include <map>

#define NUM_COL         8 //number of column
#define NUM_ROW         7 //number of rows
#define NUM_DEP         5 //number of depth levels
#define NUM_PTS         280 //total number of sample points: 8(column)*7(row)*5(depth)
#define X350            22 //x350
#define EC              14 //Electrical conductivity (dS m) (lab measured)
#define PH              15 //Orakton.Accumet_pH = pH (lab measured)

class Soil
{
public:
    Soil(std::string csvfile);
    ~Soil();
    int ReadCSV();
private:
    std::string _csvfile;
    std::vector<std::string> _header;
    std::map<std::string, float *> _table;
};

#endif 
