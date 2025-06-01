#include "maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Глобальные переменные из maze.h:
int mazeWidth, mazeHeight;
char** maze = NULL;
int playerX, playerY;
int startX, startY;
int finishX, finishY;

/**
 * initMaze:
 *   1) Записывает mazeWidth=width, mazeHeight=height.
 *   2) Выделяет память под maze: массив строк [mazeHeight][mazeWidth].
 *   3) Заполняет каждый символ '#'.
 */
void initMaze(int width, int height) {
    mazeWidth = width;
    mazeHeight = height;

    // Если ранее уже был выделен maze, то очищаем его:
    if (maze) {
        freeMaze();
    }

    // Выделяем массив указателей на строки:
    maze = (char**)malloc(sizeof(char*) * mazeHeight);
    if (!maze) {
        fprintf(stderr, "[initMaze] malloc failed for maze rows\n");
        exit(1);
    }

    // Для каждой строки выделяем память и заполняем '#'
    for (int y = 0; y < mazeHeight; y++) {
        maze[y] = (char*)malloc(sizeof(char) * mazeWidth);
        if (!maze[y]) {
            fprintf(stderr, "[initMaze] malloc failed for maze[%d]\n", y);
            exit(1);
        }
        memset(maze[y], '#', mazeWidth);
    }
}

/**
 * shuffle:
 *   Перемешивает int-массив длины n (алгоритм Fisher–Yates).
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
 *   Рекурсивно «прокладывает» коридоры.
 *   Предполагается, что maze[y][x] уже = ' ' (проход).
 */
void generateMaze(int x, int y) {
    int dirs[4] = { 0, 1, 2, 3 };
    shuffle(dirs, 4);

    for (int i = 0; i < 4; i++) {
        int dx = 0, dy = 0;
        switch (dirs[i]) {
        case 0: dy = 2;  break; // вниз
        case 1: dy = -2; break; // вверх
        case 2: dx = 2;  break; // вправо
        case 3: dx = -2; break; // влево
        }
        int nx = x + dx;
        int ny = y + dy;
        // Проверяем, что (nx, ny) в пределах и там стена:
        if (nx > 0 && nx < mazeWidth - 1 &&
            ny > 0 && ny < mazeHeight - 1 &&
            maze[ny][nx] == '#')
        {
            // Пробиваем стену между (x,y) и (nx,ny):
            maze[y + dy / 2][x + dx / 2] = ' ';
            maze[ny][nx] = ' ';
            // Рекурсивно продолжаем
            generateMaze(nx, ny);
        }
    }
}

/**
 * findFarthestCell:
 *   Из стартовой клетки (sx,sy) выполняет BFS по всему лабиринту,
 *   возвращая координаты клетки, наиболее удалённой (макс. расстояние).
 */
void findFarthestCell(int sx, int sy, int* outX, int* outY) {
    int h = mazeHeight;
    int w = mazeWidth;
    // Создаём 2D‑массив dist[h][w], инициализируем -1:
    int** dist = (int**)malloc(sizeof(int*) * h);
    if (!dist) {
        fprintf(stderr, "[findFarthestCell] malloc failed for dist pointers\n");
        exit(1);
    }
    for (int i = 0; i < h; i++) {
        dist[i] = (int*)malloc(sizeof(int) * w);
        if (!dist[i]) {
            fprintf(stderr, "[findFarthestCell] malloc failed for dist[%d]\n", i);
            exit(1);
        }
        for (int j = 0; j < w; j++) {
            dist[i][j] = -1;
        }
    }

    // Очередь (qx, qy) размера не менее h*w
    int total = h * w;
    int* qx = (int*)malloc(sizeof(int) * total);
    int* qy = (int*)malloc(sizeof(int) * total);
    if (!qx || !qy) {
        fprintf(stderr, "[findFarthestCell] malloc failed for queue\n");
        exit(1);
    }

    int head = 0, tail = 0;
    // Запускаем BFS из (sx,sy)
    dist[sy][sx] = 0;
    qx[tail] = sx;
    qy[tail] = sy;
    tail++;

    int bestX = sx, bestY = sy;
    int maxD = 0;

    // Смещения по 4 соседям
    const int ddx[4] = { 1, -1,  0,  0 };
    const int ddy[4] = { 0,  0,  1, -1 };

    while (head < tail) {
        int cx = qx[head];
        int cy = qy[head];
        int cd = dist[cy][cx];
        head++;

        // Обновляем «максимум»
        if (cd > maxD) {
            maxD = cd;
            bestX = cx;
            bestY = cy;
        }

        // Просматриваем 4 соседа
        for (int k = 0; k < 4; k++) {
            int nx = cx + ddx[k];
            int ny = cy + ddy[k];
            if (nx >= 0 && nx < w && ny >= 0 && ny < h) {
                if (maze[ny][nx] == ' ' && dist[ny][nx] < 0) {
                    dist[ny][nx] = cd + 1;
                    qx[tail] = nx;
                    qy[tail] = ny;
                    tail++;
                }
            }
        }
    }

    // Возвращаем найденную самую удалённую точку
    *outX = bestX;
    *outY = bestY;

    // Освобождаем память
    for (int i = 0; i < h; i++) {
        free(dist[i]);
    }
    free(dist);
    free(qx);
    free(qy);
}

/**
 * freeMaze:
 *   Освобождает всю память, занятую maze.
 */
void freeMaze(void) {
    if (!maze) return;
    for (int y = 0; y < mazeHeight; y++) {
        free(maze[y]);
    }
    free(maze);
    maze = NULL;
    mazeWidth = mazeHeight = 0;
}
