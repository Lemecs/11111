#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "maze.h"
#include "sdl_utils.h"
#include "game.h"

/*
   Вспомогательная функция: по номеру уровня (1,2,3, …)
   возвращает нужные габариты mazeWidth × mazeHeight.
   В этом примере: уровень 1 → 21×21, уровень 2 → 31×31, уровень 3 → 41×41 и т.д.
   Не даём выйти за пределы 81×81.
*/
static void getDimensionsForLevel(int level, int* outW, int* outH) {
    int size = 21 + (level - 1) * 10;
    if (size > 81) size = 81;
    *outW = size;
    *outH = size;
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    srand((unsigned)time(NULL));

    int currentLevel = 1;

    while (1) {
        // 1) Определяем размеры для текущего уровня:
        int w, h;
        getDimensionsForLevel(currentLevel, &w, &h);

        // 2) Инициализируем и генерируем лабиринт:
        initMaze(w, h);

        // Стартуем всегда из (1,1):
        startX = 1;
        startY = 1;
        maze[startY][startX] = ' ';

        // Рекурсивная генерация:
        generateMaze(startX, startY);

        // 3) Теперь находим самую удалённую клетку от (startX, startY):
        findFarthestCell(startX, startY, &finishX, &finishY);
        // finishX, finishY теперь в самой «дальней» точке графа.

        // 4) Считаем размер окна под SDL:
        int windowW = w * CELL_SIZE;
        int windowH = h * CELL_SIZE + 30; // +30 пикселей для тайм‑бара

        // 5) Инициализируем SDL:
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        if (initSDL(&window, &renderer, windowW, windowH) != 0) {
            fprintf(stderr, "[main] SDL initialization failed. Exiting.\n");
            freeMaze();
            return 1;
        }

        // Устанавливаем игрока в стартовую точку:
        playerX = startX;
        playerY = startY;

        // 6) Запускаем игровой цикл для этого уровня:
        int result = gameLoop(window, renderer);

        // 7) После завершения уровня чистим SDL и лабиринт:
        cleanupSDL(window, renderer);
        freeMaze();

        // 8) Обрабатываем результат:
        if (result == 0) {
            // Игрок дошёл до финиша вовремя:
            printf("=== Level %d completed! Moving to level %d ===\n\n",
                currentLevel, currentLevel + 1);
            currentLevel++;
            // Переходим к следующему уровню:
            continue;
        }
        else if (result == 1) {
            // Игрок закрыл окно (SDL_QUIT):
            printf("You exited the game (SDL_QUIT). Goodbye!\n");
            break;
        }
        else if (result == 2) {
            // Время истекло:
            printf("Time's up! You lost on level %d.\n", currentLevel);
            break;
        }
        else {
            // На всякий случай:
            printf("Unknown return code from gameLoop: %d. Exiting.\n", result);
            break;
        }
    }

    return 0;
}
