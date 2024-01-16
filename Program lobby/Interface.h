#define _CRT_SECURE_NO_WARNINGS 1
#undef UNICODE
#undef _UNICODE

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <easyx.h>
#include <mmsystem.h>  
#pragma comment(lib,"winmm.lib")
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <graphics.h>
#include <windows.h>
#include <unordered_map>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

//1. Minesweeper
#define ROW 10
#define COL 10
#define ROWS ROW+2
#define COLS COL+2
#define MODE_EAZY 10
#define MODE_MID 15
#define MODE_HARD 20

void InitBoard(char Board[ROWS][COLS], int rows, int cols, int set);
void SetBoard(char Board[ROWS][COLS], int row, int col, int mode);
void StartToPlay(char inBoard[ROWS][COLS], char outBoard[ROWS][COLS], int row, int col, int mode, int* keep_playing);

//2. Mazegame
#define mROW 21
#define mCOL 29
void MazeGame();

//3. Contact
#define NAME_MAX 16
#define SEX_MAX 8
#define ADD_MAX 16
#define TELE_MAX 16
#define KEY_MAX 16
#define CON_MAX 32
#define AVA_MAX 32

typedef struct PeoInfo  //通过学号定位
{
	long long stu_number;  //学号
	char name[NAME_MAX];
	char key[KEY_MAX];  //密码
	int age;
	char sex[SEX_MAX];
	char address[ADD_MAX];
	char tele[TELE_MAX];
	char contact[CON_MAX];   //该学生的通讯录
	char avatar[AVA_MAX];   //头像
}IP;

void ContactInter(IP info, unordered_map<long long, IP> ID, long long* saveId);

//基于map的所有用户管理
void LoadStudentsID(unordered_map<long long, IP>& ID);
void SaveStudentsID(long long* id, IP* info);
void DelstudentsID(long long del);
