#include "maze.h"
#include <stdio.h>
#include <time.h>

// ���������� ���������� �� maze.h
int mazeWidth, mazeHeight;
char** maze = NULL;
int playerX, playerY;
int startX, startY;
int finishX, finishY;

/**
 * initMaze:
 *   1) ���������� mazeWidth=width, mazeHeight=height.
 *   2) �������� ������ ��� maze: ������ ���������� �� ������, � ����� ���� ������.
 *   3) ��������� �� �������� '#'.
 *   4) ������������� ��������� ������ � (1,1) ��� ������, � finish ���� �� ������������� �
 *      ��� ������� � main.c (����� generateMaze).
 */
void initMaze(int width, int height) {
    mazeWidth = width;
    mazeHeight = height;

    // ����������� ������ ������ (���� ����� ��� ��� ���� ��������).
    if (maze) {
        freeMaze();
    }

    // �������� ������ ���������� �� ������:
    maze = (char**)malloc(sizeof(char*) * mazeHeight);
    if (!maze) {
        fprintf(stderr, "[initMaze] ������ malloc(mazeHeight)\n");
        exit(1);
    }

    // �������� ������ ������ � ��������� '#':
    for (int y = 0; y < mazeHeight; y++) {
        maze[y] = (char*)malloc(sizeof(char) * mazeWidth);
        if (!maze[y]) {
            fprintf(stderr, "[initMaze] ������ malloc(maze[%d])\n", y);
            exit(1);
        }
        for (int x = 0; x < mazeWidth; x++) {
            maze[y][x] = '#';
        }
    }
}

/**
 * shuffle:
 *   ����������� Fisher�Yates: ���������� ������ array ����� n.
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
 *   ���������� ������������� ������� � ���������.
 *   ���� ������� ���������� (x, y) � ���������, ��� maze[y][x] ��� = ' '.
 *   ������ ���������� �������� (�������� �� 2 ������ �� ���), ���� �� ������ ��� �������.
 */
void generateMaze(int x, int y) {
    // ������ �� ������ �����������: 0 = ����, 1 = �����, 2 = ������, 3 = �����
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
        // ���������, ��� (nx,ny) � �������� � ��� �� ��� �����:
        if (nx > 0 && nx < mazeWidth - 1 && ny > 0 && ny < mazeHeight - 1 && maze[ny][nx] == '#') {
            // ��������� �������� ����� (x,y) � (nx,ny):
            maze[y + dy / 2][x + dx / 2] = ' ';
            maze[ny][nx] = ' ';
            // ���������� �����������:
            generateMaze(nx, ny);
        }
    }
}

/**
 * freeMaze:
 *   ����������� ��� ������, ������� �maze� (������� ��� ������, ����� ��� ������ ����������).
 */
void freeMaze(void) {
    if (!maze) return;
    for (int y = 0; y < mazeHeight; y++) {
        free(maze[y]);
    }
    free(maze);
    maze = NULL;
    mazeWidth = mazeHeight = 0;
    // playerX/Y, startX/Y, finishX/Y �� ������ ���������, �� ����� ������ �� �����������.
}
