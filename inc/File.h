
#pragma once
#ifndef _FILE_H_
#define _FILE_H_
#include <iostream>
#include "Data.h"
#include "Analysis.h"
using namespace std;

//初始创建动态数据库
vector<Table> Create();

//覆盖database.txt
void Cover_main(vector<Table>);

//覆盖具体表格对应文件
void Cover_table(string, vector<Table>);

#endif