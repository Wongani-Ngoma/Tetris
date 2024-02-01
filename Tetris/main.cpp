/*
* Program: Tetris
* Author: Wonagni Ngoma
* Info: Just my replica of the famous game, nothing too special here.
*       I flooded the code with comments everywhere because this version is not very well designed.
*       You can change the board's size by modifying the height and width constant integers in the Tetris.h file
*		If your computer updates the game on the console too slow, it'll look bad, you'll see two or more blocks insted of one, the brighter
*		one is the real one, anyway the Sleep(int) method right after printBoard() in loop() is ment to address this issue, the game doesent run as fast,
*		but it looks better, you can remove it if your machine is fast If you still see these 'ghost' blocks, try setting the argument to Sleep to a higher value.
*/

#include <Windows.h>
#include "Tetris.h"
#include <iostream>

void loop() {
	int rowsCleared = 0;
	bool blockAlreadyCreated = false;
	while (!gameover) {
		std::cout << "Level: " << level << " Score: " << score << "\n";
		input(); //Listen for input and do what it says
		if (depressBlock() == 0) { //If the block cannot be depressed any further
			//Clear all full rows, if any, and record their number
			while (clearFullRow() == true) {
				rowsCleared++;
			}
			if (rowsCleared > 0) {
				increaseScore(rowsCleared);
				rowsCleared = 0; //Set this to 0 so as to prepare it for more row clearing
				createNewBlock(); //If a row was cleared, create another block immediately to avoid the problem addressed in the clearFullRow method
				blockAlreadyCreated = true;
			}
			//If a block was already created, creating it again, on the same location, would result in an unnecessary gameover
			if (!blockAlreadyCreated) {
				if (createNewBlock() == 0) {
					gameover = true;
				}
			}
			//If this isn't here, then the game, after the 1st row clear(s), shall always think a block has been created
			//in the if(rowsCleared > 0) nest, so it will create no block, meaning it will never have a full row,
			//so the last created block justs hangs
			blockAlreadyCreated = false;
		}
		printBoard();
		Sleep(100);
		system("cls");
	}
}

void prepareGame() {
	gameover = false;
	score = 0;
	level = 1;
	srand((unsigned int)time(NULL));
	createNewBlock();
	setupBoard();
	putToBoard('O');
	printBoard();
}

int main() {
	prepareGame();
	loop();
	system("pause");
	return 0;
}