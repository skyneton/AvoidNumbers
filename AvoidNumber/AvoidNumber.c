#include <stdio.h>
#include <Windows.h>

#define KEY_UP 1
#define KEY_DOWN -1
#define KEY_LEFT 2
#define KEY_RIGHT -2
#define KEY_SPACE 0
#define KEY_ENTER 10

#define PLAYER 19252
#define GUN 9999
#define ITEM 1952
#define GUN_DISABLE 9998

void init();
void MainScreen();
void gotoxy(int, int);
void GamePlay();

int map[50][60] = { 0 };

int main() {
	init();
	MainScreen();
	int menuID = menuDraw();

	if (menuID == 0) {
		system("cls");
		GamePlay();
	}
	
	return 0;
}

int keyControl() {
	if(kbhit()) {
		int temp = _getch();
		switch (temp) {
		case 72: return KEY_UP;
		case 80: return KEY_DOWN;
		case 75: return KEY_LEFT;
		case 77: return KEY_RIGHT;
		case 13: return KEY_ENTER;
		case 32: return KEY_SPACE;
		}
	}
	return -9999;
}

void init() {
	system("mode con cols=60 lines=50 | title P L A Y");

	for (int i = 0; i < 50; i++) {
		for (int x = 0; x < 60; x++) {
			map[i][x] = 0;
		}
	}

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = 0;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(consoleHandle, &ConsoleCursor);
}

void MainScreen() {
	printf("\n\n\n");
	printf("          ####        #             #     #       #\n");
	printf("          #   #      #            # #      #   #\n");
	printf("         ####       #           #   #      ##\n");
	printf("         #         #          #######      #\n");
	printf("        #         #######   #       #     #\n");
}

int menuDraw() {
	int y = 20;
	gotoxy(24 - 2, y);
	printf("> 게 임 시 작");
	gotoxy(24, y+2);
	printf("종       료");
	Sleep(1000);
	while (1) {
		int k = keyControl();
		switch (k) {
		case KEY_UP: case KEY_DOWN:
			gotoxy(24 - 2, y);
			printf(" ");
			if (y == 20)
				y += 2;
			else
				y -= 2;
			gotoxy(24 - 2, y);
			printf(">");
			break;
		case KEY_ENTER: case KEY_SPACE:
			return y - 20;
		}
	}
}

void gotoxy(int x, int y) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(consoleHandle, pos);
}

int CanGun = 0;

void GamePlay() {
	double speed = 35;
	int x = 29, y = 48;
	int damageDouble = 0;
	int max_spawn = 1;
	int next_stage = 30;
	int spawn_per = 1;
	int move = 3;
	int stage = 0;
	int num = 0;
	map[y][x - 1] = PLAYER;
	map[y][x] = PLAYER;
	map[y][x + 1] = PLAYER;
	map[y + 1][x] = PLAYER;
	gotoxy(x - 1, y);
	printf("000");
	gotoxy(x, y + 1);
	printf("0");
	while (1) {
		num++;
		if (num >= next_stage) {
			next_stage = 570;
			num = 0;
			stage++;
			if (spawn_per <= 3) {
				max_spawn++;
				spawn_per++;
			}
		}
		gotoxy(1, 1);
		printf("STAGE %d", stage);

		if (stage < 0) stage = 30;
		if (speed > 20)
			speed -= 0.2;
		if (move < 0) move = 3;
		move--;
		if (rand() % spawn_per && rand() % spawn_per && rand() % spawn_per) {
			int a = ((rand() % max_spawn) + (rand() % max_spawn)) / 2;
			gotoxy(1, 1);
			for (int i = 1; i <= a; i++) {
				map[2][(rand()%56)+2] = (rand() % 9) + 1;
			}
		}

		if (CanGun > 0) CanGun--;
		if (damageDouble > 0) damageDouble--;

		if (move) {
			for (int mapY = 48; mapY >= 2; mapY--) {
				for (int mapX = 2; mapX <= 57; mapX++) {
					switch (map[mapY][mapX]) {
					case GUN_DISABLE:
						map[mapY][mapX] = GUN;
						break;
					case GUN:
						if (mapY - 1 < 2) {
							map[mapY][mapX] = 0;
							gotoxy(mapX, mapY);
							printf(" ");
							continue;
						}
						if (map[mapY - 1][mapX] > 0 && map[mapY - 1][mapX] <= 9) {
							if (damageDouble) {
								if (map[mapY - 1][mapX] - 2 < 0)
									map[mapY - 1][mapX] = 0;
								else
									map[mapY - 1][mapX] -= 2;
							}
							else
								map[mapY - 1][mapX]--;
						}
						else if (map[mapY - 1][mapX] == ITEM) {
							damageDouble = 5;
							map[mapY - 1][mapX] = GUN_DISABLE;
						}
						else {
							map[mapY - 1][mapX] = GUN_DISABLE;
						}
						map[mapY][mapX] = 0;
						gotoxy(mapX, mapY);
						printf(" ");
						if (map[mapY - 1][mapX] == GUN_DISABLE) {
							gotoxy(mapX, mapY - 1);
							printf("*");
						}
						else if (map[mapY - 1][mapX] != 0) {
							gotoxy(mapX, mapY - 1);
							printf("%d", map[mapY - 1][mapX]);
						}
						break;
					case ITEM:
						break;
					case PLAYER:
						continue;
					case 0:
						continue;
					default:
						if (mapY + 1 > 48) {
							map[mapY][mapX] = 0;
							gotoxy(mapX, mapY);
							printf(" ");
							continue;
						}
						if (map[mapY + 1][mapX] == GUN || map[mapY + 1][mapX] == GUN_DISABLE) {
							if (damageDouble) {
								if (map[mapY][mapX] - 2 < 0) {
									gotoxy(mapX, mapY);
									printf(" ");
									map[mapY][mapX] = 0;
									continue;
								}
								else
									map[mapY][mapX] -= 2;
							}
							else {
								map[mapY][mapX]--;
							}
						}
						if (map[mapY + 1][mapX] == PLAYER) {
							main();
							return 0;
						}
						map[mapY + 1][mapX] = map[mapY][mapX];
						map[mapY][mapX] = 0;
						gotoxy(mapX, mapY);
						printf(" ");
						gotoxy(mapX, mapY + 1);
						printf("%d", map[mapY + 1][mapX]);
					}
				}
			}
		}

		int k = keyControl();
		switch (k) {
		case KEY_UP:
			if (y > 2) {
				map[y][x - 1] = 0;
				map[y][x] = 0;
				map[y][x + 1] = 0;
				map[y + 1][x] = 0;
				gotoxy(x-1, y);
				printf("   ");
				gotoxy(x, y+1);
				printf(" ");
				y--;
				map[y][x-1] = PLAYER;
				map[y][x] = PLAYER;
				map[y][x+1] = PLAYER;
				map[y+1][x] = PLAYER;
				gotoxy(x - 1, y);
				printf("000");
				gotoxy(x, y+1);
				printf("0");
			}
			break;
		case KEY_DOWN:
			if (y < 48) {
				map[y][x - 1] = 0;
				map[y][x] = 0;
				map[y][x + 1] = 0;
				map[y + 1][x] = 0;
				gotoxy(x - 1, y);
				printf("   ");
				gotoxy(x, y + 1);
				printf(" ");
				y++;
				map[y][x - 1] = PLAYER;
				map[y][x] = PLAYER;
				map[y][x + 1] = PLAYER;
				map[y + 1][x] = PLAYER;
				gotoxy(x - 1, y);
				printf("000");
				gotoxy(x, y + 1);
				printf("0");
			}
			break;
		case KEY_LEFT:
			if (x > 2) {
				map[y][x - 1] = 0;
				map[y][x] = 0;
				map[y][x + 1] = 0;
				map[y + 1][x] = 0;
				gotoxy(x - 1, y);
				printf("   ");
				gotoxy(x, y + 1);
				printf(" ");
				x--;
				map[y][x - 1] = PLAYER;
				map[y][x] = PLAYER;
				map[y][x + 1] = PLAYER;
				map[y + 1][x] = PLAYER;
				gotoxy(x - 1, y);
				printf("000");
				gotoxy(x, y + 1);
				printf("0");
			}
			break;
		case KEY_RIGHT:
			if (x < 57) {
				map[y][x - 1] = 0;
				map[y][x] = 0;
				map[y][x + 1] = 0;
				map[y + 1][x] = 0;
				gotoxy(x - 1, y);
				printf("   ");
				gotoxy(x, y + 1);
				printf(" ");
				x++;
				map[y][x - 1] = PLAYER;
				map[y][x] = PLAYER;
				map[y][x + 1] = PLAYER;
				map[y + 1][x] = PLAYER;
				gotoxy(x - 1, y);
				printf("000");
				gotoxy(x, y + 1);
				printf("0");
			}
			break;
		case 0:
			if (CanGun <= 0) {
				CanGun = 12;
				map[y - 1][x-1] = GUN;
				map[y - 1][x] = GUN;
				map[y - 1][x+1] = GUN;
				gotoxy(x, y-1);
				printf("*");
			}
		}
		Sleep(speed);
	}
}