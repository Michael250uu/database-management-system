#include "../inc/Process.h"
#include "../inc/Data.h"
#include <iostream>
#include <sstream>
using namespace std;

//----------------------------------------------------------------------------------首字母选择
int Fchoose(vector<string> P)
{
    if (P.size() < 1)
        return 10;
    else
    {
        string first = P[0];
        if (first == "CREATE")
        {
            return 1;
        }
        else if (first == "SHOW")
        {
            return 2;
        }
        else if (first == "DROP")
        {
            return 3;
        }
        else if (first == "INSERT")
        {
            return 4;
        }
        else if (first == "SELECT")
        {
            return 5;
        }
        else if (first == "UPDATE")
        {
            return 6;
        }
        else if (first == "DELETE")
        {
            return 7;
        }
        else if (first == "ALTER")
        {
            return 8;
        }
        else if (first == "QUIT;")
        {
            return 9;
        }
        else if (first == "CONNECT")
        {
            return 10;
        }
        else
        {
            return 11;
        }
    }
}

//--------------------------------------------------------------------------------------新建表
vector<Table> Addtable(vector<Table> database, string name, string sen)
{
    if (check_sign(sen)) //检查（）;是否正确
    {
        cout << "instruction error!" << endl;
        return database;
    }
    int num = count_sign(sen);     //逗号数目
    sen = remove_sign(sen);        //去除（）;，
    vector<string> pre = Cut(sen); //将字符串分开
    //cout<<pre.size()<<" "<<num;
    if (pre.size() - num == 1) //--------------------不存在主键的情况
    {
        int n = count_P(pre);
        if (n != 0) //判断是否有字段叫PRIMARY_KEY
        {
            cout << "PRIMARY_KEY confusion!" << endl;
            return database;
        }
        if (find_table(name, database)) //是否会与其他表重名
        {
            cout << "name repeated!" << endl;
            return database;
        }
        if (check_column(pre)) //字段是否重名
        {
            cout << "column repeated!" << endl;
            return database;
        }
        Table table;
        Data data;
        data.values = pre;
        table.content.push_back(data);
        table.name = name;
        database.push_back(table);

        Cover_main(database);
        Cover_table(name, database);
        cout << "create successfully!" << endl;
        return database;
    }
    else if (pre.size() - num == 2) //-------------含有主键的情况
    {
        int n = count_P(pre);
        if (n != 1) //判断是否有字段叫PRIMARY_KEY
        {
            cout << "PRIMARY_KEY confusion!" << endl;
            return database;
        }
        int pos = 0;
        for (vector<string>::iterator it = pre.begin(); it != pre.end() - 1; it++, pos++)
            if (*(it + 1) == "PRIMARY_KEY") //判断是否有PRIMARY KEY这个字符串
            {
                pre.erase(it + 1);
                break;
            }
        if (find_table(name, database)) //是否会与其他表重名
        {
            cout << "name repeated!" << endl;
            return database;
        }
        if (check_column(pre)) //字段是否重名
        {
            cout << "column repeated!" << endl;
            return database;
        }
        Table table;
        Data data;
        data.values = pre;
        table.content.push_back(data);
        table.name = name;
        table.primary = true;
        table.position = pos;
        database.push_back(table);

        Cover_main(database);
        Cover_table(name, database);
        cout << "create successfully!" << endl;
        return database;
    }
    else
    {
        cout << "instruction error!" << endl;
        return database;
    }
}

//-------------------------------------------------------------------------------------展示所有表
void show(vector<Table> database)
{
    cout << database.size() << " table(s):" << endl;
    for (vector<Table>::iterator it = database.begin(); it != database.end(); it++)
    {
        cout << it->name << "(";
        if (it->primary == 0) //无主键的情况
            for (vector<string>::iterator ite = it->content[0].values.begin(); ite != it->content[0].values.end(); ite++)
            {
                cout << *ite;
                if (ite == it->content[0].values.end() - 1)
                    cout << ");" << endl;
                else
                    cout << ",";
            }
        else //有主键的情况
        {
            for (int i = 0; i <= it->content[0].values.size() - 1; i++)
            {
                cout << it->content[0].values[i];
                if (i == it->position)
                    cout << " PRIMARY_KEY";
                if (i == it->content[0].values.size() - 1)
                    cout << ");" << endl;
                else
                    cout << ",";
            }
        }
    }
}

//-------------------------------------------------------------------------------------------删除表
vector<Table> drop(string str, vector<Table> database)
{
    if (str.size() < 2 || *(str.end() - 1) != ';')
    {
        cout << "instruction error!" << endl;
        return database;
    }
    str.erase(str.end() - 1);
    if (find_table(str, database)) //存在该表
    {
        for (vector<Table>::iterator it = database.begin(); it != database.end(); it++)
            if (it->name == str)
            {
                database.erase(it);
                Cover_main(database);
                str += ".txt";
                remove(str.c_str());
                cout << "drop successfully" << endl;
                return database;
            }
    }
    else //不存在该表
    {
        cout << "cannot find table " << str << endl;
        return database;
    }
}

//--------------------------------------------------------------------------------------------插入新数据
vector<Table> insert(string sen, string name, vector<Table> database)
{
    if (check_sign(sen)) //检查格式（）;
    {
        cout << "instruction error!" << endl;
        return database;
    }
    sen = remove_sign(sen);         //除（）;，
    vector<string> pre = Cut(sen);  //将字符串分开
    if (find_table(name, database)) //存在表
    {
        vector<Table>::iterator it = database.begin();
        for (; it != database.end(); it++)
            if (it->name == name)
                break;                                  //it指向对应的表
        if (it->content[0].values.size() != pre.size()) //大小不匹配
        {
            //cout << it->content[0].values.size() << " " << pre.size() << endl;
            cout << "column error!" << endl;
            return database;
        }
        else //大小匹配
        {
            if (it->primary == 0) //没有主键
            {
                Data data;
                data.values = pre;
                it->content.push_back(data);
                Cover_table(it->name, database);
                cout << "insert successfully!" << endl;
                return database;
            }
            else //含有主键
            {
                if (check_re(it->position, pre[it->position], it))
                {
                    cout << "PRIMARY_KEY repeated!" << endl;
                    return database;
                }
                else
                {
                    Data data;
                    data.values = pre;
                    it->content.push_back(data);
                    Cover_table(it->name, database);
                    cout << "insert successfully!" << endl;
                    return database;
                }
            }
        }
    }
    else //不存在该表
    {
        cout << "cannot find table " << name << endl;
        return database;
    }
}

//---------------------------------------------------------------------------------------SELECT操作
void select(vector<string> parts, vector<Table> database)
{
    if (parts.size() < 4) //大小限制
    {
        cout << "instruction error!" << endl;
        return;
    }
    if (parts[1] == "*") //----------------------------所有字段
    {
        if (parts[2] != "FROM")
        {
            cout << "instruction error!" << endl;
            return;
        }
        if (parts.size() == 4) //无限制条件
        {
            if (*(parts[3].end() - 1) != ';') //判断末尾结构
            {
                cout << "instruction error!" << endl;
                return;
            }
            parts[3].erase(parts[3].end() - 1);
            string name = parts[3];
            if (find_table(name, database)) //查找表是否存在
            {
                for (vector<Table>::iterator it = database.begin(); it != database.end(); it++)
                    if (it->name == name)
                    {
                        for (vector<Data>::iterator ite = it->content.begin(); ite != it->content.end(); ite++)
                        {
                            for (vector<string>::iterator i = ite->values.begin(); i != ite->values.end(); i++)
                                cout << *i << '\t';
                            cout << endl;
                        }
                        return;
                    }
            }
            else
            {
                cout << "cannot find table " << name << endl;
                return;
            }
        }
        else //有限制条件
        {
            if (parts.size() == 6) //仅一个条件
            {
                if (*(parts[5].end() - 1) != ';' || parts[4] != "WHERE") //判断大致结构
                {
                    cout << "instruction error!" << endl;
                    return;
                }
                parts[5].erase(parts[5].end() - 1);
                string require = parts[5];
                if (check_require(require)) //判断约束条件格式
                {
                    if (find_table(parts[3], database)) //查找表是否存在
                    {
                        vector<Table>::iterator it = database.begin(); //it指向对应的表
                        for (; it != database.end(); it++)
                            if (it->name == parts[3])
                                break;
                        string col = extract_col(require);
                        string thing = extract_thing(require);
                        if (check_exist(col, it)) //查看字段是否存在
                        {
                            int pos = find_pos(col, it);
                            for (vector<string>::iterator i = it->content[0].values.begin(); i != it->content[0].values.end(); i++)
                                cout << *i << '\t';
                            cout << endl;
                            for (vector<Data>::iterator i1 = it->content.begin() + 1; i1 != it->content.end(); i1++)
                                if (i1->values[pos] == thing)
                                {
                                    for (vector<string>::iterator i2 = i1->values.begin(); i2 != i1->values.end(); i2++)
                                        cout << *i2 << '\t';
                                    cout << endl;
                                }
                            return;
                        }
                        else
                        {
                            cout << "cannot find column " << col << endl;
                            return;
                        }
                    }
                    else
                    {
                        cout << "cannot find table " << parts[3] << endl;
                        return;
                    }
                }
                else
                {
                    cout << "instruction error!" << endl;
                    return;
                }
            }
            else if (parts.size() == 8) //两个条件
            {
                if (parts[4] != "WHERE" || (parts[6] != "AND" && parts[6] != "OR") || *(parts[7].end() - 1) != ';') //判断大致结构
                {
                    cout << "instruction error!" << endl;
                    return;
                }
                parts[7].erase(parts[7].end() - 1);
                string R1 = parts[5], R2 = parts[7];
                if (check_require(R1) && check_require(R2)) //判断约束两个条件格式
                {
                    if (find_table(parts[3], database)) //判断表是否存在
                    {
                        vector<Table>::iterator it = database.begin(); //it指向对应的表
                        for (; it != database.end(); it++)
                            if (it->name == parts[3])
                                break;
                        string C1 = extract_col(R1), C2 = extract_col(R2), T1 = extract_thing(R1), T2 = extract_thing(R2);
                        if (check_exist(C1, it) && check_exist(C2, it)) //判断字段是否存在
                        {
                            int pos1 = find_pos(C1, it), pos2 = find_pos(C2, it);
                            for (vector<string>::iterator i = it->content[0].values.begin(); i != it->content[0].values.end(); i++)
                                cout << *i << '\t';
                            cout << endl;
                            if (parts[6] == "AND") //and情况
                            {
                                for (vector<Data>::iterator i1 = it->content.begin() + 1; i1 != it->content.end(); i1++)
                                    if (i1->values[pos1] == T1 && i1->values[pos2] == T2)
                                    {
                                        for (vector<string>::iterator i2 = i1->values.begin(); i2 != i1->values.end(); i2++)
                                            cout << *i2 << '\t';
                                        cout << endl;
                                    }
                                return;
                            }
                            else //or情况
                            {
                                for (vector<Data>::iterator i1 = it->content.begin() + 1; i1 != it->content.end(); i1++)
                                    if (i1->values[pos1] == T1 || i1->values[pos2] == T2)
                                    {
                                        for (vector<string>::iterator i2 = i1->values.begin(); i2 != i1->values.end(); i2++)
                                            cout << *i2 << '\t';
                                        cout << endl;
                                    }
                                return;
                            }
                        }
                        else
                        {
                            cout << "cannot find table " << C1 << " or " << C2 << endl;
                            return;
                        }
                    }
                    else
                    {
                        cout << "cannot find column " << parts[3] << endl;
                        return;
                    }
                }
                else
                {
                    cout << "instruction error!\n";
                    return;
                }
            }
            else
            {
                cout << "instruction error!" << endl;
                return;
            }
        }
    }
    else if (parts[1] == "DISTINCT") //---------------含有DISTINCT
    {
        if (parts.size() < 5) //大小判断
        {
            cout << "instruction error!\n";
            return;
        }
        if (parts[3] != "FROM")
        {
            cout << "instruction error!\n";
            return;
        }
        if (parts[2] == "*") //DISTINCT中的所有字段
        {
            if (parts.size() == 5) //无限制条件
            {
                string name = parts[4];
                if (*(name.end() - 1) != ';')
                {
                    cout << "instruction error!\n";
                    return;
                }
                name.erase(name.end() - 1);
                if (find_table(name, database)) //查找表是否存在
                {
                    vector<Table>::iterator it = database.begin();
                    for (; it != database.end(); it++)
                        if (it->name == name)
                            break; //it指向对应的表
                    for (vector<string>::iterator i = it->content[0].values.begin(); i != it->content[0].values.end(); i++)
                        cout << *i << '\t';
                    cout << endl;
                    it->content.erase(it->content.begin());
                    vector<Data> result = remove_repeat(it->content); //去重
                    show(result);
                    return;
                }
                else
                {
                    cout << "cannot find table " << name << endl;
                    return;
                }
            }
            else if (parts.size() == 7) //有一个限制
            {
                if (parts[5] != "WHERE" || *(parts[6].end() - 1) != ';') //大致结构判断
                {
                    cout << "instruction error!\n";
                    return;
                }
                string R = parts[6];
                R.erase(R.end() - 1);
                if (check_require(R)) //检查约束条件格式
                {
                    if (find_table(parts[4], database)) //查找表是否存在
                    {
                        vector<Table>::iterator it = database.begin(); //it指向对应的表
                        for (; it != database.end(); it++)
                            if (it->name == parts[4])
                                break;
                        string C = extract_col(R), T = extract_thing(R);
                        if (check_exist(C, it)) //查看字段是否存在
                        {
                            int pos = find_pos(C, it); //字段位置
                            for (vector<string>::iterator i = it->content[0].values.begin(); i != it->content[0].values.end(); i++)
                                cout << *i << '\t';
                            cout << endl;
                            it->content.erase(it->content.begin());
                            vector<Data> result = remove_repeat(it->content); //去重
                            for (vector<Data>::iterator i1 = result.begin(); i1 < result.end(); i1++)
                                if (i1->values[pos] == T)
                                {
                                    for (vector<string>::iterator i2 = i1->values.begin(); i2 < i1->values.end(); i2++)
                                        cout << *i2 << '\t';
                                    cout << endl;
                                }
                            return;
                        }
                        else
                        {
                            cout << "cannot find column " << C << " in table " << parts[4] << endl;
                            return;
                        }
                    }
                    else
                    {
                        cout << "cannot find table " << parts[4] << endl;
                        return;
                    }
                }
                else
                {
                    cout << "instruction error\n";
                    return;
                }
            }
            else if (parts.size() == 9) //两个限制条件
            {
                if (parts[5] != "WHERE" || (parts[7] != "AND" && parts[7] != "OR") || *(parts[8].end() - 1) != ';') //大致格式
                {
                    cout << "instruction error!\n";
                    return;
                }
                string R1 = parts[6], R2 = parts[8];
                R2.erase(R2.end() - 1);
                if (check_require(R1) && check_require(R2)) //判断约束条件格式
                {
                    if (find_table(parts[4], database)) //判断表是否存在
                    {
                        vector<Table>::iterator it = database.begin(); //it指向对应的表
                        for (; it != database.end(); it++)
                            if (it->name == parts[4])
                                break;
                        string C1 = extract_col(R1), C2 = extract_col(R2), T1 = extract_thing(R1), T2 = extract_thing(R2);
                        if (check_exist(C1, it) && check_exist(C2, it)) //查看字段是否都存在
                        {
                            int pos1 = find_pos(C1, it), pos2 = find_pos(C2, it); //约束条件位置
                            for (vector<string>::iterator i = it->content[0].values.begin(); i != it->content[0].values.end(); i++)
                                cout << *i << '\t';
                            cout << endl;
                            it->content.erase(it->content.begin());
                            vector<Data> result = remove_repeat(it->content); //去重
                            if (parts[7] == "AND")                            //AND
                            {
                                for (vector<Data>::iterator i1 = result.begin(); i1 < result.end(); i1++)
                                    if (i1->values[pos1] == T1 && i1->values[pos2] == T2)
                                    {
                                        for (vector<string>::iterator i2 = i1->values.begin(); i2 < i1->values.end(); i2++)
                                            cout << *i2 << '\t';
                                        cout << endl;
                                    }
                                return;
                            }
                            else //OR
                            {
                                for (vector<Data>::iterator i1 = result.begin(); i1 < result.end(); i1++)
                                    if (i1->values[pos1] == T1 || i1->values[pos2] == T2)
                                    {
                                        for (vector<string>::iterator i2 = i1->values.begin(); i2 < i1->values.end(); i2++)
                                            cout << *i2 << '\t';
                                        cout << endl;
                                    }
                                return;
                            }
                        }
                        else
                        {
                            cout << "cannot find column " << C1 << " or " << C2 << " in table " << parts[4] << endl;
                            return;
                        }
                    }
                    else
                    {
                        cout << "cannot find table " << parts[4] << endl;
                        return;
                    }
                }
                else
                {
                    cout << "instruction error!\n";
                    return;
                }
            }
            else
            {
                cout << "instruction error!\n";
                return;
            }
        }
        else //具体字段要求
        {
            string temp = remove_sign(parts[2]);
            vector<string> N = Cut(temp);
            if (parts.size() == 5) //无约束条件
            {
                string name = parts[4];
                if (*(name.end() - 1) != ';')
                {
                    cout << "instruction error!\n";
                    return;
                }
                name.erase(name.end() - 1);
                if (find_table(name, database)) //查看表是否存在
                {
                    vector<Table>::iterator it = database.begin(); //it指向对应的表
                    for (; it != database.end(); it++)
                        if (it->name == name)
                            break;
                    if (check_all_c(N, it)) //查看是否所有字段都存在
                    {
                        vector<int> poses = tran_col(N, it);
                        for (vector<string>::iterator i = N.begin(); i < N.end(); i++)
                            cout << *i << '\t';
                        cout << endl;
                        it->content.erase(it->content.begin());
                        vector<Data> V = it->content;
                        vector<Data> bra;
                        for (vector<Data>::iterator i1 = V.begin(); i1 < V.end(); i1++)
                        {
                            Data data;
                            for (vector<int>::iterator i2 = poses.begin(); i2 < poses.end(); i2++)
                            {
                                string temp;
                                temp = i1->values[*i2];
                                data.values.push_back(temp);
                            }
                            bra.push_back(data);
                        }
                        bra = remove_repeat(bra);
                        show(bra);
                        return;
                    }
                    else
                    {
                        cout << "not all columns exist!\n";
                        return;
                    }
                }
                else
                {
                    cout << "cannot find table " << name << endl;
                    return;
                }
            }
            else if (parts.size() == 7) //一个约束条件
            {
                if (parts[5] != "WHERE" || *(parts[6].end() - 1) != ';') //大致格式
                {
                    cout << "instruction error!\n";
                    cout << endl;
                }
                parts[6].erase(parts[6].end() - 1);
                string R = parts[6];
                if (check_require(R)) //条件格式
                {
                    if (find_table(parts[4], database)) //找表
                    {
                        vector<Table>::iterator it = database.begin(); //it指向对应的表
                        for (; it != database.end(); it++)
                            if (it->name == parts[4])
                                break;

                        string C = extract_col(R), T = extract_thing(R);

                        if (check_exist(C, it)) //约束字段是否存在
                        {
                            if (check_all_c(N, it)) //需显示字段是否都在
                            {
                                vector<int> poses = tran_col(N, it);
                                int pos = find_pos(C, it);

                                for (vector<string>::iterator i = N.begin(); i < N.end(); i++)
                                    cout << *i << '\t';
                                cout << endl;
                                it->content.erase(it->content.begin());
                                vector<Data> V = it->content, bra;
                                for (vector<Data>::iterator i1 = V.begin(); i1 < V.end(); i1++)
                                    if (i1->values[pos] == T)
                                    {
                                        Data data;
                                        for (vector<int>::iterator i2 = poses.begin(); i2 < poses.end(); i2++)
                                        {
                                            string temp = i1->values[*i2];
                                            data.values.push_back(temp);
                                        }
                                        bra.push_back(data);
                                    }
                                bra = remove_repeat(bra);
                                show(bra);
                                return;
                            }
                            else
                            {
                                cout << "not all columns exist in table " << parts[4] << endl;
                                return;
                            }
                        }
                        else
                        {
                            cout << "cannot find " << C << " in table " << parts[4] << endl;
                            return;
                        }
                    }
                    else
                    {
                        cout << "cannot find table " << parts[4] << endl;
                        return;
                    }
                }
                else
                {
                    cout << "instruction error!\n";
                    cout << endl;
                }
            }
            else if (parts.size() == 9) //两个约束条件
            {
                if (parts[5] != "WHERE" || (parts[7] != "AND" && parts[7] != "OR") || *(parts[8].end() - 1) != ';') //大致格式
                {
                    cout << "instruction error!\n";
                    return;
                }
                parts[8].erase(parts[8].end() - 1);
                string R1 = parts[6], R2 = parts[8];
                if (check_require(R1) && check_require(R2)) //约束条件格式
                {
                    if (find_table(parts[4], database)) //表是否存在
                    {
                        vector<Table>::iterator it = database.begin(); //it指向对应的表
                        for (; it != database.end(); it++)
                            if (it->name == parts[4])
                                break;
                        string C1 = extract_col(R1), C2 = extract_col(R2), T1 = extract_thing(R1), T2 = extract_thing(R2);
                        if (check_exist(C1, it) && check_exist(C2, it)) //约束字段是否存在
                        {
                            if (check_all_c(N, it)) //所需字段是否都存在
                            {
                                vector<int> poses = tran_col(N, it);
                                int pos1 = find_pos(C1, it), pos2 = find_pos(C2, it);
                                for (vector<string>::iterator i = N.begin(); i < N.end(); i++)
                                    cout << *i << '\t';
                                cout << endl;
                                it->content.erase(it->content.begin());
                                vector<Data> V = it->content, bra;
                                if (parts[7] == "AND") //AND情况
                                {
                                    for (vector<Data>::iterator i1 = V.begin(); i1 < V.end(); i1++)
                                        if (i1->values[pos1] == T1 && i1->values[pos2] == T2)
                                        {
                                            Data data;
                                            for (vector<int>::iterator i2 = poses.begin(); i2 < poses.end(); i2++)
                                            {
                                                string temp = i1->values[*i2];
                                                data.values.push_back(temp);
                                            }
                                            bra.push_back(data);
                                        }
                                    bra = remove_repeat(bra);
                                    show(bra);
                                    return;
                                }
                                else
                                {
                                    for (vector<Data>::iterator i1 = V.begin(); i1 < V.end(); i1++)
                                        if (i1->values[pos1] == T1 || i1->values[pos2] == T2)
                                        {
                                            Data data;
                                            for (vector<int>::iterator i2 = poses.begin(); i2 < poses.end(); i2++)
                                            {
                                                string temp = i1->values[*i2];
                                                data.values.push_back(temp);
                                            }
                                            bra.push_back(data);
                                        }
                                    bra = remove_repeat(bra);
                                    show(bra);
                                    return;
                                }
                            }
                            else
                            {
                                cout << "not all columns exist!\n";
                                return;
                            }
                        }
                        else
                        {
                            cout << "cannot find column " << C1 << " or " << C2 << " in table " << parts[4] << endl;
                            return;
                        }
                    }
                    else
                    {
                        cout << "cannot find table " << parts[4] << "!\n";
                        return;
                    }
                }
                else
                {
                    cout << "instruction error!\n";
                    return;
                }
            }
            else
            {
                cout << "instruction error!\n";
                return;
            }
        }
    }
    else //普通
    {
        if (parts.size() < 4)
        {
            cout << "instruction error!\n";
            return;
        }
        if (parts[2] != "FROM")
        {
            cout << "instruction error!\n";
            return;
        }
        string temp = remove_sign(parts[1]);
        vector<string> N = Cut(temp);
        if (parts.size() == 4) //无约束条件
        {
            string name = parts[3];
            if (*(name.end() - 1) != ';')
            {
                cout << "instruction error!\n";
                return;
            }
            name.erase(name.end() - 1);
            if (find_table(name, database)) //找表
            {
                vector<Table>::iterator it = database.begin(); //it指向对应的表
                for (; it != database.end(); it++)
                    if (it->name == name)
                        break;

                if (check_all_c(N, it))
                {
                    vector<int> poses = tran_col(N, it);
                    for (vector<string>::iterator i = N.begin(); i < N.end(); i++)
                        cout << *i << '\t';
                    cout << endl;
                    it->content.erase(it->content.begin());
                    vector<Data> V = it->content;
                    vector<Data> bra;
                    for (vector<Data>::iterator i1 = V.begin(); i1 < V.end(); i1++)
                    {
                        Data data;
                        for (vector<int>::iterator i2 = poses.begin(); i2 < poses.end(); i2++)
                        {
                            string temp;
                            temp = i1->values[*i2];
                            data.values.push_back(temp);
                        }
                        bra.push_back(data);
                    }
                    show(bra);
                    return;
                }
                else
                {
                    cout << "not all columns exist!\n";
                    return;
                }
            }
            else
            {
                cout << "cannot find table " << name << "!\n";
                return;
            }
        }
        else if (parts.size() == 6) //一个约束条件
        {
            if (parts[4] != "WHERE" || *(parts[5].end() - 1) != ';')
            {
                cout << "instruction error!\n";
                return;
            }
            string R = parts[5];
            R.erase(R.end() - 1);
            if (check_require(R)) //约束条件格式
            {
                if (find_table(parts[3], database))
                {
                    vector<Table>::iterator it = database.begin(); //it指向对应的表
                    for (; it != database.end(); it++)
                        if (it->name == parts[3])
                            break;
                    string C = extract_col(R), T = extract_thing(R);
                    if (check_exist(C, it)) //约束条件存在
                    {
                        int pos = find_pos(C, it);
                        if (check_all_c(N, it))
                        {
                            vector<int> poses = tran_col(N, it);
                            for (vector<string>::iterator i = N.begin(); i < N.end(); i++)
                                cout << *i << '\t';
                            cout << endl;
                            it->content.erase(it->content.begin());
                            vector<Data> V = it->content, bra;
                            for (vector<Data>::iterator i1 = V.begin(); i1 < V.end(); i1++)
                                if (i1->values[pos] == T)
                                {
                                    Data data;
                                    for (vector<int>::iterator i2 = poses.begin(); i2 < poses.end(); i2++)
                                    {
                                        string temp;
                                        temp = i1->values[*i2];
                                        data.values.push_back(temp);
                                    }
                                    bra.push_back(data);
                                }
                            show(bra);
                            return;
                        }
                        else
                        {
                            cout << "not all columns exist!\n";
                            return;
                        }
                    }
                    else
                    {
                        cout << "no " << C << " in table " << parts[3] << endl;
                        return;
                    }
                }
                else
                {
                    cout << "cannot find table " << parts[3] << "!\n";
                    return;
                }
            }
            else
            {
                cout << "instruction error!\n";
                return;
            }
        }
        else if (parts.size() == 8) //两个约束条件
        {
            if (parts[4] != "WHERE" || (parts[6] != "AND" && parts[6] != "OR") || *(parts[7].end() - 1) != ';') //大致格式
            {
                cout << "instruction error!\n";
                return;
            }
            parts[7].erase(parts[7].end() - 1);
            string R1 = parts[5], R2 = parts[7];
            if (check_require(R1) && check_require(R2)) //约束条件格式
            {
                if (find_table(parts[3], database)) //表是否存在
                {
                    vector<Table>::iterator it = database.begin(); //it指向对应的表
                    for (; it != database.end(); it++)
                        if (it->name == parts[3])
                            break;
                    string C1 = extract_col(R1), C2 = extract_col(R2), T1 = extract_thing(R1), T2 = extract_thing(R2);
                    if (check_exist(C1, it) && check_exist(C2, it)) //约束字段是否存在
                    {
                        if (check_all_c(N, it)) //所需字段是否都存在
                        {
                            vector<int> poses = tran_col(N, it);
                            int pos1 = find_pos(C1, it), pos2 = find_pos(C2, it);
                            for (vector<string>::iterator i = N.begin(); i < N.end(); i++)
                                cout << *i << '\t';
                            cout << endl;
                            it->content.erase(it->content.begin());
                            vector<Data> V = it->content, bra;
                            if (parts[6] == "AND") //AND情况
                            {
                                for (vector<Data>::iterator i1 = V.begin(); i1 < V.end(); i1++)
                                    if (i1->values[pos1] == T1 && i1->values[pos2] == T2)
                                    {
                                        Data data;
                                        for (vector<int>::iterator i2 = poses.begin(); i2 < poses.end(); i2++)
                                        {
                                            string temp = i1->values[*i2];
                                            data.values.push_back(temp);
                                        }
                                        bra.push_back(data);
                                    }
                                show(bra);
                                return;
                            }
                            else
                            {
                                for (vector<Data>::iterator i1 = V.begin(); i1 < V.end(); i1++)
                                    if (i1->values[pos1] == T1 || i1->values[pos2] == T2)
                                    {
                                        Data data;
                                        for (vector<int>::iterator i2 = poses.begin(); i2 < poses.end(); i2++)
                                        {
                                            string temp = i1->values[*i2];
                                            data.values.push_back(temp);
                                        }
                                        bra.push_back(data);
                                    }
                                show(bra);
                                return;
                            }
                        }
                        else
                        {
                            cout << "not all columns exist!\n";
                            return;
                        }
                    }
                    else
                    {
                        cout << "cannot find column " << C1 << " or " << C2 << " in table " << parts[3] << endl;
                        return;
                    }
                }
                else
                {
                    cout << "cannot find table " << parts[3] << "!\n";
                    return;
                }
            }
            else
            {
                cout << "instruction error!\n";
                return;
            }
        }
        else
        {
            cout << "instruction error!\n";
            return;
        }
    }
}

//UPDATE操作
vector<Table> update(vector<string> parts, vector<Table> database)
{
    if (parts.size() == 4) //无约束条件
    {
        if (parts[2] != "SET" || *(parts[3].end() - 1) != ';')
        {
            cout << "instruction error!\n";
            return database;
        }
        string R = parts[3];
        R.erase(R.end() - 1);
        if (check_require(R)) //条件格式
        {
            if (find_table(parts[1], database)) //找表
            {
                string C = extract_col(R), T = extract_thing(R);
                vector<Table>::iterator it = database.begin(); //it指向对应的表
                for (; it != database.end(); it++)
                    if (it->name == parts[1])
                        break;
                if (check_exist(C, it)) //改变字段是否存在
                {
                    if (it->primary) //含有主键
                    {
                        vector<Table> before = database;
                        int pos = find_pos(C, it);
                        for (vector<Data>::iterator i1 = it->content.begin() + 1; i1 < it->content.end(); i1++)
                            i1->values[pos] = T;
                        if (check_u(it))
                        {
                            cout << "PRIMARY_KEY repeated\n";
                            return before;
                        }
                        else
                        {
                            Cover_table(parts[1], database);
                            cout << "update successfully!\n";
                            return database;
                        }
                    }
                    else
                    {
                        int pos = find_pos(C, it);
                        for (vector<Data>::iterator i1 = it->content.begin() + 1; i1 < it->content.end(); i1++)
                            i1->values[pos] = T;
                        Cover_table(parts[1], database);
                        cout << "update successfully!\n";
                        return database;
                    }
                }
                else
                {
                    cout << "no " << C << " in table " << parts[1] << endl;
                    return database;
                }
            }
            else
            {
                cout << "cannot find table " << parts[1] << "!\n";
                return database;
            }
        }
        else
        {
            cout << "instruction error!\n";
            return database;
        }
    }
    else if (parts.size() == 6) //一个约束条件
    {
        if (parts[2] != "SET" || parts[4] != "WHERE" || *(parts[5].end() - 1) != ';')
        {
            cout << "instruction error!\n";
            return database;
        }
        string U = parts[3], R = parts[5];
        R.erase(R.end() - 1);
        if (check_require(U) && check_require(R)) //条件格式
        {
            string C_ = extract_col(U), T_ = extract_thing(U), C = extract_col(R), T = extract_thing(R);
            if (find_table(parts[1], database)) //找表
            {
                vector<Table>::iterator it = database.begin(); //it指向对应的表
                for (; it != database.end(); it++)
                    if (it->name == parts[1])
                        break;
                if (check_exist(C_, it) && check_exist(C, it)) //字段是否存在
                {
                    if (it->primary) //含有主键
                    {
                        vector<Table> before = database;
                        int pos_ = find_pos(C_, it), pos = find_pos(C, it);
                        for (vector<Data>::iterator i1 = it->content.begin() + 1; i1 < it->content.end(); i1++)
                            if (i1->values[pos] == T)
                                i1->values[pos_] = T_;
                        if (check_u(it))
                        {
                            cout << "PRIMARY_KEY repeated!\n";
                            return before;
                        }
                        else
                        {
                            Cover_table(parts[1], database);
                            cout << "update successfully!\n";
                            return database;
                        }
                    }
                    else
                    {
                        int pos_ = find_pos(C_, it), pos = find_pos(C, it);
                        for (vector<Data>::iterator i1 = it->content.begin() + 1; i1 < it->content.end(); i1++)
                            if (i1->values[pos] == T)
                                i1->values[pos_] = T_;
                        Cover_table(parts[1], database);
                        cout << "update successfully!\n";
                        return database;
                    }
                }
                else
                {
                    cout << "no " << C_ << " or " << C << " in table " << parts[1] << "!\n";
                    return database;
                }
            }
            else
            {
                cout << "cannot find table " << parts[1] << "!\n";
                return database;
            }
        }
        else
        {
            cout << "instruction error!\n";
            return database;
        }
    }
    else if (parts.size() == 8) //两个约束条件
    {
        if (parts[2] != "SET" || parts[4] != "WHERE" || *(parts[7].end() - 1) != ';' || (parts[6] != "AND" && parts[6] != "OR"))
        {
            cout << "instruction error!\n";
            return database;
        }
        string U = parts[3], R1 = parts[5], R2 = parts[7];
        R2.erase(R2.end() - 1);
        if (check_require(U) && check_require(R1) && check_require(R2)) //条件格式
        {
            string C_ = extract_col(U), T_ = extract_thing(U), C1 = extract_col(R1), T1 = extract_thing(R1), C2 = extract_col(R2), T2 = extract_thing(R2);
            if (find_table(parts[1], database)) //找表
            {
                vector<Table>::iterator it = database.begin(); //it指向对应的表
                for (; it < database.end(); it++)
                    if (it->name == parts[1])
                        break;
                if (check_exist(C_, it) && check_exist(C1, it) && check_exist(C2, it)) //字段是否存在
                {
                    int pos_ = find_pos(C_, it), pos1 = find_pos(C1, it), pos2 = find_pos(C2, it);
                    if (parts[6] == "AND")
                    {
                        if (it->primary) //含有主键
                        {
                            vector<Table> before = database;
                            for (vector<Data>::iterator i1 = it->content.begin() + 1; i1 < it->content.end(); i1++)
                                if (i1->values[pos1] == T1 && i1->values[pos2] == T2)
                                    i1->values[pos_] = T_;
                            if (check_u(it))
                            {
                                cout << "PRIMARY_KEY repeated!\n";
                                return before;
                            }
                            else
                            {
                                Cover_table(parts[1], database);
                                cout << "update successfully!\n";
                                return database;
                            }
                        }
                        else
                        {

                            for (vector<Data>::iterator i1 = it->content.begin() + 1; i1 < it->content.end(); i1++)
                                if (i1->values[pos1] == T1 && i1->values[pos2] == T2)
                                    i1->values[pos_] = T_;
                            Cover_table(parts[1], database);
                            cout << "update successfully!\n";
                            return database;
                        }
                    }
                    else
                    {
                        if (it->primary) //含有主键
                        {
                            vector<Table> before = database;
                            for (vector<Data>::iterator i1 = it->content.begin() + 1; i1 < it->content.end(); i1++)
                                if (i1->values[pos1] == T1 || i1->values[pos2] == T2)
                                    i1->values[pos_] = T_;
                            if (check_u(it))
                            {
                                cout << "PRIMARY_KEY repeated!\n";
                                return before;
                            }
                            else
                            {
                                Cover_table(parts[1], database);
                                cout << "update successfully!\n";
                                return database;
                            }
                        }
                        else
                        {

                            for (vector<Data>::iterator i1 = it->content.begin() + 1; i1 < it->content.end(); i1++)
                                if (i1->values[pos1] == T1 || i1->values[pos2] == T2)
                                    i1->values[pos_] = T_;
                            Cover_table(parts[1], database);
                            cout << "update successfully!\n";
                            return database;
                        }
                    }
                }
                else
                {
                    cout << "not all columns exist!\n";
                    return database;
                }
            }
            else
            {
                cout << "cannot find table " << parts[1] << "!\n";
                return database;
            }
        }
        else
        {
            cout << "instruction error!\n";
            return database;
        }
    }
    else
    {
        cout << "instruction error!\n";
        return database;
    }
}

//---------------------------------------------------------------DELETE操作
vector<Table> Delete(vector<string> parts, vector<Table> database)
{
    if (parts.size() == 3) //无条件
    {
        if (parts[1] != "FROM" || *(parts[2].end() - 1) != ';')
        {
            cout << "instruction error!\n";
            return database;
        }
        string name = parts[2];
        name.erase(name.end() - 1);
        if (find_table(name, database)) //找表
        {
            vector<Table>::iterator it = database.begin(); //it指向对应的表
            for (; it < database.end(); it++)
                if (it->name == name)
                    break;
            it->content.erase(it->content.begin() + 1, it->content.end());
            Cover_table(name, database);
            cout << "delete successfully!\n";
            return database;
        }
        else
        {
            cout << "no table " << name << "!\n";
            return database;
        }
    }
    else if (parts.size() == 5) //一个条件
    {
        if (parts[1] != "FROM" || parts[3] != "WHERE" || *(parts[4].end() - 1) != ';')
        {
            cout << "instruction error!\n";
            return database;
        }
        string R = parts[4];
        R.erase(R.end() - 1);
        if (check_require(R))
        {
            if (find_table(parts[2], database))
            {
                string C = extract_col(R), T = extract_thing(R);
                vector<Table>::iterator it = database.begin(); //it指向对应的表
                for (; it < database.end(); it++)
                    if (it->name == parts[2])
                        break;
                if (check_exist(C, it))
                {
                    int pos = find_pos(C, it);
                    for (vector<Data>::iterator i = it->content.begin() + 1; i < it->content.end();)
                        if (i->values[pos] == T)
                            it->content.erase(i);
                        else
                            i++;
                    Cover_table(parts[2], database);
                    cout << "delete successfully!\n";
                    return database;
                }
                else
                {
                    cout << "no column " << C << " in table " << parts[2] << "!\n";
                    return database;
                }
            }
            else
            {
                cout << "no table " << parts[2] << "!\n";
                return database;
            }
        }
        else
        {
            cout << "instruction error!\n";
            return database;
        }
    }
    else if (parts.size() == 7) //两个条件
    {
        if (parts[1] != "FROM" || parts[3] != "WHERE" || *(parts[6].end() - 1) != ';' || (parts[5] != "AND" && parts[5] != "OR"))
        {
            cout << "instruction error!\n";
            return database;
        }
        string R1 = parts[4], R2 = parts[6];
        R2.erase(R2.end() - 1);
        if (check_require(R1) && check_require(R2))
        {
            if (find_table(parts[2], database))
            {
                string C1 = extract_col(R1), T1 = extract_thing(R1), C2 = extract_col(R2), T2 = extract_thing(R2);
                vector<Table>::iterator it = database.begin(); //it指向对应的表
                for (; it < database.end(); it++)
                    if (it->name == parts[2])
                        break;
                if (check_exist(C1, it) && check_exist(C2, it))
                {
                    int pos1 = find_pos(C1, it), pos2 = find_pos(C2, it);
                    if (parts[5] == "AND")
                    {
                        for (vector<Data>::iterator i = it->content.begin() + 1; i < it->content.end();)
                            if (i->values[pos1] == T1 && i->values[pos2] == T2)
                                it->content.erase(i);
                            else
                                i++;
                        Cover_table(parts[2], database);
                        cout << "delete successfully!\n";
                        return database;
                    }
                    else
                    {
                        for (vector<Data>::iterator i = it->content.begin() + 1; i < it->content.end();)
                            if (i->values[pos1] == T1 || i->values[pos2] == T2)
                                it->content.erase(i);
                            else
                                i++;
                        Cover_table(parts[2], database);
                        cout << "delete successfully!\n";
                        return database;
                    }
                }
                else
                {
                    cout << "no column " << C1 << " or " << C2 << " in table " << parts[2] << "!\n";
                    return database;
                }
            }
            else
            {
                cout << "no table " << parts[2] << "!\n";
                return database;
            }
        }
        else
        {
            cout << "instruction error!\n";
            return database;
        }
    }
    else
    {
        cout << "instruction error!\n";
        return database;
    }
}

//ALTER操作
vector<Table> alter(vector<string> parts, vector<Table> database)
{
    if (parts.size() == 5)
    {
        if (parts[1] != "TABLE" || (parts[3] != "ADD" && parts[3] != "DROP") || *(parts[4].end() - 1) != ';' || parts[4].size() < 2)
        {
            cout << "instruction error!\n";
            return database;
        }
        if (find_table(parts[2], database)) //找表
        {
            vector<Table>::iterator it = database.begin(); //it指向对应的表
            for (; it < database.end(); it++)
                if (it->name == parts[2])
                    break;
            string C = parts[4];
            C.erase(C.end() - 1);
            if (parts[3] == "ADD") //增加
            {
                if (C == "PRIMARY_KEY") //增加主键
                {
                    cout << "instruction error!\n";
                    return database;
                }
                else //增加字段
                {
                    if (check_exist(C, it)) //字段重复
                    {
                        cout << "column repeated!\n";
                        return database;
                    }
                    else
                    {
                        it->content[0].values.push_back(C);
                        for (vector<Data>::iterator i = it->content.begin() + 1; i < it->content.end(); i++)
                            i->values.push_back("NULL");
                        Cover_table(parts[2], database);
                        cout << "alter successfully!\n";
                        return database;
                    }
                }
            }
            else //减少
            {
                if (C == "PRIMARY_KEY") //删除主键
                {
                    if (it->primary) //存在
                    {
                        it->primary = false;
                        it->position = 0;
                        Cover_main(database);
                        cout << "drop PRIMARY_KEY successfully!\n";
                        return database;
                    }
                    else
                    {
                        cout << "no PRIMARY_KEY in " << parts[2] << "!\n";
                        return database;
                    }
                }
                else //删除字段
                {
                    if (check_exist(C, it)) //存在
                    {
                        int pos = find_pos(C, it);
                        if (it->primary && it->position == pos) //含有主键且删除的字段为主键
                        {
                            it->primary = false;
                            it->position = 0;
                            for (vector<Data>::iterator i = it->content.begin(); i < it->content.end(); i++)
                            {
                                int n = 0;
                                vector<string>::iterator ite = i->values.begin();
                                for (; ite < i->values.end(); ite++, n++)
                                    if (n == pos)
                                        break;
                                i->values.erase(ite);
                            }
                            Cover_main(database);
                            Cover_table(parts[2], database);
                            cout << "drop " << C << " successfully!\n";
                            return database;
                        }
                        else
                        {
                            for (vector<Data>::iterator i = it->content.begin(); i < it->content.end(); i++)
                            {
                                int n = 0;
                                vector<string>::iterator ite = i->values.begin();
                                for (; ite < i->values.end(); ite++, n++)
                                    if (n == pos)
                                        break;
                                i->values.erase(ite);
                            }
                            Cover_table(parts[2], database);
                            cout << "drop " << C << " successfully!\n";
                            return database;
                        }
                    }
                    else
                    {
                        cout << "no " << C << " in table " << parts[2] << "!\n";
                        return database;
                    }
                }
            }
        }
        else
        {
            cout << "no table " << parts[2] << "!\n";
            return database;
        }
    }
    else if (parts.size() == 6)
    {
        if (parts[1] == "TABLE" && parts[3] == "ADD" && parts[4] == "PRIMARY_KEY" && !check_sign(parts[5]))
        {

            if (find_table(parts[2], database)) //找表
            {

                vector<Table>::iterator it = database.begin(); //it指向对应的表
                for (; it < database.end(); it++)
                    if (it->name == parts[2])
                        break;
                if (it->primary) //已有主键
                {
                    cout << "PRIMARY_KEY has existed!\n";
                    return database;
                }
                else
                {
                    string C = remove_sign(parts[5]);
                    stringstream input(C);
                    input >> C;
                    if (check_exist(C, it)) //字段是否存在
                    {
                        it->position = find_pos(C, it);
                        it->primary = 1;
                        if (check_u(it)) //内容是否重复
                        {
                            it->primary = 0;
                            it->position = 0;
                            cout << "content repeated!\n";
                            return database;
                        }
                        else
                        {
                            Cover_main(database);
                            cout << "add PRIMARY_KEY successfully!\n";
                            return database;
                        }
                    }
                    else
                    {
                        cout << "no " << C << " in table " << parts[2] << "!\n";
                        return database;
                    }
                }
            }
            else
            {
                cout << "no table " << parts[2] << "!\n";
                return database;
            }
        }
        else
        {
            cout << "instruction error!\n";
            return database;
        }
    }
    else
    {
        cout << "instruction error!\n";
        return database;
    }
}

//将一个表插入到另一个表后面
vector<Table> connect(vector<string> parts, vector<Table> database)
{
    if (parts.size() == 5 && parts[1] == "TABLE" && parts[3] == "AFTER" && *(parts[4].end() - 1) == ';')
    {
        string n1 = parts[2], n2 = parts[4];
        n2.erase(n2.end() - 1);
        if (find_table(n1, database) && find_table(n2, database)) //找表
        {
            vector<Table>::iterator it1 = database.begin(); //it指向对应的表
            for (; it1 < database.end(); it1++)
                if (it1->name == n1)
                    break;
            vector<Table>::iterator it2 = database.begin(); //it指向对应的表
            for (; it2 < database.end(); it2++)
                if (it2->name == n2)
                    break;
            if (it1->content[0].values == it2->content[0].values && it1->primary == it2->primary && it1->primary == 0 && n1 != n2)
            {
                it2->content.insert(it2->content.end(), it1->content.begin() + 1, it1->content.end());
                Cover_table(n2, database);
                cout << "connect successfully!\n";
                return database;
            }
            else if (it1->content[0].values == it2->content[0].values && it1->primary == it2->primary && it1->primary == 1 && it1->position == it2->position && n1 != n2)
            {
                vector<Table> before = database;
                it2->content.insert(it2->content.end(), it1->content.begin() + 1, it1->content.end());
                if (check_u(it2))
                {
                    cout << "PRIMARY_KEY repeated!\n";
                    return before;
                }
                else
                {
                    cout << "connect successfully!\n";
                    return database;
                }
            }
            else
            {
                cout << "cannot connect!\n";
                return database;
            }
        }
        else
        {
            cout << "not both tables exist!\n";
            return database;
        }
    }
    else
    {
        cout << "instruction error!\n";
        return database;
    }
}