#include<iostream>
#include<string>
#include<sqlite3.h>
#include<ctime>
#include<Windows.h>
#include<cstdio>

using namespace std;

sqlite3* db = NULL;
const char* fileName = "职工信息.db";

//GBK转UTF8
char* GtoU(const char*);
//UTF-8转GBK
char* UtoG(const char*);

void CreateTable();


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

void CreateTable() {
	//连接数据库
	int res = sqlite3_open_v2(GtoU(fileName), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if (res == SQLITE_OK) {
		cout << "数据库连接成功..." << endl;
	}
	else {
		cout << "数据库连接失败--" << sqlite3_errcode(db);
		exit(0);
	}
	sqlite3_exec(db, "pragma foreign_keys=on;", NULL, NULL, NULL);
	//创建Person表语句
	char* sql = "create table if not exists Person(" \
		"Pid integer primary key autoincrement," \
		"Pname nvarchar[8] unique not null," \
		"Psex nvarchar[4] not null, " \
		"Page int not null);";
	//执行sql语句
	res = sqlite3_exec(db, sql, NULL, NULL, NULL);
	if (res == SQLITE_OK) {
		cout << "Person表创建成功..." << endl;
	}
	else {
		cout << "Person表创建失败:" << sqlite3_errcode(db);
		exit(0);
	}
	//创建Staff表语句
	sql = "create table if not exists Staff(" \
		"Sid integer primary key autoincrement," \
		"Snum text unique not null," \
		"SPid int unique not null," \
		"Sdept text not null," \
		"Spos text not null," \
		"foreign key(SPid) references Person(Pid)"
		"on delete cascade on update cascade);";
	//执行sql语句
	res = sqlite3_exec(db, sql, NULL, NULL, NULL);
	if (res == SQLITE_OK) {
		cout << "Staff表创建成功..." << endl;
	}
	else {
		cout << "Staff表创建失败:" << sqlite3_errcode(db);
		exit(0);
	}
	sqlite3_close_v2(db);
}

int main() {
	CreateTable();
	return 0;
}
