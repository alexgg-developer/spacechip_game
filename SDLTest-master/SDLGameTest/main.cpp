#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SDL.h>
#include <SDL_image.h>
#elif defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#endif

#include <iostream>

int main(int, char **) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Window *win =
      SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
  if (win == nullptr) {
    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(
      win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr) {
    SDL_DestroyWindow(win);
    std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  SDL_Surface *space_ship_surface = IMG_Load("Assets/Player/spaceship.png");
  if (!space_ship_surface) {
    std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
    return 0;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, space_ship_surface);
  if (!texture) {
    std::cout << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
    return 0;
  }

  SDL_Event e;
  bool running = true;
  while (running) {
    SDL_PollEvent(&e);
    switch (e.type) {
      case SDL_QUIT:
        running = false;
        break;
      case SDL_MOUSEBUTTONDOWN:
        break;
      case SDL_KEYDOWN:
        break;
      case SDL_USEREVENT:
        break;
      default:
        break;
    }

    SDL_Rect texture_rect;
    texture_rect.x = 0;   // the x coordinate
    texture_rect.y = 0;   // the y coordinate
    texture_rect.w = 64;  // the width of the texture
    texture_rect.h = 64;  // the height of the texture

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, &texture_rect);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(space_ship_surface);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(win);
  SDL_Quit();

  return 0;
}
