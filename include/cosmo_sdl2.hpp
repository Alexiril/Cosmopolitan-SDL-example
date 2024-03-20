#pragma once
#ifndef COSMO_SDL2
#define COSMO_SDL2

/* Windows and Linux only yet. Other platforms come later :) */

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_net.h>

#include <libc/isystem/string>

#ifdef COSMO_SDL2_VULKAN
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#endif

enum class ErrorLevel {
    log,
    info,
    warning,
    error
};

/// @brief Prints an error with the exact level(info, warning, error) to cerr (error output stream)
/// @param error is the string to be printed
/// @param level is the level of importance of the error (info, warning, error)
void LogError(const std::string& error, ErrorLevel level = ErrorLevel::error, std::ostream& out = std::cerr);

/// @brief Unpacks a file from the ZIP archive inside the application.
/// @param filename path and name of the file inside the archive
/// @return True if unpacked successfully and False otherwise
bool UnpackFile(const std::string& filename, bool do_log = true);

namespace SDL2 {

    /// @brief Turns off and on logging
    /// @param do_log defines if the library should log or not
    void SwitchLog(bool do_log);

    /// @brief Opens a dynamic library (shared object) using dlopen, and saves the pointer to it if opening was successfull. Otherwise prints an error to the log.
    /// @param libname is the actual name of a library, used only for logging in the case of an error
    /// @param pointer is the variable where the pointer to the library will be stored in the case of successfull loading
    /// @param filename is the actual name of a file containing the dynamic library
    /// @param library_path is the path to the library (separated from filename to make it possible to load the system library fastly)
    /// @return True if the loading was successfull and False otherwise.
    bool OpenRequiredLibrary(const std::string& libname, void*& pointer, std::string filename, const std::string& library_path = "./");

    /// @brief Loads the full SDL package and all supported SDL2 functions.
    /// @param library_path is not necessary, define the path to the libraries (current by default)
    /// @return True if the lib was loaded correctly and False otherwise.
    bool LoadSDLLibrary(const std::string& library_path = "./");

    /// @brief Unloads the SDL library
    void UnloadLibrary();

    /// @brief Returns if SDL2 library is loaded or not.
    /// @return True if the lib is loaded and False otherwise.
    bool IsLoaded();

    /* SDL2 functions */


#if 1 // To hide the list

    /// @brief Loads the SDL library (if not loaded yet) and initialize it. https://wiki.libsdl.org/SDL2/SDL_Init
    /// @param flags subsystem initialization flags
    /// @param library_path is not necessary, define the path to the library (current by default)
    /// @return Zero on success or a negative error code on failure; call SDL_GetError() for more information.
    int Init(uint32_t flags, const std::string& library_path = "./");
    
    /// @brief Clean up all initialized subsystems and unloads the library. https://wiki.libsdl.org/SDL2/SDL_Quit
    void Quit();

    /// @brief Use this function to create a new SDL_RWops structure for reading from and/or writing to a named file. https://wiki.libsdl.org/SDL2/SDL_RWFromFile
    /// @param file a UTF-8 string representing the filename to open
    /// @param mode an ASCII string representing the mode to be used for opening the file.
    /// @return Returns a pointer to the SDL_RWops structure that is created, or NULL on failure; call SDL_GetError() for more information.
    SDL_RWops* RWFromFile(const char *file, const char *mode);

    /// @brief Add a callback to be triggered when an event is added to the event queue. https://wiki.libsdl.org/SDL2/SDL_AddEventWatch
    /// @param filter an SDL_EventFilter function to call when an event happens.
    /// @param userdata a pointer that is passed to filter
    void AddEventWatch(SDL_EventFilter filter,  void*userdata);

    /// @brief Add a function to watch a particular hint. https://wiki.libsdl.org/SDL2/SDL_AddHintCallback
    /// @param name the hint to watch
    /// @param callback An SDL_HintCallback function that will be called when the hint value changes
    /// @param userdata a pointer to pass to the callback function
    void AddHintCallback(const char *name,  SDL_HintCallback callback,  void*userdata);

    /// @brief Call a callback function at a future time. https://wiki.libsdl.org/SDL2/SDL_AddTimer
    /// @param interval the timer delay, in milliseconds, passed to callback
    /// @param callback the SDL_TimerCallback function to call when the specified interval elapses
    /// @param param a pointer that is passed to callback
    /// @return Returns a timer ID or 0 if an error occurs; call SDL_GetError() for more information.
    SDL_TimerID AddTimer(uint32_t interval,  SDL_TimerCallback callback,  void*param);

    /// @brief Create an SDL_PixelFormat structure corresponding to a pixel format. https://wiki.libsdl.org/SDL2/SDL_AllocFormat
    /// @param pixel_format one of the SDL_PixelFormatEnum values
    /// @return Returns the new SDL_PixelFormat structure or NULL on failure; call SDL_GetError() for more information.
    SDL_PixelFormat * AllocFormat(uint32_t pixel_format);

    /// @brief Create a palette structure with the specified number of color entries. https://wiki.libsdl.org/SDL2/SDL_AllocPalette
    /// @param ncolors represents the number of color entries in the color palette
    /// @return Returns a new SDL_Palette structure on success or NULL on failure (e.g. if there wasn't enough memory); call SDL_GetError() for more information.
    SDL_Palette* AllocPalette(int ncolors);

    /// @brief Use this function to allocate an empty, unpopulated SDL_RWops structure. https://wiki.libsdl.org/SDL2/SDL_AllocRW
    /// @return Returns a pointer to the allocated memory on success, or NULL on failure; call SDL_GetError() for more information.
    SDL_RWops* AllocRW();

    /// @brief Add to an atomic variable. https://wiki.libsdl.org/SDL2/SDL_AtomicAdd
    /// @param a a pointer to an SDL_atomic_t variable to be modified
    /// @param v the desired value to add
    /// @return Returns the previous value of the atomic variable.
    int AtomicAdd(SDL_atomic_t *a, int v);

    /// @brief Set an atomic variable to a new value if it is currently an old value. https://wiki.libsdl.org/SDL2/SDL_AtomicCAS
    /// @param a a pointer to an SDL_atomic_t variable to be modified
    /// @param oldval the old value
    /// @param newval the new value
    /// @return Returns SDL_TRUE if the atomic variable was set, SDL_FALSE otherwise.
    bool AtomicCAS(SDL_atomic_t *a, int oldval, int newval);

    /// @brief Set a pointer to a new value if it is currently an old value. https://wiki.libsdl.org/SDL2/SDL_AtomicCASPtr
    /// @param a a pointer to a pointer
    /// @param oldval the old pointer value
    /// @param newval the new pointer value
    /// @return Returns SDL_TRUE if the pointer was set, SDL_FALSE otherwise.
    bool AtomicCASPtr(void**a, void*oldval, void*newval);

    /// @brief Use this function to decrement an atomic variable used as a reference count. https://wiki.libsdl.org/SDL2/SDL_AtomicDecRef
    /// @param a a pointer to an SDL_atomic_t variable to be modified
    /// @return Returns SDL_TRUE if the variable reached zero after decrementing, SDL_FALSE otherwise.
    bool AtomicDecRef(SDL_atomic_t* a);

    /// @brief Get the value of an atomic variable. https://wiki.libsdl.org/SDL2/SDL_AtomicGet
    /// @param a a pointer to an SDL_atomic_t variable
    /// @return Returns the current value of an atomic variable.
    int AtomicGet(SDL_atomic_t *a);

    /// @brief Get the value of a pointer atomically. https://wiki.libsdl.org/SDL2/SDL_AtomicGetPtr
    /// @param a a pointer to a pointer
    /// @return Returns the current value of a pointer.
    void* AtomicGetPtr(void **a);

    /// @brief Use this function to increment an atomic variable used as a reference count. https://wiki.libsdl.org/SDL2/SDL_AtomicIncRef
    /// @param a a pointer to an SDL_atomic_t variable to be modified
    void AtomicIncRef(SDL_atomic_t* a);

    /// @brief Lock a spin lock by setting it to a non-zero value. https://wiki.libsdl.org/SDL2/SDL_AtomicLock
    /// @param lock a pointer to a lock variable
    void AtomicLock(SDL_SpinLock *lock);

    /// @brief Set an atomic variable to a value. https://wiki.libsdl.org/SDL2/SDL_AtomicSet
    /// @param a a pointer to an SDL_atomic_t variable to be modified
    /// @param v the desired value
    /// @return Returns the previous value of the atomic variable.
    int AtomicSet(SDL_atomic_t *a, int v);

    /// @brief Set a pointer to a value atomically. https://wiki.libsdl.org/SDL2/SDL_AtomicSetPtr
    /// @param a a pointer to a pointer
    /// @param v the desired pointer value
    /// @return Returns the previous value of the pointer.
    void* AtomicSetPtr(void **a, void* v);

    /// @brief Try to lock a spin lock by setting it to a non-zero value. https://wiki.libsdl.org/SDL2/SDL_AtomicTryLock
    /// @param lock a pointer to a lock variable
    /// @return Returns SDL_TRUE if the lock succeeded, SDL_FALSE if the lock is already held.
    bool AtomicTryLock(SDL_SpinLock *lock);

    /// @brief Unlock a spin lock by setting it to 0. https://wiki.libsdl.org/SDL2/SDL_AtomicUnlock
    /// @param lock a pointer to a lock variable
    void AtomicUnlock(SDL_SpinLock *lock);

    /// @brief Use this function to initialize a particular audio driver. https://wiki.libsdl.org/SDL2/SDL_AudioInit
    /// @param driver_name the name of the desired audio driver
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int AudioInit(const char *driver_name);

    /// @brief Use this function to shut down audio if you initialized it with SDL_AudioInit(). https://wiki.libsdl.org/SDL2/SDL_AudioQuit
    void AudioQuit();

    /// @brief Get the number of converted/resampled bytes available. https://wiki.libsdl.org/SDL2/SDL_AudioStreamAvailable
    int AudioStreamAvailable(SDL_AudioStream *stream);

    /// @brief Clear any pending data in the stream without converting it https://wiki.libsdl.org/SDL2/SDL_AudioStreamClear
    void AudioStreamClear(SDL_AudioStream *stream);

    /// @brief Tell the stream that you're done sending data, and anything being buffered should be converted/resampled and made available immediately. https://wiki.libsdl.org/SDL2/SDL_AudioStreamFlush
    int AudioStreamFlush(SDL_AudioStream *stream);

    /// @brief Get converted/resampled data from the stream https://wiki.libsdl.org/SDL2/SDL_AudioStreamGet
    /// @param stream The stream the audio is being requested from
    /// @param buf A buffer to fill with audio data
    /// @param len The maximum number of bytes to fill
    /// @return Returns the number of bytes read from the stream, or -1 on error
    int AudioStreamGet(SDL_AudioStream *stream, void *buf, int len);

    /// @brief Add data to be converted/resampled to the stream. https://wiki.libsdl.org/SDL2/SDL_AudioStreamPut
    /// @param stream The stream the audio data is being added to
    /// @param buf A pointer to the audio data to add
    /// @param len The number of bytes to write to the stream
    /// @return Returns 0 on success, or -1 on error.
    int AudioStreamPut(SDL_AudioStream *stream, void *buf, int len);

    /// @brief Use this function to perform a scaled surface copy to a destination surface. https://wiki.libsdl.org/SDL2/SDL_BlitScaled
    /// @param src the SDL_Surface structure to be copied from
    /// @param srcrect the SDL_Rect structure representing the rectangle to be copied, or NULL to copy the entire surface
    /// @param dst the SDL_Surface structure that is the blit target
    /// @param dstrect the SDL_Rect structure representing the rectangle that is copied into, or NULL to copy into the entire surface
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int BlitScaled(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Surface* dst, SDL_Rect* dstrect);

    /// @brief Use this function to perform a fast surface copy to a destination surface. https://wiki.libsdl.org/SDL2/SDL_BlitSurface
    /// @param src the SDL_Surface structure to be copied from
    /// @param srcrect the SDL_Rect structure representing the rectangle to be copied, or NULL to copy the entire surface
    /// @param dst the SDL_Surface structure that is the blit target
    /// @param dstrect the SDL_Rect structure representing the rectangle that is copied into
    /// @return Returns 0 if the blit is successful or a negative error code on failure; call SDL_GetError() for more information.
    int BlitSurface(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Surface* dst, SDL_Rect* dstrect);

    /// @brief Initialize an SDL_AudioCVT structure for conversion. https://wiki.libsdl.org/SDL2/SDL_BuildAudioCVT
    /// @param cvt an SDL_AudioCVT structure filled in with audio conversion information
    /// @param src_format the source format of the audio data; for more info see SDL_AudioFormat
    /// @param src_channels the number of channels in the source
    /// @param src_rate the frequency (sample-frames-per-second) of the source
    /// @param dst_format the destination format of the audio data; for more info see SDL_AudioFormat
    /// @param dst_channels the number of channels in the destination
    /// @param dst_rate the frequency (sample-frames-per-second) of the destination
    /// @return Returns 1 if the audio filter is prepared, 0 if no conversion is needed, or a negative error code on failure; call SDL_GetError() for more information.
    int BuildAudioCVT(SDL_AudioCVT * cvt, SDL_AudioFormat src_format, Uint8 src_channels, int src_rate, SDL_AudioFormat dst_format, Uint8 dst_channels, int dst_rate);

    /// @brief Calculate a 256 entry gamma ramp for a gamma value. https://wiki.libsdl.org/SDL2/SDL_CalculateGammaRamp
    /// @param gamma a gamma value where 0.0 is black and 1.0 is identity
    /// @param ramp an array of 256 values filled in with the gamma ramp
    void CalculateGammaRamp(float gamma, uint16_t * ramp);

    /// @brief Capture the mouse and to track input outside an SDL window. https://wiki.libsdl.org/SDL2/SDL_CaptureMouse
    /// @param enabled SDL_TRUE to enable capturing, SDL_FALSE to disable.
    /// @return Returns 0 on success or -1 if not supported; call SDL_GetError() for more information.
    int CaptureMouse(bool enabled);

    /// @brief Dismiss the composition window/IME without disabling the subsystem. https://wiki.libsdl.org/SDL2/SDL_ClearComposition
    void ClearComposition();

    /// @brief Clear any previous error message for this thread. https://wiki.libsdl.org/SDL2/SDL_ClearError
    void ClearError();

    /// @brief Clear all hints. https://wiki.libsdl.org/SDL2/SDL_ClearHints
    void ClearHints();

    /// @brief Drop any queued audio data waiting to be sent to the hardware. https://wiki.libsdl.org/SDL2/SDL_ClearQueuedAudio
    /// @param dev the device ID of which to clear the audio queue
    void ClearQueuedAudio(SDL_AudioDeviceID dev);

    /// @brief This function is a legacy means of closing the audio device. https://wiki.libsdl.org/SDL2/SDL_CloseAudio
    void CloseAudio();

    /// @brief Use this function to shut down audio processing and close the audio device. https://wiki.libsdl.org/SDL2/SDL_CloseAudioDevice
    /// @param dev an audio device previously opened with SDL_OpenAudioDevice()
    void CloseAudioDevice(SDL_AudioDeviceID dev);

    /// @brief Use this function to prevent the compiler from reordering reads and writes to globally visible variables across the call. https://wiki.libsdl.org/SDL2/SDL_CompilerBarrier
    void CompilerBarrier();

    /// @brief Compose a custom blend mode for renderers. https://wiki.libsdl.org/SDL2/SDL_ComposeCustomBlendMode
    /// @param srcColorFactor the SDL_BlendFactor applied to the red, green, and blue components of the source pixels
    /// @param dstColorFactor the SDL_BlendFactor applied to the red, green, and blue components of the destination pixels
    /// @param colorOperation the SDL_BlendOperation used to combine the red, green, and blue components of the source and destination pixels
    /// @param srcAlphaFactor the SDL_BlendFactor applied to the alpha component of the source pixels
    /// @param dstAlphaFactor the SDL_BlendFactor applied to the alpha component of the destination pixels
    /// @param alphaOperation the SDL_BlendOperation used to combine the alpha component of the source and destination pixels
    /// @return Returns an SDL_BlendMode that represents the chosen factors and operations.
    SDL_BlendMode ComposeCustomBlendMode(SDL_BlendFactor srcColorFactor, SDL_BlendFactor dstColorFactor, SDL_BlendOperation colorOperation, SDL_BlendFactor srcAlphaFactor, SDL_BlendFactor dstAlphaFactor, SDL_BlendOperation alphaOperation);

    /// @brief Restart all threads that are waiting on the condition variable. https://wiki.libsdl.org/SDL2/SDL_CondBroadcast
    /// @param cond the condition variable to signal
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int CondBroadcast(SDL_cond * cond);

    /// @brief Restart one of the threads that are waiting on the condition variable. https://wiki.libsdl.org/SDL2/SDL_CondSignal
    /// @param cond the condition variable to signal
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int CondSignal(SDL_cond * cond);

    /// @brief Wait until a condition variable is signaled. https://wiki.libsdl.org/SDL2/SDL_CondWait
    /// @param cond the condition variable to wait on
    /// @param mutex the mutex used to coordinate thread access
    /// @return Returns 0 when it is signaled or a negative error code on failure; call SDL_GetError() for more information.
    int CondWait(SDL_cond * cond, SDL_mutex * mutex);

    /// @brief Wait until a condition variable is signaled or a certain time has passed. https://wiki.libsdl.org/SDL2/SDL_CondWaitTimeout
    /// @param cond the condition variable to wait on
    /// @param mutex the mutex used to coordinate thread access
    /// @param ms the maximum time to wait, in milliseconds, or SDL_MUTEX_MAXWAIT to wait indefinitely
    /// @return Returns 0 if the condition variable is signaled, SDL_MUTEX_TIMEDOUT if the condition is not signaled in the allotted time, or a negative error code on failure; call SDL_GetError() for more information.
    int CondWaitTimeout(SDL_cond * cond, SDL_mutex * mutex, Uint32 ms);

    /// @brief Convert audio data to a desired audio format. https://wiki.libsdl.org/SDL2/SDL_ConvertAudio
    /// @param cvt an SDL_AudioCVT structure that was previously set up by SDL_BuildAudioCVT().
    /// @return Returns 0 if the conversion was completed successfully or a negative error code on failure; call SDL_GetError() for more information.
    int ConvertAudio(SDL_AudioCVT * cvt);

    /// @brief Copy a block of pixels of one format to another format. https://wiki.libsdl.org/SDL2/SDL_ConvertPixels
    /// @param width the width of the block to copy, in pixels
    /// @param height the height of the block to copy, in pixels
    /// @param src_format an SDL_PixelFormatEnum value of the src pixels format
    /// @param src a pointer to the source pixels
    /// @param src_pitch the pitch of the source pixels, in bytes
    /// @param dst_format an SDL_PixelFormatEnum value of the dst pixels format
    /// @param dst a pointer to be filled in with new pixel data
    /// @param dst_pitch the pitch of the destination pixels, in bytes
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int ConvertPixels(int width, int height, Uint32 src_format, void * src, int src_pitch, Uint32 dst_format, void * dst, int dst_pitch);

    /// @brief Copy an existing surface to a new surface of the specified format. https://wiki.libsdl.org/SDL2/SDL_ConvertSurface
    /// @param src the existing SDL_Surface structure to convert
    /// @param fmt the SDL_PixelFormat structure that the new surface is optimized for
    /// @param flags the flags are unused and should be set to 0; this is a leftover from SDL 1.2's API
    /// @return Returns the new SDL_Surface structure that is created or NULL if it fails; call SDL_GetError() for more information.
    SDL_Surface* ConvertSurface(SDL_Surface * src, const SDL_PixelFormat * fmt, Uint32 flags);

    /// @brief Copy an existing surface to a new surface of the specified format enum. https://wiki.libsdl.org/SDL2/SDL_ConvertSurfaceFormat
    /// @param src the existing SDL_Surface structure to convert
    /// @param pixel_format the SDL_PixelFormatEnum that the new surface is optimized for
    /// @param flags the flags are unused and should be set to 0; this is a leftover from SDL 1.2's API
    /// @return Returns the new SDL_Surface structure that is created or NULL if it fails; call SDL_GetError() for more information.
    SDL_Surface* ConvertSurfaceFormat(SDL_Surface * src, Uint32 pixel_format, Uint32 flags);

    /// @brief Create a color cursor. https://wiki.libsdl.org/SDL2/SDL_CreateColorCursor
    /// @param surface an SDL_Surface structure representing the cursor image
    /// @param hot_x the x position of the cursor hot spot
    /// @param hot_y the y position of the cursor hot spot
    /// @return Returns the new cursor on success or NULL on failure; call SDL_GetError() for more information.
    SDL_Cursor* CreateColorCursor(SDL_Surface *surface, int hot_x, int hot_y);

    /// @brief Create a condition variable. https://wiki.libsdl.org/SDL2/SDL_CreateCond
    /// @return Returns a new condition variable or NULL on failure; call SDL_GetError() for more information.
    SDL_cond* CreateCond();

    /// @brief Create a cursor using the specified bitmap data and mask (in MSB format). https://wiki.libsdl.org/SDL2/SDL_CreateCursor
    /// @param data the color value for each pixel of the cursor
    /// @param mask the mask value for each pixel of the cursor
    /// @param w the width of the cursor
    /// @param h the height of the cursor
    /// @param hot_x the X-axis location of the upper left corner of the cursor relative to the actual mouse position
    /// @param hot_y the Y-axis location of the upper left corner of the cursor relative to the actual mouse position
    /// @return Returns a new cursor with the specified parameters on success or NULL on failure; call SDL_GetError() for more information.
    SDL_Cursor* CreateCursor(const Uint8 * data, const Uint8 * mask, int w, int h, int hot_x, int hot_y);

    /// @brief Create a new mutex. https://wiki.libsdl.org/SDL2/SDL_CreateMutex
    /// @return Returns the initialized and unlocked mutex or NULL on failure; call SDL_GetError() for more information.
    SDL_mutex* CreateMutex();

    /// @brief Create a 2D rendering context for a window. https://wiki.libsdl.org/SDL2/SDL_CreateRenderer
    /// @param window the window where rendering is displayed
    /// @param index the index of the rendering driver to initialize, or -1 to initialize the first one supporting the requested flags
    /// @param flags 0, or one or more SDL_RendererFlags OR'd together
    /// @return Returns a valid rendering context or NULL if there was an error; call SDL_GetError() for more information.
    SDL_Renderer * CreateRenderer(SDL_Window * window, int index, Uint32 flags);

    /// @brief Allocate a new RGB surface. https://wiki.libsdl.org/SDL2/SDL_CreateRGBSurface
    /// @param flags the flags are unused and should be set to 0
    /// @param width the width of the surface
    /// @param height the height of the surface
    /// @param depth the depth of the surface in bits
    /// @param Rmask the red mask for the pixels
    /// @param Gmask the green mask for the pixels
    /// @param Bmask the blue mask for the pixels
    /// @param Amask the alpha mask for the pixels
    /// @return Returns the new SDL_Surface structure that is created or NULL if it fails; call SDL_GetError() for more information.
    SDL_Surface* CreateRGBSurface(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);

    /// @brief Allocate a new RGB surface with existing pixel data. https://wiki.libsdl.org/SDL2/SDL_CreateRGBSurfaceFrom
    /// @param pixels a pointer to existing pixel data
    /// @param width the width of the surface
    /// @param height the height of the surface
    /// @param depth the depth of the surface in bits
    /// @param pitch the pitch of the surface in bytes
    /// @param Rmask the red mask for the pixels
    /// @param Gmask the green mask for the pixels
    /// @param Bmask the blue mask for the pixels
    /// @param Amask the alpha mask for the pixels
    /// @return Returns the new SDL_Surface structure that is created or NULL if it fails; call SDL_GetError() for more information.
    SDL_Surface* CreateRGBSurfaceFrom(void *pixels, int width, int height, int depth, int pitch, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);

    /// @brief Allocate a new RGB surface with a specific pixel format. https://wiki.libsdl.org/SDL2/SDL_CreateRGBSurfaceWithFormat
    /// @param flags the flags are unused and should be set to 0
    /// @param width the width of the surface
    /// @param height the height of the surface
    /// @param depth the depth of the surface in bits
    /// @param format the SDL_PixelFormatEnum for the new surface's pixel format.
    /// @return Returns the new SDL_Surface structure that is created or NULL if it fails; call SDL_GetError() for more information.
    SDL_Surface* CreateRGBSurfaceWithFormat(Uint32 flags, int width, int height, int depth, Uint32 format);

    /// @brief Allocate a new RGB surface with with a specific pixel format and existing pixel data. https://wiki.libsdl.org/SDL2/SDL_CreateRGBSurfaceWithFormatFrom
    /// @param pixels a pointer to existing pixel data
    /// @param width the width of the surface
    /// @param height the height of the surface
    /// @param depth the depth of the surface in bits
    /// @param pitch the pitch of the surface in bytes
    /// @param format the SDL_PixelFormatEnum for the new surface's pixel format.
    /// @return Returns the new SDL_Surface structure that is created or NULL if it fails; call SDL_GetError() for more information.
    SDL_Surface* CreateRGBSurfaceWithFormatFrom(void *pixels, int width, int height, int depth, int pitch, Uint32 format);

    /// @brief Create a semaphore. https://wiki.libsdl.org/SDL2/SDL_CreateSemaphore
    /// @param initial_value the starting value of the semaphore
    /// @return Returns a new semaphore or NULL on failure; call SDL_GetError() for more information.
    SDL_sem* CreateSemaphore(Uint32 initial_value);

    /// @brief Create a window that can be shaped with the specified position, dimensions, and flags. https://wiki.libsdl.org/SDL2/SDL_CreateShapedWindow
    /// @param title The title of the window, in UTF-8 encoding.
    /// @param x The x position of the window, ::SDL_WINDOWPOS_CENTERED, or ::SDL_WINDOWPOS_UNDEFINED.
    /// @param y The y position of the window, ::SDL_WINDOWPOS_CENTERED, or ::SDL_WINDOWPOS_UNDEFINED.
    /// @param w The width of the window.
    /// @param h The height of the window.
    /// @param flags The flags for the window, a mask of SDL_WINDOW_BORDERLESS with any of the following: ::SDL_WINDOW_OPENGL, ::SDL_WINDOW_INPUT_GRABBED, ::SDL_WINDOW_HIDDEN, ::SDL_WINDOW_RESIZABLE, ::SDL_WINDOW_MAXIMIZED, ::SDL_WINDOW_MINIMIZED, ::SDL_WINDOW_BORDERLESS is always set, and ::SDL_WINDOW_FULLSCREEN is always unset.
    /// @return Return the window created, or NULL if window creation failed.
    SDL_Window * CreateShapedWindow(const char *title,unsigned int x,unsigned int y,unsigned int w,unsigned int h,Uint32 flags);

    /// @brief Create a 2D software rendering context for a surface. https://wiki.libsdl.org/SDL2/SDL_CreateSoftwareRenderer
    /// @param surface the SDL_Surface structure representing the surface where rendering is done
    /// @return Returns a valid rendering context or NULL if there was an error; call SDL_GetError() for more information.
    SDL_Renderer * CreateSoftwareRenderer(SDL_Surface * surface);

    /// @brief Create a system cursor. https://wiki.libsdl.org/SDL2/SDL_CreateSystemCursor
    /// @param id an SDL_SystemCursor enum value
    /// @return Returns a cursor on success or NULL on failure; call SDL_GetError() for more information.
    SDL_Cursor* CreateSystemCursor(SDL_SystemCursor id);

    /// @brief Create a texture for a rendering context. https://wiki.libsdl.org/SDL2/SDL_CreateTexture
    /// @param renderer the rendering context
    /// @param format one of the enumerated values in SDL_PixelFormatEnum
    /// @param access one of the enumerated values in SDL_TextureAccess
    /// @param w the width of the texture in pixels
    /// @param h the height of the texture in pixels
    /// @return Returns a pointer to the created texture or NULL if no rendering context was active, the format was unsupported, or the width or height were out of range; call SDL_GetError() for more information.
    SDL_Texture * CreateTexture(SDL_Renderer * renderer, Uint32 format, int access, int w, int h);

    /// @brief Create a texture from an existing surface. https://wiki.libsdl.org/SDL2/SDL_CreateTextureFromSurface
    /// @param renderer the rendering context
    /// @param surface the SDL_Surface structure containing pixel data used to fill the texture
    /// @return Returns the created texture or NULL on failure; call SDL_GetError() for more information.
    SDL_Texture * CreateTextureFromSurface(SDL_Renderer * renderer, SDL_Surface * surface);

    /// @brief Create a new thread with a default stack size. https://wiki.libsdl.org/SDL2/SDL_CreateThread
    /// @param fn the SDL_ThreadFunction function to call in the new thread
    /// @param name the name of the thread
    /// @param data a pointer that is passed to fn
    /// @return Returns an opaque pointer to the new thread object on success, NULL if the new thread could not be created; call SDL_GetError() for more information.
    SDL_Thread * CreateThread(SDL_ThreadFunction fn, const char *name, void *data);

    /// @brief Create a new thread with a specific stack size. https://wiki.libsdl.org/SDL2/SDL_CreateThreadWithStackSize
    /// @param fn the SDL_ThreadFunction function to call in the new thread
    /// @param name the name of the thread
    /// @param stacksize the size, in bytes, to allocate for the new thread stack.
    /// @param data a pointer that is passed to fn
    /// @return Returns an opaque pointer to the new thread object on success, NULL if the new thread could not be created; call SDL_GetError() for more information.
    SDL_Thread * CreateThreadWithStackSize(SDL_ThreadFunction fn, const char *name, const size_t stacksize, void *data);

    /// @brief Create a window with the specified position, dimensions, and flags. https://wiki.libsdl.org/SDL2/SDL_CreateWindow
    /// @param title the title of the window, in UTF-8 encoding
    /// @param x the x position of the window, SDL_WINDOWPOS_CENTERED, or SDL_WINDOWPOS_UNDEFINED
    /// @param y the y position of the window, SDL_WINDOWPOS_CENTERED, or SDL_WINDOWPOS_UNDEFINED
    /// @param w the width of the window, in screen coordinates
    /// @param h the height of the window, in screen coordinates
    /// @param flags 0, or one or more SDL_WindowFlags OR'd together
    /// @return Returns the window that was created or NULL on failure; call SDL_GetError() for more information.
    SDL_Window * CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags);

    /// @brief Create a window and default renderer. https://wiki.libsdl.org/SDL2/SDL_CreateWindowAndRenderer
    /// @param width the width of the window
    /// @param height the height of the window
    /// @param window_flags the flags used to create the window (see SDL_CreateWindow())
    /// @param window a pointer filled with the window, or NULL on error
    /// @param renderer a pointer filled with the renderer, or NULL on error
    /// @return Returns 0 on success, or -1 on error; call SDL_GetError() for more information.
    int CreateWindowAndRenderer( int width, int height, Uint32 window_flags, SDL_Window **window, SDL_Renderer **renderer);

    /// @brief Create an SDL window from an existing native window. https://wiki.libsdl.org/SDL2/SDL_CreateWindowFrom
    /// @param data a pointer to driver-dependent window creation data, typically your native window cast to a void*
    /// @return Returns the window that was created or NULL on failure; call SDL_GetError() for more information.
    SDL_Window * CreateWindowFrom(void *data);

    /// @brief Wait a specified number of milliseconds before returning. https://wiki.libsdl.org/SDL2/SDL_Delay
    /// @param ms the number of milliseconds to delay
    void Delay(Uint32 ms);

    /// @brief Remove an event watch callback added with SDL_AddEventWatch(). https://wiki.libsdl.org/SDL2/SDL_DelEventWatch
    /// @param filter the function originally passed to SDL_AddEventWatch()
    /// @param userdata the pointer originally passed to SDL_AddEventWatch()
    void DelEventWatch(SDL_EventFilter filter, void *userdata);

    /// @brief Remove a function watching a particular hint. https://wiki.libsdl.org/SDL2/SDL_DelHintCallback
    /// @param name the hint being watched
    /// @param callback An SDL_HintCallback function that will be called when the hint value changes
    /// @param userdata a pointer being passed to the callback function
    void DelHintCallback(const char *name, SDL_HintCallback callback, void *userdata);

    /// @brief Dequeue more audio on non-callback devices. https://wiki.libsdl.org/SDL2/SDL_DequeueAudio
    /// @param dev the device ID from which we will dequeue audio
    /// @param data a pointer into where audio data should be copied
    /// @param len the number of bytes (not samples!) to which (data) points
    /// @return Returns the number of bytes dequeued, which could be less than requested; call SDL_GetError() for more information.
    Uint32 DequeueAudio(SDL_AudioDeviceID dev, void *data, Uint32 len);

    /// @brief Destroy a condition variable. https://wiki.libsdl.org/SDL2/SDL_DestroyCond
    /// @param cond the condition variable to destroy
    void DestroyCond(SDL_cond * cond);

    /// @brief Destroy a mutex created with SDL_CreateMutex(). https://wiki.libsdl.org/SDL2/SDL_DestroyMutex
    /// @param mutex the mutex to destroy
    void DestroyMutex(SDL_mutex * mutex);

    /// @brief Destroy the rendering context for a window and free associated textures. https://wiki.libsdl.org/SDL2/SDL_DestroyRenderer
    /// @param renderer the rendering context
    void DestroyRenderer(SDL_Renderer * renderer);

    /// @brief Destroy a semaphore. https://wiki.libsdl.org/SDL2/SDL_DestroySemaphore
    /// @param sem the semaphore to destroy
    void DestroySemaphore(SDL_sem * sem);

    /// @brief Destroy the specified texture. https://wiki.libsdl.org/SDL2/SDL_DestroyTexture
    /// @param texture the texture to destroy
    void DestroyTexture(SDL_Texture * texture);

    /// @brief Destroy a window. https://wiki.libsdl.org/SDL2/SDL_DestroyWindow
    /// @param window the window to destroy
    void DestroyWindow(SDL_Window * window);

    /// @brief Destroy the surface associated with the window. https://wiki.libsdl.org/SDL2/SDL_DestroyWindowSurface
    /// @param window the window to update
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int DestroyWindowSurface(SDL_Window *window);

    /// @brief Let a thread clean up on exit without intervention. https://wiki.libsdl.org/SDL2/SDL_DetachThread
    /// @param thread the SDL_Thread pointer that was returned from the SDL_CreateThread() call that started this thread
    void DetachThread(SDL_Thread * thread);

    /// @brief Prevent the screen from being blanked by a screen saver. https://wiki.libsdl.org/SDL2/SDL_DisableScreenSaver
    void DisableScreenSaver();

    /// @brief Allow the screen to be blanked by a screen saver. https://wiki.libsdl.org/SDL2/SDL_EnableScreenSaver
    void EnableScreenSaver();

    /// @brief Calculate a minimal rectangle enclosing a set of points with float precision. https://wiki.libsdl.org/SDL2/SDL_EncloseFPoints
    /// @param points an array of SDL_FPoint structures representing points to be enclosed
    /// @param count the number of structures in the points array
    /// @param clip an SDL_FRect used for clipping or NULL to enclose all points
    /// @param result an SDL_FRect structure filled in with the minimal enclosing rectangle
    /// @return Returns SDL_TRUE if any points were enclosed or SDL_FALSE if all the points were outside of the clipping rectangle.
    bool EncloseFPoints(const SDL_FPoint * points, int count, const SDL_FRect * clip, SDL_FRect * result);

    /// @brief Calculate a minimal rectangle enclosing a set of points. https://wiki.libsdl.org/SDL2/SDL_EnclosePoints
    /// @param points an array of SDL_Point structures representing points to be enclosed
    /// @param count the number of structures in the points array
    /// @param clip an SDL_Rect used for clipping or NULL to enclose all points
    /// @param result an SDL_Rect structure filled in with the minimal enclosing rectangle
    /// @return Returns SDL_TRUE if any points were enclosed or SDL_FALSE if all the points were outside of the clipping rectangle.
    bool EnclosePoints(const SDL_Point * points, int count, const SDL_Rect * clip, SDL_Rect * result);

    /// @brief Set the state of processing events by type. https://wiki.libsdl.org/SDL2/SDL_EventState
    /// @param type the type of event; see SDL_EventType for details
    /// @param state how to process the event
    /// @return Returns SDL_DISABLE or SDL_ENABLE, representing the processing state of the event before this function makes any changes to it.
    Uint8 EventState(Uint32 type, int state);

    /// @brief Perform a fast fill of a rectangle with a specific color. https://wiki.libsdl.org/SDL2/SDL_FillRect
    /// @param dst the SDL_Surface structure that is the drawing target
    /// @param rect the SDL_Rect structure representing the rectangle to fill, or NULL to fill the entire surface
    /// @param color the color to fill with
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int FillRect(SDL_Surface * dst, const SDL_Rect * rect, Uint32 color);

    /// @brief Perform a fast fill of a set of rectangles with a specific color. https://wiki.libsdl.org/SDL2/SDL_FillRects
    /// @param dst the SDL_Surface structure that is the drawing target
    /// @param rects an array of SDL_Rects representing the rectangles to fill.
    /// @param count the number of rectangles in the array
    /// @param color the color to fill with
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int FillRects(SDL_Surface * dst, const SDL_Rect * rects, int count, Uint32 color);

    /// @brief Run a specific filter function on the current event queue, removing any events for which the filter returns 0. https://wiki.libsdl.org/SDL2/SDL_FilterEvents
    /// @param filter the SDL_EventFilter function to call when an event happens
    /// @param userdata a pointer that is passed to filter
    void FilterEvents(SDL_EventFilter filter, void *userdata);

    /// @brief Request a window to demand attention from the user. https://wiki.libsdl.org/SDL2/SDL_FlashWindow
    /// @param window the window to be flashed
    /// @param operation the flash operation
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int FlashWindow(SDL_Window * window, SDL_FlashOperation operation);

    /// @brief Clear events of a specific type from the event queue. https://wiki.libsdl.org/SDL2/SDL_FlushEvent
    /// @param type the type of event to be cleared; see SDL_EventType for details
    void FlushEvent(Uint32 type);

    /// @brief Clear events of a range of types from the event queue. https://wiki.libsdl.org/SDL2/SDL_FlushEvents
    /// @param minType the low end of event type to be cleared, inclusive; see SDL_EventType for details
    /// @param maxType the high end of event type to be cleared, inclusive; see SDL_EventType for details
    void FlushEvents(Uint32 minType, Uint32 maxType);

    /// @brief Use this function to check whether a rectangle has no area. https://wiki.libsdl.org/SDL2/SDL_FRectEmpty
    /// @param r the SDL_FRect structure representing the rectangle to check
    /// @return Returns SDL_TRUE if the rectangle has no area or SDL_FALSE if it does; see Remarks for details.
    bool FRectEmpty(const SDL_FRect* r);

    /// @brief Use this function to check whether two rectangles are equal. https://wiki.libsdl.org/SDL2/SDL_FRectEquals
    /// @param a an SDL_FRect structure representing the first rectangle
    /// @param b an SDL_FRect structure representing the second rectangle
    /// @return Returns SDL_TRUE if the two rectangles are exactly the same, SDL_FALSE otherwise; see Remarks for details.
    bool FRectEquals(const SDL_FRect* a,  const SDL_FRect* b);

    /// @brief Free an audio stream https://wiki.libsdl.org/SDL2/SDL_FreeAudioStream
    void FreeAudioStream(SDL_AudioStream *stream);

    /// @brief Free a previously-created cursor. https://wiki.libsdl.org/SDL2/SDL_FreeCursor
    /// @param cursor the cursor to free
    void FreeCursor(SDL_Cursor * cursor);

    /// @brief Free an SDL_PixelFormat structure allocated by SDL_AllocFormat(). https://wiki.libsdl.org/SDL2/SDL_FreeFormat
    /// @param format the SDL_PixelFormat structure to free
    void FreeFormat(SDL_PixelFormat *format);

    /// @brief Free a palette created with SDL_AllocPalette(). https://wiki.libsdl.org/SDL2/SDL_FreePalette
    /// @param palette the SDL_Palette structure to be freed
    void FreePalette(SDL_Palette * palette);

    /// @brief Use this function to free an SDL_RWops structure allocated by SDL_AllocRW(). https://wiki.libsdl.org/SDL2/SDL_FreeRW
    /// @param area the SDL_RWops structure to be freed
    void FreeRW(SDL_RWops * area);

    /// @brief Free an RGB surface. https://wiki.libsdl.org/SDL2/SDL_FreeSurface
    /// @param surface the SDL_Surface to free.
    void FreeSurface(SDL_Surface * surface);

    /// @brief Free data previously allocated with SDL_LoadWAV() or SDL_LoadWAV_RW(). https://wiki.libsdl.org/SDL2/SDL_FreeWAV
    /// @param audio_buf a pointer to the buffer created by SDL_LoadWAV() or SDL_LoadWAV_RW()
    void FreeWAV(Uint8 * audio_buf);

    /// @brief Add support for controllers that SDL is unaware of or to cause an existing controller to have a different binding. https://wiki.libsdl.org/SDL2/SDL_GameControllerAddMapping
    /// @param mappingString the mapping string
    /// @return Returns 1 if a new mapping is added, 0 if an existing mapping is updated, -1 on error; call SDL_GetError() for more information.
    int GameControllerAddMapping(const char* mappingString);

    /// @brief Load a set of Game Controller mappings from a seekable SDL data stream. https://wiki.libsdl.org/SDL2/SDL_GameControllerAddMappingsFromRW
    /// @param rw the data stream for the mappings to be added
    /// @param freerw non-zero to close the stream after being read
    /// @return Returns the number of mappings added or -1 on error; call SDL_GetError() for more information.
    int GameControllerAddMappingsFromRW(SDL_RWops * rw, int freerw);

    /// @brief Close a game controller previously opened with SDL_GameControllerOpen(). https://wiki.libsdl.org/SDL2/SDL_GameControllerClose
    /// @param gamecontroller a game controller identifier previously returned by SDL_GameControllerOpen()
    void GameControllerClose(SDL_GameController *gamecontroller);

    /// @brief Query or change current state of Game Controller events. https://wiki.libsdl.org/SDL2/SDL_GameControllerEventState
    /// @param state can be one of SDL_QUERY, SDL_IGNORE, or SDL_ENABLE
    /// @return Returns the same value passed to the function, with exception to -1 (SDL_QUERY), which will return the current state.
    int GameControllerEventState(int state);

    /// @brief Get the SDL_GameController associated with an instance id. https://wiki.libsdl.org/SDL2/SDL_GameControllerFromInstanceID
    /// @param joyid the instance id to get the SDL_GameController for
    /// @return Returns an SDL_GameController on success or NULL on failure; call SDL_GetError() for more information.
    SDL_GameController* GameControllerFromInstanceID(SDL_JoystickID joyid);

    /// @brief Get the SDL_GameController associated with a player index. https://wiki.libsdl.org/SDL2/SDL_GameControllerFromPlayerIndex
    /// @param player_index the player index, which is not the device index or the instance id!
    /// @return Returns the SDL_GameController associated with a player index.
    SDL_GameController* GameControllerFromPlayerIndex(int player_index);

    /// @brief Return the sfSymbolsName for a given axis on a game controller on Apple platforms. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetAppleSFSymbolsNameForAxis
    /// @param gamecontroller the controller to query
    /// @param axis an axis on the game controller
    /// @return Returns the sfSymbolsName or NULL if the name can't be found
    const char* GameControllerGetAppleSFSymbolsNameForAxis(SDL_GameController *gamecontroller, SDL_GameControllerAxis axis);

    /// @brief Return the sfSymbolsName for a given button on a game controller on Apple platforms. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetAppleSFSymbolsNameForButton
    /// @param gamecontroller the controller to query
    /// @param button a button on the game controller
    /// @return Returns the sfSymbolsName or NULL if the name can't be found
    const char* GameControllerGetAppleSFSymbolsNameForButton(SDL_GameController *gamecontroller, SDL_GameControllerButton button);

    /// @brief Check if a controller has been opened and is currently connected. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetAttached
    /// @param gamecontroller a game controller identifier previously returned by SDL_GameControllerOpen()
    /// @return Returns SDL_TRUE if the controller has been opened and is currently connected, or SDL_FALSE if not.
    bool GameControllerGetAttached(SDL_GameController *gamecontroller);

    /// @brief Get the current state of an axis control on a game controller. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetAxis
    /// @param gamecontroller a game controller
    /// @param axis an axis index (one of the SDL_GameControllerAxis values)
    /// @return Returns axis state (including 0) on success or 0 (also) on failure; call SDL_GetError() for more information.
    Sint16 GameControllerGetAxis(SDL_GameController *gamecontroller, SDL_GameControllerAxis axis);

    /// @brief Convert a string into SDL_GameControllerAxis enum. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetAxisFromString
    /// @param str string representing a SDL_GameController axis
    /// @return Returns the SDL_GameControllerAxis enum corresponding to the input string, or SDL_CONTROLLER_AXIS_INVALID if no match was found.
    SDL_GameControllerAxis GameControllerGetAxisFromString(const char *str);

    /// @brief Get the SDL joystick layer binding for a controller axis mapping. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetBindForAxis
    /// @param gamecontroller a game controller
    /// @param axis an axis enum value (one of the SDL_GameControllerAxis values)
    /// @return Returns a SDL_GameControllerButtonBind describing the bind. On failure (like the given Controller axis doesn't exist on the device), its .bindType will be SDL_CONTROLLER_BINDTYPE_NONE.
    SDL_GameControllerButtonBind GameControllerGetBindForAxis(SDL_GameController *gamecontroller, SDL_GameControllerAxis axis);

    /// @brief Get the SDL joystick layer binding for a controller button mapping. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetBindForButton
    /// @param gamecontroller a game controller
    /// @param button an button enum value (an SDL_GameControllerButton value)
    /// @return Returns a SDL_GameControllerButtonBind describing the bind. On failure (like the given Controller button doesn't exist on the device), its .bindType will be SDL_CONTROLLER_BINDTYPE_NONE.
    SDL_GameControllerButtonBind GameControllerGetBindForButton(SDL_GameController *gamecontroller, SDL_GameControllerButton button);

    /// @brief Get the current state of a button on a game controller. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetButton
    /// @param gamecontroller a game controller
    /// @param button a button index (one of the SDL_GameControllerButton values)
    /// @return Returns 1 for pressed state or 0 for not pressed state or error; call SDL_GetError() for more information.
    Uint8 GameControllerGetButton(SDL_GameController *gamecontroller, SDL_GameControllerButton button);

    /// @brief Convert a string into an SDL_GameControllerButton enum. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetButtonFromString
    /// @param str string representing a SDL_GameController axis
    /// @return Returns the SDL_GameControllerButton enum corresponding to the input string, or SDL_CONTROLLER_AXIS_INVALID if no match was found.
    SDL_GameControllerButton GameControllerGetButtonFromString(const char *str);

    /// @brief Get the firmware version of an opened controller, if available. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetFirmwareVersion
    /// @param gamecontroller the game controller object to query.
    /// @return Return the controller firmware version, or zero if unavailable.
    uint16_t GameControllerGetFirmwareVersion(SDL_GameController *gamecontroller);

    /// @brief Get the Joystick ID from a Game Controller. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetJoystick
    /// @param gamecontroller the game controller object that you want to get a joystick from
    /// @return Returns a SDL_Joystick object; call SDL_GetError() for more information.
    SDL_Joystick* GameControllerGetJoystick(SDL_GameController *gamecontroller);

    /// @brief Get the number of supported simultaneous fingers on a touchpad on a game controller. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetNumTouchpadFingers
    int GameControllerGetNumTouchpadFingers(SDL_GameController *gamecontroller, int touchpad);

    /// @brief Get the number of touchpads on a game controller. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetNumTouchpads
    int GameControllerGetNumTouchpads(SDL_GameController *gamecontroller);

    /// @brief Get the player index of an opened game controller. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetPlayerIndex
    /// @param gamecontroller the game controller object to query.
    /// @return Returns the player index for controller, or -1 if it's not available.
    int GameControllerGetPlayerIndex(SDL_GameController *gamecontroller);

    /// @brief Get the USB product ID of an opened controller, if available. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetProduct
    /// @param gamecontroller the game controller object to query.
    /// @return Return the USB product ID, or zero if unavailable.
    uint16_t GameControllerGetProduct(SDL_GameController *gamecontroller);

    /// @brief Get the product version of an opened controller, if available. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetProductVersion
    /// @param gamecontroller the game controller object to query.
    /// @return Return the USB product version, or zero if unavailable.
    uint16_t GameControllerGetProductVersion(SDL_GameController *gamecontroller);

    /// @brief Get the current state of a game controller sensor. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetSensorData
    /// @param gamecontroller The controller to query
    /// @param type The type of sensor to query
    /// @param data A pointer filled with the current sensor state
    /// @param num_values The number of values to write to data
    /// @return Return 0 or -1 if an error occurred.
    int GameControllerGetSensorData(SDL_GameController *gamecontroller, SDL_SensorType type, float *data, int num_values);

    /// @brief Get the data rate (number of events per second) of a game controller sensor. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetSensorDataRate
    /// @param gamecontroller The controller to query
    /// @param type The type of sensor to query
    /// @return Return the data rate, or 0.0f if the data rate is not available.
    float GameControllerGetSensorDataRate(SDL_GameController *gamecontroller, SDL_SensorType type);

    /// @brief Get the current state of a game controller sensor with the timestamp of the last update. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetSensorDataWithTimestamp
    /// @param gamecontroller The controller to query
    /// @param type The type of sensor to query
    /// @param timestamp A pointer filled with the timestamp in microseconds of the current sensor reading if available, or 0 if not
    /// @param data A pointer filled with the current sensor state
    /// @param num_values The number of values to write to data
    /// @return Return 0 or -1 if an error occurred.
    int GameControllerGetSensorDataWithTimestamp(SDL_GameController *gamecontroller, SDL_SensorType type, Uint64 *timestamp, float *data, int num_values);

    /// @brief Get the serial number of an opened controller, if available. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetSerial
    /// @param gamecontroller the game controller object to query.
    /// @return Return the serial number, or NULL if unavailable.
    const char * GameControllerGetSerial(SDL_GameController *gamecontroller);

    /// @brief Convert from an SDL_GameControllerAxis enum to a string. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetStringForAxis
    /// @param axis an enum value for a given SDL_GameControllerAxis
    /// @return Returns a string for the given axis, or NULL if an invalid axis is specified. The string returned is of the format used by SDL_GameController mapping strings.
    const char* GameControllerGetStringForAxis(SDL_GameControllerAxis axis);

    /// @brief Convert from an SDL_GameControllerButton enum to a string. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetStringForButton
    /// @param button an enum value for a given SDL_GameControllerButton
    /// @return Returns a string for the given button, or NULL if an invalid button is specified. The string returned is of the format used by SDL_GameController mapping strings.
    const char* GameControllerGetStringForButton(SDL_GameControllerButton button);

    /// @brief Get the current state of a finger on a touchpad on a game controller. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetTouchpadFinger
    int GameControllerGetTouchpadFinger(SDL_GameController *gamecontroller, int touchpad, int finger, Uint8 *state, float *x, float *y, float *pressure);

    /// @brief Get the type of this currently opened controller https://wiki.libsdl.org/SDL2/SDL_GameControllerGetType
    /// @param gamecontroller the game controller object to query.
    /// @return Returns the controller type.
    SDL_GameControllerType GameControllerGetType(SDL_GameController *gamecontroller);

    /// @brief Get the USB vendor ID of an opened controller, if available. https://wiki.libsdl.org/SDL2/SDL_GameControllerGetVendor
    /// @param gamecontroller the game controller object to query.
    /// @return Return the USB vendor ID, or zero if unavailable.
    uint16_t GameControllerGetVendor(SDL_GameController *gamecontroller);

    /// @brief Query whether a game controller has a given axis. https://wiki.libsdl.org/SDL2/SDL_GameControllerHasAxis
    /// @param gamecontroller a game controller
    /// @param axis an axis enum value (an SDL_GameControllerAxis value)
    /// @return Returns SDL_TRUE if the controller has this axis, SDL_FALSE otherwise.
    bool  GameControllerHasAxis(SDL_GameController *gamecontroller, SDL_GameControllerAxis axis);

    /// @brief Query whether a game controller has a given button. https://wiki.libsdl.org/SDL2/SDL_GameControllerHasButton
    /// @param gamecontroller a game controller
    /// @param button a button enum value (an SDL_GameControllerButton value)
    /// @return Returns SDL_TRUE if the controller has this button, SDL_FALSE otherwise.
    bool GameControllerHasButton(SDL_GameController *gamecontroller, SDL_GameControllerButton button);

    /// @brief Query whether a game controller has an LED. https://wiki.libsdl.org/SDL2/SDL_GameControllerHasLED
    /// @param gamecontroller The controller to query
    /// @return Returns SDL_TRUE, or SDL_FALSE if this controller does not have a modifiable LED
    bool GameControllerHasLED(SDL_GameController *gamecontroller);

    /// @brief Query whether a game controller has rumble support. https://wiki.libsdl.org/SDL2/SDL_GameControllerHasRumble
    /// @param gamecontroller The controller to query
    /// @return Returns SDL_TRUE, or SDL_FALSE if this controller does not have rumble support
    bool GameControllerHasRumble(SDL_GameController *gamecontroller);

    /// @brief Query whether a game controller has rumble support on triggers. https://wiki.libsdl.org/SDL2/SDL_GameControllerHasRumbleTriggers
    /// @param gamecontroller The controller to query
    /// @return Returns SDL_TRUE, or SDL_FALSE if this controller does not have trigger rumble support
    bool GameControllerHasRumbleTriggers(SDL_GameController *gamecontroller);

    /// @brief Return whether a game controller has a particular sensor. https://wiki.libsdl.org/SDL2/SDL_GameControllerHasSensor
    /// @param gamecontroller The controller to query
    /// @param type The type of sensor to query
    /// @return Returns SDL_TRUE if the sensor exists, SDL_FALSE otherwise.
    bool GameControllerHasSensor(SDL_GameController *gamecontroller, SDL_SensorType type);

    /// @brief Query whether sensor data reporting is enabled for a game controller. https://wiki.libsdl.org/SDL2/SDL_GameControllerIsSensorEnabled
    /// @param gamecontroller The controller to query
    /// @param type The type of sensor to query
    /// @return Returns SDL_TRUE if the sensor is enabled, SDL_FALSE otherwise.
    bool GameControllerIsSensorEnabled(SDL_GameController *gamecontroller, SDL_SensorType type);

    /// @brief Get the current mapping of a Game Controller. https://wiki.libsdl.org/SDL2/SDL_GameControllerMapping
    /// @param gamecontroller the game controller you want to get the current mapping for
    /// @return Returns a string that has the controller's mapping or NULL if no mapping is available; call SDL_GetError() for more information.
    char * GameControllerMapping(SDL_GameController *gamecontroller);

    /// @brief Get the mapping of a game controller. https://wiki.libsdl.org/SDL2/SDL_GameControllerMappingForDeviceIndex
    /// @param joystick_index the device_index of a device, from zero to SDL_NumJoysticks()-1
    /// @return Returns the mapping string. Must be freed with SDL_free(). Returns NULL if no mapping is available.
    char* GameControllerMappingForDeviceIndex(int joystick_index);

    /// @brief Get the game controller mapping string for a given GUID. https://wiki.libsdl.org/SDL2/SDL_GameControllerMappingForGUID
    /// @param guid a structure containing the GUID for which a mapping is desired
    /// @return Returns a mapping string or NULL on error; call SDL_GetError() for more information.
    char * GameControllerMappingForGUID(SDL_JoystickGUID guid);

    /// @brief Get the mapping at a particular index. https://wiki.libsdl.org/SDL2/SDL_GameControllerMappingForIndex
    /// @return Returns the mapping string. Must be freed with SDL_free(). Returns NULL if the index is out of range.
    char * GameControllerMappingForIndex(int mapping_index);

    /// @brief Get the implementation-dependent name for an opened game controller. https://wiki.libsdl.org/SDL2/SDL_GameControllerName
    /// @param gamecontroller a game controller identifier previously returned by SDL_GameControllerOpen()
    /// @return Returns the implementation dependent name for the game controller, or NULL if there is no name or the identifier passed is invalid.
    const char* GameControllerName(SDL_GameController *gamecontroller);

    /// @brief Get the implementation dependent name for the game controller. https://wiki.libsdl.org/SDL2/SDL_GameControllerNameForIndex
    /// @param joystick_index the device_index of a device, from zero to SDL_NumJoysticks()-1
    /// @return Returns the implementation-dependent name for the game controller, or NULL if there is no name or the index is invalid.
    const char* GameControllerNameForIndex(int joystick_index);

    /// @brief Get the number of mappings installed. https://wiki.libsdl.org/SDL2/SDL_GameControllerNumMappings
    /// @return Returns the number of mappings.
    int GameControllerNumMappings();

    /// @brief Open a game controller for use. https://wiki.libsdl.org/SDL2/SDL_GameControllerOpen
    /// @param joystick_index the device_index of a device, up to SDL_NumJoysticks()
    /// @return Returns a gamecontroller identifier or NULL if an error occurred; call SDL_GetError() for more information.
    SDL_GameController* GameControllerOpen(int joystick_index);

    /// @brief Get the implementation-dependent path for an opened game controller. https://wiki.libsdl.org/SDL2/SDL_GameControllerPath
    /// @param gamecontroller a game controller identifier previously returned by SDL_GameControllerOpen()
    /// @return Returns the implementation dependent path for the game controller, or NULL if there is no path or the identifier passed is invalid.
    const char* GameControllerPath(SDL_GameController *gamecontroller);

    /// @brief Get the implementation dependent path for the game controller. https://wiki.libsdl.org/SDL2/SDL_GameControllerPathForIndex
    /// @param joystick_index the device_index of a device, from zero to SDL_NumJoysticks()-1
    /// @return Returns the implementation-dependent path for the game controller, or NULL if there is no path or the index is invalid.
    const char* GameControllerPathForIndex(int joystick_index);

    /// @brief Start a rumble effect on a game controller. https://wiki.libsdl.org/SDL2/SDL_GameControllerRumble
    /// @param gamecontroller The controller to vibrate
    /// @param low_frequency_rumble The intensity of the low frequency (left) rumble motor, from 0 to 0xFFFF
    /// @param high_frequency_rumble The intensity of the high frequency (right) rumble motor, from 0 to 0xFFFF
    /// @param duration_ms The duration of the rumble effect, in milliseconds
    /// @return Returns 0, or -1 if rumble isn't supported on this controller
    int GameControllerRumble(SDL_GameController *gamecontroller, uint16_t low_frequency_rumble, uint16_t high_frequency_rumble, Uint32 duration_ms);

    /// @brief Start a rumble effect in the game controller's triggers. https://wiki.libsdl.org/SDL2/SDL_GameControllerRumbleTriggers
    /// @param gamecontroller The controller to vibrate
    /// @param left_rumble The intensity of the left trigger rumble motor, from 0 to 0xFFFF
    /// @param right_rumble The intensity of the right trigger rumble motor, from 0 to 0xFFFF
    /// @param duration_ms The duration of the rumble effect, in milliseconds
    /// @return Returns 0, or -1 if trigger rumble isn't supported on this controller
    int GameControllerRumbleTriggers(SDL_GameController *gamecontroller, uint16_t left_rumble, uint16_t right_rumble, Uint32 duration_ms);

    /// @brief Send a controller specific effect packet https://wiki.libsdl.org/SDL2/SDL_GameControllerSendEffect
    /// @param gamecontroller The controller to affect
    /// @param data The data to send to the controller
    /// @param size The size of the data to send to the controller
    /// @return Returns 0, or -1 if this controller or driver doesn't support effect packets
    int GameControllerSendEffect(SDL_GameController *gamecontroller, void *data, int size);

    /// @brief Update a game controller's LED color. https://wiki.libsdl.org/SDL2/SDL_GameControllerSetLED
    /// @param gamecontroller The controller to update
    /// @param red The intensity of the red LED
    /// @param green The intensity of the green LED
    /// @param blue The intensity of the blue LED
    /// @return Returns 0, or -1 if this controller does not have a modifiable LED
    int GameControllerSetLED(SDL_GameController *gamecontroller, Uint8 red, Uint8 green, Uint8 blue);

    /// @brief Set the player index of an opened game controller. https://wiki.libsdl.org/SDL2/SDL_GameControllerSetPlayerIndex
    /// @param gamecontroller the game controller object to adjust.
    /// @param player_index Player index to assign to this controller, or -1 to clear the player index and turn off player LEDs.
    void GameControllerSetPlayerIndex(SDL_GameController *gamecontroller, int player_index);

    /// @brief Set whether data reporting for a game controller sensor is enabled. https://wiki.libsdl.org/SDL2/SDL_GameControllerSetSensorEnabled
    /// @param gamecontroller The controller to update
    /// @param type The type of sensor to enable/disable
    /// @param enabled Whether data reporting should be enabled
    /// @return Returns 0 or -1 if an error occurred.
    int GameControllerSetSensorEnabled(SDL_GameController *gamecontroller, SDL_SensorType type, bool enabled);

    /// @brief Get the type of a game controller. https://wiki.libsdl.org/SDL2/SDL_GameControllerTypeForIndex
    /// @param joystick_index the device_index of a device, from zero to SDL_NumJoysticks()-1
    /// @return Returns the controller type.
    SDL_GameControllerType GameControllerTypeForIndex(int joystick_index);

    /// @brief Manually pump game controller updates if not using the loop. https://wiki.libsdl.org/SDL2/SDL_GameControllerUpdate
    void GameControllerUpdate();

    /// @brief Get the current assertion handler. https://wiki.libsdl.org/SDL2/SDL_GetAssertionHandler
    /// @param puserdata pointer which is filled with the "userdata" pointer that was passed to SDL_SetAssertionHandler()
    /// @return Returns the SDL_AssertionHandler that is called when an assert triggers.
    SDL_AssertionHandler GetAssertionHandler(void **puserdata);

    /// @brief Get a list of all assertion failures. https://wiki.libsdl.org/SDL2/SDL_GetAssertionReport
    /// @return Returns a list of all failed assertions or NULL if the list is empty. This memory should not be modified or freed by the application.
    const SDL_AssertData * GetAssertionReport();

    /// @brief Get the human-readable name of a specific audio device. https://wiki.libsdl.org/SDL2/SDL_GetAudioDeviceName
    /// @param index the index of the audio device; valid values range from 0 to SDL_GetNumAudioDevices() - 1
    /// @param iscapture non-zero to query the list of recording devices, zero to query the list of output devices.
    /// @return Returns the name of the audio device at the requested index, or NULL on error.
    const char* GetAudioDeviceName(int index, int iscapture);

    /// @brief Get the preferred audio format of a specific audio device. https://wiki.libsdl.org/SDL2/SDL_GetAudioDeviceSpec
    /// @param index the index of the audio device; valid values range from 0 to SDL_GetNumAudioDevices() - 1
    /// @param iscapture non-zero to query the list of recording devices, zero to query the list of output devices.
    /// @param spec The SDL_AudioSpec to be initialized by this function.
    /// @return Returns 0 on success, nonzero on error
    int GetAudioDeviceSpec(int index, int iscapture, SDL_AudioSpec *spec);

    /// @brief Use this function to get the current audio state of an audio device. https://wiki.libsdl.org/SDL2/SDL_GetAudioDeviceStatus
    /// @param dev the ID of an audio device previously opened with SDL_OpenAudioDevice()
    /// @return Returns the SDL_AudioStatus of the specified audio device.
    SDL_AudioStatus GetAudioDeviceStatus(SDL_AudioDeviceID dev);

    /// @brief Use this function to get the name of a built in audio driver. https://wiki.libsdl.org/SDL2/SDL_GetAudioDriver
    /// @param index the index of the audio driver; the value ranges from 0 to SDL_GetNumAudioDrivers() - 1
    /// @return Returns the name of the audio driver at the requested index, or NULL if an invalid index was specified.
    const char* GetAudioDriver(int index);

    /// @brief This function is a legacy means of querying the audio device. https://wiki.libsdl.org/SDL2/SDL_GetAudioStatus
    /// @return Returns the SDL_AudioStatus of the audio device opened by SDL_OpenAudio().
    SDL_AudioStatus GetAudioStatus();

    /// @brief Get the directory where the application was run from. https://wiki.libsdl.org/SDL2/SDL_GetBasePath
    /// @return Returns an absolute path in UTF-8 encoding to the application data directory. NULL will be returned on error or when the platform doesn't implement this functionality, call SDL_GetError() for more information.
    char* GetBasePath();

    /// @brief Get UTF-8 text from the clipboard, which must be freed with SDL_free(). https://wiki.libsdl.org/SDL2/SDL_GetClipboardText
    /// @return Returns the clipboard text on success or an empty string on failure; call SDL_GetError() for more information. Caller must call SDL_free() on the returned pointer when done with it (even if there was an error).
    char * GetClipboardText();

    /// @brief Get the clipping rectangle for a surface. https://wiki.libsdl.org/SDL2/SDL_GetClipRect
    /// @param surface the SDL_Surface structure representing the surface to be clipped
    /// @param rect an SDL_Rect structure filled in with the clipping rectangle for the surface
    void GetClipRect(SDL_Surface * surface, SDL_Rect * rect);

    /// @brief Get the closest match to the requested display mode. https://wiki.libsdl.org/SDL2/SDL_GetClosestDisplayMode
    /// @param displayIndex the index of the display to query
    /// @param mode an SDL_DisplayMode structure containing the desired display mode
    /// @param closest an SDL_DisplayMode structure filled in with the closest match of the available display modes
    /// @return Returns the passed in value closest or NULL if no matching video mode was available; call SDL_GetError() for more information.
    SDL_DisplayMode * GetClosestDisplayMode(int displayIndex, const SDL_DisplayMode * mode, SDL_DisplayMode * closest);

    /// @brief Get the color key (transparent pixel) for a surface. https://wiki.libsdl.org/SDL2/SDL_GetColorKey
    /// @param surface the SDL_Surface structure to query
    /// @param key a pointer filled in with the transparent pixel
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetColorKey(SDL_Surface * surface, Uint32 * key);

    /// @brief Determine the L1 cache line size of the CPU. https://wiki.libsdl.org/SDL2/SDL_GetCPUCacheLineSize
    /// @return Returns the L1 cache line size of the CPU, in bytes.
    int GetCPUCacheLineSize();

    /// @brief Get the number of CPU cores available. https://wiki.libsdl.org/SDL2/SDL_GetCPUCount
    /// @return Returns the total number of logical CPU cores. On CPUs that include technologies such as hyperthreading, the number of logical cores may be more than the number of physical cores.
    int GetCPUCount();

    /// @brief Get the name of the current audio driver. https://wiki.libsdl.org/SDL2/SDL_GetCurrentAudioDriver
    /// @return Returns the name of the current audio driver or NULL if no driver has been initialized.
    const char* GetCurrentAudioDriver();

    /// @brief Get information about the current display mode. https://wiki.libsdl.org/SDL2/SDL_GetCurrentDisplayMode
    /// @param displayIndex the index of the display to query
    /// @param mode an SDL_DisplayMode structure filled in with the current display mode
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetCurrentDisplayMode(int displayIndex, SDL_DisplayMode * mode);

    /// @brief Get the name of the currently initialized video driver. https://wiki.libsdl.org/SDL2/SDL_GetCurrentVideoDriver
    /// @return Returns the name of the current video driver or NULL if no driver has been initialized.
    const char* GetCurrentVideoDriver();

    /// @brief Get the active cursor. https://wiki.libsdl.org/SDL2/SDL_GetCursor
    /// @return Returns the active cursor or NULL if there is no mouse.
    SDL_Cursor* GetCursor();

    /// @brief Get the default assertion handler. https://wiki.libsdl.org/SDL2/SDL_GetDefaultAssertionHandler
    /// @return Returns the default SDL_AssertionHandler that is called when an assert triggers.
    SDL_AssertionHandler GetDefaultAssertionHandler();

    /// @brief Get the name and preferred format of the default audio device. https://wiki.libsdl.org/SDL2/SDL_GetDefaultAudioInfo
    /// @param name A pointer to be filled with the name of the default device (can be NULL). Please call SDL_free() when you are done with this pointer!
    /// @param spec The SDL_AudioSpec to be initialized by this function.
    /// @param iscapture non-zero to query the default recording device, zero to query the default output device.
    /// @return Returns 0 on success, nonzero on error
    int GetDefaultAudioInfo(char **name, SDL_AudioSpec *spec, int iscapture);

    /// @brief Get the default cursor. https://wiki.libsdl.org/SDL2/SDL_GetDefaultCursor
    /// @return Returns the default cursor on success or NULL on failure.
    SDL_Cursor* GetDefaultCursor();

    /// @brief Get information about the desktop's display mode. https://wiki.libsdl.org/SDL2/SDL_GetDesktopDisplayMode
    /// @param displayIndex the index of the display to query
    /// @param mode an SDL_DisplayMode structure filled in with the current display mode
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetDesktopDisplayMode(int displayIndex, SDL_DisplayMode * mode);

    /// @brief Get the desktop area represented by a display. https://wiki.libsdl.org/SDL2/SDL_GetDisplayBounds
    /// @param displayIndex the index of the display to query
    /// @param rect the SDL_Rect structure filled in with the display bounds
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetDisplayBounds(int displayIndex, SDL_Rect * rect);

    /// @brief Get the dots/pixels-per-inch for a display. https://wiki.libsdl.org/SDL2/SDL_GetDisplayDPI
    /// @param displayIndex the index of the display from which DPI information should be queried
    /// @param ddpi a pointer filled in with the diagonal DPI of the display; may be NULL
    /// @param hdpi a pointer filled in with the horizontal DPI of the display; may be NULL
    /// @param vdpi a pointer filled in with the vertical DPI of the display; may be NULL
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetDisplayDPI(int displayIndex, float * ddpi, float * hdpi, float * vdpi);

    /// @brief Get information about a specific display mode. https://wiki.libsdl.org/SDL2/SDL_GetDisplayMode
    /// @param displayIndex the index of the display to query
    /// @param modeIndex the index of the display mode to query
    /// @param mode an SDL_DisplayMode structure filled in with the mode at modeIndex
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetDisplayMode(int displayIndex, int modeIndex, SDL_DisplayMode * mode);

    /// @brief Get the name of a display in UTF-8 encoding. https://wiki.libsdl.org/SDL2/SDL_GetDisplayName
    /// @param displayIndex the index of display from which the name should be queried
    /// @return Returns the name of a display or NULL for an invalid display index or failure; call SDL_GetError() for more information.
    const char * GetDisplayName(int displayIndex);

    /// @brief Get the orientation of a display. https://wiki.libsdl.org/SDL2/SDL_GetDisplayOrientation
    /// @param displayIndex the index of the display to query
    /// @return Returns The SDL_DisplayOrientation enum value of the display, or SDL_ORIENTATION_UNKNOWN if it isn't available.
    SDL_DisplayOrientation GetDisplayOrientation(int displayIndex);

    /// @brief Get the usable desktop area represented by a display. https://wiki.libsdl.org/SDL2/SDL_GetDisplayUsableBounds
    /// @param displayIndex the index of the display to query the usable bounds from
    /// @param rect the SDL_Rect structure filled in with the display bounds
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetDisplayUsableBounds(int displayIndex, SDL_Rect * rect);

    /// @brief Retrieve a message about the last error that occurred on the current thread. https://wiki.libsdl.org/SDL2/SDL_GetError
    /// @return Returns a message with information about the specific error that occurred, or an empty string if there hasn't been an error message set since the last call to SDL_ClearError(). The message is only applicable when an SDL function has signaled an error. You must check the return values of SDL function calls to determine when to appropriately call SDL_GetError().
    const char* GetError();

    /// @brief Get the last error message that was set for the current thread. https://wiki.libsdl.org/SDL2/SDL_GetErrorMsg
    /// @param errstr A buffer to fill with the last error message that was set for the current thread
    /// @param maxlen The size of the buffer pointed to by the errstr parameter
    /// @return Returns the pointer passed in as the errstr parameter.
    char * GetErrorMsg(char *errstr, int maxlen);

    /// @brief Query the current event filter. https://wiki.libsdl.org/SDL2/SDL_GetEventFilter
    /// @param filter the current callback function will be stored here
    /// @param userdata the pointer that is passed to the current event filter will be stored here
    /// @return Returns SDL_TRUE on success or SDL_FALSE if there is no event filter set.
    bool GetEventFilter(SDL_EventFilter * filter, void **userdata);

    /// @brief Use this macro to query the current processing state of a specified SDL_EventType. https://wiki.libsdl.org/SDL2/SDL_GetEventState
    /// @param type the SDL_EventType to query
    /// @return Returns SDL_DISABLE or SDL_ENABLE.
    uint8_t GetEventState(uint32_t type);

    /// @brief Get the current state of the mouse in relation to the desktop. https://wiki.libsdl.org/SDL2/SDL_GetGlobalMouseState
    /// @param x filled in with the current X coord relative to the desktop; can be NULL
    /// @param y filled in with the current Y coord relative to the desktop; can be NULL
    /// @return Returns the current button state as a bitmask which can be tested using the SDL_BUTTON(X) macros.
    Uint32 GetGlobalMouseState(int *x, int *y);

    /// @brief Get the window that currently has an input grab enabled. https://wiki.libsdl.org/SDL2/SDL_GetGrabbedWindow
    /// @return Returns the window if input is grabbed or NULL otherwise.
    SDL_Window * GetGrabbedWindow();

    /// @brief Get the value of a hint. https://wiki.libsdl.org/SDL2/SDL_GetHint
    /// @param name the hint to query
    /// @return Returns the string value of a hint or NULL if the hint isn't set.
    const char * GetHint(const char *name);

    /// @brief Get the boolean value of a hint variable. https://wiki.libsdl.org/SDL2/SDL_GetHintBoolean
    /// @param name the name of the hint to get the boolean value from
    /// @param default_value the value to return if the hint does not exist
    /// @return Returns the boolean value of a hint or the provided default value if the hint does not exist.
    bool GetHintBoolean(const char *name, bool default_value);

    /// @brief Get the device information encoded in a SDL_JoystickGUID structure https://wiki.libsdl.org/SDL2/SDL_GetJoystickGUIDInfo
    /// @param guid the SDL_JoystickGUID you wish to get info about
    /// @param vendor A pointer filled in with the device VID, or 0 if not available
    /// @param product A pointer filled in with the device PID, or 0 if not available
    /// @param version A pointer filled in with the device version, or 0 if not available
    /// @param crc16 A pointer filled in with a CRC used to distinguish different products with the same VID/PID, or 0 if not available
    void GetJoystickGUIDInfo(SDL_JoystickGUID guid, uint16_t *vendor, uint16_t *product, uint16_t *version, uint16_t *crc16);

    /// @brief Query the window which currently has keyboard focus. https://wiki.libsdl.org/SDL2/SDL_GetKeyboardFocus
    /// @return Returns the window with keyboard focus.
    SDL_Window * GetKeyboardFocus();

    /// @brief Get a snapshot of the current state of the keyboard. https://wiki.libsdl.org/SDL2/SDL_GetKeyboardState
    /// @param numkeys if non-NULL, receives the length of the returned array
    /// @return Returns a pointer to an array of key states.
    const Uint8* GetKeyboardState(int *numkeys);

    /// @brief Get a key code from a human-readable name. https://wiki.libsdl.org/SDL2/SDL_GetKeyFromName
    /// @param name the human-readable key name
    /// @return Returns key code, or SDLK_UNKNOWN if the name wasn't recognized; call SDL_GetError() for more information.
    SDL_Keycode GetKeyFromName(const char *name);

    /// @brief Get the key code corresponding to the given scancode according to the current keyboard layout. https://wiki.libsdl.org/SDL2/SDL_GetKeyFromScancode
    /// @param scancode the desired SDL_Scancode to query
    /// @return Returns the SDL_Keycode that corresponds to the given SDL_Scancode.
    SDL_Keycode GetKeyFromScancode(SDL_Scancode scancode);

    /// @brief Get a human-readable name for a key. https://wiki.libsdl.org/SDL2/SDL_GetKeyName
    /// @param key the desired SDL_Keycode to query
    /// @return Returns a pointer to a UTF-8 string that stays valid at least until the next call to this function. If you need it around any longer, you must copy it. If the key doesn't have a name, this function returns an empty string ("").
    const char* GetKeyName(SDL_Keycode key);

    /// @brief Get the current set of SDL memory functions https://wiki.libsdl.org/SDL2/SDL_GetMemoryFunctions
    void GetMemoryFunctions(SDL_malloc_func *malloc_func, SDL_calloc_func *calloc_func, SDL_realloc_func *realloc_func, SDL_free_func *free_func);

    /// @brief Get the current key modifier state for the keyboard. https://wiki.libsdl.org/SDL2/SDL_GetModState
    /// @return Returns an OR'd combination of the modifier keys for the keyboard. See SDL_Keymod for details.
    SDL_Keymod GetModState();

    /// @brief Get the window which currently has mouse focus. https://wiki.libsdl.org/SDL2/SDL_GetMouseFocus
    /// @return Returns the window with mouse focus.
    SDL_Window * GetMouseFocus();

    /// @brief Retrieve the current state of the mouse. https://wiki.libsdl.org/SDL2/SDL_GetMouseState
    /// @param x the x coordinate of the mouse cursor position relative to the focus window
    /// @param y the y coordinate of the mouse cursor position relative to the focus window
    /// @return Returns a 32-bit button bitmask of the current button state.
    Uint32 GetMouseState(int *x, int *y);

    /// @brief Get the number of outstanding (unfreed) allocations https://wiki.libsdl.org/SDL2/SDL_GetNumAllocations
    int GetNumAllocations();

    /// @brief Get the number of built-in audio devices. https://wiki.libsdl.org/SDL2/SDL_GetNumAudioDevices
    /// @param iscapture zero to request playback devices, non-zero to request recording devices
    /// @return Returns the number of available devices exposed by the current driver or -1 if an explicit list of devices can't be determined. A return value of -1 does not necessarily mean an error condition.
    int GetNumAudioDevices(int iscapture);

    /// @brief Use this function to get the number of built-in audio drivers. https://wiki.libsdl.org/SDL2/SDL_GetNumAudioDrivers
    /// @return Returns the number of built-in audio drivers.
    int GetNumAudioDrivers();

    /// @brief Get the number of available display modes. https://wiki.libsdl.org/SDL2/SDL_GetNumDisplayModes
    /// @param displayIndex the index of the display to query
    /// @return Returns a number >= 1 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetNumDisplayModes(int displayIndex);

    /// @brief Get the number of 2D rendering drivers available for the current display. https://wiki.libsdl.org/SDL2/SDL_GetNumRenderDrivers
    /// @return Returns a number >= 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetNumRenderDrivers();

    /// @brief Get the number of registered touch devices. https://wiki.libsdl.org/SDL2/SDL_GetNumTouchDevices
    /// @return Returns the number of registered touch devices.
    int GetNumTouchDevices();

    /// @brief Get the number of active fingers for a given touch device. https://wiki.libsdl.org/SDL2/SDL_GetNumTouchFingers
    /// @param touchID the ID of a touch device
    /// @return Returns the number of active fingers for a given touch device on success or 0 on failure; call SDL_GetError() for more information.
    int GetNumTouchFingers(SDL_TouchID touchID);

    /// @brief Get the number of available video displays. https://wiki.libsdl.org/SDL2/SDL_GetNumVideoDisplays
    /// @return Returns a number >= 1 or a negative error code on failure; call SDL_GetError() for more information.
    int GetNumVideoDisplays();

    /// @brief Get the number of video drivers compiled into SDL. https://wiki.libsdl.org/SDL2/SDL_GetNumVideoDrivers
    /// @return Returns a number >= 1 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetNumVideoDrivers();

    /// @brief Get the original set of SDL memory functions https://wiki.libsdl.org/SDL2/SDL_GetOriginalMemoryFunctions
    void GetOriginalMemoryFunctions(SDL_malloc_func *malloc_func, SDL_calloc_func *calloc_func, SDL_realloc_func *realloc_func, SDL_free_func *free_func);

    /// @brief Get the current value of the high resolution counter. https://wiki.libsdl.org/SDL2/SDL_GetPerformanceCounter
    /// @return Returns the current counter value.
    Uint64 GetPerformanceCounter();

    /// @brief Get the count per second of the high resolution counter. https://wiki.libsdl.org/SDL2/SDL_GetPerformanceFrequency
    /// @return Returns a platform-specific count per second.
    Uint64 GetPerformanceFrequency();

    /// @brief Get the human readable name of a pixel format. https://wiki.libsdl.org/SDL2/SDL_GetPixelFormatName
    /// @param format the pixel format to query
    /// @return Returns the human readable name of the specified pixel format or SDL_PIXELFORMAT_UNKNOWN if the format isn't recognized.
    const char* GetPixelFormatName(Uint32 format);

    /// @brief Get the name of the platform. https://wiki.libsdl.org/SDL2/SDL_GetPlatform
    /// @return Returns the name of the platform. If the correct platform name is not available, returns a string beginning with the text "Unknown".
    const char * GetPlatform();

    /// @brief Get the index of the display containing a point https://wiki.libsdl.org/SDL2/SDL_GetPointDisplayIndex
    /// @param point the point to query
    /// @return Returns the index of the display containing the point or a negative error code on failure; call SDL_GetError() for more information.
    int GetPointDisplayIndex(const SDL_Point * point);

    /// @brief Get the current power supply details. https://wiki.libsdl.org/SDL2/SDL_GetPowerInfo
    /// @param seconds seconds of battery life left, you can pass a NULL here if you don't care, will return -1 if we can't determine a value, or we're not running on a battery
    /// @param percent percentage of battery life left, between 0 and 100, you can pass a NULL here if you don't care, will return -1 if we can't determine a value, or we're not running on a battery
    /// @return Returns an SDL_PowerState enum representing the current battery state.
    SDL_PowerState GetPowerInfo(int *seconds, int *percent);

    /// @brief Report the user's preferred locale. https://wiki.libsdl.org/SDL2/SDL_GetPreferredLocales
    /// @return Return array of locales, terminated with a locale with a NULL language field. Will return NULL on error.
    SDL_Locale * GetPreferredLocales();

    /// @brief Get the user-and-app-specific path where files can be written. https://wiki.libsdl.org/SDL2/SDL_GetPrefPath
    /// @param org the name of your organization
    /// @param app the name of your application
    /// @return Returns a UTF-8 string of the user directory in platform-dependent notation. NULL if there's a problem (creating directory failed, etc.).
    char* GetPrefPath(const char *org, const char *app);

    /// @brief Get UTF-8 text from the primary selection, which must be freed with SDL_free(). https://wiki.libsdl.org/SDL2/SDL_GetPrimarySelectionText
    /// @return Returns the primary selection text on success or an empty string on failure; call SDL_GetError() for more information. Caller must call SDL_free() on the returned pointer when done with it (even if there was an error).
    char * GetPrimarySelectionText();

    /// @brief Get the number of bytes of still-queued audio. https://wiki.libsdl.org/SDL2/SDL_GetQueuedAudioSize
    /// @param dev the device ID of which we will query queued audio size
    /// @return Returns the number of bytes (not samples!) of queued audio.
    Uint32 GetQueuedAudioSize(SDL_AudioDeviceID dev);

    /// @brief Get the index of the display primarily containing a rect https://wiki.libsdl.org/SDL2/SDL_GetRectDisplayIndex
    /// @param rect the rect to query
    /// @return Returns the index of the display entirely containing the rect or closest to the center of the rect on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetRectDisplayIndex(const SDL_Rect * rect);

    /// @brief Query whether relative mouse mode is enabled. https://wiki.libsdl.org/SDL2/SDL_GetRelativeMouseMode
    /// @return Returns SDL_TRUE if relative mode is enabled or SDL_FALSE otherwise.
    bool GetRelativeMouseMode();

    /// @brief Retrieve the relative state of the mouse. https://wiki.libsdl.org/SDL2/SDL_GetRelativeMouseState
    /// @param x a pointer filled with the last recorded x coordinate of the mouse
    /// @param y a pointer filled with the last recorded y coordinate of the mouse
    /// @return Returns a 32-bit button bitmask of the relative button state.
    Uint32 GetRelativeMouseState(int *x, int *y);

    /// @brief Get the blend mode used for drawing operations. https://wiki.libsdl.org/SDL2/SDL_GetRenderDrawBlendMode
    /// @param renderer the rendering context
    /// @param blendMode a pointer filled in with the current SDL_BlendMode
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetRenderDrawBlendMode(SDL_Renderer * renderer, SDL_BlendMode *blendMode);

    /// @brief Get the color used for drawing operations (Rect, Line and Clear). https://wiki.libsdl.org/SDL2/SDL_GetRenderDrawColor
    /// @param renderer the rendering context
    /// @param r a pointer filled in with the red value used to draw on the rendering target
    /// @param g a pointer filled in with the green value used to draw on the rendering target
    /// @param b a pointer filled in with the blue value used to draw on the rendering target
    /// @param a a pointer filled in with the alpha value used to draw on the rendering target; usually SDL_ALPHA_OPAQUE (255)
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetRenderDrawColor(SDL_Renderer * renderer, Uint8 * r, Uint8 * g, Uint8 * b, Uint8 * a);

    /// @brief Get info about a specific 2D rendering driver for the current display. https://wiki.libsdl.org/SDL2/SDL_GetRenderDriverInfo
    /// @param index the index of the driver to query information about
    /// @param info an SDL_RendererInfo structure to be filled with information on the rendering driver
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetRenderDriverInfo(int index, SDL_RendererInfo * info);

    /// @brief Get the renderer associated with a window. https://wiki.libsdl.org/SDL2/SDL_GetRenderer
    /// @param window the window to query
    /// @return Returns the rendering context on success or NULL on failure; call SDL_GetError() for more information.
    SDL_Renderer * GetRenderer(SDL_Window * window);

    /// @brief Get information about a rendering context. https://wiki.libsdl.org/SDL2/SDL_GetRendererInfo
    /// @param renderer the rendering context
    /// @param info an SDL_RendererInfo structure filled with information about the current renderer
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetRendererInfo(SDL_Renderer * renderer, SDL_RendererInfo * info);

    /// @brief Get the output size in pixels of a rendering context. https://wiki.libsdl.org/SDL2/SDL_GetRendererOutputSize
    /// @param renderer the rendering context
    /// @param w an int filled with the width
    /// @param h an int filled with the height
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetRendererOutputSize(SDL_Renderer * renderer, int *w, int *h);

    /// @brief Get the current render target. https://wiki.libsdl.org/SDL2/SDL_GetRenderTarget
    /// @param renderer the rendering context
    /// @return Returns the current render target or NULL for the default render target.
    SDL_Texture * GetRenderTarget(SDL_Renderer *renderer);

    /// @brief Get the code revision of SDL that is linked against your program. https://wiki.libsdl.org/SDL2/SDL_GetRevision
    /// @return Returns an arbitrary string, uniquely identifying the exact revision of the SDL library in use.
    const char* GetRevision();

    /// @brief Obsolete function, do not use. https://wiki.libsdl.org/SDL2/SDL_GetRevisionNumber
    /// @return Returns zero, always, in modern SDL releases.
    int GetRevisionNumber();

    /// @brief Get RGB values from a pixel in the specified format. https://wiki.libsdl.org/SDL2/SDL_GetRGB
    /// @param pixel a pixel value
    /// @param format an SDL_PixelFormat structure describing the format of the pixel
    /// @param r a pointer filled in with the red component
    /// @param g a pointer filled in with the green component
    /// @param b a pointer filled in with the blue component
    void GetRGB(Uint32 pixel, const SDL_PixelFormat * format, Uint8 * r, Uint8 * g, Uint8 * b);

    /// @brief Get RGBA values from a pixel in the specified format. https://wiki.libsdl.org/SDL2/SDL_GetRGBA
    /// @param pixel a pixel value
    /// @param format an SDL_PixelFormat structure describing the format of the pixel
    /// @param r a pointer filled in with the red component
    /// @param g a pointer filled in with the green component
    /// @param b a pointer filled in with the blue component
    /// @param a a pointer filled in with the alpha component
    void GetRGBA(Uint32 pixel, const SDL_PixelFormat * format, Uint8 * r, Uint8 * g, Uint8 * b, Uint8 * a);

    /// @brief Get the scancode corresponding to the given key code according to the current keyboard layout. https://wiki.libsdl.org/SDL2/SDL_GetScancodeFromKey
    /// @param key the desired SDL_Keycode to query
    /// @return Returns the SDL_Scancode that corresponds to the given SDL_Keycode.
    SDL_Scancode GetScancodeFromKey(SDL_Keycode key);

    /// @brief Get a scancode from a human-readable name. https://wiki.libsdl.org/SDL2/SDL_GetScancodeFromName
    /// @param name the human-readable scancode name
    /// @return Returns the SDL_Scancode, or SDL_SCANCODE_UNKNOWN if the name wasn't recognized; call SDL_GetError() for more information.
    SDL_Scancode GetScancodeFromName(const char *name);

    /// @brief Get a human-readable name for a scancode. https://wiki.libsdl.org/SDL2/SDL_GetScancodeName
    /// @param scancode the desired SDL_Scancode to query
    /// @return Returns a pointer to the name for the scancode. If the scancode doesn't have a name this function returns an empty string ("").
    const char* GetScancodeName(SDL_Scancode scancode);

    /// @brief Get the shape parameters of a shaped window. https://wiki.libsdl.org/SDL2/SDL_GetShapedWindowMode
    /// @param window The shaped window whose parameters should be retrieved.
    /// @param shape_mode An empty shape-mode structure to fill, or NULL to check whether the window has a shape.
    /// @return Return 0 if the window has a shape and, provided shape_mode was not NULL, shape_mode has been filled with the mode data, SDL_NONSHAPEABLE_WINDOW if the SDL_Window given is not a shaped window, or SDL_WINDOW_LACKS_SHAPE if the SDL_Window given is a shapeable window currently lacking a shape.
    int GetShapedWindowMode(SDL_Window *window,SDL_WindowShapeMode *shape_mode);

    /// @brief Get the additional alpha value used in blit operations. https://wiki.libsdl.org/SDL2/SDL_GetSurfaceAlphaMod
    /// @param surface the SDL_Surface structure to query
    /// @param alpha a pointer filled in with the current alpha value
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetSurfaceAlphaMod(SDL_Surface * surface, Uint8 * alpha);

    /// @brief Get the blend mode used for blit operations. https://wiki.libsdl.org/SDL2/SDL_GetSurfaceBlendMode
    /// @param surface the SDL_Surface structure to query
    /// @param blendMode a pointer filled in with the current SDL_BlendMode
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetSurfaceBlendMode(SDL_Surface * surface, SDL_BlendMode *blendMode);

    /// @brief Get the additional color value multiplied into blit operations. https://wiki.libsdl.org/SDL2/SDL_GetSurfaceColorMod
    /// @param surface the SDL_Surface structure to query
    /// @param r a pointer filled in with the current red color value
    /// @param g a pointer filled in with the current green color value
    /// @param b a pointer filled in with the current blue color value
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetSurfaceColorMod(SDL_Surface * surface, Uint8 * r, Uint8 * g, Uint8 * b);

    /// @brief Get the amount of RAM configured in the system. https://wiki.libsdl.org/SDL2/SDL_GetSystemRAM
    /// @return Returns the amount of RAM configured in the system in MiB.
    int GetSystemRAM();

    /// @brief Get the additional alpha value multiplied into render copy operations. https://wiki.libsdl.org/SDL2/SDL_GetTextureAlphaMod
    /// @param texture the texture to query
    /// @param alpha a pointer filled in with the current alpha value
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetTextureAlphaMod(SDL_Texture * texture, Uint8 * alpha);

    /// @brief Get the blend mode used for texture copy operations. https://wiki.libsdl.org/SDL2/SDL_GetTextureBlendMode
    /// @param texture the texture to query
    /// @param blendMode a pointer filled in with the current SDL_BlendMode
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetTextureBlendMode(SDL_Texture * texture, SDL_BlendMode *blendMode);

    /// @brief Get the additional color value multiplied into render copy operations. https://wiki.libsdl.org/SDL2/SDL_GetTextureColorMod
    /// @param texture the texture to query
    /// @param r a pointer filled in with the current red color value
    /// @param g a pointer filled in with the current green color value
    /// @param b a pointer filled in with the current blue color value
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetTextureColorMod(SDL_Texture * texture, Uint8 * r, Uint8 * g, Uint8 * b);

    /// @brief Get the scale mode used for texture scale operations. https://wiki.libsdl.org/SDL2/SDL_GetTextureScaleMode
    /// @param texture the texture to query.
    /// @param scaleMode a pointer filled in with the current scale mode.
    /// @return Return 0 on success, or -1 if the texture is not valid.
    int GetTextureScaleMode(SDL_Texture * texture, SDL_ScaleMode *scaleMode);

    /// @brief Get the user-specified pointer associated with a texture https://wiki.libsdl.org/SDL2/SDL_GetTextureUserData
    /// @param texture the texture to query.
    /// @return Return the pointer associated with the texture, or NULL if the texture is not valid.
    void * GetTextureUserData(SDL_Texture * texture);

    /// @brief Get the thread identifier for the specified thread. https://wiki.libsdl.org/SDL2/SDL_GetThreadID
    /// @param thread the thread to query
    /// @return Returns the ID of the specified thread, or the ID of the current thread if thread is NULL.
    SDL_threadID GetThreadID(SDL_Thread * thread);

    /// @brief Get the thread name as it was specified in SDL_CreateThread(). https://wiki.libsdl.org/SDL2/SDL_GetThreadName
    /// @param thread the thread to query
    /// @return Returns a pointer to a UTF-8 string that names the specified thread, or NULL if it doesn't have a name.
    const char* GetThreadName(SDL_Thread *thread);

    /// @brief Get the number of milliseconds since SDL library initialization. https://wiki.libsdl.org/SDL2/SDL_GetTicks
    /// @return Returns an unsigned 32-bit value representing the number of milliseconds since the SDL library initialized.
    Uint32 GetTicks();

    /// @brief Get the number of milliseconds since SDL library initialization. https://wiki.libsdl.org/SDL2/SDL_GetTicks64
    /// @return Returns an unsigned 64-bit value representing the number of milliseconds since the SDL library initialized.
    Uint64 GetTicks64();

    /// @brief Get the touch ID with the given index. https://wiki.libsdl.org/SDL2/SDL_GetTouchDevice
    /// @param index the touch device index
    /// @return Returns the touch ID with the given index on success or 0 if the index is invalid; call SDL_GetError() for more information.
    SDL_TouchID GetTouchDevice(int index);

    /// @brief Get the type of the given touch device. https://wiki.libsdl.org/SDL2/SDL_GetTouchDeviceType
    SDL_TouchDeviceType GetTouchDeviceType(SDL_TouchID touchID);

    /// @brief Get the finger object for specified touch device ID and finger index. https://wiki.libsdl.org/SDL2/SDL_GetTouchFinger
    /// @param touchID the ID of the requested touch device
    /// @param index the index of the requested finger
    /// @return Returns a pointer to the SDL_Finger object or NULL if no object at the given ID and index could be found.
    SDL_Finger * GetTouchFinger(SDL_TouchID touchID, int index);

    /// @brief Get the touch device name as reported from the driver or NULL if the index is invalid. https://wiki.libsdl.org/SDL2/SDL_GetTouchName
    const char* GetTouchName(int index);

    /// @brief Get the version of SDL that is linked against your program. https://wiki.libsdl.org/SDL2/SDL_GetVersion
    /// @param ver the SDL_version structure that contains the version information
    void GetVersion(SDL_version * ver);

    /// @brief Get the name of a built in video driver. https://wiki.libsdl.org/SDL2/SDL_GetVideoDriver
    /// @param index the index of a video driver
    /// @return Returns the name of the video driver with the given index.
    const char* GetVideoDriver(int index);

    /// @brief Get the size of a window's borders (decorations) around the client area. https://wiki.libsdl.org/SDL2/SDL_GetWindowBordersSize
    /// @param window the window to query the size values of the border (decorations) from
    /// @param top pointer to variable for storing the size of the top border; NULL is permitted
    /// @param left pointer to variable for storing the size of the left border; NULL is permitted
    /// @param bottom pointer to variable for storing the size of the bottom border; NULL is permitted
    /// @param right pointer to variable for storing the size of the right border; NULL is permitted
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetWindowBordersSize(SDL_Window * window, int *top, int *left, int *bottom, int *right);

    /// @brief Get the brightness (gamma multiplier) for a given window's display. https://wiki.libsdl.org/SDL2/SDL_GetWindowBrightness
    /// @param window the window used to select the display whose brightness will be queried
    /// @return Returns the brightness for the display where 0.0 is completely dark and 1.0 is normal brightness.
    float GetWindowBrightness(SDL_Window * window);

    /// @brief Retrieve the data pointer associated with a window. https://wiki.libsdl.org/SDL2/SDL_GetWindowData
    /// @param window the window to query
    /// @param name the name of the pointer
    /// @return Returns the value associated with name.
    void* GetWindowData(SDL_Window * window, const char *name);

    /// @brief Get the index of the display associated with a window. https://wiki.libsdl.org/SDL2/SDL_GetWindowDisplayIndex
    /// @param window the window to query
    /// @return Returns the index of the display containing the center of the window on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetWindowDisplayIndex(SDL_Window * window);

    /// @brief Query the display mode to use when a window is visible at fullscreen. https://wiki.libsdl.org/SDL2/SDL_GetWindowDisplayMode
    /// @param window the window to query
    /// @param mode an SDL_DisplayMode structure filled in with the fullscreen display mode
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetWindowDisplayMode(SDL_Window * window, SDL_DisplayMode * mode);

    /// @brief Get the window flags. https://wiki.libsdl.org/SDL2/SDL_GetWindowFlags
    /// @param window the window to query
    /// @return Returns a mask of the SDL_WindowFlags associated with window
    Uint32 GetWindowFlags(SDL_Window * window);

    /// @brief Get a window from a stored ID. https://wiki.libsdl.org/SDL2/SDL_GetWindowFromID
    /// @param id the ID of the window
    /// @return Returns the window associated with id or NULL if it doesn't exist; call SDL_GetError() for more information.
    SDL_Window * GetWindowFromID(Uint32 id);

    /// @brief Get the gamma ramp for a given window's display. https://wiki.libsdl.org/SDL2/SDL_GetWindowGammaRamp
    /// @param window the window used to select the display whose gamma ramp will be queried
    /// @param red a 256 element array of 16-bit quantities filled in with the translation table for the red channel, or NULL
    /// @param green a 256 element array of 16-bit quantities filled in with the translation table for the green channel, or NULL
    /// @param blue a 256 element array of 16-bit quantities filled in with the translation table for the blue channel, or NULL
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetWindowGammaRamp(SDL_Window * window, uint16_t * red, uint16_t * green, uint16_t * blue);

    /// @brief Get a window's input grab mode. https://wiki.libsdl.org/SDL2/SDL_GetWindowGrab
    /// @param window the window to query
    /// @return Returns SDL_TRUE if input is grabbed, SDL_FALSE otherwise.
    bool GetWindowGrab(SDL_Window * window);

    /// @brief Get the raw ICC profile data for the screen the window is currently on. https://wiki.libsdl.org/SDL2/SDL_GetWindowICCProfile
    /// @param window the window to query
    /// @param size the size of the ICC profile
    /// @return Returns the raw ICC profile data on success or NULL on failure; call SDL_GetError() for more information.
    void* GetWindowICCProfile(SDL_Window * window, size_t* size);

    /// @brief Get the numeric ID of a window. https://wiki.libsdl.org/SDL2/SDL_GetWindowID
    /// @param window the window to query
    /// @return Returns the ID of the window on success or 0 on failure; call SDL_GetError() for more information.
    Uint32 GetWindowID(SDL_Window * window);

    /// @brief Get a window's keyboard grab mode. https://wiki.libsdl.org/SDL2/SDL_GetWindowKeyboardGrab
    /// @param window the window to query
    /// @return Returns SDL_TRUE if keyboard is grabbed, and SDL_FALSE otherwise.
    bool GetWindowKeyboardGrab(SDL_Window * window);

    /// @brief Get the maximum size of a window's client area. https://wiki.libsdl.org/SDL2/SDL_GetWindowMaximumSize
    /// @param window the window to query
    /// @param w a pointer filled in with the maximum width of the window, may be NULL
    /// @param h a pointer filled in with the maximum height of the window, may be NULL
    void GetWindowMaximumSize(SDL_Window * window, int *w, int *h);

    /// @brief Get the minimum size of a window's client area. https://wiki.libsdl.org/SDL2/SDL_GetWindowMinimumSize
    /// @param window the window to query
    /// @param w a pointer filled in with the minimum width of the window, may be NULL
    /// @param h a pointer filled in with the minimum height of the window, may be NULL
    void GetWindowMinimumSize(SDL_Window * window, int *w, int *h);

    /// @brief Get a window's mouse grab mode. https://wiki.libsdl.org/SDL2/SDL_GetWindowMouseGrab
    /// @param window the window to query
    /// @return Returns SDL_TRUE if mouse is grabbed, and SDL_FALSE otherwise.
    bool GetWindowMouseGrab(SDL_Window * window);

    /// @brief Get the mouse confinement rectangle of a window. https://wiki.libsdl.org/SDL2/SDL_GetWindowMouseRect
    /// @param window The window to query
    /// @return Returns A pointer to the mouse confinement rectangle of a window, or NULL if there isn't one.
    const SDL_Rect * GetWindowMouseRect(SDL_Window * window);

    /// @brief Get the opacity of a window. https://wiki.libsdl.org/SDL2/SDL_GetWindowOpacity
    /// @param window the window to get the current opacity value from
    /// @param out_opacity the float filled in (0.0f - transparent, 1.0f - opaque)
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GetWindowOpacity(SDL_Window * window, float * out_opacity);

    /// @brief Get the pixel format associated with the window. https://wiki.libsdl.org/SDL2/SDL_GetWindowPixelFormat
    /// @param window the window to query
    /// @return Returns the pixel format of the window on success or SDL_PIXELFORMAT_UNKNOWN on failure; call SDL_GetError() for more information.
    Uint32 GetWindowPixelFormat(SDL_Window * window);

    /// @brief Get the position of a window. https://wiki.libsdl.org/SDL2/SDL_GetWindowPosition
    /// @param window the window to query
    /// @param x a pointer filled in with the x position of the window, in screen coordinates, may be NULL
    /// @param y a pointer filled in with the y position of the window, in screen coordinates, may be NULL
    void GetWindowPosition(SDL_Window * window, int *x, int *y);

    /// @brief Get the size of a window's client area. https://wiki.libsdl.org/SDL2/SDL_GetWindowSize
    /// @param window the window to query the width and height from
    /// @param w a pointer filled in with the width of the window, in screen coordinates, may be NULL
    /// @param h a pointer filled in with the height of the window, in screen coordinates, may be NULL
    void GetWindowSize(SDL_Window * window, int *w, int *h);

    /// @brief Get the size of a window in pixels. https://wiki.libsdl.org/SDL2/SDL_GetWindowSizeInPixels
    /// @param window the window from which the drawable size should be queried
    /// @param w a pointer to variable for storing the width in pixels, may be NULL
    /// @param h a pointer to variable for storing the height in pixels, may be NULL
    void GetWindowSizeInPixels(SDL_Window * window, int *w, int *h);

    /// @brief Get the SDL surface associated with the window. https://wiki.libsdl.org/SDL2/SDL_GetWindowSurface
    /// @param window the window to query
    /// @return Returns the surface associated with the window, or NULL on failure; call SDL_GetError() for more information.
    SDL_Surface * GetWindowSurface(SDL_Window * window);

    /// @brief Get the title of a window. https://wiki.libsdl.org/SDL2/SDL_GetWindowTitle
    /// @param window the window to query
    /// @return Returns the title of the window in UTF-8 format or "" if there is no title.
    const char* GetWindowTitle(SDL_Window * window);

    /// @brief Get the YUV conversion mode https://wiki.libsdl.org/SDL2/SDL_GetYUVConversionMode
    SDL_YUV_CONVERSION_MODE GetYUVConversionMode();

    /// @brief Get the YUV conversion mode, returning the correct mode for the resolution when the current conversion mode is SDL_YUV_CONVERSION_AUTOMATIC https://wiki.libsdl.org/SDL2/SDL_GetYUVConversionModeForResolution
    SDL_YUV_CONVERSION_MODE GetYUVConversionModeForResolution(int width, int height);

    /// @brief Bind an OpenGL/ES/ES2 texture to the current context. https://wiki.libsdl.org/SDL2/SDL_GL_BindTexture
    /// @param texture the texture to bind to the current OpenGL/ES/ES2 context
    /// @param texw a pointer to a float value which will be filled with the texture width or NULL if you don't need that value
    /// @param texh a pointer to a float value which will be filled with the texture height or NULL if you don't need that value
    /// @return Returns 0 on success, or -1 if the operation is not supported; call SDL_GetError() for more information.
    int GL_BindTexture(SDL_Texture *texture, float *texw, float *texh);

    /// @brief Create an OpenGL context for an OpenGL window, and make it current. https://wiki.libsdl.org/SDL2/SDL_GL_CreateContext
    /// @param window the window to associate with the context
    /// @return Returns the OpenGL context associated with window or NULL on error; call SDL_GetError() for more details.
    SDL_GLContext GL_CreateContext(SDL_Window * window);

    /// @brief Delete an OpenGL context. https://wiki.libsdl.org/SDL2/SDL_GL_DeleteContext
    /// @param context the OpenGL context to be deleted
    void GL_DeleteContext(SDL_GLContext context);

    /// @brief Check if an OpenGL extension is supported for the current context. https://wiki.libsdl.org/SDL2/SDL_GL_ExtensionSupported
    /// @param extension the name of the extension to check
    /// @return Returns SDL_TRUE if the extension is supported, SDL_FALSE otherwise.
    bool GL_ExtensionSupported(const char *extension);

    /// @brief Get the actual value for an attribute from the current context. https://wiki.libsdl.org/SDL2/SDL_GL_GetAttribute
    /// @param attr an SDL_GLattr enum value specifying the OpenGL attribute to get
    /// @param value a pointer filled in with the current value of attr
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GL_GetAttribute(SDL_GLattr attr, int *value);

    /// @brief Get the currently active OpenGL context. https://wiki.libsdl.org/SDL2/SDL_GL_GetCurrentContext
    /// @return Returns the currently active OpenGL context or NULL on failure; call SDL_GetError() for more information.
    SDL_GLContext GL_GetCurrentContext();

    /// @brief Get the currently active OpenGL window. https://wiki.libsdl.org/SDL2/SDL_GL_GetCurrentWindow
    /// @return Returns the currently active OpenGL window on success or NULL on failure; call SDL_GetError() for more information.
    SDL_Window* GL_GetCurrentWindow();

    /// @brief Get the size of a window's underlying drawable in pixels. https://wiki.libsdl.org/SDL2/SDL_GL_GetDrawableSize
    /// @param window the window from which the drawable size should be queried
    /// @param w a pointer to variable for storing the width in pixels, may be NULL
    /// @param h a pointer to variable for storing the height in pixels, may be NULL
    void GL_GetDrawableSize(SDL_Window * window, int *w, int *h);

    /// @brief Get an OpenGL function by name. https://wiki.libsdl.org/SDL2/SDL_GL_GetProcAddress
    /// @param proc the name of an OpenGL function
    /// @return Returns a pointer to the named OpenGL function. The returned pointer should be cast to the appropriate function signature.
    void* GL_GetProcAddress(const char *proc);

    /// @brief Get the swap interval for the current OpenGL context. https://wiki.libsdl.org/SDL2/SDL_GL_GetSwapInterval
    /// @return Returns 0 if there is no vertical retrace synchronization, 1 if the buffer swap is synchronized with the vertical retrace, and -1 if late swaps happen immediately instead of waiting for the next retrace; call SDL_GetError() for more information.
    int GL_GetSwapInterval();

    /// @brief Dynamically load an OpenGL library. https://wiki.libsdl.org/SDL2/SDL_GL_LoadLibrary
    /// @param path the platform dependent OpenGL library name, or NULL to open the default OpenGL library
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GL_LoadLibrary(const char *path);

    /// @brief Set up an OpenGL context for rendering into an OpenGL window. https://wiki.libsdl.org/SDL2/SDL_GL_MakeCurrent
    /// @param window the window to associate with the context
    /// @param context the OpenGL context to associate with the window
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GL_MakeCurrent(SDL_Window * window, SDL_GLContext context);

    /// @brief Reset all previously set OpenGL context attributes to their default values. https://wiki.libsdl.org/SDL2/SDL_GL_ResetAttributes
    void GL_ResetAttributes();

    /// @brief Set an OpenGL window attribute before window creation. https://wiki.libsdl.org/SDL2/SDL_GL_SetAttribute
    /// @param attr an SDL_GLattr enum value specifying the OpenGL attribute to set
    /// @param value the desired value for the attribute
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int GL_SetAttribute(SDL_GLattr attr, int value);

    /// @brief Set the swap interval for the current OpenGL context. https://wiki.libsdl.org/SDL2/SDL_GL_SetSwapInterval
    /// @param interval 0 for immediate updates, 1 for updates synchronized with the vertical retrace, -1 for adaptive vsync
    /// @return Returns 0 on success or -1 if setting the swap interval is not supported; call SDL_GetError() for more information.
    int GL_SetSwapInterval(int interval);

    /// @brief Update a window with OpenGL rendering. https://wiki.libsdl.org/SDL2/SDL_GL_SwapWindow
    /// @param window the window to change
    void GL_SwapWindow(SDL_Window * window);

    /// @brief Unbind an OpenGL/ES/ES2 texture from the current context. https://wiki.libsdl.org/SDL2/SDL_GL_UnbindTexture
    /// @param texture the texture to unbind from the current OpenGL/ES/ES2 context
    /// @return Returns 0 on success, or -1 if the operation is not supported
    int GL_UnbindTexture(SDL_Texture *texture);

    /// @brief Unload the OpenGL library previously loaded by SDL_GL_LoadLibrary(). https://wiki.libsdl.org/SDL2/SDL_GL_UnloadLibrary
    void GL_UnloadLibrary();

    /// @brief Convert a GUID string into a ::SDL_GUID structure. https://wiki.libsdl.org/SDL2/SDL_GUIDFromString
    /// @param pchGUID string containing an ASCII representation of a GUID
    /// @return Returns a ::SDL_GUID structure.
    SDL_GUID GUIDFromString(const char *pchGUID);

    /// @brief Get an ASCII string representation for a given ::SDL_GUID. https://wiki.libsdl.org/SDL2/SDL_GUIDToString
    /// @param guid the ::SDL_GUID you wish to convert to string
    /// @param pszGUID buffer in which to write the ASCII string
    /// @param cbGUID the size of pszGUID
    void GUIDToString(SDL_GUID guid, char *pszGUID, int cbGUID);

    /// @brief Close a haptic device previously opened with SDL_HapticOpen(). https://wiki.libsdl.org/SDL2/SDL_HapticClose
    /// @param haptic the SDL_Haptic device to close
    void HapticClose(SDL_Haptic * haptic);

    /// @brief Destroy a haptic effect on the device. https://wiki.libsdl.org/SDL2/SDL_HapticDestroyEffect
    /// @param haptic the SDL_Haptic device to destroy the effect on
    /// @param effect the ID of the haptic effect to destroy
    void HapticDestroyEffect(SDL_Haptic * haptic, int effect);

    /// @brief Check to see if an effect is supported by a haptic device. https://wiki.libsdl.org/SDL2/SDL_HapticEffectSupported
    /// @param haptic the SDL_Haptic device to query
    /// @param effect the desired effect to query
    /// @return Returns SDL_TRUE if effect is supported, SDL_FALSE if it isn't, or a negative error code on failure; call SDL_GetError() for more information.
    int HapticEffectSupported(SDL_Haptic * haptic, SDL_HapticEffect * effect);

    /// @brief Get the status of the current effect on the specified haptic device. https://wiki.libsdl.org/SDL2/SDL_HapticGetEffectStatus
    /// @param haptic the SDL_Haptic device to query for the effect status on
    /// @param effect the ID of the haptic effect to query its status
    /// @return Returns 0 if it isn't playing, 1 if it is playing, or a negative error code on failure; call SDL_GetError() for more information.
    int HapticGetEffectStatus(SDL_Haptic * haptic, int effect);

    /// @brief Get the index of a haptic device. https://wiki.libsdl.org/SDL2/SDL_HapticIndex
    /// @param haptic the SDL_Haptic device to query
    /// @return Returns the index of the specified haptic device or a negative error code on failure; call SDL_GetError() for more information.
    int HapticIndex(SDL_Haptic * haptic);

    /// @brief Get the implementation dependent name of a haptic device. https://wiki.libsdl.org/SDL2/SDL_HapticName
    /// @param device_index index of the device to query.
    /// @return Returns the name of the device or NULL on failure; call SDL_GetError() for more information.
    const char* HapticName(int device_index);

    /// @brief Create a new haptic effect on a specified device. https://wiki.libsdl.org/SDL2/SDL_HapticNewEffect
    /// @param haptic an SDL_Haptic device to create the effect on
    /// @param effect an SDL_HapticEffect structure containing the properties of the effect to create
    /// @return Returns the ID of the effect on success or a negative error code on failure; call SDL_GetError() for more information.
    int HapticNewEffect(SDL_Haptic * haptic, SDL_HapticEffect * effect);

    /// @brief Get the number of haptic axes the device has. https://wiki.libsdl.org/SDL2/SDL_HapticNumAxes
    /// @param haptic the SDL_Haptic device to query
    /// @return Returns the number of axes on success or a negative error code on failure; call SDL_GetError() for more information.
    int HapticNumAxes(SDL_Haptic * haptic);

    /// @brief Get the number of effects a haptic device can store. https://wiki.libsdl.org/SDL2/SDL_HapticNumEffects
    /// @param haptic the SDL_Haptic device to query
    /// @return Returns the number of effects the haptic device can store or a negative error code on failure; call SDL_GetError() for more information.
    int HapticNumEffects(SDL_Haptic * haptic);

    /// @brief Get the number of effects a haptic device can play at the same time. https://wiki.libsdl.org/SDL2/SDL_HapticNumEffectsPlaying
    /// @param haptic the SDL_Haptic device to query maximum playing effects
    /// @return Returns the number of effects the haptic device can play at the same time or a negative error code on failure; call SDL_GetError() for more information.
    int HapticNumEffectsPlaying(SDL_Haptic * haptic);

    /// @brief Open a haptic device for use. https://wiki.libsdl.org/SDL2/SDL_HapticOpen
    /// @param device_index index of the device to open
    /// @return Returns the device identifier or NULL on failure; call SDL_GetError() for more information.
    SDL_Haptic* HapticOpen(int device_index);

    /// @brief Check if the haptic device at the designated index has been opened. https://wiki.libsdl.org/SDL2/SDL_HapticOpened
    /// @param device_index the index of the device to query
    /// @return Returns 1 if it has been opened, 0 if it hasn't or on failure; call SDL_GetError() for more information.
    int HapticOpened(int device_index);

    /// @brief Open a haptic device for use from a joystick device. https://wiki.libsdl.org/SDL2/SDL_HapticOpenFromJoystick
    /// @param joystick the SDL_Joystick to create a haptic device from
    /// @return Returns a valid haptic device identifier on success or NULL on failure; call SDL_GetError() for more information.
    SDL_Haptic* HapticOpenFromJoystick(SDL_Joystick * joystick);

    /// @brief Try to open a haptic device from the current mouse. https://wiki.libsdl.org/SDL2/SDL_HapticOpenFromMouse
    /// @return Returns the haptic device identifier or NULL on failure; call SDL_GetError() for more information.
    SDL_Haptic* HapticOpenFromMouse();

    /// @brief Pause a haptic device. https://wiki.libsdl.org/SDL2/SDL_HapticPause
    /// @param haptic the SDL_Haptic device to pause
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int HapticPause(SDL_Haptic * haptic);

    /// @brief Get the haptic device's supported features in bitwise manner. https://wiki.libsdl.org/SDL2/SDL_HapticQuery
    /// @param haptic the SDL_Haptic device to query
    /// @return Returns a list of supported haptic features in bitwise manner (OR'd), or 0 on failure; call SDL_GetError() for more information.
    unsigned int HapticQuery(SDL_Haptic * haptic);

    /// @brief Initialize a haptic device for simple rumble playback. https://wiki.libsdl.org/SDL2/SDL_HapticRumbleInit
    /// @param haptic the haptic device to initialize for simple rumble playback
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int HapticRumbleInit(SDL_Haptic * haptic);

    /// @brief Run a simple rumble effect on a haptic device. https://wiki.libsdl.org/SDL2/SDL_HapticRumblePlay
    /// @param haptic the haptic device to play the rumble effect on
    /// @param strength strength of the rumble to play as a 0-1 float value
    /// @param length length of the rumble to play in milliseconds
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int HapticRumblePlay(SDL_Haptic * haptic, float strength, Uint32 length );

    /// @brief Stop the simple rumble on a haptic device. https://wiki.libsdl.org/SDL2/SDL_HapticRumbleStop
    /// @param haptic the haptic device to stop the rumble effect on
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int HapticRumbleStop(SDL_Haptic * haptic);

    /// @brief Check whether rumble is supported on a haptic device. https://wiki.libsdl.org/SDL2/SDL_HapticRumbleSupported
    /// @param haptic haptic device to check for rumble support
    /// @return Returns SDL_TRUE if effect is supported, SDL_FALSE if it isn't, or a negative error code on failure; call SDL_GetError() for more information.
    int HapticRumbleSupported(SDL_Haptic * haptic);

    /// @brief Run the haptic effect on its associated haptic device. https://wiki.libsdl.org/SDL2/SDL_HapticRunEffect
    /// @param haptic the SDL_Haptic device to run the effect on
    /// @param effect the ID of the haptic effect to run
    /// @param iterations the number of iterations to run the effect; use SDL_HAPTIC_INFINITY to repeat forever
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int HapticRunEffect(SDL_Haptic * haptic, int effect, Uint32 iterations);

    /// @brief Set the global autocenter of the device. https://wiki.libsdl.org/SDL2/SDL_HapticSetAutocenter
    /// @param haptic the SDL_Haptic device to set autocentering on
    /// @param autocenter value to set autocenter to (0-100)
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int HapticSetAutocenter(SDL_Haptic * haptic, int autocenter);

    /// @brief Set the global gain of the specified haptic device. https://wiki.libsdl.org/SDL2/SDL_HapticSetGain
    /// @param haptic the SDL_Haptic device to set the gain on
    /// @param gain value to set the gain to, should be between 0 and 100 (0 - 100)
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int HapticSetGain(SDL_Haptic * haptic, int gain);

    /// @brief Stop all the currently playing effects on a haptic device. https://wiki.libsdl.org/SDL2/SDL_HapticStopAll
    /// @param haptic the SDL_Haptic device to stop
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int HapticStopAll(SDL_Haptic * haptic);

    /// @brief Stop the haptic effect on its associated haptic device. https://wiki.libsdl.org/SDL2/SDL_HapticStopEffect
    /// @param haptic the SDL_Haptic device to stop the effect on
    /// @param effect the ID of the haptic effect to stop
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int HapticStopEffect(SDL_Haptic * haptic, int effect);

    /// @brief Unpause a haptic device. https://wiki.libsdl.org/SDL2/SDL_HapticUnpause
    /// @param haptic the SDL_Haptic device to unpause
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int HapticUnpause(SDL_Haptic * haptic);

    /// @brief Update the properties of an effect. https://wiki.libsdl.org/SDL2/SDL_HapticUpdateEffect
    /// @param haptic the SDL_Haptic device that has the effect
    /// @param effect the identifier of the effect to update
    /// @param data an SDL_HapticEffect structure containing the new effect properties to use
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int HapticUpdateEffect(SDL_Haptic * haptic, int effect, SDL_HapticEffect * data);

    /// @brief Determine whether the CPU has 3DNow! features. https://wiki.libsdl.org/SDL2/SDL_Has3DNow
    /// @return Returns SDL_TRUE if the CPU has 3DNow! features or SDL_FALSE if not.
    bool Has3DNow();

    /// @brief Determine whether the CPU has AltiVec features. https://wiki.libsdl.org/SDL2/SDL_HasAltiVec
    /// @return Returns SDL_TRUE if the CPU has AltiVec features or SDL_FALSE if not.
    bool HasAltiVec();

    /// @brief Determine whether the CPU has ARM SIMD (ARMv6) features. https://wiki.libsdl.org/SDL2/SDL_HasARMSIMD
    /// @return Returns SDL_TRUE if the CPU has ARM SIMD features or SDL_FALSE if not.
    bool HasARMSIMD();

    /// @brief Determine whether the CPU has AVX features. https://wiki.libsdl.org/SDL2/SDL_HasAVX
    /// @return Returns SDL_TRUE if the CPU has AVX features or SDL_FALSE if not.
    bool HasAVX();

    /// @brief Determine whether the CPU has AVX2 features. https://wiki.libsdl.org/SDL2/SDL_HasAVX2
    /// @return Returns SDL_TRUE if the CPU has AVX2 features or SDL_FALSE if not.
    bool HasAVX2();

    /// @brief Determine whether the CPU has AVX-512F (foundation) features. https://wiki.libsdl.org/SDL2/SDL_HasAVX512F
    /// @return Returns SDL_TRUE if the CPU has AVX-512F features or SDL_FALSE if not.
    bool HasAVX512F();

    /// @brief Query whether the clipboard exists and contains a non-empty text string. https://wiki.libsdl.org/SDL2/SDL_HasClipboardText
    /// @return Returns SDL_TRUE if the clipboard has text, or SDL_FALSE if it does not.
    bool HasClipboardText();

    /// @brief Returns whether the surface has a color key https://wiki.libsdl.org/SDL2/SDL_HasColorKey
    /// @param surface the SDL_Surface structure to query
    /// @return Return SDL_TRUE if the surface has a color key, SDL_FALSE otherwise.
    bool HasColorKey(SDL_Surface * surface);

    /// @brief Check for the existence of a certain event type in the event queue. https://wiki.libsdl.org/SDL2/SDL_HasEvent
    /// @param type the type of event to be queried; see SDL_EventType for details
    /// @return Returns SDL_TRUE if events matching type are present, or SDL_FALSE if events matching type are not present.
    bool HasEvent(Uint32 type);

    /// @brief Check for the existence of certain event types in the event queue. https://wiki.libsdl.org/SDL2/SDL_HasEvents
    /// @param minType the low end of event type to be queried, inclusive; see SDL_EventType for details
    /// @param maxType the high end of event type to be queried, inclusive; see SDL_EventType for details
    /// @return Returns SDL_TRUE if events with type >= minType and <= maxType are present, or SDL_FALSE if not.
    bool HasEvents(Uint32 minType, Uint32 maxType);

    /// @brief Determine whether two rectangles intersect. https://wiki.libsdl.org/SDL2/SDL_HasIntersection
    /// @param A an SDL_Rect structure representing the first rectangle
    /// @param B an SDL_Rect structure representing the second rectangle
    /// @return Returns SDL_TRUE if there is an intersection, SDL_FALSE otherwise.
    bool HasIntersection(const SDL_Rect * A, const SDL_Rect * B);

    /// @brief Determine whether two rectangles intersect with float precision. https://wiki.libsdl.org/SDL2/SDL_HasIntersectionF
    /// @param A an SDL_FRect structure representing the first rectangle
    /// @param B an SDL_FRect structure representing the second rectangle
    /// @return Returns SDL_TRUE if there is an intersection, SDL_FALSE otherwise.
    bool HasIntersectionF(const SDL_FRect * A, const SDL_FRect * B);

    /// @brief Determine whether the CPU has LASX (LOONGARCH SIMD) features. https://wiki.libsdl.org/SDL2/SDL_HasLASX
    /// @return Returns SDL_TRUE if the CPU has LOONGARCH LASX features or SDL_FALSE if not.
    bool HasLASX();

    /// @brief Determine whether the CPU has LSX (LOONGARCH SIMD) features. https://wiki.libsdl.org/SDL2/SDL_HasLSX
    /// @return Returns SDL_TRUE if the CPU has LOONGARCH LSX features or SDL_FALSE if not.
    bool HasLSX();

    /// @brief Determine whether the CPU has MMX features. https://wiki.libsdl.org/SDL2/SDL_HasMMX
    /// @return Returns SDL_TRUE if the CPU has MMX features or SDL_FALSE if not.
    bool HasMMX();

    /// @brief Determine whether the CPU has NEON (ARM SIMD) features. https://wiki.libsdl.org/SDL2/SDL_HasNEON
    /// @return Returns SDL_TRUE if the CPU has ARM NEON features or SDL_FALSE if not.
    bool HasNEON();

    /// @brief Query whether the primary selection exists and contains a non-empty text string. https://wiki.libsdl.org/SDL2/SDL_HasPrimarySelectionText
    /// @return Returns SDL_TRUE if the primary selection has text, or SDL_FALSE if it does not.
    bool HasPrimarySelectionText();

    /// @brief Determine whether the CPU has the RDTSC instruction. https://wiki.libsdl.org/SDL2/SDL_HasRDTSC
    /// @return Returns SDL_TRUE if the CPU has the RDTSC instruction or SDL_FALSE if not.
    bool HasRDTSC();

    /// @brief Check whether the platform has screen keyboard support. https://wiki.libsdl.org/SDL2/SDL_HasScreenKeyboardSupport
    /// @return Returns SDL_TRUE if the platform has some screen keyboard support or SDL_FALSE if not.
    bool HasScreenKeyboardSupport();

    /// @brief Determine whether the CPU has SSE features. https://wiki.libsdl.org/SDL2/SDL_HasSSE
    /// @return Returns SDL_TRUE if the CPU has SSE features or SDL_FALSE if not.
    bool HasSSE();

    /// @brief Determine whether the CPU has SSE2 features. https://wiki.libsdl.org/SDL2/SDL_HasSSE2
    /// @return Returns SDL_TRUE if the CPU has SSE2 features or SDL_FALSE if not.
    bool HasSSE2();

    /// @brief Determine whether the CPU has SSE3 features. https://wiki.libsdl.org/SDL2/SDL_HasSSE3
    /// @return Returns SDL_TRUE if the CPU has SSE3 features or SDL_FALSE if not.
    bool HasSSE3();

    /// @brief Determine whether the CPU has SSE4.1 features. https://wiki.libsdl.org/SDL2/SDL_HasSSE41
    /// @return Returns SDL_TRUE if the CPU has SSE4.1 features or SDL_FALSE if not.
    bool HasSSE41();

    /// @brief Determine whether the CPU has SSE4.2 features. https://wiki.libsdl.org/SDL2/SDL_HasSSE42
    /// @return Returns SDL_TRUE if the CPU has SSE4.2 features or SDL_FALSE if not.
    bool HasSSE42();

    /// @brief Returns whether the surface is RLE enabled https://wiki.libsdl.org/SDL2/SDL_HasSurfaceRLE
    /// @param surface the SDL_Surface structure to query
    /// @return Returns SDL_TRUE if the surface is RLE enabled, SDL_FALSE otherwise.
    bool HasSurfaceRLE(SDL_Surface * surface);

    /// @brief Return whether the window has a surface associated with it. https://wiki.libsdl.org/SDL2/SDL_HasWindowSurface
    /// @return Returns SDL_TRUE if there is a surface associated with the window, or SDL_FALSE otherwise.
    bool HasWindowSurface(SDL_Window *window);

    /// @brief Start or stop a BLE scan on iOS and tvOS to pair Steam Controllers https://wiki.libsdl.org/SDL2/SDL_hid_ble_scan
    /// @param active SDL_TRUE to start the scan, SDL_FALSE to stop the scan
    void hid_ble_scan(bool active);

    /// @brief Close a HID device. https://wiki.libsdl.org/SDL2/SDL_hid_close
    /// @param dev A device handle returned from SDL_hid_open().
    void hid_close(SDL_hid_device *dev);

    /// @brief Check to see if devices may have been added or removed. https://wiki.libsdl.org/SDL2/SDL_hid_device_change_count
    /// @return Returns a change counter that is incremented with each potential device change, or 0 if device change detection isn't available.
    Uint32 hid_device_change_count();

    /// @brief Enumerate the HID Devices. https://wiki.libsdl.org/SDL2/SDL_hid_enumerate
    /// @param vendor_id The Vendor ID (VID) of the types of device to open.
    /// @param product_id The Product ID (PID) of the types of device to open.
    /// @return Returns a pointer to a linked list of type SDL_hid_device_info, containing information about the HID devices attached to the system, or NULL in the case of failure. Free this linked list by calling SDL_hid_free_enumeration().
    SDL_hid_device_info * hid_enumerate(unsigned short vendor_id, unsigned short product_id);

    /// @brief Finalize the HIDAPI library. https://wiki.libsdl.org/SDL2/SDL_hid_exit
    /// @return Returns 0 on success and -1 on error.
    int hid_exit();

    /// @brief Free an enumeration Linked List https://wiki.libsdl.org/SDL2/SDL_hid_free_enumeration
    /// @param devs Pointer to a list of struct_device returned from SDL_hid_enumerate().
    void hid_free_enumeration(SDL_hid_device_info *devs);

    /// @brief Get a feature report from a HID device. https://wiki.libsdl.org/SDL2/SDL_hid_get_feature_report
    /// @param dev A device handle returned from SDL_hid_open().
    /// @param data A buffer to put the read data into, including the Report ID. Set the first byte of data to the Report ID of the report to be read, or set it to zero if your device does not use numbered reports.
    /// @param length The number of bytes to read, including an extra byte for the report ID. The buffer can be longer than the actual report.
    /// @return Returns the number of bytes read plus one for the report ID (which is still in the first byte), or -1 on error.
    int hid_get_feature_report(SDL_hid_device *dev, unsigned char *data, size_t length);

    /// @brief Get a string from a HID device, based on its string index. https://wiki.libsdl.org/SDL2/SDL_hid_get_indexed_string
    /// @param dev A device handle returned from SDL_hid_open().
    /// @param string_index The index of the string to get.
    /// @param string A wide string buffer to put the data into.
    /// @param maxlen The length of the buffer in multiples of wchar_t.
    /// @return Returns 0 on success and -1 on error.
    int hid_get_indexed_string(SDL_hid_device *dev, int string_index, wchar_t *string, size_t maxlen);

    /// @brief Get The Manufacturer String from a HID device. https://wiki.libsdl.org/SDL2/SDL_hid_get_manufacturer_string
    /// @param dev A device handle returned from SDL_hid_open().
    /// @param string A wide string buffer to put the data into.
    /// @param maxlen The length of the buffer in multiples of wchar_t.
    /// @return Returns 0 on success and -1 on error.
    int hid_get_manufacturer_string(SDL_hid_device *dev, wchar_t *string, size_t maxlen);

    /// @brief Get The Product String from a HID device. https://wiki.libsdl.org/SDL2/SDL_hid_get_product_string
    /// @param dev A device handle returned from SDL_hid_open().
    /// @param string A wide string buffer to put the data into.
    /// @param maxlen The length of the buffer in multiples of wchar_t.
    /// @return Returns 0 on success and -1 on error.
    int hid_get_product_string(SDL_hid_device *dev, wchar_t *string, size_t maxlen);

    /// @brief Get The Serial Number String from a HID device. https://wiki.libsdl.org/SDL2/SDL_hid_get_serial_number_string
    /// @param dev A device handle returned from SDL_hid_open().
    /// @param string A wide string buffer to put the data into.
    /// @param maxlen The length of the buffer in multiples of wchar_t.
    /// @return Returns 0 on success and -1 on error.
    int hid_get_serial_number_string(SDL_hid_device *dev, wchar_t *string, size_t maxlen);

    /// @brief Initialize the HIDAPI library. https://wiki.libsdl.org/SDL2/SDL_hid_init
    /// @return Returns 0 on success and -1 on error.
    int hid_init();

    /// @brief Open a HID device using a Vendor ID (VID), Product ID (PID) and optionally a serial number. https://wiki.libsdl.org/SDL2/SDL_hid_open
    /// @param vendor_id The Vendor ID (VID) of the device to open.
    /// @param product_id The Product ID (PID) of the device to open.
    /// @param serial_number The Serial Number of the device to open (Optionally NULL).
    /// @return Returns a pointer to a SDL_hid_device object on success or NULL on failure.
    SDL_hid_device * hid_open(unsigned short vendor_id, unsigned short product_id, const wchar_t *serial_number);

    /// @brief Open a HID device by its path name. https://wiki.libsdl.org/SDL2/SDL_hid_open_path
    /// @param path The path name of the device to open
    /// @return Returns a pointer to a SDL_hid_device object on success or NULL on failure.
    SDL_hid_device * hid_open_path(const char *path, int bExclusive /* = false */);

    /// @brief Read an Input report from a HID device. https://wiki.libsdl.org/SDL2/SDL_hid_read
    /// @param dev A device handle returned from SDL_hid_open().
    /// @param data A buffer to put the read data into.
    /// @param length The number of bytes to read. For devices with multiple reports, make sure to read an extra byte for the report number.
    /// @return Returns the actual number of bytes read and -1 on error. If no packet was available to be read and the handle is in non-blocking mode, this function returns 0.
    int hid_read(SDL_hid_device *dev, unsigned char *data, size_t length);

    /// @brief Read an Input report from a HID device with timeout. https://wiki.libsdl.org/SDL2/SDL_hid_read_timeout
    /// @param dev A device handle returned from SDL_hid_open().
    /// @param data A buffer to put the read data into.
    /// @param length The number of bytes to read. For devices with multiple reports, make sure to read an extra byte for the report number.
    /// @param milliseconds timeout in milliseconds or -1 for blocking wait.
    /// @return Returns the actual number of bytes read and -1 on error. If no packet was available to be read within the timeout period, this function returns 0.
    int hid_read_timeout(SDL_hid_device *dev, unsigned char *data, size_t length, int milliseconds);

    /// @brief Send a Feature report to the device. https://wiki.libsdl.org/SDL2/SDL_hid_send_feature_report
    /// @param dev A device handle returned from SDL_hid_open().
    /// @param data The data to send, including the report number as the first byte.
    /// @param length The length in bytes of the data to send, including the report number.
    /// @return Returns the actual number of bytes written and -1 on error.
    int hid_send_feature_report(SDL_hid_device *dev, const unsigned char *data, size_t length);

    /// @brief Set the device handle to be non-blocking. https://wiki.libsdl.org/SDL2/SDL_hid_set_nonblocking
    /// @param dev A device handle returned from SDL_hid_open().
    /// @param nonblock enable or not the nonblocking reads - 1 to enable nonblocking - 0 to disable nonblocking.
    /// @return Returns 0 on success and -1 on error.
    int hid_set_nonblocking(SDL_hid_device *dev, int nonblock);

    /// @brief Write an Output report to a HID device. https://wiki.libsdl.org/SDL2/SDL_hid_write
    /// @param dev A device handle returned from SDL_hid_open().
    /// @param data The data to send, including the report number as the first byte.
    /// @param length The length in bytes of the data to send.
    /// @return Returns the actual number of bytes written and -1 on error.
    int hid_write(SDL_hid_device *dev, const unsigned char *data, size_t length);

    /// @brief Hide a window. https://wiki.libsdl.org/SDL2/SDL_HideWindow
    /// @param window the window to hide
    void HideWindow(SDL_Window * window);

    /// @brief This function converts a buffer or string between encodings in one pass, returning a string that must be freed with SDL_free() or NULL on error. https://wiki.libsdl.org/SDL2/SDL_iconv_string
    char* iconv_string(const char *tocode, const char *fromcode, const char *inbuf, size_t inbytesleft);

    /// @brief Compatibility function to initialize the SDL library. https://wiki.libsdl.org/SDL2/SDL_InitSubSystem
    /// @param flags any of the flags used by SDL_Init(); see SDL_Init for details.
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int InitSubSystem(Uint32 flags);

    /// @brief Calculate the intersection of two rectangles with float precision. https://wiki.libsdl.org/SDL2/SDL_IntersectFRect
    /// @param A an SDL_FRect structure representing the first rectangle
    /// @param B an SDL_FRect structure representing the second rectangle
    /// @param result an SDL_FRect structure filled in with the intersection of rectangles A and B
    /// @return Returns SDL_TRUE if there is an intersection, SDL_FALSE otherwise.
    bool IntersectFRect(const SDL_FRect * A, const SDL_FRect * B, SDL_FRect * result);

    /// @brief Calculate the intersection of a rectangle and line segment with float precision. https://wiki.libsdl.org/SDL2/SDL_IntersectFRectAndLine
    /// @param rect an SDL_FRect structure representing the rectangle to intersect
    /// @param X1 a pointer to the starting X-coordinate of the line
    /// @param Y1 a pointer to the starting Y-coordinate of the line
    /// @param X2 a pointer to the ending X-coordinate of the line
    /// @param Y2 a pointer to the ending Y-coordinate of the line
    /// @return Returns SDL_TRUE if there is an intersection, SDL_FALSE otherwise.
    bool IntersectFRectAndLine(const SDL_FRect * rect, float *X1, float *Y1, float *X2, float *Y2);

    /// @brief Calculate the intersection of two rectangles. https://wiki.libsdl.org/SDL2/SDL_IntersectRect
    /// @param A an SDL_Rect structure representing the first rectangle
    /// @param B an SDL_Rect structure representing the second rectangle
    /// @param result an SDL_Rect structure filled in with the intersection of rectangles A and B
    /// @return Returns SDL_TRUE if there is an intersection, SDL_FALSE otherwise.
    bool IntersectRect(const SDL_Rect * A, const SDL_Rect * B, SDL_Rect * result);

    /// @brief Calculate the intersection of a rectangle and line segment. https://wiki.libsdl.org/SDL2/SDL_IntersectRectAndLine
    /// @param rect an SDL_Rect structure representing the rectangle to intersect
    /// @param X1 a pointer to the starting X-coordinate of the line
    /// @param Y1 a pointer to the starting Y-coordinate of the line
    /// @param X2 a pointer to the ending X-coordinate of the line
    /// @param Y2 a pointer to the ending Y-coordinate of the line
    /// @return Returns SDL_TRUE if there is an intersection, SDL_FALSE otherwise.
    bool IntersectRectAndLine(const SDL_Rect * rect, int *X1, int *Y1, int *X2, int *Y2);

    /// @brief Check if the given joystick is supported by the game controller interface. https://wiki.libsdl.org/SDL2/SDL_IsGameController
    /// @param joystick_index the device_index of a device, up to SDL_NumJoysticks()
    /// @return Returns SDL_TRUE if the given joystick is supported by the game controller interface, SDL_FALSE if it isn't or it's an invalid index.
    bool IsGameController(int joystick_index);

    /// @brief Check whether the screen keyboard is shown for given window. https://wiki.libsdl.org/SDL2/SDL_IsScreenKeyboardShown
    /// @param window the window for which screen keyboard should be queried
    /// @return Returns SDL_TRUE if screen keyboard is shown or SDL_FALSE if not.
    bool IsScreenKeyboardShown(SDL_Window *window);

    /// @brief Check whether the screensaver is currently enabled. https://wiki.libsdl.org/SDL2/SDL_IsScreenSaverEnabled
    /// @return Returns SDL_TRUE if the screensaver is enabled, SDL_FALSE if it is disabled.
    bool IsScreenSaverEnabled();

    /// @brief Return whether the given window is a shaped window. https://wiki.libsdl.org/SDL2/SDL_IsShapedWindow
    /// @param window The window to query for being shaped.
    /// @return Return SDL_TRUE if the window is a window that can be shaped, SDL_FALSE if the window is unshaped or NULL.
    bool IsShapedWindow(const SDL_Window *window);

    /// @brief Query if the current device is a tablet. https://wiki.libsdl.org/SDL2/SDL_IsTablet
    /// @return Returns SDL_TRUE if the device is a tablet, SDL_FALSE otherwise.
    bool IsTablet();

    /// @brief Check whether or not Unicode text input events are enabled. https://wiki.libsdl.org/SDL2/SDL_IsTextInputActive
    /// @return Returns SDL_TRUE if text input events are enabled else SDL_FALSE.
    bool IsTextInputActive();

    /// @brief Returns if an IME Composite or Candidate window is currently shown. https://wiki.libsdl.org/SDL2/SDL_IsTextInputShown
    bool IsTextInputShown();

    /// @brief Attach a new virtual joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickAttachVirtual
    /// @return Returns the joystick's device index, or -1 if an error occurred.
    int JoystickAttachVirtual(SDL_JoystickType type, int naxes, int nbuttons, int nhats);

    /// @brief Attach a new virtual joystick with extended properties. https://wiki.libsdl.org/SDL2/SDL_JoystickAttachVirtualEx
    /// @return Returns the joystick's device index, or -1 if an error occurred.
    int JoystickAttachVirtualEx(const SDL_VirtualJoystickDesc *desc);

    /// @brief Close a joystick previously opened with SDL_JoystickOpen(). https://wiki.libsdl.org/SDL2/SDL_JoystickClose
    /// @param joystick The joystick device to close
    void JoystickClose(SDL_Joystick *joystick);

    /// @brief Get the battery level of a joystick as SDL_JoystickPowerLevel. https://wiki.libsdl.org/SDL2/SDL_JoystickCurrentPowerLevel
    /// @param joystick the SDL_Joystick to query
    /// @return Returns the current battery level as SDL_JoystickPowerLevel on success or SDL_JOYSTICK_POWER_UNKNOWN if it is unknown
    SDL_JoystickPowerLevel JoystickCurrentPowerLevel(SDL_Joystick *joystick);

    /// @brief Detach a virtual joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickDetachVirtual
    /// @param device_index a value previously returned from SDL_JoystickAttachVirtual()
    /// @return Returns 0 on success, or -1 if an error occurred.
    int JoystickDetachVirtual(int device_index);

    /// @brief Enable/disable joystick event polling. https://wiki.libsdl.org/SDL2/SDL_JoystickEventState
    /// @param state can be one of SDL_QUERY, SDL_IGNORE, or SDL_ENABLE
    /// @return Returns 1 if enabled, 0 if disabled, or a negative error code on failure; call SDL_GetError() for more information.
    int JoystickEventState(int state);

    /// @brief Get the SDL_Joystick associated with an instance id. https://wiki.libsdl.org/SDL2/SDL_JoystickFromInstanceID
    /// @param instance_id the instance id to get the SDL_Joystick for
    /// @return Returns an SDL_Joystick on success or NULL on failure; call SDL_GetError() for more information.
    SDL_Joystick* JoystickFromInstanceID(SDL_JoystickID instance_id);

    /// @brief Get the SDL_Joystick associated with a player index. https://wiki.libsdl.org/SDL2/SDL_JoystickFromPlayerIndex
    /// @param player_index the player index to get the SDL_Joystick for
    /// @return Returns an SDL_Joystick on success or NULL on failure; call SDL_GetError() for more information.
    SDL_Joystick* JoystickFromPlayerIndex(int player_index);

    /// @brief Get the status of a specified joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickGetAttached
    /// @param joystick the joystick to query
    /// @return Returns SDL_TRUE if the joystick has been opened, SDL_FALSE if it has not; call SDL_GetError() for more information.
    bool JoystickGetAttached(SDL_Joystick *joystick);

    /// @brief Get the current state of an axis control on a joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickGetAxis
    /// @param joystick an SDL_Joystick structure containing joystick information
    /// @param axis the axis to query; the axis indices start at index 0
    /// @return Returns a 16-bit signed integer representing the current position of the axis or 0 on failure; call SDL_GetError() for more information.
    Sint16 JoystickGetAxis(SDL_Joystick *joystick, int axis);

    /// @brief Get the initial state of an axis control on a joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickGetAxisInitialState
    /// @param joystick an SDL_Joystick structure containing joystick information
    /// @param axis the axis to query; the axis indices start at index 0
    /// @param state Upon return, the initial value is supplied here.
    /// @return Return SDL_TRUE if this axis has any initial value, or SDL_FALSE if not.
    bool JoystickGetAxisInitialState(SDL_Joystick *joystick, int axis, Sint16 *state);

    /// @brief Get the ball axis change since the last poll. https://wiki.libsdl.org/SDL2/SDL_JoystickGetBall
    /// @param joystick the SDL_Joystick to query
    /// @param ball the ball index to query; ball indices start at index 0
    /// @param dx stores the difference in the x axis position since the last poll
    /// @param dy stores the difference in the y axis position since the last poll
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int JoystickGetBall(SDL_Joystick *joystick, int ball, int *dx, int *dy);

    /// @brief Get the current state of a button on a joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickGetButton
    /// @param joystick an SDL_Joystick structure containing joystick information
    /// @param button the button index to get the state from; indices start at index 0
    /// @return Returns 1 if the specified button is pressed, 0 otherwise.
    Uint8 JoystickGetButton(SDL_Joystick *joystick, int button);

    /// @brief Get the implementation-dependent GUID for the joystick at a given device index. https://wiki.libsdl.org/SDL2/SDL_JoystickGetDeviceGUID
    /// @param device_index the index of the joystick to query (the N'th joystick on the system
    /// @return Returns the GUID of the selected joystick. If called on an invalid index, this function returns a zero GUID
    SDL_JoystickGUID JoystickGetDeviceGUID(int device_index);

    /// @brief Get the instance ID of a joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickGetDeviceInstanceID
    /// @param device_index the index of the joystick to query (the N'th joystick on the system
    /// @return Returns the instance id of the selected joystick. If called on an invalid index, this function returns -1.
    SDL_JoystickID JoystickGetDeviceInstanceID(int device_index);

    /// @brief Get the player index of a joystick, or -1 if it's not available This can be called before any joysticks are opened. https://wiki.libsdl.org/SDL2/SDL_JoystickGetDevicePlayerIndex
    int JoystickGetDevicePlayerIndex(int device_index);

    /// @brief Get the USB product ID of a joystick, if available. https://wiki.libsdl.org/SDL2/SDL_JoystickGetDeviceProduct
    /// @param device_index the index of the joystick to query (the N'th joystick on the system
    /// @return Returns the USB product ID of the selected joystick. If called on an invalid index, this function returns zero
    uint16_t JoystickGetDeviceProduct(int device_index);

    /// @brief Get the product version of a joystick, if available. https://wiki.libsdl.org/SDL2/SDL_JoystickGetDeviceProductVersion
    /// @param device_index the index of the joystick to query (the N'th joystick on the system
    /// @return Returns the product version of the selected joystick. If called on an invalid index, this function returns zero
    uint16_t JoystickGetDeviceProductVersion(int device_index);

    /// @brief Get the type of a joystick, if available. https://wiki.libsdl.org/SDL2/SDL_JoystickGetDeviceType
    /// @param device_index the index of the joystick to query (the N'th joystick on the system
    /// @return Returns the SDL_JoystickType of the selected joystick. If called on an invalid index, this function returns SDL_JOYSTICK_TYPE_UNKNOWN
    SDL_JoystickType JoystickGetDeviceType(int device_index);

    /// @brief Get the USB vendor ID of a joystick, if available. https://wiki.libsdl.org/SDL2/SDL_JoystickGetDeviceVendor
    /// @param device_index the index of the joystick to query (the N'th joystick on the system
    /// @return Returns the USB vendor ID of the selected joystick. If called on an invalid index, this function returns zero
    uint16_t JoystickGetDeviceVendor(int device_index);

    /// @brief Get the firmware version of an opened joystick, if available. https://wiki.libsdl.org/SDL2/SDL_JoystickGetFirmwareVersion
    /// @param joystick the SDL_Joystick obtained from SDL_JoystickOpen()
    /// @return Returns the firmware version of the selected joystick, or 0 if unavailable.
    uint16_t JoystickGetFirmwareVersion(SDL_Joystick *joystick);

    /// @brief Get the implementation-dependent GUID for the joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickGetGUID
    /// @param joystick the SDL_Joystick obtained from SDL_JoystickOpen()
    /// @return Returns the GUID of the given joystick. If called on an invalid index, this function returns a zero GUID; call SDL_GetError() for more information.
    SDL_JoystickGUID JoystickGetGUID(SDL_Joystick *joystick);

    /// @brief Convert a GUID string into a SDL_JoystickGUID structure. https://wiki.libsdl.org/SDL2/SDL_JoystickGetGUIDFromString
    /// @param pchGUID string containing an ASCII representation of a GUID
    /// @return Returns a SDL_JoystickGUID structure.
    SDL_JoystickGUID JoystickGetGUIDFromString(const char *pchGUID);

    /// @brief Get an ASCII string representation for a given SDL_JoystickGUID. https://wiki.libsdl.org/SDL2/SDL_JoystickGetGUIDString
    /// @param guid the SDL_JoystickGUID you wish to convert to string
    /// @param pszGUID buffer in which to write the ASCII string
    /// @param cbGUID the size of pszGUID
    void JoystickGetGUIDString(SDL_JoystickGUID guid, char *pszGUID, int cbGUID);

    /// @brief Get the current state of a POV hat on a joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickGetHat
    /// @param joystick an SDL_Joystick structure containing joystick information
    /// @param hat the hat index to get the state from; indices start at index 0
    /// @return Returns the current hat position.
    Uint8 JoystickGetHat(SDL_Joystick *joystick, int hat);

    /// @brief Get the player index of an opened joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickGetPlayerIndex
    /// @param joystick the SDL_Joystick obtained from SDL_JoystickOpen()
    /// @return Returns the player index, or -1 if it's not available.
    int JoystickGetPlayerIndex(SDL_Joystick *joystick);

    /// @brief Get the USB product ID of an opened joystick, if available. https://wiki.libsdl.org/SDL2/SDL_JoystickGetProduct
    /// @param joystick the SDL_Joystick obtained from SDL_JoystickOpen()
    /// @return Returns the USB product ID of the selected joystick, or 0 if unavailable.
    uint16_t JoystickGetProduct(SDL_Joystick *joystick);

    /// @brief Get the product version of an opened joystick, if available. https://wiki.libsdl.org/SDL2/SDL_JoystickGetProductVersion
    /// @param joystick the SDL_Joystick obtained from SDL_JoystickOpen()
    /// @return Returns the product version of the selected joystick, or 0 if unavailable.
    uint16_t JoystickGetProductVersion(SDL_Joystick *joystick);

    /// @brief Get the serial number of an opened joystick, if available. https://wiki.libsdl.org/SDL2/SDL_JoystickGetSerial
    /// @param joystick the SDL_Joystick obtained from SDL_JoystickOpen()
    /// @return Returns the serial number of the selected joystick, or NULL if unavailable.
    const char * JoystickGetSerial(SDL_Joystick *joystick);

    /// @brief Get the type of an opened joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickGetType
    /// @param joystick the SDL_Joystick obtained from SDL_JoystickOpen()
    /// @return Returns the SDL_JoystickType of the selected joystick.
    SDL_JoystickType JoystickGetType(SDL_Joystick *joystick);

    /// @brief Get the USB vendor ID of an opened joystick, if available. https://wiki.libsdl.org/SDL2/SDL_JoystickGetVendor
    /// @param joystick the SDL_Joystick obtained from SDL_JoystickOpen()
    /// @return Returns the USB vendor ID of the selected joystick, or 0 if unavailable.
    uint16_t JoystickGetVendor(SDL_Joystick *joystick);

    /// @brief Query whether a joystick has an LED. https://wiki.libsdl.org/SDL2/SDL_JoystickHasLED
    /// @param joystick The joystick to query
    /// @return Return SDL_TRUE if the joystick has a modifiable LED, SDL_FALSE otherwise.
    bool JoystickHasLED(SDL_Joystick *joystick);

    /// @brief Query whether a joystick has rumble support. https://wiki.libsdl.org/SDL2/SDL_JoystickHasRumble
    /// @param joystick The joystick to query
    /// @return Return SDL_TRUE if the joystick has rumble, SDL_FALSE otherwise.
    bool JoystickHasRumble(SDL_Joystick *joystick);

    /// @brief Query whether a joystick has rumble support on triggers. https://wiki.libsdl.org/SDL2/SDL_JoystickHasRumbleTriggers
    /// @param joystick The joystick to query
    /// @return Return SDL_TRUE if the joystick has trigger rumble, SDL_FALSE otherwise.
    bool JoystickHasRumbleTriggers(SDL_Joystick *joystick);

    /// @brief Get the instance ID of an opened joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickInstanceID
    /// @param joystick an SDL_Joystick structure containing joystick information
    /// @return Returns the instance ID of the specified joystick on success or a negative error code on failure; call SDL_GetError() for more information.
    SDL_JoystickID JoystickInstanceID(SDL_Joystick *joystick);

    /// @brief Query if a joystick has haptic features. https://wiki.libsdl.org/SDL2/SDL_JoystickIsHaptic
    /// @param joystick the SDL_Joystick to test for haptic capabilities
    /// @return Returns SDL_TRUE if the joystick is haptic, SDL_FALSE if it isn't, or a negative error code on failure; call SDL_GetError() for more information.
    int JoystickIsHaptic(SDL_Joystick * joystick);

    /// @brief Query whether or not the joystick at a given device index is virtual. https://wiki.libsdl.org/SDL2/SDL_JoystickIsVirtual
    /// @param device_index a joystick device index.
    /// @return Returns SDL_TRUE if the joystick is virtual, SDL_FALSE otherwise.
    bool JoystickIsVirtual(int device_index);

    /// @brief Get the implementation dependent name of a joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickName
    /// @param joystick the SDL_Joystick obtained from SDL_JoystickOpen()
    /// @return Returns the name of the selected joystick. If no name can be found, this function returns NULL; call SDL_GetError() for more information.
    const char* JoystickName(SDL_Joystick *joystick);

    /// @brief Get the implementation dependent name of a joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickNameForIndex
    /// @param device_index the index of the joystick to query (the N'th joystick on the system)
    /// @return Returns the name of the selected joystick. If no name can be found, this function returns NULL; call SDL_GetError() for more information.
    const char* JoystickNameForIndex(int device_index);

    /// @brief Get the number of general axis controls on a joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickNumAxes
    /// @param joystick an SDL_Joystick structure containing joystick information
    /// @return Returns the number of axis controls/number of axes on success or a negative error code on failure; call SDL_GetError() for more information.
    int JoystickNumAxes(SDL_Joystick *joystick);

    /// @brief Get the number of trackballs on a joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickNumBalls
    /// @param joystick an SDL_Joystick structure containing joystick information
    /// @return Returns the number of trackballs on success or a negative error code on failure; call SDL_GetError() for more information.
    int JoystickNumBalls(SDL_Joystick *joystick);

    /// @brief Get the number of buttons on a joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickNumButtons
    /// @param joystick an SDL_Joystick structure containing joystick information
    /// @return Returns the number of buttons on success or a negative error code on failure; call SDL_GetError() for more information.
    int JoystickNumButtons(SDL_Joystick *joystick);

    /// @brief Get the number of POV hats on a joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickNumHats
    /// @param joystick an SDL_Joystick structure containing joystick information
    /// @return Returns the number of POV hats on success or a negative error code on failure; call SDL_GetError() for more information.
    int JoystickNumHats(SDL_Joystick *joystick);

    /// @brief Open a joystick for use. https://wiki.libsdl.org/SDL2/SDL_JoystickOpen
    /// @param device_index the index of the joystick to query
    /// @return Returns a joystick identifier or NULL if an error occurred; call SDL_GetError() for more information.
    SDL_Joystick* JoystickOpen(int device_index);

    /// @brief Get the implementation dependent path of a joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickPath
    /// @param joystick the SDL_Joystick obtained from SDL_JoystickOpen()
    /// @return Returns the path of the selected joystick. If no path can be found, this function returns NULL; call SDL_GetError() for more information.
    const char* JoystickPath(SDL_Joystick *joystick);

    /// @brief Get the implementation dependent path of a joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickPathForIndex
    /// @param device_index the index of the joystick to query (the N'th joystick on the system)
    /// @return Returns the path of the selected joystick. If no path can be found, this function returns NULL; call SDL_GetError() for more information.
    const char* JoystickPathForIndex(int device_index);

    /// @brief Start a rumble effect. https://wiki.libsdl.org/SDL2/SDL_JoystickRumble
    /// @param joystick The joystick to vibrate
    /// @param low_frequency_rumble The intensity of the low frequency (left) rumble motor, from 0 to 0xFFFF
    /// @param high_frequency_rumble The intensity of the high frequency (right) rumble motor, from 0 to 0xFFFF
    /// @param duration_ms The duration of the rumble effect, in milliseconds
    /// @return Returns 0, or -1 if rumble isn't supported on this joystick
    int JoystickRumble(SDL_Joystick *joystick, uint16_t low_frequency_rumble, uint16_t high_frequency_rumble, Uint32 duration_ms);

    /// @brief Start a rumble effect in the joystick's triggers https://wiki.libsdl.org/SDL2/SDL_JoystickRumbleTriggers
    /// @param joystick The joystick to vibrate
    /// @param left_rumble The intensity of the left trigger rumble motor, from 0 to 0xFFFF
    /// @param right_rumble The intensity of the right trigger rumble motor, from 0 to 0xFFFF
    /// @param duration_ms The duration of the rumble effect, in milliseconds
    /// @return Returns 0, or -1 if trigger rumble isn't supported on this joystick
    int JoystickRumbleTriggers(SDL_Joystick *joystick, uint16_t left_rumble, uint16_t right_rumble, Uint32 duration_ms);

    /// @brief Send a joystick specific effect packet https://wiki.libsdl.org/SDL2/SDL_JoystickSendEffect
    /// @param joystick The joystick to affect
    /// @param data The data to send to the joystick
    /// @param size The size of the data to send to the joystick
    /// @return Returns 0, or -1 if this joystick or driver doesn't support effect packets
    int JoystickSendEffect(SDL_Joystick *joystick, void *data, int size);

    /// @brief Update a joystick's LED color. https://wiki.libsdl.org/SDL2/SDL_JoystickSetLED
    /// @param joystick The joystick to update
    /// @param red The intensity of the red LED
    /// @param green The intensity of the green LED
    /// @param blue The intensity of the blue LED
    /// @return Returns 0 on success, -1 if this joystick does not have a modifiable LED
    int JoystickSetLED(SDL_Joystick *joystick, Uint8 red, Uint8 green, Uint8 blue);

    /// @brief Set the player index of an opened joystick. https://wiki.libsdl.org/SDL2/SDL_JoystickSetPlayerIndex
    /// @param joystick the SDL_Joystick obtained from SDL_JoystickOpen()
    /// @param player_index Player index to assign to this joystick, or -1 to clear the player index and turn off player LEDs.
    void JoystickSetPlayerIndex(SDL_Joystick *joystick, int player_index);

    /// @brief Set values on an opened, virtual-joystick's axis. https://wiki.libsdl.org/SDL2/SDL_JoystickSetVirtualAxis
    /// @param joystick the virtual joystick on which to set state.
    /// @param axis the specific axis on the virtual joystick to set.
    /// @param value the new value for the specified axis.
    /// @return Returns 0 on success, -1 on error.
    int JoystickSetVirtualAxis(SDL_Joystick *joystick, int axis, Sint16 value);

    /// @brief Set values on an opened, virtual-joystick's button. https://wiki.libsdl.org/SDL2/SDL_JoystickSetVirtualButton
    /// @param joystick the virtual joystick on which to set state.
    /// @param button the specific button on the virtual joystick to set.
    /// @param value the new value for the specified button.
    /// @return Returns 0 on success, -1 on error.
    int JoystickSetVirtualButton(SDL_Joystick *joystick, int button, Uint8 value);

    /// @brief Set values on an opened, virtual-joystick's hat. https://wiki.libsdl.org/SDL2/SDL_JoystickSetVirtualHat
    /// @param joystick the virtual joystick on which to set state.
    /// @param hat the specific hat on the virtual joystick to set.
    /// @param value the new value for the specified hat.
    /// @return Returns 0 on success, -1 on error.
    int JoystickSetVirtualHat(SDL_Joystick *joystick, int hat, Uint8 value);

    /// @brief Update the current state of the open joysticks. https://wiki.libsdl.org/SDL2/SDL_JoystickUpdate
    void JoystickUpdate();

    /// @brief Sets the UNIX nice value for a thread. https://wiki.libsdl.org/SDL2/SDL_LinuxSetThreadPriority
    /// @param threadID the Unix thread ID to change priority of.
    /// @param priority The new, Unix-specific, priority value.
    /// @return Returns 0 on success, or -1 on error.
    int LinuxSetThreadPriority(Sint64 threadID, int priority);

    /// @brief Sets the priority (not nice level) and scheduling policy for a thread. https://wiki.libsdl.org/SDL2/SDL_LinuxSetThreadPriorityAndPolicy
    /// @param threadID The Unix thread ID to change priority of.
    /// @param sdlPriority The new SDL_ThreadPriority value.
    /// @param schedPolicy The new scheduling policy (SCHED_FIFO, SCHED_RR, SCHED_OTHER, etc...)
    /// @return Returns 0 on success, or -1 on error.
    int LinuxSetThreadPriorityAndPolicy(Sint64 threadID, int sdlPriority, int schedPolicy);

    /// @brief Load a BMP image from a seekable SDL data stream. https://wiki.libsdl.org/SDL2/SDL_LoadBMP_RW
    /// @param src the data stream for the surface
    /// @param freesrc non-zero to close the stream after being read
    /// @return Returns a pointer to a new SDL_Surface structure or NULL if there was an error; call SDL_GetError() for more information.
    SDL_Surface* LoadBMP_RW(SDL_RWops * src, int freesrc);

    /// @brief Load a BMP image from a file path. https://wiki.libsdl.org/SDL2/SDL_LoadBMP
    /// @param file the file containing a BMP image
    /// @return Returns a pointer to a new SDL_Surface structure or NULL if there was an error; call SDL_GetError() for more information.
    SDL_Surface* LoadBMP(const char* file);

    /// @brief Load Dollar Gesture templates from a file. https://wiki.libsdl.org/SDL2/SDL_LoadDollarTemplates
    /// @param touchId a touch id
    /// @param src a SDL_RWops to load from
    /// @return Returns the number of loaded templates on success or a negative error code (or 0) on failure; call SDL_GetError() for more information.
    int LoadDollarTemplates(SDL_TouchID touchId, SDL_RWops *src);

    /// @brief Load all the data from a file path. https://wiki.libsdl.org/SDL2/SDL_LoadFile
    /// @param file the path to read all available data from
    /// @param datasize if not NULL, will store the number of bytes read
    /// @return Returns the data, or NULL if there was an error.
    void* LoadFile(const char *file, size_t *datasize);

    /// @brief Load all the data from an SDL data stream. https://wiki.libsdl.org/SDL2/SDL_LoadFile_RW
    /// @param src the SDL_RWops to read all available data from
    /// @param datasize if not NULL, will store the number of bytes read
    /// @param freesrc if non-zero, calls SDL_RWclose() on src before returning
    /// @return Returns the data, or NULL if there was an error.
    void* LoadFile_RW(SDL_RWops *src, size_t *datasize, int freesrc);

    /// @brief Look up the address of the named function in a shared object. https://wiki.libsdl.org/SDL2/SDL_LoadFunction
    /// @param handle a valid shared object handle returned by SDL_LoadObject()
    /// @param name the name of the function to look up
    /// @return Returns a pointer to the function or NULL if there was an error; call SDL_GetError() for more information.
    void* LoadFunction(void *handle, const char *name);

    /// @brief Dynamically load a shared object. https://wiki.libsdl.org/SDL2/SDL_LoadObject
    /// @param sofile a system-dependent name of the object file
    /// @return Returns an opaque pointer to the object handle or NULL if there was an error; call SDL_GetError() for more information.
    void* LoadObject(const char *sofile);

    /// @brief Load the audio data of a WAVE file into memory. https://wiki.libsdl.org/SDL2/SDL_LoadWAV_RW
    /// @param src The data source for the WAVE data
    /// @param freesrc If non-zero, SDL will always free the data source
    /// @param spec An SDL_AudioSpec that will be filled in with the wave file's format details
    /// @param audio_buf A pointer filled with the audio data, allocated by the function.
    /// @param audio_len A pointer filled with the length of the audio data buffer in bytes
    /// @return Returns This function, if successfully called, returns spec, which will be filled with the audio data format of the wave source data. audio_buf will be filled with a pointer to an allocated buffer containing the audio data, and audio_len is filled with the length of that audio buffer in bytes.
    SDL_AudioSpec* LoadWAV_RW(SDL_RWops * src, int freesrc, SDL_AudioSpec * spec, Uint8 ** audio_buf, Uint32 * audio_len);

    /// @brief Use this function to load a WAVE from a file. https://wiki.libsdl.org/SDL2/SDL_LoadWAV
    /// @param file the name of the file to load
    /// @param spec an SDL_AudioSpec structure that will be filled in with the wave file's format details
    /// @param audio_buf the audio buffer
    /// @param audio_len the length of the audio buffer in bytes
    /// @return <<Include(SDL_LoadWAV_RW, , , from="== Return Value ==", to="== Code Examples")>>
    SDL_AudioSpec* LoadWAV(const char* file, SDL_AudioSpec* spec, Uint8** audio_buf, Uint32* audio_len);

    /// @brief This function is a legacy means of locking the audio device. https://wiki.libsdl.org/SDL2/SDL_LockAudio
    void LockAudio();

    /// @brief Use this function to lock out the audio callback function for a specified device. https://wiki.libsdl.org/SDL2/SDL_LockAudioDevice
    /// @param dev the ID of the device to be locked
    void LockAudioDevice(SDL_AudioDeviceID dev);

    /// @brief Locking for multi-threaded access to the joystick API https://wiki.libsdl.org/SDL2/SDL_LockJoysticks
    void LockJoysticks();

    /// @brief Lock the mutex. https://wiki.libsdl.org/SDL2/SDL_LockMutex
    /// @param mutex the mutex to lock
    /// @return Return 0, or -1 on error.
    int LockMutex(SDL_mutex * mutex);

    /// @brief Locking for multi-threaded access to the sensor API https://wiki.libsdl.org/SDL2/SDL_LockSensors
    void LockSensors();

    /// @brief Set up a surface for directly accessing the pixels. https://wiki.libsdl.org/SDL2/SDL_LockSurface
    /// @param surface the SDL_Surface structure to be locked
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int LockSurface(SDL_Surface * surface);

    /// @brief Lock a portion of the texture for write-only pixel access. https://wiki.libsdl.org/SDL2/SDL_LockTexture
    /// @param texture the texture to lock for access, which was created with SDL_TEXTUREACCESS_STREAMING
    /// @param rect an SDL_Rect structure representing the area to lock for access; NULL to lock the entire texture
    /// @param pixels this is filled in with a pointer to the locked pixels, appropriately offset by the locked area
    /// @param pitch this is filled in with the pitch of the locked pixels; the pitch is the length of one row in bytes
    /// @return Returns 0 on success or a negative error code if the texture is not valid or was not created with SDL_TEXTUREACCESS_STREAMING; call SDL_GetError() for more information.
    int LockTexture(SDL_Texture * texture, const SDL_Rect * rect, void **pixels, int *pitch);

    /// @brief Lock a portion of the texture for write-only pixel access, and expose it as a SDL surface. https://wiki.libsdl.org/SDL2/SDL_LockTextureToSurface
    /// @param texture the texture to lock for access, which was created with SDL_TEXTUREACCESS_STREAMING
    /// @param rect a pointer to the rectangle to lock for access. If the rect is NULL, the entire texture will be locked
    /// @param surface this is filled in with an SDL surface representing the locked area
    /// @return Returns 0 on success, or -1 if the texture is not valid or was not created with SDL_TEXTUREACCESS_STREAMING
    int LockTextureToSurface(SDL_Texture *texture, const SDL_Rect *rect, SDL_Surface **surface);

    /// @brief Get the current log output function. https://wiki.libsdl.org/SDL2/SDL_LogGetOutputFunction
    /// @param callback an SDL_LogOutputFunction filled in with the current log callback
    /// @param userdata a pointer filled in with the pointer that is passed to callback
    void LogGetOutputFunction(SDL_LogOutputFunction *callback, void **userdata);

    /// @brief Get the priority of a particular log category. https://wiki.libsdl.org/SDL2/SDL_LogGetPriority
    /// @param category the category to query
    /// @return Returns the SDL_LogPriority for the requested category
    SDL_LogPriority LogGetPriority(int category);

    /// @brief Reset all priorities to default. https://wiki.libsdl.org/SDL2/SDL_LogResetPriorities
    void LogResetPriorities();

    /// @brief Set the priority of all log categories. https://wiki.libsdl.org/SDL2/SDL_LogSetAllPriority
    /// @param priority the SDL_LogPriority to assign
    void LogSetAllPriority(SDL_LogPriority priority);

    /// @brief Replace the default log output function with one of your own. https://wiki.libsdl.org/SDL2/SDL_LogSetOutputFunction
    /// @param callback an SDL_LogOutputFunction to call instead of the default
    /// @param userdata a pointer that is passed to callback
    void LogSetOutputFunction(SDL_LogOutputFunction callback, void *userdata);

    /// @brief Set the priority of a particular log category. https://wiki.libsdl.org/SDL2/SDL_LogSetPriority
    /// @param category the category to assign a priority to
    /// @param priority the SDL_LogPriority to assign
    void LogSetPriority(int category, SDL_LogPriority priority);

    /// @brief Perform low-level surface blitting only. https://wiki.libsdl.org/SDL2/SDL_LowerBlit
    /// @param src the SDL_Surface structure to be copied from
    /// @param srcrect the SDL_Rect structure representing the rectangle to be copied, or NULL to copy the entire surface
    /// @param dst the SDL_Surface structure that is the blit target
    /// @param dstrect the SDL_Rect structure representing the rectangle that is copied into
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int LowerBlit(SDL_Surface * src, SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect);

    /// @brief Perform low-level surface scaled blitting only. https://wiki.libsdl.org/SDL2/SDL_LowerBlitScaled
    /// @param src the SDL_Surface structure to be copied from
    /// @param srcrect the SDL_Rect structure representing the rectangle to be copied
    /// @param dst the SDL_Surface structure that is the blit target
    /// @param dstrect the SDL_Rect structure representing the rectangle that is copied into
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int LowerBlitScaled(SDL_Surface * src, SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect);

    /// @brief Map an RGB triple to an opaque pixel value for a given pixel format. https://wiki.libsdl.org/SDL2/SDL_MapRGB
    /// @param format an SDL_PixelFormat structure describing the pixel format
    /// @param r the red component of the pixel in the range 0-255
    /// @param g the green component of the pixel in the range 0-255
    /// @param b the blue component of the pixel in the range 0-255
    /// @return Returns a pixel value
    Uint32 MapRGB(const SDL_PixelFormat * format, Uint8 r, Uint8 g, Uint8 b);

    /// @brief Map an RGBA quadruple to a pixel value for a given pixel format. https://wiki.libsdl.org/SDL2/SDL_MapRGBA
    /// @param format an SDL_PixelFormat structure describing the format of the pixel
    /// @param r the red component of the pixel in the range 0-255
    /// @param g the green component of the pixel in the range 0-255
    /// @param b the blue component of the pixel in the range 0-255
    /// @param a the alpha component of the pixel in the range 0-255
    /// @return Returns a pixel value
    Uint32 MapRGBA(const SDL_PixelFormat * format, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /// @brief Convert a bpp value and RGBA masks to an enumerated pixel format. https://wiki.libsdl.org/SDL2/SDL_MasksToPixelFormatEnum
    /// @param bpp a bits per pixel value; usually 15, 16, or 32
    /// @param Rmask the red mask for the format
    /// @param Gmask the green mask for the format
    /// @param Bmask the blue mask for the format
    /// @param Amask the alpha mask for the format
    /// @return Returns one of the SDL_PixelFormatEnum values
    Uint32 MasksToPixelFormatEnum(int bpp, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);

    /// @brief Make a window as large as possible. https://wiki.libsdl.org/SDL2/SDL_MaximizeWindow
    /// @param window the window to maximize
    void MaximizeWindow(SDL_Window * window);

    /// @brief Memory barriers are designed to prevent reads and writes from being reordered by the compiler and being seen out of order on multi-core CPUs. https://wiki.libsdl.org/SDL2/SDL_MemoryBarrierReleaseFunction
    void MemoryBarrierReleaseFunction();

    /// @brief Create a CAMetalLayer-backed NSView/UIView and attach it to the specified window. https://wiki.libsdl.org/SDL2/SDL_Metal_CreateView
    SDL_MetalView Metal_CreateView(SDL_Window * window);

    /// @brief Destroy an existing SDL_MetalView object. https://wiki.libsdl.org/SDL2/SDL_Metal_DestroyView
    void Metal_DestroyView(SDL_MetalView view);

    /// @brief Get the size of a window's underlying drawable in pixels (for use with setting viewport, scissor & etc). https://wiki.libsdl.org/SDL2/SDL_Metal_GetDrawableSize
    /// @param window SDL_Window from which the drawable size should be queried
    /// @param w Pointer to variable for storing the width in pixels, may be NULL
    /// @param h Pointer to variable for storing the height in pixels, may be NULL
    void Metal_GetDrawableSize(SDL_Window* window, int *w, int *h);

    /// @brief Get a pointer to the backing CAMetalLayer for the given view. https://wiki.libsdl.org/SDL2/SDL_Metal_GetLayer
    void* Metal_GetLayer(SDL_MetalView view);

    /// @brief Minimize a window to an iconic representation. https://wiki.libsdl.org/SDL2/SDL_MinimizeWindow
    /// @param window the window to minimize
    void MinimizeWindow(SDL_Window * window);

    /// @brief This function is a legacy means of mixing audio. https://wiki.libsdl.org/SDL2/SDL_MixAudio
    /// @param dst the destination for the mixed audio
    /// @param src the source audio buffer to be mixed
    /// @param len the length of the audio buffer in bytes
    /// @param volume ranges from 0 - 128, and should be set to SDL_MIX_MAXVOLUME for full audio volume
    void MixAudio(Uint8 * dst, const Uint8 * src, Uint32 len, int volume);

    /// @brief Mix audio data in a specified format. https://wiki.libsdl.org/SDL2/SDL_MixAudioFormat
    /// @param dst the destination for the mixed audio
    /// @param src the source audio buffer to be mixed
    /// @param format the SDL_AudioFormat structure representing the desired audio format
    /// @param len the length of the audio buffer in bytes
    /// @param volume ranges from 0 - 128, and should be set to SDL_MIX_MAXVOLUME for full audio volume
    void MixAudioFormat(Uint8 * dst, const Uint8 * src, SDL_AudioFormat format, Uint32 len, int volume);

    /// @brief Use this function to get the index of the most significant (set) bit in a 32-bit number. This is also the same as the base 2 logarithm of the number. https://wiki.libsdl.org/SDL2/SDL_MostSignificantBitIndex32
    /// @param x the number to find the MSB of
    /// @return Returns the index of the most significant bit of x, or -1 if x is 0.
    int MostSignificantBitIndex32(uint32_t x);

    /// @brief Query whether or not the current mouse has haptic capabilities. https://wiki.libsdl.org/SDL2/SDL_MouseIsHaptic
    /// @return Returns SDL_TRUE if the mouse is haptic or SDL_FALSE if it isn't.
    int MouseIsHaptic();

    /// @brief Use this function to determine whether a surface must be locked for access. https://wiki.libsdl.org/SDL2/SDL_MUSTLOCK
    /// @param surface the SDL_Surface structure to query
    /// @return Returns SDL_TRUE if the surface must be locked for access, SDL_FALSE if not.
    bool MUSTLOCK(SDL_Surface* surface);

    /// @brief Create a new audio stream. https://wiki.libsdl.org/SDL2/SDL_NewAudioStream
    /// @param src_format The format of the source audio
    /// @param src_channels The number of channels of the source audio
    /// @param src_rate The sampling rate of the source audio
    /// @param dst_format The format of the desired audio output
    /// @param dst_channels The number of channels of the desired audio output
    /// @param dst_rate The sampling rate of the desired audio output
    /// @return Returns 0 on success, or -1 on error.
    SDL_AudioStream * NewAudioStream(const SDL_AudioFormat src_format, const Uint8 src_channels, const int src_rate, const SDL_AudioFormat dst_format, const Uint8 dst_channels, const int dst_rate);

    /// @brief Count the number of haptic devices attached to the system. https://wiki.libsdl.org/SDL2/SDL_NumHaptics
    /// @return Returns the number of haptic devices detected on the system or a negative error code on failure; call SDL_GetError() for more information.
    int NumHaptics();

    /// @brief Count the number of joysticks attached to the system. https://wiki.libsdl.org/SDL2/SDL_NumJoysticks
    /// @return Returns the number of attached joysticks on success or a negative error code on failure; call SDL_GetError() for more information.
    int NumJoysticks();

    /// @brief Count the number of sensors attached to the system right now. https://wiki.libsdl.org/SDL2/SDL_NumSensors
    /// @return Returns the number of sensors detected.
    int NumSensors();

    /// @brief This function is a legacy means of opening the audio device. https://wiki.libsdl.org/SDL2/SDL_OpenAudio
    /// @param desired an SDL_AudioSpec structure representing the desired output format. Please refer to the SDL_OpenAudioDevice documentation for details on how to prepare this structure.
    /// @param obtained an SDL_AudioSpec structure filled in with the actual parameters, or NULL.
    /// @return Returns 0 if successful, placing the actual hardware parameters in the structure pointed to by obtained.
    int OpenAudio(SDL_AudioSpec * desired, SDL_AudioSpec * obtained);

    /// @brief Open a specific audio device. https://wiki.libsdl.org/SDL2/SDL_OpenAudioDevice
    /// @param device a UTF-8 string reported by SDL_GetAudioDeviceName() or a driver-specific name as appropriate. NULL requests the most reasonable default device.
    /// @param iscapture non-zero to specify a device should be opened for recording, not playback
    /// @param desired an SDL_AudioSpec structure representing the desired output format; see SDL_OpenAudio() for more information
    /// @param obtained an SDL_AudioSpec structure filled in with the actual output format; see SDL_OpenAudio() for more information
    /// @param allowed_changes 0, or one or more flags OR'd together
    /// @return Returns a valid device ID that is > 0 on success or 0 on failure; call SDL_GetError() for more information.
    SDL_AudioDeviceID OpenAudioDevice( const char *device, int iscapture, const SDL_AudioSpec *desired, SDL_AudioSpec *obtained, int allowed_changes);

    /// @brief Open a URL/URI in the browser or other appropriate external application. https://wiki.libsdl.org/SDL2/SDL_OpenURL
    /// @param url A valid URL/URI to open. Use file:///full/path/to/file for local files, if supported.
    /// @return Returns 0 on success, or -1 on error; call SDL_GetError() for more information.
    int OpenURL(const char *url);

    /// @brief This function is a legacy means of pausing the audio device. https://wiki.libsdl.org/SDL2/SDL_PauseAudio
    /// @param pause_on non-zero to pause, 0 to unpause
    void PauseAudio(int pause_on);

    /// @brief Use this function to pause and unpause audio playback on a specified device. https://wiki.libsdl.org/SDL2/SDL_PauseAudioDevice
    /// @param dev a device opened by SDL_OpenAudioDevice()
    /// @param pause_on non-zero to pause, 0 to unpause
    void PauseAudioDevice(SDL_AudioDeviceID dev, int pause_on);

    /// @brief Check the event queue for messages and optionally return them. https://wiki.libsdl.org/SDL2/SDL_PeepEvents
    /// @param events destination buffer for the retrieved events
    /// @param numevents if action is SDL_ADDEVENT, the number of events to add back to the event queue; if action is SDL_PEEKEVENT or SDL_GETEVENT, the maximum number of events to retrieve
    /// @param action action to take; see [[#action
    /// @param minType minimum value of the event type to be considered; SDL_FIRSTEVENT is a safe choice
    /// @param maxType maximum value of the event type to be considered; SDL_LASTEVENT is a safe choice
    /// @return Returns the number of events actually stored or a negative error code on failure; call SDL_GetError() for more information.
    int PeepEvents(SDL_Event * events, int numevents, SDL_eventaction action, Uint32 minType, Uint32 maxType);

    /// @brief Convert one of the enumerated pixel formats to a bpp value and RGBA masks. https://wiki.libsdl.org/SDL2/SDL_PixelFormatEnumToMasks
    /// @param format one of the SDL_PixelFormatEnum values
    /// @param bpp a bits per pixel value; usually 15, 16, or 32
    /// @param Rmask a pointer filled in with the red mask for the format
    /// @param Gmask a pointer filled in with the green mask for the format
    /// @param Bmask a pointer filled in with the blue mask for the format
    /// @param Amask a pointer filled in with the alpha mask for the format
    /// @return Returns SDL_TRUE on success or SDL_FALSE if the conversion wasn't possible; call SDL_GetError() for more information.
    bool PixelFormatEnumToMasks(Uint32 format, int *bpp, Uint32 * Rmask, Uint32 * Gmask, Uint32 * Bmask, Uint32 * Amask);


    /// @brief Use this function to check if a point resides inside a rectangle. https://wiki.libsdl.org/SDL2/SDL_PointInFRect
    /// @param p an SDL_FPoint which is the point
    /// @param r an SDL_FRect which is the rectangle
    /// @return Returns SDL_TRUE if point resides inside rectangle or SDL_FALSE if not.
    bool PointInFRect(const SDL_FPoint* p,  const SDL_FRect* r);

    /// @brief Use this function to check if a point resides inside a rectangle. https://wiki.libsdl.org/SDL2/SDL_PointInRect
    /// @param p an SDL_Point which is the point
    /// @param r an SDL_Rect which is the rectangle
    /// @return Returns SDL_TRUE if point resides inside rectangle or SDL_FALSE if not.
    bool PointInRect(const SDL_Point* p,  const SDL_Rect* r);

    /// @brief Poll for currently pending events. https://wiki.libsdl.org/SDL2/SDL_PollEvent
    /// @param event the SDL_Event structure to be filled with the next event from the queue, or NULL
    /// @return Returns 1 if there is a pending event or 0 if there are none available.
    int PollEvent(SDL_Event * event);

    /// @brief Premultiply the alpha on a block of pixels. https://wiki.libsdl.org/SDL2/SDL_PremultiplyAlpha
    /// @param width the width of the block to convert, in pixels
    /// @param height the height of the block to convert, in pixels
    /// @param src_format an SDL_PixelFormatEnum value of the src pixels format
    /// @param src a pointer to the source pixels
    /// @param src_pitch the pitch of the source pixels, in bytes
    /// @param dst_format an SDL_PixelFormatEnum value of the dst pixels format
    /// @param dst a pointer to be filled in with premultiplied pixel data
    /// @param dst_pitch the pitch of the destination pixels, in bytes
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int PremultiplyAlpha(int width, int height, Uint32 src_format, void * src, int src_pitch, Uint32 dst_format, void * dst, int dst_pitch);

    /// @brief Pump the event loop, gathering events from the input devices. https://wiki.libsdl.org/SDL2/SDL_PumpEvents
    void PumpEvents();

    /// @brief Add an event to the event queue. https://wiki.libsdl.org/SDL2/SDL_PushEvent
    /// @param event the SDL_Event to be added to the queue
    /// @return Returns 1 on success, 0 if the event was filtered, or a negative error code on failure; call SDL_GetError() for more information. A common reason for error is the event queue being full.
    int PushEvent(SDL_Event * event);

    /// @brief Query the attributes of a texture. https://wiki.libsdl.org/SDL2/SDL_QueryTexture
    /// @param texture the texture to query
    /// @param format a pointer filled in with the raw format of the texture; the actual format may differ, but pixel transfers will use this format (one of the SDL_PixelFormatEnum values). This argument can be NULL if you don't need this information.
    /// @param access a pointer filled in with the actual access to the texture (one of the SDL_TextureAccess values). This argument can be NULL if you don't need this information.
    /// @param w a pointer filled in with the width of the texture in pixels. This argument can be NULL if you don't need this information.
    /// @param h a pointer filled in with the height of the texture in pixels. This argument can be NULL if you don't need this information.
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int QueryTexture(SDL_Texture * texture, Uint32 * format, int *access, int *w, int *h);

    /// @brief Queue more audio on non-callback devices. https://wiki.libsdl.org/SDL2/SDL_QueueAudio
    /// @param dev the device ID to which we will queue audio
    /// @param data the data to queue to the device for later playback
    /// @param len the number of bytes (not samples!) to which data points
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int QueueAudio(SDL_AudioDeviceID dev, void *data, Uint32 len);

    /// @brief Use this function to see whether an SDL_QUIT event is queued. https://wiki.libsdl.org/SDL2/SDL_QuitRequested
    /// @return Returns SDL_TRUE if SDL_QUIT is queued or SDL_FALSE otherwise.
    bool QuitRequested();

    /// @brief Shut down specific SDL subsystems. https://wiki.libsdl.org/SDL2/SDL_QuitSubSystem
    /// @param flags any of the flags used by SDL_Init(); see SDL_Init for details.
    void QuitSubSystem(Uint32 flags);

    /// @brief Raise a window above other windows and set the input focus. https://wiki.libsdl.org/SDL2/SDL_RaiseWindow
    /// @param window the window to raise
    void RaiseWindow(SDL_Window * window);

    /// @brief Use this function to read 16 bits of big-endian data from an SDL_RWops and return in native format. https://wiki.libsdl.org/SDL2/SDL_ReadBE16
    /// @param src the stream from which to read data
    /// @return Returns 16 bits of data in the native byte order of the platform.
    uint16_t ReadBE16(SDL_RWops * src);

    /// @brief Use this function to read 32 bits of big-endian data from an SDL_RWops and return in native format. https://wiki.libsdl.org/SDL2/SDL_ReadBE32
    /// @param src the stream from which to read data
    /// @return Returns 32 bits of data in the native byte order of the platform.
    Uint32 ReadBE32(SDL_RWops * src);

    /// @brief Use this function to read 64 bits of big-endian data from an SDL_RWops and return in native format. https://wiki.libsdl.org/SDL2/SDL_ReadBE64
    /// @param src the stream from which to read data
    /// @return Returns 64 bits of data in the native byte order of the platform.
    Uint64 ReadBE64(SDL_RWops * src);

    /// @brief Use this function to read 16 bits of little-endian data from an SDL_RWops and return in native format. https://wiki.libsdl.org/SDL2/SDL_ReadLE16
    /// @param src the stream from which to read data
    /// @return Returns 16 bits of data in the native byte order of the platform.
    uint16_t ReadLE16(SDL_RWops * src);

    /// @brief Use this function to read 32 bits of little-endian data from an SDL_RWops and return in native format. https://wiki.libsdl.org/SDL2/SDL_ReadLE32
    /// @param src the stream from which to read data
    /// @return Returns 32 bits of data in the native byte order of the platform.
    Uint32 ReadLE32(SDL_RWops * src);

    /// @brief Use this function to read 64 bits of little-endian data from an SDL_RWops and return in native format. https://wiki.libsdl.org/SDL2/SDL_ReadLE64
    /// @param src the stream from which to read data
    /// @return Returns 64 bits of data in the native byte order of the platform.
    Uint64 ReadLE64(SDL_RWops * src);

    /// @brief Use this function to read a byte from an SDL_RWops. https://wiki.libsdl.org/SDL2/SDL_ReadU8
    /// @param src the SDL_RWops to read from
    /// @return Returns the read byte on success or 0 on failure; call SDL_GetError() for more information.
    Uint8 ReadU8(SDL_RWops * src);

    /// @brief Begin recording a gesture on a specified touch device or all touch devices. https://wiki.libsdl.org/SDL2/SDL_RecordGesture
    /// @param touchId the touch device id, or -1 for all touch devices
    /// @return Returns 1 on success or 0 if the specified device could not be found.
    int RecordGesture(SDL_TouchID touchId);

    /// @brief Use this function to check whether a rectangle has no area. https://wiki.libsdl.org/SDL2/SDL_RectEmpty
    /// @param r the SDL_Rect structure representing the rectangle to check
    /// @return Returns SDL_TRUE if the rectangle has no area or SDL_FALSE if it does; see Remarks for details.
    bool RectEmpty(const SDL_Rect* r);

    /// @brief Use this function to check whether two rectangles are equal. https://wiki.libsdl.org/SDL2/SDL_RectEquals
    /// @param a an SDL_Rect structure representing the first rectangle
    /// @param b an SDL_Rect structure representing the second rectangle
    /// @return Returns SDL_TRUE if the two rectangles are exactly the same, SDL_FALSE otherwise; see Remarks for details.
    bool RectEquals(const SDL_Rect* a,  const SDL_Rect* b);

    /// @brief Allocate a set of user-defined events, and return the beginning event number for that set of events. https://wiki.libsdl.org/SDL2/SDL_RegisterEvents
    /// @param numevents the number of events to be allocated
    /// @return Returns the beginning event number, or (Uint32)-1 if there are not enough user-defined events left.
    Uint32 RegisterEvents(int numevents);

    /// @brief Remove a timer created with SDL_AddTimer(). https://wiki.libsdl.org/SDL2/SDL_RemoveTimer
    /// @param id the ID of the timer to remove
    /// @return Returns SDL_TRUE if the timer is removed or SDL_FALSE if the timer wasn't found.
    bool RemoveTimer(SDL_TimerID id);

    /// @brief Clear the current rendering target with the drawing color. https://wiki.libsdl.org/SDL2/SDL_RenderClear
    /// @param renderer the rendering context
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderClear(SDL_Renderer * renderer);

    /// @brief Copy a portion of the texture to the current rendering target. https://wiki.libsdl.org/SDL2/SDL_RenderCopy
    /// @param renderer the rendering context
    /// @param texture the source texture
    /// @param srcrect the source SDL_Rect structure or NULL for the entire texture
    /// @param dstrect the destination SDL_Rect structure or NULL for the entire rendering target; the texture will be stretched to fill the given rectangle
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderCopy(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect);

    /// @brief Copy a portion of the texture to the current rendering, with optional rotation and flipping. https://wiki.libsdl.org/SDL2/SDL_RenderCopyEx
    /// @param renderer the rendering context
    /// @param texture the source texture
    /// @param srcrect the source SDL_Rect structure or NULL for the entire texture
    /// @param dstrect the destination SDL_Rect structure or NULL for the entire rendering target
    /// @param angle an angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
    /// @param center a pointer to a point indicating the point around which dstrect will be rotated (if NULL, rotation will be done around dstrect.w / 2, dstrect.h / 2)
    /// @param flip a SDL_RendererFlip value stating which flipping actions should be performed on the texture
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderCopyEx(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect, const double angle, const SDL_Point *center, const SDL_RendererFlip flip);

    /// @brief Copy a portion of the source texture to the current rendering target, with rotation and flipping, at subpixel precision. https://wiki.libsdl.org/SDL2/SDL_RenderCopyExF
    /// @param renderer The renderer which should copy parts of a texture.
    /// @param texture The source texture.
    /// @param srcrect A pointer to the source rectangle, or NULL for the entire texture.
    /// @param dstrect A pointer to the destination rectangle, or NULL for the entire rendering target.
    /// @param angle An angle in degrees that indicates the rotation that will be applied to dstrect, rotating it in a clockwise direction
    /// @param center A pointer to a point indicating the point around which dstrect will be rotated (if NULL, rotation will be done around dstrect.w/2, dstrect.h/2).
    /// @param flip An SDL_RendererFlip value stating which flipping actions should be performed on the texture
    /// @return Return 0 on success, or -1 on error
    int RenderCopyExF(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_FRect * dstrect, const double angle, const SDL_FPoint *center, const SDL_RendererFlip flip);

    /// @brief Copy a portion of the texture to the current rendering target at subpixel precision. https://wiki.libsdl.org/SDL2/SDL_RenderCopyF
    /// @param renderer The renderer which should copy parts of a texture.
    /// @param texture The source texture.
    /// @param srcrect A pointer to the source rectangle, or NULL for the entire texture.
    /// @param dstrect A pointer to the destination rectangle, or NULL for the entire rendering target.
    /// @return Return 0 on success, or -1 on error
    int RenderCopyF(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_FRect * dstrect);

    /// @brief Draw a line on the current rendering target. https://wiki.libsdl.org/SDL2/SDL_RenderDrawLine
    /// @param renderer the rendering context
    /// @param x1 the x coordinate of the start point
    /// @param y1 the y coordinate of the start point
    /// @param x2 the x coordinate of the end point
    /// @param y2 the y coordinate of the end point
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderDrawLine(SDL_Renderer * renderer, int x1, int y1, int x2, int y2);

    /// @brief Draw a line on the current rendering target at subpixel precision. https://wiki.libsdl.org/SDL2/SDL_RenderDrawLineF
    /// @param renderer The renderer which should draw a line.
    /// @param x1 The x coordinate of the start point.
    /// @param y1 The y coordinate of the start point.
    /// @param x2 The x coordinate of the end point.
    /// @param y2 The y coordinate of the end point.
    /// @return Return 0 on success, or -1 on error
    int RenderDrawLineF(SDL_Renderer * renderer, float x1, float y1, float x2, float y2);

    /// @brief Draw a series of connected lines on the current rendering target. https://wiki.libsdl.org/SDL2/SDL_RenderDrawLines
    /// @param renderer the rendering context
    /// @param points an array of SDL_Point structures representing points along the lines
    /// @param count the number of points, drawing count-1 lines
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderDrawLines(SDL_Renderer * renderer, const SDL_Point * points, int count);

    /// @brief Draw a series of connected lines on the current rendering target at subpixel precision. https://wiki.libsdl.org/SDL2/SDL_RenderDrawLinesF
    /// @param renderer The renderer which should draw multiple lines.
    /// @param points The points along the lines
    /// @param count The number of points, drawing count-1 lines
    /// @return Return 0 on success, or -1 on error
    int RenderDrawLinesF(SDL_Renderer * renderer, const SDL_FPoint * points, int count);

    /// @brief Draw a point on the current rendering target. https://wiki.libsdl.org/SDL2/SDL_RenderDrawPoint
    /// @param renderer the rendering context
    /// @param x the x coordinate of the point
    /// @param y the y coordinate of the point
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderDrawPoint(SDL_Renderer * renderer, int x, int y);

    /// @brief Draw a point on the current rendering target at subpixel precision. https://wiki.libsdl.org/SDL2/SDL_RenderDrawPointF
    /// @param renderer The renderer which should draw a point.
    /// @param x The x coordinate of the point.
    /// @param y The y coordinate of the point.
    /// @return Return 0 on success, or -1 on error
    int RenderDrawPointF(SDL_Renderer * renderer, float x, float y);

    /// @brief Draw multiple points on the current rendering target. https://wiki.libsdl.org/SDL2/SDL_RenderDrawPoints
    /// @param renderer the rendering context
    /// @param points an array of SDL_Point structures that represent the points to draw
    /// @param count the number of points to draw
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderDrawPoints(SDL_Renderer * renderer, const SDL_Point * points, int count);

    /// @brief Draw multiple points on the current rendering target at subpixel precision. https://wiki.libsdl.org/SDL2/SDL_RenderDrawPointsF
    /// @param renderer The renderer which should draw multiple points.
    /// @param points The points to draw
    /// @param count The number of points to draw
    /// @return Return 0 on success, or -1 on error
    int RenderDrawPointsF(SDL_Renderer * renderer, const SDL_FPoint * points, int count);

    /// @brief Draw a rectangle on the current rendering target. https://wiki.libsdl.org/SDL2/SDL_RenderDrawRect
    /// @param renderer the rendering context
    /// @param rect an SDL_Rect structure representing the rectangle to draw, or NULL to outline the entire rendering target
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderDrawRect(SDL_Renderer * renderer, const SDL_Rect * rect);

    /// @brief Draw a rectangle on the current rendering target at subpixel precision. https://wiki.libsdl.org/SDL2/SDL_RenderDrawRectF
    /// @param renderer The renderer which should draw a rectangle.
    /// @param rect A pointer to the destination rectangle, or NULL to outline the entire rendering target.
    /// @return Return 0 on success, or -1 on error
    int RenderDrawRectF(SDL_Renderer * renderer, const SDL_FRect * rect);

    /// @brief Draw some number of rectangles on the current rendering target. https://wiki.libsdl.org/SDL2/SDL_RenderDrawRects
    /// @param renderer the rendering context
    /// @param rects an array of SDL_Rect structures representing the rectangles to be drawn
    /// @param count the number of rectangles
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderDrawRects(SDL_Renderer * renderer, const SDL_Rect * rects, int count);

    /// @brief Draw some number of rectangles on the current rendering target at subpixel precision. https://wiki.libsdl.org/SDL2/SDL_RenderDrawRectsF
    /// @param renderer The renderer which should draw multiple rectangles.
    /// @param rects A pointer to an array of destination rectangles.
    /// @param count The number of rectangles.
    /// @return Return 0 on success, or -1 on error
    int RenderDrawRectsF(SDL_Renderer * renderer, const SDL_FRect * rects, int count);

    /// @brief Fill a rectangle on the current rendering target with the drawing color. https://wiki.libsdl.org/SDL2/SDL_RenderFillRect
    /// @param renderer the rendering context
    /// @param rect the SDL_Rect structure representing the rectangle to fill, or NULL for the entire rendering target
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderFillRect(SDL_Renderer * renderer, const SDL_Rect * rect);

    /// @brief Fill a rectangle on the current rendering target with the drawing color at subpixel precision. https://wiki.libsdl.org/SDL2/SDL_RenderFillRectF
    /// @param renderer The renderer which should fill a rectangle.
    /// @param rect A pointer to the destination rectangle, or NULL for the entire rendering target.
    /// @return Return 0 on success, or -1 on error
    int RenderFillRectF(SDL_Renderer * renderer, const SDL_FRect * rect);

    /// @brief Fill some number of rectangles on the current rendering target with the drawing color. https://wiki.libsdl.org/SDL2/SDL_RenderFillRects
    /// @param renderer the rendering context
    /// @param rects an array of SDL_Rect structures representing the rectangles to be filled
    /// @param count the number of rectangles
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderFillRects(SDL_Renderer * renderer, const SDL_Rect * rects, int count);

    /// @brief Fill some number of rectangles on the current rendering target with the drawing color at subpixel precision. https://wiki.libsdl.org/SDL2/SDL_RenderFillRectsF
    /// @param renderer The renderer which should fill multiple rectangles.
    /// @param rects A pointer to an array of destination rectangles.
    /// @param count The number of rectangles.
    /// @return Return 0 on success, or -1 on error
    int RenderFillRectsF(SDL_Renderer * renderer, const SDL_FRect * rects, int count);

    /// @brief Force the rendering context to flush any pending commands to the underlying rendering API. https://wiki.libsdl.org/SDL2/SDL_RenderFlush
    /// @param renderer the rendering context
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderFlush(SDL_Renderer * renderer);

    /// @brief Render a list of triangles, optionally using a texture and indices into the vertex array Color and alpha modulation is done per vertex (SDL_SetTextureColorMod and SDL_SetTextureAlphaMod are ignored). https://wiki.libsdl.org/SDL2/SDL_RenderGeometry
    /// @param renderer The rendering context.
    /// @param texture (optional) The SDL texture to use.
    /// @param vertices Vertices.
    /// @param num_vertices Number of vertices.
    /// @param indices (optional) An array of integer indices into the 'vertices' array, if NULL all vertices will be rendered in sequential order.
    /// @param num_indices Number of indices.
    /// @return Return 0 on success, or -1 if the operation is not supported
    int RenderGeometry(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Vertex *vertices, int num_vertices, const int *indices, int num_indices);

    /// @brief Render a list of triangles, optionally using a texture and indices into the vertex arrays Color and alpha modulation is done per vertex (SDL_SetTextureColorMod and SDL_SetTextureAlphaMod are ignored). https://wiki.libsdl.org/SDL2/SDL_RenderGeometryRaw
    /// @param renderer The rendering context.
    /// @param texture (optional) The SDL texture to use.
    /// @param xy Vertex positions
    /// @param xy_stride Byte size to move from one element to the next element
    /// @param color Vertex colors (as SDL_Color)
    /// @param color_stride Byte size to move from one element to the next element
    /// @param uv Vertex normalized texture coordinates
    /// @param uv_stride Byte size to move from one element to the next element
    /// @param num_vertices Number of vertices.
    /// @param indices (optional) An array of indices into the 'vertices' arrays, if NULL all vertices will be rendered in sequential order.
    /// @param num_indices Number of indices.
    /// @param size_indices Index size: 1 (byte), 2 (short), 4 (int)
    /// @return Return 0 on success, or -1 if the operation is not supported
    int RenderGeometryRaw(SDL_Renderer *renderer, SDL_Texture *texture, const float *xy, int xy_stride, const SDL_Color *color, int color_stride, const float *uv, int uv_stride, int num_vertices, void *indices, int num_indices, int size_indices);

    /// @brief Get the clip rectangle for the current target. https://wiki.libsdl.org/SDL2/SDL_RenderGetClipRect
    /// @param renderer the rendering context from which clip rectangle should be queried
    /// @param rect an SDL_Rect structure filled in with the current clipping area or an empty rectangle if clipping is disabled
    void RenderGetClipRect(SDL_Renderer * renderer, SDL_Rect * rect);

    /// @brief Get whether integer scales are forced for resolution-independent rendering. https://wiki.libsdl.org/SDL2/SDL_RenderGetIntegerScale
    /// @param renderer the renderer from which integer scaling should be queried
    /// @return Returns SDL_TRUE if integer scales are forced or SDL_FALSE if not and on failure; call SDL_GetError() for more information.
    bool RenderGetIntegerScale(SDL_Renderer * renderer);

    /// @brief Get device independent resolution for rendering. https://wiki.libsdl.org/SDL2/SDL_RenderGetLogicalSize
    /// @param renderer a rendering context
    /// @param w an int to be filled with the width
    /// @param h an int to be filled with the height
    void RenderGetLogicalSize(SDL_Renderer * renderer, int *w, int *h);

    /// @brief Get the Metal command encoder for the current frame https://wiki.libsdl.org/SDL2/SDL_RenderGetMetalCommandEncoder
    /// @param renderer The renderer to query
    /// @return Returns an id<MTLRenderCommandEncoder> on success, or NULL if the renderer isn't a Metal renderer or there was an error.
    void* RenderGetMetalCommandEncoder(SDL_Renderer * renderer);

    /// @brief Get the CAMetalLayer associated with the given Metal renderer. https://wiki.libsdl.org/SDL2/SDL_RenderGetMetalLayer
    /// @param renderer The renderer to query
    /// @return Returns a CAMetalLayer * on success, or NULL if the renderer isn't a Metal renderer
    void* RenderGetMetalLayer(SDL_Renderer * renderer);

    /// @brief Get the drawing scale for the current target. https://wiki.libsdl.org/SDL2/SDL_RenderGetScale
    /// @param renderer the renderer from which drawing scale should be queried
    /// @param scaleX a pointer filled in with the horizontal scaling factor
    /// @param scaleY a pointer filled in with the vertical scaling factor
    void RenderGetScale(SDL_Renderer * renderer, float *scaleX, float *scaleY);

    /// @brief Get the drawing area for the current target. https://wiki.libsdl.org/SDL2/SDL_RenderGetViewport
    /// @param renderer the rendering context
    /// @param rect an SDL_Rect structure filled in with the current drawing area
    void RenderGetViewport(SDL_Renderer * renderer, SDL_Rect * rect);

    /// @brief Get the window associated with a renderer. https://wiki.libsdl.org/SDL2/SDL_RenderGetWindow
    /// @param renderer the renderer to query
    /// @return Returns the window on success or NULL on failure; call SDL_GetError() for more information.
    SDL_Window * RenderGetWindow(SDL_Renderer *renderer);

    /// @brief Get whether clipping is enabled on the given renderer. https://wiki.libsdl.org/SDL2/SDL_RenderIsClipEnabled
    /// @param renderer the renderer from which clip state should be queried
    /// @return Returns SDL_TRUE if clipping is enabled or SDL_FALSE if not; call SDL_GetError() for more information.
    bool RenderIsClipEnabled(SDL_Renderer * renderer);

    /// @brief Get real coordinates of point in window when given logical coordinates of point in renderer. https://wiki.libsdl.org/SDL2/SDL_RenderLogicalToWindow
    /// @param renderer the renderer from which the window coordinates should be calculated
    /// @param logicalX the logical x coordinate
    /// @param logicalY the logical y coordinate
    /// @param windowX the pointer filled with the real X coordinate in the window
    /// @param windowY the pointer filled with the real Y coordinate in the window
    void RenderLogicalToWindow(SDL_Renderer * renderer, float logicalX, float logicalY, int *windowX, int *windowY);

    /// @brief Update the screen with any rendering performed since the previous call. https://wiki.libsdl.org/SDL2/SDL_RenderPresent
    /// @param renderer the rendering context
    void RenderPresent(SDL_Renderer * renderer);

    /// @brief Read pixels from the current rendering target to an array of pixels. https://wiki.libsdl.org/SDL2/SDL_RenderReadPixels
    /// @param renderer the rendering context
    /// @param rect an SDL_Rect structure representing the area to read, or NULL for the entire render target
    /// @param format an SDL_PixelFormatEnum value of the desired format of the pixel data, or 0 to use the format of the rendering target
    /// @param pixels a pointer to the pixel data to copy into
    /// @param pitch the pitch of the pixels parameter
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderReadPixels(SDL_Renderer * renderer, const SDL_Rect * rect, Uint32 format, void *pixels, int pitch);

    /// @brief Set the clip rectangle for rendering on the specified target. https://wiki.libsdl.org/SDL2/SDL_RenderSetClipRect
    /// @param renderer the rendering context for which clip rectangle should be set
    /// @param rect an SDL_Rect structure representing the clip area, relative to the viewport, or NULL to disable clipping
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderSetClipRect(SDL_Renderer * renderer, const SDL_Rect * rect);

    /// @brief Set whether to force integer scales for resolution-independent rendering. https://wiki.libsdl.org/SDL2/SDL_RenderSetIntegerScale
    /// @param renderer the renderer for which integer scaling should be set
    /// @param enable enable or disable the integer scaling for rendering
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderSetIntegerScale(SDL_Renderer * renderer, bool enable);

    /// @brief Set a device independent resolution for rendering. https://wiki.libsdl.org/SDL2/SDL_RenderSetLogicalSize
    /// @param renderer the renderer for which resolution should be set
    /// @param w the width of the logical resolution
    /// @param h the height of the logical resolution
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderSetLogicalSize(SDL_Renderer * renderer, int w, int h);

    /// @brief Set the drawing scale for rendering on the current target. https://wiki.libsdl.org/SDL2/SDL_RenderSetScale
    /// @param renderer a rendering context
    /// @param scaleX the horizontal scaling factor
    /// @param scaleY the vertical scaling factor
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderSetScale(SDL_Renderer * renderer, float scaleX, float scaleY);

    /// @brief Set the drawing area for rendering on the current target. https://wiki.libsdl.org/SDL2/SDL_RenderSetViewport
    /// @param renderer the rendering context
    /// @param rect the SDL_Rect structure representing the drawing area, or NULL to set the viewport to the entire target
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RenderSetViewport(SDL_Renderer * renderer, const SDL_Rect * rect);

    /// @brief Toggle VSync of the given renderer. https://wiki.libsdl.org/SDL2/SDL_RenderSetVSync
    /// @param renderer The renderer to toggle
    /// @param vsync 1 for on, 0 for off. All other values are reserved
    /// @return Returns a 0 int on success, or non-zero on failure
    int RenderSetVSync(SDL_Renderer* renderer, int vsync);

    /// @brief Determine whether a renderer supports the use of render targets. https://wiki.libsdl.org/SDL2/SDL_RenderTargetSupported
    /// @param renderer the renderer that will be checked
    /// @return Returns SDL_TRUE if supported or SDL_FALSE if not.
    bool RenderTargetSupported(SDL_Renderer *renderer);

    /// @brief Get logical coordinates of point in renderer when given real coordinates of point in window. https://wiki.libsdl.org/SDL2/SDL_RenderWindowToLogical
    /// @param renderer the renderer from which the logical coordinates should be calculated
    /// @param windowX the real X coordinate in the window
    /// @param windowY the real Y coordinate in the window
    /// @param logicalX the pointer filled with the logical x coordinate
    /// @param logicalY the pointer filled with the logical y coordinate
    void RenderWindowToLogical(SDL_Renderer * renderer, int windowX, int windowY, float *logicalX, float *logicalY);

    /// @brief Clear the list of all assertion failures. https://wiki.libsdl.org/SDL2/SDL_ResetAssertionReport
    void ResetAssertionReport();

    /// @brief Reset a hint to the default value. https://wiki.libsdl.org/SDL2/SDL_ResetHint
    /// @param name the hint to set
    /// @return Returns SDL_TRUE if the hint was set, SDL_FALSE otherwise.
    bool ResetHint(const char *name);

    /// @brief Reset all hints to the default values. https://wiki.libsdl.org/SDL2/SDL_ResetHints
    void ResetHints();

    /// @brief Clear the state of the keyboard https://wiki.libsdl.org/SDL2/SDL_ResetKeyboard
    void ResetKeyboard();

    /// @brief Restore the size and position of a minimized or maximized window. https://wiki.libsdl.org/SDL2/SDL_RestoreWindow
    /// @param window the window to restore
    void RestoreWindow(SDL_Window * window);

    /// @brief Close and free an allocated SDL_RWops structure. https://wiki.libsdl.org/SDL2/SDL_RWclose
    /// @param context SDL_RWops structure to close
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int RWclose(SDL_RWops *context);

    /// @brief Use this function to prepare a read-only memory buffer for use with RWops. https://wiki.libsdl.org/SDL2/SDL_RWFromConstMem
    /// @param mem a pointer to a read-only buffer to feed an SDL_RWops stream
    /// @param size the buffer size, in bytes
    /// @return Returns a pointer to a new SDL_RWops structure, or NULL if it fails; call SDL_GetError() for more information.
    SDL_RWops* RWFromConstMem(void *mem, int size);


    /// @brief Use this function to create an SDL_RWops structure from a standard I/O file pointer (stdio.h's FILE*). https://wiki.libsdl.org/SDL2/SDL_RWFromFP
    /// @param fp the FILE* that feeds the SDL_RWops stream
    /// @param autoclose SDL_TRUE to close the FILE* when closing the SDL_RWops, SDL_FALSE to leave the FILE* open when the RWops is closed
    /// @return Returns a pointer to the SDL_RWops structure that is created, or NULL on failure; call SDL_GetError() for more information.
    SDL_RWops* RWFromFP(void * fp, bool autoclose);

    /// @brief Use this function to prepare a read-write memory buffer for use with SDL_RWops. https://wiki.libsdl.org/SDL2/SDL_RWFromMem
    /// @param mem a pointer to a buffer to feed an SDL_RWops stream
    /// @param size the buffer size, in bytes
    /// @return Returns a pointer to a new SDL_RWops structure, or NULL if it fails; call SDL_GetError() for more information.
    SDL_RWops* RWFromMem(void *mem, int size);

    /// @brief Read from a data source. https://wiki.libsdl.org/SDL2/SDL_RWread
    /// @param context a pointer to an SDL_RWops structure
    /// @param ptr a pointer to a buffer to read data into
    /// @param size the size of each object to read, in bytes
    /// @param maxnum the maximum number of objects to be read
    /// @return Returns the number of objects read, or 0 at error or end of file; call SDL_GetError() for more information.
    size_t RWread(SDL_RWops *context, void *ptr, size_t size, size_t maxnum);

    /// @brief Seek within an SDL_RWops data stream. https://wiki.libsdl.org/SDL2/SDL_RWseek
    /// @param context a pointer to an SDL_RWops structure
    /// @param offset an offset in bytes, relative to whence location; can be negative
    /// @param whence any of RW_SEEK_SET, RW_SEEK_CUR, RW_SEEK_END
    /// @return Returns the final offset in the data stream after the seek or -1 on error.
    Sint64 RWseek(SDL_RWops *context, Sint64 offset, int whence);

    /// @brief Use this function to get the size of the data stream in an SDL_RWops. https://wiki.libsdl.org/SDL2/SDL_RWsize
    /// @param context the SDL_RWops to get the size of the data stream from
    /// @return Returns the size of the data stream in the SDL_RWops on success, -1 if unknown or a negative error code on failure; call SDL_GetError() for more information.
    Sint64 RWsize(SDL_RWops *context);

    /// @brief Determine the current read/write offset in an SDL_RWops data stream. https://wiki.libsdl.org/SDL2/SDL_RWtell
    /// @param context a SDL_RWops data stream object from which to get the current offset
    /// @return Returns the current offset in the stream, or -1 if the information can not be determined.
    Sint64 RWtell(SDL_RWops *context);

    /// @brief Write to an SDL_RWops data stream. https://wiki.libsdl.org/SDL2/SDL_RWwrite
    /// @param context a pointer to an SDL_RWops structure
    /// @param ptr a pointer to a buffer containing data to write
    /// @param size the size of an object to write, in bytes
    /// @param num the number of objects to write
    /// @return Returns the number of objects written, which will be less than num on error; call SDL_GetError() for more information.
    size_t RWwrite(SDL_RWops *context, void *ptr, size_t size, size_t num);

    /// @brief Save all currently loaded Dollar Gesture templates. https://wiki.libsdl.org/SDL2/SDL_SaveAllDollarTemplates
    /// @param dst a SDL_RWops to save to
    /// @return Returns the number of saved templates on success or 0 on failure; call SDL_GetError() for more information.
    int SaveAllDollarTemplates(SDL_RWops *dst);

    /// @brief Save a surface to a seekable SDL data stream in BMP format. https://wiki.libsdl.org/SDL2/SDL_SaveBMP_RW
    /// @param surface the SDL_Surface structure containing the image to be saved
    /// @param dst a data stream to save to
    /// @param freedst non-zero to close the stream after being written
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SaveBMP_RW(SDL_Surface * surface, SDL_RWops * dst, int freedst);

    /// @brief Use this function to save a surface to a BMP file. https://wiki.libsdl.org/SDL2/SDL_SaveBMP
    /// @param surface the SDL_Surface structure containing the image to be saved
    /// @param file the file to be saved to
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SaveBMP(SDL_Surface* surface,  const char* file);

    /// @brief Save a currently loaded Dollar Gesture template. https://wiki.libsdl.org/SDL2/SDL_SaveDollarTemplate
    /// @param gestureId a gesture id
    /// @param dst a SDL_RWops to save to
    /// @return Returns 1 on success or 0 on failure; call SDL_GetError() for more information.
    int SaveDollarTemplate(SDL_GestureID gestureId,SDL_RWops *dst);

    /// @brief Atomically increment a semaphore's value and wake waiting threads. https://wiki.libsdl.org/SDL2/SDL_SemPost
    /// @param sem the semaphore to increment
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SemPost(SDL_sem * sem);

    /// @brief See if a semaphore has a positive value and decrement it if it does. https://wiki.libsdl.org/SDL2/SDL_SemTryWait
    /// @param sem the semaphore to wait on
    /// @return Returns 0 if the wait succeeds, SDL_MUTEX_TIMEDOUT if the wait would block, or a negative error code on failure; call SDL_GetError() for more information.
    int SemTryWait(SDL_sem * sem);

    /// @brief Get the current value of a semaphore. https://wiki.libsdl.org/SDL2/SDL_SemValue
    /// @param sem the semaphore to query
    /// @return Returns the current value of the semaphore.
    Uint32 SemValue(SDL_sem * sem);

    /// @brief Wait until a semaphore has a positive value and then decrements it. https://wiki.libsdl.org/SDL2/SDL_SemWait
    /// @param sem the semaphore wait on
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SemWait(SDL_sem * sem);

    /// @brief Wait until a semaphore has a positive value and then decrements it. https://wiki.libsdl.org/SDL2/SDL_SemWaitTimeout
    /// @param sem the semaphore to wait on
    /// @param timeout the length of the timeout, in milliseconds
    /// @return Returns 0 if the wait succeeds, SDL_MUTEX_TIMEDOUT if the wait does not succeed in the allotted time, or a negative error code on failure; call SDL_GetError() for more information.
    int SemWaitTimeout(SDL_sem *sem, Uint32 timeout);

    /// @brief Close a sensor previously opened with SDL_SensorOpen(). https://wiki.libsdl.org/SDL2/SDL_SensorClose
    /// @param sensor The SDL_Sensor object to close
    void SensorClose(SDL_Sensor *sensor);

    /// @brief Return the SDL_Sensor associated with an instance id. https://wiki.libsdl.org/SDL2/SDL_SensorFromInstanceID
    /// @param instance_id The sensor from instance id
    /// @return Returns an SDL_Sensor object.
    SDL_Sensor* SensorFromInstanceID(SDL_SensorID instance_id);

    /// @brief Get the current state of an opened sensor. https://wiki.libsdl.org/SDL2/SDL_SensorGetData
    /// @param sensor The SDL_Sensor object to query
    /// @param data A pointer filled with the current sensor state
    /// @param num_values The number of values to write to data
    /// @return Returns 0 or -1 if an error occurred.
    int SensorGetData(SDL_Sensor *sensor, float *data, int num_values);

    /// @brief Get the current state of an opened sensor with the timestamp of the last update. https://wiki.libsdl.org/SDL2/SDL_SensorGetDataWithTimestamp
    /// @param sensor The SDL_Sensor object to query
    /// @param timestamp A pointer filled with the timestamp in microseconds of the current sensor reading if available, or 0 if not
    /// @param data A pointer filled with the current sensor state
    /// @param num_values The number of values to write to data
    /// @return Returns 0 or -1 if an error occurred.
    int SensorGetDataWithTimestamp(SDL_Sensor *sensor, Uint64 *timestamp, float *data, int num_values);

    /// @brief Get the instance ID of a sensor. https://wiki.libsdl.org/SDL2/SDL_SensorGetDeviceInstanceID
    /// @param device_index The sensor to get instance id from
    /// @return Returns the sensor instance ID, or -1 if device_index is out of range.
    SDL_SensorID SensorGetDeviceInstanceID(int device_index);

    /// @brief Get the implementation dependent name of a sensor. https://wiki.libsdl.org/SDL2/SDL_SensorGetDeviceName
    /// @param device_index The sensor to obtain name from
    /// @return Returns the sensor name, or NULL if device_index is out of range.
    const char* SensorGetDeviceName(int device_index);

    /// @brief Get the platform dependent type of a sensor. https://wiki.libsdl.org/SDL2/SDL_SensorGetDeviceNonPortableType
    /// @param device_index The sensor to check
    /// @return Returns the sensor platform dependent type, or -1 if device_index is out of range.
    int SensorGetDeviceNonPortableType(int device_index);

    /// @brief Get the type of a sensor. https://wiki.libsdl.org/SDL2/SDL_SensorGetDeviceType
    /// @param device_index The sensor to get the type from
    /// @return Returns the SDL_SensorType, or SDL_SENSOR_INVALID if device_index is out of range.
    SDL_SensorType SensorGetDeviceType(int device_index);

    /// @brief Get the instance ID of a sensor. https://wiki.libsdl.org/SDL2/SDL_SensorGetInstanceID
    /// @param sensor The SDL_Sensor object to inspect
    /// @return Returns the sensor instance ID, or -1 if sensor is NULL.
    SDL_SensorID SensorGetInstanceID(SDL_Sensor *sensor);

    /// @brief Get the implementation dependent name of a sensor https://wiki.libsdl.org/SDL2/SDL_SensorGetName
    /// @param sensor The SDL_Sensor object
    /// @return Returns the sensor name, or NULL if sensor is NULL.
    const char* SensorGetName(SDL_Sensor *sensor);

    /// @brief Get the platform dependent type of a sensor. https://wiki.libsdl.org/SDL2/SDL_SensorGetNonPortableType
    /// @param sensor The SDL_Sensor object to inspect
    /// @return Returns the sensor platform dependent type, or -1 if sensor is NULL.
    int SensorGetNonPortableType(SDL_Sensor *sensor);

    /// @brief Get the type of a sensor. https://wiki.libsdl.org/SDL2/SDL_SensorGetType
    /// @param sensor The SDL_Sensor object to inspect
    /// @return Returns the SDL_SensorType type, or SDL_SENSOR_INVALID if sensor is NULL.
    SDL_SensorType SensorGetType(SDL_Sensor *sensor);

    /// @brief Open a sensor for use. https://wiki.libsdl.org/SDL2/SDL_SensorOpen
    /// @param device_index The sensor to open
    /// @return Returns an SDL_Sensor sensor object, or NULL if an error occurred.
    SDL_Sensor* SensorOpen(int device_index);

    /// @brief Update the current state of the open sensors. https://wiki.libsdl.org/SDL2/SDL_SensorUpdate
    void SensorUpdate();

    /// @brief Set an application-defined assertion handler. https://wiki.libsdl.org/SDL2/SDL_SetAssertionHandler
    /// @param handler the SDL_AssertionHandler function to call when an assertion fails or NULL for the default handler
    /// @param userdata a pointer that is passed to handler
    void SetAssertionHandler( SDL_AssertionHandler handler, void *userdata);

    /// @brief Put UTF-8 text into the clipboard. https://wiki.libsdl.org/SDL2/SDL_SetClipboardText
    /// @param text the text to store in the clipboard
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetClipboardText(const char *text);

    /// @brief Set the clipping rectangle for a surface. https://wiki.libsdl.org/SDL2/SDL_SetClipRect
    /// @param surface the SDL_Surface structure to be clipped
    /// @param rect the SDL_Rect structure representing the clipping rectangle, or NULL to disable clipping
    /// @return Returns SDL_TRUE if the rectangle intersects the surface, otherwise SDL_FALSE and blits will be completely clipped.
    bool SetClipRect(SDL_Surface * surface, const SDL_Rect * rect);

    /// @brief Set the color key (transparent pixel) in a surface. https://wiki.libsdl.org/SDL2/SDL_SetColorKey
    /// @param surface the SDL_Surface structure to update
    /// @param flag SDL_TRUE to enable color key, SDL_FALSE to disable color key
    /// @param key the transparent pixel
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetColorKey(SDL_Surface * surface, int flag, Uint32 key);

    /// @brief Set the active cursor. https://wiki.libsdl.org/SDL2/SDL_SetCursor
    /// @param cursor a cursor to make active
    void SetCursor(SDL_Cursor * cursor);

    /// @brief Set up a filter to process all events before they change internal state and are posted to the internal event queue. https://wiki.libsdl.org/SDL2/SDL_SetEventFilter
    /// @param filter An SDL_EventFilter function to call when an event happens
    /// @param userdata a pointer that is passed to filter
    void SetEventFilter(SDL_EventFilter filter, void *userdata);

    /// @brief Set a hint with normal priority. https://wiki.libsdl.org/SDL2/SDL_SetHint
    /// @param name the hint to set
    /// @param value the value of the hint variable
    /// @return Returns SDL_TRUE if the hint was set, SDL_FALSE otherwise.
    bool SetHint(const char *name, const char *value);

    /// @brief Set a hint with a specific priority. https://wiki.libsdl.org/SDL2/SDL_SetHintWithPriority
    /// @param name the hint to set
    /// @param value the value of the hint variable
    /// @param priority the SDL_HintPriority level for the hint
    /// @return Returns SDL_TRUE if the hint was set, SDL_FALSE otherwise.
    bool SetHintWithPriority(const char *name, const char *value, SDL_HintPriority priority);

    /// @brief Circumvent failure of SDL_Init() when not using SDL_main() as an entry point. https://wiki.libsdl.org/SDL2/SDL_SetMainReady
    void SetMainReady();

    /// @brief Replace SDL's memory allocation functions with a custom set https://wiki.libsdl.org/SDL2/SDL_SetMemoryFunctions
    int SetMemoryFunctions(SDL_malloc_func malloc_func, SDL_calloc_func calloc_func, SDL_realloc_func realloc_func, SDL_free_func free_func);

    /// @brief Set the current key modifier state for the keyboard. https://wiki.libsdl.org/SDL2/SDL_SetModState
    /// @param modstate the desired SDL_Keymod for the keyboard
    void SetModState(SDL_Keymod modstate);

    /// @brief Set a range of colors in a palette. https://wiki.libsdl.org/SDL2/SDL_SetPaletteColors
    /// @param palette the SDL_Palette structure to modify
    /// @param colors an array of SDL_Color structures to copy into the palette
    /// @param firstcolor the index of the first palette entry to modify
    /// @param ncolors the number of entries to modify
    /// @return Returns 0 on success or a negative error code if not all of the colors could be set; call SDL_GetError() for more information.
    int SetPaletteColors(SDL_Palette * palette, const SDL_Color * colors, int firstcolor, int ncolors);

    /// @brief Set the palette for a pixel format structure. https://wiki.libsdl.org/SDL2/SDL_SetPixelFormatPalette
    /// @param format the SDL_PixelFormat structure that will use the palette
    /// @param palette the SDL_Palette structure that will be used
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetPixelFormatPalette(SDL_PixelFormat * format, SDL_Palette *palette);

    /// @brief Put UTF-8 text into the primary selection. https://wiki.libsdl.org/SDL2/SDL_SetPrimarySelectionText
    /// @param text the text to store in the primary selection
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetPrimarySelectionText(const char *text);

    /// @brief Set relative mouse mode. https://wiki.libsdl.org/SDL2/SDL_SetRelativeMouseMode
    /// @param enabled SDL_TRUE to enable relative mode, SDL_FALSE to disable.
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetRelativeMouseMode(bool enabled);

    /// @brief Set the blend mode used for drawing operations (Fill and Line). https://wiki.libsdl.org/SDL2/SDL_SetRenderDrawBlendMode
    /// @param renderer the rendering context
    /// @param blendMode the SDL_BlendMode to use for blending
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetRenderDrawBlendMode(SDL_Renderer * renderer, SDL_BlendMode blendMode);

    /// @brief Set the color used for drawing operations (Rect, Line and Clear). https://wiki.libsdl.org/SDL2/SDL_SetRenderDrawColor
    /// @param renderer the rendering context
    /// @param r the red value used to draw on the rendering target
    /// @param g the green value used to draw on the rendering target
    /// @param b the blue value used to draw on the rendering target
    /// @param a the alpha value used to draw on the rendering target; usually SDL_ALPHA_OPAQUE (255). Use SDL_SetRenderDrawBlendMode to specify how the alpha channel is used
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetRenderDrawColor(SDL_Renderer * renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /// @brief Set a texture as the current rendering target. https://wiki.libsdl.org/SDL2/SDL_SetRenderTarget
    /// @param renderer the rendering context
    /// @param texture the targeted texture, which must be created with the SDL_TEXTUREACCESS_TARGET flag, or NULL to render to the window instead of a texture.
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetRenderTarget(SDL_Renderer *renderer, SDL_Texture *texture);

    /// @brief Set an additional alpha value used in blit operations. https://wiki.libsdl.org/SDL2/SDL_SetSurfaceAlphaMod
    /// @param surface the SDL_Surface structure to update
    /// @param alpha the alpha value multiplied into blit operations
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetSurfaceAlphaMod(SDL_Surface * surface, Uint8 alpha);

    /// @brief Set the blend mode used for blit operations. https://wiki.libsdl.org/SDL2/SDL_SetSurfaceBlendMode
    /// @param surface the SDL_Surface structure to update
    /// @param blendMode the SDL_BlendMode to use for blit blending
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetSurfaceBlendMode(SDL_Surface * surface, SDL_BlendMode blendMode);

    /// @brief Set an additional color value multiplied into blit operations. https://wiki.libsdl.org/SDL2/SDL_SetSurfaceColorMod
    /// @param surface the SDL_Surface structure to update
    /// @param r the red color value multiplied into blit operations
    /// @param g the green color value multiplied into blit operations
    /// @param b the blue color value multiplied into blit operations
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetSurfaceColorMod(SDL_Surface * surface, Uint8 r, Uint8 g, Uint8 b);

    /// @brief Set the palette used by a surface. https://wiki.libsdl.org/SDL2/SDL_SetSurfacePalette
    /// @param surface the SDL_Surface structure to update
    /// @param palette the SDL_Palette structure to use
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetSurfacePalette(SDL_Surface * surface, SDL_Palette * palette);

    /// @brief Set the RLE acceleration hint for a surface. https://wiki.libsdl.org/SDL2/SDL_SetSurfaceRLE
    /// @param surface the SDL_Surface structure to optimize
    /// @param flag 0 to disable, non-zero to enable RLE acceleration
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetSurfaceRLE(SDL_Surface * surface, int flag);

    /// @brief Set the rectangle used to type Unicode text inputs. https://wiki.libsdl.org/SDL2/SDL_SetTextInputRect
    /// @param rect the SDL_Rect structure representing the rectangle to receive text (ignored if NULL)
    void SetTextInputRect(const SDL_Rect *rect);

    /// @brief Set an additional alpha value multiplied into render copy operations. https://wiki.libsdl.org/SDL2/SDL_SetTextureAlphaMod
    /// @param texture the texture to update
    /// @param alpha the source alpha value multiplied into copy operations
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetTextureAlphaMod(SDL_Texture * texture, Uint8 alpha);

    /// @brief Set the blend mode for a texture, used by SDL_RenderCopy(). https://wiki.libsdl.org/SDL2/SDL_SetTextureBlendMode
    /// @param texture the texture to update
    /// @param blendMode the SDL_BlendMode to use for texture blending
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetTextureBlendMode(SDL_Texture * texture, SDL_BlendMode blendMode);

    /// @brief Set an additional color value multiplied into render copy operations. https://wiki.libsdl.org/SDL2/SDL_SetTextureColorMod
    /// @param texture the texture to update
    /// @param r the red color value multiplied into copy operations
    /// @param g the green color value multiplied into copy operations
    /// @param b the blue color value multiplied into copy operations
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetTextureColorMod(SDL_Texture * texture, Uint8 r, Uint8 g, Uint8 b);

    /// @brief Set the scale mode used for texture scale operations. https://wiki.libsdl.org/SDL2/SDL_SetTextureScaleMode
    /// @param texture The texture to update.
    /// @param scaleMode the SDL_ScaleMode to use for texture scaling.
    /// @return Returns 0 on success, or -1 if the texture is not valid.
    int SetTextureScaleMode(SDL_Texture * texture, SDL_ScaleMode scaleMode);

    /// @brief Associate a user-specified pointer with a texture. https://wiki.libsdl.org/SDL2/SDL_SetTextureUserData
    /// @param texture the texture to update.
    /// @param userdata the pointer to associate with the texture.
    /// @return Returns 0 on success, or -1 if the texture is not valid.
    int SetTextureUserData(SDL_Texture * texture, void *userdata);

    /// @brief Set the priority for the current thread. https://wiki.libsdl.org/SDL2/SDL_SetThreadPriority
    /// @param priority the SDL_ThreadPriority to set
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetThreadPriority(SDL_ThreadPriority priority);

    /// @brief Set the window to always be above the others. https://wiki.libsdl.org/SDL2/SDL_SetWindowAlwaysOnTop
    /// @param window The window of which to change the always on top state
    /// @param on_top SDL_TRUE to set the window always on top, SDL_FALSE to disable
    void SetWindowAlwaysOnTop(SDL_Window * window, bool on_top);

    /// @brief Set the border state of a window. https://wiki.libsdl.org/SDL2/SDL_SetWindowBordered
    /// @param window the window of which to change the border state
    /// @param bordered SDL_FALSE to remove border, SDL_TRUE to add border
    void SetWindowBordered(SDL_Window * window, bool bordered);

    /// @brief Set the brightness (gamma multiplier) for a given window's display. https://wiki.libsdl.org/SDL2/SDL_SetWindowBrightness
    /// @param window the window used to select the display whose brightness will be changed
    /// @param brightness the brightness (gamma multiplier) value to set where 0.0 is completely dark and 1.0 is normal brightness
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetWindowBrightness(SDL_Window * window, float brightness);

    /// @brief Associate an arbitrary named pointer with a window. https://wiki.libsdl.org/SDL2/SDL_SetWindowData
    /// @param window the window to associate with the pointer
    /// @param name the name of the pointer
    /// @param userdata the associated pointer
    /// @return Returns the previous value associated with name.
    void* SetWindowData(SDL_Window * window, const char *name, void *userdata);

    /// @brief Set the display mode to use when a window is visible at fullscreen. https://wiki.libsdl.org/SDL2/SDL_SetWindowDisplayMode
    /// @param window the window to affect
    /// @param mode the SDL_DisplayMode structure representing the mode to use, or NULL to use the window's dimensions and the desktop's format and refresh rate
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetWindowDisplayMode(SDL_Window * window, const SDL_DisplayMode * mode);

    /// @brief Set a window's fullscreen state. https://wiki.libsdl.org/SDL2/SDL_SetWindowFullscreen
    /// @param window the window to change
    /// @param flags SDL_WINDOW_FULLSCREEN, SDL_WINDOW_FULLSCREEN_DESKTOP or 0
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetWindowFullscreen(SDL_Window * window, Uint32 flags);

    /// @brief Set the gamma ramp for the display that owns a given window. https://wiki.libsdl.org/SDL2/SDL_SetWindowGammaRamp
    /// @param window the window used to select the display whose gamma ramp will be changed
    /// @param red a 256 element array of 16-bit quantities representing the translation table for the red channel, or NULL
    /// @param green a 256 element array of 16-bit quantities representing the translation table for the green channel, or NULL
    /// @param blue a 256 element array of 16-bit quantities representing the translation table for the blue channel, or NULL
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetWindowGammaRamp(SDL_Window * window, const uint16_t * red, const uint16_t * green, const uint16_t * blue);

    /// @brief Set a window's input grab mode. https://wiki.libsdl.org/SDL2/SDL_SetWindowGrab
    /// @param window the window for which the input grab mode should be set
    /// @param grabbed SDL_TRUE to grab input or SDL_FALSE to release input
    void SetWindowGrab(SDL_Window * window, bool grabbed);

    /// @brief Provide a callback that decides if a window region has special properties. https://wiki.libsdl.org/SDL2/SDL_SetWindowHitTest
    /// @param window the window to set hit-testing on
    /// @param callback the function to call when doing a hit-test
    /// @param callback_data an app-defined void pointer passed to callback
    /// @return Returns 0 on success or -1 on error (including unsupported); call SDL_GetError() for more information.
    int SetWindowHitTest(SDL_Window * window, SDL_HitTest callback, void *callback_data);

    /// @brief Set the icon for a window. https://wiki.libsdl.org/SDL2/SDL_SetWindowIcon
    /// @param window the window to change
    /// @param icon an SDL_Surface structure containing the icon for the window
    void SetWindowIcon(SDL_Window * window, SDL_Surface * icon);

    /// @brief Explicitly set input focus to the window. https://wiki.libsdl.org/SDL2/SDL_SetWindowInputFocus
    /// @param window the window that should get the input focus
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetWindowInputFocus(SDL_Window * window);

    /// @brief Set a window's keyboard grab mode. https://wiki.libsdl.org/SDL2/SDL_SetWindowKeyboardGrab
    /// @param window The window for which the keyboard grab mode should be set.
    /// @param grabbed This is SDL_TRUE to grab keyboard, and SDL_FALSE to release.
    void SetWindowKeyboardGrab(SDL_Window * window, bool grabbed);

    /// @brief Set the maximum size of a window's client area. https://wiki.libsdl.org/SDL2/SDL_SetWindowMaximumSize
    /// @param window the window to change
    /// @param max_w the maximum width of the window in pixels
    /// @param max_h the maximum height of the window in pixels
    void SetWindowMaximumSize(SDL_Window * window, int max_w, int max_h);

    /// @brief Set the minimum size of a window's client area. https://wiki.libsdl.org/SDL2/SDL_SetWindowMinimumSize
    /// @param window the window to change
    /// @param min_w the minimum width of the window in pixels
    /// @param min_h the minimum height of the window in pixels
    void SetWindowMinimumSize(SDL_Window * window, int min_w, int min_h);

    /// @brief Set the window as a modal for another window. https://wiki.libsdl.org/SDL2/SDL_SetWindowModalFor
    /// @param modal_window the window that should be set modal
    /// @param parent_window the parent window for the modal window
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetWindowModalFor(SDL_Window * modal_window, SDL_Window * parent_window);

    /// @brief Set a window's mouse grab mode. https://wiki.libsdl.org/SDL2/SDL_SetWindowMouseGrab
    /// @param window The window for which the mouse grab mode should be set.
    /// @param grabbed This is SDL_TRUE to grab mouse, and SDL_FALSE to release.
    void SetWindowMouseGrab(SDL_Window * window, bool grabbed);

    /// @brief Confines the cursor to the specified area of a window. https://wiki.libsdl.org/SDL2/SDL_SetWindowMouseRect
    /// @param window The window that will be associated with the barrier.
    /// @param rect A rectangle area in window-relative coordinates. If NULL the barrier for the specified window will be destroyed.
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetWindowMouseRect(SDL_Window * window, const SDL_Rect * rect);

    /// @brief Set the opacity for a window. https://wiki.libsdl.org/SDL2/SDL_SetWindowOpacity
    /// @param window the window which will be made transparent or opaque
    /// @param opacity the opacity value (0.0f - transparent, 1.0f - opaque)
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int SetWindowOpacity(SDL_Window * window, float opacity);

    /// @brief Set the position of a window. https://wiki.libsdl.org/SDL2/SDL_SetWindowPosition
    /// @param window the window to reposition
    /// @param x the x coordinate of the window in screen coordinates, or SDL_WINDOWPOS_CENTERED or SDL_WINDOWPOS_UNDEFINED
    /// @param y the y coordinate of the window in screen coordinates, or SDL_WINDOWPOS_CENTERED or SDL_WINDOWPOS_UNDEFINED
    void SetWindowPosition(SDL_Window * window, int x, int y);

    /// @brief Set the user-resizable state of a window. https://wiki.libsdl.org/SDL2/SDL_SetWindowResizable
    /// @param window the window of which to change the resizable state
    /// @param resizable SDL_TRUE to allow resizing, SDL_FALSE to disallow
    void SetWindowResizable(SDL_Window * window, bool resizable);

    /// @brief Set the shape and parameters of a shaped window. https://wiki.libsdl.org/SDL2/SDL_SetWindowShape
    /// @param window The shaped window whose parameters should be set.
    /// @param shape A surface encoding the desired shape for the window.
    /// @param shape_mode The parameters to set for the shaped window.
    /// @return Return 0 on success, SDL_INVALID_SHAPE_ARGUMENT on an invalid shape argument, or SDL_NONSHAPEABLE_WINDOW if the SDL_Window given does not reference a valid shaped window.
    int SetWindowShape(SDL_Window *window,SDL_Surface *shape,SDL_WindowShapeMode *shape_mode);

    /// @brief Set the size of a window's client area. https://wiki.libsdl.org/SDL2/SDL_SetWindowSize
    /// @param window the window to change
    /// @param w the width of the window in pixels, in screen coordinates, must be > 0
    /// @param h the height of the window in pixels, in screen coordinates, must be > 0
    void SetWindowSize(SDL_Window * window, int w, int h);

    /// @brief Set the title of a window. https://wiki.libsdl.org/SDL2/SDL_SetWindowTitle
    /// @param window the window to change
    /// @param title the desired window title in UTF-8 format
    void SetWindowTitle(SDL_Window * window, const char *title);

    /// @brief Set the YUV conversion mode https://wiki.libsdl.org/SDL2/SDL_SetYUVConversionMode
    void SetYUVConversionMode(SDL_YUV_CONVERSION_MODE mode);

    /// @brief Toggle whether or not the cursor is shown. https://wiki.libsdl.org/SDL2/SDL_ShowCursor
    /// @param toggle SDL_ENABLE to show the cursor, SDL_DISABLE to hide it, SDL_QUERY to query the current state without changing it.
    /// @return Returns SDL_ENABLE if the cursor is shown, or SDL_DISABLE if the cursor is hidden, or a negative error code on failure; call SDL_GetError() for more information.
    int ShowCursor(int toggle);

    /// @brief Create a modal message box. https://wiki.libsdl.org/SDL2/SDL_ShowMessageBox
    /// @param messageboxdata the SDL_MessageBoxData structure with title, text and other options
    /// @param buttonid the pointer to which user id of hit button should be copied
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int ShowMessageBox(const SDL_MessageBoxData *messageboxdata, int *buttonid);

    /// @brief Display a simple modal message box. https://wiki.libsdl.org/SDL2/SDL_ShowSimpleMessageBox
    /// @param flags an SDL_MessageBoxFlags value
    /// @param title UTF-8 title text
    /// @param message UTF-8 message text
    /// @param window the parent window, or NULL for no parent
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int ShowSimpleMessageBox(Uint32 flags, const char *title, const char *message, SDL_Window *window);

    /// @brief Show a window. https://wiki.libsdl.org/SDL2/SDL_ShowWindow
    /// @param window the window to show
    void ShowWindow(SDL_Window * window);

    /// @brief Allocate memory in a SIMD-friendly way. https://wiki.libsdl.org/SDL2/SDL_SIMDAlloc
    /// @param len The length, in bytes, of the block to allocate. The actual allocated block might be larger due to padding, etc.
    /// @return Returns a pointer to the newly-allocated block, NULL if out of memory.
    void * SIMDAlloc(const size_t len);

    /// @brief Deallocate memory obtained from SDL_SIMDAlloc https://wiki.libsdl.org/SDL2/SDL_SIMDFree
    /// @param ptr The pointer, returned from SDL_SIMDAlloc or SDL_SIMDRealloc, to deallocate. NULL is a legal no-op.
    void SIMDFree(void *ptr);

    /// @brief Report the alignment this system needs for SIMD allocations. https://wiki.libsdl.org/SDL2/SDL_SIMDGetAlignment
    /// @return Returns the alignment in bytes needed for available, known SIMD instructions.
    size_t SIMDGetAlignment();

    /// @brief Reallocate memory obtained from SDL_SIMDAlloc https://wiki.libsdl.org/SDL2/SDL_SIMDRealloc
    /// @param mem The pointer obtained from SDL_SIMDAlloc. This function also accepts NULL, at which point this function is the same as calling SDL_SIMDAlloc with a NULL pointer.
    /// @param len The length, in bytes, of the block to allocated. The actual allocated block might be larger due to padding, etc. Passing 0 will return a non-NULL pointer, assuming the system isn't out of memory.
    /// @return Returns a pointer to the newly-reallocated block, NULL if out of memory.
    void * SIMDRealloc(void *mem, const size_t len);

    /// @brief Perform a fast, low quality, stretch blit between two surfaces of the same format. https://wiki.libsdl.org/SDL2/SDL_SoftStretch
    int SoftStretch(SDL_Surface * src, const SDL_Rect * srcrect, SDL_Surface * dst, const SDL_Rect * dstrect);

    /// @brief Perform bilinear scaling between two surfaces of the same format, 32BPP. https://wiki.libsdl.org/SDL2/SDL_SoftStretchLinear
    int SoftStretchLinear(SDL_Surface * src, const SDL_Rect * srcrect, SDL_Surface * dst, const SDL_Rect * dstrect);

    /// @brief Start accepting Unicode text input events. https://wiki.libsdl.org/SDL2/SDL_StartTextInput
    void StartTextInput();

    /// @brief Stop receiving any text input events. https://wiki.libsdl.org/SDL2/SDL_StopTextInput
    void StopTextInput();

    /// @brief Use this function to swap the byte order of a 16-bit value. https://wiki.libsdl.org/SDL2/SDL_Swap16
    /// @param x the value to be swapped
    /// @return Returns the swapped value.
    uint16_t Swap16(uint16_t x);

    /// @brief Use this function to swap the byte order of a 32-bit value. https://wiki.libsdl.org/SDL2/SDL_Swap32
    /// @param x the value to be swapped
    /// @return Returns the swapped value.
    uint32_t Swap32(uint32_t x);

    /// @brief Use this function to swap the byte order of a 64-bit value. https://wiki.libsdl.org/SDL2/SDL_Swap64
    /// @param x the value to be swapped
    /// @return Returns the swapped value.
    uint64_t Swap64(uint64_t x);

    /// @brief Use this function to swap the byte order of a 16-bit big-endian value to native ordering. https://wiki.libsdl.org/SDL2/SDL_SwapBE16
    /// @param x the value to be swapped
    /// @return Returns the native 16-bit value.
    uint16_t SwapBE16(uint16_t x);

    /// @brief Use this function to swap the byte order of a 32-bit big-endian value to native ordering. https://wiki.libsdl.org/SDL2/SDL_SwapBE32
    /// @param x the value to be swapped
    /// @return Returns the native 32-bit value.
    uint32_t SwapBE32(uint32_t x);

    /// @brief Use this function to swap the byte order of a 64-bit big-endian value to native ordering. https://wiki.libsdl.org/SDL2/SDL_SwapBE64
    /// @param x the value to be swapped
    /// @return Returns the native 64-bit value.
    uint64_t SwapBE64(uint64_t x);

    /// @brief Use this function to swap the byte order of a floating point value. https://wiki.libsdl.org/SDL2/SDL_SwapFloat
    /// @param x the value to be swapped
    /// @return Returns the swapped value.
    float SwapFloat(float x);

    /// @brief Use this function to swap the byte order of a big-endian floating point value to native ordering. https://wiki.libsdl.org/SDL2/SDL_SwapFloatBE
    /// @param x the value to be swapped
    /// @return Returns the native floating point value.
    float SwapFloatBE(float x);

    /// @brief Use this function to swap the byte order of a little-endian floating point value to native ordering. https://wiki.libsdl.org/SDL2/SDL_SwapFloatLE
    /// @param x the value to be swapped
    /// @return Returns the native floating point value.
    float SwapFloatLE(float x);

    /// @brief Use this function to swap the byte order of a 16-bit little-endian value to native ordering. https://wiki.libsdl.org/SDL2/SDL_SwapLE16
    /// @param x the value to be swapped
    /// @return Returns the native 16-bit value.
    uint16_t SwapLE16(uint16_t x);

    /// @brief Use this function to swap the byte order of a 32-bit little-endian value to native ordering. https://wiki.libsdl.org/SDL2/SDL_SwapLE32
    /// @param x the value to be swapped
    /// @return Returns the native 32-bit value.
    uint32_t SwapLE32(uint32_t x);

    /// @brief Use this function to swap the byte order of a 64-bit little-endian value to native ordering. https://wiki.libsdl.org/SDL2/SDL_SwapLE64
    /// @param x the value to be swapped
    /// @return Returns the native 64-bit value.
    uint64_t SwapLE64(uint64_t x);

    /// @brief Get the thread identifier for the current thread. https://wiki.libsdl.org/SDL2/SDL_ThreadID
    /// @return Returns the ID of the current thread.
    SDL_threadID ThreadID();

    /// @brief Cleanup all TLS data for this thread. https://wiki.libsdl.org/SDL2/SDL_TLSCleanup
    void TLSCleanup();

    /// @brief Create a piece of thread-local storage. https://wiki.libsdl.org/SDL2/SDL_TLSCreate
    /// @return Returns the newly created thread local storage identifier or 0 on error.
    SDL_TLSID TLSCreate();

    /// @brief Get the current thread's value associated with a thread local storage ID. https://wiki.libsdl.org/SDL2/SDL_TLSGet
    /// @param id the thread local storage ID
    /// @return Returns the value associated with the ID for the current thread or NULL if no value has been set; call SDL_GetError() for more information.
    void * TLSGet(SDL_TLSID id);

    /// @brief Set the current thread's value associated with a thread local storage ID. https://wiki.libsdl.org/SDL2/SDL_TLSSet
    /// @param id the thread local storage ID
    /// @param value the value to associate with the ID for the current thread
    /// @param destructor a function called when the thread exits, to free the value
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int TLSSet(SDL_TLSID id, void *value, void ( *destructor)(void*));

    /// @brief Use this function to trigger a breakpoint during debugging. https://wiki.libsdl.org/SDL2/SDL_TriggerBreakpoint
    void TriggerBreakpoint();

    /// @brief Try to lock a mutex without blocking. https://wiki.libsdl.org/SDL2/SDL_TryLockMutex
    /// @param mutex the mutex to try to lock
    /// @return Returns 0, SDL_MUTEX_TIMEDOUT, or -1 on error; call SDL_GetError() for more information.
    int TryLockMutex(SDL_mutex * mutex );

    /// @brief Calculate the union of two rectangles with float precision. https://wiki.libsdl.org/SDL2/SDL_UnionFRect
    /// @param A an SDL_FRect structure representing the first rectangle
    /// @param B an SDL_FRect structure representing the second rectangle
    /// @param result an SDL_FRect structure filled in with the union of rectangles A and B
    void UnionFRect(const SDL_FRect * A, const SDL_FRect * B, SDL_FRect * result);

    /// @brief Calculate the union of two rectangles. https://wiki.libsdl.org/SDL2/SDL_UnionRect
    /// @param A an SDL_Rect structure representing the first rectangle
    /// @param B an SDL_Rect structure representing the second rectangle
    /// @param result an SDL_Rect structure filled in with the union of rectangles A and B
    void UnionRect(const SDL_Rect * A, const SDL_Rect * B, SDL_Rect * result);

    /// @brief Unload a shared object from memory. https://wiki.libsdl.org/SDL2/SDL_UnloadObject
    /// @param handle a valid shared object handle returned by SDL_LoadObject()
    void UnloadObject(void *handle);

    /// @brief This function is a legacy means of unlocking the audio device. https://wiki.libsdl.org/SDL2/SDL_UnlockAudio
    void UnlockAudio();

    /// @brief Use this function to unlock the audio callback function for a specified device. https://wiki.libsdl.org/SDL2/SDL_UnlockAudioDevice
    /// @param dev the ID of the device to be unlocked
    void UnlockAudioDevice(SDL_AudioDeviceID dev);

    /// @brief Unlocking for multi-threaded access to the joystick API https://wiki.libsdl.org/SDL2/SDL_UnlockJoysticks
    void UnlockJoysticks();

    /// @brief Unlock the mutex. https://wiki.libsdl.org/SDL2/SDL_UnlockMutex
    /// @param mutex the mutex to unlock.
    /// @return Returns 0, or -1 on error.
    int UnlockMutex(SDL_mutex * mutex );

    /// @brief Release a surface after directly accessing the pixels. https://wiki.libsdl.org/SDL2/SDL_UnlockSurface
    /// @param surface the SDL_Surface structure to be unlocked
    void UnlockSurface(SDL_Surface * surface);

    /// @brief Unlock a texture, uploading the changes to video memory, if needed. https://wiki.libsdl.org/SDL2/SDL_UnlockTexture
    /// @param texture a texture locked by SDL_LockTexture()
    void UnlockTexture(SDL_Texture * texture);

    /// @brief Update a rectangle within a planar NV12 or NV21 texture with new pixels. https://wiki.libsdl.org/SDL2/SDL_UpdateNVTexture
    /// @param texture the texture to update
    /// @param rect a pointer to the rectangle of pixels to update, or NULL to update the entire texture.
    /// @param Yplane the raw pixel data for the Y plane.
    /// @param Ypitch the number of bytes between rows of pixel data for the Y plane.
    /// @param UVplane the raw pixel data for the UV plane.
    /// @param UVpitch the number of bytes between rows of pixel data for the UV plane.
    /// @return Return 0 on success, or -1 if the texture is not valid.
    int UpdateNVTexture(SDL_Texture * texture, const SDL_Rect * rect, const Uint8 *Yplane, int Ypitch, const Uint8 *UVplane, int UVpitch);

    /// @brief Update the given texture rectangle with new pixel data. https://wiki.libsdl.org/SDL2/SDL_UpdateTexture
    /// @param texture the texture to update
    /// @param rect an SDL_Rect structure representing the area to update, or NULL to update the entire texture
    /// @param pixels the raw pixel data in the format of the texture
    /// @param pitch the number of bytes in a row of pixel data, including padding between lines
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int UpdateTexture(SDL_Texture * texture, const SDL_Rect * rect, void *pixels, int pitch);

    /// @brief Copy the window surface to the screen. https://wiki.libsdl.org/SDL2/SDL_UpdateWindowSurface
    /// @param window the window to update
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int UpdateWindowSurface(SDL_Window * window);

    /// @brief Copy areas of the window surface to the screen. https://wiki.libsdl.org/SDL2/SDL_UpdateWindowSurfaceRects
    /// @param window the window to update
    /// @param rects an array of SDL_Rect structures representing areas of the surface to copy, in pixels
    /// @param numrects the number of rectangles
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int UpdateWindowSurfaceRects(SDL_Window * window, const SDL_Rect * rects, int numrects);

    /// @brief Update a rectangle within a planar YV12 or IYUV texture with new pixel data. https://wiki.libsdl.org/SDL2/SDL_UpdateYUVTexture
    /// @param texture the texture to update
    /// @param rect a pointer to the rectangle of pixels to update, or NULL to update the entire texture
    /// @param Yplane the raw pixel data for the Y plane
    /// @param Ypitch the number of bytes between rows of pixel data for the Y plane
    /// @param Uplane the raw pixel data for the U plane
    /// @param Upitch the number of bytes between rows of pixel data for the U plane
    /// @param Vplane the raw pixel data for the V plane
    /// @param Vpitch the number of bytes between rows of pixel data for the V plane
    /// @return Returns 0 on success or -1 if the texture is not valid; call SDL_GetError() for more information.
    int UpdateYUVTexture(SDL_Texture * texture, const SDL_Rect * rect, const Uint8 *Yplane, int Ypitch, const Uint8 *Uplane, int Upitch, const Uint8 *Vplane, int Vpitch);

    /// @brief Perform a fast blit from the source surface to the destination surface. https://wiki.libsdl.org/SDL2/SDL_UpperBlit
    int UpperBlit(SDL_Surface * src, const SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect);

    /// @brief Perform a scaled surface copy to a destination surface. https://wiki.libsdl.org/SDL2/SDL_UpperBlitScaled
    int UpperBlitScaled(SDL_Surface * src, const SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect);

    /// @brief Initialize the video subsystem, optionally specifying a video driver. https://wiki.libsdl.org/SDL2/SDL_VideoInit
    /// @param driver_name the name of a video driver to initialize, or NULL for the default driver
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int VideoInit(const char *driver_name);

    /// @brief Shut down the video subsystem, if initialized with SDL_VideoInit(). https://wiki.libsdl.org/SDL2/SDL_VideoQuit
    void VideoQuit();

#ifdef COSMO_SDL2_VULKAN

    /// @brief Create a Vulkan rendering surface for a window. https://wiki.libsdl.org/SDL2/SDL_Vulkan_CreateSurface
    /// @param window The window to which to attach the Vulkan surface
    /// @param instance The Vulkan instance handle
    /// @param surface A pointer to a VkSurfaceKHR handle to output the newly created surface
    /// @return Returns SDL_TRUE on success, SDL_FALSE on error.
    bool Vulkan_CreateSurface(SDL_Window *window, VkInstance instance, VkSurfaceKHR* surface);

    /// @brief Get the size of the window's underlying drawable dimensions in pixels. https://wiki.libsdl.org/SDL2/SDL_Vulkan_GetDrawableSize
    /// @param window an SDL_Window for which the size is to be queried
    /// @param w Pointer to the variable to write the width to or NULL
    /// @param h Pointer to the variable to write the height to or NULL
    void Vulkan_GetDrawableSize(SDL_Window * window, int *w, int *h);

    /// @brief Get the names of the Vulkan instance extensions needed to create a surface with SDL_Vulkan_CreateSurface. https://wiki.libsdl.org/SDL2/SDL_Vulkan_GetInstanceExtensions
    /// @param window A window for which the required Vulkan instance extensions should be retrieved (will be deprecated in a future release)
    /// @param pCount A pointer to an unsigned int corresponding to the number of extensions to be returned
    /// @param pNames NULL or a pointer to an array to be filled with required Vulkan instance extensions
    /// @return Returns SDL_TRUE on success, SDL_FALSE on error.
    bool Vulkan_GetInstanceExtensions(SDL_Window *window, unsigned int *pCount, const char **pNames);

    /// @brief Get the address of the vkGetInstanceProcAddr function. https://wiki.libsdl.org/SDL2/SDL_Vulkan_GetVkGetInstanceProcAddr
    /// @return Returns the function pointer for vkGetInstanceProcAddr or NULL on error.
    void* Vulkan_GetVkGetInstanceProcAddr();

    /// @brief Use this function to get the address of the ```vkGetInstanceProcAddr``` function. https://wiki.libsdl.org/SDL2/SDL_Vulkan_GetVkInstanceProcAddr
    /// @return Returns a pointer to the vkGetInstanceProcAddr which can be used to load Vulkan function pointers at runtime.
    void * SDL_Vulkan_GetVkGetInstanceProcAddr()Vulkan_GetVkInstanceProcAddr();

    /// @brief Dynamically load the Vulkan loader library. https://wiki.libsdl.org/SDL2/SDL_Vulkan_LoadLibrary
    /// @param path The platform dependent Vulkan loader library name or NULL
    /// @return Returns 0 on success or -1 if the library couldn't be loaded; call SDL_GetError() for more information.
    int Vulkan_LoadLibrary(const char *path);

    /// @brief Unload the Vulkan library previously loaded by SDL_Vulkan_LoadLibrary() https://wiki.libsdl.org/SDL2/SDL_Vulkan_UnloadLibrary
    void Vulkan_UnloadLibrary();

#endif

    /// @brief Wait indefinitely for the next available event. https://wiki.libsdl.org/SDL2/SDL_WaitEvent
    /// @param event the SDL_Event structure to be filled in with the next event from the queue, or NULL
    /// @return Returns 1 on success or 0 if there was an error while waiting for events; call SDL_GetError() for more information.
    int WaitEvent(SDL_Event * event);

    /// @brief Wait until the specified timeout (in milliseconds) for the next available event. https://wiki.libsdl.org/SDL2/SDL_WaitEventTimeout
    /// @param event the SDL_Event structure to be filled in with the next event from the queue, or NULL
    /// @param timeout the maximum number of milliseconds to wait for the next available event
    /// @return Returns 1 on success or 0 if there was an error while waiting for events; call SDL_GetError() for more information. This also returns 0 if the timeout elapsed without an event arriving.
    int WaitEventTimeout(SDL_Event * event, int timeout);

    /// @brief Wait for a thread to finish. https://wiki.libsdl.org/SDL2/SDL_WaitThread
    /// @param thread the SDL_Thread pointer that was returned from the SDL_CreateThread() call that started this thread
    /// @param status pointer to an integer that will receive the value returned from the thread function by its 'return', or NULL to not receive such value back.
    void WaitThread(SDL_Thread * thread, int *status);

    /// @brief Move the mouse to the given position in global screen space. https://wiki.libsdl.org/SDL2/SDL_WarpMouseGlobal
    /// @param x the x coordinate
    /// @param y the y coordinate
    /// @return Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
    int WarpMouseGlobal(int x, int y);

    /// @brief Move the mouse cursor to the given position within the window. https://wiki.libsdl.org/SDL2/SDL_WarpMouseInWindow
    /// @param window the window to move the mouse into, or NULL for the current mouse focus
    /// @param x the x coordinate within the window
    /// @param y the y coordinate within the window
    void WarpMouseInWindow(SDL_Window * window, int x, int y);

    /// @brief Get a mask of the specified subsystems which are currently initialized. https://wiki.libsdl.org/SDL2/SDL_WasInit
    /// @param flags any of the flags used by SDL_Init(); see SDL_Init for details.
    /// @return Returns a mask of all initialized subsystems if flags is 0, otherwise it returns the initialization status of the specified subsystems.
    Uint32 WasInit(Uint32 flags);

    /// @brief Use this function to write 16 bits in native format to a SDL_RWops as big-endian data. https://wiki.libsdl.org/SDL2/SDL_WriteBE16
    /// @param dst the stream to which data will be written
    /// @param value the data to be written, in native format
    /// @return Returns 1 on successful write, 0 on error.
    size_t WriteBE16(SDL_RWops * dst, Uint16 value);

    /// @brief Use this function to write 32 bits in native format to a SDL_RWops as big-endian data. https://wiki.libsdl.org/SDL2/SDL_WriteBE32
    /// @param dst the stream to which data will be written
    /// @param value the data to be written, in native format
    /// @return Returns 1 on successful write, 0 on error.
    size_t WriteBE32(SDL_RWops * dst, Uint32 value);

    /// @brief Use this function to write 64 bits in native format to a SDL_RWops as big-endian data. https://wiki.libsdl.org/SDL2/SDL_WriteBE64
    /// @param dst the stream to which data will be written
    /// @param value the data to be written, in native format
    /// @return Returns 1 on successful write, 0 on error.
    size_t WriteBE64(SDL_RWops * dst, Uint64 value);

    /// @brief Use this function to write 16 bits in native format to a SDL_RWops as little-endian data. https://wiki.libsdl.org/SDL2/SDL_WriteLE16
    /// @param dst the stream to which data will be written
    /// @param value the data to be written, in native format
    /// @return Returns 1 on successful write, 0 on error.
    size_t WriteLE16(SDL_RWops * dst, Uint16 value);

    /// @brief Use this function to write 32 bits in native format to a SDL_RWops as little-endian data. https://wiki.libsdl.org/SDL2/SDL_WriteLE32
    /// @param dst the stream to which data will be written
    /// @param value the data to be written, in native format
    /// @return Returns 1 on successful write, 0 on error.
    size_t WriteLE32(SDL_RWops * dst, Uint32 value);

    /// @brief Use this function to write 64 bits in native format to a SDL_RWops as little-endian data. https://wiki.libsdl.org/SDL2/SDL_WriteLE64
    /// @param dst the stream to which data will be written
    /// @param value the data to be written, in native format
    /// @return Returns 1 on successful write, 0 on error.
    size_t WriteLE64(SDL_RWops * dst, Uint64 value);

    /// @brief Use this function to write a byte to an SDL_RWops. https://wiki.libsdl.org/SDL2/SDL_WriteU8
    /// @param dst the SDL_RWops to write to
    /// @param value the byte value to write
    /// @return Returns 1 on success or 0 on failure; call SDL_GetError() for more information.
    size_t WriteU8(SDL_RWops * dst, Uint8 value);

    namespace Net {

        /// @brief Add a socket to a socket set, to be checked for available data. https://wiki.libsdl.org/SDL2_net/SDLNet_AddSocket
        /// @param set the socket set to add a new socket to.
        /// @param sock the socket to add to the set.
        /// @return Returns the total number of sockets contained in the set (including this new one), or -1 if the set is already full.
        int AddSocket(SDLNet_SocketSet set, SDLNet_GenericSocket sock);

        /// @brief Allocate a single UDP packet. https://wiki.libsdl.org/SDL2_net/SDLNet_AllocPacket
        /// @param size the maximum number of bytes of payload this packet will contain.
        /// @return Returns the new packet, or NULL if the function ran out of memory.
        UDPpacket * AllocPacket(int size);

        /// @brief Allocate a UDP packet vector (array of packets). https://wiki.libsdl.org/SDL2_net/SDLNet_AllocPacketV
        /// @param howmany the number of packets to allocate.
        /// @param size the maximum bytes of payload each packet should contain.
        /// @return Returns a pointer to the first packet in the array, or NULL if the function ran out of memory.
        UDPpacket ** AllocPacketV(int howmany, int size);

        /// @brief Allocate a socket set for use with SDLNet_CheckSockets(). https://wiki.libsdl.org/SDL2_net/SDLNet_AllocSocketSet
        /// @param maxsockets the maximum amount of sockets to include in this set.
        /// @return Returns a socket set for up to maxsockets sockets, or NULL if the function ran out of memory.
        SDLNet_SocketSet AllocSocketSet(int maxsockets);

        /// @brief Check a socket set for data availability. https://wiki.libsdl.org/SDL2_net/SDLNet_CheckSockets
        /// @param set the socket set to check for ready sockets.
        /// @param timeout the time to wait in milliseconds for new data to arrive. A timeout of zero checks for new data and returns without blocking.
        /// @return Returns the number of sockets ready for reading, or -1 if there was an error with the select() system call.
        int CheckSockets(SDLNet_SocketSet set, Uint32 timeout);

        /// @brief Remove a socket from a set of sockets to be checked for available data. https://wiki.libsdl.org/SDL2_net/SDLNet_DelSocket
        /// @param set the socket set to remove a socket from.
        /// @param sock the socket to remove from the set.
        /// @return Returns the total number of sockets contained in the set (after sock's removal), or -1 if sock was not in the set.
        int DelSocket(SDLNet_SocketSet set, SDLNet_GenericSocket sock);

        /// @brief Dispose of a UDP packet. https://wiki.libsdl.org/SDL2_net/SDLNet_FreePacket
        /// @param packet the packet to free.
        void FreePacket(UDPpacket *packet);

        /// @brief Free a UDP packet vector (array of packets). https://wiki.libsdl.org/SDL2_net/SDLNet_FreePacketV
        /// @param packetV the results of a call to SDLNet_AllocPacketV().
        void FreePacketV(UDPpacket **packetV);

        /// @brief Free a set of sockets allocated by SDLNet_AllocSocketSet(). https://wiki.libsdl.org/SDL2_net/SDLNet_FreeSocketSet
        /// @param set the socket set to free.
        void FreeSocketSet(SDLNet_SocketSet set);

        /// @brief Get the latest error message from SDL_net. https://wiki.libsdl.org/SDL2_net/SDLNet_GetError
        /// @return Returns the last set error message in UTF-8 encoding.
        const char * GetError();

        /// @brief Get the addresses of network interfaces on this system. https://wiki.libsdl.org/SDL2_net/SDLNet_GetLocalAddresses
        /// @param addresses where to store the returned information.
        /// @param maxcount the number of results that can be stored at addresses
        /// @return Returns the number of addresses saved in addresses
        int GetLocalAddresses(IPaddress *addresses, int maxcount);

        /// @brief Initialize SDL_net. https://wiki.libsdl.org/SDL2_net/SDLNet_Init
        /// @return Returns 0 on success, -1 on error.
        int Init();

        /// @brief Query the version of SDL_net that the program is linked against. https://wiki.libsdl.org/SDL2_net/SDLNet_Linked_Version
        /// @return Returns a pointer to the version information.
        const SDLNet_version * Linked_Version();

        /// @brief Deinitialize SDL_net. https://wiki.libsdl.org/SDL2_net/SDLNet_Quit
        void Quit();

        /// @brief Reallocate a UDP packet's payload space. https://wiki.libsdl.org/SDL2_net/SDLNet_ResizePacket
        /// @param newsize the new maximum number of bytes of payload this packet will contain.
        /// @return Returns the new maximum payload size, which will be unchanged from the previous if the system ran out of memory.
        int ResizePacket(UDPpacket *packet, int newsize);

        /// @brief Resolve a host name and port to an IP address in network form. https://wiki.libsdl.org/SDL2_net/SDLNet_ResolveHost
        /// @param address to be filled in with the resolved address and port.
        /// @param host the hostname to lookup (like "libsdl.org")
        /// @param port the port intended to be connected to, to fill into address.
        /// @return Returns zero on success, -1 on error.
        int ResolveHost(IPaddress *address, const char *host, uint16_t port);

        /// @brief Resolve an IP address to a host name in canonical form. https://wiki.libsdl.org/SDL2_net/SDLNet_ResolveIP
        /// @param ip the IP address to resolve into a hostname.
        const char * ResolveIP(const IPaddress *ip);

        /// @brief Accept an incoming connection on the given server socket. https://wiki.libsdl.org/SDL2_net/SDLNet_TCP_Accept
        /// @param server the server socket to accept a connection on.
        /// @return Returns the newly created socket, or NULL if there was an error.
        TCPsocket TCP_Accept(TCPsocket server);

        /// @brief Close a TCP network socket. https://wiki.libsdl.org/SDL2_net/SDLNet_TCP_Close
        /// @param sock socket to close.
        void TCP_Close(TCPsocket sock);

        /// @brief Get the IP address of the remote system associated with the socket. https://wiki.libsdl.org/SDL2_net/SDLNet_TCP_GetPeerAddress
        /// @param sock the socket to query.
        /// @return Returns the address information for the socket.
        IPaddress * TCP_GetPeerAddress(TCPsocket sock);

        /// @brief Open a TCP network socket. https://wiki.libsdl.org/SDL2_net/SDLNet_TCP_Open
        /// @param ip The address to open a connection to (or to host a server on).
        /// @return Returns the newly created socket, or NULL if there was an error.
        TCPsocket TCP_Open(IPaddress *ip);

        /// @brief Receive data from a non-server socket. https://wiki.libsdl.org/SDL2_net/SDLNet_TCP_Recv
        /// @param sock the socket to send data to.
        /// @param data a pointer to where to store received data.
        /// @param maxlen the maximum number of bytes that can be stored at data.
        /// @return Returns number of bytes received, which might be less than maxlen.
        int TCP_Recv(TCPsocket sock, void *data, int maxlen);

        /// @brief Send data over a non-server socket. https://wiki.libsdl.org/SDL2_net/SDLNet_TCP_Send
        /// @param sock the socket to send data to.
        /// @param data a pointer to the bytes to send.
        /// @param len the number of bytes, pointed to by data, to transmit.
        /// @return Returns number of bytes sent, which might be less if there was a problem or connection failure. If the socket is invalid, this function can return -1, but in valid uses it'll return >= 0.
        int TCP_Send(TCPsocket sock, void *data, int len);

        /// @brief Bind an address to the requested channel on the UDP socket. https://wiki.libsdl.org/SDL2_net/SDLNet_UDP_Bind
        /// @param sock the UDP socket to bind an address to a channel on.
        /// @param channel the channel of the socket to bind to, or -1 to use the first available channel.
        /// @param address the address to bind to the socket's channel.
        /// @return Returns the channel which was bound, or -1 on error.
        int UDP_Bind(UDPsocket sock, int channel, const IPaddress *address);

        /// @brief Close a UDP socket. https://wiki.libsdl.org/SDL2_net/SDLNet_UDP_Close
        /// @param sock UDP socket to close.
        void UDP_Close(UDPsocket sock);

        /// @brief Get the IP address of the remote system for a socket and channel. https://wiki.libsdl.org/SDL2_net/SDLNet_UDP_GetPeerAddress
        /// @param sock the UDP socket to unbind addresses from a channel on.
        /// @param channel the channel of the socket to unbind.
        /// @return Returns the address bound to the socket's channel, or
        IPaddress * UDP_GetPeerAddress(UDPsocket sock, int channel);

        /// @brief Open a UDP network socket. https://wiki.libsdl.org/SDL2_net/SDLNet_UDP_Open
        /// @param port the UDP port to bind this socket to.
        /// @return Returns a new UDP socket, ready to communicate.
        UDPsocket UDP_Open(uint16_t port);

        /// @brief Receive a single packet from a UDP socket. https://wiki.libsdl.org/SDL2_net/SDLNet_UDP_Recv
        /// @param sock the UDP socket to receive packets on.
        /// @param packet a single packet to receive data into from the network.
        /// @return Returns 1 if a new packet is available, or -1 on error. 0 means no packets were currently available.
        int UDP_Recv(UDPsocket sock, UDPpacket *packet);

        /// @brief Receive a vector of pending packets from a UDP socket. https://wiki.libsdl.org/SDL2_net/SDLNet_UDP_RecvV
        /// @param sock the UDP socket to receive packets on.
        /// @param packets an array of packets, NULL terminated.
        /// @return Returns the number of packets read from the network, or -1 on error. 0 means no packets were currently available.
        int UDP_RecvV(UDPsocket sock, UDPpacket **packets);

        /// @brief Send a single UDP packet to the specified channel. https://wiki.libsdl.org/SDL2_net/SDLNet_UDP_Send
        /// @param sock the UDP socket to send packets on.
        /// @param packet a single packet to send to the network.
        /// @return Returns 1 if the packet was sent, or 0 on error.
        int UDP_Send(UDPsocket sock, int channel, UDPpacket *packet);

        /// @brief Send a vector of packets to the the channels specified within the packet. https://wiki.libsdl.org/SDL2_net/SDLNet_UDP_SendV
        /// @param sock the UDP socket to send packets on.
        /// @param packets an array of packets to send to the network.
        /// @param npackets the number of packets in the packets array.
        /// @return Returns the number of packets successfully sent from this machine.
        int UDP_SendV(UDPsocket sock, UDPpacket **packets, int npackets);

        /// @brief Set the percentage of simulated packet loss for packets sent on the socket. https://wiki.libsdl.org/SDL2_net/SDLNet_UDP_SetPacketLoss
        /// @param sock the socket to simulate packet loss on.
        /// @param percent a value from 0 to 100 of likelihood to drop a packet (higher the number means more likelihood of dropping.
        void UDP_SetPacketLoss(UDPsocket sock, int percent);

        /// @brief Unbind all addresses from the given channel. https://wiki.libsdl.org/SDL2_net/SDLNet_UDP_Unbind
        /// @param sock the UDP socket to unbind addresses from a channel on.
        /// @param channel the channel of the socket to unbind.
        void UDP_Unbind(UDPsocket sock, int channel);

    }

    namespace Mixer {

        /// @brief Dynamically change the number of channels managed by the mixer. https://wiki.libsdl.org/SDL2_mixer/Mix_AllocateChannels
        /// @param numchans the new number of channels, or < 0 to query current channel count.
        /// @return Returns the new number of allocated channels.
        int AllocateChannels(int numchans);

        /// @brief Set a callback that runs when a channel has finished playing. https://wiki.libsdl.org/SDL2_mixer/Mix_ChannelFinished
        /// @param channel_finished the callback function to become the new notification mechanism.
        void ChannelFinished(void (*channel_finished)(int channel));

        /// @brief Close the mixer, halting all playing audio. https://wiki.libsdl.org/SDL2_mixer/Mix_CloseAudio
        void CloseAudio();

        /// @brief Iterate SoundFonts paths to use by supported MIDI backends. https://wiki.libsdl.org/SDL2_mixer/Mix_EachSoundFont
        /// @param function the callback function to call once per path.
        /// @param data a pointer to pass to the callback for its own personal use.
        /// @return Returns non-zero if callback ever returned non-zero, 0 on error or the callback never returned non-zero.
        int EachSoundFont(int (*function)(const char*, void*), void *data);

        /// @brief Change the expiration delay for a particular channel. https://wiki.libsdl.org/SDL2_mixer/Mix_ExpireChannel
        /// @param channel the channel to change the expiration time on.
        /// @param ticks number of milliseconds from now to let channel play before halting, -1 to not halt.
        /// @return Returns the number of channels that changed expirations.
        int ExpireChannel(int channel, int ticks);

        /// @brief Play an audio chunk on a specific channel, fading in the audio. https://wiki.libsdl.org/SDL2_mixer/Mix_FadeInChannel
        /// @param channel the channel on which to play the new chunk, or -1 to find any available.
        /// @param chunk the new chunk to play.
        /// @param loops the number of times the chunk should loop, -1 to loop (not actually) infinitely.
        /// @param ms the number of milliseconds to spend fading in.
        /// @return Returns which channel was used to play the sound, or -1 if sound could not be played.
        int FadeInChannel(int channel, Mix_Chunk *chunk, int loops, int ms);

        /// @brief Play an audio chunk on a specific channel, fading in the audio, for a maximum time. https://wiki.libsdl.org/SDL2_mixer/Mix_FadeInChannelTimed
        /// @param channel the channel on which to play the new chunk, or -1 to find any available.
        /// @param chunk the new chunk to play.
        /// @param loops the number of times the chunk should loop, -1 to loop (not actually) infinitely.
        /// @param ms the number of milliseconds to spend fading in.
        /// @param ticks the maximum number of milliseconds of this chunk to mix for playback.
        /// @return Returns which channel was used to play the sound, or -1 if sound could not be played.
        int FadeInChannelTimed(int channel, Mix_Chunk *chunk, int loops, int ms, int ticks);

        /// @brief Play a new music object, fading in the audio. https://wiki.libsdl.org/SDL2_mixer/Mix_FadeInMusic
        /// @param music the new music object to play.
        /// @param loops the number of times the chunk should loop, -1 to loop (not actually) infinitely.
        /// @param ms the number of milliseconds to spend fading in.
        /// @return Returns zero on success, -1 on error.
        int FadeInMusic(Mix_Music *music, int loops, int ms);

        /// @brief Play a new music object, fading in the audio, from a starting position. https://wiki.libsdl.org/SDL2_mixer/Mix_FadeInMusicPos
        /// @param music the new music object to play.
        /// @param loops the number of times the chunk should loop, -1 to loop (not actually) infinitely.
        /// @param ms the number of milliseconds to spend fading in.
        /// @param position the start position within the music, in seconds, where playback should start.
        /// @return Returns zero on success, -1 on error.
        int FadeInMusicPos(Mix_Music *music, int loops, int ms, double position);

        /// @brief Halt a channel after fading it out for a specified time. https://wiki.libsdl.org/SDL2_mixer/Mix_FadeOutChannel
        /// @param which the channel to fade out.
        /// @param ms number of milliseconds to fade before halting the channel.
        /// @return Returns the number of channels scheduled to fade.
        int FadeOutChannel(int which, int ms);

        /// @brief Halt a playing group of channels by arbitrary tag, after fading them out for a specified time. https://wiki.libsdl.org/SDL2_mixer/Mix_FadeOutGroup
        /// @param tag an arbitrary value, assigned to channels, to search for.
        /// @param ms number of milliseconds to fade before halting the group.
        /// @return Returns the number of channels that were scheduled for fading.
        int FadeOutGroup(int tag, int ms);

        /// @brief Halt the music stream after fading it out for a specified time. https://wiki.libsdl.org/SDL2_mixer/Mix_FadeOutMusic
        /// @param ms number of milliseconds to fade before halting the channel.
        /// @return Returns non-zero if music was scheduled to fade, zero otherwise. If no music is currently playing, this returns zero.
        int FadeOutMusic(int ms);

        /// @brief Query the fading status of a channel. https://wiki.libsdl.org/SDL2_mixer/Mix_FadingChannel
        /// @param which the channel to query.
        /// @return Returns the current fading status of the channel.
        Mix_Fading FadingChannel(int which);

        /// @brief Query the fading status of the music stream. https://wiki.libsdl.org/SDL2_mixer/Mix_FadingMusic
        /// @return Returns the current fading status of the music stream.
        Mix_Fading FadingMusic();

        /// @brief Free an audio chunk. https://wiki.libsdl.org/SDL2_mixer/Mix_FreeChunk
        /// @param chunk the chunk to free.
        void FreeChunk(Mix_Chunk *chunk);

        /// @brief Free a music object. https://wiki.libsdl.org/SDL2_mixer/Mix_FreeMusic
        /// @param music the music object to free.
        void FreeMusic(Mix_Music *music);

        /// @brief Get the Mix_Chunk currently associated with a mixer channel. https://wiki.libsdl.org/SDL2_mixer/Mix_GetChunk
        /// @param channel the channel to query.
        /// @return Returns the associated chunk, if any, or NULL if it's an invalid channel.
        Mix_Chunk * GetChunk(int channel);

        /// @brief Get a chunk decoder's name. https://wiki.libsdl.org/SDL2_mixer/Mix_GetChunkDecoder
        /// @param index index of the chunk decoder.
        /// @return Returns the chunk decoder's name.
        const char * GetChunkDecoder(int index);

        /// @brief Get the album name for a music object. https://wiki.libsdl.org/SDL2_mixer/Mix_GetMusicAlbumTag
        /// @param music the music object to query, or NULL for the currently-playing music.
        /// @return Returns the music's album name if available, or "".
        const char* GetMusicAlbumTag(const Mix_Music *music);

        /// @brief Get the artist name for a music object. https://wiki.libsdl.org/SDL2_mixer/Mix_GetMusicArtistTag
        /// @param music the music object to query, or NULL for the currently-playing music.
        /// @return Returns the music's artist name if available, or "".
        const char* GetMusicArtistTag(const Mix_Music *music);

        /// @brief Get the copyright text for a music object. https://wiki.libsdl.org/SDL2_mixer/Mix_GetMusicCopyrightTag
        /// @param music the music object to query, or NULL for the currently-playing music.
        /// @return Returns the music's copyright text if available, or "".
        const char* GetMusicCopyrightTag(const Mix_Music *music);

        /// @brief Get a music decoder's name. https://wiki.libsdl.org/SDL2_mixer/Mix_GetMusicDecoder
        /// @param index index of the music decoder.
        /// @return Returns the music decoder's name.
        const char * GetMusicDecoder(int index);

        /// @brief Get a pointer to the user data for the current music hook. https://wiki.libsdl.org/SDL2_mixer/Mix_GetMusicHookData
        /// @return Returns pointer to the user data previously passed to Mix_HookMusic.
        void * GetMusicHookData();

        /// @brief Get the loop end time position of music stream, in seconds. https://wiki.libsdl.org/SDL2_mixer/Mix_GetMusicLoopEndTime
        /// @param music the music object to query.
        /// @return Returns -1.0 if this feature is not used for this music or not supported for some codec
        double GetMusicLoopEndTime(Mix_Music *music);

        /// @brief Get the loop time length of music stream, in seconds. https://wiki.libsdl.org/SDL2_mixer/Mix_GetMusicLoopLengthTime
        /// @param music the music object to query.
        /// @return Returns -1.0 if this feature is not used for this music or not supported for some codec
        double GetMusicLoopLengthTime(Mix_Music *music);

        /// @brief Get the loop start time position of music stream, in seconds. https://wiki.libsdl.org/SDL2_mixer/Mix_GetMusicLoopStartTime
        /// @param music the music object to query.
        /// @return Returns -1.0 if this feature is not used for this music or not supported for some codec
        double GetMusicLoopStartTime(Mix_Music *music);

        /// @brief Get the time current position of music stream, in seconds. https://wiki.libsdl.org/SDL2_mixer/Mix_GetMusicPosition
        /// @param music the music object to query.
        /// @return Returns -1.0 if this feature is not supported for some codec.
        double GetMusicPosition(Mix_Music *music);

        /// @brief Get the title for a music object, or its filename. https://wiki.libsdl.org/SDL2_mixer/Mix_GetMusicTitle
        /// @param music the music object to query, or NULL for the currently-playing music.
        /// @return Returns the music's title if available, or the filename if not, or "".
        const char* GetMusicTitle(const Mix_Music *music);

        /// @brief Get the title for a music object. https://wiki.libsdl.org/SDL2_mixer/Mix_GetMusicTitleTag
        /// @param music the music object to query, or NULL for the currently-playing music.
        /// @return Returns the music's title if available, or "".
        const char* GetMusicTitleTag(const Mix_Music *music);

        /// @brief Find out the format of a mixer music. https://wiki.libsdl.org/SDL2_mixer/Mix_GetMusicType
        /// @param music the music object to query, or NULL for the currently-playing music.
        /// @return Returns the Mix_MusicType for the music object.
        Mix_MusicType GetMusicType(const Mix_Music *music);

        /// @brief Query the current volume value for a music object. https://wiki.libsdl.org/SDL2_mixer/Mix_GetMusicVolume
        /// @param music the music object to query.
        /// @return Returns the music's current volume, between 0 and MIX_MAX_VOLUME (128).
        int GetMusicVolume(Mix_Music *music);

        /// @brief Get a list of chunk decoders that this build of SDL_mixer provides. https://wiki.libsdl.org/SDL2_mixer/Mix_GetNumChunkDecoders
        /// @return Returns number of chunk decoders available.
        int GetNumChunkDecoders();

        /// @brief Get a list of music decoders that this build of SDL_mixer provides. https://wiki.libsdl.org/SDL2_mixer/Mix_GetNumMusicDecoders
        /// @return Returns number of music decoders available.
        int GetNumMusicDecoders();

        /// @brief Get SoundFonts paths to use by supported MIDI backends. https://wiki.libsdl.org/SDL2_mixer/Mix_GetSoundFonts
        /// @return Returns semicolon-separated list of sound font paths.
        const char* GetSoundFonts();

        /// @brief This function does nothing, do not use. https://wiki.libsdl.org/SDL2_mixer/Mix_GetSynchroValue
        /// @return Returns -1.
        int GetSynchroValue();

        /// @brief Get full path of a previously-specified Timidity config file. https://wiki.libsdl.org/SDL2_mixer/Mix_GetTimidityCfg
        /// @return Returns the previously-specified path, or NULL if not set.
        const char* GetTimidityCfg();

        /// @brief Finds the first available channel in a group of channels. https://wiki.libsdl.org/SDL2_mixer/Mix_GroupAvailable
        /// @param tag an arbitrary value, assigned to channels, to search for.
        /// @return Returns first available channel, or -1 if none are available.
        int GroupAvailable(int tag);

        /// @brief Assign a tag to a channel. https://wiki.libsdl.org/SDL2_mixer/Mix_GroupChannel
        /// @param which the channel to set the tag on.
        /// @param tag an arbitrary value to assign a channel.
        /// @return Returns non-zero on success, zero on error (no such channel).
        int GroupChannel(int which, int tag);

        /// @brief Assign several consecutive channels to the same tag. https://wiki.libsdl.org/SDL2_mixer/Mix_GroupChannels
        /// @param from the first channel to set the tag on.
        /// @param to the last channel to set the tag on, inclusive.
        /// @param tag an arbitrary value to assign a channel.
        /// @return Returns 0 if successful, negative on error
        int GroupChannels(int from, int to, int tag);

        /// @brief Returns the number of channels in a group. https://wiki.libsdl.org/SDL2_mixer/Mix_GroupCount
        /// @param tag an arbitrary value, assigned to channels, to search for.
        /// @return Returns the number of channels assigned the specified tag.
        int GroupCount(int tag);

        /// @brief Find the "most recent" sample playing in a group of channels. https://wiki.libsdl.org/SDL2_mixer/Mix_GroupNewer
        /// @param tag an arbitrary value, assigned to channels, to search through.
        /// @return Returns the "most recent" sample playing in a group of channels
        int GroupNewer(int tag);

        /// @brief Find the "oldest" sample playing in a group of channels. https://wiki.libsdl.org/SDL2_mixer/Mix_GroupOldest
        /// @param tag an arbitrary value, assigned to channels, to search through.
        /// @return Returns the "oldest" sample playing in a group of channels
        int GroupOldest(int tag);

        /// @brief Halt playing of a particular channel. https://wiki.libsdl.org/SDL2_mixer/Mix_HaltChannel
        /// @param channel channel to halt, or -1 to halt all channels.
        /// @return Returns 0 on success, or -1 on error.
        int HaltChannel(int channel);

        /// @brief Halt playing of a group of channels by arbitrary tag. https://wiki.libsdl.org/SDL2_mixer/Mix_HaltGroup
        /// @param tag an arbitrary value, assigned to channels, to search for.
        /// @return Returns zero, whether any channels were halted or not.
        int HaltGroup(int tag);

        /// @brief Halt playing of the music stream. https://wiki.libsdl.org/SDL2_mixer/Mix_HaltMusic
        /// @return Returns zero, regardless of whether any music was halted.
        int HaltMusic();

        /// @brief Check if a chunk decoder is available by name. https://wiki.libsdl.org/SDL2_mixer/Mix_HasChunkDecoder
        /// @param name the decoder name to query.
        /// @return Returns SDL_TRUE if a decoder by that name is available, SDL_FALSE otherwise.
        bool HasChunkDecoder(const char *name);

        /// @brief Check if a music decoder is available by name. https://wiki.libsdl.org/SDL2_mixer/Mix_HasMusicDecoder
        /// @param name the decoder name to query.
        /// @return Returns SDL_TRUE if a decoder by that name is available, SDL_FALSE otherwise.
        bool HasMusicDecoder(const char *name);

        /// @brief Add your own music player or additional mixer function. https://wiki.libsdl.org/SDL2_mixer/Mix_HookMusic
        /// @param mix_func the callback function to become the new post-mix callback.
        /// @param arg a pointer that is passed, untouched, to the callback.
        void HookMusic(void ( *mix_func)(void *udata, Uint8 *stream, int len), void *arg);

        /// @brief Set a callback that runs when a music object has stopped playing. https://wiki.libsdl.org/SDL2_mixer/Mix_HookMusicFinished
        /// @param music_finished the callback function to become the new notification mechanism.
        void HookMusicFinished(void ( *music_finished)(void));

        /// @brief Initialize SDL_mixer. https://wiki.libsdl.org/SDL2_mixer/Mix_Init
        /// @param flags initialization flags, OR'd together.
        /// @return Returns all currently initialized flags.
        int Init(int flags);

        /// @brief Query the version of SDL_mixer that the program is linked against. https://wiki.libsdl.org/SDL2_mixer/Mix_Linked_Version
        /// @return Returns a pointer to the version information.
        const SDL_version * Linked_Version();

        /// @brief Load a supported audio format into a music object. https://wiki.libsdl.org/SDL2_mixer/Mix_LoadMUS
        /// @param file a file path from where to load music data.
        /// @return Returns a new music object, or NULL on error.
        Mix_Music * LoadMUS(const char *file);

        /// @brief Load a supported audio format into a music object. https://wiki.libsdl.org/SDL2_mixer/Mix_LoadMUS_RW
        /// @param src an SDL_RWops that data will be read from.
        /// @param freesrc non-zero to close/free the SDL_RWops before returning, zero to leave it open.
        /// @return Returns a new music object, or NULL on error.
        Mix_Music * LoadMUS_RW(SDL_RWops *src, int freesrc);

        /// @brief Load an audio format into a music object, assuming a specific format. https://wiki.libsdl.org/SDL2_mixer/Mix_LoadMUSType_RW
        /// @param src an SDL_RWops that data will be read from.
        /// @param type the type of audio data provided by src.
        /// @param freesrc non-zero to close/free the SDL_RWops before returning, zero to leave it open.
        /// @return Returns a new music object, or NULL on error.
        Mix_Music * LoadMUSType_RW(SDL_RWops *src, Mix_MusicType type, int freesrc);

        /// @brief Load a supported audio format into a chunk. https://wiki.libsdl.org/SDL2_mixer/Mix_LoadWAV
        /// @param file the filesystem path to load data from.
        /// @return Returns a new chunk, or NULL on error.
        Mix_Chunk * LoadWAV(const char *file);

        /// @brief Load a supported audio format into a chunk. https://wiki.libsdl.org/SDL2_mixer/Mix_LoadWAV_RW
        /// @param src an SDL_RWops that data will be read from.
        /// @param freesrc non-zero to close/free the SDL_RWops before returning, zero to leave it open.
        /// @return Returns a new chunk, or NULL on error.
        Mix_Chunk * LoadWAV_RW(SDL_RWops *src, int freesrc);

        /// @brief Set the master volume for all channels. https://wiki.libsdl.org/SDL2_mixer/Mix_MasterVolume
        /// @param volume the new volume, between 0 and MIX_MAX_VOLUME, or -1 to query.
        /// @return Returns the previous volume. If the specified volume is -1, this returns the current volume.
        int MasterVolume(int volume);

        /// @brief Jump to a given order in mod music. https://wiki.libsdl.org/SDL2_mixer/Mix_ModMusicJumpToOrder
        /// @param order order
        /// @return Returns 0 if successful, or -1 if failed or isn't implemented.
        int ModMusicJumpToOrder(int order);

        /// @brief Get a music object's duration, in seconds. https://wiki.libsdl.org/SDL2_mixer/Mix_MusicDuration
        /// @param music the music object to query.
        /// @return Returns music duration in seconds, or -1.0 on error.
        double MusicDuration(Mix_Music *music);

        /// @brief Open the default audio device for playback. https://wiki.libsdl.org/SDL2_mixer/Mix_OpenAudio
        /// @param frequency the frequency to playback audio at (in Hz).
        /// @param format audio format, one of SDL's AUDIO_* values.
        /// @param channels number of channels (1 is mono, 2 is stereo, etc).
        /// @param chunksize audio buffer size in sample FRAMES (total samples divided by channel count).
        /// @return Returns 0 if successful, -1 on error.
        int OpenAudio(int frequency, uint16_t format, int channels, int chunksize);

        /// @brief Open a specific audio device for playback. https://wiki.libsdl.org/SDL2_mixer/Mix_OpenAudioDevice
        /// @param frequency the frequency to playback audio at (in Hz).
        /// @param format audio format, one of SDL's AUDIO_* values.
        /// @param channels number of channels (1 is mono, 2 is stereo, etc).
        /// @param chunksize audio buffer size in sample FRAMES (total samples divided by channel count).
        /// @param device the device name to open, or NULL to choose a reasonable default.
        /// @param allowed_changes Allow change flags (see SDL_AUDIO_ALLOW_* flags)
        /// @return Returns 0 if successful, -1 on error.
        int OpenAudioDevice(int frequency, uint16_t format, int channels, int chunksize, const char* device, int allowed_changes);

        /// @brief Pause a particular channel. https://wiki.libsdl.org/SDL2_mixer/Mix_Pause
        /// @param channel the channel to pause, or -1 to pause all channels.
        void Pause(int channel);

        /// @brief Query whether a particular channel is paused. https://wiki.libsdl.org/SDL2_mixer/Mix_Paused
        /// @param channel the channel to query, or -1 to query all channels.
        /// @return Return 1 if channel paused, 0 otherwise. If channel is -1, returns the number of paused channels.
        int Paused(int channel);

        /// @brief Query whether the music stream is paused. https://wiki.libsdl.org/SDL2_mixer/Mix_PausedMusic
        /// @return Return 1 if music is paused, 0 otherwise.
        int PausedMusic();

        /// @brief Pause the music stream. https://wiki.libsdl.org/SDL2_mixer/Mix_PauseMusic
        void PauseMusic();

        /// @brief Play an audio chunk on a specific channel. https://wiki.libsdl.org/SDL2_mixer/Mix_PlayChannel
        /// @param channel the channel on which to play the new chunk.
        /// @param chunk the new chunk to play.
        /// @param loops the number of times the chunk should loop, -1 to loop (not actually) infinitely.
        /// @return Returns which channel was used to play the sound, or -1 if sound could not be played.
        int PlayChannel(int channel, Mix_Chunk *chunk, int loops);

        /// @brief Play an audio chunk on a specific channel for a maximum time. https://wiki.libsdl.org/SDL2_mixer/Mix_PlayChannelTimed
        /// @param channel the channel on which to play the new chunk.
        /// @param chunk the new chunk to play.
        /// @param loops the number of times the chunk should loop, -1 to loop (not actually) infinitely.
        /// @param ticks the maximum number of milliseconds of this chunk to mix for playback.
        /// @return Returns which channel was used to play the sound, or -1 if sound could not be played.
        int PlayChannelTimed(int channel, Mix_Chunk *chunk, int loops, int ticks);

        /// @brief Check the playing status of a specific channel. https://wiki.libsdl.org/SDL2_mixer/Mix_Playing
        /// @param channel channel
        /// @return Returns non-zero if channel is playing, zero otherwise. If channel is -1, return the total number of channel playings.
        int Playing(int channel);

        /// @brief Check the playing status of the music stream. https://wiki.libsdl.org/SDL2_mixer/Mix_PlayingMusic
        /// @return Returns non-zero if music is playing, zero otherwise.
        int PlayingMusic();

        /// @brief Play a new music object. https://wiki.libsdl.org/SDL2_mixer/Mix_PlayMusic
        /// @param music the new music object to schedule for mixing.
        /// @param loops the number of loops to play the music for (0 means "play once and stop").
        /// @return Returns zero on success, -1 on error.
        int PlayMusic(Mix_Music *music, int loops);

        /// @brief Find out what the actual audio device parameters are. https://wiki.libsdl.org/SDL2_mixer/Mix_QuerySpec
        /// @param frequency On return, will be filled with the audio device's frequency in Hz.
        /// @param format On return, will be filled with the audio device's format.
        /// @param channels On return, will be filled with the audio device's channel count.
        /// @return Returns 1 if the audio device has been opened, 0 otherwise.
        int QuerySpec(int *frequency, uint16_t *format, int *channels);

        /// @brief Load a raw audio data from memory as quickly as possible. https://wiki.libsdl.org/SDL2_mixer/Mix_QuickLoad_RAW
        /// @param mem memory buffer containing raw PCM data.
        /// @param len length of buffer pointed to by mem, in bytes.
        /// @return Returns a new chunk, or NULL on error.
        Mix_Chunk * QuickLoad_RAW(Uint8 *mem, Uint32 len);

        /// @brief Load a WAV file from memory as quickly as possible. https://wiki.libsdl.org/SDL2_mixer/Mix_QuickLoad_WAV
        /// @param mem memory buffer containing of a WAV file.
        /// @return Returns a new chunk, or NULL on error.
        Mix_Chunk * QuickLoad_WAV(Uint8 *mem);

        /// @brief Deinitialize SDL_mixer. https://wiki.libsdl.org/SDL2_mixer/Mix_Quit
        void Quit();

        /// @brief Register a special effect function. https://wiki.libsdl.org/SDL2_mixer/Mix_RegisterEffect
        /// @param chan the channel to register an effect to, or MIX_CHANNEL_POST.
        /// @param f effect the callback to run when more of this channel is to be mixed.
        /// @param d effect done callback
        /// @param arg argument
        /// @return Returns zero if error (no such channel), nonzero if added. Error messages can be retrieved from Mix_GetError().
        int RegisterEffect(int chan, Mix_EffectFunc_t f, Mix_EffectDone_t d, void *arg);

        /// @brief Reserve the first channels for the application. https://wiki.libsdl.org/SDL2_mixer/Mix_ReserveChannels
        /// @param num number of channels to reserve, starting at index zero.
        /// @return Returns the number of reserved channels.
        int ReserveChannels(int num);

        /// @brief Resume a particular channel. https://wiki.libsdl.org/SDL2_mixer/Mix_Resume
        /// @param channel the channel to resume, or -1 to resume all paused channels.
        void Resume(int channel);

        /// @brief Resume the music stream. https://wiki.libsdl.org/SDL2_mixer/Mix_ResumeMusic
        void ResumeMusic();

        /// @brief Rewind the music stream. https://wiki.libsdl.org/SDL2_mixer/Mix_RewindMusic
        void RewindMusic();

        /// @brief Set the "distance" of a channel. https://wiki.libsdl.org/SDL2_mixer/Mix_SetDistance
        /// @param channel The mixer channel to attenuate, or MIX_CHANNEL_POST.
        /// @param distance distance; 0 is the listener, 255 is maxiumum distance away.
        /// @return Returns zero if error (no such channel or Mix_RegisterEffect() fails), nonzero if position effect is enabled. Error messages can be retrieved from Mix_GetError().
        int SetDistance(int channel, Uint8 distance);

        /// @brief Run an external command as the music stream. https://wiki.libsdl.org/SDL2_mixer/Mix_SetMusicCMD
        /// @param command command
        /// @return Returns 0 if successful, -1 on error
        int SetMusicCMD(const char *command);

        /// @brief Set the current position in the music stream, in seconds. https://wiki.libsdl.org/SDL2_mixer/Mix_SetMusicPosition
        /// @param position the new position, in seconds (as a double).
        /// @return Returns 0 if successful, or -1 if it failed or not implemented.
        int SetMusicPosition(double position);

        /// @brief Set the panning of a channel. https://wiki.libsdl.org/SDL2_mixer/Mix_SetPanning
        /// @param channel The mixer channel to pan or MIX_CHANNEL_POST.
        /// @param left Volume of stereo left channel, 0 is silence, 255 is full volume.
        /// @param right Volume of stereo right channel, 0 is silence, 255 is full volume.
        /// @return Returns zero if error (no such channel or Mix_RegisterEffect() fails), nonzero if panning effect enabled.
        int SetPanning(int channel, Uint8 left, Uint8 right);

        /// @brief Set the position of a channel. https://wiki.libsdl.org/SDL2_mixer/Mix_SetPosition
        /// @param channel The mixer channel to position, or MIX_CHANNEL_POST.
        /// @param angle angle, in degrees. North is 0, and goes clockwise.
        /// @param distance distance; 0 is the listener, 255 is maxiumum distance away.
        /// @return Returns zero if error (no such channel or Mix_RegisterEffect() fails), nonzero if position effect is enabled. Error messages can be retrieved from Mix_GetError().
        int SetPosition(int channel, Sint16 angle, Uint8 distance);

        /// @brief Set a function that is called after all mixing is performed. https://wiki.libsdl.org/SDL2_mixer/Mix_SetPostMix
        /// @param mix_func the callback function to become the new post-mix callback.
        /// @param arg a pointer that is passed, untouched, to the callback.
        void SetPostMix(void (*mix_func)(void *udata, Uint8 *stream, int len), void *arg);

        /// @brief Cause a channel to reverse its stereo. https://wiki.libsdl.org/SDL2_mixer/Mix_SetReverseStereo
        /// @param channel The mixer channel to reverse, or MIX_CHANNEL_POST.
        /// @param flip non-zero to reverse stereo, zero to disable this effect.
        /// @return Returns zero if error (no such channel or Mix_RegisterEffect() fails), nonzero if reversing effect is enabled. Note that an audio device in mono mode is a no-op, but this call will return successful in that case. Error messages can be retrieved from Mix_GetError().
        int SetReverseStereo(int channel, int flip);

        /// @brief Set SoundFonts paths to use by supported MIDI backends. https://wiki.libsdl.org/SDL2_mixer/Mix_SetSoundFonts
        /// @param paths Paths on the filesystem where SoundFonts are available, separated by semicolons.
        /// @return Returns 1 if successful, 0 on error (out of memory).
        int SetSoundFonts(const char *paths);

        /// @brief This function does nothing, do not use. https://wiki.libsdl.org/SDL2_mixer/Mix_SetSynchroValue
        /// @param value this parameter is ignored.
        /// @return Returns -1.
        int SetSynchroValue(int value);

        /// @brief Set full path of the Timidity config file. https://wiki.libsdl.org/SDL2_mixer/Mix_SetTimidityCfg
        /// @param path path to a Timidity config file.
        /// @return Returns 1 if successful, 0 on error
        int SetTimidityCfg(const char *path);

        /// @brief Explicitly unregister all special effect functions. https://wiki.libsdl.org/SDL2_mixer/Mix_UnregisterAllEffects
        /// @param channel the channel to unregister all effects on, or MIX_CHANNEL_POST.
        /// @return Returns zero if error (no such channel), nonzero if all effects removed. Error messages can be retrieved from Mix_GetError().
        int UnregisterAllEffects(int channel);

        /// @brief Explicitly unregister a special effect function. https://wiki.libsdl.org/SDL2_mixer/Mix_UnregisterEffect
        /// @param channel the channel to unregister an effect on, or MIX_CHANNEL_POST.
        /// @param f effect the callback stop calling in future mixing iterations.
        /// @return Returns zero if error (no such channel or effect), nonzero if removed. Error messages can be retrieved from Mix_GetError().
        int UnregisterEffect(int channel, Mix_EffectFunc_t f);

        /// @brief Set the volume for a specific channel. https://wiki.libsdl.org/SDL2_mixer/Mix_Volume
        /// @param channel the channel on set/query the volume on, or -1 for all channels.
        /// @param volume the new volume, between 0 and MIX_MAX_VOLUME, or -1 to query.
        /// @return Returns the previous volume. If the specified volume is -1, this returns the current volume. If channel is -1, this returns the average of all channels.
        int Volume(int channel, int volume);

        /// @brief Set the volume for a specific chunk. https://wiki.libsdl.org/SDL2_mixer/Mix_VolumeChunk
        /// @param chunk the chunk whose volume to adjust.
        /// @param volume the new volume, between 0 and MIX_MAX_VOLUME, or -1 to query.
        /// @return Returns the previous volume. If the specified volume is -1, this returns the current volume. If chunk is NULL, this returns -1.
        int VolumeChunk(Mix_Chunk *chunk, int volume);

        /// @brief Set the volume for the music channel. https://wiki.libsdl.org/SDL2_mixer/Mix_VolumeMusic
        /// @param volume the new volume, between 0 and MIX_MAX_VOLUME, or -1 to query.
        /// @return Returns the previous volume. If the specified volume is -1, this returns the current volume.
        int VolumeMusic(int volume);

    } // namespace Mixer

    namespace TTF {

        /// @brief Tell SDL_ttf whether UNICODE text is generally byteswapped. https://wiki.libsdl.org/SDL2_ttf/TTF_ByteSwappedUNICODE
        /// @param swapped boolean to indicate whether text is byteswapped
        void ByteSwappedUNICODE(bool swapped);

        /// @brief Dispose of a previously-created font. https://wiki.libsdl.org/SDL2_ttf/TTF_CloseFont
        /// @param font the font to dispose of.
        void CloseFont(TTF_Font *font);

        /// @brief Query the offset from the baseline to the top of a font. https://wiki.libsdl.org/SDL2_ttf/TTF_FontAscent
        /// @param font the font to query.
        /// @return Returns the font's ascent.
        int FontAscent(const TTF_Font *font);

        /// @brief Query the offset from the baseline to the bottom of a font. https://wiki.libsdl.org/SDL2_ttf/TTF_FontDescent
        /// @param font the font to query.
        /// @return Returns the font's descent.
        int FontDescent(const TTF_Font *font);

        /// @brief Query a font's family name. https://wiki.libsdl.org/SDL2_ttf/TTF_FontFaceFamilyName
        /// @param font the font to query.
        /// @return Returns the font's family name.
        const char * FontFaceFamilyName(const TTF_Font *font);

        /// @brief Query whether a font is fixed-width. https://wiki.libsdl.org/SDL2_ttf/TTF_FontFaceIsFixedWidth
        /// @param font the font to query.
        /// @return Returns non-zero if fixed-width, zero if not.
        int FontFaceIsFixedWidth(const TTF_Font *font);

        /// @brief Query the number of faces of a font. https://wiki.libsdl.org/SDL2_ttf/TTF_FontFaces
        /// @param font the font to query.
        /// @return Returns the number of FreeType font faces.
        long FontFaces(const TTF_Font *font);

        /// @brief Query a font's style name. https://wiki.libsdl.org/SDL2_ttf/TTF_FontFaceStyleName
        /// @param font the font to query.
        /// @return Returns the font's style name.
        const char * FontFaceStyleName(const TTF_Font *font);

        /// @brief Query the total height of a font. https://wiki.libsdl.org/SDL2_ttf/TTF_FontHeight
        /// @param font the font to query.
        /// @return Returns the font's height.
        int FontHeight(const TTF_Font *font);

        /// @brief Query the recommended spacing between lines of text for a font. https://wiki.libsdl.org/SDL2_ttf/TTF_FontLineSkip
        /// @param font the font to query.
        /// @return Returns the font's recommended spacing.
        int FontLineSkip(const TTF_Font *font);

        /// @brief Query a font's current FreeType hinter setting. https://wiki.libsdl.org/SDL2_ttf/TTF_GetFontHinting
        /// @param font the font to query.
        /// @return Returns the font's current hinter value.
        int GetFontHinting(const TTF_Font *font);

        /// @brief Query whether or not kerning is allowed for a font. https://wiki.libsdl.org/SDL2_ttf/TTF_GetFontKerning
        /// @param font the font to query.
        /// @return Returns non-zero if kerning is enabled, zero otherwise.
        int GetFontKerning(const TTF_Font *font);

        /// @brief Query a font's current outline. https://wiki.libsdl.org/SDL2_ttf/TTF_GetFontOutline
        /// @param font the font to query.
        /// @return Returns the font's current outline value.
        int GetFontOutline(const TTF_Font *font);

        /// @brief Query a font's current style. https://wiki.libsdl.org/SDL2_ttf/TTF_GetFontStyle
        /// @param font the font to query.
        /// @return Returns the current font style, as a set of bit flags.
        int GetFontStyle(const TTF_Font *font);

        /// @brief Query a font's current wrap alignment option. https://wiki.libsdl.org/SDL2_ttf/TTF_GetFontWrappedAlign
        /// @param font the font to query.
        /// @return Returns the font's current wrap alignment option.
        int GetFontWrappedAlign(const TTF_Font *font);

        /// @brief Query the version of the FreeType library in use. https://wiki.libsdl.org/SDL2_ttf/TTF_GetFreeTypeVersion
        /// @param major to be filled in with the major version number. Can be NULL.
        /// @param minor to be filled in with the minor version number. Can be NULL.
        /// @param patch to be filled in with the param version number. Can be NULL.
        void GetFreeTypeVersion(int *major, int *minor, int *patch);

        /// @brief Query the version of the HarfBuzz library in use. https://wiki.libsdl.org/SDL2_ttf/TTF_GetHarfBuzzVersion
        /// @param major to be filled in with the major version number. Can be NULL.
        /// @param minor to be filled in with the minor version number. Can be NULL.
        /// @param patch to be filled in with the param version number. Can be NULL.
        void GetHarfBuzzVersion(int *major, int *minor, int *patch);

        /// @brief Check whether a glyph is provided by the font for a 16-bit codepoint. https://wiki.libsdl.org/SDL2_ttf/TTF_GlyphIsProvided
        /// @param font the font to query.
        /// @param ch the character code to check.
        /// @return Returns non-zero if font provides a glyph for this character, zero if not.
        int GlyphIsProvided(TTF_Font *font, uint16_t ch);

        /// @brief Check whether a glyph is provided by the font for a 32-bit codepoint. https://wiki.libsdl.org/SDL2_ttf/TTF_GlyphIsProvided32
        /// @param font the font to query.
        /// @param ch the character code to check.
        /// @return Returns non-zero if font provides a glyph for this character, zero if not.
        int GlyphIsProvided32(TTF_Font *font, Uint32 ch);

        /// @brief Query the metrics (dimensions) of a font's 16-bit glyph. https://wiki.libsdl.org/SDL2_ttf/TTF_GlyphMetrics
        /// @param font the font to query.
        /// @param ch the character code to check.
        int GlyphMetrics(TTF_Font *font, uint16_t ch, int *minx, int *maxx, int *miny, int *maxy, int *advance);

        /// @brief Query the metrics (dimensions) of a font's 32-bit glyph. https://wiki.libsdl.org/SDL2_ttf/TTF_GlyphMetrics32
        /// @param font the font to query.
        /// @param ch the character code to check.
        int GlyphMetrics32(TTF_Font *font, Uint32 ch, int *minx, int *maxx, int *miny, int *maxy, int *advance);

        /// @brief Initialize SDL_ttf. https://wiki.libsdl.org/SDL2_ttf/TTF_Init
        /// @return Returns 0 on success, -1 on error.
        int Init();

        /// @brief Query the version of SDL_ttf that the program is linked against. https://wiki.libsdl.org/SDL2_ttf/TTF_Linked_Version
        /// @return Returns a pointer to the version information.
        const SDL_version * Linked_Version();

        /// @brief Calculate how much of a Latin1 string will fit in a given width. https://wiki.libsdl.org/SDL2_ttf/TTF_MeasureText
        /// @param font the font to query.
        /// @param text text to calculate, in Latin1 encoding.
        /// @param measure_width maximum width, in pixels, available for the string.
        /// @param count on return, filled with number of characters that can be rendered.
        /// @param extent on return, filled with latest calculated width.
        /// @return Returns 0 if successful, -1 on error.
        int MeasureText(TTF_Font *font, const char *text, int measure_width, int *extent, int *count);

        /// @brief Calculate how much of a UCS-2 string will fit in a given width. https://wiki.libsdl.org/SDL2_ttf/TTF_MeasureUNICODE
        /// @param font the font to query.
        /// @param text text to calculate, in UCS-2 encoding.
        /// @param measure_width maximum width, in pixels, available for the string.
        /// @param count on return, filled with number of characters that can be rendered.
        /// @param extent on return, filled with latest calculated width.
        /// @return Returns 0 if successful, -1 on error.
        int MeasureUNICODE(TTF_Font *font, const uint16_t *text, int measure_width, int *extent, int *count);

        /// @brief Calculate how much of a UTF-8 string will fit in a given width. https://wiki.libsdl.org/SDL2_ttf/TTF_MeasureUTF8
        /// @param font the font to query.
        /// @param text text to calculate, in UTF-8 encoding.
        /// @param measure_width maximum width, in pixels, available for the string.
        /// @param count on return, filled with number of characters that can be rendered.
        /// @param extent on return, filled with latest calculated width.
        /// @return Returns 0 if successful, -1 on error.
        int MeasureUTF8(TTF_Font *font, const char *text, int measure_width, int *extent, int *count);

        /// @brief Create a font from a file, using a specified point size. https://wiki.libsdl.org/SDL2_ttf/TTF_OpenFont
        /// @param file path to font file.
        /// @param ptsize point size to use for the newly-opened font.
        /// @return Returns a valid TTF_Font, or NULL on error.
        TTF_Font * OpenFont(const char *file, int ptsize);

        /// @brief Create a font from a file, using target resolutions (in DPI). https://wiki.libsdl.org/SDL2_ttf/TTF_OpenFontDPI
        /// @param file path to font file.
        /// @param ptsize point size to use for the newly-opened font.
        /// @param hdpi the target horizontal DPI.
        /// @param vdpi the target vertical DPI.
        /// @return Returns a valid TTF_Font, or NULL on error.
        TTF_Font * OpenFontDPI(const char *file, int ptsize, unsigned int hdpi, unsigned int vdpi);

        /// @brief Opens a font from an SDL_RWops with target resolutions (in DPI). https://wiki.libsdl.org/SDL2_ttf/TTF_OpenFontDPIRW
        /// @param src an SDL_RWops to provide a font file's data.
        /// @param freesrc non-zero to close the RWops when the font is closed, zero to leave it open.
        /// @param ptsize point size to use for the newly-opened font.
        /// @param hdpi the target horizontal DPI.
        /// @param vdpi the target vertical DPI.
        /// @return Returns a valid TTF_Font, or NULL on error.
        TTF_Font * OpenFontDPIRW(SDL_RWops *src, int freesrc, int ptsize, unsigned int hdpi, unsigned int vdpi);

        /// @brief Create a font from a file, using a specified face index. https://wiki.libsdl.org/SDL2_ttf/TTF_OpenFontIndex
        /// @param file path to font file.
        /// @param ptsize point size to use for the newly-opened font.
        /// @param index index of the face in the font file.
        /// @return Returns a valid TTF_Font, or NULL on error.
        TTF_Font * OpenFontIndex(const char *file, int ptsize, long index);

        /// @brief Create a font from a file, using target resolutions (in DPI). https://wiki.libsdl.org/SDL2_ttf/TTF_OpenFontIndexDPI
        /// @param file path to font file.
        /// @param ptsize point size to use for the newly-opened font.
        /// @param index index of the face in the font file.
        /// @param hdpi the target horizontal DPI.
        /// @param vdpi the target vertical DPI.
        /// @return Returns a valid TTF_Font, or NULL on error.
        TTF_Font * OpenFontIndexDPI(const char *file, int ptsize, long index, unsigned int hdpi, unsigned int vdpi);

        /// @brief Opens a font from an SDL_RWops with target resolutions (in DPI). https://wiki.libsdl.org/SDL2_ttf/TTF_OpenFontIndexDPIRW
        /// @param src an SDL_RWops to provide a font file's data.
        /// @param freesrc non-zero to close the RWops when the font is closed, zero to leave it open.
        /// @param ptsize point size to use for the newly-opened font.
        /// @param index index of the face in the font file.
        /// @param hdpi the target horizontal DPI.
        /// @param vdpi the target vertical DPI.
        /// @return Returns a valid TTF_Font, or NULL on error.
        TTF_Font * OpenFontIndexDPIRW(SDL_RWops *src, int freesrc, int ptsize, long index, unsigned int hdpi, unsigned int vdpi);

        /// @brief Create a font from an SDL_RWops, using a specified face index. https://wiki.libsdl.org/SDL2_ttf/TTF_OpenFontIndexRW
        /// @param src an SDL_RWops to provide a font file's data.
        /// @param freesrc non-zero to close the RWops when the font is closed, zero to leave it open.
        /// @param ptsize point size to use for the newly-opened font.
        /// @param index index of the face in the font file.
        /// @return Returns a valid TTF_Font, or NULL on error.
        TTF_Font * OpenFontIndexRW(SDL_RWops *src, int freesrc, int ptsize, long index);

        /// @brief Create a font from an SDL_RWops, using a specified point size. https://wiki.libsdl.org/SDL2_ttf/TTF_OpenFontRW
        /// @param src an SDL_RWops to provide a font file's data.
        /// @param freesrc non-zero to close the RWops when the font is closed, zero to leave it open.
        /// @param ptsize point size to use for the newly-opened font.
        /// @return Returns a valid TTF_Font, or NULL on error.
        TTF_Font * OpenFontRW(SDL_RWops *src, int freesrc, int ptsize);

        /// @brief Deinitialize SDL_ttf. https://wiki.libsdl.org/SDL2_ttf/TTF_Quit
        void Quit();

        /// @brief Render a single 32-bit glyph at high quality to a new ARGB surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderGlyph32_Blended
        /// @param font the font to render with.
        /// @param ch the character to render.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 32-bit, ARGB surface, or NULL if there was an error.
        SDL_Surface * RenderGlyph32_Blended(TTF_Font *font, Uint32 ch, SDL_Color fg);

        /// @brief Render a single 32-bit glyph at LCD subpixel quality to a new ARGB surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderGlyph32_LCD
        /// @param font the font to render with.
        /// @param ch the character to render.
        /// @param fg the foreground color for the text.
        /// @param bg the background color for the text.
        /// @return Returns a new 32-bit, ARGB surface, or NULL if there was an error.
        SDL_Surface * RenderGlyph32_LCD(TTF_Font *font, Uint32 ch, SDL_Color fg, SDL_Color bg);

        /// @brief Render a single 32-bit glyph at high quality to a new 8-bit surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderGlyph32_Shaded
        /// @param font the font to render with.
        /// @param ch the character to render.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 8-bit, palettized surface, or NULL if there was an error.
        SDL_Surface * RenderGlyph32_Shaded(TTF_Font *font, Uint32 ch, SDL_Color fg, SDL_Color bg);

        /// @brief Render a single 32-bit glyph at fast quality to a new 8-bit surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderGlyph32_Solid
        /// @param font the font to render with.
        /// @param ch the character to render.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 8-bit, palettized surface, or NULL if there was an error.
        SDL_Surface * RenderGlyph32_Solid(TTF_Font *font, Uint32 ch, SDL_Color fg);

        /// @brief Render a single 16-bit glyph at high quality to a new ARGB surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderGlyph_Blended
        /// @param font the font to render with.
        /// @param ch the character to render.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 32-bit, ARGB surface, or NULL if there was an error.
        SDL_Surface * RenderGlyph_Blended(TTF_Font *font, uint16_t ch, SDL_Color fg);

        /// @brief Render a single 16-bit glyph at LCD subpixel quality to a new ARGB surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderGlyph_LCD
        /// @param font the font to render with.
        /// @param ch the character to render.
        /// @param fg the foreground color for the text.
        /// @param bg the background color for the text.
        /// @return Returns a new 32-bit, ARGB surface, or NULL if there was an error.
        SDL_Surface * RenderGlyph_LCD(TTF_Font *font, uint16_t ch, SDL_Color fg, SDL_Color bg);

        /// @brief Render a single 16-bit glyph at high quality to a new 8-bit surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderGlyph_Shaded
        /// @param font the font to render with.
        /// @param ch the character to render.
        /// @param fg the foreground color for the text.
        /// @param bg the background color for the text.
        /// @return Returns a new 8-bit, palettized surface, or NULL if there was an error.
        SDL_Surface * RenderGlyph_Shaded(TTF_Font *font, uint16_t ch, SDL_Color fg, SDL_Color bg);

        /// @brief Render a single 16-bit glyph at fast quality to a new 8-bit surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderGlyph_Solid
        /// @param font the font to render with.
        /// @param ch the character to render.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 8-bit, palettized surface, or NULL if there was an error.
        SDL_Surface * RenderGlyph_Solid(TTF_Font *font, uint16_t ch, SDL_Color fg);

        /// @brief Render Latin1 text at high quality to a new ARGB surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderText_Blended
        /// @param font the font to render with.
        /// @param text text to render, in Latin1 encoding.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 32-bit, ARGB surface, or NULL if there was an error.
        SDL_Surface * RenderText_Blended(TTF_Font *font, const char *text, SDL_Color fg);

        /// @brief Render word-wrapped Latin1 text at high quality to a new ARGB surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderText_Blended_Wrapped
        /// @param font the font to render with.
        /// @param text text to render, in Latin1 encoding.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 32-bit, ARGB surface, or NULL if there was an error.
        SDL_Surface * RenderText_Blended_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, Uint32 wrapLength);

        /// @brief Render Latin1 text at LCD subpixel quality to a new ARGB surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderText_LCD
        /// @param font the font to render with.
        /// @param text text to render, in Latin1 encoding.
        /// @param fg the foreground color for the text.
        /// @param bg the background color for the text.
        /// @return Returns a new 32-bit, ARGB surface, or NULL if there was an error.
        SDL_Surface * RenderText_LCD(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg);

        /// @brief Render word-wrapped Latin1 text at LCD subpixel quality to a new ARGB surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderText_LCD_Wrapped
        /// @param font the font to render with.
        /// @param text text to render, in Latin1 encoding.
        /// @param fg the foreground color for the text.
        /// @param bg the background color for the text.
        /// @return Returns a new 32-bit, ARGB surface, or NULL if there was an error.
        SDL_Surface * RenderText_LCD_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);

        /// @brief Render Latin1 text at high quality to a new 8-bit surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderText_Shaded
        /// @param font the font to render with.
        /// @param text text to render, in Latin1 encoding.
        /// @param fg the foreground color for the text.
        /// @param bg the background color for the text.
        /// @return Returns a new 8-bit, palettized surface, or NULL if there was an error.
        SDL_Surface * RenderText_Shaded(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg);

        /// @brief Render word-wrapped Latin1 text at high quality to a new 8-bit surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderText_Shaded_Wrapped
        /// @param font the font to render with.
        /// @param text text to render, in Latin1 encoding.
        /// @param fg the foreground color for the text.
        /// @param bg the background color for the text.
        /// @return Returns a new 8-bit, palettized surface, or NULL if there was an error.
        SDL_Surface * RenderText_Shaded_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);

        /// @brief Render Latin1 text at fast quality to a new 8-bit surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderText_Solid
        /// @param font the font to render with.
        /// @param text text to render, in Latin1 encoding.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 8-bit, palettized surface, or NULL if there was an error.
        SDL_Surface * RenderText_Solid(TTF_Font *font, const char *text, SDL_Color fg);

        /// @brief Render word-wrapped Latin1 text at fast quality to a new 8-bit surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderText_Solid_Wrapped
        /// @param font the font to render with.
        /// @param text text to render, in Latin1 encoding.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 8-bit, palettized surface, or NULL if there was an error.
        SDL_Surface * RenderText_Solid_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, Uint32 wrapLength);

        /// @brief Render UCS-2 text at high quality to a new ARGB surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderUNICODE_Blended
        /// @param font the font to render with.
        /// @param text text to render, in UCS-2 encoding.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 32-bit, ARGB surface, or NULL if there was an error.
        SDL_Surface * RenderUNICODE_Blended(TTF_Font *font, const uint16_t *text, SDL_Color fg);

        /// @brief Render word-wrapped UCS-2 text at high quality to a new ARGB surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderUNICODE_Blended_Wrapped
        /// @param font the font to render with.
        /// @param text text to render, in UCS-2 encoding.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 32-bit, ARGB surface, or NULL if there was an error.
        SDL_Surface * RenderUNICODE_Blended_Wrapped(TTF_Font *font, const uint16_t *text, SDL_Color fg, Uint32 wrapLength);

        /// @brief Render UCS-2 text at LCD subpixel quality to a new ARGB surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderUNICODE_LCD
        /// @param font the font to render with.
        /// @param text text to render, in UCS-2 encoding.
        /// @param fg the foreground color for the text.
        /// @param bg the background color for the text.
        /// @return Returns a new 32-bit, ARGB surface, or NULL if there was an error.
        SDL_Surface * RenderUNICODE_LCD(TTF_Font *font, const uint16_t *text, SDL_Color fg, SDL_Color bg);

        /// @brief Render word-wrapped UCS-2 text at LCD subpixel quality to a new ARGB surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderUNICODE_LCD_Wrapped
        /// @param font the font to render with.
        /// @param text text to render, in UCS-2 encoding.
        /// @param fg the foreground color for the text.
        /// @param bg the background color for the text.
        /// @return Returns a new 32-bit, ARGB surface, or NULL if there was an error.
        SDL_Surface * RenderUNICODE_LCD_Wrapped(TTF_Font *font, const uint16_t *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);

        /// @brief Render UCS-2 text at high quality to a new 8-bit surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderUNICODE_Shaded
        /// @param font the font to render with.
        /// @param text text to render, in UCS-2 encoding.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 8-bit, palettized surface, or NULL if there was an error.
        SDL_Surface * RenderUNICODE_Shaded(TTF_Font *font, const uint16_t *text, SDL_Color fg, SDL_Color bg);

        /// @brief Render word-wrapped UCS-2 text at high quality to a new 8-bit surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderUNICODE_Shaded_Wrapped
        /// @param font the font to render with.
        /// @param text text to render, in UCS-2 encoding.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 8-bit, palettized surface, or NULL if there was an error.
        SDL_Surface * RenderUNICODE_Shaded_Wrapped(TTF_Font *font, const uint16_t *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);

        /// @brief Render UCS-2 text at fast quality to a new 8-bit surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderUNICODE_Solid
        /// @param font the font to render with.
        /// @param text text to render, in UCS-2 encoding.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 8-bit, palettized surface, or NULL if there was an error.
        SDL_Surface * RenderUNICODE_Solid(TTF_Font *font, const uint16_t *text, SDL_Color fg);

        /// @brief Render word-wrapped UCS-2 text at fast quality to a new 8-bit surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderUNICODE_Solid_Wrapped
        /// @param font the font to render with.
        /// @param text text to render, in UCS-2 encoding.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 8-bit, palettized surface, or NULL if there was an error.
        SDL_Surface * RenderUNICODE_Solid_Wrapped(TTF_Font *font, const uint16_t *text, SDL_Color fg, Uint32 wrapLength);

        /// @brief Render UTF-8 text at high quality to a new ARGB surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderUTF8_Blended
        /// @param font the font to render with.
        /// @param text text to render, in UTF-8 encoding.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 32-bit, ARGB surface, or NULL if there was an error.
        SDL_Surface * RenderUTF8_Blended(TTF_Font *font, const char *text, SDL_Color fg);

        /// @brief Render word-wrapped UTF-8 text at high quality to a new ARGB surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderUTF8_Blended_Wrapped
        /// @param font the font to render with.
        /// @param text text to render, in UTF-8 encoding.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 32-bit, ARGB surface, or NULL if there was an error.
        SDL_Surface * RenderUTF8_Blended_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, Uint32 wrapLength);

        /// @brief Render UTF-8 text at LCD subpixel quality to a new ARGB surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderUTF8_LCD
        /// @param font the font to render with.
        /// @param text text to render, in UTF-8 encoding.
        /// @param fg the foreground color for the text.
        /// @param bg the background color for the text.
        /// @return Returns a new 32-bit, ARGB surface, or NULL if there was an error.
        SDL_Surface * RenderUTF8_LCD(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg);

        /// @brief Render word-wrapped UTF-8 text at LCD subpixel quality to a new ARGB surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderUTF8_LCD_Wrapped
        /// @param font the font to render with.
        /// @param text text to render, in UTF-8 encoding.
        /// @param fg the foreground color for the text.
        /// @param bg the background color for the text.
        /// @return Returns a new 32-bit, ARGB surface, or NULL if there was an error.
        SDL_Surface * RenderUTF8_LCD_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);

        /// @brief Render UTF-8 text at high quality to a new 8-bit surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderUTF8_Shaded
        /// @param font the font to render with.
        /// @param text text to render, in UTF-8 encoding.
        /// @param fg the foreground color for the text.
        /// @param bg the background color for the text.
        /// @return Returns a new 8-bit, palettized surface, or NULL if there was an error.
        SDL_Surface * RenderUTF8_Shaded(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg);

        /// @brief Render word-wrapped UTF-8 text at high quality to a new 8-bit surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderUTF8_Shaded_Wrapped
        /// @param font the font to render with.
        /// @param text text to render, in UTF-8 encoding.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 8-bit, palettized surface, or NULL if there was an error.
        SDL_Surface * RenderUTF8_Shaded_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);

        /// @brief Render UTF-8 text at fast quality to a new 8-bit surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderUTF8_Solid
        /// @param font the font to render with.
        /// @param text text to render, in UTF-8 encoding.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 8-bit, palettized surface, or NULL if there was an error.
        SDL_Surface * RenderUTF8_Solid(TTF_Font *font, const char *text, SDL_Color fg);

        /// @brief Render word-wrapped UTF-8 text at fast quality to a new 8-bit surface. https://wiki.libsdl.org/SDL2_ttf/TTF_RenderUTF8_Solid_Wrapped
        /// @param font the font to render with.
        /// @param text text to render, in UTF-8 encoding.
        /// @param fg the foreground color for the text.
        /// @return Returns a new 8-bit, palettized surface, or NULL if there was an error.
        SDL_Surface * RenderUTF8_Solid_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, Uint32 wrapLength);

        /// @brief Set a global direction to be used for text shaping. https://wiki.libsdl.org/SDL2_ttf/TTF_SetDirection
        /// @param direction an hb_direction_t value.
        /// @return Returns 0, or -1 if SDL_ttf is not compiled with HarfBuzz support.
        int SetDirection(int direction /* hb_direction_t */);

        /// @brief Set direction to be used for text shaping by a font. https://wiki.libsdl.org/SDL2_ttf/TTF_SetFontDirection
        /// @param font the font to specify a direction for.
        /// @param direction the new direction for text to flow.
        /// @return Returns 0 on success, or -1 on error.
        int SetFontDirection(TTF_Font *font, TTF_Direction direction);

        /// @brief Set a font's current hinter setting. https://wiki.libsdl.org/SDL2_ttf/TTF_SetFontHinting
        /// @param font the font to set a new hinter setting on.
        /// @param hinting the new hinter setting.
        void SetFontHinting(TTF_Font *font, int hinting);

        /// @brief Set if kerning is allowed for a font. https://wiki.libsdl.org/SDL2_ttf/TTF_SetFontKerning
        /// @param font the font to set kerning on.
        /// @param allowed non-zero to allow kerning, zero to disallow.
        void SetFontKerning(TTF_Font *font, int allowed);

        /// @brief Set a font's current outline. https://wiki.libsdl.org/SDL2_ttf/TTF_SetFontOutline
        /// @param font the font to set a new outline on.
        /// @param outline positive outline value, 0 to default.
        void SetFontOutline(TTF_Font *font, int outline);

        /// @brief Set script to be used for text shaping by a font. https://wiki.libsdl.org/SDL2_ttf/TTF_SetFontScriptName
        /// @param font the font to specify a direction for.
        /// @param script null-terminated string of exactly 4 characters.
        /// @return Returns 0 on success, or -1 on error.
        int SetFontScriptName(TTF_Font *font, const char *script);

        /// @brief Set a font's size dynamically. https://wiki.libsdl.org/SDL2_ttf/TTF_SetFontSize
        /// @param font the font to resize.
        /// @param ptsize the new point size.
        /// @return Returns 0 if successful, -1 on error
        int SetFontSize(TTF_Font *font, int ptsize);

        /// @brief Set font size dynamically with target resolutions (in DPI). https://wiki.libsdl.org/SDL2_ttf/TTF_SetFontSizeDPI
        /// @param font the font to resize.
        /// @param ptsize the new point size.
        /// @param hdpi the target horizontal DPI.
        /// @param vdpi the target vertical DPI.
        /// @return Returns 0 if successful, -1 on error.
        int SetFontSizeDPI(TTF_Font *font, int ptsize, unsigned int hdpi, unsigned int vdpi);

        /// @brief Set a font's current style. https://wiki.libsdl.org/SDL2_ttf/TTF_SetFontStyle
        /// @param font the font to set a new style on.
        /// @param style the new style values to set, OR'd together.
        void SetFontStyle(TTF_Font *font, int style);

        /// @brief Set a font's current wrap alignment option. https://wiki.libsdl.org/SDL2_ttf/TTF_SetFontWrappedAlign
        /// @param font the font to set a new wrap alignment option on.
        /// @param align the new wrap alignment option.
        void SetFontWrappedAlign(TTF_Font *font, int align);

        /// @brief Set a global script to be used for text shaping. https://wiki.libsdl.org/SDL2_ttf/TTF_SetScript
        /// @return Returns 0, or -1 if SDL_ttf is not compiled with HarfBuzz support.
        int SetScript(int script /* hb_script_t */);

        /// @brief Calculate the dimensions of a rendered string of Latin1 text. https://wiki.libsdl.org/SDL2_ttf/TTF_SizeText
        /// @param font the font to query.
        /// @param text text to calculate, in Latin1 encoding.
        /// @param w will be filled with width, in pixels, on return.
        /// @param h will be filled with height, in pixels, on return.
        /// @return Returns 0 if successful, -1 on error.
        int SizeText(TTF_Font *font, const char *text, int *w, int *h);

        /// @brief Calculate the dimensions of a rendered string of UCS-2 text. https://wiki.libsdl.org/SDL2_ttf/TTF_SizeUNICODE
        /// @param font the font to query.
        /// @param text text to calculate, in UCS-2 encoding.
        /// @param w will be filled with width, in pixels, on return.
        /// @param h will be filled with height, in pixels, on return.
        /// @return Returns 0 if successful, -1 on error.
        int SizeUNICODE(TTF_Font *font, const uint16_t *text, int *w, int *h);

        /// @brief Calculate the dimensions of a rendered string of UTF-8 text. https://wiki.libsdl.org/SDL2_ttf/TTF_SizeUTF8
        /// @param font the font to query.
        /// @param text text to calculate, in UTF-8 encoding.
        /// @param w will be filled with width, in pixels, on return.
        /// @param h will be filled with height, in pixels, on return.
        /// @return Returns 0 if successful, -1 on error.
        int SizeUTF8(TTF_Font *font, const char *text, int *w, int *h);

        /// @brief Check if SDL_ttf is initialized. https://wiki.libsdl.org/SDL2_ttf/TTF_WasInit
        /// @return Returns the current number of initialization calls, that need to eventually be paired with this many calls to TTF_Quit().
        int WasInit();

    } // namespace TTF
    
    namespace Image {

        /// @brief Dispose of an IMG_Animation and free its resources. https://wiki.libsdl.org/SDL2_image/IMG_FreeAnimation
        /// @param anim IMG_Animation to dispose of.
        void FreeAnimation(IMG_Animation *anim);

        /// @brief Initialize SDL_image. https://wiki.libsdl.org/SDL2_image/IMG_Init
        /// @param flags initialization flags, OR'd together.
        /// @return Returns all currently initialized flags.
        int Init(int flags);

        /// @brief Detect AVIF image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isAVIF
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is AVIF data, zero otherwise.
        int isAVIF(SDL_RWops *src);

        /// @brief Detect BMP image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isBMP
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is BMP data, zero otherwise.
        int isBMP(SDL_RWops *src);

        /// @brief Detect CUR image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isCUR
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is CUR data, zero otherwise.
        int isCUR(SDL_RWops *src);

        /// @brief Detect GIF image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isGIF
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is GIF data, zero otherwise.
        int isGIF(SDL_RWops *src);

        /// @brief Detect ICO image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isICO
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is ICO data, zero otherwise.
        int isICO(SDL_RWops *src);

        /// @brief Detect JPG image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isJPG
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is JPG data, zero otherwise.
        int isJPG(SDL_RWops *src);

        /// @brief Detect JXL image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isJXL
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is JXL data, zero otherwise.
        int isJXL(SDL_RWops *src);

        /// @brief Detect LBM image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isLBM
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is LBM data, zero otherwise.
        int isLBM(SDL_RWops *src);

        /// @brief Detect PCX image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isPCX
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is PCX data, zero otherwise.
        int isPCX(SDL_RWops *src);

        /// @brief Detect PNG image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isPNG
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is PNG data, zero otherwise.
        int isPNG(SDL_RWops *src);

        /// @brief Detect PNM image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isPNM
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is PNM data, zero otherwise.
        int isPNM(SDL_RWops *src);

        /// @brief Detect QOI image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isQOI
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is QOI data, zero otherwise.
        int isQOI(SDL_RWops *src);

        /// @brief Detect SVG image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isSVG
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is SVG data, zero otherwise.
        int isSVG(SDL_RWops *src);

        /// @brief Detect TIFF image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isTIF
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is TIFF data, zero otherwise.
        int isTIF(SDL_RWops *src);

        /// @brief Detect WEBP image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isWEBP
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is WEBP data, zero otherwise.
        int isWEBP(SDL_RWops *src);

        /// @brief Detect XCF image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isXCF
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is XCF data, zero otherwise.
        int isXCF(SDL_RWops *src);

        /// @brief Detect XPM image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isXPM
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is XPM data, zero otherwise.
        int isXPM(SDL_RWops *src);

        /// @brief Detect XV image data on a readable/seekable SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_isXV
        /// @param src a seekable/readable SDL_RWops to provide image data.
        /// @return Returns non-zero if this is XV data, zero otherwise.
        int isXV(SDL_RWops *src);

        /// @brief This function gets the version of the dynamically linked SDL_image library. https://wiki.libsdl.org/SDL2_image/IMG_Linked_Version
        /// @return Returns SDL_image version
        const SDL_version * Linked_Version();

        /// @brief Load an image from a filesystem path into a software surface. https://wiki.libsdl.org/SDL2_image/IMG_Load
        /// @param file a path on the filesystem to load an image from.
        /// @return Returns a new SDL surface, or NULL on error.
        SDL_Surface * Load(const char *file);

        /// @brief Load an image from an SDL data source into a software surface. https://wiki.libsdl.org/SDL2_image/IMG_Load_RW
        /// @param src an SDL_RWops that data will be read from.
        /// @param freesrc non-zero to close/free the SDL_RWops before returning, zero to leave it open.
        /// @return Returns a new SDL surface, or NULL on error.
        SDL_Surface * Load_RW(SDL_RWops *src, int freesrc);

        /// @brief Load an animation from a file. https://wiki.libsdl.org/SDL2_image/IMG_LoadAnimation
        /// @param file path on the filesystem containing an animated image.
        /// @return Returns a new IMG_Animation, or NULL on error.
        IMG_Animation * LoadAnimation(const char *file);

        /// @brief Load an animation from an SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_LoadAnimation_RW
        /// @param src an SDL_RWops that data will be read from.
        /// @param freesrc non-zero to close/free the SDL_RWops before returning, zero to leave it open.
        /// @return Returns a new IMG_Animation, or NULL on error.
        IMG_Animation * LoadAnimation_RW(SDL_RWops *src, int freesrc);

        /// @brief Load an animation from an SDL datasource https://wiki.libsdl.org/SDL2_image/IMG_LoadAnimationTyped_RW
        /// @param src an SDL_RWops that data will be read from.
        /// @param freesrc non-zero to close/free the SDL_RWops before returning, zero to leave it open.
        /// @param type a filename extension that represent this data ("GIF", etc).
        /// @return Returns a new IMG_Animation, or NULL on error.
        IMG_Animation * LoadAnimationTyped_RW(SDL_RWops *src, int freesrc, const char *type);

        /// @brief Load a AVIF image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadAVIF_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadAVIF_RW(SDL_RWops *src);

        /// @brief Load a BMP image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadBMP_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadBMP_RW(SDL_RWops *src);

        /// @brief Load a CUR image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadCUR_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadCUR_RW(SDL_RWops *src);

        /// @brief Load a GIF image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadGIF_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadGIF_RW(SDL_RWops *src);

        /// @brief Load a GIF animation directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadGIFAnimation_RW
        /// @param src an SDL_RWops that data will be read from.
        /// @return Returns a new IMG_Animation, or NULL on error.
        IMG_Animation * LoadGIFAnimation_RW(SDL_RWops *src);

        /// @brief Load a ICO image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadICO_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadICO_RW(SDL_RWops *src);

        /// @brief Load a JPG image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadJPG_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadJPG_RW(SDL_RWops *src);

        /// @brief Load a JXL image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadJXL_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadJXL_RW(SDL_RWops *src);

        /// @brief Load a LBM image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadLBM_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadLBM_RW(SDL_RWops *src);

        /// @brief Load a PCX image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadPCX_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadPCX_RW(SDL_RWops *src);

        /// @brief Load a PNG image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadPNG_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadPNG_RW(SDL_RWops *src);

        /// @brief Load a PNM image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadPNM_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadPNM_RW(SDL_RWops *src);

        /// @brief Load a QOI image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadQOI_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadQOI_RW(SDL_RWops *src);

        /// @brief Load an SVG image, scaled to a specific size. https://wiki.libsdl.org/SDL2_image/IMG_LoadSizedSVG_RW
        /// @param src an SDL_RWops to load SVG data from.
        /// @param width desired width of the generated surface, in pixels.
        /// @param height desired height of the generated surface, in pixels.
        /// @return Returns a new SDL surface, or NULL on error.
        SDL_Surface * LoadSizedSVG_RW(SDL_RWops *src, int width, int height);

        /// @brief Load a SVG image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadSVG_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadSVG_RW(SDL_RWops *src);

        /// @brief Load an image from a filesystem path into a GPU texture. https://wiki.libsdl.org/SDL2_image/IMG_LoadTexture
        /// @param renderer the SDL_Renderer to use to create the GPU texture.
        /// @param file a path on the filesystem to load an image from.
        /// @return Returns a new texture, or NULL on error.
        SDL_Texture * LoadTexture(SDL_Renderer *renderer, const char *file);

        /// @brief Load an image from an SDL data source into a GPU texture. https://wiki.libsdl.org/SDL2_image/IMG_LoadTexture_RW
        /// @param renderer the SDL_Renderer to use to create the GPU texture.
        /// @param src an SDL_RWops that data will be read from.
        /// @param freesrc non-zero to close/free the SDL_RWops before returning, zero to leave it open.
        /// @return Returns a new texture, or NULL on error.
        SDL_Texture * LoadTexture_RW(SDL_Renderer *renderer, SDL_RWops *src, int freesrc);

        /// @brief Load an image from an SDL data source into a GPU texture. https://wiki.libsdl.org/SDL2_image/IMG_LoadTextureTyped_RW
        /// @param renderer the SDL_Renderer to use to create the GPU texture.
        /// @param src an SDL_RWops that data will be read from.
        /// @param freesrc non-zero to close/free the SDL_RWops before returning, zero to leave it open.
        /// @param type a filename extension that represent this data ("BMP", "GIF", "PNG", etc).
        /// @return Returns a new texture, or NULL on error.
        SDL_Texture * LoadTextureTyped_RW(SDL_Renderer *renderer, SDL_RWops *src, int freesrc, const char *type);

        /// @brief Load a TGA image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadTGA_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadTGA_RW(SDL_RWops *src);

        /// @brief Load a TIFF image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadTIF_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadTIF_RW(SDL_RWops *src);

        /// @brief Load an image from an SDL data source into a software surface. https://wiki.libsdl.org/SDL2_image/IMG_LoadTyped_RW
        /// @param src an SDL_RWops that data will be read from.
        /// @param freesrc non-zero to close/free the SDL_RWops before returning, zero to leave it open.
        /// @param type a filename extension that represent this data ("BMP", "GIF", "PNG", etc).
        /// @return Returns a new SDL surface, or NULL on error.
        SDL_Surface * LoadTyped_RW(SDL_RWops *src, int freesrc, const char *type);

        /// @brief Load a WEBP image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadWEBP_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadWEBP_RW(SDL_RWops *src);

        /// @brief Load a XCF image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadXCF_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadXCF_RW(SDL_RWops *src);

        /// @brief Load a XPM image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadXPM_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadXPM_RW(SDL_RWops *src);

        /// @brief Load a XV image directly. https://wiki.libsdl.org/SDL2_image/IMG_LoadXV_RW
        /// @param src an SDL_RWops to load image data from.
        /// @return Returns SDL surface, or NULL on error
        SDL_Surface * LoadXV_RW(SDL_RWops *src);

        /// @brief Deinitialize SDL_image. https://wiki.libsdl.org/SDL2_image/IMG_Quit
        void Quit();

        /// @brief Load an XPM image from a memory array. https://wiki.libsdl.org/SDL2_image/IMG_ReadXPMFromArray
        /// @param xpm a null-terminated array of strings that comprise XPM data.
        /// @return Returns a new SDL surface, or NULL on error.
        SDL_Surface * ReadXPMFromArray(char **xpm);

        /// @brief Load an XPM image from a memory array. https://wiki.libsdl.org/SDL2_image/IMG_ReadXPMFromArrayToRGB888
        /// @param xpm a null-terminated array of strings that comprise XPM data.
        /// @return Returns a new SDL surface, or NULL on error.
        SDL_Surface * ReadXPMFromArrayToRGB888(char **xpm);

        /// @brief Save an SDL_Surface into a JPEG image file. https://wiki.libsdl.org/SDL2_image/IMG_SaveJPG
        /// @param surface the SDL surface to save
        /// @param file path on the filesystem to write new file to.
        /// @param quality [0; 33] is Lowest quality, [34; 66] is Middle quality, [67; 100] is Highest quality
        /// @return Returns 0 if successful, -1 on error
        int SaveJPG(SDL_Surface *surface, const char *file, int quality);

        /// @brief Save an SDL_Surface into JPEG image data, via an SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_SaveJPG_RW
        /// @param surface the SDL surface to save
        /// @param dst the SDL_RWops to save the image data to.
        /// @return Returns 0 if successful, -1 on error.
        int SaveJPG_RW(SDL_Surface *surface, SDL_RWops *dst, int freedst, int quality);

        /// @brief Save an SDL_Surface into a PNG image file. https://wiki.libsdl.org/SDL2_image/IMG_SavePNG
        /// @param surface the SDL surface to save
        /// @param file path on the filesystem to write new file to.
        /// @return Returns 0 if successful, -1 on error
        int SavePNG(SDL_Surface *surface, const char *file);

        /// @brief Save an SDL_Surface into PNG image data, via an SDL_RWops. https://wiki.libsdl.org/SDL2_image/IMG_SavePNG_RW
        /// @param surface the SDL surface to save
        /// @param dst the SDL_RWops to save the image data to.
        /// @return Returns 0 if successful, -1 on error.
        int SavePNG_RW(SDL_Surface *surface, SDL_RWops *dst, int freedst);

    } // namespace Image

#endif

} // namespace SDL2

#endif