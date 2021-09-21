#include"SRAFF.h"


Staff::Staff()
{
	this->sdb = NULL;
	this->errmsg =NULL;
	this->Sstmt = NULL;
	if (!sqlite3_open_v2(GtoU(fileName), &sdb, SQLITE_OPEN_READWRITE, NULL)) {
		std::cout <<fileName<< "���ӳɹ�" << std::endl;
	}
	int input;
	while (true)
	{
		Menu();
		std::cout << "ְ����Ϣ����ϵͳ >";
		std::cin >> input;
		SelectOption(input);
	}
	sqlite3_close_v2(sdb);
}

Staff::~Staff() {};
void Staff::SelectOption(int input)
{
	switch (input)
	{
	case 1:
		AddStaff();
		system("pause");
		system("cls");
		break;
	case 2:
		DelStaff();
		system("pause");
		system("cls");
		break;
	case 3:
		ModStaff();
		system("pause");
		system("cls");
		break;
	case 4:
		FindStaff();
		system("pause");
		system("cls");
		break;
	case 5:
		ShowStaff();
		system("pause");
		system("cls");
		break;
	case 0:
		exit(0);
	default:
		std::cout << "�޷�ʶ�������:" << input << std::endl;
		break;
	}
}

void Staff::AddStaff()
{
	std::string name;
	std::string dept;
	std::string sex;
	std::string pos;
	int age;
	int num=0;
	std::cout << "Ҫ��ӵ�ְ������:" << ' ';
	std::cin >> num;
	for (int i = 0; i < num; i++) {
		std::cout << "������ְ����Ϣ:" << std::endl;
		std::cout << "����:" << '\t' << "�Ա�:" << '\t' << "����:" << '\t' << "����:" << '\t' << "ְλ:" << std::endl;
		std::cin >> name >> sex >> age >> dept >> pos;

		if (sqlite3_exec(sdb, "select Max(rowid),Snum from Staff;", sql_callback, LastRowSnum, errmsg)) {
			std::cout << sqlite3_errcode(sdb);
		}
		std::cout << "LastRowSnum=" << LastRowSnum << std::endl;

		sqlite3_prepare(sdb, "insert into Staff(Snum,Sname,Ssex,Sage,Sdept,Spos) values(?,?,?,?,?,?)", -1, &Sstmt, NULL);
		sqlite3_bind_text(Sstmt, 1, GtoU(StaffNum().c_str()), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(Sstmt, 2, GtoU(name.c_str()), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(Sstmt, 3, GtoU(sex.c_str()), -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(Sstmt, 4, age);
		sqlite3_bind_text(Sstmt, 5, GtoU(dept.c_str()), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(Sstmt, 6, GtoU(pos.c_str()), -1, SQLITE_TRANSIENT);
		
		sqlite3_step(Sstmt);
		sqlite3_reset(Sstmt);
		std::cout << "��ӳɹ�" << std::endl;
	}
	sqlite3_finalize(Sstmt);
}

void Staff::DelStaff()
{
	//���жϱ��Ƿ��������
	std::string name;
	int count=0;
	std::cout << "Ҫɾ����Ա������:" << ' ';
	std::cin >> count;
	for (int i = 0; i < count; i++) {
		std::cout << "Ա������:" << ' ';
		std::cin >> name;
		sqlite3_prepare(sdb, "delete from Staff where Sname=?;", -1, &Sstmt, NULL);
		sqlite3_bind_text(Sstmt, 1, GtoU(name.c_str()), -1, SQLITE_TRANSIENT);
		sqlite3_step(Sstmt);
		sqlite3_reset(Sstmt);
		std::cout << "ɾ���ɹ�" << std::endl;
	}
	sqlite3_finalize(Sstmt);
	sqlite3_exec(sdb, "update from Staff set Sid=Max(rowid)+1;", NULL, NULL, NULL);
}

void Staff::ShowStaff()
{
	sqlite3_exec(sdb, ".headers on", NULL, NULL, NULL);
	sqlite3_exec(sdb, ".mode column", sql_callback, NULL, NULL);
	sqlite3_exec(sdb, "select*from Staff;", sql_callback, NULL, NULL);
}

void Staff::ModStaff()
{
	int count = 0;
	std::string Sname;
	std::string name;
	std::string sex;
	int age;
	std::string dept;
	std::string pos;
	std::cout << "Ҫ�޸ĵ�Ա������:" << ' ';
	std::cin >> count;
	for (int i = 0; i < count; i++) {
		std::cout << "Ҫ�޸���Ϣ��Ա������:" << ' ';
		std::cin >> Sname;
		std::cout << "Ա���µ���Ϣ:" << std::endl;
		std::cout << "����:" << '\t' << "�Ա�:" << '\t' << "����:" << '\t' << "����:" << '\t' << "ְλ:" << std::endl;
		std::cin >> name >> sex >> age >> dept >> pos;
		char* sql = "update Staff s set s.Sname=?,s.Ssex=?,s.Sage=?,s.Sdept=?,s.Spos=? where Sname=?";
		sqlite3_prepare(sdb,sql, -1, &Sstmt, NULL);
		sqlite3_bind_text(Sstmt, 1, GtoU(name.data()), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(Sstmt, 2, GtoU(sex.c_str()), -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(Sstmt, 3, age);
		sqlite3_bind_text(Sstmt, 4, GtoU(dept.c_str()), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(Sstmt, 5, GtoU(pos.c_str()), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(Sstmt, 6, GtoU(Sname.c_str()), -1, SQLITE_TRANSIENT);
	}
}

void Staff::FindStaff()
{
	std::string name;
	std::cout << "Ҫ���ҵ�Ա������:" << ' ';
	std::cin >> name;
	sqlite3_prepare(sdb, "select Sname from Staff where Sname=?", -1, &Sstmt, NULL);
	sqlite3_bind_text(Sstmt, 1, GtoU(name.c_str()), -1, SQLITE_TRANSIENT);
	sqlite3_step(Sstmt);
	sqlite3_reset(Sstmt);
	sqlite3_finalize(Sstmt);
}

