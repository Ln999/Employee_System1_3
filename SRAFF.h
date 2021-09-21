#pragma once
#include<iostream>
#include<string>
#include<cstdio>
#include<Windows.h>
#include<sqlite3.h>


extern const char* fileName;
extern char* UtoG(const char*);
extern char* GtoU(const char*);
extern char* LastRowSnum;


extern inline std::string GetDate();

extern std::string StaffNum();

extern int sql_callback(void*,int,char**,char**);

extern void Menu();


class Staff {
private:
	sqlite3* sdb;
	sqlite3_stmt* Sstmt;
	char** errmsg;
public:
	Staff();
	~Staff();
	void SelectOption(int);
	void AddStaff();
	void DelStaff();
	void ShowStaff();
	void ModStaff();
	void FindStaff();
};
