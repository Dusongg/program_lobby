#define _CRT_SECURE_NO_WARNINGS 1
#include "Interface.h"

void InitBoard(char Board[ROWS][COLS], int rows, int cols, int set) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			Board[i][j] = set;
		}
	}
}

//踩雷之后展示整个棋盘
void DisPlayBoardGra(char Board[ROWS][COLS], int r, int c) {
	for (int i = 1; i < ROWS-1; i++) {
		for (int j = 1; j < COLS-1; j++) { 
			if (i == r && j == c) continue;

			if (Board[i][j] - '0' == 1) {
				
				IMAGE bomb;
				loadimage(&bomb, "image_minesweeper\\bomb.jpg");
				putimage(100 + (j - 1) * 40, 100 + (i - 1) * 40, &bomb);
			}
			else {
				IMAGE blank2;
				loadimage(&blank2, "image_minesweeper\\blank.jpg");
				putimage(100 + (j - 1) * 40, 100 + (i - 1) * 40, &blank2);
			}
			Sleep(75); 
		}
	}
}

//随机放置mode个数的雷到底层棋盘中
void SetBoard(char Board[ROWS][COLS], int row, int col, int mode){
	int count = mode;
	while (count){
		int x = rand() % row + 1;
		int y = rand() % col + 1;
		if (Board[x][y] != '1'){
			Board[x][y] = '1';
			count--;
		}
	}
}

//计算周围雷的数量
int Count(char inBoard[ROWS][COLS], int x, int y){
	return (inBoard[x - 1][y - 1] + inBoard[x - 1][y] + inBoard[x][y - 1] +
		inBoard[x + 1][y - 1] + inBoard[x + 1][y] + inBoard[x + 1][y + 1] +
		inBoard[x][y + 1] + inBoard[x - 1][y + 1] - 8 * '0');
}

void LookForRay(char inBoard[ROWS][COLS], char outBoard[ROWS][COLS], int x, int y, int* win){
	if (outBoard[x][y] == 'F') return;  //递归到红旗返回

	if (inBoard[x][y] == '0')
		*win -= 1;    //记录win
	char sum = Count(inBoard, x, y);   //计算周围雷的个数：越界问题？
	outBoard[x][y] = sum + '0';

	//显示该点周围雷的个数
	char sum_str[30];
	sprintf(sum_str, "image_minesweeper\\%d.jpg", sum);    // 用sprintf合并字符串，通过文件相对位置找到对应图片
	IMAGE number;
	loadimage(&number, sum_str);
	putimage(100 + (y - 1) * 40, 100 + (x - 1) * 40, &number);

	//该坐标周围没有雷，想周围递归
	if (outBoard[x][y] == '0'){
		IMAGE blank;
		loadimage(&blank, "image_minesweeper\\blank.jpg");
		putimage(100 + (y - 1) * 40, 100 + (x - 1) * 40, &blank);

		if (x - 1 >= 1 && x - 1 <= ROW && y - 1 >= 1 && y - 1 <= COL && outBoard[x - 1][y - 1] == '*')    //递归条件：递归坐标未被排点，
			LookForRay(inBoard, outBoard, x - 1, y - 1, win);    //win为地址

		if (x - 1 >= 1 && x - 1 <= ROW && y >= 1 && y <= COL && outBoard[x - 1][y] == '*')
			LookForRay(inBoard, outBoard, x - 1, y, win);

		if (x - 1 >= 1 && x - 1 <= ROW && y + 1 >= 1 && y + 1 <= COL && outBoard[x - 1][y + 1] == '*')
			LookForRay(inBoard, outBoard, x - 1, y + 1, win);

		if (x >= 1 && x <= ROW && y - 1 >= 1 && y - 1 <= COL && outBoard[x][y - 1] == '*')
			LookForRay(inBoard, outBoard, x, y - 1, win);

		if (x >= 1 && x <= ROW && y + 1 >= 1 && y + 1 <= COL && outBoard[x][y + 1] == '*')
			LookForRay(inBoard, outBoard, x, y + 1, win);

		if (x + 1 >= 1 && x + 1 <= ROW && y - 1 >= 1 && y - 1 <= COL && outBoard[x + 1][y - 1] == '*')
			LookForRay(inBoard, outBoard, x + 1, y - 1, win);

		if (x + 1 >= 1 && x + 1 <= ROW && y >= 1 && y <= COL && outBoard[x + 1][y] == '*')
			LookForRay(inBoard, outBoard, x + 1, y, win);

		if (x + 1 >= 1 && x + 1 <= ROW && y + 1 >= 1 && y + 1 <= COL && outBoard[x + 1][y + 1] == '*')
			LookForRay(inBoard, outBoard, x + 1, y + 1, win);
	}
}

void StartToPlay(char inBoard[ROWS][COLS], char outBoard[ROWS][COLS], int row, int col, int mode, int* keep_playing){
	int r = 0;
	int c = 0;
	int win = row * col - mode;   //当win等于0时，即剩下的格子全是雷时，判断胜利
	bool die = false;
	//🚩数量
	int flag = mode;
	char flag_str[5];
	//防止一开始就踩雷
	int deathfree = 0;
	char deathfree_str[20];
	if (mode == MODE_EAZY) deathfree = 1;
	if (mode == MODE_MID) deathfree = 2;
	if (mode == MODE_HARD) deathfree = 3;

	//判断循环条件
	while (win && !die){
		if (MouseHit()) {
			MOUSEMSG msg = GetMouseMsg();
			switch (msg.uMsg) {
				case WM_LBUTTONDOWN: {
					r = ((msg.y - 100) / 40) + 1;   // 5位棋盘格一小格的宽度 10 * 50 = 500
					c = ((msg.x - 100) / 40) + 1;
					if (msg.x > 830 && msg.x < 970 && msg.y > 525 && msg.y < 580) {
						*keep_playing = 0;  //退出游戏，返回扫雷界面
						return;
					}

					if (r >= 1 && r <= row && c >= 1 && c <= col) {
						if (outBoard[r][c] == 'F') break;   //点击已插旗的格子

						//一开始踩到雷的情况 ：将该点插入红旗，提醒该点有雷，放置一开始就点到雷
						if (inBoard[r][c] == '1' && deathfree > 0) {
							Beep(494, 200);
							//减少红旗并插入红旗
							flag--;  
							outBoard[r][c] = 'F';
							IMAGE FLAG;
							loadimage(&FLAG, "image_minesweeper\\flag.jpg");
							putimage(100 + (c - 1) * 40, 100 + (r - 1) * 40, &FLAG);
							IMAGE flag_board;
							loadimage(&flag_board, "image_minesweeper\\flag_board.jpg");
							putimage(700, 40, &flag_board);
							sprintf(flag_str, "%d", flag);
							outtextxy(850, 40, (flag_str));
							deathfree--;
							sprintf(deathfree_str, "剩余防弹衣：%d", deathfree);
							HWND hWnd = GetHWnd();
							MessageBox(hWnd, deathfree_str, "! ! ! ! ! ! !", MB_OKCANCEL);
							break;
						}
						if (inBoard[r][c] == '1' && deathfree == 0) {
							mciSendString("open bomb.mp3", 0, 0, 0);
							mciSendString("play bomb.mp3", 0, 0, 0);
							IMAGE red_bomb;
							loadimage(&red_bomb, "image_minesweeper\\red_bomb.jpg");
							putimage(100 + (c - 1) * 40, 100 + (r - 1) * 40, &red_bomb);
							DisPlayBoardGra(inBoard, r, c);   //展示整个棋盘
							die = true;   //将die置为true跳出外层的while循环
							break;
						}
						if (outBoard[r][c] != '*') {
							break;
						}
						LookForRay(inBoard, outBoard, r, c, &win);   //排雷:DFS -->  54

						if (deathfree > 0)  //最开始的前几此排雷对deathfree--；
							deathfree--;
					}
					break;
				}
				//右键插入红旗 ： 该坐标有旗与无旗？ 剩余红旗？
				case WM_RBUTTONDOWN: {
					r = ((msg.y - 100) / 40) + 1;  
					c = ((msg.x - 100) / 40) + 1;
					if (r > 0 && r < 11 && c > 0 && c < 11) {
						if (outBoard[r][c] == '*' && outBoard[r][c] != 'F') {
							outBoard[r][c] = 'F';
							if (flag == 0) {
								Beep(494, 200);//震动提示
								HWND hWnd = GetHWnd();
								MessageBox(hWnd, "无剩余红旗可用!", "无剩余红旗可用!", MB_OKCANCEL);
								break;
							}
							flag--;

							IMAGE flag_board;
							loadimage(&flag_board, "image_minesweeper\\flag_board.jpg");
							putimage(700, 40, &flag_board);
							sprintf(flag_str, "%d", flag);
							outtextxy(850, 40, flag_str);

							//放置🚩
							IMAGE FLAG;
							loadimage(&FLAG, "image_minesweeper\\flag.jpg");
							putimage(100 + (c - 1) * 40, 100 + (r - 1) * 40, &FLAG);
						}
						else if (outBoard[r][c] == 'F') {
							outBoard[r][c] = '*';
							//回收🚩
							IMAGE back;
							loadimage(&back, "image_minesweeper\\back.jpg");
							putimage(100 + (c - 1) * 40, 100 + (r - 1) * 40, &back);

							flag++;
							IMAGE flag_board;
							loadimage(&flag_board, "image_minesweeper\\flag_board.jpg");
							putimage(700, 40, &flag_board);
							sprintf(flag_str, "%d", flag);
							outtextxy(850, 40, flag_str);
						}
					}
					break;
				}
			}
		}
	}
	
	IMAGE end;
	loadimage(&end, "image_minesweeper\\end.jpg");
	putimage(200, 150, &end);
	if (win == 0) {
		settextcolor(RGB(250, 51, 57));
		outtextxy(280, 100, "!!!WIN!!!");
	}
	while (1) {
		if (MouseHit()) {
			MOUSEMSG msg = GetMouseMsg();
			switch (msg.uMsg) {
			case WM_LBUTTONDOWN: {
				if (msg.x > 200 && msg.x < 800 && msg.y > 150 && msg.y < 300) {
					return;     //keep_playing未改变，继续玩
				}
				if (msg.x > 200 && msg.x < 800 && msg.y > 300 && msg.y < 450) {
					*keep_playing = 0;   //返回扫雷界面
					return;
				}
			}
			}
		}
	}
}