/*---------------------
地板：0
墙：1
箱子目的地：2
凛酱：3
箱子：4
凛酱站在目的地：3+2
箱子在目的地：4+2
----------------------*/
#include <graphics.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <mmsystem.h>
#include <easyx.h>
#pragma comment(lib,"winmm.lib")


#define MAT_RAT 61
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 768
#define START_X 100
#define START_Y 150
#define Lev 3

IMAGE start;
IMAGE process;
IMAGE mat[7];


int map[Lev][9][12] =
{
	{
	{1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,1,0,0,0,0,0,0,0,1,1},
	{1,0,4,0,1,2,0,1,2,0,1,1},
	{1,0,1,0,1,0,1,1,0,0,0,1},
	{1,0,1,2,1,0,0,4,0,0,0,1},
	{1,0,0,0,1,3,0,0,0,4,0,1},
	{1,0,2,0,0,4,0,0,0,0,0,1},
	{1,0,1,1,0,1,0,0,1,1,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1},
	},
	{
	{1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,1,0,0,0,0,0,0,0,1,1},
	{1,0,4,0,1,2,0,1,2,0,1,1},
	{1,0,1,0,1,0,1,1,0,0,0,1},
	{1,0,1,2,1,0,0,4,0,0,0,1},
	{1,0,0,0,1,3,0,0,0,4,0,1},
	{1,0,2,0,0,4,0,0,0,0,0,1},
	{1,0,1,1,0,1,0,0,1,1,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1},
    },
};
int pre_map[99][9][12];
int step = 1;
int lv = 0; //关卡变量
struct Lin_pos
{
	int x;
	int y;
}Lin;

void BGM_on(int x)
{
	if (x == 1)
	{
		mciSendString("open ./1.mp3", 0, 0, 0);
	    mciSendString("play ./1.mp3", 0, 0, 0);
    }
	if (x == 2)
	{
		mciSendString("open ./2.wav", 0, 0, 0);
		mciSendString("play ./2.wav", 0, 0, 0);
	}
}

void BGM_off(int x)
{
	if (x == 1)
	{
		mciSendString("close ./1.mp3", 0, 0, 0);
	}
	if (x == 2)
	{
		mciSendString("close ./2.wav", 0, 0, 0);
	}
}
	
void column() {
	HWND hWnd = GetHWnd();
	SetWindowText(hWnd, "JY's push-box");
}

void button(int x, int y, int w, int h,const char* ch)
{
	setbkmode(TRANSPARENT);
	setfillcolor(BROWN);
	fillroundrect(x, y, x + w, y + h, 20, 20);
	settextcolor(WHITE);
	settextstyle(30, 0, _T("黑体"), 0, 0, 1000, false, false, false);
	RECT r = { x, y, x + w, y + h };
	char text[50] = "button";
	strcpy_s(text, ch);
	drawtext(text, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void game();
void draw();
void rcd();
void move();

int main()
{
	initgraph(1000,861);
	column();
	loadimage(&start, "start.jpeg", 1000, 861, true);
	putimage(0, 0, &start);
	button(70, 50, 150, 50, "开始游戏");
	button(70, 150, 150, 50, "选择关卡");
	button(70, 250, 150, 50, "游戏帮助");
	BGM_on(1);
	ExMessage msg;
	while (1)
	{
		if (peekmessage(&msg, EM_MOUSE))
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
				if (msg.x >= 70 && msg.x <= 220 && msg.y >= 50 && msg.y <= 100)
				{
					BGM_off(1);
					closegraph();
					game();
				}
				if (msg.x >= 70 && msg.x <= 220 && msg.y >= 150 && msg.y <= 200)
				{
					HWND hWnd0 = GetHWnd();
					MessageBox(hWnd0, "我还没做hhh", "摆烂ZJY", MB_OK);
				}
				if (msg.x >= 70 && msg.x <= 220 && msg.y >= 250 && msg.y <= 300)
				{
					HWND hWnd0 = GetHWnd();
					MessageBox(hWnd0, "W,A,S,D或方向键控制移动，e撤回", "帮助", MB_OK);
				}
				break;
			default:
				break;
			}
		}
	}
}

void game()
{
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);
	column();
	BGM_on(2);
	loadimage(&process, "process.jpeg", SCREEN_WIDTH, SCREEN_HEIGHT, true);
	loadimage(&mat[0], "floor.png", MAT_RAT, MAT_RAT, true);
	loadimage(&mat[1], "wall.png", MAT_RAT, MAT_RAT, true);
	loadimage(&mat[2], "des.png", MAT_RAT, MAT_RAT, true);
	loadimage(&mat[3], "lin.png", MAT_RAT, MAT_RAT, true);
	loadimage(&mat[4], "box.jpg", MAT_RAT, MAT_RAT, true);
	loadimage(&mat[5], "lin.png", MAT_RAT, MAT_RAT, true);
	loadimage(&mat[6], "fin.jpg", MAT_RAT, MAT_RAT, true);
	putimage(0, 0, &process);
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 12; j++)
			pre_map[step][i][j] = map[lv][i][j];
	while (1)
	{
		draw();
		move();
		int counter = 0;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 12; j++) {
				if (map[lv][i][j] == 6)
					counter++;
			}
		}
		if (counter == 4)
			break;
	}
	closegraph();
	HWND hWnd0 = GetHWnd();
	MessageBox(hWnd0, "后续关卡我还没做hhh", "摆烂ZJY", MB_OK);
}

void draw()
{
	RECT k = { 30,40,900,90 };
	for (int i = 0;i < 9;i++)
		for (int j = 0;j < 12;j++)
			putimage(START_X + j * MAT_RAT, START_Y + i * MAT_RAT, &mat[map[lv][i][j]]);
	setbkmode(TRANSPARENT);
	settextstyle(20, 10, "黑体");
	drawtext("W,A,S,D或方向键控制移动，e撤回", &k, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void move()
{
	for (int i = 0;i < 9;i++)
		for (int j = 0;j < 12;j++)
		{
			if (map[lv][i][j] == 3 || map[lv][i][j] == 5)
			{
				Lin.x = j;
				Lin.y = i;
				break;
			}
		}
	char ch = _getch();
	if (ch)
	{
		mciSendString("close ./move.mp3", 0, 0, 0);
		mciSendString("open ./move.mp3", 0, 0, 0);
		mciSendString("play ./move.mp3", 0, 0, 0);
	}
	switch (ch)
	{
	case 72:
	case 'w':
	case 'W':
		if (map[lv][Lin.y - 1][Lin.x] == 0 || map[lv][Lin.y - 1][Lin.x]==2)
		{
			map[lv][Lin.y - 1][Lin.x] += 3;
			map[lv][Lin.y][Lin.x] -= 3;
		}
		else if (map[lv][Lin.y - 1][Lin.x] == 4 || map[lv][Lin.y - 1][Lin.x]==6)
		{
			if (map[lv][Lin.y - 2][Lin.x] != 1 && map[lv][Lin.y - 2][Lin.x] != 4 && map[lv][Lin.y - 2][Lin.x] != 6)
			{
				map[lv][Lin.y - 1][Lin.x] -= 1;
				map[lv][Lin.y - 2][Lin.x] += 4;
				map[lv][Lin.y][Lin.x] -= 3;
			}
		}
		rcd();
		break;
	
	case 75:
	case 'a':
	case 'A':
		if (map[lv][Lin.y][Lin.x - 1] == 0 || map[lv][Lin.y][Lin.x - 1] == 2)
		{
			map[lv][Lin.y][Lin.x - 1] += 3;
			map[lv][Lin.y][Lin.x] -= 3;
		}
		else if (map[lv][Lin.y][Lin.x - 1] == 4 || map[lv][Lin.y][Lin.x - 1] == 6)
		{
			if (map[lv][Lin.y][Lin.x - 2] != 1 && map[lv][Lin.y][Lin.x - 2] != 4 && map[lv][Lin.y][Lin.x - 2] != 6)
			{
				map[lv][Lin.y][Lin.x - 1] -= 1;
				map[lv][Lin.y][Lin.x - 2] += 4;
				map[lv][Lin.y][Lin.x] -= 3;
			}
		}
		rcd();
		break;
	
	case 80:
	case 's':
	case 'S':
		if (map[lv][Lin.y + 1][Lin.x] == 0 || map[lv][Lin.y + 1][Lin.x] == 2)
		{
			map[lv][Lin.y + 1][Lin.x] += 3;
			map[lv][Lin.y][Lin.x] -= 3;
		}
		else if (map[lv][Lin.y + 1][Lin.x] == 4 || map[lv][Lin.y + 1][Lin.x] == 6)
		{
			if (map[lv][Lin.y + 2][Lin.x] != 1 && map[lv][Lin.y + 2][Lin.x] != 4 && map[lv][Lin.y + 2][Lin.x] != 6)
			{
				map[lv][Lin.y + 1][Lin.x] -= 1;
				map[lv][Lin.y + 2][Lin.x] += 4;
				map[lv][Lin.y][Lin.x] -= 3;
			}
		}
		rcd();
		break;
	
	case 77:
	case 'd':
	case 'D':
		if (map[lv][Lin.y][Lin.x + 1] == 0 || map[lv][Lin.y][Lin.x + 1] == 2)
		{
			map[lv][Lin.y][Lin.x + 1] += 3;
			map[lv][Lin.y][Lin.x] -= 3;
		}
		else if (map[lv][Lin.y][Lin.x + 1] == 4 || map[lv][Lin.y][Lin.x + 1] == 6)
		{
			if (map[lv][Lin.y][Lin.x + 2] != 1 && map[lv][Lin.y][Lin.x + 2] != 4 && map[lv][Lin.y][Lin.x + 2] != 6)
			{
				map[lv][Lin.y][Lin.x + 1] -= 1;
				map[lv][Lin.y][Lin.x + 2] += 4;
				map[lv][Lin.y][Lin.x] -= 3;
			}
		}
		rcd();
		break;
	
	case 'e':
	case 'E':
		if (step > 1) {
			step--;
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 12; j++)
					map[lv][i][j] = pre_map[step][i][j];
		}
		break;
	default:
		break;
	}
}

void rcd()
{
	step++;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 12; j++)
			pre_map[step][i][j] = map[lv][i][j];
}