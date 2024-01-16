#define _CRT_SECURE_NO_WARNINGS 1
#include "Interface.h"

vector<char> path;
bool BackTracking(int maze[mROW][mCOL], int r, int c, int move[4][2], char moveS[4]) {
	if (r == 19 && c == 27) return true;   //�ҵ�����

	if (maze[r - 1][c] == 1 && maze[r + 1][c] == 1 && maze[r][c + 1] == 1 && maze[r][c - 1] == 1) {
		return false;    //��������£�����false,���ݣ��������
	}

	for (int i = 0; i < 4; i++) {
		if (maze[r + move[i][0]][c + move[i][1]] == 1) continue;
		path.push_back(moveS[i]);
		maze[r + move[i][0]][c + move[i][1]] = 1;    //����·��������ֹ���޵ݹ�

		if (BackTracking(maze, r + move[i + 4][0], c + move[i + 4][1], move, moveS)) return true;   //�ҵ�������������true
		else {
			path.pop_back();    //����path�е�����
			maze[r + move[i][0]][c + move[i][1]] = 0;     //����ȥ·���� ����ʾ��·�Ҳ�����
		}
	}
	return false;    //��·���޴𰸣�����false�����п��ޣ�
}

void AutomaticSolving(int maze[mROW][mCOL], int startrow, int startcol) {
	char moveS[4] = { 's', 'w', 'd', 'a' };
	int move[8][2] = { {1,0},{-1, 0},{0, 1},{0, -1},{2,0},{-2, 0},{0, 2},{0, -2} };    //ǰ�ĸ�Ϊ���߷��򣬺��ĸ�Ϊ���߲���
	BackTracking(maze, startrow, startcol, move, moveS);    ////��Ŀǰ[startrow, startcol]λ�ÿ�ʼѰ�Ҵ𰸣�����->��ý������  -> 5

	int i = startrow;   
	int j = startcol;
	int x = 4 + (j / 2) * 50;
	int y = 4 + (i / 2) * 50;
	settextcolor(RED);
	IMAGE mazeimage;
	loadimage(&mazeimage, "image_mazegame\\�Թ�.jpg");
	IMAGE boger;
	loadimage(&boger, "image_mazegame\\����.jpg", 45, 45);

	for (char move : path) {    //����path�е�����·��
		cleardevice();
		putimage(0, 0, &mazeimage);
		putimage(x, y, &boger);

		if (move == 'w') {
			i -= 2;
			y -= 50;
		} else if (move == 's') {
			i += 2;
			y += 50;
		} else if (move == 'a') {
			j -= 2;
			x -= 50;
		} else {
			j += 2;
			x += 50;
		}
		Sleep(100);
	}
}

void MazeGame(){
	int maze[mROW][mCOL] = { {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,1},
		{1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1},{1,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1},{1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1},
		{1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1},{1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1},
		{1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1},{1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1},
		{1,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1},{1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1},
		{1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1},{1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1},
		{1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1},{1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1},
		{1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},{1,0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1},
		{1,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1},{1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} };
	while (1) {
		int i = 1;  //��  (��ʼ����)
		int j = 1; //��
		int learnPoint = 90;
		char lPointstr[20];

		settextcolor(RED);
		IMAGE mazeimage;
		loadimage(&mazeimage, "image_mazegame\\�Թ�.jpg");
		IMAGE boger;
		loadimage(&boger, "image_mazegame\\����.jpg", 45, 45);

		int x = 4;
		int y = 4;
		int Armor = 3;   //���ף�ײǽʱ-1
		char armorStr[20];
		while (i != 19 || j != 27) {

			FlushBatchDraw();
			cleardevice();
			putimage(0, 0, &mazeimage);
			putimage(x, y, &boger);
			sprintf(lPointstr, "��ʣѧ�֣�%d", learnPoint);
			outtextxy(700, 300, lPointstr);
			EndBatchDraw();


			if (MouseHit()) {
				MOUSEMSG msg = GetMouseMsg();
				switch (msg.uMsg) {
				case WM_LBUTTONDOWN:
					if (msg.x > 700 && msg.x < 800 && msg.y > 357 && msg.y < 400) {
						AutomaticSolving(maze, i, j);   //�Զ����  ->  26
						i = 19; j = 27;   //��������������δ�յ�λ�ã�����while֮���ж�Ϊ��Ϸʤ��
						break;
					}
					if (msg.x > 700 && msg.x < 800 && msg.y > 10 && msg.y < 52) {
						return;
					}
				}
			}

			if (GetAsyncKeyState(VK_UP))
			{
				if (maze[i - 1][j] == 1) {
					if (Armor == 0)
						break;
					else {
						Beep(494, 200);
						Armor--;
						sprintf(armorStr, "ʣ�໤��ֵ��%d", Armor);
						HWND hWnd = GetHWnd();
						MessageBox(hWnd, armorStr, "! ! ! ! ! ! !", MB_OKCANCEL);
						continue;
					}
				}
				i -= 2;
				y -= 50;
				learnPoint--; 
				Sleep(300);
			}
			else if (GetAsyncKeyState(VK_DOWN))
			{
				if (maze[i + 1][j] == 1) {
					if (Armor == 0)
						break;
					else {
						Beep(494, 200);
						Armor--;
						sprintf(armorStr, "ʣ�໤��ֵ��%d", Armor);
						HWND hWnd = GetHWnd();
						MessageBox(hWnd, armorStr, "! ! ! ! ! ! !", MB_OKCANCEL);
						continue;
					}
				}
				i += 2;
				y += 50;
				learnPoint--;
				Sleep(300);
			}
			else if (GetAsyncKeyState(VK_LEFT))
			{
				if (maze[i][j - 1] == 1) {
					if (Armor == 0)
						break;
					else {
						Beep(494, 200);
						Armor--;
						sprintf(armorStr, "ʣ�໤��ֵ��%d", Armor);
						HWND hWnd = GetHWnd();
						MessageBox(hWnd, armorStr, "! ! ! ! ! ! !", MB_OKCANCEL);
						continue;
					}
				}
				j -= 2;
				x -= 50;
				learnPoint--;
				Sleep(300);
			}
			else if (GetAsyncKeyState(VK_RIGHT))
			{
				if (maze[i][j + 1] == 1) {
					if (Armor == 0)
						break;
					else {
						Beep(494, 200);
						Armor--;
						sprintf(armorStr, "ʣ�໤��ֵ��%d", Armor);
						HWND hWnd = GetHWnd();
						MessageBox(hWnd, armorStr, "! ! ! ! ! ! !", MB_OKCANCEL);
						continue;
					}
				}
				j += 2;
				x += 50;
				learnPoint--;
				Sleep(300);
			}
			if (learnPoint < 0) break;
		} 

		//���ֽ������
		if (learnPoint < 0) {
			int lod = -500;
			IMAGE game_over_point;
			loadimage(&game_over_point, "image_mazegame\\game_over_point.jpg");
			while (lod <= 0) {
				BeginBatchDraw();
				cleardevice();
				putimage(0, lod, &game_over_point);
				EndBatchDraw();
				lod++;
				Sleep(1);
			}
		}
		else if (i == 19 && j == 27) {
			int lod = 500;
			IMAGE game_over_win;
			loadimage(&game_over_win, "image_mazegame\\game_over_win.jpg");
			while (lod >= 0) {
				BeginBatchDraw();
				cleardevice();
				putimage(0, lod, &game_over_win);
				EndBatchDraw();
				lod--;
				Sleep(1);
			}
		}
		else if (maze[i - 1][j] == 1 || maze[i + 1][j] == 1 || maze[i][j - 1] == 1 || maze[i][j + 1] == 1) {
			int lod = 800;
			IMAGE game_over_die;
			loadimage(&game_over_die, "image_mazegame\\game_over_die.jpg");
			while (lod >= 0) {
				BeginBatchDraw();
				cleardevice();
				putimage(lod, 0, &game_over_die);
				EndBatchDraw();
				lod--;
				Sleep(1);
			}
		}

		while (1) {
			if (MouseHit()) {
				MOUSEMSG msg = GetMouseMsg();
				switch (msg.uMsg) {
				case WM_LBUTTONDOWN: 
					if (msg.x > 228 && msg.x < 571 && msg.y > 200 && msg.y < 300) {
						goto AGAIN;   //���ص�������whileѭ��
					}
					if (msg.x > 228 && msg.x < 571 && msg.y > 300 && msg.y < 400) {
						return;
					}
				}
			}
		}
	AGAIN:
		printf("keeping playing!\n");
	}

}