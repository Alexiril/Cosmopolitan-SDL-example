## Cosmopolitan SDL2 example (for C++)

This example loads all the libraries, the icon and the image, creates a window, sets the window icon and shows the image on the window surface.

This code uses [Cosmopolitan Libc](github.com/jart/cosmopolitan) to generate one application for a lot of operating systems. Though, due to current limitation of this SDL2 linking implementation, it doesn't work for all 7 systems.

Works with simply linking the SDL2 dynamic libraries for exact OS **(SDL2 is not compiled with Cosmopolitan yet)**.

This code is the <s>terrible</s> mix of C and C++, so it can be compiled only with C++ compiler. Futher I plan to make a clean platform for C++ and C separately.

There are some troubles with licenses (too many of them), I would be real glad to find out which licenses I have to insert here :)

Right now it was tested for those systems:
* Windows 11 x64 23H2 (OS build 22631.3296)
* Ubuntu 23.10 x64 (GNOME 45.2, Wayland, Kernel Linux 6.5.0)

The example release built on Ubuntu 23.10.

### Implementation details

For building you need to have in the PATH:

* [cosmoc++ / cosmocc compiler](https://cosmo.zip/pub/cosmocc/) for working smoothly with Cosmopolitan
* [zipobj](https://cosmo.zip/pub/cosmocc/) for making correct resources files for APE linker **(included in cosmocc)**
* [python 3](https://www.python.org/) for starting a building script

To build the code clone the repository, go to the folder with the cloned repo, and start the `build.py` file.

```sh
git clone git@github.com:Alexiril/Cosmopolitan-SDL-example.git
cd Cosmopolitan-SDL-example
python build.py
```

In the `build.py` there is `variables` section used to set up all stuff like build directory, output filename, sources, include folders, and resources.

To start the builded application just run it as usual application.

```sh
build_dir/sdltest.exe # starting in Linux
```

To open it in Windows just open the application *(sdltest.exe for example)* in the build directory as the usual one.

*Why there's no normal CMakeLists or Makefile?.. \
Well, I don't have enough skills to make a good makefile, cmake file or a bash script for this case. Even though, there is an example for makefile for Cosmopolitan projects. So, here is Python script. (Yeah, that's dumb :)*

Most of functions from SDL2, SDL2 mixer, SDL2 net, SDL2 image, SDL2 ttf were linked. You can access them using SDL2::FunctionName(arguments).

**Please, don't use SDL_FunctionName**, it won't work because in the application start SDL won't be linked the needed way to do so.

At the same time SDL2 structures weren't translated into C++ the same way (yet), so you can use them as the documentation says *(just SDL_Window for example)*.

```c++
// Using SDL_ConvertSurface
SDL2::ConvertSurface(src, fmt, flags);
```

Also, there (in the files `cosmo_sdl2`) are some non-standard functions, that are not actually part of SDL2:

* UnpackFile - used for unpacking some file from the executable package, for example dynamic libraries.
* OpenRequiredLibrary - used for simple linking to a dynamic library.
* LoadSDLLibrary - used for opening all the SDL2 dynamic libraries (such as SDL2, SDL2_ttf, SDL2_mixer, etc)
* UnloadLibrary - used for unlinking the SDL libraries.

### Example pictures

* Windows 11

    ![Windows 11 example screenshot](readme-images/Screenshot%201.png)

* Ubuntu 23.10

    ![Ubuntu 23.10 example screenshot](readme-images/Screenshot%202.png)