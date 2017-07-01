// PingPong.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>;
#include "time.h"
#include "conio.h"
#include "windows.h"

using namespace std;
void SetConsoleSize(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
	//ShowWindow(console, SW_MAXIMIZE);
}
void removeCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void gotoxy(int x, int y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	_COORD pos;
	pos.X = x;
	pos.Y = y;

	SetConsoleCursorPosition(hConsole, pos);
}
void TextColor(int x)
{
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}

enum eDir { STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6 };
class Ball
{
private:
	int x; int y;
	int originalX; int originalY;
	eDir direction;
public:
	Ball(int posX, int posY)
	{
		originalX = posX;
		originalY = posY;
		x = posX; y = posY;
		direction = STOP;
	}

	void Reset()
	{
		x = originalX;
		y = originalY;
		direction = STOP;
	}

	void changeDirection(eDir d)
	{
		direction = d;
	}

	void randomDirection()
	{
		direction = (eDir)((rand() % 6) + 1);
	}

	inline int getX() { return x; }
	inline int getY() { return y; }
	inline eDir getDirection() { return direction; }

	void Move()
	{
		switch (direction)
		{
		case STOP:
			break;

		case LEFT:
			x--;
			break;

		case RIGHT:
			x++;
			break;

		case UPLEFT:
			x--;
			y--;
			break;

		case DOWNLEFT:
			x--;
			y++;
			break;

		case UPRIGHT:
			x++;
			y--;
			break;

		case DOWNRIGHT:
			x++;
			y++;
			break;

		default:
			break;
		}
	}
	friend ostream & operator<<(ostream & o, Ball c)
	{
		o << "Ball[" << c.x << "," << c.y << "][" << c.direction << "]";
		return o;
	}
};

class Paddle
{
private:
	int x; int y;
	int originalX; int originalY;

public:
	Paddle()
	{
		x = y = 0;
	}

	Paddle(int posX, int posY) : Paddle()
	{
		originalX = posX;
		originalY = posY;
		x = originalX;
		y = originalY;
	}

	void Reset()
	{
		x = originalX;
		y = originalY;
	}

	inline int getX() { return x; }
	inline int getY() { return y; }

	inline void moveUp() { y--; }
	inline void moveDown() { y++; }

	friend ostream & operator <<(ostream &o, Paddle c)
	{
		o << "Paddle[" << c.x << "," << c.y << "]";
		return o;
	}
};

class GameManager
{
private:
	int width, height;
	int score1, score2;
	int paddleLength;
	bool quit;

	Ball *ball;
	Paddle *player1;
	Paddle *computer;

public:
	GameManager(int w, int h)
	{
		srand(time(NULL));
		quit = false;
		score1 = score2 = 0;
		paddleLength = 6;
		width = w; height = h;
		ball = new Ball(w / 2, h / 2);
		player1 = new Paddle(1, h / 2 - 3);
		computer = new Paddle(w - 2, h / 2 - 3);
	}
	~GameManager()
	{
		delete ball, player1, computer;
	}

	void Restart()
	{
		system("cls");
		ball->Reset();
		player1->Reset();
		computer->Reset();
		TextColor(0);
		Draw();
		PrintUI();
		CountDown();
	}

	void ScoreUp(Paddle *player)
	{
		if (player == player1)
		{
			TextColor(10);
			gotoxy(width / 2 - 6, height / 2); cout << "PLAYER WON";
			score1++;
			TextColor(14);
			gotoxy(width / 2 - 13, height / 2 + 1);
			system("pause");
		}
		if (player == computer)
		{
			TextColor(11);
			gotoxy(width / 2 - 6, height / 2); cout << "COMPUTER WON";
			score2++;
			TextColor(14);
			gotoxy(width / 2 - 13, height / 2 + 1);
			system("pause");
		}
		Restart();
	}

	void Draw()
	{
		gotoxy(0, 0);

		TextColor(119);
		for (int i = 0; i < width + 2; i++)
		{
			cout << '#';
		}
		TextColor(0);
		cout << endl;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int ballx = ball->getX();
				int bally = ball->getY();
				int player1x = player1->getX();
				int player1y = player1->getY();
				int computerx = computer->getX();
				int computery = computer->getY();


				if (j == 0)
				{
					TextColor(119);
					cout << '#';
					TextColor(0);
				}

				if (j == ballx && i == bally)
				{
					TextColor(12);
					cout << 'o';
					TextColor(0);
				}
				else if (j == player1x && i >= player1y && i <= player1y + paddleLength - 1)
				{
					TextColor(10);
					cout << '=';
					TextColor(0);
				}
				else if (j == computerx && i >= computery && i <= computery + paddleLength - 1)
				{
					TextColor(11);
					cout << '=';
					TextColor(0);
				}
				else
					cout << ' ';

				if (j == width - 1)
				{
					TextColor(119);
					cout << '#';
					TextColor(0);
				}
			}
			cout << endl;
		}

		TextColor(119);
		for (int i = 0; i < width + 2; i++)
			cout << '#';
		TextColor(0);
		cout << endl;
	}
	void CheckInput()
	{
		ball->Move();

		int ballx = ball->getX();
		int bally = ball->getY();
		int player1x = player1->getX();
		int player1y = player1->getY();
		int computerx = computer->getX();
		int computery = computer->getY();

		if (_kbhit())
		{
			char current = _getch();

			if (current == 'w' || current == 'W')
				if (player1y > 0)
					player1->moveUp();
			if (current == 's' || current == 'S')
				if (player1y + paddleLength - 1 < height - 1)
					player1->moveDown();

			/*if (current == up2)
			if (computery > 0)
			computer->moveUp();
			if (current == down2)
			if (computery + 6 < height)
			computer->moveDown();*/

			if (current == 'q')
				quit = true;

			if (current == 'r')
			{
				score1 = score2 = 0;
				Restart();
			}
		}

		if (ball->getDirection() == STOP)
			ball->randomDirection();

		int computerActive = rand() % 5;
		if (computerActive > 0)
		{
			if (ball->getDirection() == RIGHT)
			{
				if (bally > computery + paddleLength - 1)
				{
					if (computery + paddleLength - 1 < height - 1)
						computer->moveDown();
				}
				else if (bally < computery)
				{
					if (computery > 0)
						computer->moveUp();
				}
			}

			if (ball->getDirection() == DOWNRIGHT && bally > computery + paddleLength - 1)
			{
				if (computery + paddleLength - 1 < height - 1)
					computer->moveDown();
			}
			if (ball->getDirection() == UPRIGHT && bally < computery)
			{
				if (computery > 0)
					computer->moveUp();
			}
		}
	}

	void Logic()
	{
		int ballx = ball->getX();
		int bally = ball->getY();
		int player1x = player1->getX();
		int player1y = player1->getY();
		int computerx = computer->getX();
		int computery = computer->getY();

		for (int i = 0; i < 6; i++)
		{
			if (ballx == player1x + 1)
				if (bally == player1y + i)
					ball->changeDirection(eDir((rand() % 3) + 4));
		}

		for (int i = 0; i < 6; i++)
		{
			if (ballx == computerx - 1)
				if (bally == computery + i)
					ball->changeDirection(eDir((rand() % 3) + 1));
		}

		if (bally == height - 1)
			ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
		else if (bally == 0)
			ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);

		if (ballx == width - 1)
			ScoreUp(player1);
		else if (ballx == 0)
			ScoreUp(computer);
	}

	void PrintUI()
	{
		TextColor(10);
		gotoxy(width + 5, 5);
		cout << "Player's score: " << score1;
		TextColor(11);
		gotoxy(width + 5, 6);
		cout << "Computer's score: " << score2;

		TextColor(15);
		gotoxy(width + 5, 8);
		cout << "Press Q key to quit game";
		gotoxy(width + 5, 9);
		cout << "Press R key to restart game";

		gotoxy(width / 2 - 7, height + 3);
		TextColor(10); cout << "PING"; TextColor(11); cout << " PONG"; TextColor(12); cout << " GAME";
		gotoxy(width / 2 - 8, height + 4);
		TextColor(14); cout << "Author: Hao Phan";
	}

	void CountDown()
	{
		for (int i = 0; i < 3; i++)
		{
			gotoxy(width / 2 - 8, height / 2);
			TextColor(14);
			cout << "Ready to play: " << i + 1;
			TextColor(0);
			Sleep(1000);
		}
	}

	void Run()
	{
		Draw();
		PrintUI();
		CountDown();
		while (!quit)
		{
			srand(time(NULL));
			CheckInput();
			Logic();
			Draw();
			PrintUI();
		}
	}
};

void WindowInit()
{
	SetConsoleSize(800, 480);
	removeCursor();
}

int main()
{
	WindowInit();
	GameManager Gm(60, 20);
	Gm.Run();
	return 0;
}