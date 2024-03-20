#define _COSMO_SOURCE

#include <libc/isystem/iostream>
#include <libc/isystem/memory>
#include <libc/isystem/string>
#include <libc/nt/console.h>
#include <libc/dce.h>

#include "cosmo_sdl2.hpp"

int32_t main() {
  SDL2::SwitchLog(false);
  if (IsWindows()) {
    FreeConsole();
    UnpackFile("libs/libgme.dll");
    UnpackFile("libs/libogg-0.dll");
    UnpackFile("libs/libopus-0.dll");
    UnpackFile("libs/libopusfile-0.dll");
    UnpackFile("libs/libwavpack-1.dll");
    UnpackFile("libs/libxmp.dll");
    UnpackFile("libs/libavif-16.dll");
    UnpackFile("libs/libtiff-5.dll");
    UnpackFile("libs/libwebp-7.dll");
    UnpackFile("libs/libwebpdemux-2.dll");
  }
  UnpackFile("resources/image.png");
  UnpackFile("resources/icon.ico");
  if (SDL2::Init(SDL_INIT_VIDEO, "libs/") != 0) {
    LogError(SDL2::IsLoaded() ? SDL2::GetError() : "Unexpected error.");
    return -1;
  }
  SDL_Window* window = SDL2::CreateWindow( "Test window", 100, 100, 600, 600, SDL_WINDOW_SHOWN );
  if (window == nullptr) {
    LogError(std::string("Couldn't create a window: ") + SDL2::GetError());
    return -1;
  }
	if (SDL2::Image::Init( IMG_INIT_PNG ) != IMG_INIT_PNG) {
		LogError(std::string("Couldn't initialize SDL image: ") + SDL2::GetError());
		return -1;
	}
  SDL_Surface* window_surface = SDL2::GetWindowSurface( window );
  SDL_Surface* load_image_surface = SDL2::Image::Load("resources/image.png");
  if (load_image_surface == nullptr) {
    LogError(std::string("Couldn't load the image: ") + SDL2::GetError());
    return -1;
  }
  SDL_Surface* image_surface = SDL2::ConvertSurface(load_image_surface, window_surface->format, 0);
  if (image_surface == nullptr) {
    LogError(std::string("Couldn't convert the image: ") + SDL2::GetError());
    return -1;
  }
  SDL2::FreeSurface(load_image_surface);
  load_image_surface = SDL2::Image::Load("resources/icon.ico");
  if (load_image_surface == nullptr) {
    LogError(std::string("Couldn't load the icon: ") + SDL2::GetError());
    return -1;
  }
  SDL_Surface* icon_surface = SDL2::ConvertSurface(load_image_surface, window_surface->format, 0);
  if (icon_surface == nullptr) {
    LogError(std::string("Couldn't convert the icon: ") + SDL2::GetError());
    return -1;
  }
  SDL2::SetWindowIcon(window, icon_surface);
  bool run = true;
  SDL_Event e;
  while (run) {
    while(SDL2::PollEvent(&e) != 0) if(e.type == SDL_QUIT) run = false;
    SDL2::BlitSurface(image_surface, nullptr, window_surface, nullptr);
    SDL2::UpdateWindowSurface(window);
  }
	SDL2::FreeSurface(image_surface);
	SDL2::DestroyWindow(window);
	SDL2::Image::Quit();
  SDL2::Quit();
  return 0;
}