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
#define R 4       //�ߵ�״̬��U���� ��D���£�L:�� R����

typedef struct SNAKE //�����һ���ڵ�
{
    int x;
    int y;
    struct SNAKE* next;
}snake;

//ȫ�ֱ���//
int score = 0, add = 10;        //�ܵ÷���ÿ�γ�ʳ��÷֡�
int status, sleeptime = 200;        //ÿ�����е�ʱ����
snake* head, * food;        //��ͷָ�룬ʳ��ָ��
snake* q;       //�����ߵ�ʱ���õ���ָ��
int endgamestatus = 0;      //��Ϸ�����������1��ײ��ǽ��2��ҧ���Լ���3�������˳���Ϸ��
char u_name[8];         //����������û�����
int ID = 0;         //����������û�ID
int logView = 0;        //��־�л�
//int logText = 0;        //��־�ı�
time_t startTime = 0;       //��ʼʱ��
//time_t endTime = 0;         //��Ϸ����ʱ��
//int ganmeTime = 0;        //��Ϸʱ��(��)

//����ȫ������//
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

void zhuce();       //ע�ắ��
int YanZheng();     //��֤����
void Login();       //��־


void zhuce()
{
    FILE* fp = NULL;
	char line[100]; //�洢ÿһ��
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
            printf("�ļ��򿪳ɹ�");
        }

        system("pause");
        system("cls");

        printf("�����˺ţ�");
        scanf("%7s", user);

		//�����˺��Ƿ����
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
            printf("�˺��Ѵ��ڣ����������룡\n");
            continue;
        }
        else
        {
            break;
        }
    }
    printf("�������룺");
    scanf("%7s", password);


    //����idΨһ��
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
    printf("�˺ţ�");
    scanf("%7s", inputUser);
    printf("���룺");
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
                printf("��֤�ɹ���׼��������Ϸ��");
                system("pause");
                return 1;
            }
            /*printf("%s----%s\n",inputUser,user);
            printf("%s----%s", inputPass, password);*/
        }
    }
    printf("�������˺ţ�����ע�������ȷ���˺ţ�");
    fclose(fp);
    return 0;

}


void Pos(int x, int y)//���ù��λ��
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
    strftime(buf, sizeof(buf), "��ʼʱ��:%Y-%m-%d %H:%M:%S", &tms);

    time_t nowTime = time(NULL);
    int pass = (int)(nowTime - startTime);
    int hours = pass / 3600;
	int min = (pass % 3600) / 60;
    int sec = pass % 60;
    if (logView == 1)
    {
        Pos(64, 2);
        printf("�û�����%s", u_name);
        Pos(64, 3);
        printf("�û�ID��%d", ID);
        Pos(64, 4);
        printf("�÷֣�%d", score);
        Pos(64, 5);
        printf("%s", buf);
        Pos(64, 6);
        printf("��Ϸʱ����%02d:%02d:%02d", hours, min, sec);
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
            printf("�ļ���ʧ�ܣ��޷�������־");
            return;
        }
		fprintf(fp, "�û�����%s\n", u_name);
		fprintf(fp, "�û�ID��%d\n", ID);
		fprintf(fp, "�÷֣�%d\n", score);
		fprintf(fp, "%s\n", buf);
		fprintf(fp, "��Ϸʱ����%02d:%02d:%02d\n", hours, min, sec);
		fprintf(fp, "-----------------------------------\n");
		fclose(fp);
        Pos(24, 14);
		printf("��־�Ѵ���login.txt�ļ���");
		system("pause");



    }
    
}

void creatMap()//������ͼ
{
    int i;
    for (i = 0; i < 58; i += 2)//��ӡ���±߿�
    {
        Pos(i, 0);
        printf("��");
        Pos(i, 26);
        printf("��");
    }
    for (i = 1; i < 26; i++)//��ӡ���ұ߿�
    {
        Pos(0, i);
        printf("��");
        Pos(56, i);
        printf("��");
    }
}

void initsnake()//��ʼ������
{
    snake* tail;
    int i;
    tail = (snake*)malloc(sizeof(snake));//����β��ʼ��ͷ�巨����x,y�趨��ʼ��λ��//
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
    while (tail != NULL)//��ͷ��Ϊ���������
    {
        Pos(tail->x, tail->y);
        printf("��");
        tail = tail->next;
    }
}

int biteself()//�ж��Ƿ�ҧ�����Լ�
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

void createfood()//�������ʳ��
{
    snake* food_1;
    srand((unsigned)time(NULL));
    food_1 = (snake*)malloc(sizeof(snake));
    while ((food_1->x % 2) != 0 || food_1->x < 2 || food_1->x > 53 || food_1->y < 1 || food_1->y > 25)    //��֤��Ϊż����ʹ��ʳ��������ͷ����
    {
        food_1->x = rand() % 52 + 2;
        food_1->y = rand() % 24 + 1;
    }

    q = head;
    while (q->next != NULL)
    {
        if (q->x == food_1->x && q->y == food_1->y) //�ж������Ƿ���ʳ���غ�
        {
            free(food_1);
            createfood();
        }
        q = q->next;
    }
    Pos(food_1->x, food_1->y);
    food = food_1;
    printf("��");

}

void cantcrosswall()//���ܴ�ǽ
{
    if (head->x == 0 || head->x == 56 || head->y == 0 || head->y == 26)
    {
        endgamestatus = 1;
        endgame();
    }
}

void snakemove()//��ǰ��,��U,��D,��L,��R
{
    snake* nexthead;
    cantcrosswall();

    nexthead = (snake*)malloc(sizeof(snake));
    if (status == U)
    {
        nexthead->x = head->x;
        nexthead->y = head->y - 1;
        if (nexthead->x == food->x && nexthead->y == food->y)//�����һ����ʳ��//
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q != NULL)
            {
                Pos(q->x, q->y);
                printf("��");
                q = q->next;
            }
            score = score + add;
            createfood();
        }
        else                                               //���û��ʳ��//
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q->next->next != NULL)
            {
                Pos(q->x, q->y);
                printf("��");
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
        if (nexthead->x == food->x && nexthead->y == food->y)  //��ʳ��
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q != NULL)
            {
                Pos(q->x, q->y);
                printf("��");
                q = q->next;
            }
            score = score + add;
            createfood();
        }
        else                               //û��ʳ��
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q->next->next != NULL)
            {
                Pos(q->x, q->y);
                printf("��");
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
        if (nexthead->x == food->x && nexthead->y == food->y)//��ʳ��
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q != NULL)
            {
                Pos(q->x, q->y);
                printf("��");
                q = q->next;
            }
            score = score + add;
            createfood();
        }
        else                                //û��ʳ��
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q->next->next != NULL)
            {
                Pos(q->x, q->y);
                printf("��");
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
        if (nexthead->x == food->x && nexthead->y == food->y)//��ʳ��
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q != NULL)
            {
                Pos(q->x, q->y);
                printf("��");
                q = q->next;
            }
            score = score + add;
            createfood();
        }
        else                                         //û��ʳ��
        {
            nexthead->next = head;
            head = nexthead;
            q = head;
            while (q->next->next != NULL)
            {
                Pos(q->x, q->y);
                printf("��");
                q = q->next;
            }
            Pos(q->next->x, q->next->y);
            printf("  ");
            free(q->next);
            q->next = NULL;
        }
    }
    if (biteself() == 1)       //�ж��Ƿ��ҧ���Լ�
    {
        endgamestatus = 2;
        endgame();
    }
}

void pause()//��ͣ
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

void gamecircle()//������Ϸ        
{
    Pos(64, 1);
    printf("---��F5��ʾ��Ϸ�û���־---");
    Pos(64, 14);
    printf("*** %s ������Ϸ�У�***", u_name);
    Pos(64, 23);
    printf("���ܴ�ǽ������ҧ���Լ�\n");
    Pos(64, 24);
    printf("�á�.��.��.���ֱ�����ߵ��ƶ�.");
    Pos(64, 25);
    printf("F1 Ϊ���٣�F2 Ϊ����\n");
    Pos(64, 26);
    printf("ESC ���˳���Ϸ.space����ͣ��Ϸ.");
    Pos(64, 28);
    status = R;
    while (1)
    {
        Pos(64, 18);
        printf("�÷֣�%d  ", score);
        Pos(64, 19);
        printf("ÿ��ʳ��÷֣�%d��", add);
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
                    add = 2;//��ֹ����1֮���ټӻ����д�
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
                    add = 1;  //��֤��ͷ�Ϊ1
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

void welcometogame()//��ʼ����
{
    int n;
    int tmp;

    Pos(40, 12);
    printf("��ӭ����̰ʳ����Ϸ��");
    Pos(40, 25);
    system("pause");
    system("cls");
    Pos(25, 10);
    printf("1.ע��");
    Pos(25, 11);
    printf("2.��ʼ��Ϸ");
    Pos(25, 12);
    printf("�á�.��.��.���ֱ�����ߵ��ƶ��� F1 Ϊ���٣�2 Ϊ����\n");
    Pos(25, 13);
    printf("���ٽ��ܵõ����ߵķ�����\n");
    //system("pause");
    printf("������������Ĳ���");
    scanf_s("%d", &n);
    switch (n)
    {
    case 1:
        zhuce();
        return;
    case 2:
        system("cls");
        printf("����ǰ���������˺����������֤��");
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

void endgame()//������Ϸ
{
    system("cls");
    Pos(24, 12);
    if (endgamestatus == 1)
    {
        printf("�Բ�����ײ��ǽ�ˡ���Ϸ����!");
    }
    else if (endgamestatus == 2)
    {
        printf("�Բ�����ҧ���Լ��ˡ���Ϸ����!");
    }
    else if (endgamestatus == 3)
    {
        printf("���Ѿ���������Ϸ��");
    }
    Pos(24, 13);
    printf("���ĵ÷���%d\n", score);
    logView = 2;
    Login();

    exit(0);
}

void gamestart()//��Ϸ��ʼ��
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
