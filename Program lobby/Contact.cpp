#define _CRT_SECURE_NO_WARNINGS 1
#include "Interface.h"

//判断通讯录中是否已经有这个人
bool IsAdd(vector<IP*> user, long long stu_number) {
	for (IP* contact : user) {
		if (contact->stu_number == stu_number) {
			return true;  //如果已添加此人，则返回true
		}
	}
	return false;
}

//用于AddContact() 与 SearchContact() 函数的相互添加功能（考虑当添加-删除-添加后，如何防止对方用户重复添加）    
void InterAdd(long long stu_number, unordered_map<long long, IP> ID, IP info) {   
	//判断之前对方是否已经添加过登录用户
	FILE* pfr = fopen(ID[stu_number].contact, "rb");   //ID[stu_number].contact ---> 访问对方通讯录
	IP* ret = (IP*)malloc(sizeof(IP));
	while (fread(ret, sizeof(IP), 1, pfr)) {
		if (ret->stu_number == info.stu_number) {
			fclose(pfr);
			return;
		}
	}
	fclose(pfr);
	//若无重复则添加
	FILE* pf = fopen(ID[stu_number].contact, "ab");
	fwrite(&info, sizeof(IP), 1, pf);
	fclose(pf);
}

//bug：map传地址，否则添加到vector的指针是出函数会被摧毁
void AddContact(vector<IP*>& user, unordered_map<long long, IP>& ID, IP info) {    
	while (1) {
		long long stu_number;
		printf("学号:>");
		scanf("%lld", &stu_number);
		//考虑三种情况： 已添加、 添加自己、 添加用户不存在
		if (!ID.count(stu_number)) {
			printf("查无此人，请重新输入！！\n");
			continue;
		}
		else if (stu_number == info.stu_number) {
			printf("无法添加自己，请重新输入！！\n");
			continue;
		}
		else if (IsAdd(user, stu_number)) {
			printf("已添加此人，请重新输入\n");
		}
		else {
			user.push_back(&ID[stu_number]);
			InterAdd(stu_number, ID, info);
			return;
		}
	}
}

//删除个人账户中的联系人（不删除对方用户的通讯录中自己的信息）
void DelectContact(vector<IP*>& user) {
	if (user.empty()) {
		printf("通讯录为空，无法删除\n");
		return;
	}
	char name[NAME_MAX];
	printf("请输入你要删除的名字：>");
	scanf("%s", name);
	IP* search = nullptr;
	for (int i = 0; i < user.size(); i++) {
		if (!strcmp(name, user[i]->name)) {
			search = user[i];
			break;
		}
	}
	if (!search) return;   

	auto del = find(user.begin(), user.end(), search);    //find函数返回迭代器，指向search，配合erase删除search
	if (del == user.end()) {
		printf("没有你要查找的人\n");
		return;
	}
	user.erase(del);
	printf("删除成功！\n");
	//删除后SaveContact() 保存txt文档
}

//查找ID里的用户  --- 返回true表示查找后添加 ， 返回false表示查找后没有添加
bool SearchContact(vector<IP*>& user, unordered_map<long long, IP>& ID, IP info) {
	printf("请输入学号：>");
	long long stu_number;
	scanf("%lld", &stu_number);
	if (!ID.count(stu_number)) {
		printf("查无此人!\n");
		return false;
	}
	else {
		//展示该用户信息
		IP people = ID[stu_number];
		IMAGE search;
		loadimage(&search, "image_contact\\search.jpg");
		putimage(200, 200, &search);
		settextstyle(22, 0, "黑体");
		outtextxy(278, 224, people.name);

		char str_number[20] = { '\0' };
		sprintf(str_number, "%lld", people.stu_number);
		outtextxy(486, 224, str_number);

		char str_age[20] = { '\0' };
		sprintf(str_age, "%d", people.age);
		outtextxy(278, 290, str_age);

		outtextxy(500, 290, people.sex);
		outtextxy(278, 354, people.tele);
		outtextxy(544, 354, people.address);

		while (1) {
			if (MouseHit()) {
				MOUSEMSG msg = GetMouseMsg();
				switch (msg.uMsg) {
				case WM_LBUTTONDOWN: 
					//添加此人
					if (msg.x > 250 && msg.x < 400 && msg.y > 400 && msg.y < 450) {
						if (stu_number == info.stu_number) {
							HWND hWnd = GetHWnd();
							MessageBox(hWnd, "无法添加自己", "! ! ! ! ! ! !", MB_OKCANCEL);
							continue;
						}
						else if (IsAdd(user, stu_number)) {    //在链表中查找是否已经添加了该用户， 如果已添加返回true
							HWND hWnd = GetHWnd();
							MessageBox(hWnd, "已添加该用户", "! ! ! ! ! ! !", MB_OKCANCEL);
							continue;
						}
						else {
							user.push_back(&ID[stu_number]); 
							InterAdd(stu_number, ID, info);
							return true;   //若添加返回true, 保存txt文档
						}
					}
					//返回
					else if (msg.x > 500 && msg.x < 650 && msg.y > 400 && msg.y < 450) {
						return false;   //没添加则无需保存
					}
				}
			}
		}
	}
}

//注销学生账户(从map中将该学生信息销毁)
void DelstudentsID(long long del) {
	FILE* pf1 = fopen("studentsID.txt", "rb");  //读数据   
	FILE* pf2 = fopen("del_tmp.txt", "wb");    //写入临时文件
	
	long long* stu_number1 = (long long*)malloc(sizeof(long long));
	IP* ip1 = (IP*)malloc(sizeof(IP));
	//将除开要删除的学生的信息读取到临时文件
	while (fread(stu_number1, sizeof(long long), 1, pf1) && fread(ip1, sizeof(IP), 1, pf1)) {	
		if (*stu_number1 != del) {
			fwrite(stu_number1, sizeof(long long), 1, pf2);
			fwrite(ip1, sizeof(IP), 1, pf2);
		}
	}
	fclose(pf1);
	fclose(pf2);

	FILE* pf3 = fopen("studentsID.txt", "wb");
	FILE* pf4 = fopen("del_tmp.txt", "rb");
	long long* stu_number2 = (long long*)malloc(sizeof(long long));
	IP* ip2 = (IP*)malloc(sizeof(IP));

	//再将临时文件的信息读回到studentsID.txt文件中，以此完成对文档的信息删除
	while (fread(stu_number2, sizeof(long long), 1, pf4) && fread(ip2, sizeof(IP), 1, pf4)) {
		fwrite(stu_number2, sizeof(long long), 1, pf3);
		fwrite(ip2, sizeof(IP), 1, pf3);
	}
	fclose(pf3);
	fclose(pf4);
	printf("注销完成\n");

}

//加载info.contact文本中的数据到user中，并在ID中检查是否有注销的用户
void LoadContact(vector<IP*>& user, IP info, unordered_map<long long, IP> ID) {
	//检查该用户的txt文档中有没有被注销的账户
	FILE* pf1 = fopen(info.contact, "rb");  //读数据   
	FILE* pf2 = fopen("del_tmp.txt", "wb");    //写入临时文件

	IP* ip1 = (IP*)malloc(sizeof(IP));
	bool flag = false;  //判断是否有被注销的账户
	while (fread(ip1, sizeof(IP), 1, pf1)) {
		if (ID.count(ip1->stu_number)) {  //在map中存在则写入临时文本
			fwrite(ip1, sizeof(IP), 1, pf2);
		}
		else {
			flag = true;   //位于用户文档的信息在map中不存在， 此时flag置为true， 表示需要对临时文件读数据到info.contact中（参考DelstudentsID()）
		}
	}
	fclose(pf1);
	fclose(pf2);

	if (flag) {
		FILE* pf3 = fopen(info.contact, "wb");
		FILE* pf4 = fopen("del_tmp.txt", "rb");
		IP* ip2 = (IP*)malloc(sizeof(IP));
		while (fread(ip2, sizeof(IP), 1, pf4)) {
			fwrite(ip2, sizeof(IP), 1, pf3);
		}
		fclose(pf3);
		fclose(pf4);
	}

	FILE* pf = fopen(info.contact, "rb");
	int n = 0;
	IP* tmp = (IP*)malloc(sizeof(IP));
	while (fread(tmp, sizeof(IP), 1, pf)) {
		n++;
	}
	rewind(pf);
	while (n--) {
		IP* tmp = (IP*)malloc(sizeof(IP));
		fread(tmp, sizeof(IP), 1, pf);
		user.push_back(tmp);
	}
	fclose(pf);
	pf = NULL;
}

void SaveContact(vector<IP*>& user, IP info) {   //将数组所有节点的数据写入到文档中
	FILE* pf = fopen(info.contact, "wb");
	if (pf == NULL) {
		exit(EXIT_FAILURE);
		return;
	}
	for (int i = 0; i < user.size(); i++) {
		fwrite(user[i], sizeof(IP), 1, pf);
	}
	fclose(pf);
	pf = NULL;
}

//加载所有账户信息
void LoadStudentsID(unordered_map<long long, IP>& ID) {
	FILE* pf = fopen("studentsID.txt", "rb");

	int n = 0;
	long long* tmp1 = (long long*)malloc(sizeof(long long));
	IP* tmp2 = (IP*)malloc(sizeof(IP));
	while (fread(tmp1, sizeof(long long), 1, pf) && fread(tmp2, sizeof(IP), 1, pf)) {
		n++;
	}
	rewind(pf);
	while (n--) {
		long long* stu_number = (long long*)malloc(sizeof(long long));
		IP* ip = (IP*)malloc(sizeof(IP));
		fread(stu_number, sizeof(long long), 1, pf);
		fread(ip, sizeof(IP), 1, pf);
		ID[*stu_number] = *ip;
	}
	fclose(pf);
	pf = NULL;
}

//注册一个用户
void SaveStudentsID(long long* stu_number, IP* info) {
	FILE* pf = fopen("studentsID.txt", "ab");

	fwrite(stu_number, sizeof(long long), 1, pf);
	fwrite(info, sizeof(IP), 1, pf);

	fclose(pf);
	pf = NULL;
}

//展示用户信息，考虑是否有展开的信息
void DisplayContact(vector<IP*> user, int y, vector<bool> isunfold) {   //用isunfold数组，记录某一下标的信息是否已被展开
	settextcolor(RGB(40, 40, 40));
	setbkmode(TRANSPARENT);
	settextstyle(25, 15, "微软雅黑");

	int index = 0;
	for (IP* contact : user) {    //遍历user数组中的所有联系人
		if (isunfold[index] == false) {   
			IMAGE doc;
			loadimage(&doc, "image_contact\\doc.jpg");
			putimage(508, y, &doc);
			outtextxy(528, (y + 5), contact->name);
			y += 50;
		}
		else if (isunfold[index] == true) {
			IMAGE Unfold;
			loadimage(&Unfold, "image_contact\\Unfold.jpg");
			putimage(500, y, &Unfold);
			settextstyle(30, 0, "黑体");
			outtextxy(523, y + 11, contact->name);

			settextstyle(26, 0, "黑体");
			char str_number[20] = { '\0' };
			sprintf(str_number, "%lld", contact->stu_number);
			outtextxy(795, y + 11, str_number);

			char str_age[20] = { '\0' };
			sprintf(str_age, "%d", contact->age);
			outtextxy(582, y + 58, str_age);

			outtextxy(795, y + 58, contact->sex);
			outtextxy(582, y + 105, contact->tele);
			outtextxy(845, y + 105, contact->address);
			y += 150;
		}
		index++;
	}
}

//展示登录用户的信息
void UserInfo(IP info) {
	settextcolor(RGB(40, 40, 40));
	setbkmode(TRANSPARENT);
	settextstyle(40, 0, "黑体");
	outtextxy(675, 12, info.name);
	settextstyle(17, 0, "黑体");
	char str_number[20] = { '\0' };
	sprintf(str_number, "%lld", info.stu_number);
	outtextxy(736, 105, str_number);
}

//将鼠标点击的纵坐标传入函数，通过不同的展开与否对y减去相应的值，当y < 0时即找到点击的用户栏位置，并更改isunfold数组的值
void GetIndex(int n, vector<bool>& isunfold, int y) {
	int index = 0;
	for (int i = 0; i < n; i++) {
		if (isunfold[index] == false) {
			if (y - 50 <= 0) {
				isunfold[index] = true;
				return;
			}
			else y -= 50;  //未展开的图形高度为50
		}
		else if (isunfold[index] == true) {
			if (y - 150 <= 0) {
				isunfold[index] = false;
				return;
			}
			else y -= 150;   //已展开的图形高度为150
		}
		index++;
	}
}

//用于ContactInter函数中多次刷新界面
void RefreshInterface(IMAGE avatar, IMAGE chatInter, IMAGE desktop, IP info) {
	cleardevice();
	putimage(0, 0, &desktop);
	putimage(500, 0, &chatInter);
	putimage(520, 10, &avatar);
	UserInfo(info);
}

//重载
bool CompareName(IP* cont1, IP* cont2) {  
	return cont1->name < cont2->name;
}

void ContactInter(IP info, unordered_map<long long, IP> ID, long long* saveUser) {
	vector<IP*> user;
	LoadContact(user, info, ID);    
	
	IMAGE chatInter;  //界面
	IMAGE desktop;    //桌面
	IMAGE avatar;     //头像
	loadimage(&desktop, "desktop.jpg");
	loadimage(&chatInter, "image_contact\\chatInter.jpg");
	loadimage(&avatar, info.avatar, 130, 130);

	int enter = 1000; 
	while (enter >= 500) {
		BeginBatchDraw();
		cleardevice();
		putimage(0, 0, &desktop);
		putimage(enter, 0, &chatInter);
		putimage(enter + 20, 10, &avatar);
		EndBatchDraw();
		enter -= 10;
		Sleep(2);
	}

	vector<bool> isunfold(100, false);
	UserInfo(info);    //用户信息
	DisplayContact(user, 155, isunfold);  //联系人信息——140

	while (1) {
		if (MouseHit()) {
			MOUSEMSG msg = GetMouseMsg();
			switch (msg.uMsg) {
			case WM_LBUTTONDOWN: {
				//添加
				if (msg.x > 906 && msg.x < 929 && msg.y >123 && msg.y < 145) {
					HWND hWnd = GetHWnd();
					MessageBox(hWnd, "请在命令行输入添加用户的学号！！","!", MB_OKCANCEL);
					AddContact(user, ID, info);   //添加的用户必须是ID里存在的用户 
					//保存、刷新(覆盖图片)、展示   更改信息后必要的三步骤
					SaveContact(user, info); 
					RefreshInterface(avatar, chatInter, desktop, info);
					DisplayContact(user, 155, isunfold);
				}
				//删除
				else if (msg.x > 939 && msg.x < 961 && msg.y > 123 && msg.y < 145) {
					HWND hWnd = GetHWnd();
					MessageBox(hWnd, "请在命令行输入你要删除的人", "! ! ! ! ! ! !", MB_OKCANCEL);
					DelectContact(user);    

					SaveContact(user, info);
					RefreshInterface(avatar, chatInter, desktop, info);
					DisplayContact(user, 155, isunfold);
				}
				//通过GetIndex改变isunfold数组相应位置的值，再通过DisplayContact函数展示所有用户
				else if (msg.x > 508 && msg.x < 992 && msg.y > 155 && msg.y < 600) {
					GetIndex(user.size(),isunfold, msg.y - 155);
					RefreshInterface(avatar, chatInter, desktop, info);
					DisplayContact(user, 155, isunfold);
					
				}
				//查找 -> 展示搜索出的用户的信息 -> 选择是否添加
				else if (msg.x > 972 && msg.x < 995 && msg.y > 123 && msg.y < 145) {
					HWND hWnd = GetHWnd();
					MessageBox(hWnd, "请在命令行输入你要查找的人", "! ! ! ! ! ! !", MB_OKCANCEL);
					
					if (SearchContact(user, ID, info)) {   //返回true表示添加联系人，此时保存联系人 
						SaveContact(user, info);
					}
					RefreshInterface(avatar, chatInter, desktop, info);
					DisplayContact(user, 155, isunfold);
					
				}
				//排序
				else if (msg.x > 873 && msg.x < 897 && msg.y > 123 && msg.y < 145) {
					sort(user.begin(), user.end(), CompareName);    //->421
					RefreshInterface(avatar, chatInter, desktop, info);
					DisplayContact(user, 155, isunfold);
				}
				//退出登录
				else if (msg.x > 872 && msg.x < 995 && msg.y > 4.6 && msg.y < 24.6) {
					*saveUser = 0;
					int exit = 500;
					while (exit <= 1000) {
						BeginBatchDraw();
						cleardevice();
						putimage(0, 0, &desktop);
						putimage(exit, 0, &chatInter);
						EndBatchDraw();
						exit += 10;
						Sleep(2);
					}
					return;
				}
				//返回桌面， 保持登录
				else if (msg.x > 0 && msg.x < 500 && msg.y > 0 && msg.y < 600) {
					int exit = 500;
					while (exit <= 1000) {
						BeginBatchDraw();
						cleardevice();
						putimage(0, 0, &desktop);
						putimage(exit, 0, &chatInter);
						EndBatchDraw();
						exit += 10;
						Sleep(2);
					}
					return;
				}
			}
			}
		}
	}
}