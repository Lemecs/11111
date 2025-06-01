#pragma once
// sdl_utils.h
#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <SDL.h>

/*
   Инициализировать SDL: создаёт окно размером width×height и рендерер.
   Вернёт 0 при успехе, иначе ненулевой код ошибки.
*/
int initSDL(SDL_Window** outWindow, SDL_Renderer** outRenderer, int width, int height);

/*
   Очистить за собой: destruir renderer, window и вызвать SDL_Quit().
*/
void cleanupSDL(SDL_Window* window, SDL_Renderer* renderer);

#endif // SDL_UTILS_H
