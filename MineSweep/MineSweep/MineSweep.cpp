// MineSweep.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <time.h>       // time
#include <utility>		// std::pair, std::tuple
#include <stdlib.h>     // exit, EXIT_FAILURE, srand, rand

using namespace std;

const int WIDTH = 9;
const int HEIGHT = 9;

//@TODO NUM_MINES = 10
const int NUM_MINES = 10;

//input values
const string STRING_EXIT = "quit";
const string STRING_SHOW = "show";
const char KEY_CHECK = 'c';
const char KEY_FLAG = 'f';

//legend for display board
const char CHAR_BLANK = 'x';
const char CHAR_FLAG = 'F';



void play_minesweeper();
bool is_valid_coordinate(int x, int y);
enum class Operation
{
	Check,
	Flag
};
tuple<Operation, int, int> read_input_string(string input); // (operation, x coordinate, y coordinate)

void print_board(char board[WIDTH][HEIGHT]);
void print_board(int board[WIDTH][HEIGHT]);
void setup_board(int board[WIDTH][HEIGHT], char displayBoard[WIDTH][HEIGHT], char *mineLocations[NUM_MINES]);
void clear_display_board(char board[WIDTH][HEIGHT]);
bool check_mine(int x, int y, int *flagCount, int board[WIDTH][HEIGHT], char displayBoard[WIDTH][HEIGHT]);
void reveal_area(int x, int y, int *flagCount, int board[WIDTH][HEIGHT], char displayBoard[WIDTH][HEIGHT]);
bool set_mine_count(int x, int y, int board[WIDTH][HEIGHT], char displayBoard[WIDTH][HEIGHT]);
bool flag(int x, int y, int *flagCount, char displayBoard[WIDTH][HEIGHT], char* minelocations[NUM_MINES]); //Once all hidden mines are flagged, game is won. If mine is checked, game is lost


int main()
{
	while (true)
	{
		play_minesweeper();

		//Retry game?
		cout << "Play Again? y/n" << endl;
		char input;
		while (true)
		{
			cin >> input;
			if (input == 'y')
				break;
			else if (input == 'n')
				return 0;
		}
	}
}

void play_minesweeper()
{
	int board[WIDTH][HEIGHT] = { 0 };	// the board data with the mine locations 0 = blank space, 1 = mine
	char displayBoard[9][9];			//the printed board the player can see and flag

	char* mineLocations[NUM_MINES];	//A list of addresses of locations on displayBoard containing mines
	int flagCount = NUM_MINES;		//A count of the number of flags that can be placed

	clear_display_board(displayBoard);

	setup_board(board, displayBoard, mineLocations);

	//print_board(board);	//@TODO remove

	bool gameIsRunning = true;
	bool isLose = false;
	bool isWin = false;
	while (gameIsRunning)
	{
		cout << endl;
		cout << "Flag Count: " << flagCount << endl;
		print_board(displayBoard);
		cout << "Choose a spot to flag(f) or check(c). Example input: fA1 = flag (A,1).  Type \"" << STRING_SHOW << "\" to print the board. Type \"" << STRING_EXIT << "\" to close game. \n";

		//Get player input
		string input;
		while (true)
		{
			cin >> input;
			if (input.compare(STRING_EXIT) == 0)
			{
				//Close the program
				exit(EXIT_SUCCESS);
				return;
			}
			else if (input.compare(STRING_SHOW) == 0)
			{
				print_board(displayBoard);
			}
			else if (input.compare("new") == 0)
			{
				setup_board(board, displayBoard, mineLocations);
			}
			else
			{
				if (input.length() > 2)
				{
					tuple<Operation, int, int> processedInput = read_input_string(input);

					Operation inputOperation = get<0>(processedInput);
					int x = get<1>(processedInput);
					int y = get<2>(processedInput);

					//Process input for valid coordinate data 
					if (is_valid_coordinate(x, y))
					{
						//Operation: flag
						if (inputOperation == Operation::Flag)
						{
							cout << "Flagging (" << x << "," << y << ")\n";
							isWin = flag(x, y, &flagCount, displayBoard, mineLocations);
							//Win State
							if (isWin)
							{
								cout << "Flag Count: " << flagCount << endl;
								print_board(displayBoard);

								cout << "You win!" << endl;
								cin.ignore();
								gameIsRunning = false;
							}
							break;
						}
						//Operation: check
						if (inputOperation == Operation::Check)
						{
							cout << "Checking (" << x << "," << y << ")\n";
							isLose = check_mine(x, y, &flagCount, board, displayBoard);

							//Lose State
							if (isLose)
							{
								cout << "Flag Count: " << flagCount << endl;
								print_board(displayBoard);
								cout << "Mine found! You Lost!" << endl;
								cin.ignore();
								gameIsRunning = false;
							}
							break;
						}
					}
				}
			}
		}
	}
}

bool is_valid_coordinate(int x, int y)
{
	return (x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT);
}

// random generator function:
int my_random(int i)
{
	srand(time(0));		//randomizes seed based on current time
	return rand() % i;
}

void setup_board(int board[WIDTH][HEIGHT], char displayBoard[WIDTH][HEIGHT], char *mineLocations[NUM_MINES])
{
	//create a list of coordinates
	const int coordLength = HEIGHT * WIDTH;
	pair<int, int> coordinates[coordLength];

	int l = 0;
	for (size_t i = 0; i < HEIGHT; i++)
	{
		for (size_t j = 0; j < WIDTH; j++)
		{
			coordinates[l] = make_pair(i, j);
			++l;
		}
	}

	//shuffle array of locations
	random_shuffle(coordinates, coordinates + coordLength, my_random);

	//create a list of unique random spaces for mines to be placed from first [NUM_MINES] locations
	for (size_t i = 0; i < NUM_MINES; ++i)
	{
		int x = coordinates[i].first;
		int y = coordinates[i].second;
		mineLocations[i] = &displayBoard[x][y];
		board[x][y] = 1;
	}
}


void print_board(int board[WIDTH][HEIGHT])
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (size_t j = 0; j < WIDTH; j++) {
			cout << board[j][i] << " ";
		}
		cout << '\n';
	}
}


tuple<Operation, int, int> read_input_string(string input)
{
	char operationKey = input[0];
	char xKey = toupper(input[1]);
	char yKey = input[2];

	Operation o;
	int x = (int)xKey - 65; //A == 65
	int y = (int)yKey - '0';

	switch (operationKey)
	{
	case 'c':
		o = Operation::Check;
		break;
	case 'C':
		o = Operation::Check;
		break;
	case 'f':
		o = Operation::Flag;
		break;
	case 'F':
		o = Operation::Flag;
		break;
	default:
		o = Operation::Check;
		break;
	}
	return make_tuple(o, x, y);
}

void print_board(char board[WIDTH][HEIGHT])
{
	cout << " \t";
	char legendChar = 'A';
	for (int k = 0; k < WIDTH; k++)
	{
		cout << legendChar << " ";
		++legendChar;
	}
	cout << '\n';

	for (int i = 0; i < HEIGHT; i++)
	{
		cout << i << '\t';
		for (size_t j = 0; j < WIDTH; j++) {
			cout << board[j][i] << " ";
		}
		cout << '\n';
	}
}


void clear_display_board(char board[WIDTH][HEIGHT])
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (size_t j = 0; j < WIDTH; j++) {
			board[i][j] = 'x';
		}
	}

}

//returns true if a mine exists at (x,y)
bool check_mine(int x, int y, int *flagCount, int board[WIDTH][HEIGHT], char displayBoard[WIDTH][HEIGHT])
{
	//Assumes that initial input is within bounds

	if (displayBoard[x][y] == CHAR_FLAG)
	{
		cout << "You can't check flagged areas." << endl;
		return false;
	}

	if (board[x][y] == 1)
	{
		//Reveal all Mines

		//Greedy way : iterate on board and set character on displayBoard
		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++) {
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


void reveal_area(int x, int y, int *flagCount, int board[WIDTH][HEIGHT], char displayBoard[WIDTH][HEIGHT])
{
	//Assumes that initial input is within bounds and does not contain a mine

	//if area already revealed, return;
	if (!(displayBoard[x][y] == CHAR_BLANK || displayBoard[x][y] == CHAR_FLAG) || board[x][y] == 1)
		return;

	//if area revealed contains flag, clear the flag
	if (displayBoard[x][y] == CHAR_FLAG)
		++flagCount;

	//set the mine count for the current cell
	bool minefound = set_mine_count(x, y, board, displayBoard);

	//cout << "Check: " << x << " " << y << " " << minefound << endl; //@TODO remove

	//If a mine is found in adjacent area, return
	if (minefound)
		return;

	//traverse other cells if mine not found

	//northwest
	if (is_valid_coordinate(x - 1, y - 1))
		reveal_area(x - 1, y - 1, flagCount, board, displayBoard);
	//west
	if (is_valid_coordinate(x - 1, y))
		reveal_area(x - 1, y, flagCount, board, displayBoard);
	//southwest
	if (is_valid_coordinate(x - 1, y + 1))
		reveal_area(x - 1, y + 1, flagCount, board, displayBoard);

	//north
	if (is_valid_coordinate(x, y - 1))
		reveal_area(x, y - 1, flagCount, board, displayBoard);
	//south
	if (is_valid_coordinate(x, y + 1))
		reveal_area(x, y + 1, flagCount, board, displayBoard);

	//northeast
	if (is_valid_coordinate(x + 1, y - 1))
		reveal_area(x + 1, y - 1, flagCount, board, displayBoard);
	//east
	if (is_valid_coordinate(x + 1, y))
		reveal_area(x + 1, y, flagCount, board, displayBoard);
	//southeast
	if (is_valid_coordinate(x + 1, y + 1))
		reveal_area(x + 1, y + 1, flagCount, board, displayBoard);
}


//Given a cell, figures out the nearby mines and sets the displayed value on the displayBoard. 
//Returns true if a mine was detected in an adjacent square.
bool set_mine_count(int x, int y, int board[WIDTH][HEIGHT], char displayBoard[WIDTH][HEIGHT])
{
	//Assumes that current cell does not contain a mine and that coordinates are valid

	int mineCount = 0;

	if (is_valid_coordinate(x - 1, y - 1))
		mineCount += board[x - 1][y - 1];
	if (is_valid_coordinate(x - 1, y))
		mineCount += board[x - 1][y];
	if (is_valid_coordinate(x - 1, y + 1))
		mineCount += board[x - 1][y + 1];

	if (is_valid_coordinate(x, y - 1))
		mineCount += board[x][y - 1];
	if (is_valid_coordinate(x, y + 1))
		mineCount += board[x][y + 1];

	if (is_valid_coordinate(x + 1, y - 1))
		mineCount += board[x + 1][y - 1];
	if (is_valid_coordinate(x + 1, y))
		mineCount += board[x + 1][y];
	if (is_valid_coordinate(x + 1, y + 1))
		mineCount += board[x + 1][y + 1];

	//set the mine Count on the displayboard
	if (mineCount > 0)
		displayBoard[x][y] = '0' + mineCount;
	else
		displayBoard[x][y] = '.';

	return mineCount > 0;
}


//returns true if game is won
bool flag(int x, int y, int *flagCount, char displayBoard[WIDTH][HEIGHT], char* minelocations[NUM_MINES])
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
				for (int i = 0; i < NUM_MINES; i++)
				{
					if (*minelocations[i] != CHAR_FLAG)
						return false;
				}
				return true;
			}

		}
		else if (*temp == CHAR_FLAG && *flagCount < NUM_MINES)
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
