#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<string.h>

#define GAMEHISTORYSIZE 50

void gotoxy(int x, int y);
void title();
int menu();
int getGameType();
void singlePlayer();
void multiPlayer();
void printWin();
int movement(int player);
void boardprint();
void characterPrint();
void drop(int column, int player);
void removePiece(int column);
int checkStatus();
int tieChecker();
int horizontalChecker(int row, int column);
int verticalChecker(int row, int column);
int rightDiagonalChecker(int row, int column);
int leftDiagonalChecker(int row, int columm);
void hideCursor();
int getRandomMove();
int isLegalMove(int column);
int winningMove(int player);
int safeMove();
void gameExport();
void gameImport();
void helpMenu();
void view(int *p);
void winInGameImport();

enum turn { PLAYER1 = 1, PLAYER2 } currentTurn;
enum status { TIE, PLAYER1WIN, PLAYER2WIN, CONTINUE } gameStatus;
enum type { SINGLEPLAYER, MULTIPLAYER } gameType;

int piecePosition[6][7] = { 0 }; // array for position of pieces
char player1Name[20];
char player2Name[20];
int gameHistory[GAMEHISTORYSIZE] = { 0 };

int turn = 1;



int main() {

	srand(time(NULL));

	while (1) {

		turn = 1;

		for (int i = 0; i < 6; i++) {   // reset the position array to 0 each time
			for (int j = 0; j < 7; j++) {
				piecePosition[i][j] = 0;
			}
		}

		hideCursor(); // hide the cursor
		gameType = menu(); // get the type of game by calling the menu function

		if (gameType == SINGLEPLAYER) { // input the name of user for multiplayer game
			title(); // print the title on the screen
			gotoxy(41, 4);
			printf("SINGLEPLAYER");
			gotoxy(20, 7);
			printf("Enter player's name: ");
			gets(player1Name);
			strupr(player1Name);
			singlePlayer();
		}
		else if (gameType == MULTIPLAYER) { // input names of both players for multiplayer game
			title();
			gotoxy(41, 4);
			printf("MULTIPLAYER");
			gotoxy(25, 7);
			printf("Enter first player's name: ");
			gets(player1Name);
			strupr(player1Name);
			gotoxy(25, 8);
			fflush(stdin);
			printf("Enter second player's name: ");
			gets(player2Name);
			strupr(player2Name);
			multiPlayer();
		}
	}
}

void gotoxy(int x, int y) { // move the cursor to the coordinates given as arguments

	COORD c; // predefiend structure
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void title() { // print the title of the game in a colourful format

	gotoxy(37, 2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	printf("~ * ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("F");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
	printf("O");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("UR IN A R");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	printf("O");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("W");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
	printf(" * ~");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

int menu() { // display an interactive menu to the user and return game type to main

	while (1) {

		char ch;
		system("cls"); // clear the screen
		title(); // print the title
		gotoxy(42, 5);
		printf("1. New game");
		gotoxy(42, 6);
		printf("2. Import game");
		gotoxy(42, 7);
		printf("3. Controls");
		gotoxy(42, 8);
		printf("4. Rules");
		gotoxy(42, 9);
		printf("5. Exit");
		ch = getch(); // get choice from user
		system("cls");
		switch (ch) {
		case '1':
			return getGameType(); // get whther user wants to play a multipler or singleplayer game
		case '2':
			gameImport(); // game import
			continue;
		case '3': // show the controls of the game to the user
			title(); 
			gotoxy(43, 4);
			printf("CONTROLS");
			gotoxy(25, 6);
			printf("Use arrow buttons to move piece left or right.");
			gotoxy(25, 7);
			printf("Press \"space\" to drop the piece in the boxes available.");
			gotoxy(25, 8);
			printf("Blue pieces are for Player 1 and red pieces are for Player 2.");
			gotoxy(40, 11);
			printf("Press any key to go back.");
			getch();
			getch();
			continue;
		case '4':
			title();
			gotoxy(45, 4);
			printf("RULES");
			gotoxy(20, 6);
			printf("Four in a row is game for two. The board is composed of six rows and seven columns");
			gotoxy(20, 7);
			printf("The board stands on its bottom side and the pieces are dropped from the top.");
			gotoxy(20, 8);
			printf("If a column is empty, the new piece drops all the way to the bottom of the board.");
			gotoxy(20, 9);
			printf("If there already are pieces in a column, the new piece drops on the top of the old ones.");
			gotoxy(20, 10);
			printf("Both players have 21 pieces and they take turns dropping them. The player who first gets a ");
			gotoxy(20, 11);
			printf("row of four of their pieces either vertically, horizontally or diagonally, wins the game. The");
			gotoxy(20, 12);
			printf("game ends in a draw if the board gets full, but neither of the players has formed a row of four.");
			gotoxy(40, 15);
			printf("Press any key to go back.");
			getch();
			getch();
			continue;
			break;
		case '5':
			exit(0); // exit the game if 5 is pressed
		}
	}
}

int getGameType() { // return the type of game by providing options to the user

	char choice;
	system("cls"); // clear the screen
	title(); // print the title
	gotoxy(42, 5);
	printf("1. Single Player");
	gotoxy(42, 6);
	printf("2. Multi Player");
	getch();
	choice = getch(); // get the user's choise
	system("cls");

	switch (choice) { // return the user's choice

	case '1':
		return SINGLEPLAYER;
	case '2':
		return MULTIPLAYER;

	}
}

void singlePlayer() {

	gameStatus = CONTINUE; 
	strcpy(player2Name, "COMPUTER"); // player 2 is the computer for singleplayer game

	while (1) {

		currentTurn = PLAYER1;
		boardprint();
		drop(movement(currentTurn), currentTurn);
		boardprint();
		gameStatus = checkStatus();

		if (gameStatus != CONTINUE) { // if there is a tie or either of the players have won, break from the loop
			break;
		}

		currentTurn = PLAYER2;
		boardprint();
		Sleep(1000);

		int move = winningMove(currentTurn); // icheck if AI has a winning move

		if (move != -1) { // if the AI has a winning move, drop the piece there
			drop(move, currentTurn);
		}

		else if ((move = winningMove(PLAYER1)) != -1) { // incase AI is not winning then it sees if the user is winning, then blocks the user
			drop(move, currentTurn);
		}


		else { // get a safe move which does not benifit the user
			drop(safeMove(), currentTurn);
		}

		gameStatus = checkStatus(); // check the status of the game

		if (gameStatus != CONTINUE) { // break if either player has won, or if it's a tie
			break;
		}

		boardprint();

	}

	turn--; // for correct turn
	boardprint();
	printWin(); // print the status of the game after it ends
	getch();
	getch();
}


void multiPlayer() {

	gameStatus = CONTINUE;

	while (1) {
		currentTurn = PLAYER1;
		boardprint();
		drop(movement(currentTurn), currentTurn); // drop the piece in the column returned by movement function
		boardprint();
		gameStatus = checkStatus(); // check the status of the game after every drop
		if (gameStatus != CONTINUE) {// break if either player has won, or if it's a tie
			break;
		}

		currentTurn = PLAYER2;
		boardprint();
		drop(movement(currentTurn), currentTurn);
		gameStatus = checkStatus();
		boardprint();

		if (gameStatus != CONTINUE) { // break if either player has won, or if it's a tie
			break;
		}

		boardprint();
	}

	turn--; // for correct turn
	boardprint();
	printWin(); // print the status of the game after it ends
	getch();
	getch();

}

void printWin() { // print the winner's name or whether it was a tie

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	if (gameStatus == TIE) {

		gotoxy(53, 3);
		printf("TIE!");
	}

	if (gameStatus == PLAYER1WIN) {

		gotoxy(51 - strlen(player1Name) / 2, 3); // to align the player's name properly
		printf("%s WINS!", player1Name);
	}

	if (gameStatus == PLAYER2WIN) {
		gotoxy(51 - strlen(player2Name) / 2, 3); // to align the player's name properly
		printf("%s WINS!", player2Name);
	}

	gameExport(); // export the game to a file at the end
}

int movement(int player) { // current player is passed as argument to determine colour

	int colour;

	if (player == PLAYER1)
		colour = 9; // blue colour

	else
		colour = 12; // red colour

	int x = 42; // initial coordinates
	int y = 6;
	char ch;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour);
	gotoxy(x, y);
	printf("O"); // the piece which should appear on the screen
	while (1)
	{
		ch = getch();

		if (ch == ' ') {

			if (isLegalMove((x - 42) / 4) == 0) // check if the move is legal
				continue;

			else
				break;
		}

		switch (ch) {

		case 75: // left arrow key
			if (x == 42) // move the cursor to the right most column if it reaches left boundary
				x = 70;
			x -= 4;
			boardprint();
			gotoxy(x, y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour);
			printf("O");
			break;

		case 77: // right arrow key
			x += 4;
			if (x > 66) // move the cursor back to starting position if it reaches right boundary
				x = 42;
			boardprint();
			gotoxy(x, y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour);
			printf("O");
			break;
		}
	}

	return ((x - 42) / 4); // return the column where user wants to drop the piece
}

void boardprint() {

	int colour;
	system("cls");
	if (currentTurn == PLAYER1)
		colour = 9;
	else
		colour = 12;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour);
	gotoxy(51, 1);
	printf("TURN %d", turn); // print the current turn

	if (currentTurn == PLAYER1) {

		gotoxy(54 - strlen(player1Name) / 2, 2); // to align the player's name properly
		puts(player1Name);
	}

	else if (currentTurn == PLAYER2) {

		gotoxy(54 - strlen(player2Name) / 2, 2); // to align the player's name properly
		puts(player2Name);
	}

	int x = 40, y = 8;
	gotoxy(x, y);

	for (int i = 0; i < 6; i++) { // to display the columns on the screen

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("|   |   |   |   |   |   |   |\n");
		gotoxy(x, ++y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
		printf("-----------------------------\n");
		gotoxy(x, ++y);
	}

	characterPrint();  // print the characters that have been dropped throughout the game
}

void characterPrint() // print the characters on the screen
{
	int x = 42, y = 18;
	for (int row = 0; row < 6; row++) {

		for (int column = 0; column < 7; column++) // loop to print pieces in each row
		{
			gotoxy(x, y);
			if (piecePosition[row][column] == 1) {// check if it's a blue piece or a red piece
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
				printf("O");
			}
			if (piecePosition[row][column] == 2) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				printf("O");
			}

			x += 4; // increase x-coordinate so that entire row can be filled if rings are present
		}

		x = 42;  // reinitializing the x-coordinate back to column zero
		y -= 2; // decreasr y-coordinate to shift cursor to upper row

	}
}

void drop(int column, int player) { // drop the ring in the correct row and column, taking player and column number as input

	for (int row = 0; row < 6; row++) {

		if (piecePosition[row][column] != 0) { // search for the correct row
			continue;
		}

		else {

			if (player == PLAYER1) { // drop the piece to the correct row and column in the array of piece positions

				piecePosition[row][column] = 1;
				break;
			}
			else {
				piecePosition[row][column] = 2;
				break;
			}
		}
	}

	gameHistory[turn - 1] = column;
	turn++; // increase turn number
}

void removePiece(int column) { // remove the highest piece in the given column

	for (int row = 5; row >= 0; row--) { // search through each row until a piece is found, then remove it

		if (piecePosition[row][column] != 0) {

			piecePosition[row][column] = 0;
			gameHistory[turn] = 0;
			turn--; // reduce turn number
			break;
		}
	}
}

int checkStatus() { // check the status, whether either player has won or if it's a tie

	int winner = 0;

	for (int row = 0; row < 6; row++) { // horizontal checker starting from bottom row

		for (int column = 0; column < 4; column++) { // check each column for which horizontal win is possible

			winner = horizontalChecker(row, column); // check if there is a winner by having 4 pieces in a row horizontally

			if (winner != 0) // if player wins through horizontal connection, function terminates and returns the winner
				return winner;
		}
	}

	for (int column = 0; column < 7; column++) // vertical checker starting from left column
	{
		for (int row = 0; row < 3; row++) // check each row for which vertical win is possible
		{
			winner = verticalChecker(row, column); // check if there is a winner by having 4 pieces in a column vertically

			if (winner != 0)
				return winner; // if player wins through horizontal connection, function terminates and returns the winner

		}
	}

	for (int column = 0; column < 4; column++) { // right diagonal checker starting from left column

		for (int row = 0; row < 3; row++) { // check each row for which right diagonal win is possible

			winner = rightDiagonalChecker(row, column); // check if there is a winner by having 4 pieces in a row diagonally

			if (winner != 0)
				return winner; // if player wins through right diagonal connection, function terminates and returns the winner

		}
	}

	for (int column = 6; column >= 3; column--) { // right diagonal checker starting from right column

		for (int row = 0; row < 3; row++) { // check each row for which left diagonal win is possible

			winner = leftDiagonalChecker(row, column); // check if there is a winner by having 4 pieces in a row diagonally

			if (winner != 0)
				return winner; // if player wins through right diagonal connection, function terminates and returns the winner
		}
	}

	if (tieChecker() == 1) // if the board gets full, the tie checker returns 1 and TIE is returned to the calling function
		return TIE;

	return CONTINUE; // if there is neither tie nor any winner, the game continues
}

int tieChecker() { // return 1 if the board is filled and 0 otherwise

	for (int column = 0; column < 7; column++) {

		if (piecePosition[5][column] == 0) // checks the top row if it is filled for each column
			return 0;
	}

	return 1;
}

int horizontalChecker(int row, int column) {

	int currentElement = piecePosition[row][column]; // assign the current element for which connection is being checked
	
	if (currentElement != 0) { // check for connection only if current box is not empty
	
		for (int i = 1; i < 4; i++) { // check if there are three same pieces on the right
		
			if (piecePosition[row][column + i] != currentElement) { // if other player's piece is found or the box being checked is empty
				return 0;
			}
		}
		return currentElement; // return the player if it is a win
	}

	return 0;
}

int verticalChecker(int row, int column) {

	int currentElement = piecePosition[row][column]; // assign the current element for which connection is being checked
	
	if (currentElement != 0) { // check for connection only if current box is not empty

		for (int i = 1; i < 4; i++) { // check if there are three same pieces above the current piece
		
			if (piecePosition[row + i][column] != currentElement) { // if other player's piece is found or the box being checked is empty
				return 0;
			}
		}
		return currentElement; // return the player if it is a win
	}

	return 0;
}

int rightDiagonalChecker(int row, int column) {

	int currentElement = piecePosition[row][column]; // assign the current element for which connection is being checked

	if (currentElement != 0) { // check for connection only if current box is not empty

		for (int i = 1; i < 4; i++) { // check if there are three same pieces right diagonally from the current piece

			if (piecePosition[row + i][column + i] != currentElement) { // if other player's piece is found or the box being checked is empty
				return 0;
			}
		}
		return currentElement; // return the player if it is a win
	}

	return 0;
}

int leftDiagonalChecker(int row, int column) {

	int currentElement = piecePosition[row][column]; // assign the current element for which connection is being checked
	
	if (currentElement != 0) { // check for connection only if current box is not empty

		for (int i = 1; i < 4; i++) { // check if there are three same pieces left diagonally from the current piece
		
			if (piecePosition[row + i][column - i] != currentElement) { // if other player's piece is found or the box being checked is empty
			
				return 0;
			}
		}
		return currentElement; // return the player if it is a win
	}

	return 0;
}


void hideCursor() { // to hide the cursor

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}



int getRandomMove() { // random legal move generator

	int move;

	while (1) {
		move = rand() % 7;

		if (isLegalMove(move) == 1) // return the move if it is legal
			return move;
	}
}


int isLegalMove(int column) { // check if dropping a piece in the column is legal

	if (piecePosition[5][column] == 0) // check if there is a piece present on the top row
		return 1;

	else
		return 0;
}

int winningMove(int player) { // check if the player given as parameter has any winning move

	for (int column = 0; column < 7; column++) {

		if (isLegalMove(column) == 0) // if the move is not legal, check another column
			continue;

		drop(column, player); // drop in the column

		if (checkStatus() == player) { // check if dropping the piece lets the player win, if it does then remove the piece and return that column
			removePiece(column);
			return column;
		}

		removePiece(column);
	}

	return -1;
}

int safeMove() { // get a move that does not benifit the user

	int column;
	int movesChecked[7] = { 0 }; // array of moves that have been checked

	for (int column = 0; column < 7; column++) {

		if (isLegalMove(column) == 0) // illegal moves will not be returned by getRandomMove(), so setting them to 1
			movesChecked[column] = 1;
	}

	while (1) {

		column = getRandomMove(); // assign random legal column number
		movesChecked[column] = 1; // set checked status of that column to 1
		drop(column, PLAYER2); // drop the piece in that random column

		if (isLegalMove(column) == 1) { // check if dropping in the same column again is a legal move

			drop(column, PLAYER1); // drop user's piece in the same column

			if (checkStatus() != PLAYER1WIN) { // after dropping, check status and if user isn't winning, then remove the pieces and return the move
				removePiece(column); // remove player's piece
				removePiece(column); // remove AI's piece
				break;
			}

			else {
				removePiece(column); // remove player's piece

				int allChecked = 1;

				for (int column = 0; column < 7; column++) { // check if all columns have been checked

					if (movesChecked[column] == 0) {
						allChecked = 0;
						break;
					}
				}

				if (allChecked == 1) { // if the user is winning for every possible move by the AI, then return the current random legal move

					removePiece(column); // remove AI's piece
					return column;

				}

			}
		}

		else { // if dropping the piece makes the next move in that column illegal, return the move as it does not benifit the player

			removePiece(column); // remove the AI's piece
			break;
		}

		removePiece(column); // remove the AI's piece
	}

	return column;
}

void gameExport(void) { // export game to file

	FILE *filePtr;
	int temp;
	char filename[10] = "1";
	while (1) { // check which name should be assigned to saved game
		strcat(filename, ".txt");
		filePtr = fopen(filename, "r");
		if (filePtr != NULL) {
			temp = atoi(filename);
			temp++;
			itoa(temp, filename, 10);
		}
		else
			break;
	}
	filePtr = fopen(filename, "w");
	fputs(player1Name, filePtr); // export the player's name
	fputc('\n', filePtr); // new line in file
	fputs(player2Name, filePtr);
	fputc('\n', filePtr);

	for (int i = 0; i < turn; i++) { // export game turn by turn
		fputc(gameHistory[i], filePtr);
	}

	fclose(filePtr);
}

void gameImport() { // import previously saved game

	FILE *fp;
	turn = 1;
	int temp;
	int gameRecovered[50];
	char filename[10];
	title();
	gotoxy(39, 4);
	printf("IMPORTING GAME");

	char savedfiles[10] = "1";

	while (1) { // calculate the number of previously saved games

		strcat(savedfiles, ".txt");
		fp = fopen(savedfiles, "r");

		if (fp != NULL) {
			temp = atoi(savedfiles);
			temp++;
			itoa(temp, savedfiles, 10);
		}

		else
			break;
	}

	temp = atoi(savedfiles);
	temp--;
	itoa(temp, savedfiles, 10);
	gotoxy(30, 12);
	printf("Available games: 1-%s", savedfiles); // show the number of saved games/files available
	gotoxy(25, 7);
	printf("Enter game number: "); // take file number to display from user
	gets(filename);
	strcat(filename, ".txt"); // add extension to end of file number entered by user
	fp = fopen(filename, "r");

	if (fp == NULL) { // print error if file does not exist
		gotoxy(28, 9);
		printf("File doesn't exist.");
		getch();
		getch();
		return 1;
	}

	else {

		fgets(player1Name, 19, fp); // import player1's name from file
		fgets(player2Name, 19, fp); // import player2's name from file

		gameRecovered[turn - 1] = fgetc(fp); // import the turns from file one by one and store them in array

		while (!feof(fp)) { // continue till the end of file is reached

			if (turn % 2 == 0) { // checking which colour to drop in the board 
				currentTurn = PLAYER2;
				drop(gameRecovered[turn - 1], currentTurn);
			}

			else {
				currentTurn = PLAYER1;
				drop(gameRecovered[turn - 1], currentTurn);
			}

			gameRecovered[turn - 1] = fgetc(fp); // automatically read another character
		}
		turn--; // for correct turn

		player1Name[strlen(player1Name) - 1] = '\0'; // end imported string with NULL character
		player2Name[strlen(player2Name) - 1] = '\0'; // end imported string with NULL character
		winInGameImport(); // print the board
		view(gameRecovered); // call view fucntion so that user can view imported game turn by turn
		getch();
		fclose(fp);
	}

	while (turn >= 1) { // remove all pieces in case any piece is left for cleaner view next time/game
		removePiece(gameRecovered[turn - 1]);
	}

	turn = 1;
}

void helpMenu() { // help menu for user

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	gotoxy(87, 11);
	printf("Press \"Left Key\" to rewind.");
	gotoxy(87, 12);
	printf("Press \"Right Key\" to go forward.");
	gotoxy(87, 13);
	printf("Press \"Esc\" to exit view mode.");
}



void view(int *p) { // allow user to view the imported game turn by turn
	char ch;
	int bound = turn; // upper bound
	while (1) {
		helpMenu(); // print help menu
		ch = getch(); // get command from user.

		if (ch == 27) {  // break if user presses "Esc"
			break;
		}

		else {

			(turn % 2 == 0) ? (currentTurn = PLAYER1) : (currentTurn = PLAYER2); // even turn is for player 1, odd for player 2, for character printing and turn showing

			switch (ch) {
			case 75: // left key for rewinding

				if (turn > 1) { // lower bound
					removePiece(*(p + turn - 1)); // rewind game by removing pieces and turn
					boardprint();
				}
				break;

			case 77: // right key for forwarding

				if (bound == turn) {
					winInGameImport(); // if user reaches upper bound, then print win
				}

				else {
					drop(*(p + turn), currentTurn); // forward game by adding pieces and turn
					boardprint();
					if (bound == turn)
						winInGameImport(); // if user reaches upper bound, then print win
				}
				break;
			}
		}
	}
}

void winInGameImport() { // print the win status for game import function

	(turn % 2 == 0) ? (currentTurn = PLAYER2) : (currentTurn = PLAYER1); // depends upon the last turn, if it is even then player 2 wins, else player 1 wins
	boardprint();

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	if (turn == 42) {
		gotoxy(53, 3);
		printf("TIE!");
	}
	if (turn % 2 == 0) {
		gotoxy(51 - strlen(player2Name) / 2, 3);
		printf("%s WINS!", player2Name);
	}
	else {
		gotoxy(51 - strlen(player1Name) / 2, 3);
		printf("%s WINS!", player1Name);
	}
}