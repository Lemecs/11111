#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "maze.h"
#include "sdl_utils.h"
#include "game.h"

/*
  Функция, которая по номеру уровня возвращает «ширину» и «высоту» лабиринта.
  В этом примере: level 1 → 21×21; level 2 → 31×31; level 3 → 41×41; ...
  Пока не превышаем MAX_LEVEL_SIZES[i], либо пока не уходим за MAX_LVL.
*/
static void getDimensionsForLevel(int level, int* outW, int* outH) {
    // Базовый пример: каждые +10 к каждой стороне (21,31,41,51,...).
    // Если нужно ограничение на max‑размер — можно здесь вернуть фиксированное max.
    int sz = 21 + (level - 1) * 10;
    // Ограничим, скажем, максимальным 81 (чтобы не слишком тяжко было отрисовывать).
    if (sz > 81) sz = 81;
    *outW = sz;
    *outH = sz;
}

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    srand((unsigned)time(NULL));

    int currentLevel = 1;

    while (1) {
        // 1) Определяем размер лабиринта для этого уровня:
        int w, h;
        getDimensionsForLevel(currentLevel, &w, &h);

        // 2) Инициализируем лабиринт и генерируем его:
        initMaze(w, h);

        // Задаём старт/финиш:
        // Пусть стартовая точка всегда (1,1), а финиш — (w-2, h-2).
        // (В любых лабиринтах с нечётными w/h эта точка гарантированно в пустом коридоре.)
        startX = 1;       startY = 1;
        finishX = w - 2;  finishY = h - 2;

        // Ставим стартовую клетку в ' ' и рекурсивно генерируем
        maze[startY][startX] = ' ';
        generateMaze(startX, startY);

        // Перед тем как запускать SDL, определим размеры окна:
        int windowW = w * CELL_SIZE;
        int windowH = h * CELL_SIZE + 30; // +30 для тайм‑бара сверху

        // 3) Инициализируем SDL:
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        if (initSDL(&window, &renderer, windowW, windowH) != 0) {
            fprintf(stderr, "[main] Ошибка инициализации SDL\n");
            freeMaze();
            return 1;
        }

        // Устанавливаем позицию игрока на старт
        playerX = startX;
        playerY = startY;

        // 4) Запускаем главный игровой цикл для этого уровня:
        int result = gameLoop(window, renderer);

        // 5) После выхода из gameLoop — разрушаем SDL, чистим maze:
        cleanupSDL(window, renderer);
        freeMaze();

        // 6) Обрабатываем результат:
        if (result == 0) {
            // Игрок дошёл до финиша → уровень пройден!
            printf("=== Level %d пройден! Переходим к уровню %d ===\n\n", currentLevel, currentLevel + 1);
            currentLevel++;
            // Если мы упёрлись в максимальное разрешение (например, 81×81), 
            // но хотим оставить «бесконечную» прокрутку, можно сбросить currentLevel = 1;
            // Или просто продолжить генерировать 81×81 каждый раз.
            continue;
        }
        else if (result == 1) {
            // Игрок закрыл окно — выходим из всей программы.
            printf("Вы вышли из игры (SDL_QUIT). До свидания!\n");
            break;
        }
        else if (result == 2) {
            // Время вышло на этом уровне — показываем сообщение и выходим.
            printf("Время вышло! Вы проиграли на уровне %d.\n", currentLevel);
            break;
        }
        else {
            // Неизвестный код — просто выходим.
            printf("Неизвестный код возврата из gameLoop: %d\n", result);
            break;
        }
    }

    return 0;
}
