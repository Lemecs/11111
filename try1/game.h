#pragma once
#ifndef GAME_H
#define GAME_H

#include <SDL.h>

/*
  Запускает главный цикл игры для уже сгенерированного лабиринта (maze[][]).
  Window и Renderer уже созданы.

  Возвращает:
    0  — игрок дошёл до финиша,
    1  — игрок закрыл окно (SDL_QUIT),
    2  — время вышло (тайм‑аут).
*/
int gameLoop(SDL_Window* window, SDL_Renderer* renderer);

#endif // GAME_H
