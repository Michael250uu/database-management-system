#pragma once
#ifndef _DATA_H_
#define _DATA_H_  
#include <string>
#include <vector>
using namespace std;
struct Data
{
    vector<string> values;
};

struct Table
{
    vector<Data> content;
    string name;
    bool primary = 0;
    int position = 0;
};



#endif