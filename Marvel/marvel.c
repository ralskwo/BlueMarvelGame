#define _CRT_SECURE_NO_WARNINGS

// ���ӿ� ���������� �ʿ��� ���, �Լ� ���� ������ ������ ��.

#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <string.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")
#include <mmsystem.h>
#include "Digitalv.h"

int dwID;
MCI_PLAY_PARMS mciPlay; // MCI_PLAY_PARAMS ����ü ����
MCI_OPEN_PARMS mciOpen; // MCI_OPEN_PARAMS ����ü ����

// ���� ����
#define BLACK 0
#define BLUE1 1
#define GREEN1 2
#define CYAN1 3
#define RED1 4
#define MAGENTA1 5
#define YELLOW1 6
#define GRAY1 7
#define GRAY2 8
#define BLUE2 9
#define GREEN2 10
#define CYAN2 11
#define RED2 12
#define MAGENTA2 13
#define YELLOW2 14
#define WHITE 15

// ���� �������� ����

typedef struct
{
	int x;
	int y;
	char city_name[20];
	int owner;
	int building;
	int price;
} CityInfo;

typedef struct
{
	int spot;
	int money;
	char myknight[2];
	int knightnum;
	int stop_move; // ���ε� ���� ��
	char textcolor1, textcolor2;
	int getout; //���ε� Ż���
} Player;

void gotoxy(int x, int y) // Ŀ�� �����̵�
{
	COORD spot; // ������ ����� ����
	spot.X = x;
	spot.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), spot);
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

void removeCursor(void)
{ // Ŀ�� �����
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void cls(int bg_color, int text_color) // ȭ�� ����
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);
}

// ���������� ���� ������ ����!

//�� �׸���
void make_map(int x1, int y1, int x2, int y2, char* ch)
{
	int x, y;
	char cmd[100];

	sprintf(cmd, "mode con cols=%d lines=%d", 122, 35);
	cls(WHITE, BLACK);
	system(cmd);

	// �� ������
	for (x = x1; x <= x2; x = x + 2)
	{
		gotoxy(x, y1);
		textcolor(GREEN1, BLACK);
		printf("%s", ch);
		textcolor(BLACK, WHITE);
		gotoxy(x, 5);
		printf("��");
		if (x < 12 || x > 108)
		{
			gotoxy(x, 10);
			printf("��");
			gotoxy(x, 15);
			printf("��");
		}
		gotoxy(x, y2 - 5);
		printf("��");
		gotoxy(x, y2);
		textcolor(GREEN1, BLACK);
		printf("%s", ch);
		textcolor(BLACK, WHITE);
	}
	// �� ������
	for (y = y1; y <= y2; y++)
	{
		gotoxy(x1, y);
		textcolor(GREEN1, BLACK);
		printf("%s", ch);
		textcolor(BLACK, WHITE);

		if (y > 0 && y < 25)
		{
			if (y <= 5 || y >= 20)
			{
				for (x = 14; x <= x2; x = x + 2)
				{
					if (x % 12 == 0)
					{
						gotoxy(x, y);
						if (y == 5)
							printf("��");
						else if (y == 20)
							printf("��");
						else
							printf("��");
					}
				}
			}

			gotoxy(12, y);
			if (y == 5 || y == 20)
				printf("��");
			else if (y == 10 || y == 15)
				printf("��");
			else
				printf("��");
			gotoxy(x2 - 12, y);
			if (y == 5 || y == 20)
				printf("��");
			else if (y == 10 || y == 15)
				printf("��");
			else
				printf("��");
		}
		gotoxy(x2, y);
		textcolor(GREEN1, BLACK);
		printf("%s", ch);
		textcolor(BLACK, WHITE);
	}
}

//�� ������ ���� ����
void init_city(CityInfo city[])
{
	int i, k;
	char arr_city[26][20] = { {"���"}, {"Ÿ������"}, {"���Ҷ�"}, {"���ֵ�"}, {"�̰�����"}, {"ī�̷�"}, {"�̽�ź��"}, {"���׳�"}, {"�����ϰ�"}, {"���ε�"}, {"����"}, {"������"}, {"��Ÿ��"}, {"�����"}, {"����"}, {"���Ŀ��"}, {"�õ��"}, {"�λ�"}, {"Ȳ�ݿ���"}, {"������"}, {"���帮��"}, {"�ĸ�"}, {"��ǳ"}, {"����"}, {"����"}, {"����"} };
	int arr_cityprice[26] = { 0, 5, 8, 10, 12, 14, 16, 18, 20, 0, 22, 24, 26, 0, 28, 30, 32, 34, 0, 38, 40, 42, 0, 44, 45, 100 };

	for (i = 0, k = 0; i < 10; i++)
	{
		city[i].x = k + 2;
		city[i].y = 2;
		k = k + 12;
		gotoxy(city[i].x, city[i].y - 1);
		strcpy(city[i].city_name, arr_city[i]);
		printf("%s", city[i].city_name);
		city[i].owner = 0;
		city[i].building = 0;
		city[i].price = arr_cityprice[i] * 1000;
	}
	for (i = 10, k = 0; i < 14; i++)
	{
		city[i].x = 110;
		city[i].y = k + 7;
		k = k + 5;
		gotoxy(city[i].x, city[i].y - 1);
		strcpy(city[i].city_name, arr_city[i]);
		printf("%s", city[i].city_name);
		city[i].owner = 0;
		city[i].building = 0;
		city[i].price = arr_cityprice[i] * 1000;
	}
	for (i = 14, k = 0; i < 23; i++)
	{
		city[i].x = 98 - k;
		city[i].y = 22;
		k = k + 12;
		gotoxy(city[i].x, city[i].y - 1);
		strcpy(city[i].city_name, arr_city[i]);
		printf("%s", city[i].city_name);
		city[i].owner = 0;
		city[i].building = 0;
		city[i].price = arr_cityprice[i] * 1000;
	}
	for (i = 23, k = 0; i < 26; i++)
	{
		city[i].x = 2;
		city[i].y = 17 - k;
		k = k + 5;
		gotoxy(city[i].x, city[i].y - 1);
		strcpy(city[i].city_name, arr_city[i]);
		printf("%s", city[i].city_name);
		city[i].owner = 0;
		city[i].building = 0;
		city[i].price = arr_cityprice[i] * 1000;
	}
}

//�÷��̾� �ʱ�ȭ
void init_player(Player* s1, Player* s2)
{
	s1->money = 300000;
	s2->money = 300000;
	s1->spot = 0;
	s2->spot = 0;
	s1->knightnum = 1;
	s2->knightnum = 2;
	s1->stop_move = 0;
	s2->stop_move = 0;
	s1->textcolor1 = BLUE2; //MAGENTA1
	s1->textcolor2 = CYAN2; //CYAN2
	s2->textcolor1 = RED2;
	s2->textcolor2 = GREEN2;
	s1->getout = 0;
	s2->getout = 0;
	gotoxy(0, 26);
	printf("ù��° �÷��̾��� �� ����� �Է��ϼ��� : ");
	scanf("%s", s1->myknight);
	gotoxy(0, 26);
	printf("									");
	gotoxy(0, 26);
	printf("�ι�° �÷��̾��� �� ����� �Է��ϼ��� : ");
	scanf("%s", s2->myknight);
	gotoxy(0, 26);
	printf("									");
	gotoxy(2, 4);
	printf("%s %s", s1->myknight, s2->myknight);
}

// ���� �ݾ��� ���ΰ�?

void howmoney(Player* player1, Player* player2)
{
	gotoxy(14, 18);
	textcolor(player1->textcolor1, player1->textcolor2);
	printf("��");
	textcolor(BLACK, WHITE);
	printf("Player1 %s �� : ", player1->myknight);
	textcolor(player1->textcolor1, WHITE);
	printf("%d��", player1->money);
	textcolor(BLACK, WHITE);
	gotoxy(14, 19);
	textcolor(player2->textcolor1, player2->textcolor2);
	printf("��");
	textcolor(BLACK, WHITE);
	printf("Player2 %s �� : ", player2->myknight);
	textcolor(player2->textcolor1, WHITE);
	printf("%d��", player2->money);
	textcolor(BLACK, WHITE);
}

void clear_downpart()
{
	gotoxy(0, 26);
	printf("                                                                                                                         \n");
	printf("                                                                                                                         \n");
	printf("                                                                                                                         \n");
	printf("                                                                                                                         \n");
	printf("                                                                                                                         \n");
	printf("                                                                                                                         \n");
	printf("                                                                                                                         \n");
	printf("                                                                                                                         \n");
	printf("                                                                                                                         ");
} // ���찳 ����

//�� �̵� �Լ�
void move_player(CityInfo city[], int n, Player* pl, Player* another_pl) // ���� ��ǥ��, ������ ����, ������ �÷��̾�
{
	int i;
	char destination[20];

	for (i = 0; i < n; i++)
	{
		gotoxy(city[(pl->spot) % 26].x, city[(pl->spot) % 26].y + 2);
		if (pl->spot % 26 == another_pl->spot % 26)
			printf("%s    ", another_pl->myknight);
		else
			printf("  "); // ���� ��ġ ����� �߿� !!!!!!!!!!!!!!!!!!
		pl->spot++;
		gotoxy(city[(pl->spot) % 26].x, city[(pl->spot) % 26].y + 2);
		if (pl->spot % 26 == another_pl->spot % 26) // ��ġ�� �ȴٸ�
			printf("%s %s", pl->myknight, another_pl->myknight);
		else
			printf("%s", pl->myknight);
		Sleep(300);
	}

	if (pl->spot == 13)
	{ //�����
	RE:
		gotoxy(0, 26);
		printf("                                                                   ");
		gotoxy(0, 26);
		printf("����⸦ �����ϴ�. ���� ���� �Է��ϼ���.");
		scanf("%s", destination);
		gotoxy(0, 27);
		printf("                                                                   ");
		if (!strcmp(destination, "  �����"))
		{ // �ѹ��� ���� �ٽ� ����� ����
			gotoxy(0, 28);
			printf("����⸦ Ÿ�� �ѹ��� ���°� ����մϴ�!");
			_getch();
			gotoxy(0, 28);
			printf("                                                                   ");
			goto RE; // goto �Լ��� ����ؾ� ���ѷ����� ����!
		}
		for (i = 0; i < 26; i++)
		{
			if (!strcmp(destination, city[i].city_name))
			{
				if (pl->spot < i)
					move_player(city, i - (pl->spot), pl, another_pl); //  ����� ��������
				else
					move_player(city, 26 - (pl->spot) + i, pl, another_pl); //  ����� ��������
				break;
			}
		}
		if (i == 26)
		{
			gotoxy(0, 28);
			printf("�������� �߸� �Է��� ����⸦ ���ƽ��ϴ�. ���͸� ������ ���� ����Ⱑ �����մϴ�.");
			_getch();
			gotoxy(0, 28);
			printf("                                                                                         ");
			goto RE;
		}
		gotoxy(0, 26);
		printf("����⸦ Ÿ�� %s�� �Խ��ϴ�. ����Ϸ��� ����Ű�� ��������.", city[pl->spot % 26].city_name);
		removeCursor();
	}
}

// Ȳ�ݿ���

void goldkey(Player* pl, Player* another_pl, CityInfo city[])
{
	int event_num;
	srand(time(NULL));

	event_num = rand() % 100;
	if (event_num < 25)
	{
		gotoxy(0, 27);
		printf(" �뵷���� 5000���� �޾ҽ��ϴ� �Ǥ̤� ");
		_getch();
		pl->money += 5000;
		gotoxy(0, 27);
		printf("                                                                           ");
	}
	else if (event_num >= 25 && event_num < 40)
	{
		gotoxy(0, 27);
		printf("���ǿ� ��÷\n");
		_getch();
		printf("�Ǵ� �����̾����ϴ�. �ƹ��ϵ� �Ͼ�� �ʾҽ��ϴ�");
		_getch();
		gotoxy(0, 27);
		printf("                                 \n                                                                           ");
	}
	else if (event_num >= 40 && event_num < 65)
	{
		gotoxy(0, 27);
		printf("�������� ���� ������ ������ϴ� 8000���� �ҽ��ϴ�.");
		_getch();
		pl->money -= 8000;
		gotoxy(0, 27);
		printf("                                                                                 ");
	}
	else if (event_num >= 65 && event_num < 88)
	{
		gotoxy(0, 27);
		printf("���ε����� Ż���� �� �ִ� ���ε� Ż��� ȹ��!");
		pl->getout++;
		_getch();
	}
	else
	{
		gotoxy(0, 27);
		printf("���ε��� ����Ǿ����ϴ�");
		_getch();
		move_player(city, 17, pl, another_pl);
		pl->stop_move = 3;
		gotoxy(0, 27);
		printf("                                                                           ");
	}
}

//����ģ �ָ� ����� < ����ε� ó�� >
void dicemotion_clear()
{
	int i;
	for (i = 6; i < 20; i++)
	{
		gotoxy(76, i);
		printf("               ");
	}
}

//�ָ� ����ġ��
void dicemotion()
{
	int high;

	for (high = 6; high < 14; high++)
	{ //�ָ� �������� ���
		gotoxy(76, high);
		printf("  ������������ ");
		gotoxy(76, high + 1);
		printf("������   ������");
		gotoxy(76, high + 2);
		printf("����    ������");
		gotoxy(76, high + 3);
		printf("����    ������");
		gotoxy(76, high + 4);
		printf("����    ������");
		gotoxy(76, high + 5);
		printf("������������������");
		Sleep(20);
		if (high == 13)
			break;
		dicemotion_clear();
	}
}

//�ֻ��� ���
void dice(int n)
{
	textcolor(YELLOW1, WHITE);
	if (n == 1)
	{
		gotoxy(94, 12);
		printf("�ɢǢǢǢǢǢ�\n");
		gotoxy(94, 13);
		printf("��    ��    ��\n");
		gotoxy(94, 14);
		printf("��    ��    ��\n");
		gotoxy(94, 15);
		printf("��    ��    ��\n");
		gotoxy(94, 16);
		printf("��    ��    ��\n");
		gotoxy(94, 17);
		printf("��    ��    ��\n");
		gotoxy(94, 18);
		printf("�ʢǢǢǢǢǢ�\n");
	}
	else if (n == 2)
	{
		gotoxy(94, 12);
		printf("�ɢǢǢǢǢǢ�\n");
		gotoxy(94, 13);
		printf("��  �ââ�  ��\n");
		gotoxy(94, 14);
		printf("��      ��  ��\n");
		gotoxy(94, 15);
		printf("��  �ââ�  ��\n");
		gotoxy(94, 16);
		printf("��  ��      ��\n");
		gotoxy(94, 17);
		printf("��  �ââ�  ��\n");
		gotoxy(94, 18);
		printf("�ʢǢǢǢǢǢ�\n");
	}
	else if (n == 3)
	{
		gotoxy(94, 12);
		printf("�ɢǢǢǢǢǢ�\n");
		gotoxy(94, 13);
		printf("��  �ââ�  ��\n");
		gotoxy(94, 14);
		printf("��      ��  ��\n");
		gotoxy(94, 15);
		printf("��  �ââ�  ��\n");
		gotoxy(94, 16);
		printf("��      ��  ��\n");
		gotoxy(94, 17);
		printf("��  �ââ�  ��\n");
		gotoxy(94, 18);
		printf("�ʢǢǢǢǢǢ�\n");
	}
	else if (n == 4)
	{
		gotoxy(94, 12);
		printf("�ɢǢǢǢǢǢ�\n");
		gotoxy(94, 13);
		printf("��  ��  ��  ��\n");
		gotoxy(94, 14);
		printf("��  ��  ��  ��\n");
		gotoxy(94, 15);
		printf("��  �ââ�  ��\n");
		gotoxy(94, 16);
		printf("��      ��  ��\n");
		gotoxy(94, 17);
		printf("��      ��  ��\n");
		gotoxy(94, 18);
		printf("�ʢǢǢǢǢǢ�\n");
	}
	else if (n == 5)
	{
		gotoxy(94, 12);
		printf("�ɢǢǢǢǢǢ�\n");
		gotoxy(94, 13);
		printf("��  �ââ�  ��\n");
		gotoxy(94, 14);
		printf("��  ��      ��\n");
		gotoxy(94, 15);
		printf("��  �ââ�  ��\n");
		gotoxy(94, 16);
		printf("��      ��  ��\n");
		gotoxy(94, 17);
		printf("��  �ââ�  ��\n");
		gotoxy(94, 18);
		printf("�ʢǢǢǢǢǢ�\n");
	}
	else if (n == 6)
	{
		gotoxy(94, 12);
		printf("�ɢǢǢǢǢǢ�\n");
		gotoxy(94, 13);
		printf("��  �ââ�  ��\n");
		gotoxy(94, 14);
		printf("��  ��      ��\n");
		gotoxy(94, 15);
		printf("��  �ââ�  ��\n");
		gotoxy(94, 16);
		printf("��  ��  ��  ��\n");
		gotoxy(94, 17);
		printf("��  �ââ�  ��\n");
		gotoxy(94, 18);
		printf("�ʢǢǢǢǢǢ�\n");
	}
	textcolor(BLACK, WHITE);
}

//----------------------------------------------------------------------------------------------------------------------------------------
// ���ݺ��ʹ� �÷��� ������ �������� �̺�Ʈ ���ַ� ������ ��

//������ ���� �̺�Ʈ

void sellcity(CityInfo city[], Player* pl) //���� �ȱ�
{
	int i;
	int sellprice;
	int tab = 0;
	char sellcityname[20];

	for (i = 0; i < 26; i++)
	{
		if (city[i].owner == pl->knightnum)
		{
			sellprice = city[i].price + (city[i].building * city[i].price / 2);
			printf("%s(����:%d��)    ", city[i].city_name, sellprice); // �ŸŰ� = ���� + ( �ǹ��� * ����/2 )
			tab++;
			if (tab % 4 == 0)
				printf("\n");
		}
	}

RE:
	printf("\n�� ���� �Է��ϼ���(�Ļ��Է½� �Ļ�) ");
	scanf("%s", sellcityname);
	fflush(stdin);
	if (!strcmp(sellcityname, "�Ļ�"))
	{
		pl->money = -999;
		return;
	}

	for (i = 0; i < 26; i++)
	{
		if (!strcmp(sellcityname, city[i].city_name))
		{
			printf("%s�� �Ȱڽ��ϴ�.", sellcityname);
			_getch();
			pl->money += sellprice;
			city[i].owner = 0;
			city[i].building = 0;
			gotoxy(city[i].x, city[i].y - 1);
			textcolor(BLACK, WHITE);
			printf("%s", city[i].city_name);
			gotoxy(city[i].x, city[i].y);
			printf("       ");
			clear_downpart();
			break;
		}
	}

	if (i == 26)
	{
		printf("�߸� �Է��ϼ̽��ϴ�.");
		_getch();
		goto RE;
	}
}

//���ÿ� �����Ͽ� üũ�� �ϴ� �Լ�

void checkin(CityInfo city[], Player* pl, Player* another_pl)
{
	int k, j;
	if (pl->spot / 26 >= 1)
	{
		gotoxy(0, 26);
		printf("                                                                   ");
		gotoxy(0, 26);
		printf("������ �� �������Ƿ� �����!");
		pl->money += 10000;
		pl->spot = pl->spot % 26;
		_getch();
		if (pl->spot == 0)
			return;
	}
	if (pl->spot == 9)
	{ //���ε�
		gotoxy(0, 27);
		printf("����� ���ε��� �������ϴ�.");
		pl->stop_move = 3;
		_getch();
		return;
	}
	if (pl->spot == 18)
	{
		goldkey(pl, another_pl, city);
		return;
	}
	if (pl->spot == 22)
	{
		gotoxy(0, 26);
		printf("                                                                   ");
		gotoxy(0, 26);
		printf("����ġ ���� ��ǳ�� �Ͼ���ϴ�. �����ϱ� ���� 5000���� �����մϴ�.");
		_getch();
		if (pl->money - 500 < 0)
		{
			int random;
			srand(time(NULL));

			printf("\n������ ���� �����ϴ�. �� ���� ��ǳ�� ���ư��ϴ�.");
			while (1)
			{
				random = rand() % 26;
				if (city[random].owner == pl->knightnum)
				{
					city[random].owner = 0;
					city[random].building = 0;
					gotoxy(city[random].x, city[random].y - 1);
					textcolor(BLACK, WHITE);
					printf("%s", city[random].city_name);
					return;
				}
			}
		}
		pl->money -= 1000;
		return;
	}
	if (city[pl->spot].owner == 0)
	{ // ���� ���� ���� ��
		gotoxy(14, 6);
		printf("*%s�� ���� : %d��*", city[pl->spot].city_name, city[pl->spot].price);
		gotoxy(0, 27);
		printf("%s�� �����ϰڽ��ϱ�? \n", city[pl->spot % 26].city_name);
		printf("1.�����Ѵ� 2. �������� �ʴ´�  �Է� : ");
		scanf("%d", &k);
		fflush(stdin);
		gotoxy(14, 6);
		printf("                                                                   ");
		gotoxy(0, 27);
		printf("                                                                   \n                                                                   ");
		if (k == 1)
		{
			if (pl->money - city[pl->spot].price < 0)
			{
				gotoxy(0, 29);
				printf("���ø� ������ ���� �����ϴ�.");
				_getch();
				gotoxy(0, 29);
				printf("                                   ");
				return;
			}
			city[pl->spot].owner = pl->knightnum;
			pl->money = pl->money - city[pl->spot].price;
			gotoxy(city[pl->spot].x, city[pl->spot].y - 1);
			textcolor(pl->textcolor1, pl->textcolor2); // ������ ���� ǥ��
			printf("%s", city[pl->spot].city_name);
			textcolor(BLACK, WHITE);
		}
		else
			return;
		gotoxy(0, 26);
		printf("                                                                   ");
	}
	else if (city[pl->spot].owner == pl->knightnum)
	{ //���ð� �� �����̸�
		gotoxy(0, 27);
		printf("%s�� �ǹ��� �����ðڽ��ϱ�?                               \n", city[pl->spot].city_name);

		if (city[pl->spot].building == 0)
		{
			printf("1. ���´� 2. ���� �ʴ´�.  �Է� : ");
			gotoxy(14, 6);
			printf("*%s�� �Ǽ��� : %d��*", city[pl->spot].city_name, city[pl->spot].price * 6 / 10);
		}

		else if (city[pl->spot].building == 1)
		{
			printf("1. ���´� 2. ���� �ʴ´�.   �Է� : ");
			gotoxy(14, 6);
			printf("*%s�� �Ǽ��� : %d��*", city[pl->spot].city_name, city[pl->spot].price * 17 / 10);
		}

		else if (city[pl->spot].building == 2)
		{
			printf("1. ���´� 2. ���� �ʴ´�.  �Է� : ");
			gotoxy(14, 6);
			printf("*%s�� �Ǽ��� : %d��*", city[pl->spot].city_name, city[pl->spot].price * 3);
		}

		else
		{
			printf("�� �̻� ���� �� �ִ� �ǹ��� �����ϴ�.\n");
			_getch();
			return;
		}

		gotoxy(30, 28);
		scanf("%d", &j);
		clear_downpart();
		gotoxy(14, 6);
		printf("                                              ");
		gotoxy(0, 26);

		if (j == 1)
		{
			if (city[pl->spot].building == 0)
			{
				if ((pl->money - city[pl->spot].price * 6 / 10) < 0)
				{
					printf("�ǹ��� �� ���� �����ϴ�.\n");
				}

				else
				{
					pl->money = pl->money - city[pl->spot].price * 6 / 10;
					city[pl->spot].building++;
					printf("�� ����, %s���� �ǹ��� %d�� �ֽ��ϴ�.", city[pl->spot].city_name, city[pl->spot].building);
					gotoxy(city[pl->spot].x, city[pl->spot].y);
					printf("��");
				}
			}
			else if (city[pl->spot].building == 1)
			{ // ������ �ΰ� ������������ ����ġ 1.7��
				if ((pl->money - city[pl->spot].price * 17 / 10) < 0)
				{
					printf("�ǹ��� �� ���� �����ϴ�.\n");
				}
				else
				{
					pl->money = pl->money - city[pl->spot].price * 17 / 10;
					city[pl->spot].building++;
					printf("�� ����, %s���� �ǹ��� %d�� �ֽ��ϴ�.", city[pl->spot].city_name, city[pl->spot].building);
					gotoxy(city[pl->spot].x, city[pl->spot].y);
					printf("���");
				}
			}
			else if (city[pl->spot].building == 2)
			{ // ������ ���� ������������ - ����ġ 3��
				if ((pl->money - city[pl->spot].price * 3) < 0)
				{
					printf("�ǹ��� �� ���� �����ϴ�.\n");
				}
				else
				{
					pl->money = pl->money - city[pl->spot].price * 3;
					city[pl->spot].building++;
					printf("���� %s���� �ǹ��� %d�� �ֽ��ϴ�.", city[pl->spot].city_name, city[pl->spot].building);
					gotoxy(city[pl->spot].x, city[pl->spot].y);
					printf("��͢�");
				}
			}
			_getch();
		}
		return;
	}
	else
	{
		gotoxy(0, 27);
		if (city[pl->spot].building == 0)
		{
			if (pl->money - city[pl->spot].price / 10 >= 0)
			{
				pl->money -= city[pl->spot].price / 10;
				another_pl->money += city[pl->spot].price / 10;
				printf("�����(%d��)�� �����Ͽ����ϴ�.", city[pl->spot].price / 10);
			}
			else
			{
				printf("����Ḧ ������ ���� �����ϴ�. ������ ���� �Ⱦƾ��մϴ�.\n");
				_getch();
				sellcity(city, pl);
			}
		}

		else if (city[pl->spot].building == 1)
		{
			if (pl->money - city[pl->spot].price / 2 >= 0)
			{
				pl->money -= city[pl->spot].price / 2;
				another_pl->money += city[pl->spot].price * 4 / 5;
				printf("�����(%d��)�� �����Ͽ����ϴ�.", city[pl->spot].price / 2);
			}
			else
			{
				printf("����Ḧ ������ ���� �����ϴ�. ������ ���� �Ⱦƾ��մϴ�.\n");
				_getch();
				sellcity(city, pl);
			}
		}

		else if (city[pl->spot].building == 2)
		{
			if (pl->money - city[pl->spot].price * 3 / 2 >= 0)
			{
				pl->money -= city[pl->spot].price * 3 / 2;
				another_pl->money += city[pl->spot].price * 3 / 2;
				printf("�����(%d��)�� �����Ͽ����ϴ�.", city[pl->spot].price * 3 / 2);
			}
			else
			{
				printf("����Ḧ ������ ���� �����ϴ�. ������ ���� �Ⱦƾ��մϴ�.\n");
				_getch();
				sellcity(city, pl);
			}
		}
		else if (city[pl->spot].building == 3)
		{
			if (pl->money - city[pl->spot].price * 2 >= 0)
			{
				pl->money -= city[pl->spot].price * 2;
				another_pl->money += city[pl->spot].price * 3;
				printf("�����(%d��)�� �����Ͽ����ϴ�.", city[pl->spot].price * 2);
			}
			else
			{
				printf("����Ḧ ������ ���� �����ϴ�. ������ ���� �Ⱦƾ��մϴ�.\n");
				_getch();
				sellcity(city, pl);
			}
		}
		_getch();
	}
}

// ���� ũ����---------------------------------------------------------------------------------------------------------------------

void winner(Player* pl)
{
	int i, j;
	char cmd[100];
	char winner_message_1[30] = "Thank you for playing.";
	char winner_message_2[50] = "2019,06,12  Hyeok";

	//�Ҹ����
	PlaySound(L"clap.wav", NULL, SND_ASYNC);

	//����
	cls(BLACK, YELLOW2);
	textcolor(YELLOW2, BLACK);
	gotoxy(0, 0);
	printf("                                                                                                                         \n");
	printf("           ��    ��  ��  ���   ��  ���   ��  ����  ����       ��  ����                                           \n");
	printf("           �� �� ��  ��  �� ��  ��  �� ��  ��  ��      ��   ��      ��  ��                                               \n");
	printf("           �� �� ��  ��  ��  �� ��  ��  �� ��  ����  ����       ��  ����                                           \n");
	printf("           �� �� ��  ��  ��   ���  ��   ���  ��      ��   ��      ��      ��                                           \n");
	printf("           �����  ��  ��     ��  ��     ��  ����  ��   ��      ��  ����    ��    ��    ��                         \n");
	printf("                                                                                                                         \n");
	printf("                                                                                                                         \n");
	printf("                                                                                                                         \n");
	textcolor(BLUE2, BLACK);
	printf("              �������          P   L                      ��    ��                                                  \n");
	printf("              ���������  ��         A                 �������                                                \n");
	printf("              ����������             Y                 �ব��������                                                \n");
	printf("              ����     ������            E              ��(^  ^)��                                                \n");
	printf("              ����     ���    ��             R            ��� �� ���                                                \n");
	printf("              ����     ���                                  �����                                                 \n");
	printf("              ����                                           �����                                                 \n");
	printf("              ����     ���    ��                       ���������   ��                                        \n");
	printf("              ����     ���    ��             R        ������������                                          \n");
	printf("              ����     �����              E         ����������                                              \n");
	printf("              ���������               Y           ��  �������                                                \n");
	printf("              �������      ��         A                 �������                                                \n");
	printf("              �����              P   L                      �����                                                  \n");
	printf("                                                             ���    ���                                                \n");
	printf("                                                           ����    ����                                              \n");
	printf("                                                                                                                         \n");
	printf("                                                                                                                         \n");
	printf("                                                                                                                         \n");
	printf("                                                                                                                         \n");
	printf("                                                                                                                         \n");
	textcolor(YELLOW2, BLACK);

	if (pl->knightnum == 1)
	{
		for (j = 10; j <= 22; j++)
		{
			gotoxy(80, j);
			if (j == 11)
			{
				printf("player            ����");
				continue;
			}
			printf("player              ���");
			Sleep(100);
		}
		gotoxy(80, j);
		printf("player            �����");

		//����
		gotoxy(98, 24);
		printf("~~~~~~~~");
		for (j = 0; j < 3; j++)
		{
			gotoxy(97 - j, 23);
			printf("(");
			gotoxy(106 + j, 23);
			printf(")");
			Sleep(100);
		}

		for (j = 0; j < 3; j++)
		{
			gotoxy(97 - j, 23);
			printf(" ");
			gotoxy(106 + j, 23);
			printf(" ");
			Sleep(100);
		}
		gotoxy(98, 24);
		printf("        ");
	}
	if (pl->knightnum == 2)
	{
		for (j = 10; j <= 23; j++)
		{
			gotoxy(80, j);
			if (j == 10 || j == 11 || j == 16 || j == 17 || j == 22 || j == 23)
				printf("player    �������");
			else if (j > 11 && j < 16)
				printf("player            ���");
			else
				printf("player    ���");
			Sleep(100);
		}
		gotoxy(89, 24);
		printf("~~~~~~~~~~~~~~");
		for (j = 0; j < 3; j++)
		{
			gotoxy(89 - j, 23);
			printf("(");
			gotoxy(102 + j, 23);
			printf(")");
			Sleep(100);
		}
		for (j = 0; j < 3; j++)
		{
			gotoxy(89 - j, 23);
			printf(" ");
			gotoxy(102 + j, 23);
			printf(" ");
			Sleep(100);
		}
		gotoxy(89, 24);
		printf("              ");
	}

	gotoxy(10, 28);
	for (i = 0; winner_message_1[i]; i++)
	{
		printf("%c", winner_message_1[i]);
		Sleep(100);
	}
	gotoxy(10, 29);
	for (i = 0; winner_message_2[i]; i++)
	{
		printf("%c", winner_message_2[i]);
		Sleep(100);
	}

	removeCursor();
	_getch();
	exit(1);
}

void subject()
{
	int i;
	for (i = 1; i <= 5; i++)
	{
		textcolor(BLACK, YELLOW2);
		gotoxy(48, i);
		printf("     Term Project.    ");
		textcolor(BLUE2, BLACK);
		Sleep(100);
		if (i != 5)
		{
			gotoxy(48, i);
			printf("                        ");
		}
	}
	for (i = 0; i < 15; i++)
	{
		textcolor(WHITE, BLACK);
		gotoxy(48 - i, 5);
		printf("(");
		gotoxy(73 + i, 5);
		printf(")");
		Sleep(30);
		if (i > 10)
		{
			gotoxy(59 - i, 5);
			printf(" ");
			gotoxy(62 + i, 5);
			printf(" ");
		}
	}
	for (; i < 26; i++)
	{
		gotoxy(59 - i, 5);
		printf(" ");
		if (i == 25)
			printf("*");
		gotoxy(62 + i, 5);
		printf(" ");
		Sleep(30);
		if (i == 25)
			printf("*");
	}
	textcolor(BLUE2, BLACK);
}

// ǥ�� ���� ------------------------------------------------------------------------------------------------------------------

void title() //�Ѽ�
{
	gotoxy(28, 6);
	printf("   ��    ��               ��                  ��            ��");
	Sleep(100);
	gotoxy(28, 7);
	printf(" ����  ��        ��     ��                  ��            ��");
	Sleep(100);
	gotoxy(28, 8);
	printf("   ��    ���     ���  ���                  ���������");
	Sleep(100);
	gotoxy(28, 9);
	printf(" ��  ��  ��      ��  ��   ��                  ��            ��");
	Sleep(100);
	gotoxy(28, 10);
	printf("   ��    ��            ��          ��   ��    ��            ��");
	Sleep(100);
	gotoxy(28, 11);
	printf("  ��                 ��  ��      ��  �� ��    ���������");
	Sleep(100);
	gotoxy(28, 12);
	printf("  ��                   ��          ��   �� ");
	Sleep(100);
	gotoxy(28, 13);
	printf("  ������                    ������ �����������");
	Sleep(100);
	gotoxy(28, 14);
	printf("                                        ��");
	Sleep(100);
	gotoxy(28, 15);
	printf("                            ��");
	Sleep(100);
	gotoxy(28, 16);
	printf("     �����������   ��                ���������");
	Sleep(100);
	gotoxy(28, 17);
	printf("     ��                ��   ��                              ��");
	Sleep(100);
	gotoxy(28, 18);
	printf("     ��                ��   ��                              ��");
	Sleep(100);
	gotoxy(28, 19);
	printf("     ��                ��   ��                ���������");
	Sleep(100);
	gotoxy(28, 20);
	printf("     ��                ��   ����            ��");
	Sleep(100);
	gotoxy(28, 21);
	printf("     ��                ��   ��                ��");
	Sleep(100);
	gotoxy(28, 22);
	printf("     ��                ��   ��                ���������");
	Sleep(100);
	gotoxy(28, 23);
	printf("     �����������   ��");
	Sleep(100);
	gotoxy(28, 24);
	printf("                            ��");
	Sleep(100);
}

void leftman()
{
	gotoxy(0, 6);
	printf("������������������������������������������������������\n");
	printf("                          ��\n");
	printf("                          ��\n");
	printf("                          ��\n");
	printf("                          ��\n");
	printf("                          ��\n");
	printf("��(��)��������������������������������������������\n");
	printf("                          ��\n");
	printf("������������������������������������������������������\n");
	printf("                          ��\n");
	printf("                          ��\n");
	printf("         ����������            ��\n");
	printf("         ������������           ��\n");
	printf("        �� ��  ��           ��\n");
	printf("         ������������           ��\n");
	printf("��                        ��\n");
	printf("                          ��\n");
	printf("����������                     ��\n");
	printf("                          ��\n");
	printf("������������������������������������������������������\n");
	gotoxy(0, 7);
	textcolor(WHITE, BLUE1);
	printf("                          \n");
	printf("                          \n");
	printf("                          \n");
	printf("                          \n");
	printf("                          \n");
	textcolor(WHITE, GREEN1);
	printf("��(��)������������������������������������������\n");
	textcolor(WHITE, YELLOW1);
	printf("                          \n");
	textcolor(WHITE, BLACK);
	printf("\n\n");
	textcolor(BLACK, WHITE);
	printf("         ����������            \n");
	printf("         ������������           \n");
	printf("         �� �� ��           \n");
	printf("         ������������           \n");
	printf(" ��                       \n");
	printf("                          \n");
	printf("����������                     \n");
	printf("                          \n");
	printf("                          \n");
	gotoxy(16, 20);
	textcolor(WHITE, GREEN1);
	printf("     ");
	gotoxy(16, 21);
	textcolor(WHITE, RED1);
	printf("     ");
	gotoxy(16, 22);
	textcolor(WHITE, BLACK);
	printf("     ");
}

void loading() // �ε� �� ����
{
	int i, per = 5;
	textcolor(GREEN2, BLACK);
	gotoxy(40, 29);
	printf("���������������������");
	for (i = 0; i < 20; i++)
	{
		gotoxy(40 + i * 2, 28);
		printf("%d%%", per);
		gotoxy(40 + i * 2, 29);
		printf("��>��");
		if (i == 4)
			Sleep(500);
		else if (i < 10)
			Sleep(150);
		else
			Sleep(50);
		per += 5;
		if (i != 19)
		{
			gotoxy(40 + i * 2, 28);
			printf("    ", per);
		}
	}
}

void intro()
{
	char cmd[100];
	sprintf(cmd, "mode con cols=%d lines=%d", 122, 35); //����â ũ�� �ø���
	system(cmd);
	cls(BLACK, BLUE2);

	leftman();
	subject();
	title();
	loading();

	gotoxy(45, 33);
	printf("����Ϸ��� �ƹ� Ű�� ��������");
	gotoxy(100, 34);
	printf("Made by Mingyu kim");
	removeCursor();
	_getch();
}

// ���ݱ��� �� ��� �Լ� ���� �ֱ� ----------------------------------------------------------------------------------------------
// ���ε��� �ִ��� ������ ���θ� Ȯ���ϴ� �Լ��� ��Ծ main�� �߰��Ѱ� ��ȭ��;

int main()
{
	CityInfo city[26];
	Player player1, player2;
	int dice_num;

	//BGM
	mciOpen.lpstrDeviceType = L"mpegvideo";		   // mpegvideo : mp3, waveaudio : wav, avivideo : avi
	mciOpen.lpstrElementName = L"GameWaiting.mp3"; // �����̸�.  L�� ���̸� �����ڵ尡 �ذ�?
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mciOpen);
	dwID = mciOpen.wDeviceID;
	mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlay);

	/*mciOpen.lpstrElementName = L"GamePlayBefore.mp3";
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mciOpen);
	dwID = mciOpen.wDeviceID;
	mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mciPlay);*/
	// �ڿ� ������ �߰��ϰ� �;����� ����

	//
	intro();
	srand(time(NULL));

	make_map(0, 0, 120, 25, "��");
	init_city(city);
	init_player(&player1, &player2);
	//winner(&player2);					// �ٷ� ������ Ȯ�ν�
	while (1)
	{
		howmoney(&player1, &player2);
		gotoxy(0, 26);
		if (player1.stop_move == 0)
		{ //���ε� üũ
			printf("PLAYER1�� �����Դϴ�. �ֻ����� ���߷��� ���� Ű�� ��������.");
			//�ֻ��� ���ư���, ���߱�. �� ����ŭ player �����̱�
			while (1)
			{
				dice_num = rand() % 6 + 1;
				dice(dice_num);
				fflush(stdin);
				Sleep(60);
				if (_kbhit())
				{
					//dice ����
					PlaySound(L"dice.wav", NULL, SND_ASYNC);
					dicemotion();
					fflush(stdin);
					gotoxy(94, 19);
					printf("%dĭ �̵��մϴ�", dice_num);
					move_player(city, dice_num, &player1, &player2); //������ player�� ��������
					gotoxy(0, 26);
					_getch();
					break;
				}
			}
			checkin(city, &player1, &player2); // ���� üũ��
			dicemotion_clear();
		}

		//���ε��� ������
		else
		{
			printf("���ε����� 3�� ���Ե˴ϴ�. 3�� �� %d�� ���ҽ��ϴ�.", player1.stop_move);
			player1.stop_move--;
			_getch();
			if (player1.getout > 0)
			{
				gotoxy(0, 27);
				printf("��Ⱑ �����Ϸ� �Խ��ϴ�!");
				_getch();
				player1.stop_move = 0;
				player1.getout--;
			}
		}

		howmoney(&player1, &player2);
		clear_downpart();

		if (player1.money < 0)
		{
			winner(&player2);
		}
		else if (player2.money < 0)
		{
			winner(&player1);
		}

		clear_downpart();
		gotoxy(0, 26);
		if (player2.stop_move == 0)
		{
			printf("PLAYER2�� �����Դϴ�. �ֻ����� ���߷��� ���͸� ��������.");

			while (1)
			{
				dice_num = rand() % 6 + 1;
				dice(dice_num);
				Sleep(60);
				if (_kbhit())
				{
					//dice ����
					PlaySound(L"dice.wav", NULL, SND_ASYNC);
					//���� ��
					dicemotion(); //�ָ� ��
					gotoxy(94, 19);
					printf("%dĭ �̵��մϴ�\n", dice_num);
					move_player(city, dice_num, &player2, &player1);
					gotoxy(0, 26);
					_getch();
					break;
				}
			}
			checkin(city, &player2, &player1); // ���� üũ��
			dicemotion_clear();
		}

		else
		{
			printf("���ε��� ���� 3�� ���Ե˴ϴ�. 3�� �� %d�� ���ҽ��ϴ�.", player2.stop_move);
			player2.stop_move--;
			_getch();
			if (player2.getout > 0)
			{
				gotoxy(0, 27);
				printf("���ε��� Ż���� �� �ְ� �Ǿ����ϴ�");
				_getch();
				player2.stop_move = 0;
				player2.getout--;
			}
		}

		clear_downpart();
		if (player1.money < 0)
		{
			winner(&player2);
		}
		else if (player2.money < 0)
		{
			winner(&player1);
		}
	}
}

/* void draw_end(int x1, int y1, int x2, int y2, char *ch)
{
	int x, y;

	for (x = x1; x <= x2; x = x + 2) {
		gotoxy(x, y1);
		printf("%s", ch);
		gotoxy(x, y2);
		printf("%s", ch);
	}
	for (y = y1; y <= y2; y++) {
		gotoxy(x1, y);
		printf("%s", ch);
		gotoxy(x2, y);
		printf("%s", ch);
	}
} */

//