/*
 * @Author: your name
 * @Date: 2021-04-25 21:48:18
 * @LastEditTime: 2021-05-06 17:09:31
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /proj2/src/Analysis.cc
 */
#include "../inc/Analysis.h"
#include <sstream>
#include <algorithm>
//#include <iostream>
using namespace std;

//------------------------------------------------------------------------------分割字符串
vector<string> Cut(string str)
{
    vector<string> a;
    stringstream input(str);
    string temp;
    while (input >> temp)
    {
        a.push_back(temp);
    }
    return a;
}

bool check_sign(string str)
{
    if (str.size() < 3)
        return 1;
    if (str[0] != '(' || str[str.size() - 1] != ';' || str[str.size() - 2] != ')')
        return 1;
    return 0;
}

//-----------------------------------------------------------------------------------逗号个数
int count_sign(string str)
{
    int n = 0;
    for (string::iterator it = str.begin(); it != str.end(); it++)
        if (*it == ',')
            n++;
    return n;
}

//-----------------------------------------------------------------------------------除（）;
string remove_sign(string str)
{
    for (string::iterator it = str.begin(); it != str.end(); it++)
        if (*it == '(' || *it == ')' || *it == ';' || *it == ',')
            *it = ' ';
    return str;
}

//----------------------------------------------------------------------------------查找表
bool find_table(string name, vector<Table> database)
{
    for (vector<Table>::iterator it = database.begin(); it != database.end(); it++)
        if (it->name == name)
            return 1;
    return 0;
}

//---------------------------------------------------------------------------------判断字段是否重复
bool check_column(vector<string> v)
{
    sort(v.begin(), v.end());
    vector<string>::iterator it = unique(v.begin(), v.end());
    if (it != v.end())
        return 1;
    return 0;
}

//-------------------------------------------------------------------------------判断地一个是否为PRIMARY
bool check_P(string str)
{
    if (str.size() < 13)
        return 0;
    if (str[0] == 'P' && str[1] == 'R' && str[2] == 'I' && str[3] == 'M' && str[4] == 'A' && str[5] == 'R' && str[6] == 'Y' && str[7] == '_' && str[8] == 'K' && str[9] == 'E' && str[10] == 'Y' && (str[11] == ',' || str[11] == ')'))
        return 1;
    return 0;
}

//------------------------------------------------------------------------------------PRIMARY_KEY的数量
int count_P(vector<string> v)
{
    int n = 0;
    for (vector<string>::iterator it = v.begin(); it != v.end(); it++)
        if (*it == "PRIMARY_KEY")
            n++;
    return n;
}

//-------------------------------------------------------------------------------------查看主键是否重复
bool check_re(int pos, string str, vector<Table>::iterator i)
{
    for (vector<Data>::iterator it = i->content.begin() + 1; it < i->content.end(); it++)
        if (str == it->values[pos])
            return 1;
    return 0;
}

//-------------------------------------------------------------------------------------判断约束条件格式
bool check_require(string str)
{
    if (str.size() < 3)
        return 0;
    if (count(str.begin() + 1, str.end() - 1, '=') == 1 && count(str.begin(), str.end(), '=') == 1)
        return 1;
    return 0;
}

//-------------------------------------------------------------------------------------解析约束条件前的字段
string extract_col(string str)
{
    int pos = str.find('=');
    return str.substr(0, pos);
}

//-------------------------------------------------------------------------------------解析约束条件后的内容
string extract_thing(string str)
{
    int pos = str.find('=');
    return str.substr(pos + 1, str.size());
}

//--------------------------------------------------------------------------------------判断字段是否存在
bool check_exist(string str, vector<Table>::iterator i)
{
    for (vector<string>::iterator it = i->content[0].values.begin(); it < i->content[0].values.end(); it++)
        if (*it == str)
            return 1;
    return 0;
}

//----------------------------------------------------------------------------------------字段的位置
int find_pos(string str, vector<Table>::iterator i)
{
    int n = 0;
    for (vector<string>::iterator it = i->content[0].values.begin(); it != i->content[0].values.end(); it++, n++)
        if (*it == str)
            return n;
}

//-----------------------------------------------------------------------------------------DISTINCT的去重
vector<Data> remove_repeat(vector<Data> v)
{
    for (vector<Data>::iterator it = v.begin(); it < v.end(); it++)
        for (vector<Data>::iterator ite = it + 1; ite < v.end();)
            if (ite->values == it->values)
            {
                v.erase(ite);
            }
            else
                ite++;
    return v;
}

//------------------------------------------------------------------------------------输出vector<Data>内容
void show(vector<Data> v)
{
    for (vector<Data>::iterator i = v.begin(); i < v.end(); i++)
    {
        for (vector<string>::iterator it = i->values.begin(); it < i->values.end(); it++)
            cout << *it << '\t';
        cout << endl;
    }
}

//------------------------------------------------------------------------------------查看所有字段是否都存在
bool check_all_c(vector<string> c, vector<Table>::iterator i)
{
    for (vector<string>::iterator it1 = c.begin(); it1 < c.end(); it1++)
    {
        bool flag = 0;
        for (vector<string>::iterator it2 = i->content[0].values.begin(); it2 < i->content[0].values.end(); it2++)
            if (*it1 == *it2)
            {
                flag = 1;
                break;
            }
        if (flag == 0)
            return 0;
    }
    return 1;
}

//-----------------------------------------------------------------------------------将一串字段转化为位置
vector<int> tran_col(vector<string> c, vector<Table>::iterator it)
{
    vector<int> V;
    for (vector<string>::iterator i = c.begin(); i < c.end(); i++)
    {
        int temp = find_pos(*i, it);
        V.push_back(temp);
    }
    return V;
}

//判断主键有无重复
bool check_u(vector<Table>::iterator it)
{
    for (vector<Data>::iterator i1 = it->content.begin() + 1; i1 < it->content.end(); i1++)
        for (vector<Data>::iterator i2 = i1 + 1; i2 < it->content.end(); i2++)
            if (i1->values[it->position] == i2->values[it->position])
                return 1;
    return 0;
}


