#ifndef MAZE_H
#define MAZE_H

#include <SDL.h>
#include <stdlib.h>

// Размер клетки (пиксели) — статический, не меняется от уровня к уровню.
#define CELL_SIZE 20
// Тайм‑лимит одного уровня (мс).
#define TIME_LIMIT_MS (60 * 1000)

/*
   Глобальные переменные, задающие текущий размер лабиринта.
   При старте уровня мы их выставляем, а затем используем везде, где раньше стояли WIDTH/HEIGHT.
*/
extern int mazeWidth;
extern int mazeHeight;

/*
   Двумерный массив-лабиринт. Мы будем выделять его динамически:
   maze[y][x] будет либо '#' для стены, либо ' ' для свободного коридора, либо, при желании, ещё
   какие-то метки ('.' для уже пройденных клеток или 'S', 'F' и т.п.).
*/
extern char** maze;

// Позиция игрока (текущая).
extern int playerX, playerY;
// Координаты старта (будем хранить в этих глобальных переменных).
extern int startX, startY;
// Координаты финиша.
extern int finishX, finishY;

// Прототипы функций:

// Инициализировать mazeWidth/mazeHeight, выделить maze[mazeHeight][mazeWidth] и заполнить стенами ('#').
void initMaze(int width, int height);

// «Перемешать» массив ints длины n (алгоритм Fisher–Yates).
void shuffle(int* array, int n);

// Рекурсивная генерация лабиринта «от печати» – аналогично recursive backtracker.
// Стартуем из (x, y), предполагая, что maze[y][x] уже помечено как ' ' (проход).
void generateMaze(int x, int y);

// Освободить память, занятую maze (после окончания уровня или перед выходом).
void freeMaze(void);

#endif // MAZE_H
