#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define WINDOW_X 500
#define WINDOW_Y 550
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400
#define BOX_SIZE 7
#define MAX_POINTS 12

enum action {
        NONE,
        CLICK,
        QUIT
};

int init(SDL_Window **window, SDL_Renderer **renderer);
enum action handle_events();

int main(void) {
        SDL_Window *window;
        SDL_Renderer *renderer;

        SDL_Rect box = {0, 0, BOX_SIZE, BOX_SIZE};

        SDL_Point points[MAX_POINTS];
        SDL_Point p0;

        enum action input_scan = NONE;

        size_t i = 0;
        size_t p_count = 0;

        if (!init(&window, &renderer)) {
                fprintf(stderr, "Initialization failed.\n");
                exit(EXIT_FAILURE);
        }

        while ((input_scan = handle_events()) != QUIT) {
                if (input_scan == CLICK) {
                        SDL_GetMouseState(&(points[p_count].x),
                                          &(points[p_count].y));

                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderClear(renderer);

                        if (p_count == 0) {
                                p0 = points[p_count];
                        } else if (p0.y > points[p_count].y) {
                                p0 = points[p_count];
                        } else if (p0.y == points[p_count].y) {
                                if (p0.x > points[p_count].x)
                                        p0 = points[p_count];
                        }

                        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                        for (i = 0; i <= p_count; ++i) {
                                box.x = points[i].x - (box.w / 2);
                                box.y = points[i].y - (box.h / 2);
                                SDL_RenderDrawRect(renderer, &box);
                        }

                        if (++p_count >= MAX_POINTS) {
                                p_count = 0;
                        }

                        box.x = p0.x - (box.w / 2);
                        box.y = p0.y - (box.h / 2);
                        SDL_RenderFillRect(renderer, &box);

                        SDL_RenderPresent(renderer);
                }
        }

        /* CLEAN UP */
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        exit(EXIT_SUCCESS);
}


/*
 * SDL2 initialization process
 * SDL_Init, and initializing the window and the renderer.
 */
int init(SDL_Window **window, SDL_Renderer **renderer) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                fprintf(stderr, "Couldn't initialize SDL: %s\n",
                        SDL_GetError());
                return 0;
        }

        *window = SDL_CreateWindow(
                "Test",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN
        );
        if (*window == NULL) {
                fprintf(stderr, "Failed to open %dx%d window: %s\n",
                        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
                SDL_Quit();
                return 0;
        }

        *renderer = SDL_CreateRenderer(
                *window,
                -1,
                SDL_RENDERER_ACCELERATED
        );
        if (*renderer == NULL) {
                fprintf(stderr, "Failed to create renderer: %s\n",
                        SDL_GetError());
                SDL_DestroyWindow(*window);
                SDL_Quit();
                return 0;
        }

        return 1;
}

/*
 * Scan through the events to determine if the user is trying to quit or click.
 * They can quit by closing the window or pressing Escape or Q on the keyboard.
 * The only other event is a click, otherwise nothing happens.
 *
 * Quit events will immediately return, disregarding all other events.
 */
enum action handle_events() {
        SDL_Event event;
        enum action scan = NONE;

        while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_QUIT:
                        return QUIT;
                case SDL_KEYDOWN:
                        switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                                return QUIT;
                        case SDLK_q:
                                return QUIT;
                        default:
                                break;
                        }
                        break;
                case SDL_MOUSEBUTTONDOWN:
                        scan = CLICK;
                        break;
                default:
                        break;
                }
        }

        return scan;
}

