// MineSweep.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <utility>		//pair

using namespace std;

const int width = 9;
const int height = 9;
const int numMines = 10;

//input values
const string exitString = "quit";
const string showString = "show";
const char KEY_CHECK = 'c';
const char KEY_FLAG = 'f';

//legend for display board
const char CHAR_BLANK = 'x';
const char CHAR_FLAG = 'F';


void print_board(char board[width][height]);
void print_board(int board[width][height]);
void setup_board(int board[width][height], char displayBoard[width][height], char *mineLocations[numMines]);
void clear_display_board(char board[width][height]);
bool check_mine(int x, int y, int *flagCount, int board[width][height], char displayBoard[width][height]);
void reveal_area(int x, int y, int *flagCount, int board[width][height], char displayBoard[width][height]);
bool set_mine_count(int x, int y, int board[width][height], char displayBoard[width][height]);
bool flag(int x, int y, int *flagCount, char displayBoard[width][height], char* minelocations[numMines]); //Once all hidden mines are flagged, game is won. If mine is checked, game is lost


int main()
{
	int board[width][height] = { 0 }; // the actual board data
	char displayBoard[9][9]; //the printed board

	char* mineLocations[numMines];
	int flagCount = numMines;
	clear_display_board(displayBoard);

	setup_board(board, displayBoard, mineLocations);

	print_board(board);

	bool game = true;
	bool isLose = false;
	while (game)
	{
		cout << endl;
		cout << "Flag Count: " << flagCount << endl;
		print_board(displayBoard);
		cout << "Choose a spot to flag(f) or check(c). Example input: fA1 = flag (A,1).  Type \"" << showString << "\" to print the board. Type \"" << exitString << "\" to close game. \n";

		//Get player input
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
				print_board(displayBoard);
			}
			else if (input.compare("new") == 0)
			{
				setup_board(board, displayBoard, mineLocations);
			}
			else
			{
				if (input.length() > 2) {
					//use flags = 'f'
					//check for mine = 'c'
					char key = input[0];
					char xKey = input[1];
					char yKey = input[2];
					//convert from ascii
					int x = (int)xKey - 65; //A == 65
					int y = (int)yKey - '0';
					//Check if input coordinates are valid
					if (x >= 0 && y >= 0 && x < width && y < height)
					{
						//process input for valid coordinate data 
						//flag
						if (key == KEY_FLAG)
						{
							cout << "Flagging (" << x << "," << y << ")\n";
							flag(x, y, &flagCount, displayBoard, mineLocations);
							break;
						}
						//check
						if (key == KEY_CHECK)
						{
							cout << "Checking (" << x << "," << y << ")\n";
							isLose = check_mine(x, y, &flagCount, board, displayBoard);
							if (isLose)
							{
								cout << "Flag Count: " << flagCount << endl;
								print_board(displayBoard);
								cout << "Mine found! You Lost!" << endl;
								cin.ignore();
								game = false;
							}
							break;
						}
					}
				}
			}
		}
		//Once valid input exists, process move
	}


}


void setup_board(int board[width][height], char displayBoard[width][height], char *mineLocations[numMines])
{
	cout << " STUFF" << endl;
	print_board(board);

	//create a list of pointers to board spaces
	const int ptrLength = height * width;
	pair<int, int> coordinates[ptrLength];

	int l = 0;
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			coordinates[l] = make_pair(i, j);
			++l;
		}
	}

	//shuffle array of locations
	random_shuffle(coordinates, coordinates + ptrLength);

	//create a list of unique random spaces for mines to be placed
	for (size_t i = 0; i < numMines; ++i)
	{
		int x = coordinates[i].first;
		int y = coordinates[i].second;
		mineLocations[i] = &displayBoard[x][y];
		cout << x << " " << y << endl;
		board[x][y] = 1;
	}
	print_board(board);
	cout << " STUFF" << endl;
}


void print_board(int board[width][height])
{
	for (int i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++) {
			cout << board[j][i] << " ";
		}
		cout << '\n';
	}
}


void print_board(char board[width][height])
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
			cout << board[j][i] << " ";
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

//return: is there a mine there?
bool check_mine(int x, int y, int *flagCount, int board[width][height], char displayBoard[width][height])
{
	if (displayBoard[x][y] == CHAR_FLAG)
	{
		cout << "You can't check flagged areas." << endl;
		return false;
	}

	//Assumes that initial input is within bounds
	if (board[x][y] == 1)
	{
		//Reveal all Mines
		
		//Greedy way : iterate on board
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++) {
				if (board[j][i] == 1)
				displayBoard[j][i] = 'M';
			}
		}

		return true;
	}
	else
	{
		reveal_area(x, y, flagCount, board, displayBoard);
		return false;
	}
}


void reveal_area(int x, int y, int *flagCount, int board[width][height], char displayBoard[width][height])
{
	//Assumes that initial input is within bounds

	//if area already checked, return;
	if (displayBoard[x][y] != CHAR_BLANK && displayBoard[x][y] != CHAR_FLAG)
		return;

	if (displayBoard[x][y] == CHAR_FLAG)
		++flagCount;

	//set the mine count for the current cell
	bool minefound = set_mine_count(x, y, board, displayBoard);

	if (minefound)
		return;

	//traverse other cells if mine not found
	if (x > 0 && y > 0)
		reveal_area(x - 1, y - 1, flagCount, board, displayBoard);
	if (x > 0)
		reveal_area(x - 1, y, flagCount, board, displayBoard);
	if (x > 0 && y < height)
		reveal_area(x - 1, y + 1, flagCount, board, displayBoard);
	if (y > 0)
		reveal_area(x, y - 1, flagCount, board, displayBoard);

	//mineCount += board[x][y];
	if (y < height)
		reveal_area(x, y + 1, flagCount, board, displayBoard);
	if (x < width && y > 0)
		reveal_area(x + 1, y - 1, flagCount, board, displayBoard);
	if (x < width)
		reveal_area(x + 1, y, flagCount, board, displayBoard);
	if (x < width && y < height)
		reveal_area(x + 1, y + 1, flagCount, board, displayBoard);
}


//Given a cell, figures out the nearby mines and sets the displayed value on the displayBoard. Returns a bool is a mind was detected.
bool set_mine_count(int x, int y, int board[width][height], char displayBoard[width][height])
{
	//Assumes that current cell is not a mine
	int mineCount = 0;

	if (x > 0 && y > 0)
		mineCount += board[x - 1][y - 1];
	if (x > 0)
		mineCount += board[x - 1][y];
	if (x > 0 && y < height)
		mineCount += board[x - 1][y + 1];

	if (y > 0)
		mineCount += board[x][y - 1];

	//mineCount += board[x][y];
	if (y < height)
		mineCount += board[x][y + 1];

	if (x < width && y > 0)
		mineCount += board[x + 1][y - 1];
	if (x < width)
		mineCount += board[x + 1][y];
	if (x < width && y < height)
		mineCount += board[x + 1][y + 1];

	displayBoard[x][y] = '0' + mineCount; //set the mine Count on the displayboard

	//return if a mine was detected or not.
	return mineCount > 0;
}


//return: if game is won
bool flag(int x, int y, int *flagCount, char displayBoard[width][height], char* minelocations[numMines])
{
	char *temp = &displayBoard[x][y];
	if (*temp != CHAR_BLANK && *temp != CHAR_FLAG)
	{
		cout << "Invalid flag operation" << endl;
		return false; // invalid flag operation, can not act on 
	}
	else
	{
		//Toggle flag or blank
		if (*temp == CHAR_BLANK && flagCount > 0)
		{
			*temp = CHAR_FLAG;
			--*flagCount;

			//Check if game is won if all flags placed on mines
			if (*flagCount == 0)
			{
				for (int i = 0; i < numMines; i++)
				{
					if (*minelocations[i] != CHAR_FLAG)
						return false;
				}
				return true;
			}

		}
		else if (*temp == CHAR_FLAG && *flagCount < numMines)
		{
			*temp = CHAR_BLANK;
			++*flagCount;
		}
		return false;
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
