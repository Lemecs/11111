#include "game.h"
#include "maze.h"
#include <SDL.h>

/*
  Вспомогательная функция: отрисовать весь лабиринт текущих размеров:
  - рисуем стены (maze[i][j] == '#'),
  - рисуем старт (startX, startY) синим квадратиком,
  - рисуем финиш (finishX, finishY) зелёным,
  - рисуем игрока (playerX, playerY) красным,
  - рисуем тайм‑бар сверху.
*/
static void renderScene(SDL_Renderer* ren, Uint32 elapsed) {
    // 1) Очистить экран белым
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderClear(ren);

    // 2) Рисуем стены (черные квадраты)
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    for (int i = 0; i < mazeHeight; i++) {
        for (int j = 0; j < mazeWidth; j++) {
            if (maze[i][j] == '#') {
                SDL_Rect r = {
                    j * CELL_SIZE,
                    i * CELL_SIZE + 30, // смещаем вниз, чтобы зарезервировать место для тайм‑бара
                    CELL_SIZE,
                    CELL_SIZE
                };
                SDL_RenderFillRect(ren, &r);
            }
        }
    }

    // 3) Рисуем старт (синий квадратик)
    SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
    SDL_Rect sr = {
        startX * CELL_SIZE,
        startY * CELL_SIZE + 30,
        CELL_SIZE,
        CELL_SIZE
    };
    SDL_RenderFillRect(ren, &sr);

    // 4) Рисуем финиш (зелёный квадратик)
    SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
    SDL_Rect fr = {
        finishX * CELL_SIZE,
        finishY * CELL_SIZE + 30,
        CELL_SIZE,
        CELL_SIZE
    };
    SDL_RenderFillRect(ren, &fr);

    // 5) Рисуем игрока (красный квадратик)
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_Rect pr = {
        playerX * CELL_SIZE,
        playerY * CELL_SIZE + 30,
        CELL_SIZE,
        CELL_SIZE
    };
    SDL_RenderFillRect(ren, &pr);

    // 6) Тайм‑бар сверху: показываем оставшееся время
    float ratio = 1.0f - (float)elapsed / TIME_LIMIT_MS;
    if (ratio < 0) ratio = 0;
    int barWidth = (int)(mazeWidth * CELL_SIZE * ratio);

    // Фон‑бар (серый)
    SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
    SDL_Rect bgBar = { 0, 0, mazeWidth * CELL_SIZE, 30 };
    SDL_RenderFillRect(ren, &bgBar);

    // Оставшееся время (зелёный)
    SDL_SetRenderDrawColor(ren, 0, 200, 0, 255);
    SDL_Rect fgBar = { 0, 0, barWidth, 30 };
    SDL_RenderFillRect(ren, &fgBar);

    // 7) Present всё на экран
    SDL_RenderPresent(ren);
}

/*
  Самый главный цикл — пока игрок не дошел до финиша,
  не закрыл окно и не вышло время. Возвращает код исхода.
*/
int gameLoop(SDL_Window* window, SDL_Renderer* renderer) {
    Uint32 startTime = SDL_GetTicks();
    SDL_Event e;
    int running = 1;

    while (running) {
        Uint32 elapsed = SDL_GetTicks() - startTime;

        // Проверяем тайм‑аут
        if (elapsed >= TIME_LIMIT_MS) {
            return 2; // время вышло
        }

        // Обрабатываем события
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
                break;
            }
            if (e.type == SDL_KEYDOWN) {
                int nx = playerX, ny = playerY;
                switch (e.key.keysym.sym) {
                case SDLK_w: ny--; break;
                case SDLK_s: ny++; break;
                case SDLK_a: nx--; break;
                case SDLK_d: nx++; break;
                default: break;
                }
                // Проверяем границы и нет ли стен:
                if (nx >= 0 && nx < mazeWidth && ny >= 0 && ny < mazeHeight) {
                    if (maze[ny][nx] == ' ' ||
                        (nx == startX && ny == startY) ||
                        (nx == finishX && ny == finishY)) {
                        playerX = nx;
                        playerY = ny;
                    }
                }
            }
        }

        // Если закрыл окно — выходим
        if (!running) {
            return 1;
        }
        // Если дошёл до финиша — выходим с кодом «успех»
        if (playerX == finishX && playerY == finishY) {
            return 0;
        }

        // Отрисовываем текущую сцену
        renderScene(renderer, elapsed);

        // Задержка (~60 FPS)
        SDL_Delay(16);
    }

    return 1;
}
