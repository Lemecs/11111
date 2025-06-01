#include "maze.h"
#include <stdio.h>
#include <time.h>

// Глобальные переменные из maze.h
int mazeWidth, mazeHeight;
char** maze = NULL;
int playerX, playerY;
int startX, startY;
int finishX, finishY;

/**
 * initMaze:
 *   1) Записывает mazeWidth=width, mazeHeight=height.
 *   2) Выделяет память под maze: массив указателей на строки, а потом сами строки.
 *   3) Заполняет всё символом '#'.
 *   4) Устанавливает стартовую клетку в (1,1) как проход, а finish пока не устанавливаем —
 *      это сделаем в main.c (после generateMaze).
 */
void initMaze(int width, int height) {
    mazeWidth = width;
    mazeHeight = height;

    // Освобождаем старую память (если вдруг она уже была выделена).
    if (maze) {
        freeMaze();
    }

    // Выделяем массив указателей на строки:
    maze = (char**)malloc(sizeof(char*) * mazeHeight);
    if (!maze) {
        fprintf(stderr, "[initMaze] Ошибка malloc(mazeHeight)\n");
        exit(1);
    }

    // Выделяем каждую строку и заполняем '#':
    for (int y = 0; y < mazeHeight; y++) {
        maze[y] = (char*)malloc(sizeof(char) * mazeWidth);
        if (!maze[y]) {
            fprintf(stderr, "[initMaze] Ошибка malloc(maze[%d])\n", y);
            exit(1);
        }
        for (int x = 0; x < mazeWidth; x++) {
            maze[y][x] = '#';
        }
    }
}

/**
 * shuffle:
 *   Стандартный Fisher–Yates: перемешать массив array длины n.
 */
void shuffle(int* array, int n) {
    for (int i = 0; i < n - 1; i++) {
        int r = i + rand() % (n - i);
        int tmp = array[i];
        array[i] = array[r];
        array[r] = tmp;
    }
}

/**
 * generateMaze:
 *   Рекурсивно «прокладывает» проходы в лабиринте.
 *   Берёт текущие координаты (x, y) — считается, что maze[y][x] уже = ' '.
 *   Строит деревянные «мостики» (двигаясь по 2 клетки за раз), пока не выйдет «из массива».
 */
void generateMaze(int x, int y) {
    // Массив из четырёх направлений: 0 = вниз, 1 = вверх, 2 = вправо, 3 = влево
    int dirs[4] = { 0, 1, 2, 3 };
    shuffle(dirs, 4);

    for (int i = 0; i < 4; i++) {
        int dx = 0, dy = 0;
        switch (dirs[i]) {
        case 0: dy = 2; break;
        case 1: dy = -2; break;
        case 2: dx = 2; break;
        case 3: dx = -2; break;
        }
        int nx = x + dx;
        int ny = y + dy;
        // Проверяем, что (nx,ny) в пределах и это всё ещё стена:
        if (nx > 0 && nx < mazeWidth - 1 && ny > 0 && ny < mazeHeight - 1 && maze[ny][nx] == '#') {
            // Пробиваем «коридор» между (x,y) и (nx,ny):
            maze[y + dy / 2][x + dx / 2] = ' ';
            maze[ny][nx] = ' ';
            // Рекурсивно углубляемся:
            generateMaze(nx, ny);
        }
    }
}

/**
 * freeMaze:
 *   Освобождает всю память, занятую «maze» (сначала все строки, потом сам массив указателей).
 */
void freeMaze(void) {
    if (!maze) return;
    for (int y = 0; y < mazeHeight; y++) {
        free(maze[y]);
    }
    free(maze);
    maze = NULL;
    mazeWidth = mazeHeight = 0;
    // playerX/Y, startX/Y, finishX/Y мы просто оставляем, но новые уровни их перезапишут.
}
