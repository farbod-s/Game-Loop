/*********************
* Farbod Samsamipour *
* 88521068           *
*********************/

#include <iostream>
#include <fstream>
#include <windows.h>
#include <time.h>
#include <vector>
#include <string>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

using namespace std;

enum Status {
	Left = -1,
	Right = 1,
	Idle = 0,
	Jump = 2
};

const int maxWidth = 60;
const int maxHeight = 20;
vector< vector<char> > map;
int pos_x = maxHeight - 2;
int pos_y = maxWidth - 2;
int playerHeight = 0;
const int maxJumpHeight = 3;
bool running = true;
bool jumping = false;
const int frequency = 60;
Status status = Status::Idle;

void loadMap();
void update();
void render();

void loadMap()
{
	map.clear();
	string line;
	ifstream file("map.GL");
	if(file.is_open())
	{
		while(file.good())
		{
			getline(file, line);
			vector<char> temp;
			for(int i = 0; i < maxWidth; ++i)
				temp.push_back(line[i]);

			map.push_back(temp);
		}
		file.close();
	}
}

void update()
{
	map[pos_x][pos_y] = ' ';
	status = Status::Idle;
	if(jumping)
	{
		if(playerHeight < maxJumpHeight && map[pos_x - 1][pos_y] == ' ')
		{
			pos_x--;
			playerHeight++;
		}
		else
		{
			jumping = false;
			playerHeight = 0;
		}
	}
	
	else if(map[pos_x + 1][pos_y] == ' ')
		pos_x++;
	
	if(GetAsyncKeyState(VK_ESCAPE) || map[pos_x][pos_y - 1] == 'E' || map[pos_x + 1][pos_y] == 'E')
		running = false;

	if(GetAsyncKeyState(VK_LEFT))
		if(map[pos_x][pos_y - 1] == ' ')
		{
			status = Status::Left;
			pos_y--;
		}

	if(GetAsyncKeyState(VK_RIGHT))
		if(map[pos_x][pos_y + 1] == ' ')
		{
			status = Status::Right;
			pos_y++;
		}

	if(GetAsyncKeyState(VK_UP))
		if(map[pos_x + 1][pos_y] == '*' || map[pos_x + 1][pos_y] == '+')
		{
			status = Status::Jump;
			jumping = true;
		}
	
	// set player!
	switch(status)
	{
		case Status::Left: map[pos_x][pos_y] = 'q'; break;
		case Status::Right: map[pos_x][pos_y] = 'p'; break;
		case Status::Jump: map[pos_x][pos_y] = 'Y'; break;
		case Status::Idle: map[pos_x][pos_y] = 'X'; break;
		default:map[pos_x][pos_y] = 'X'; break;
	}
}

void render()
{
	for(int y = 0; y < maxHeight; ++y)
	{
		for(int x = 0; x < maxWidth; ++x)
		{
			printf("%c", map[y][x]);
		}
		printf("\n");
	}
	
	system("cls");
}

int main()
{
	system("cls");
	unsigned long int timeLastCall = GetTickCount();

	// load game map
	loadMap();

	// set player!
	map[pos_x][pos_y] = 'X';
	
	// background sound
	PlaySound("back.wav", NULL, SND_ASYNC | SND_LOOP);
	time_t seconds;

	/* GAME LOOP */
	while(running)
	{
		if((GetTickCount() - timeLastCall) > (1000 / frequency))
		{
			update();
			timeLastCall = GetTickCount();
		}
		render();
	}

	system("cls");
	printf("%s", "*************\n* Game Over *\n*************\n\nPress any key to continue ...");
	char res = '\0';
	cin >> res;

	return EXIT_SUCCESS;
}
