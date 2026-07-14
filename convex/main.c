#include <SDL2/SDL.h>

#define WINDOW_X 500
#define WINDOW_Y 550
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400
#define BOX_SIZE 7
#define MAX_POINTS 8

enum action {
        QUIT,
        DEFAULT, /* For if nothing happened */
        CLICK
};

/*
 * Scan through the events to determine if the user is trying to quit or click.
 * They can quit by closing the window or pressing Escape or Q on the keyboard.
 * If they click, then determine where they clicked.
 *
 * Quit events will immediately return, disregarding all other events.
 */
enum action handle_events(SDL_Point *mouse) {
        SDL_Event event;
        enum action scan = DEFAULT;

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
                        SDL_GetMouseState(&(mouse->x), &(mouse->y));
                        scan = CLICK;
                        break;
                default:
                        break;
                }
        }

        return scan;
}

int main(void) {
        SDL_Window *window;
        SDL_Renderer *renderer;

        SDL_Rect box = {0, 0, BOX_SIZE, BOX_SIZE};

        SDL_Point points[MAX_POINTS];
        SDL_Point p0;

        enum action input_scan = DEFAULT;
        SDL_Point mouse = {0, 0};

        size_t i = 0;
        size_t p_count = 0;

        /* 
        * SDL2 initialization process 
        * (this is the messy section)
        */
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                printf("Couldn't initialize SDL: %s\n", SDL_GetError());
                return 1;
        }

        window = SDL_CreateWindow("Test", WINDOW_X, WINDOW_Y, 
                                  SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        if (window == NULL) {
                printf("Failed to open %dx%d window: %s\n", 
                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
                SDL_Quit();
                return 1;
        }

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL) {
                printf("Failed to create renderer: %s\n", SDL_GetError());
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 1;
        }


        while (input_scan != QUIT) {
                input_scan = handle_events(&mouse);

                if (input_scan == CLICK) {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderClear(renderer);

                        points[p_count].x = mouse.x;
                        points[p_count].y = mouse.y;

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

        return 0;
}
