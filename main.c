#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

HANDLE mutex;
struct sendInfo* inf;
char field[9];

struct sendInfo {
	char field[9];
	bool playerXturn,
		playerOturn;
};

int checkPlayerXWin(char field[9]) {
	if (field[0] == 'X' && field[1] == 'X' && field[2] == 'X') { // X X X
		return 1;												// 3 4 5
	}															// 6 7 8
	else if (field[3] == 'X' && field[4] == 'X' && field[5] == 'X') {// 0 1 2
		return 1;													// X X X
	}																// 6 7 8
	else if (field[6] == 'X' && field[7] == 'X' && field[8] == 'X') {	// 0 1 2
		return 1;													// 3 4 5
	}																// X X X
	else if (field[0] == 'X' && field[3] == 'X' && field[6] == 'X') {		// X 1 2
		return 1;														// X 4 5
	}																	// X 7 8
	else if (field[1] == 'X' && field[4] == 'X' && field[7] == 'X') {	// 0 X 2
		return 1;													// 3 X 5
	}																// 6 X 8
	else if (field[2] == 'X' && field[5] == 'X' && field[8] == 'X') {		// 0 1 X
		return 1;														// 3 4 X
	}																	// 6 7 X
	else if (field[0] == 'X' && field[4] == 'X' && field[8] == 'X') {	// X 1 2
		return 1;													// 3 X 5
	}																// 6 7 X
	else if (field[2] == 'X' && field[4] == 'X' && field[6] == 'X') {		// 0 1 X
		return 1;														// 3 X 5
	}																	// X 7 8
	else {
		return 0;
	}
}

int checkPlayerOWin(char field[9]) {
	if (field[0] == 'O' && field[1] == 'O' && field[2] == 'O') {// O O O
		return 1;												// 3 4 5
	}															// 6 7 8
	else if (field[3] == 'O' && field[4] == 'O' && field[5] == 'O') {// 0 1 2
		return 1;													// O O O
	}																// 6 7 8
	else if (field[6] == 'O' && field[7] == 'O' && field[8] == 'O') {	// 0 1 2
		return 1;														// 3 4 5
	}																	// O O O
	else if (field[0] == 'O' && field[3] == 'O' && field[6] == 'O') {		// O 1 2
		return 1;															// O 4 5
	}																		// O 7 8
	else if (field[1] == 'O' && field[4] == 'O' && field[7] == 'O') {	// 0 O 2
		return 1;														// 3 O 5
	}																	// 6 O 8
	else if (field[2] == 'O' && field[5] == 'O' && field[8] == 'O') {// 0 1 O
		return 1;													// 3 4 O
	}																// 6 7 O
	else if (field[0] == 'O' && field[4] == 'O' && field[8] == 'O') {	// O 1 2
		return 1;														// 3 O 5
	}																	// 6 7 O
	else if (field[2] == 'O' && field[4] == 'O' && field[6] == 'O') {// 0 1 O
		return 1;													// 3 O 5
	}																// O 7 8
	else {
		return 0;
	}
}

void printField(char field[9]) {
	for (int i = 0; i < 9; i++) {
		printf("%c", (field[i] == ' ') ? (i + '0') : field[i]);

		if (i == 2 || i == 5) {
			printf("\n--+---+--\n");
		}
		else if (i != 8) {
			printf(" | ");
		}
	}

	printf("\n");
}

bool checkValidInput(char field[9], int input) {
	return (input >= 0 && input <= 8 && 
		inf->field[input] != 'X' && inf->field[input] != 'O');
}

int main(int argc, char* argv[]) {
	char* lpName = argv[1];
	bool isPlayerX = false;

	mutex = CreateMutex(0, 0, 0);

	HANDLE map = OpenFileMapping(FILE_MAP_ALL_ACCESS,
		0,
		lpName);

	if (!map) {
		isPlayerX = true;
		map = CreateFileMapping(0,
			0,
			PAGE_READWRITE,
			0,
			sizeof(struct sendInfo),
			lpName);
	}

	inf = (struct sendInfo*) MapViewOfFile(map,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(struct sendInfo));

	if (isPlayerX) {
		memset(inf->field, ' ', 9);
		inf->playerXturn = true;
		inf->playerOturn = false;
	}

	while (true) {
		WaitForSingleObject(mutex, INFINITE);

		if (checkPlayerXWin(inf->field)) {
			printf("X VICTORY!\n");
			break;
		}

		if (checkPlayerOWin(inf->field)) {
			printf("O VICTORY!\n");
			break;
		}

		if ((!isPlayerX ^ inf->playerXturn)) {
			int input;

			printField(inf->field);
			printf("%c: ", isPlayerX ? 'X' : 'O');
			scanf("%d", &input);

			if (checkValidInput(field, input)) {
				inf->field[input] = isPlayerX ? 'X' : 'O';
				inf->playerXturn = !inf->playerXturn;
				inf->playerOturn = !inf->playerOturn;

				printField(inf->field);
			}
		}

		ReleaseMutex(mutex);
	}

	return 0;
}