#include <iostream>
#include <stdio.h>
#include <vector>
#include "../inc/Data.h"
#include "../inc/Process.h"
#include "../inc/File.h"
#include "../inc/Analysis.h"
using namespace std;

int main()
{
    vector<Table> Database = Create();

    while (true)
    {
        cout << "mysql>";
        string ALL;
        int choose, flag = 0;
        getline(cin, ALL);
        vector<string> Parts = Cut(ALL);
        choose = Fchoose(Parts);
        switch (choose)
        {
        //---------------------------------------------------------新建表
        case 1:
        {
            if (Parts.size() != 4 && Parts.size() != 5)
            {
                cout << "instruction error!" << endl;
                break;
            }
            if (Parts[1] != "TABLE")
            {
                cout << "instruction error!" << endl;
                break;
            }
            string last;
            if (Parts.size() == 4)
                last = Parts[3];
            else
            {
                if (check_P(Parts[4])) //检查末尾的开头是否是PRIMARY——KEY;
                    last = Parts[3] + " " + Parts[4];
                else
                {
                    cout << "instruction error!" << endl;
                    break;
                }
            }

            Database = Addtable(Database, Parts[2], last);

            break;
        }
        case 2:
        {
            if (Parts.size() == 2 && Parts[1] == "TABLES;")
                show(Database);
            else
            {
                cout << "instruction error!" << endl;
            }
            break;
        }
        case 3:
        {
            if (Parts.size() == 3 && Parts[1] == "TABLE")
            {
                Database = drop(Parts[2], Database);
            }
            else
            {
                cout << "instruction error!" << endl;
            }
            break;
        }
        case 4:
        {
            if (Parts.size() == 5 && Parts[1] == "INTO" && Parts[3] == "VALUES")
                Database = insert(Parts[4], Parts[2], Database);
            else
                cout << "instruction error!" << endl;
            break;
        }
        case 5:
        {
            select(Parts, Database);
            break;
        }
        case 6:
        {
            Database = update(Parts, Database);
            break;
        }
        case 7:
        {
            Database = Delete(Parts, Database);
            break;
        }
        case 8:
            Database = alter(Parts, Database);
            break;
        case 9:
        {
            if (Parts.size() == 1)
            {
                flag = 1;
                break;
            }
            else
            {
                cout << "instruction error!" << endl;
                break;
            }
        }
        case 10:
        {
            Database = connect(Parts, Database);
            break;
        }
        case 11:
        {
            cout << "instruction error!" << endl;
            break;
        }
        }
        if (flag == 1)
            break;
    }
    return 0;
}
