// 大球吃小球游戏.cpp : 定义控制台应用程序的入口点。
//
#include<graphics.h>
#include<ctime>
#include<iostream>
#include<vector>
#include<cstdio>
#include<cstring>
#include<conio.h>
using namespace std;



void intiGame();
void drawGame();
void controlGame();


using namespace std;

#define WIDE 600
#define HEIGHT 800
#define NUM 20
double LIVETIME =60; //存活时间
const int MOVE_DIS = 10;
struct Fx {
	int x;//单位时间x轴移动距离  正：往右 负：往左
	int y;

};
struct Ball {
	int x;
	int y;
	int radius;
	COLORREF color;
	Fx fx;
};

struct User {
	char name[20];
	int score;
};

struct List {
	struct User data;
	struct List* next;
};


COLORREF colors[7] = { GREEN,MAGENTA,BROWN,YELLOW,BLUE,CYAN,LIGHTGRAY };
vector<Ball> Sballs;
Ball BigBall;

HWND hWnd;
bool isLive = true;

void intiGame();
void drawGame();
void controlGame();
void proc();
void setBigBall();
void Welcom();
void Grade();
void sortList(struct List *head);
int lenList(struct List *head);
struct List * loadFile();
void saveFile(struct List *head);
void drawGrade(struct List *head);


int  _tmain(int  argc, _TCHAR* argv[])
{
	Welcom();
	intiGame();
	drawGame();
	Grade();
	return 0;
}

int lenList(struct List *head)
{	
	if (head == NULL)		
		return 0;	
	struct List *p = head;	
	int len = 0;	
	while(p != NULL)	
	{		
		len++;		
		p = p->next;	
	}	
	return len;
} 

struct List * loadFile()
{
	struct List *head=NULL;
	struct List *p;
	struct User temp;
	FILE* fp=fopen("score.data","r+");
	if(fp==NULL)return NULL;
	fread(&temp,sizeof(struct User),1,fp);
	while(!feof(fp))
	{
		p=(struct List*)malloc(sizeof(struct List));
		p->data=temp;
		p->next=head;
		head=p;
		fread(&temp,sizeof(struct User),1,fp);
	}
	fclose(fp);
	return head;
}

void saveFile(struct List *head)
{
	struct List *p=head;
	FILE* fp=fopen("score.data","w+");
	if(fp==NULL)return ;
	while(p)
	{
		fwrite(&p->data,sizeof(struct User),1,fp);
		p=p->next;
	}
	fclose(fp);
}


void sortList(struct List *head)
{	
	int len = lenList(head);	
	if (len == 0)		
		return;	
	struct List *p = head;	
	int i, j;
	struct User temp;
	for (i = 0; i < len - 1; i++)	
	{		
		p = head;		
		for (j = 0; j < len -i -1; j++)		
		{			
			if (p->data.score < p->next->data.score)			
			{	
				temp=p->data;
				p->data=p->next->data;
				p->next->data=temp;
			}			
			p = p->next;		
		}	
	}
}

void drawGrade(struct List *head)
{
	char t[100];
	sortList(head);
	struct List *p=head;
	int i;
	IMAGE img1;
	loadimage(&img1, _T("1.jpg"),150,150);
	
	RECT r = {0, 0, 600, 200};
	setbkcolor(WHITE);
	cleardevice();
	putimage(450, 650, &img1);
	settextstyle(48, 24, _T("Consolas"));
	drawtext(_T("排行榜"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r2 = {0, 0, 700, 300};
	settextstyle(24, 12, _T("Consolas"));
	sprintf(t,"%-8s%-10s%-10s","名次","姓名","得分");
	drawtext(_T(t), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);	
	for(i=0;i<10&&p!=NULL;i++,p=p->next)
	{
		r2.bottom+=100;
		sprintf(t,"%-8d%-10s%-10d",i+1,p->data.name,p->data.score);
		drawtext(_T(t), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);	
	}
	getch();
}

void Grade()
{
	struct List* head;
	head=loadFile();
	char s[100];
	bool flag=InputBox(s, 100, "英雄，请输入大名:");
	if(flag)
	{
		struct List* p=(struct List*)malloc(sizeof(struct List));
		strcpy(p->data.name,s);
		p->data.score=BigBall.radius-20;
		p->next=head;
		head=p;
	}
	saveFile(head);
	drawGrade(head);
}

void Welcom()
{
	char ch;
	hWnd = initgraph(WIDE, HEIGHT);
	RECT r = {0, 0, WIDE, HEIGHT-180};
	IMAGE img1;
	do
	{
		setbkcolor(WHITE);
		cleardevice();
		loadimage(&img1, _T("peiqi.png"),600,400);
		putimage(0, 400, &img1);
		
		settextcolor(BLUE);
		RECT r2 = {0, 0, 600, 400};
		settextstyle(48, 24, _T("Consolas"));
		drawtext(_T("欢乐球吃球"), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		settextstyle(36, 18, _T("Consolas"));
		r.bottom=HEIGHT-180;
		drawtext(_T("1:开始游戏"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		r.bottom=HEIGHT;
		drawtext(_T("2:排行榜"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		ch=getch();
		if(ch=='2')
		{
			struct List* head;
			head=loadFile();
			drawGrade(head);
		}
	}while(ch!='1');
}

void intiGame() {
	Ball* pb = NULL;
	for (int i = 0; i < NUM; i++) {
		pb = new Ball;
		pb->x = rand() % WIDE;
		pb->y = rand() % HEIGHT;
		pb->radius = rand() % 10 + 1;
		pb->color = colors[rand() % 8];
		pb->fx.x = rand() % 5 + 1;
		pb->fx.y = (rand() % 10) - 5;

		Sballs.push_back(*pb);
	}
	BigBall.color = RED;
	BigBall.x = rand()%WIDE;
	BigBall.y = rand()%HEIGHT;
	BigBall.radius = 20;
}

void drawTimeScore(double time)
{
	char t[100];
	RECT r = {0, 0, 60, 60};
	settextstyle(12, 6, _T("Consolas"));
	sprintf(t,"时间:%d",(int)(LIVETIME-time));
	drawtext(_T(t), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	RECT r2 = {0, 0, 60, 100};
	settextstyle(18, 9, _T("Consolas"));
	sprintf(t,"得分:%d",BigBall.radius-20);
	drawtext(_T(t), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);	
}

void setKeyboardBigBall();

void drawGame() {
	clock_t start, finish;
	double time;
	

	SetTimer(hWnd, 10086, 50,(TIMERPROC)proc);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)setBigBall, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)setKeyboardBigBall, NULL, NULL, NULL);
	start = clock();//开始记时
	while (1) {
		controlGame();
		finish = clock();//当前时间
		time = (double)(finish - start) / CLOCKS_PER_SEC;
		if(time>=LIVETIME)isLive=false;
		if (!isLive) break;
		BeginBatchDraw();
		setbkcolor(WHITE);
		cleardevice();
	
		drawTimeScore(time);
		setfillcolor(BigBall.color);
		solidcircle(BigBall.x, BigBall.y, BigBall.radius);
		for (int i = 0; i < Sballs.size(); i++) {
			setfillcolor(Sballs[i].color);
			solidcircle(Sballs[i].x, Sballs[i].y, Sballs[i].radius);
		}
		EndBatchDraw();
	}
	setbkcolor(WHITE);
	cleardevice();
}


void controlGame() {

	int xlen, ylen;
	int score = 0;
	for (int i = 0; i < Sballs.size(); i++) {
		xlen = (Sballs[i].x > BigBall.x) ? (Sballs[i].x - BigBall.x) : (BigBall.x - Sballs[i].x);
		ylen = (Sballs[i].y> BigBall.y) ? (Sballs[i].y - BigBall.y) : (BigBall.y - Sballs[i].y);
		if ((xlen*xlen+ylen*ylen)<(Sballs[i].radius+BigBall.radius)* (Sballs[i].radius + BigBall.radius) ){
			Sballs[i].x = rand() % WIDE;
			Sballs[i].y = rand() % HEIGHT;
			Sballs[i].radius = rand() % 10 + 1;
			Sballs[i].color = colors[rand() % 7];
			Sballs[i].fx.x = rand() % 5 + 1;
			Sballs[i].fx.y = (rand() % 10) - 5;
			BigBall.radius += 1;
		}
		if (Sballs[i].x > WIDE || Sballs[i].x<0 || Sballs[i].y>HEIGHT || Sballs[i].y < 0) {
			Sballs[i].x = rand() % WIDE;
			Sballs[i].y = rand() % HEIGHT;
			Sballs[i].radius = rand() % 10 + 1;
			Sballs[i].color = colors[rand() % 7];
			Sballs[i].fx.x = rand() % 5 + 1;
			Sballs[i].fx.y = (rand() % 10) - 5;
		}
	}
}

void proc() {
	for (int i = 0; i < Sballs.size(); i++) {
		Sballs[i].x += Sballs[i].fx.x;
		Sballs[i].y += Sballs[i].fx.y;
	}
}
void setBigBall() {
	MOUSEMSG msg;
	while (1) {
		msg= GetMouseMsg();
		if (msg.mkLButton) {
			BigBall.x = msg.x;
			BigBall.y = msg.y;
		}
	}
		

}

void setKeyboardBigBall() {
	char ch;
	while(1)
	{
		if(kbhit())
		{
			ch = getch();
			switch(ch)
			{
			case 'a'://左边
			case 'A':
				if(BigBall.x-MOVE_DIS>0)
				BigBall.x-=MOVE_DIS;
				break;
			case 'w'://向上
			case 'W':
				if(BigBall.y-MOVE_DIS>0)
				BigBall.y-=MOVE_DIS;
				break;
			case 'd'://向右
			case 'D':
				if(BigBall.x+MOVE_DIS<WIDE)
				BigBall.x+=MOVE_DIS;
				break;
			case 's'://向下
			case 'S':
				if(BigBall.y+MOVE_DIS<HEIGHT)
				BigBall.y+=MOVE_DIS;
				break;
			}
		}	
	}
}