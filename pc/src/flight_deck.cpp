#include <iostream>

#include <thread>
#include <SDL2/SDL.h>
#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/packet.hpp"
#include "rcplane/common/io/serial.hpp"

int main(int argc, char *argv[]) {
  RCPLANE_SEVERITY(trace);

  RCPLANE_LOG(trace, "test", 1);
  RCPLANE_LOG(debug, "test", 2);
  RCPLANE_LOG(info, "test", 3);
  RCPLANE_LOG(warn, "test", 4);
  RCPLANE_LOG(error, "test", 5);

  rcplane::common::io::serial s;

  // TODO CLEAN THIS UP
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* window = SDL_CreateWindow("PROTOTYPE 1.2",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       250, 220, 0);
    SDL_Renderer* renderer = NULL;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 18, 18, 18, 255);

    SDL_RenderClear(renderer);

    
    s.register_cb([&](std::vector<rcplane::common::io::packet> &packets) {
      SDL_SetRenderDrawColor(renderer, 18, 18, 18, 255);
      SDL_RenderClear(renderer);

      SDL_Rect flight_indicator;
      flight_indicator.x = 40;
      flight_indicator.y = 10;
      flight_indicator.w = 20;
      flight_indicator.h = 20;

      bool test_flags[] = {
        (packets[0].data() & 0x2),
        (packets[0].data() & 0x4),
        (packets[0].data() & 0x8),
        (packets[0].data() & 0x10),
      };

      bool flight_mode = (packets[0].data() & 0x1);

      if (flight_mode) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
      } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      }
      SDL_RenderFillRect(renderer, &flight_indicator);

      for (int i = 0; i < 4; ++i) {
        SDL_Rect test_indicator;
        test_indicator.x = 40 + (i * 30);
        test_indicator.y = 40;
        test_indicator.w = 20;
        test_indicator.h = 20;

        if (test_flags[i]) {
          SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        } else {
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        SDL_RenderFillRect(renderer, &test_indicator);
      }

      SDL_Rect power_box;
      power_box.x = 10;
      power_box.y = 10;
      power_box.w = 20;
      power_box.h = 200;
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderDrawRect(renderer, &power_box);

      int pwr = packets[1].data() * 0.78039215686;
      SDL_Rect power_indicator;
      power_indicator.x = 11;
      power_indicator.y = 209 - pwr;
      power_indicator.w = 18;
      power_indicator.h = pwr;

      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      SDL_RenderFillRect(renderer, &power_indicator);

      for (int i = 0; i < 5; ++i) {
        auto y = 70 + 30 *i;
        SDL_Rect border;
        border.x = 40;
        border.y = y;
        border.w = 200;
        border.h = 20;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &border);

        SDL_Rect value;
        value.x = 140;
        value.y = y;
        value.h = 19;
        switch (i) {
          case 0:
            value.w = -packets[2].data();
            break;
          case 1:
            value.w = packets[2].data();
            break;
          case 2:
          case 3:
            value.w = packets[3].data();
            break;
          case 4:
            value.w = packets[4].data();
            break;
        }
        value.w *= 2;

        SDL_SetRenderDrawColor(renderer, 0, 191, 255, 255);
        SDL_RenderFillRect(renderer, &value);

        for (int j = 1; j <10; ++j) {
          SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
          SDL_RenderDrawLine(renderer, 40 + 20 * j, y + 1, 40 + 20 * j, y + 19);
        }
      }

      SDL_RenderPresent(renderer);
    });

    std::thread serial_thread([&]() {
      s.read_serial();
    });
    serial_thread.detach();


    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
      if (event.type == SDL_QUIT) {
        break;
      }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
