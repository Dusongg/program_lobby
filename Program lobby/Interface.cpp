#define _CRT_SECURE_NO_WARNINGS 1

#include "Interface.h"

//扫雷游戏
void MinesweeperGame(int mode)
{
	settextcolor(RGB(199, 51, 57));
	setbkmode(TRANSPARENT);
	settextstyle(80, 50, "楷体");

	cleardevice();
	int keep_playing = 1;
	//keep_playing 在其他函数内改变，判断是否继续重新玩
	while (keep_playing) {
		IMAGE wall;
		loadimage(&wall, "image_minesweeper\\wall.jpg");
		putimage(0, 0, &wall);

		//显示难度图像
		char mode_str[30];
		sprintf(mode_str, "image_minesweeper\\(%d).jpg", mode);
		IMAGE MODE;
		loadimage(&MODE, mode_str);
		putimage(836, 477, &MODE);
		//棋盘
		IMAGE board;
		loadimage(&board, "image_minesweeper\\board.jpg");
		putimage(100, 100, &board);
		//红旗
		IMAGE flag_board;
		loadimage(&flag_board, "image_minesweeper\\flag_board.jpg");
		putimage(700, 40, &flag_board);
		//红旗数量
		char flag_str[5];
		sprintf(flag_str, "%d", mode);
		outtextxy(850, 40, flag_str);

		char inBoard[ROWS][COLS] = { '\0' };
		char outBoard[ROWS][COLS] = { '\0' };

		// --> Minesweeper.cpp
		InitBoard(inBoard, ROWS, COLS, '0');  //初始化棋盘
		InitBoard(outBoard, ROWS, COLS, '*');  
		SetBoard(inBoard, ROW, COL, mode);    //随机放置雷
		StartToPlay(inBoard, outBoard, ROW, COL, mode, &keep_playing);
	}
}

//扫雷界面
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
		//记录右键点击后接受坐标信息，之后将该坐标视为原点设置模式
		int modex = 0;
		int modey = 0;
		while (1) {
			if (MouseHit()) {
				MOUSEMSG msg = GetMouseMsg();
				switch (msg.uMsg) {
				case WM_LBUTTONDOWN: {
					if (modex != 0 && modey != 0) {   //接收左键信息设置模式，当右键点击之后
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
						//设置完模式后加载扫雷界面，覆盖模式图片
						putimage(0, 0, &wall_old);
						modex = 0;
						modey = 0;
					}
					else if (msg.x > 250 && msg.x < 750 && msg.y > 120 && msg.y < 250) {
						MinesweeperGame(mode);    // ---> 6
						goto Inter;   //跳出多层循环， 重新加载扫雷界面
					}
					else if (msg.x > 250 && msg.x < 750 && msg.y > 350 && msg.y < 450) {
						return;  //直接退出到主界面
					}
					break;
				}
				case WM_RBUTTONDOWN: {
					//右键设置模式
					modex = msg.x;
					modey = msg.y;
					IMAGE MODE;
					loadimage(&MODE, "image_minesweeper\\mode.jpg");   //在当前坐标放置模式图片
					putimage(modex, modey, &MODE);
					break;
				}
				}
			}
		}
	Inter:
		printf("扫雷界面\n");
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
		loadimage(&img, "image_mazegame\\开头.jpg");
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
						MessageBox(hWnd, "小心boger刹不住车导致碰壁", "！！温馨提示！！ ", MB_OKCANCEL);
						MazeGame();    //-> Mazegame.cpp
						goto Inter;
					}
				}
			}
		}
	Inter:
		printf("扫雷界面\n");
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

	//账号已登录，直接加载页面
	if (*saveId != 0) {
		ContactInter(ID[*saveId], ID, saveId);    //  -> Contact.cpp
		return;
	}

	while (1) {
		if (MouseHit()) {
			MOUSEMSG msg = GetMouseMsg();
			switch (msg.uMsg) {
			case WM_LBUTTONDOWN: 
				//注册
				if (msg.x > 550 && msg.x < 650 && msg.y > 300 && msg.y < 350) {   
					HWND hWnd = GetHWnd();
					MessageBox(hWnd, "请在命令行输入相关信息", "! ", MB_OKCANCEL);

					while (1) {
						printf("学号:> ");
						long long stu_number = 0;
						scanf("%lld", &stu_number);

						IP info;   //创建登录的用户的信息的临时变量

						if (ID.count(stu_number)) {
							printf("该学号已被注册, 请重新输入!\n");
							continue;
						}
						printf("密码:> "); scanf("%s", info.key);
						printf("名字:> "); scanf("%s", info.name);
						printf("年龄:> "); scanf("%d", &(info.age));
						printf("性别:> "); scanf("%s", info.sex);
						printf("地址:> "); scanf("%s", info.address);
						printf("电话:> "); scanf("%s", info.tele);
						info.stu_number = stu_number;

						//储存一个随机头像在该注册用户中
						char th = rand() % 10 + 97;
						sprintf(info.avatar, "avatarfile\\%c.jpg", th);
						//以学号名作为文件名，再以char* 类型储存在结构体中
						sprintf(info.contact, "%lld.txt", stu_number);   

						FILE* pf = fopen(info.contact, "w");   //在当前目录下创建以该学号为名的txt文档
						fclose(pf);

						ID[stu_number] = info;
						SaveStudentsID(&stu_number, &info);   //在ID中保存该用户

						HWND hWnd = GetHWnd();
						MessageBox(hWnd, "注册成功！请登录！", "! ", MB_OKCANCEL);
						break;
					}
				}
				//登录
				if (msg.x > 350 && msg.x < 450 && msg.y > 300 && msg.y < 350) {   
					HWND hWnd = GetHWnd();
					MessageBox(hWnd, "请在命令行输入相关信息", "! ", MB_OKCANCEL);
					printf("学号:> ");
					long long stu_number = 0;
					scanf("%lld", &stu_number);
					if (!ID.count(stu_number)) {
						printf("没有该学生信息！\n");
						continue;
					}
					while (1) {
						printf("密码:>");
						char ikey[20];
						memset(ikey, '\0', sizeof(ikey));
						scanf("%s", ikey);
						if (!strcmp(ID[stu_number].key, ikey)) {    //密码匹配
							*saveId = stu_number;    //保存此时登录的用户
							ContactInter(ID[stu_number], ID, saveId);   //ID[stu_number] 指 user 表示登录的用户的信息
							return;  //返回桌面
						}
						else {
							printf("key wrong!\n");
							continue;
						}
					}
				}
				//注销账号
				if (msg.x > 600 && msg.x < 660 && msg.y > 205 && msg.y < 229)
				{
					HWND hWnd = GetHWnd();
					MessageBox(hWnd, "请在命令行输入需要注销的账号", "! ", MB_OKCANCEL);
					long long del;
					printf("请输入你要注销的学号：>");
					scanf("%lld", &del);
					if (!ID.count(del)) {
						printf("没有该学生信息\n");
						break;
					}
					DelstudentsID(del);
					LoadStudentsID(ID);   //注销之后重新加载ID，更新
					return;
				}
				//退出——> 返回桌面
				if (msg.x > 670 && msg.x < 693 && msg.y > 205 && msg.y < 229)  return;    
			}
		}
	}
}

int main() {
	long long saveUser = 0;    //保留正在登录的用户的学号

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
						MinesweeperInter();   //进入扫雷界面   -> 51
						goto Desktop;
					}
					if (msg.x > 110 && msg.x < 210 && msg.y > 0 && msg.y < 100) {
						MazegameInter();    //进入迷宫界面     -> 115
						initgraph(1000, 600, SHOWCONSOLE);
						goto Desktop;
					}
					if (msg.x > 900 && msg.x < 1000 && msg.y > 0 && msg.y < 100) {
						LoginScreen(&saveUser);    //进入登录界面   -> 151
						goto Desktop;
					}
				}
			}
		}
	Desktop:
		printf("go_to_Desktop\n");    //覆盖之前的图像
	}
	return 0;
}