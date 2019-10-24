// MineSweep.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>]

#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

using namespace std;

const int width = 9;
const int height = 9;
const int numMines = 10;
const string exitString = "quit";
const string showString = "show";
const char CHAR_BLANK = 'x';
const char CHAR_FLAG = 'f';

void print_test();
void draw_board(char board[width][height]);
void draw_board(int board[width][height]);
void initialize_board(int board[width][height]);
void clear_display_board(char board[width][height]);
bool check(int x, int y, int board[width][height]);
bool flag(int x, int y, char board[width][height]);

int main()
{
	std::cout << "Hello World!\n";
	print_test();
	int board[width][height] = { 0 };
	char displayBoard[9][9];
	clear_display_board(displayBoard);
	draw_board(board);

	bool gameRunning = true;
	bool isWin = false;
	while (gameRunning)
	{
		draw_board(displayBoard);
		cout << "Choose a spot. Type \"" << exitString << "\" to exit. \n";
		//prompt input
		string input;
		while (1)
		{
			cin >> input;
			if (input.compare(exitString) == 0)
			{
				return 0;
			}
			else if (input.compare(showString) == 0)
			{
				//use flags = 'f'
				//check = 'c'
				draw_board(displayBoard);
			}
			else
			{
			}
		}
	}


}


void print_test() {
	cout << width << " " << height << '\n';
}

void draw_board(int board[width][height])
{
	for (int i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++) {
			cout << board[i][j] << " ";
		}
		cout << '\n';
	}
}

void initialize_board(int board[width][height])
{
	int *ptr[height * width];

	int l = 0;
	for (int i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++) {
			ptr[l] = &board[i][j];
		}
	}
	

}

void draw_board(char board[width][height])
{
	cout << " \t";
	char legendChar = 'A';
	for (int k = 0; k < width; k++)
	{
		cout << legendChar << " ";
		++legendChar;
	}
	cout << '\n';

	for (int i = 0; i < height; i++)
	{
		cout << i << '\t';
		for (size_t j = 0; j < width; j++) {
			cout << board[i][j] << " ";
		}
		cout << '\n';
	}
}


void clear_display_board(char board[width][height])
{
	for (int i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++) {
			board[i][j] = 'x';
		}
	}

}

bool check(int x, int y, int board[width][height])
{
	//checks board[x][y] if it is a mine Returns true if living, returns false if mine
	return false;
}

bool flag(int x, int y, char board[width][height])
{
	char temp = board[x][y];
	if (temp != CHAR_BLANK || temp != CHAR_FLAG)
		return false; // invalid flag operation
	else
	{
		//Toggle flag or blank
		if (temp == CHAR_BLANK)
			temp = CHAR_FLAG;
		else if (temp == CHAR_FLAG)
			temp = CHAR_BLANK;
		return true;
	}
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
