/*
 * @Author: your name
 * @Date: 2021-04-29 16:42:44
 * @LastEditTime: 2021-04-30 20:53:50
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: /proj2/inc/Process.h
 */
#ifndef _PROCESS_H_
#define _PROCESS_H_
#include "Data.h"
#include "File.h"
#include "Analysis.h"

//通过第一个字符串选择操作
int Fchoose(vector<string>);

//增添表
vector<Table> Addtable(vector<Table>, string name, string sen);

//展示所有表
void show(vector<Table>);

//删除表
vector<Table> drop(string, vector<Table>);

//向表中插入一行新数据
vector<Table> insert(string sen,string name, vector<Table>);

//SELECT操作
void select(vector<string>parts,vector<Table>database);

//UPDATE操作
vector<Table>update(vector<string>,vector<Table>);

//DELETE操作
vector<Table>Delete(vector<string>,vector<Table>);

//ALTER操作
vector<Table>alter(vector<string>,vector<Table>);

//将一个表插入到另一个表后面
vector<Table>connect(vector<string>parts,vector<Table>database);

#endif