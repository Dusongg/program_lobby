#define _CRT_SECURE_NO_WARNINGS 1

#include "Interface.h"

//ɨ����Ϸ
void MinesweeperGame(int mode)
{
	settextcolor(RGB(199, 51, 57));
	setbkmode(TRANSPARENT);
	settextstyle(80, 50, "����");

	cleardevice();
	int keep_playing = 1;
	//keep_playing �����������ڸı䣬�ж��Ƿ����������
	while (keep_playing) {
		IMAGE wall;
		loadimage(&wall, "image_minesweeper\\wall.jpg");
		putimage(0, 0, &wall);

		//��ʾ�Ѷ�ͼ��
		char mode_str[30];
		sprintf(mode_str, "image_minesweeper\\(%d).jpg", mode);
		IMAGE MODE;
		loadimage(&MODE, mode_str);
		putimage(836, 477, &MODE);
		//����
		IMAGE board;
		loadimage(&board, "image_minesweeper\\board.jpg");
		putimage(100, 100, &board);
		//����
		IMAGE flag_board;
		loadimage(&flag_board, "image_minesweeper\\flag_board.jpg");
		putimage(700, 40, &flag_board);
		//��������
		char flag_str[5];
		sprintf(flag_str, "%d", mode);
		outtextxy(850, 40, flag_str);

		char inBoard[ROWS][COLS] = { '\0' };
		char outBoard[ROWS][COLS] = { '\0' };

		// --> Minesweeper.cpp
		InitBoard(inBoard, ROWS, COLS, '0');  //��ʼ������
		InitBoard(outBoard, ROWS, COLS, '*');  
		SetBoard(inBoard, ROW, COL, mode);    //���������
		StartToPlay(inBoard, outBoard, ROW, COL, mode, &keep_playing);
	}
}

//ɨ�׽���
void MinesweeperInter()
{
	srand((unsigned int)time(NULL));
	initgraph(1000, 600);
	HWND hnd = GetHWnd();
	SetWindowText(hnd, "---------------------Mineswepper_Interface---------------------");
	cleardevice();

	while (1) {
		IMAGE wall_old;
		loadimage(&wall_old, "image_minesweeper\\wall_old.jpg");
		putimage(0, 0, &wall_old);

		int mode = MODE_EAZY;
		//��¼�Ҽ���������������Ϣ��֮�󽫸�������Ϊԭ������ģʽ
		int modex = 0;
		int modey = 0;
		while (1) {
			if (MouseHit()) {
				MOUSEMSG msg = GetMouseMsg();
				switch (msg.uMsg) {
				case WM_LBUTTONDOWN: {
					if (modex != 0 && modey != 0) {   //���������Ϣ����ģʽ�����Ҽ����֮��
						if (msg.x > modex && msg.x < modex + 199 && msg.y > modey && msg.y < modey + 95) {
							mode = MODE_EAZY;
						}
						if (msg.x > modex && msg.x < modex + 199 && msg.y > modey + 95 && msg.y < modey + 180) {
							mode = MODE_MID;
						}
						if (msg.x > modex && msg.x < modex + 199 && msg.y > modey + 180 && msg.y < modey + 260) {
							mode = MODE_HARD;
						}
						cleardevice();
						//������ģʽ�����ɨ�׽��棬����ģʽͼƬ
						putimage(0, 0, &wall_old);
						modex = 0;
						modey = 0;
					}
					else if (msg.x > 250 && msg.x < 750 && msg.y > 120 && msg.y < 250) {
						MinesweeperGame(mode);    // ---> 6
						goto Inter;   //�������ѭ���� ���¼���ɨ�׽���
					}
					else if (msg.x > 250 && msg.x < 750 && msg.y > 350 && msg.y < 450) {
						return;  //ֱ���˳���������
					}
					break;
				}
				case WM_RBUTTONDOWN: {
					//�Ҽ�����ģʽ
					modex = msg.x;
					modey = msg.y;
					IMAGE MODE;
					loadimage(&MODE, "image_minesweeper\\mode.jpg");   //�ڵ�ǰ�������ģʽͼƬ
					putimage(modex, modey, &MODE);
					break;
				}
				}
			}
		}
	Inter:
		printf("ɨ�׽���\n");
	}
}

void MazegameInter() {
	while (1) {
		initgraph(800, 500);
		setbkcolor(RGB(209, 212, 193));
		HWND hnd = GetHWnd();
		SetWindowText(hnd, "---------------------Mazegame_Interface---------------------");
		cleardevice();

		IMAGE img;
		loadimage(&img, "image_mazegame\\��ͷ.jpg");
		putimage(50, 0, &img);

		while (1) {
			if (MouseHit()) {
				MOUSEMSG msg = GetMouseMsg();
				switch (msg.uMsg) {
				case WM_LBUTTONDOWN:
					if (msg.x > 50 && msg.x < 166 && msg.y > 384 && msg.y < 500) {
						return;
					}
					else {
						HWND hWnd = GetHWnd();
						MessageBox(hWnd, "С��bogerɲ��ס����������", "������ܰ��ʾ���� ", MB_OKCANCEL);
						MazeGame();    //-> Mazegame.cpp
						goto Inter;
					}
				}
			}
		}
	Inter:
		printf("ɨ�׽���\n");
	}
	getchar();
	closegraph();
}

void LoginScreen(long long* saveId) {
	srand((unsigned int)time(NULL));

	IMAGE logInter;
	loadimage(&logInter, "image_contact\\logInter.jpg");
	putimage(300, 200, &logInter);

	unordered_map<long long, IP> ID;
	LoadStudentsID(ID);

	//�˺��ѵ�¼��ֱ�Ӽ���ҳ��
	if (*saveId != 0) {
		ContactInter(ID[*saveId], ID, saveId);    //  -> Contact.cpp
		return;
	}

	while (1) {
		if (MouseHit()) {
			MOUSEMSG msg = GetMouseMsg();
			switch (msg.uMsg) {
			case WM_LBUTTONDOWN: 
				//ע��
				if (msg.x > 550 && msg.x < 650 && msg.y > 300 && msg.y < 350) {   
					HWND hWnd = GetHWnd();
					MessageBox(hWnd, "�������������������Ϣ", "! ", MB_OKCANCEL);

					while (1) {
						printf("ѧ��:> ");
						long long stu_number = 0;
						scanf("%lld", &stu_number);

						IP info;   //������¼���û�����Ϣ����ʱ����

						if (ID.count(stu_number)) {
							printf("��ѧ���ѱ�ע��, ����������!\n");
							continue;
						}
						printf("����:> "); scanf("%s", info.key);
						printf("����:> "); scanf("%s", info.name);
						printf("����:> "); scanf("%d", &(info.age));
						printf("�Ա�:> "); scanf("%s", info.sex);
						printf("��ַ:> "); scanf("%s", info.address);
						printf("�绰:> "); scanf("%s", info.tele);
						info.stu_number = stu_number;

						//����һ�����ͷ���ڸ�ע���û���
						char th = rand() % 10 + 97;
						sprintf(info.avatar, "avatarfile\\%c.jpg", th);
						//��ѧ������Ϊ�ļ���������char* ���ʹ����ڽṹ����
						sprintf(info.contact, "%lld.txt", stu_number);   

						FILE* pf = fopen(info.contact, "w");   //�ڵ�ǰĿ¼�´����Ը�ѧ��Ϊ����txt�ĵ�
						fclose(pf);

						ID[stu_number] = info;
						SaveStudentsID(&stu_number, &info);   //��ID�б�����û�

						HWND hWnd = GetHWnd();
						MessageBox(hWnd, "ע��ɹ������¼��", "! ", MB_OKCANCEL);
						break;
					}
				}
				//��¼
				if (msg.x > 350 && msg.x < 450 && msg.y > 300 && msg.y < 350) {   
					HWND hWnd = GetHWnd();
					MessageBox(hWnd, "�������������������Ϣ", "! ", MB_OKCANCEL);
					printf("ѧ��:> ");
					long long stu_number = 0;
					scanf("%lld", &stu_number);
					if (!ID.count(stu_number)) {
						printf("û�и�ѧ����Ϣ��\n");
						continue;
					}
					while (1) {
						printf("����:>");
						char ikey[20];
						memset(ikey, '\0', sizeof(ikey));
						scanf("%s", ikey);
						if (!strcmp(ID[stu_number].key, ikey)) {    //����ƥ��
							*saveId = stu_number;    //�����ʱ��¼���û�
							ContactInter(ID[stu_number], ID, saveId);   //ID[stu_number] ָ user ��ʾ��¼���û�����Ϣ
							return;  //��������
						}
						else {
							printf("key wrong!\n");
							continue;
						}
					}
				}
				//ע���˺�
				if (msg.x > 600 && msg.x < 660 && msg.y > 205 && msg.y < 229)
				{
					HWND hWnd = GetHWnd();
					MessageBox(hWnd, "����������������Ҫע�����˺�", "! ", MB_OKCANCEL);
					long long del;
					printf("��������Ҫע����ѧ�ţ�>");
					scanf("%lld", &del);
					if (!ID.count(del)) {
						printf("û�и�ѧ����Ϣ\n");
						break;
					}
					DelstudentsID(del);
					LoadStudentsID(ID);   //ע��֮�����¼���ID������
					return;
				}
				//�˳�����> ��������
				if (msg.x > 670 && msg.x < 693 && msg.y > 205 && msg.y < 229)  return;    
			}
		}
	}
}

int main() {
	long long saveUser = 0;    //�������ڵ�¼���û���ѧ��

	initgraph(1000, 600, SHOWCONSOLE);
	while (1) {
		HWND hnd = GetHWnd();
		SetWindowText(hnd, "---------------------Desktop---------------------");
		IMAGE desktop;
		loadimage(&desktop, "desktop.jpg");
		putimage(0, 0, &desktop);

		while (1) {
			if (MouseHit()) {
				MOUSEMSG msg = GetMouseMsg();
				switch (msg.uMsg) {
				case WM_LBUTTONDOWN:
					if (msg.x > 0 && msg.x < 100 && msg.y > 0 && msg.y < 100) {
						MinesweeperInter();   //����ɨ�׽���   -> 51
						goto Desktop;
					}
					if (msg.x > 110 && msg.x < 210 && msg.y > 0 && msg.y < 100) {
						MazegameInter();    //�����Թ�����     -> 115
						initgraph(1000, 600, SHOWCONSOLE);
						goto Desktop;
					}
					if (msg.x > 900 && msg.x < 1000 && msg.y > 0 && msg.y < 100) {
						LoginScreen(&saveUser);    //�����¼����   -> 151
						goto Desktop;
					}
				}
			}
		}
	Desktop:
		printf("go_to_Desktop\n");    //����֮ǰ��ͼ��
	}
	return 0;
}