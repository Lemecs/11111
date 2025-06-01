#ifndef MAZE_H
#define MAZE_H

#include <SDL.h>
#include <stdlib.h>

// Размер одной клетки в пикселях
#define CELL_SIZE 20
// Тайм‑лимит на один уровень (мс)
#define TIME_LIMIT_MS (60 * 1000)

/*
   Текущие размеры лабиринта. Устанавливаются при инициализации уровня.
*/
extern int mazeWidth;
extern int mazeHeight;

/*
   Двумерный массив-лабиринт. Будет выделяться динамически:
   maze[y][x] == '#'  — стена
   maze[y][x] == ' '  — свободный коридор
*/
extern char** maze;

// Координаты игрока внутри лабиринта (динамические).
extern int playerX, playerY;
// Координаты старта (обычно 1,1) и финиша (будем вычислять через BFS).
extern int startX, startY;
extern int finishX, finishY;

// Выделяет память и заполняет лабиринт символом '#':
//   mazeWidth = width, mazeHeight = height
//   maze = malloc(...), заполнить '#' по всем ячейкам.
//   (после этого в main вы помечаете maze[startY][startX]=' ' и вызываете generateMaze)
void initMaze(int width, int height);

// Перемешивает массив целых чисел длины n (алгоритм Fisher–Yates).
void shuffle(int* array, int n);

// Генерирует рекурсивно «дерево коридоров» от клетки (x,y). Предполагается, что
// maze[y][x] уже = ' ' (проход). Алгоритм похож на «recursive backtracker».
void generateMaze(int x, int y);

// Находит из точки (sx,sy) клетку лабиринта, наиболее удалённую по графовому расстоянию,
// и записывает её координаты в *outX и *outY. Реализовано через BFS по всему лабиринту.
void findFarthestCell(int sx, int sy, int* outX, int* outY);

// Освобождает всю память, занятую maze.
void freeMaze(void);

#endif // MAZE_H
