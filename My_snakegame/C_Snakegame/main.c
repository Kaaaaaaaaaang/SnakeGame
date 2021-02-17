#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

// kbhit() => keyboard hit

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define PAUSE 112
#define ESC 27

#define MAP_X 3
#define MAP_Y 2
#define MAP_WIDTH 30
#define MAP_HEIGHT 20

int x[100], y[100]; 
int target_x, target_y; 
int length; 
int speed;
int score; 
int best_score = 0; 
int last_score = 0;
int dir; 
int key;
int status_on = 0; 

void gotoxy(int x, int y, char* s) { 
    COORD pos = { 2 * x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("%s", s);
}

void title(void); 
void reset(void); 
void draw_map(void);
void move(int dir);  
void pause(void);
void game_over(void);
void target(void); 
void status(void);

//-------------------------MAIN START---------------------------//

int main() {
    title();

    while (1) {
        if (_kbhit()) do { key = _getch(); } while (key == 224);
        Sleep(speed);

        switch (key) { 
        case LEFT:
        case RIGHT:
        case UP:
        case DOWN:
            if ((dir == LEFT && key != RIGHT) || (dir == RIGHT && key != LEFT) || (dir == UP && key != DOWN) ||
                (dir == DOWN && key != UP))
                dir = key;
            key = 0; 
            break;
        case PAUSE: 
            pause();
            break;
        case 115: 
            if (status_on == 0) status_on = 1;
            else status_on = 0;
            key = 0;
            break;
        case ESC: 
            exit(0);
        }
        move(dir);

        if (status_on == 1) status();
    }
}

//----------------------MAIN END--------------------------//

void title(void) {
    int i, j;

    while (_kbhit()) _getch(); 

    draw_map(); 
    for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++) { 
        for (j = MAP_X + 1; j < MAP_X + MAP_WIDTH - 1; j++) gotoxy(j, i, "  ");
    }

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 5, "+--------------------------+");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 6, "|        S N A K E         |");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 7, "+--------------------------+");

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 9, " < PRESS ANY KEY TO START > ");

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 11, "   �� ��,��,��,�� : Move      ");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 12, "   �� P : Pause the game              ");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 13, "   �� ESC : Quit the game             ");

    while (1) {
        if (_kbhit()) {
            key = _getch();
            if (key == ESC) exit(0);
            else break;
        }
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 9, " < PRESS ANY KEY TO START > ");
        Sleep(400);
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 9, "                            ");
        Sleep(400);

    }
    reset();
}

void reset(void) {
    int i;
    system("cls");
    draw_map(); 
    while (_kbhit()) _getch(); 

    dir = LEFT;
    speed = 100; 
    length = 5;
    score = 0;
    for (i = 0; i < length; i++) { 
        x[i] = MAP_WIDTH / 2 + i;
        y[i] = MAP_HEIGHT / 2;
        gotoxy(MAP_X + x[i], MAP_Y + y[i], "��");
    }
    gotoxy(MAP_X + x[0], MAP_Y + y[0], "��"); 
    target(); 
}

void draw_map(void) { 
    int i, j;
    for (i = 0; i < MAP_WIDTH; i++) {
        gotoxy(MAP_X + i, MAP_Y, "��");
    }
    for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++) {
        gotoxy(MAP_X, i, "��");
        gotoxy(MAP_X + MAP_WIDTH - 1, i, "��");
    }
    for (i = 0; i < MAP_WIDTH; i++) {
        gotoxy(MAP_X + i, MAP_Y + MAP_HEIGHT - 1, "��");
    }
}

void move(int dir) {
    int i;

    if (x[0] == target_x && y[0] == target_y) { 
        score += 10; 
        target();
        length++;
        x[length - 1] = x[length - 2]; 
        y[length - 1] = y[length - 2];
    }
    if (x[0] == 0 || x[0] == MAP_WIDTH - 1 || y[0] == 0 || y[0] == MAP_HEIGHT - 1) { 
        game_over();
        return; 
    }
    for (i = 1; i < length; i++) { 
        if (x[0] == x[i] && y[0] == y[i]) {
            game_over();
            return;
        }
    }

    gotoxy(MAP_X + x[length - 1], MAP_Y + y[length - 1], "  "); 
    for (i = length - 1; i > 0; i--) {
        x[i] = x[i - 1];
        y[i] = y[i - 1];
    }
    gotoxy(MAP_X + x[0], MAP_Y + y[0], "��"); 
    if (dir == LEFT) --x[0]; 
    if (dir == RIGHT) ++x[0];
    if (dir == UP) --y[0];
    if (dir == DOWN) ++y[0];
    gotoxy(MAP_X + x[i], MAP_Y + y[i], "��"); 
}

void pause(void) {
    while (1) {
        if (key == PAUSE) {
            gotoxy(MAP_X + (MAP_WIDTH / 2) - 9, MAP_Y, "< PAUSE : PRESS ANY KEY TO RESUME > ");
            Sleep(400);
            gotoxy(MAP_X + (MAP_WIDTH / 2) - 9, MAP_Y, "                                    ");
            Sleep(400);
        }
        else {
            draw_map();
            return;
        }
        if (_kbhit()) {

            do {
                key = _getch();
            } while (key == 224);
        }

    }
}

void game_over(void) { //�������� �Լ� 
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 5, "+----------------------+");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 6, "|      GAME OVER..     |");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 7, "+----------------------+");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 8, " YOUR SCORE : ");
    printf("%d", last_score = score);

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 12, " Press any keys to restart.. ");

    if (score > best_score) {
        best_score = score;
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 4, MAP_Y + 10, "�� BEST SCORE ��");
    }
    Sleep(500);
    while (_kbhit()) _getch();
    key = _getch();
    title();
}

void target(void) {
    int i;

    int target_crush_on = 0;
    int r = 0;
    gotoxy(MAP_X, MAP_Y + MAP_HEIGHT + 2, " YOUR SCORE: ");  
    printf("%3d, LAST SCORE: %3d, BEST SCORE: %3d", score, last_score, best_score);

    while (1) {
        target_crush_on = 0;
        srand((unsigned)time(NULL) + r); 
        target_x = (rand() % (MAP_WIDTH - 2)) + 1;  
        target_y = (rand() % (MAP_HEIGHT - 2)) + 1;

        for (i = 0; i < length; i++) { 
            if (target_x == x[i] && target_y == y[i]) {
                target_crush_on = 1; 
                r++;
                break;
            }
        }

        if (target_crush_on == 1) continue; 

        gotoxy(MAP_X + target_x, MAP_Y + target_y, "��"); 
        speed -= 3; 
        break;

    }
}

void status(void) { 
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y, "head= ");
    printf("%2d,%2d", x[0], y[0]);
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 1, "food= ");
    printf("%2d,%2d", target_x, target_y);
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 2, "leng= ");
    printf("%2d", length);
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 3, "key= ");
    printf("%3d", key);
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 4, "spd= ");
    printf("%3d", speed);
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 6, "score= ");
    printf("%3d", score);
}