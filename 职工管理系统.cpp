/*
项目配置:
	1,包含sqlite3头文件路径,项目属性->C/C++->常规->附加包含目录
	2,包含sqlite3库文件路径,项目属性->链接器->常规->附加库目录
	3,添加sqlite3库文件路径,项目属性->链接器->输入->附加依赖项
	4,加载动态库文件,项目属性->生成事件->生成后事件->命令行->copy+.dll文件路径
	5,项目属性->C/C++->语言->符合模式->否

*/
#pragma warning(disable:4996)
#include<cstdio>
#include<Windows.h>
#include"SRAFF.h"

using namespace std;

sqlite3* db;
const char* fileName="职工信息.db";
char* LastRowSnum="";

void Menu();

void CreateTable();

char* UtoG(const char*);

char* GtoU(const char*);

inline string GetDate();

string StaffNum();

int sql_callback(void*, int, char**, char**);

//GBK转UTF-8
char* GtoU(const char* gbk)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gbk, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gbk, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);

	if (wstr) delete[] wstr;
	return str;
};

//UTF-8转GBK
char* UtoG(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

inline string GetDate() {
	char ch[12];
	time_t Time = time(NULL);
	strftime(ch, sizeof(ch) - 1, "%Y%m%d", localtime(&Time));
	return ch;
}

string StaffNum() {
	static int i = 0;
	string str;
	//不存在职工编号或存在职工编号不是同一天就自动生成编号
	if (LastRowSnum == "" || strcmp(LastRowSnum, GetDate().c_str()) < 0) {
		i += 1;
		cout << "i=" << i << endl;
		if (i < 10) {
			str = GetDate() + "00" + to_string(i);
		}
		else if (i >= 10 && i < 100) {
			str = GetDate() + "0" + to_string(i);
		}
		else if (i >= 100 && i < 1000) {
			str = GetDate() + to_string(i);
		}
		else {
			cout << "太多了" << endl;
			exit(0);
		}
		cout << "str=" << str << endl;
		return str;
	}
	else {
		if (LastRowSnum != NULL) {
			str = LastRowSnum;
			string strHead;
			string strTail;
			strHead.append(str, 0, str.size() - 3);
			strTail.append(str, str.size() - 3, str.size());
			int num = stoi(strTail);
			cout << "日期号:" << strHead << endl;
			cout << "尾号:" << strTail << endl;
			num += 1;
			if (num < 10) {
				str = strHead + "00" + to_string(num);
			}
			else if (num >= 10 && num < 100) {
				str = strHead + "0" + to_string(num);
			}
			else if (num >= 100 && num < 1000) {
				str = strHead + to_string(num);
			}
			else {
				cout << "太多了" << endl;
			}
			return str;
		}
	}
}

int sql_callback(void* Data, int Record, char** ColumnVal, char** errmsg)
{
	if (*ColumnVal == NULL) {
		return 0;
	}
	else {
		LastRowSnum = (char*)Data;
		for (int i = 0; i < Record; i++) {
			LastRowSnum = UtoG(ColumnVal[i]);
			//cout << "LastRowSnum=" << LastRowSnum << endl;
			cout << UtoG(ColumnVal[i])<<' ';
		}
		cout << endl;
		return 0;
	}
}

void Menu()
{
	std::cout << "*****	     欢迎使用职工信息管理系统		*****" << std::endl;
	std::cout << std::endl;
	std::cout << "*****		1,添加员工信息			*****" << std::endl;
	std::cout << "*****		2,删除员工信息			*****" << std::endl;
	std::cout << "*****		3,修改员工信息			*****" << std::endl;
	std::cout << "*****		4,查找员工信息			*****" << std::endl;
	std::cout << "*****		5,显示员工信息			*****" << std::endl;
	//std::cout << "*****		6,清空屏幕信息			*****" << std::endl;
	std::cout << "*****		0,退出管理系统			*****" << std::endl;
}

void CreateTable()
{
	int res = sqlite3_open_v2(GtoU(fileName), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if (!res) {
		cout << "数据库连接成功..." << endl;
	}
	else {
		cout << "数据库连接失败:" << sqlite3_errcode(db);
		exit(0);
	}
	//sql语句
	char* sql = "create table if not exists Staff(" \
		"Sid integer primary key autoincrement," \
		"Snum text unique not null," \
		"Sname nvarchar[8] unique not null," \
		"Ssex nvarchar[4] not null," \
		"Sage int not null ," \
		"Sdept text not null," \
		"Spos text not null);";
	if (!sqlite3_exec(db, sql, NULL, NULL, NULL)) {
		cout << "Staff数据库表创建成功" << endl;
	}
	else {
		cout << "数据库表创建失败:" << sqlite3_errcode(db);
	}
	sqlite3_close_v2(db);
}


int main() {
	CreateTable();
	Staff stf;
	return 0;
}
