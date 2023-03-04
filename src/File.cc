/*
 * @Author: your name
 * @Date: 2021-04-23 11:46:23
 * @LastEditTime: 2021-05-02 14:57:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /proj2/src/File.cc
 */
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../inc/File.h"
using namespace std;

//------------------------------------------------------------------------初始创建动态数据库
vector<Table> Create()
{
    vector<Table> database;
    fstream F("database.txt");
    if (!F)
    {
        cout << "error opening database.txt" << endl;
        exit(1);
    }

    while (F.peek() != EOF)
    {
        Table table;
        F >> table.name;
        F >> table.primary;
        F >> table.position;
        fstream G(table.name + ".txt");
        if (!G)
        {
            cout << "error opening " << table.name << ".txt" << endl;
            exit(1);
        }
        char ch;
        G.get(ch);
        while (!G.eof())
        {
            Data data;
            string all;
            string temp;
            getline(G, all);
            data.values = Cut(all);
            table.content.push_back(data);
        }
        database.push_back(table);
    }
    F.close();
    return database;
}

//-----------------------------------------------------------------------------覆盖主文件
void Cover_main(vector<Table> database)
{
    ofstream F("database.txt");
    for (vector<Table>::iterator it = database.begin(); it != database.end(); it++)
        F << '\n'
          << it->name << " " << it->primary << " " << it->position;
    F.close();
}

//----------------------------------------------------------------------------覆盖表格文件
void Cover_table(string name, vector<Table> T)
{
    ofstream F(name + ".txt");
    vector<Table>::iterator it1 = T.begin();
    for (; it1 != T.end(); it1++)
        if (it1->name == name)
            break;
    for (vector<Data>::iterator it2 = it1->content.begin(); it2 != it1->content.end(); it2++)
    {
        F << '\n';
        for (vector<string>::iterator it3 = it2->values.begin(); it3 != it2->values.end(); it3++)
            F << *it3 << " ";
    }
    F.close();
}
