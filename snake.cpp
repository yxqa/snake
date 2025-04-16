#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>
#include <fstream>
#include <iostream>
using namespace std;

#define U 1
#define D 2
#define L 3 
#define R 4       //蛇的状态，U：上 ；D：下；L:左 R：右

typedef struct SNAKE //蛇身的一个节点
{
    int x;
    int y;
    struct SNAKE* next;
}snake;

//全局变量//
int score = 0, add = 10;        //总得分与每次吃食物得分。
int status, sleeptime = 200;        //每次运行的时间间隔
snake* head, * food;        //蛇头指针，食物指针
snake* q;       //遍历蛇的时候用到的指针
int endgamestatus = 0;      //游戏结束的情况，1：撞到墙；2：咬到自己；3：主动退出游戏。
char u_name[8];         //正在游玩的用户名称
int ID = 0;         //正在游玩的用户ID
int logView = 0;        //日志切换
//int logText = 0;        //日志文本
time_t startTime = 0;       //开始时间
//time_t endTime = 0;         //游戏结束时间
//int ganmeTime = 0;        //游戏时长(秒)

//声明全部函数//
void Pos();
void creatMap();
void initsnake();
int biteself();
void createfood();
void cantcrosswall();
void snakemove();
void pause();
void gamecircle();
void welcometogame();
void endgame();
void gamestart();

void zhuce();       //注册函数
int YanZheng();     //验证函数
void Login();       //日志


void zhuce()
{
    FILE* fp = NULL;
	char line[100]; //存储每一行
    char user[8], password[8];
	char tmp_user[8], tmp_password[8];
    int id;
	int tmp_id;
    int index = 0;
    srand((unsigned)time(NULL));


    while (1)
    {
        int user_index = 0;
        fp = fopen("text.txt", "a+");

        if (fp == NULL)
        {
            printf("ERROR!");
            exit(0);
        }
        else
        {
            printf("文件打开成功");
        }

        system("pause");
        system("cls");

        printf("输入账号：");
        scanf("%7s", user);

		//检验账号是否存在
        while (fgets(line, sizeof(line), fp))
        {
            if (sscanf(line, "id:%d\tname:%7s\tpass:%7s", &tmp_id, tmp_user, tmp_password) == 3)
            {
                if (strcmp(user, tmp_user) == 0)
                {
                    user_index = 1;
                    break;
                }
            }
        }
        fclose(fp);

        if (user_index)
        {
            printf("账号已存在，请重新输入！\n");
            continue;
        }
        else
        {
            break;
        }
    }
    printf("输入密码：");
    scanf("%7s", password);


    //生成id唯一性
    while (!index)
    {
        id = rand() % 1000 + 100;
		fp = fopen("text.txt", "a+");
        index = 1;
        while (fgets(line, sizeof(line), fp))
        {
			if (sscanf(line, "id:%d\tname:%7s\tpass:%7s", &tmp_id, tmp_user, tmp_password) == 3)
			{
				if (id == tmp_id)
				{
					index = 0;
					break;
				}
			}
        }
		fclose(fp);
    }
	fp = fopen("text.txt", "a+");
    fprintf(fp, "id:%d\tname:%s\tpass:%s\t\n", id, user, password);
    fclose(fp);

    system("cls");
    welcometogame();
    

}

int YanZheng()
{
    FILE* fp = NULL;
    fp = fopen("text.txt", "r");

    char line[100];
    char inputUser[8], inputPass[8];
    char user[8], password[8];
    int id = 0;
    printf("账号：");
    scanf("%7s", inputUser);
    printf("密码：");
    scanf("%7s", inputPass);

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line, "id:%d\tname:%7s\tpass:%7s",&id, user, password) == 3)
        {
            if (strcmp(inputUser, user) == 0 && strcmp(inputPass, password) == 0)
            {
                strcpy(u_name, inputUser);
                ID = id;
                //printf("%s\n", u_name);
                fclose(fp);
                printf("验证成功，准备进入游戏！");
                system("pause");
                return 1;
            }
            /*printf("%s----%s\n",inputUser,user);
            printf("%s----%s", inputPass, password);*/
        }
    }
    printf("不存在账号，请先注册或重新确认账号！");
    fclose(fp);
    return 0;

}


void Pos(int x, int y)//设置光标位置
{
    COORD pos;
    HANDLE hOutput;
    pos.X = x;
    pos.Y = y;
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOutput, pos);
}

void Login()
{

    char buf[64];
    struct tm tms = *localtime(&startTime);
    strftime(buf, sizeof(buf), "开始时间:%Y-%m-%d %H:%M:%S", &tms);

    time_t nowTime = time(NULL);
    int pass = (int)(nowTime - startTime);
    int hours = pass / 3600;
	int min = (pass % 3600) / 60;
    int sec = pass % 60;
    if (logView == 1)
    {
        Pos(64, 2);
        printf("用户名：%s", u_name);
        Pos(64, 3);
        printf("用户ID：%d", ID);
        Pos(64, 4);
        printf("得分：%d", score);
        Pos(64, 5);
        printf("%s", buf);
        Pos(64, 6);
        printf("游戏时长：%02d:%02d:%02d", hours, min, sec);
    }
    else if (logView == 0)
    {
        for (int i = 2;i <= 6;i++)
        {
            Pos(64, i);
            printf("                       ");
        }
    }

    if (logView == 2)
    {
        FILE* fp = NULL;
		fp = fopen("login.txt", "a+");
        if (fp == NULL)
        {
            printf("文件打开失败，无法存入日志");
            return;
        }
		fprintf(fp, "用户名：%s\n", u_name);
		fprintf(fp, "用户ID：%d\n", ID);
		fprintf(fp, "得分：%d\n", score);
		fprintf(fp, "%s\n", buf);
		fprintf(fp, "游戏时长：%02d:%02d:%02d\n", hours, min, sec);
		fprintf(fp, "-----------------------------------\n");
		fclose(fp);
        Pos(24, 14);
		printf("日志已存入login.txt文件中");
		system("pause");



    }
    
}

void creatMap()//创建地图
{
    int i;
    for (i = 0; i < 58; i += 2)//打印上下边框
    {
        Pos(i, 0);
        printf("■");
        Pos(i, 26);
        printf("■");
    }
    for (i = 1; i < 26; i++)//打印左右边框
    {
        Pos(0, i);
        printf("■");
        Pos(56, i);
        printf("■");
    }
}

void initsnake()//初始化蛇身
{
    snake* tail;
    int i;
    tail = (snake*)malloc(sizeof(snake));//从蛇尾开始，头插法，以x,y设定开始的位置//
    tail->x = 24;
    tail->y = 5;
    tail->next = NULL;
    for (i = 1; i <= 4; i++)
    {
        head = (snake*)malloc(sizeof(snake));
        head->next = tail;
        head->x = 24 + 2 * i;
        head->y = 5;
        tail = head;
    }
    while (tail != NULL)//从头到为，输出蛇身
    {
        Pos(tail->x, tail->y);
        printf("■");
        tail = tail->next;
    }
}

int biteself()//判断是否咬到了自己
{
    snake* self;
    self = head->next;
    while (self != NULL)
    {
        if (self->x == head->x && self->y == head->y)
        {
            return 1;
        }
        self = self->next;
    }
    return 0;
}

void createfood()//随机出现食物
{
    snake* food_1;
    srand((unsigned)time(NULL));
    food_1 = (snake*)malloc(sizeof(snake));
    while ((food_1->x % 2) != 0 || food_1->x < 2 || food_1->x > 53 || food_1->y < 1 || food_1->y > 25)    //保证其为偶数，使得食物能与蛇头对其
    {
        food_1->x = rand() % 52 + 2;
        food_1->y = rand() % 24 + 1;
    }

    q = head;
    while (q->next != NULL)
    {
        if (q->x == food_1->x && q->y == food_1->y) //判断蛇身是否与食物重合
        {
            free(food_1);
            createfood();
        }
        q = q->next;
    }
    Pos(food_1->x, food_1->y);
    food = food_1;
    printf("■");

}

void cantcrosswall()//不能穿墙
{
    if (head->x == 0 || head->x == 56 || head->y == 0 || head->y == 26)
    {
        endgamestatus = 1;
        endgame();
    }
}

void snakemove()//蛇前进,上U,下D,左L,右R
{
    snake* nexthead;
    cantcrosswall();

    nexthead = (snake*)malloc(sizeof(snake));
    if (status == U)
    {
        nexthead->x = head->x;
        nexthead->y = head->y - 1;
        if (nexthead->x == food->x && nexthead->y == food->y)//如果下一个有食物//
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q != NULL)
            {
                Pos(q->x, q->y);
                printf("■");
                q = q->next;
            }
            score = score + add;
            createfood();
        }
        else                                               //如果没有食物//
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q->next->next != NULL)
            {
                Pos(q->x, q->y);
                printf("■");
                q = q->next;
            }
            Pos(q->next->x, q->next->y);
            printf("  ");
            free(q->next);
            q->next = NULL;
        }
    }
    if (status == D)
    {
        nexthead->x = head->x;
        nexthead->y = head->y + 1;
        if (nexthead->x == food->x && nexthead->y == food->y)  //有食物
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q != NULL)
            {
                Pos(q->x, q->y);
                printf("■");
                q = q->next;
            }
            score = score + add;
            createfood();
        }
        else                               //没有食物
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q->next->next != NULL)
            {
                Pos(q->x, q->y);
                printf("■");
                q = q->next;
            }
            Pos(q->next->x, q->next->y);
            printf("  ");
            free(q->next);
            q->next = NULL;
        }
    }
    if (status == L)
    {
        nexthead->x = head->x - 2;
        nexthead->y = head->y;
        if (nexthead->x == food->x && nexthead->y == food->y)//有食物
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q != NULL)
            {
                Pos(q->x, q->y);
                printf("■");
                q = q->next;
            }
            score = score + add;
            createfood();
        }
        else                                //没有食物
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q->next->next != NULL)
            {
                Pos(q->x, q->y);
                printf("■");
                q = q->next;
            }
            Pos(q->next->x, q->next->y);
            printf("  ");
            free(q->next);
            q->next = NULL;
        }
    }
    if (status == R)
    {
        nexthead->x = head->x + 2;
        nexthead->y = head->y;
        if (nexthead->x == food->x && nexthead->y == food->y)//有食物
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q != NULL)
            {
                Pos(q->x, q->y);
                printf("■");
                q = q->next;
            }
            score = score + add;
            createfood();
        }
        else                                         //没有食物
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q->next->next != NULL)
            {
                Pos(q->x, q->y);
                printf("■");
                q = q->next;
            }
            Pos(q->next->x, q->next->y);
            printf("  ");
            free(q->next);
            q->next = NULL;
        }
    }
    if (biteself() == 1)       //判断是否会咬到自己
    {
        endgamestatus = 2;
        endgame();
    }
}

void pause()//暂停
{
    while (1)
    {
        Sleep(300);
        if (GetAsyncKeyState(VK_SPACE))
        {
            break;
        }

    }
}

void gamecircle()//控制游戏        
{
    Pos(64, 1);
    printf("---按F5显示游戏用户日志---");
    Pos(64, 14);
    printf("*** %s 正在游戏中！***", u_name);
    Pos(64, 23);
    printf("不能穿墙，不能咬到自己\n");
    Pos(64, 24);
    printf("用↑.↓.←.→分别控制蛇的移动.");
    Pos(64, 25);
    printf("F1 为加速，F2 为减速\n");
    Pos(64, 26);
    printf("ESC ：退出游戏.space：暂停游戏.");
    Pos(64, 28);
    status = R;
    while (1)
    {
        Pos(64, 18);
        printf("得分：%d  ", score);
        Pos(64, 19);
        printf("每个食物得分：%d分", add);
        if (GetAsyncKeyState(VK_UP) && status != D)
        {
            status = U;
        }
        else if (GetAsyncKeyState(VK_DOWN) && status != U)
        {
            status = D;
        }
        else if (GetAsyncKeyState(VK_LEFT) && status != R)
        {
            status = L;
        }
        else if (GetAsyncKeyState(VK_RIGHT) && status != L)
        {
            status = R;
        }
        else if (GetAsyncKeyState(VK_SPACE))
        {
            pause();
        }
        else if (GetAsyncKeyState(VK_ESCAPE))
        {
            endgamestatus = 3;
            break;
        }
        else if (GetAsyncKeyState(VK_F1))
        {
            if (sleeptime >= 50)
            {
                sleeptime = sleeptime - 30;
                add = add + 2;
                if (sleeptime == 320)
                {
                    add = 2;//防止减到1之后再加回来有错
                }
            }
        }
        else if (GetAsyncKeyState(VK_F2))
        {
            if (sleeptime < 350)
            {
                sleeptime = sleeptime + 30;
                add = add - 2;
                if (sleeptime == 350)
                {
                    add = 1;  //保证最低分为1
                }
            }
        }
        else if (GetAsyncKeyState(VK_F5) & 1)
        {
            logView = !logView;
        }
        Login();
        Sleep(sleeptime);
        snakemove();
    }
}

void welcometogame()//开始界面
{
    int n;
    int tmp;

    Pos(40, 12);
    printf("欢迎来到贪食蛇游戏！");
    Pos(40, 25);
    system("pause");
    system("cls");
    Pos(25, 10);
    printf("1.注册");
    Pos(25, 11);
    printf("2.开始游戏");
    Pos(25, 12);
    printf("用↑.↓.←.→分别控制蛇的移动， F1 为加速，2 为减速\n");
    Pos(25, 13);
    printf("加速将能得到更高的分数。\n");
    //system("pause");
    printf("请输入接下来的操作");
    scanf_s("%d", &n);
    switch (n)
    {
    case 1:
        zhuce();
        return;
    case 2:
        system("cls");
        printf("游玩前请先输入账号密码进行验证：");
        tmp = YanZheng();
        if (tmp == 1)
        {
            system("cls");
            return;
        }
        else
        {
            welcometogame();
            return;
        }
    }

    //system("cls");
}

void endgame()//结束游戏
{
    system("cls");
    Pos(24, 12);
    if (endgamestatus == 1)
    {
        printf("对不起，您撞到墙了。游戏结束!");
    }
    else if (endgamestatus == 2)
    {
        printf("对不起，您咬到自己了。游戏结束!");
    }
    else if (endgamestatus == 3)
    {
        printf("您已经结束了游戏。");
    }
    Pos(24, 13);
    printf("您的得分是%d\n", score);
    logView = 2;
    Login();

    exit(0);
}

void gamestart()//游戏初始化
{
    system("mode con cols=100 lines=30");
    welcometogame();
    creatMap();
    initsnake();
    createfood();
    startTime = time(NULL);

}

int main()
{
    gamestart();
    gamecircle();
    endgame();
    return 0;
}
