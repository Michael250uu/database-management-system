/*
 * @Author: your name
 * @Date: 2021-04-25 21:47:35
 * @LastEditTime: 2021-05-02 23:28:48
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: /proj2/inc/Analysis.h
 */
#ifndef _ANALYSIS_H_
#define _ANALYSIS_H_
#include "Data.h"
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

//按空格分割
vector<string> Cut(string);

//查找是否存在该名字的表
bool find_table(string, vector<Table>);

//检查（）;格式
bool check_sign(string);

//求逗号个数
int count_sign(string);

//去除（）;
string remove_sign(string);

//查看字段是否重复
bool check_column(vector<string>);

//判断第一个是否为PRIMARY_KEY
bool check_P(string);

//计算PRIMARY的数量
int count_P(vector<string>);

//查看主键是否重复
bool check_re(int pos, string str, vector<Table>::iterator i);

//判断约束条件格式
bool check_require(string);

//解析约束条件前的字段
string extract_col(string);

//解析约束条件后的内容
string extract_thing(string);

//判断字段是否存在
bool check_exist(string, vector<Table>::iterator);

//字段的位置
int find_pos(string, vector<Table>::iterator);

//DISTINCT的去重
vector<Data> remove_repeat(vector<Data>);

//输出vector<Data>内容
void show(vector<Data>);

//查看所有字段是否都存在
bool check_all_c(vector<string>,vector<Table>::iterator);

//将一串字段转化为位置
vector<int> tran_col(vector<string>,vector<Table>::iterator);

//判断主键有无重复
bool check_u(vector<Table>::iterator);



#endif