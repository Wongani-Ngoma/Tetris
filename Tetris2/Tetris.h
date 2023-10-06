
#include <iostream>
#include <conio.h>
#include <time.h>

using namespace std;

enum BlockType { A, B, C, D, E, F, G }; //Defines type of block's current type
enum BlockRotation { a, b, c, d }; //Defines the block's current orientation

struct Block {
	BlockType blockType;
	BlockRotation blockRotation;
	//The block'coordinates on the board
	int x;
	int y;
};

//The board. The first and last row as well as the first and last column are occupied by the wall
const int height = 22;
const int width = 14;
char board[height][width];

bool gameover;
int level;
int score;

Block block;
//This is to restore a block's previous rotation in case the block changes rotation but clips through other objects
//in other words, the block hanges rotation and makes putToBoard return a 0
BlockRotation tempRotation;
constexpr int startX = 8;
constexpr int startY = 1;

BlockRotation nextRotation(BlockRotation br) {
	tempRotation = block.blockRotation;
	if (br == d)  return a;
	if (br == a)  return b;
	if (br == b)  return c;
	if (br == c)  return d;
	return a;
}
void setupBoard() {
	//Boundaries
	for (int i = 0; i < width; i++) {
		board[0][i] = '=';
	}
	for (int i = 1; i < height - 1; i++) {
		board[i][0] = '=';
		board[i][width - 1] = '=';
	}
	for (int i = 0; i < width; i++) {
		board[height - 1][i] = '=';
	}
	//Actual map
	for (int i = 1; i < height - 1; i++) {
		for (int e = 1; e < width - 1; e++) {
			board[i][e] = ' ';
		}
	}
}

void clearRow(int row) {
	for (int e = 1; e < width - 1; e++) {
		board[row][e] = ' ';
	}
}

//The name is self explainatorty, i is the row from which it will start dropping the chars
void dropEveryThingDown(int i) {
	for (int e = i; e > 0; e--) {
		for (int r = 1; r < width - 1; r++) {
			board[e + 1][r] = board[e][r];
		}
	}
}
//Chech each row for 'fulness' and if found for fulness, clears it and drops every char in the board by one y-axis unit
//isRowCleared tells the method's caller if a row has been cleared, if so, a new block needs to be immediately created
//or else the last block that filled the row will not disappear
bool clearFullRow() {
	bool isRowCleared = false;
	bool currentRowFull;
	for (int i = height - 2; i > 0; i--) { //For each row...
		currentRowFull = true; //Let's assume the current row is full
		for (int e = 1; e < width - 1; e++) { //For each column...
			if (board[i][e] == ' ') {
				currentRowFull = false;
				break;
			}
		}
		if (currentRowFull) {
			clearRow(i);
			isRowCleared = true;
			dropEveryThingDown(i - 1);
		}
	}
	return isRowCleared;
}

void drawMap() {
	for (int i = 0; i < height; i++) {
		for (int e = 0; e < width; e++) {
			cout << board[i][e];
		}
		cout << "\n";
	}
}

//This puts the char t to all the spaces that can be occupied by the block, depending on it's type and rotation
//Returns 0 if the putting was unsuccessful, putting is unsuccessful if the space is contains something (!= ' '),
//that is, the space does not contain nothing
//However, if this method is trying to put ' ', that is, nothing, into the space, it should not check if the space has
//something, hence, the checking only happens when (t != ' ')
int putToBoard(char t) {
	switch (block.blockType) {
	case A:
		if (t != ' ') {
			if (board[block.y][block.x] != ' ' || board[block.y][block.x + 1] != ' ' ||
				board[block.y + 1][block.x] != ' ' || board[block.y + 1][block.x + 1] != ' ') return 0;
		}
		board[block.y][block.x] = t;
		board[block.y][block.x + 1] = t;
		board[block.y + 1][block.x] = t;
		board[block.y + 1][block.x + 1] = t;
		break;
	case B:
		switch (block.blockRotation) {
		case a:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y][block.x - 1] != ' ' ||
					board[block.y][block.x + 1] != ' ' || board[block.y + 1][block.x] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y][block.x - 1] = t;
			board[block.y][block.x + 1] = t;
			board[block.y + 1][block.x] = t;
			break;
		case b:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y - 1][block.x] != ' ' ||
					board[block.y + 1][block.x] != ' ' || board[block.y][block.x - 1] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y - 1][block.x] = t;
			board[block.y + 1][block.x] = t;
			board[block.y][block.x - 1] = t;
			break;
		case c:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y - 1][block.x] != ' ' ||
					board[block.y][block.x + 1] != ' ' || board[block.y][block.x - 1] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y - 1][block.x] = t;
			board[block.y][block.x + 1] = t;
			board[block.y][block.x - 1] = t;
			break;
		case d:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y - 1][block.x] != ' ' ||
					board[block.y + 1][block.x] != ' ' || board[block.y][block.x + 1] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y - 1][block.x] = t;
			board[block.y + 1][block.x] = t;
			board[block.y][block.x + 1] = t;
			break;
		}
		break;
	case C:
		switch (block.blockRotation) {
		case a:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y][block.x + 1] != ' ' ||
					board[block.y][block.x + 2] != ' ' || board[block.y + 1][block.x] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y][block.x + 1] = t;
			board[block.y][block.x + 2] = t;
			board[block.y + 1][block.x] = t;
			break;
		case b:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y][block.x - 1] != ' ' ||
					board[block.y + 1][block.x] != ' ' || board[block.y + 2][block.x] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y][block.x - 1] = t;
			board[block.y + 1][block.x] = t;
			board[block.y + 2][block.x] = t;
			break;
		case c:

			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y - 1][block.x] != ' ' ||
					board[block.y][block.x - 1] != ' ' || board[block.y][block.x - 2] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y - 1][block.x] = t;
			board[block.y][block.x - 1] = t;
			board[block.y][block.x - 2] = t;
			break;
		case d:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y - 1][block.x] != ' ' ||
					board[block.y - 2][block.x] != ' ' || board[block.y][block.x + 1] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y - 1][block.x] = t;
			board[block.y - 2][block.x] = t;
			board[block.y][block.x + 1] = t;
			break;
		}
		break;
	case D:
		switch (block.blockRotation) {
		case a:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y + 1][block.x] != ' ' ||
					board[block.y][block.x - 1] != ' ' || board[block.y][block.x - 2] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y + 1][block.x] = t;
			board[block.y][block.x - 1] = t;
			board[block.y][block.x - 2] = t;
			break;
		case b:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y - 1][block.x] != ' ' ||
					board[block.y - 2][block.x] != ' ' || board[block.y][block.x - 1] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y - 1][block.x] = t;
			board[block.y - 2][block.x] = t;
			board[block.y][block.x - 1] = t;
			break;
		case c:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y - 1][block.x] != ' ' ||
					board[block.y][block.x + 1] != ' ' || board[block.y][block.x + 2] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y - 1][block.x] = t;
			board[block.y][block.x + 1] = t;
			board[block.y][block.x + 2] = t;
			break;
		case d:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y][block.x + 1] != ' ' ||
					board[block.y + 1][block.x] != ' ' || board[block.y + 2][block.x] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y][block.x + 1] = t;
			board[block.y + 1][block.x] = t;
			board[block.y + 2][block.x] = t;
			break;
		}
		break;
	case E:
		switch (block.blockRotation) {
		case a:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y + 1][block.x] != ' ' ||
					board[block.y + 1][block.x + 1] != ' ' || board[block.y][block.x - 1] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y + 1][block.x] = t;
			board[block.y + 1][block.x + 1] = t;
			board[block.y][block.x - 1] = t;
			break;
		case b:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y - 1][block.x] != ' ' ||
					board[block.y + 1][block.x - 1] != ' ' || board[block.y][block.x - 1] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y - 1][block.x] = t;
			board[block.y + 1][block.x - 1] = t;
			board[block.y][block.x - 1] = t;
			break;
		case c:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y + 1][block.x] != ' ' ||
					board[block.y + 1][block.x + 1] != ' ' || board[block.y][block.x - 1] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y + 1][block.x] = t;
			board[block.y + 1][block.x + 1] = t;
			board[block.y][block.x - 1] = t;
			break;
		case d:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y - 1][block.x] != ' ' ||
					board[block.y + 1][block.x - 1] != ' ' || board[block.y][block.x - 1] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y - 1][block.x] = t;
			board[block.y + 1][block.x - 1] = t;
			board[block.y][block.x - 1] = t;
			break;
		}
		break;
	case F:
		switch (block.blockRotation) {
		case a:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y + 1][block.x] != ' ' ||
					board[block.y + 1][block.x - 1] != ' ' || board[block.y][block.x + 1] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y + 1][block.x] = t;
			board[block.y + 1][block.x - 1] = t;
			board[block.y][block.x + 1] = t;
			break;
		case b:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y + 1][block.x] != ' ' ||
					board[block.y - 1][block.x - 1] != ' ' || board[block.y][block.x - 1] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y + 1][block.x] = t;
			board[block.y - 1][block.x - 1] = t;
			board[block.y][block.x - 1] = t;
			break;
		case c:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y + 1][block.x] != ' ' ||
					board[block.y + 1][block.x - 1] != ' ' || board[block.y][block.x + 1] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y + 1][block.x] = t;
			board[block.y + 1][block.x - 1] = t;
			board[block.y][block.x + 1] = t;
			break;
		case d:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y + 1][block.x] != ' ' ||
					board[block.y - 1][block.x - 1] != ' ' || board[block.y][block.x - 1] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y + 1][block.x] = t;
			board[block.y - 1][block.x - 1] = t;
			board[block.y][block.x - 1] = t;
			break;
		}
		break;
	case G:
		switch (block.blockRotation) {
		case a:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y][block.x - 1] != ' ' ||
					board[block.y][block.x + 1] != ' ' || board[block.y][block.x + 2] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y][block.x - 1] = t;
			board[block.y][block.x + 1] = t;
			board[block.y][block.x + 2] = t;
			break;
		case b:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y - 1][block.x] != ' ' ||
					board[block.y + 1][block.x] != ' ' || board[block.y + 2][block.x] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y - 1][block.x] = t;
			board[block.y + 1][block.x] = t;
			board[block.y + 2][block.x] = t;
			break;
		case c:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y][block.x + 1] != ' ' ||
					board[block.y][block.x - 1] != ' ' || board[block.y][block.x - 2] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y][block.x + 1] = t;
			board[block.y][block.x - 1] = t;
			board[block.y][block.x - 2] = t;
			break;
		case d:
			if (t != ' ') {
				if (board[block.y][block.x] != ' ' || board[block.y - 1][block.x] != ' ' ||
					board[block.y - 2][block.x] != ' ' || board[block.y + 1][block.x] != ' ') return 0;
			}
			board[block.y][block.x] = t;
			board[block.y - 1][block.x] = t;
			board[block.y - 2][block.x] = t;
			board[block.y + 1][block.x] = t;
			break;
		}
		break;
	default:
		break;
	}
	return 1;
}

bool starfeBlock(char d) {
	if (d == 'a') {
		putToBoard(' ');
		block.x--;
		if (putToBoard('O') == 0) {
			block.x++;
			return 0;
		}
		return 1;
	}
	//d == 'd'
	putToBoard(' ');
	block.x++;
	if (putToBoard('O') == 0) {
		block.x--;
		return 0;
	}
	return 1;
}

bool depressBlock() {
	putToBoard(' ');
	block.y++;
	if (putToBoard('O') == 0) {
		block.y--;
		putToBoard('O');
		return 0;
	}
	return 1;
}

bool rotateBlock() {
	putToBoard(' ');
	tempRotation = block.blockRotation;
	block.blockRotation = nextRotation(tempRotation);
	if (putToBoard('O') == 0) {
		block.blockRotation = tempRotation;
		return 0;
	}
	return 1;
}

bool createNewBlock() {
	//Generate a random number from 1 to 4 and assign a bllock type based on the number
	switch (1 + (int)rand() % 7) {
	case 1:
		block.blockType = A;
		break;
	case 2:
		block.blockType = B;
		break;
	case 3:
		block.blockType = C;
		break;
	case 4:
		block.blockType = D;
		break;
	case 5:
		block.blockType = E;
		break;
	case 6:
		block.blockType = F;
		break;
	case 7:
		block.blockType = G;
		break;
	}
	//The new block's rotation must always start at a, blocks with this orientation never clip through the ceiling
	block.blockRotation = a;
	block.x = startX;
	block.y = startY;

	if (putToBoard('O') == 0) {
		return 0;
	}
	return 1;
}

void increaseScore(int rowsCleared) {
	score += (rowsCleared * 100) * level;
	level = (int)(score / 1000) + 1;
}

void input() {
	if (_kbhit()) {
		switch (_getch()) {
		case 's':
			depressBlock();
			break;
		case 'a':
			starfeBlock('a');
			break;
		case 'd':
			starfeBlock('d');
			break;
		case 'r':
			rotateBlock();
			break;
		case 'q':
			gameover = true;
			break;
		case 'S':
			depressBlock();
			break;
		case 'A':
			starfeBlock('a');
			break;
		case 'D':
			starfeBlock('d');
			break;
		case 'R':
			rotateBlock();
			break;
		case 'Q':
			gameover = true;
			break;
		default:
			break;
		}
	}
}