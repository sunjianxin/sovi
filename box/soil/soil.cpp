// Copyright (C) 2018, Vis Lab, CSE
// University of Nebraska-Lincoln
// Author: Jianxin Sun, Hongfeng Yu

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <silo.h>
#include "soil.hpp"

Soil::Soil(std::string csvfile)
    :
    _csvfile(csvfile)
{
    //std::cout << "Soil Constructing" << std::endl;
}

Soil::~Soil()
{
    //std::cout << "Soil destructing" << std::endl;
}

/* ReadCSV(): 
 * read int a .csv file 
 * create a .silo that can be used by VisIt
 * return 0 when success, -1 when failed
 */
int Soil::ReadCSV()
{
    int field_idx = -1;
    int prv_field_idx = -1;
    int idx = 0;
    int dep_idx = 0;
    /* open .csv file */
    std::ifstream infile(_csvfile.c_str(), std::ifstream::in);
    if (!infile.is_open())
        return -1;
    std::string line;
    /* read the header first */
    std::getline(infile, line);
    /* build the header list */
    size_t pos = 0;
    std::string token;
    while ((pos = line.find(",")) != std::string::npos) {
        token = line.substr(0, pos);
        _header.push_back(token);
        line.erase(0, pos + 1);
    }
    line.pop_back();
    _header.push_back(line);
    /* adjust header names by replacing "." with "_" for silo function */
    for (int i = 0; i < _header.size(); i++)
        std::replace(_header.at(i).begin(), _header.at(i).end(), '.', '_');
    /* prepare the map: header->list of float*/
    for (int i = 0; i < _header.size(); i++) {
        float *data = (float *)calloc(NUM_PTS, sizeof(float));
        _table[_header.at(i)] = data;
    }
    int counter = 0;
    int num_table_col = -1;
    while (std::getline(infile, line))
    {
        counter++;
        //std::cout << "Line: " << counter << std::endl;
        /* extract a row of data */
        std::vector<std::string> row;
        //std::cout << line << std::endl;
        while ((pos = line.find(",")) != std::string::npos) {
            token = line.substr(0, pos);
            row.push_back(token);
            line.erase(0, pos + 1);
        }
        row.push_back(token);
        field_idx = stoi(row.at(2));
        num_table_col = row.size();
        //std::cout << num_table_col << std::endl;

        if (field_idx <= 56 && field_idx != -1) {
            int col_idx, row_idx;
            /* write x, y, z array */
            col_idx = (field_idx - 1)%8;
            row_idx = (field_idx - 1)/8;
            if (field_idx == prv_field_idx) {
                dep_idx++;
            } else {
                dep_idx = 0;
                prv_field_idx = field_idx;
            }
            /* fill x, y, z content */
            idx = row_idx*8 + col_idx + dep_idx*7*8;
            for (int i = 4; i < row.size(); i++) {
                //std::cout << i << std::endl;
                if (row.at(i) != "NA" && row.at(i) != "VNIRP")
                    _table[_header.at(i)][idx] = stof(row.at(i));
            }
        }
    }
    //fix zero value data
    for (int i = 4; i < num_table_col; i++) {
        for (int j = 0; j < 280; j++) {
            if (_table[_header.at(i)][j] == 0) {
                if (j >= 8) { 
                    _table[_header.at(i)][j] = _table[_header.at(i)][j - 8];
                } else {
                    _table[_header.at(i)][j] = _table[_header.at(i)][j + 8];
                }
            }
        }
    }

    for (int j = 0; j < 280; j++) {
        std::cout << _table["MCg___"][j] << std::endl;
    }
    /* creat a .silo file */
    DBfile *dbfile = NULL;
    dbfile = DBCreate("soil.silo",
              DB_CLOBBER,
              DB_LOCAL,
              "soil data",
              DB_HDF5);
    if (dbfile == NULL) {
        fprintf(stderr, "Failed!\n");
        return -1;
    }
    ///* fill the silo with data */
    int ndims = 3;
    float x[NUM_COL], y[NUM_ROW], z[NUM_DEP];
    float *coords[] = {x, y, z};
    int dims[] = {NUM_COL, NUM_ROW, NUM_DEP};
    /* initialize x, y z */
    for (int i = 0; i < NUM_ROW; i++) {
            y[i] = (float)i;
    }
    for (int i = 0; i < NUM_COL; i++) {
            x[i] = (float)i;
    }
    for (int i = 0; i < NUM_DEP; i++) {
            z[i] = (float)i;
    }

    DBPutQuadmesh(dbfile, "soil_volume", NULL, coords, dims, ndims, DB_FLOAT, DB_COLLINEAR, NULL);
    for (int i = 4; i < _header.size(); i++) {
        DBPutQuadvar1(dbfile, _header.at(i).c_str(), "soil_volume", _table[_header.at(i)], dims, ndims, NULL, 0, DB_FLOAT, DB_NODECENT, NULL);
        //std::cout << _header.at(i) << std::endl;
    }
    DBClose(dbfile);
    //std::cout << _table[_header.at(5)][48] << std::endl;
    //std::cout << _table[_header.at(5)][55] << std::endl;
    /* free allocated memory */
    for (int i = 0; i < _header.size(); i++) {
        free(_table[_header.at(i)]);
    }
    return 0;
}
