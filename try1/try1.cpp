#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 41   // Ширина лабиринта (должна быть нечетной)
#define HEIGHT 41  // Высота лабиринта (должна быть нечетной)
#define CELL_SIZE 20  // Размер клетки в пикселях
#define WINDOW_WIDTH (WIDTH * CELL_SIZE)
#define WINDOW_HEIGHT (HEIGHT * CELL_SIZE + 30) // добавлено место для таймера

// Время в миллисекундах, через которое игра завершится (например, 60 секунд)
#define TIME_LIMIT_MS (60 * 1000)

char maze[HEIGHT][WIDTH];
int playerX, playerY;
const int startX = 0, startY = 1;
const int finishX = WIDTH - 1, finishY = HEIGHT - 2;

void initMaze() {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            maze[i][j] = '#';
}

void shuffle(int* array, int n) {
    for (int i = 0; i < n; i++) {
        int r = i + rand() % (n - i);
        int tmp = array[i]; array[i] = array[r]; array[r] = tmp;
    }
}

void generateMaze(int x, int y) {
    int dirs[4] = { 0,1,2,3 }; shuffle(dirs, 4);
    for (int i = 0; i < 4; i++) {
        int dx = 0, dy = 0;
        if (dirs[i] == 0) dy = 2;
        if (dirs[i] == 1) dy = -2;
        if (dirs[i] == 2) dx = 2;
        if (dirs[i] == 3) dx = -2;
        int nx = x + dx, ny = y + dy;
        if (nx > 0 && nx < WIDTH - 1 && ny > 0 && ny < HEIGHT - 1 && maze[ny][nx] == '#') {
            maze[y + dy / 2][x + dx / 2] = ' ';
            maze[ny][nx] = ' ';
            generateMaze(nx, ny);
        }
    }
}

int SDL_main(int argc, char** argv) {
    srand((unsigned)time(NULL));
    initMaze();
    maze[1][1] = ' ';
    generateMaze(1, 1);

    playerX = startX;
    playerY = startY;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Maze Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    Uint32 startTime = SDL_GetTicks();
    int running = 1;
    SDL_Event e;
    while (running) {
        Uint32 elapsed = SDL_GetTicks() - startTime;
        if (elapsed >= TIME_LIMIT_MS) break;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { running = 0; break; }
            if (e.type == SDL_KEYDOWN) {
                int nx = playerX, ny = playerY;
                switch (e.key.keysym.sym) {
                case SDLK_w: ny--; break;
                case SDLK_s: ny++; break;
                case SDLK_a: nx--; break;
                case SDLK_d: nx++; break;
                }
                if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                    if (maze[ny][nx] == ' ' || (nx == startX && ny == startY) || (nx == finishX && ny == finishY)) {
                        playerX = nx;
                        playerY = ny;
                    }
                }
            }
        }
        if (!running || (playerX == finishX && playerY == finishY)) break;

        // Рендеринг игрового поля
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);
        // стены
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (maze[i][j] == '#') {
                    SDL_Rect r = { j * CELL_SIZE, i * CELL_SIZE + 30, CELL_SIZE, CELL_SIZE };
                    SDL_RenderFillRect(ren, &r);
                }
            }
        }
        // Старт
        SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
        SDL_Rect sr = { startX * CELL_SIZE, startY * CELL_SIZE + 30, CELL_SIZE, CELL_SIZE };
        SDL_RenderFillRect(ren, &sr);
        // Финиш
        SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
        SDL_Rect fr = { finishX * CELL_SIZE, finishY * CELL_SIZE + 30, CELL_SIZE, CELL_SIZE };
        SDL_RenderFillRect(ren, &fr);
        // Игрок
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        SDL_Rect pr = { playerX * CELL_SIZE, playerY * CELL_SIZE + 30, CELL_SIZE, CELL_SIZE };
        SDL_RenderFillRect(ren, &pr);

        // таймер-бар сверху
        float ratio = 1.0f - (float)elapsed / TIME_LIMIT_MS;
        int barWidth = WINDOW_WIDTH * ratio;
        // Фон бар (серый)
        SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
        SDL_Rect bgBar = { 0, 0, WINDOW_WIDTH, 30 };
        SDL_RenderFillRect(ren, &bgBar);
        // Остаток (зелёный)
        SDL_SetRenderDrawColor(ren, 0, 200, 0, 255);
        SDL_Rect fgBar = { 0, 0, barWidth, 30 };
        SDL_RenderFillRect(ren, &fgBar);

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
