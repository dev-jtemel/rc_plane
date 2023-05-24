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

  /*
  s.register_cb([&](std::vector<rcplane::common::io::packet> &packets) {
    auto res = system("clear"); 
    (void)res;

    bool test_complete = (packets[0].data() & 0x2);
    bool flight_mode = (packets[0].data() & 0x1);

    std::cout
      << "FLIGHT_MODE: " << flight_mode << " | TEST COMPLETE: " << test_complete
      << "\nPower: ["
      << std::string(packets[1].data()/2, '#')
      << std::string((255 - packets[1].data())/2, ' ')
      << "]\n"
      << "Aileron: ["
      << packets[2].data() << "*|"
      << (-1 * packets[2].data()) << "*]\n"
      << "Elevator: ["
      << packets[3].data() << "*|"
      << packets[3].data() << "*]\n"
      << "Rudder: [" << packets[4].data() << "*]" << std::endl;
  });
  */

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* window = SDL_CreateWindow("PROTOTYPE 1.2",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       1000, 1000, 0);
    SDL_Renderer* renderer = NULL;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 18, 18, 18, 255);

    SDL_RenderClear(renderer);

    
    s.register_cb([&](std::vector<rcplane::common::io::packet> &packets) {
      RCPLANE_LOG(trace, "sdl", packets[0].type_to_str());
      SDL_SetRenderDrawColor(renderer, 18, 18, 18, 255);
      SDL_RenderClear(renderer);

      SDL_Rect flight_indicator;
      flight_indicator.x = 10;
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
        test_indicator.y = 10;
        test_indicator.w = 20;
        test_indicator.h = 20;

        if (test_flags[i]) {
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        } else {
          SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        }
        SDL_RenderFillRect(renderer, &test_indicator);
      }

      /*
      SDL_Rect r;
      r.x = 50;
      r.y = 813 - packets[1].data() * 3;
      r.w = 50;
      r.h = packets[1].data() * 3;

      SDL_SetRenderDrawColor(renderer, 108, 18, 18, 255);
      SDL_RenderFillRect(renderer, &r);

      SDL_Rect r2;
      r2.x = 48;
      r2.y = 48;
      r2.w = 52;
      r2.h = 767;

      SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
      SDL_RenderDrawRect(renderer, &r2);

      SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
      SDL_RenderDrawLine(renderer, 800, 50, 900, 50 + packets[2].data());

      SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
      SDL_RenderDrawLine(renderer, 800, 200, 900, 200 - packets[2].data());
      */

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
