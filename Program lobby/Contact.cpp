#define _CRT_SECURE_NO_WARNINGS 1
#include "Interface.h"

//�ж�ͨѶ¼���Ƿ��Ѿ��������
bool IsAdd(vector<IP*> user, long long stu_number) {
	for (IP* contact : user) {
		if (contact->stu_number == stu_number) {
			return true;  //�������Ӵ��ˣ��򷵻�true
		}
	}
	return false;
}

//����AddContact() �� SearchContact() �������໥��ӹ��ܣ����ǵ����-ɾ��-��Ӻ���η�ֹ�Է��û��ظ���ӣ�    
void InterAdd(long long stu_number, unordered_map<long long, IP> ID, IP info) {   
	//�ж�֮ǰ�Է��Ƿ��Ѿ���ӹ���¼�û�
	FILE* pfr = fopen(ID[stu_number].contact, "rb");   //ID[stu_number].contact ---> ���ʶԷ�ͨѶ¼
	IP* ret = (IP*)malloc(sizeof(IP));
	while (fread(ret, sizeof(IP), 1, pfr)) {
		if (ret->stu_number == info.stu_number) {
			fclose(pfr);
			return;
		}
	}
	fclose(pfr);
	//�����ظ������
	FILE* pf = fopen(ID[stu_number].contact, "ab");
	fwrite(&info, sizeof(IP), 1, pf);
	fclose(pf);
}

//bug��map����ַ��������ӵ�vector��ָ���ǳ������ᱻ�ݻ�
void AddContact(vector<IP*>& user, unordered_map<long long, IP>& ID, IP info) {    
	while (1) {
		long long stu_number;
		printf("ѧ��:>");
		scanf("%lld", &stu_number);
		//������������� ����ӡ� ����Լ��� ����û�������
		if (!ID.count(stu_number)) {
			printf("���޴��ˣ����������룡��\n");
			continue;
		}
		else if (stu_number == info.stu_number) {
			printf("�޷�����Լ������������룡��\n");
			continue;
		}
		else if (IsAdd(user, stu_number)) {
			printf("����Ӵ��ˣ�����������\n");
		}
		else {
			user.push_back(&ID[stu_number]);
			InterAdd(stu_number, ID, info);
			return;
		}
	}
}

//ɾ�������˻��е���ϵ�ˣ���ɾ���Է��û���ͨѶ¼���Լ�����Ϣ��
void DelectContact(vector<IP*>& user) {
	if (user.empty()) {
		printf("ͨѶ¼Ϊ�գ��޷�ɾ��\n");
		return;
	}
	char name[NAME_MAX];
	printf("��������Ҫɾ�������֣�>");
	scanf("%s", name);
	IP* search = nullptr;
	for (int i = 0; i < user.size(); i++) {
		if (!strcmp(name, user[i]->name)) {
			search = user[i];
			break;
		}
	}
	if (!search) return;   

	auto del = find(user.begin(), user.end(), search);    //find�������ص�������ָ��search�����eraseɾ��search
	if (del == user.end()) {
		printf("û����Ҫ���ҵ���\n");
		return;
	}
	user.erase(del);
	printf("ɾ���ɹ���\n");
	//ɾ����SaveContact() ����txt�ĵ�
}

//����ID����û�  --- ����true��ʾ���Һ���� �� ����false��ʾ���Һ�û�����
bool SearchContact(vector<IP*>& user, unordered_map<long long, IP>& ID, IP info) {
	printf("������ѧ�ţ�>");
	long long stu_number;
	scanf("%lld", &stu_number);
	if (!ID.count(stu_number)) {
		printf("���޴���!\n");
		return false;
	}
	else {
		//չʾ���û���Ϣ
		IP people = ID[stu_number];
		IMAGE search;
		loadimage(&search, "image_contact\\search.jpg");
		putimage(200, 200, &search);
		settextstyle(22, 0, "����");
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
					//��Ӵ���
					if (msg.x > 250 && msg.x < 400 && msg.y > 400 && msg.y < 450) {
						if (stu_number == info.stu_number) {
							HWND hWnd = GetHWnd();
							MessageBox(hWnd, "�޷�����Լ�", "! ! ! ! ! ! !", MB_OKCANCEL);
							continue;
						}
						else if (IsAdd(user, stu_number)) {    //�������в����Ƿ��Ѿ�����˸��û��� �������ӷ���true
							HWND hWnd = GetHWnd();
							MessageBox(hWnd, "����Ӹ��û�", "! ! ! ! ! ! !", MB_OKCANCEL);
							continue;
						}
						else {
							user.push_back(&ID[stu_number]); 
							InterAdd(stu_number, ID, info);
							return true;   //����ӷ���true, ����txt�ĵ�
						}
					}
					//����
					else if (msg.x > 500 && msg.x < 650 && msg.y > 400 && msg.y < 450) {
						return false;   //û��������豣��
					}
				}
			}
		}
	}
}

//ע��ѧ���˻�(��map�н���ѧ����Ϣ����)
void DelstudentsID(long long del) {
	FILE* pf1 = fopen("studentsID.txt", "rb");  //������   
	FILE* pf2 = fopen("del_tmp.txt", "wb");    //д����ʱ�ļ�
	
	long long* stu_number1 = (long long*)malloc(sizeof(long long));
	IP* ip1 = (IP*)malloc(sizeof(IP));
	//������Ҫɾ����ѧ������Ϣ��ȡ����ʱ�ļ�
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

	//�ٽ���ʱ�ļ�����Ϣ���ص�studentsID.txt�ļ��У��Դ���ɶ��ĵ�����Ϣɾ��
	while (fread(stu_number2, sizeof(long long), 1, pf4) && fread(ip2, sizeof(IP), 1, pf4)) {
		fwrite(stu_number2, sizeof(long long), 1, pf3);
		fwrite(ip2, sizeof(IP), 1, pf3);
	}
	fclose(pf3);
	fclose(pf4);
	printf("ע�����\n");

}

//����info.contact�ı��е����ݵ�user�У�����ID�м���Ƿ���ע�����û�
void LoadContact(vector<IP*>& user, IP info, unordered_map<long long, IP> ID) {
	//�����û���txt�ĵ�����û�б�ע�����˻�
	FILE* pf1 = fopen(info.contact, "rb");  //������   
	FILE* pf2 = fopen("del_tmp.txt", "wb");    //д����ʱ�ļ�

	IP* ip1 = (IP*)malloc(sizeof(IP));
	bool flag = false;  //�ж��Ƿ��б�ע�����˻�
	while (fread(ip1, sizeof(IP), 1, pf1)) {
		if (ID.count(ip1->stu_number)) {  //��map�д�����д����ʱ�ı�
			fwrite(ip1, sizeof(IP), 1, pf2);
		}
		else {
			flag = true;   //λ���û��ĵ�����Ϣ��map�в����ڣ� ��ʱflag��Ϊtrue�� ��ʾ��Ҫ����ʱ�ļ������ݵ�info.contact�У��ο�DelstudentsID()��
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

void SaveContact(vector<IP*>& user, IP info) {   //���������нڵ������д�뵽�ĵ���
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

//���������˻���Ϣ
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

//ע��һ���û�
void SaveStudentsID(long long* stu_number, IP* info) {
	FILE* pf = fopen("studentsID.txt", "ab");

	fwrite(stu_number, sizeof(long long), 1, pf);
	fwrite(info, sizeof(IP), 1, pf);

	fclose(pf);
	pf = NULL;
}

//չʾ�û���Ϣ�������Ƿ���չ������Ϣ
void DisplayContact(vector<IP*> user, int y, vector<bool> isunfold) {   //��isunfold���飬��¼ĳһ�±����Ϣ�Ƿ��ѱ�չ��
	settextcolor(RGB(40, 40, 40));
	setbkmode(TRANSPARENT);
	settextstyle(25, 15, "΢���ź�");

	int index = 0;
	for (IP* contact : user) {    //����user�����е�������ϵ��
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
			settextstyle(30, 0, "����");
			outtextxy(523, y + 11, contact->name);

			settextstyle(26, 0, "����");
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

//չʾ��¼�û�����Ϣ
void UserInfo(IP info) {
	settextcolor(RGB(40, 40, 40));
	setbkmode(TRANSPARENT);
	settextstyle(40, 0, "����");
	outtextxy(675, 12, info.name);
	settextstyle(17, 0, "����");
	char str_number[20] = { '\0' };
	sprintf(str_number, "%lld", info.stu_number);
	outtextxy(736, 105, str_number);
}

//��������������괫�뺯����ͨ����ͬ��չ������y��ȥ��Ӧ��ֵ����y < 0ʱ���ҵ�������û���λ�ã�������isunfold�����ֵ
void GetIndex(int n, vector<bool>& isunfold, int y) {
	int index = 0;
	for (int i = 0; i < n; i++) {
		if (isunfold[index] == false) {
			if (y - 50 <= 0) {
				isunfold[index] = true;
				return;
			}
			else y -= 50;  //δչ����ͼ�θ߶�Ϊ50
		}
		else if (isunfold[index] == true) {
			if (y - 150 <= 0) {
				isunfold[index] = false;
				return;
			}
			else y -= 150;   //��չ����ͼ�θ߶�Ϊ150
		}
		index++;
	}
}

//����ContactInter�����ж��ˢ�½���
void RefreshInterface(IMAGE avatar, IMAGE chatInter, IMAGE desktop, IP info) {
	cleardevice();
	putimage(0, 0, &desktop);
	putimage(500, 0, &chatInter);
	putimage(520, 10, &avatar);
	UserInfo(info);
}

//����
bool CompareName(IP* cont1, IP* cont2) {  
	return cont1->name < cont2->name;
}

void ContactInter(IP info, unordered_map<long long, IP> ID, long long* saveUser) {
	vector<IP*> user;
	LoadContact(user, info, ID);    
	
	IMAGE chatInter;  //����
	IMAGE desktop;    //����
	IMAGE avatar;     //ͷ��
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
	UserInfo(info);    //�û���Ϣ
	DisplayContact(user, 155, isunfold);  //��ϵ����Ϣ����140

	while (1) {
		if (MouseHit()) {
			MOUSEMSG msg = GetMouseMsg();
			switch (msg.uMsg) {
			case WM_LBUTTONDOWN: {
				//���
				if (msg.x > 906 && msg.x < 929 && msg.y >123 && msg.y < 145) {
					HWND hWnd = GetHWnd();
					MessageBox(hWnd, "������������������û���ѧ�ţ���","!", MB_OKCANCEL);
					AddContact(user, ID, info);   //��ӵ��û�������ID����ڵ��û� 
					//���桢ˢ��(����ͼƬ)��չʾ   ������Ϣ���Ҫ��������
					SaveContact(user, info); 
					RefreshInterface(avatar, chatInter, desktop, info);
					DisplayContact(user, 155, isunfold);
				}
				//ɾ��
				else if (msg.x > 939 && msg.x < 961 && msg.y > 123 && msg.y < 145) {
					HWND hWnd = GetHWnd();
					MessageBox(hWnd, "����������������Ҫɾ������", "! ! ! ! ! ! !", MB_OKCANCEL);
					DelectContact(user);    

					SaveContact(user, info);
					RefreshInterface(avatar, chatInter, desktop, info);
					DisplayContact(user, 155, isunfold);
				}
				//ͨ��GetIndex�ı�isunfold������Ӧλ�õ�ֵ����ͨ��DisplayContact����չʾ�����û�
				else if (msg.x > 508 && msg.x < 992 && msg.y > 155 && msg.y < 600) {
					GetIndex(user.size(),isunfold, msg.y - 155);
					RefreshInterface(avatar, chatInter, desktop, info);
					DisplayContact(user, 155, isunfold);
					
				}
				//���� -> չʾ���������û�����Ϣ -> ѡ���Ƿ����
				else if (msg.x > 972 && msg.x < 995 && msg.y > 123 && msg.y < 145) {
					HWND hWnd = GetHWnd();
					MessageBox(hWnd, "����������������Ҫ���ҵ���", "! ! ! ! ! ! !", MB_OKCANCEL);
					
					if (SearchContact(user, ID, info)) {   //����true��ʾ�����ϵ�ˣ���ʱ������ϵ�� 
						SaveContact(user, info);
					}
					RefreshInterface(avatar, chatInter, desktop, info);
					DisplayContact(user, 155, isunfold);
					
				}
				//����
				else if (msg.x > 873 && msg.x < 897 && msg.y > 123 && msg.y < 145) {
					sort(user.begin(), user.end(), CompareName);    //->421
					RefreshInterface(avatar, chatInter, desktop, info);
					DisplayContact(user, 155, isunfold);
				}
				//�˳���¼
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
				//�������棬 ���ֵ�¼
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