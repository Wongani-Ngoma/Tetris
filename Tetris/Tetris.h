//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ---------------------------------------------------------------------
//-----------------------      Tetris.h      ---------------------------
//----  Contains Type definitions, globals and function prototypes  ----
//----------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <iostream>
#include <conio.h>
#include <time.h>

enum BlockType { A, B, C, D, E, F, G }; //Defines type of block's type, eg square, L-shaped, etc
enum BlockRotation { a, b, c, d }; //Defines the block's current rotation, rotations are:  eg 90, -90, 180 & -180

struct Block {
	BlockType blockType;
	BlockRotation blockRotation;
	int x; //Block's x-coordinate on the board
	int y; ////Block's y-coordinate on the board
};

//The board. The first and last row as well as the first and last column are occupied by the wall
const int height = 44;
const int width = 24;
char board[height][width];

bool gameover;
int level;
int score;

Block block; //The current block being controlled by the player
//This is to restore a block's previous rotation in case the block changes rotation but clips through other objects
//in other words, the block changes rotation and makes putToBoard return a 0
BlockRotation tempRotation;

//The location where a newely generated block will be placed
constexpr int startX = 8;
constexpr int startY = 1;

//Returns the next rotation type block br should take depending on it's current rotation
BlockRotation nextRotation(BlockRotation br) {
	tempRotation = block.blockRotation;
	if (br == d)  return a;
	if (br == a)  return b;
	if (br == b)  return c;
	if (br == c)  return d;
	return a;
}

//Fills the board array with walls and empty spaces for for the blocks
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

//Clears all elements in a particular row on the board
void clearRow(int row) {
	for (int e = 1; e < width - 1; e++) {
		board[row][e] = ' ';
	}
}

//The name is self explainatorty, rowToStartDropping is the row from which it will start dropping the chars
void dropEveryThingDown(int rowToStartDropping) {
	for (int e = rowToStartDropping; e > 0; e--) {
		for (int r = 1; r < width - 1; r++) {
			board[e + 1][r] = board[e][r];
		}
	}
}

//Chech each row for 'fulness' and if found for fulness, clears it and drops every char in the board by one y-axis unit
//isRowCleared tells the method's caller if a row has been cleared, if so, a new block needs to be immediately generated
//or else the last block that filled the cleared row will not be cleared completely
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

//Prints the entire board on the console
void printBoard() {
	for (int i = 0; i < height; i++) {
		for (int e = 0; e < width; e++) {
			std::cout << board[i][e];
		}
		std::cout << "\n";
	}
	
}

//This puts the char t to all the spaces that can be occupied by the block, depending on it's type and rotation
//Returns 0 if the 'putting' was unsuccessful, 'putting' is unsuccessful if the space is contains something ie (space != ' '),
//that is, the space does not contain nothing, :-|
//However, if this method is trying to put ' ', that is, nothing, into the space, it should not check if the space has
//something, hence, the checking only happens when (t != ' ')
//The reason for all this is that I want this method to be general, I use it to put a block on the block and I also use it
//to remove a block from the board
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

//Tries to move the block to the left or right depending on char d, returns 0 if it couldnt move the block and 1 if it did
bool starfeBlock(char d) {
	if (d == 'a') { //Meaning left
		putToBoard(' '); //Remove the block
		block.x--; //Set the block's x_coordiante one unit left to the current coordinate
		if (putToBoard('O') == 0) { //This means the block couldnt be put left, either theres a wall or another block
			block.x++; //Reset back to original position
			return 0;
		}
		return 1;
	}
	//d == 'd',
	putToBoard(' ');
	block.x++;
	if (putToBoard('O') == 0) {
		block.x--;
		return 0;
	}
	return 1;
}

//Tries to move the block 1 unit down, returns 1 if successful, otherwise 0
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

//Simply drops the block down until it touches something, whether it be the bottom wall or another block
void dropBlockAllTheWayDown() {
	while(depressBlock() == 1) {}
}

//Tries to rotate the block, returns 1 if successful, otherwise 0
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

//Generates a random block
bool createNewBlock() {
	//Generate a random number from 1 to 4 and assign a block type based on the number
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

//Increases score(based on level and number of rows cleared) and level(bases on score)
void increaseScore(int rowsCleared) {
	score += (rowsCleared * 100) * level;
	level = (int)(score / 1000) + 1;
}

//Checks is the keyboard recieved input, if it did, it gets that input and does something based on that input
void input() {
	if (_kbhit()) {
		char c = _getch();
		if (c < 97) c += 32; //Simple conversion to lower case letter
		switch (c) {
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
		case 'z':
			dropBlockAllTheWayDown();
			break;
		case 'q':
			gameover = true;
			break;
		
		default:
			break;
		}
	}
}