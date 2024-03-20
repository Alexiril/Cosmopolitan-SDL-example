#define _COSMO_SOURCE

#include <libc/dlopen/dlfcn.h>
#include <libc/math.h>
#include <libc/dce.h>

#include <libc/isystem/filesystem>
#include <libc/isystem/fstream>
#include <libc/isystem/algorithm>
#include <libc/isystem/iostream>
#include <libc/isystem/string>

#include "cosmo_sdl2.hpp"

void LogError(const std::string& error, ErrorLevel level, std::ostream& out) {
    switch (level)
    {
    case ErrorLevel::info:
        out << "\033[94mInfo: " << error << "\033[0m\n";
        break;
    case ErrorLevel::warning:
        out << "\033[93mWarning: " << error << "\033[0m\n";
        break;
    case ErrorLevel::error:
        out << "\033[91mError: " << error << "\033[0m\n";
        break;
    default:
        out << "Log: " << error << "\n";
        break;
    }
}

bool UnpackFile(const std::string& filename, bool do_log) {
    std::ifstream input("/zip/" + filename, std::ios::binary);
    if (not input.is_open()) {
        if (do_log) LogError("Couldn't find '" + filename + "' in the package.");
        return false;
    }
    auto output_path = std::filesystem::path(filename);
    std::filesystem::create_directories(output_path.parent_path());
    std::ofstream output(filename, std::ios::binary);
    if (not output.is_open()) {
        if (do_log) LogError("Couldn't open '" + filename + "' while unpacking.");
        return false;
    }
    std::copy(
        std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>(),
        std::ostreambuf_iterator<char>(output)
    );
    return true;
}

#define MSABI __attribute__((__ms_abi__))
#define LOADFUNC(lib, func, src_func) if ((::func = cosmo_dlsym(::lib, src_func)) == nullptr) { if (::do_log) LogError(std::string(src_func) + " function is unavaliable (" + cosmo_dlerror() + ")."); err = true; }

namespace {

    static bool loaded = false;
    static bool do_log = true;
    static void* sdllibptr = nullptr;
    static void* sdlnetlibptr = nullptr;
    static void* sdlmixlibptr = nullptr;
    static void* sdlttflibptr = nullptr;
    static void* sdlimglibptr = nullptr;

    /* SDL2 functions prototypes */

#if 1 // To hide the list
    using AddEventWatchProto = void (*)(SDL_EventFilter filter, void *userdata);
    using AddHintCallbackProto = void (*)(const char *name, SDL_HintCallback callback, void *userdata);
    using AddTimerProto = SDL_TimerID (*)(uint32_t interval, SDL_TimerCallback callback, void *param);
    using AllocFormatProto = SDL_PixelFormat * (*)(uint32_t pixel_format);
    using AllocPaletteProto = SDL_Palette* (*)(int ncolors);
    using AllocRWProto = SDL_RWops* (*)(void);
    using AtomicAddProto = int (*)(SDL_atomic_t *a, int v);
    using AtomicCASProto = bool (*)(SDL_atomic_t *a, int oldval, int newval);
    using AtomicCASPtrProto = bool (*)(void **a, void *oldval, void *newval);
    using AtomicGetProto = int (*)(SDL_atomic_t *a);
    using AtomicGetPtrProto = void* (*)(void **a);
    using AtomicLockProto = void (*)(SDL_SpinLock *lock);
    using AtomicSetProto = int (*)(SDL_atomic_t *a, int v);
    using AtomicSetPtrProto = void* (*)(void **a, void* v);
    using AtomicTryLockProto = bool (*)(SDL_SpinLock *lock);
    using AtomicUnlockProto = void (*)(SDL_SpinLock *lock);
    using AudioInitProto = int (*)(const char *driver_name);
    using AudioQuitProto = void (*)(void);
    using AudioStreamAvailableProto = int (*)(SDL_AudioStream *stream);
    using AudioStreamClearProto = void (*)(SDL_AudioStream *stream);
    using AudioStreamFlushProto = int (*)(SDL_AudioStream *stream);
    using AudioStreamGetProto = int (*)(SDL_AudioStream *stream, void *buf, int len);
    using AudioStreamPutProto = int (*)(SDL_AudioStream *stream, void *buf, int len);
    using BlitSurfaceProto = int (*)(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Surface* dst, SDL_Rect* dstrect);
    using BuildAudioCVTProto = int (*)(SDL_AudioCVT * cvt, SDL_AudioFormat src_format, uint8_t src_channels, int src_rate, SDL_AudioFormat dst_format, uint8_t dst_channels, int dst_rate);
    using CalculateGammaRampProto = void (*)(float gamma, uint16_t * ramp);
    using CaptureMouseProto = int (*)(bool enabled);
    using ClearCompositionProto = void (*)(void);
    using ClearErrorProto = void (*)(void);
    using ClearHintsProto = void (*)(void);
    using ClearQueuedAudioProto = void (*)(SDL_AudioDeviceID dev);
    using CloseAudioProto = void (*)(void);
    using CloseAudioDeviceProto = void (*)(SDL_AudioDeviceID dev);
    using ComposeCustomBlendModeProto = SDL_BlendMode (*)(SDL_BlendFactor srcColorFactor, SDL_BlendFactor dstColorFactor, SDL_BlendOperation colorOperation, SDL_BlendFactor srcAlphaFactor, SDL_BlendFactor dstAlphaFactor, SDL_BlendOperation alphaOperation);
    using CondBroadcastProto = int (*)(SDL_cond * cond);
    using CondSignalProto = int (*)(SDL_cond * cond);
    using CondWaitProto = int (*)(SDL_cond * cond, SDL_mutex * mutex);
    using CondWaitTimeoutProto = int (*)(SDL_cond * cond, SDL_mutex * mutex, uint32_t ms);
    using ConvertAudioProto = int (*)(SDL_AudioCVT * cvt);
    using ConvertPixelsProto = int (*)(int width, int height, uint32_t src_format, void * src, int src_pitch, uint32_t dst_format, void * dst, int dst_pitch);
    using ConvertSurfaceProto = SDL_Surface* (*) (SDL_Surface * src, const SDL_PixelFormat * fmt, uint32_t flags);
    using ConvertSurfaceFormatProto = SDL_Surface* (*) (SDL_Surface * src, uint32_t pixel_format, uint32_t flags);
    using CreateColorCursorProto = SDL_Cursor* (*)(SDL_Surface *surface, int hot_x, int hot_y);
    using CreateCondProto = SDL_cond* (*)(void);
    using CreateCursorProto = SDL_Cursor* (*)(const uint8_t * data, const uint8_t * mask, int w, int h, int hot_x, int hot_y);
    using CreateMutexProto = SDL_mutex* (*)(void);
    using CreateRendererProto = SDL_Renderer * (*)(SDL_Window * window, int index, uint32_t flags);
    using CreateRGBSurfaceProto = SDL_Surface* (*) (uint32_t flags, int width, int height, int depth, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask);
    using CreateRGBSurfaceFromProto = SDL_Surface* (*)(void *pixels, int width, int height, int depth, int pitch, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask);
    using CreateRGBSurfaceWithFormatProto = SDL_Surface* (*) (uint32_t flags, int width, int height, int depth, uint32_t format);
    using CreateRGBSurfaceWithFormatFromProto = SDL_Surface* (*) (void *pixels, int width, int height, int depth, int pitch, uint32_t format);
    using CreateSemaphoreProto = SDL_sem* (*)(uint32_t initial_value);
    using CreateShapedWindowProto = SDL_Window * (*)(const char *title,unsigned int x,unsigned int y,unsigned int w,unsigned int h,uint32_t flags);
    using CreateSoftwareRendererProto = SDL_Renderer * (*)(SDL_Surface * surface);
    using CreateSystemCursorProto = SDL_Cursor* (*)(SDL_SystemCursor id);
    using CreateTextureProto = SDL_Texture * (*)(SDL_Renderer * renderer, uint32_t format, int access, int w, int h);
    using CreateTextureFromSurfaceProto = SDL_Texture * (*)(SDL_Renderer * renderer, SDL_Surface * surface);
    using CreateThreadProto = SDL_Thread * (*)(SDL_ThreadFunction fn, const char *name, void *data);
    using CreateThreadWithStackSizeProto = SDL_Thread * (*)(SDL_ThreadFunction fn, const char *name, const uint64_t stacksize, void *data);
    using CreateWindowProto = SDL_Window * (*)(const char *title, int x, int y, int w, int h, uint32_t flags);
    using CreateWindowAndRendererProto = int (*)( int width, int height, uint32_t window_flags, SDL_Window **window, SDL_Renderer **renderer);
    using CreateWindowFromProto = SDL_Window * (*)(void *data);
    using DelayProto = void (*)(uint32_t ms);
    using DelEventWatchProto = void (*)(SDL_EventFilter filter, void *userdata);
    using DelHintCallbackProto = void (*)(const char *name, SDL_HintCallback callback, void *userdata);
    using DequeueAudioProto = uint32_t (*)(SDL_AudioDeviceID dev, void *data, uint32_t len);
    using DestroyCondProto = void (*)(SDL_cond * cond);
    using DestroyMutexProto = void (*)(SDL_mutex * mutex);
    using DestroyRendererProto = void (*)(SDL_Renderer * renderer);
    using DestroySemaphoreProto = void (*)(SDL_sem * sem);
    using DestroyTextureProto = void (*)(SDL_Texture * texture);
    using DestroyWindowProto = void (*)(SDL_Window * window);
    using DestroyWindowSurfaceProto = int (*)(SDL_Window *window);
    using DetachThreadProto = void (*)(SDL_Thread * thread);
    using DisableScreenSaverProto = void (*)(void);
    using EnableScreenSaverProto = void (*)(void);
    using EncloseFPointsProto = bool (*)(const SDL_FPoint * points, int count, const SDL_FRect * clip, SDL_FRect * result);
    using EnclosePointsProto = bool (*)(const SDL_Point * points, int count, const SDL_Rect * clip, SDL_Rect * result);
    using EventStateProto = uint8_t (*)(uint32_t type, int state);
    using FillRectProto = int (*) (SDL_Surface * dst, const SDL_Rect * rect, uint32_t color);
    using FillRectsProto = int (*) (SDL_Surface * dst, const SDL_Rect * rects, int count, uint32_t color);
    using FilterEventsProto = void (*)(SDL_EventFilter filter, void *userdata);
    using FlashWindowProto = int (*)(SDL_Window * window, SDL_FlashOperation operation);
    using FlushEventProto = void (*)(uint32_t type);
    using FlushEventsProto = void (*)(uint32_t minType, uint32_t maxType);
    using FreeAudioStreamProto = void (*)(SDL_AudioStream *stream);
    using FreeCursorProto = void (*)(SDL_Cursor * cursor);
    using FreeFormatProto = void (*)(SDL_PixelFormat *format);
    using FreePaletteProto = void (*)(SDL_Palette * palette);
    using FreeRWProto = void (*)(SDL_RWops * area);
    using FreeSurfaceProto = void (*)(SDL_Surface * surface);
    using FreeWAVProto = void (*)(uint8_t * audio_buf);
    using GameControllerAddMappingProto = int (*)(const char* mappingString);
    using GameControllerAddMappingsFromRWProto = int (*)(SDL_RWops * rw, int freerw);
    using GameControllerCloseProto = void (*)(SDL_GameController *gamecontroller);
    using GameControllerEventStateProto = int (*)(int state);
    using GameControllerFromInstanceIDProto = SDL_GameController* (*)(SDL_JoystickID joyid);
    using GameControllerFromPlayerIndexProto = SDL_GameController* (*)(int player_index);
    using GameControllerGetAppleSFSymbolsNameForAxisProto = const char* (*)(SDL_GameController *gamecontroller, SDL_GameControllerAxis axis);
    using GameControllerGetAppleSFSymbolsNameForButtonProto = const char* (*)(SDL_GameController *gamecontroller, SDL_GameControllerButton button);
    using GameControllerGetAttachedProto = bool (*)(SDL_GameController *gamecontroller);
    using GameControllerGetAxisProto = int16_t  (*)(SDL_GameController *gamecontroller, SDL_GameControllerAxis axis);
    using GameControllerGetAxisFromStringProto = SDL_GameControllerAxis (*)(const char *str);
    using GameControllerGetBindForAxisProto = SDL_GameControllerButtonBind  (*)(SDL_GameController *gamecontroller, SDL_GameControllerAxis axis);
    using GameControllerGetBindForButtonProto = SDL_GameControllerButtonBind  (*)(SDL_GameController *gamecontroller, SDL_GameControllerButton button);
    using GameControllerGetButtonProto = uint8_t (*)(SDL_GameController *gamecontroller, SDL_GameControllerButton button);
    using GameControllerGetButtonFromStringProto = SDL_GameControllerButton (*)(const char *str);
    using GameControllerGetFirmwareVersionProto = uint16_t (*)(SDL_GameController *gamecontroller);
    using GameControllerGetJoystickProto = SDL_Joystick* (*)(SDL_GameController *gamecontroller);
    using GameControllerGetNumTouchpadFingersProto = int (*)(SDL_GameController *gamecontroller, int touchpad);
    using GameControllerGetNumTouchpadsProto = int (*)(SDL_GameController *gamecontroller);
    using GameControllerGetPlayerIndexProto = int (*)(SDL_GameController *gamecontroller);
    using GameControllerGetProductProto = uint16_t (*)(SDL_GameController *gamecontroller);
    using GameControllerGetProductVersionProto = uint16_t (*)(SDL_GameController *gamecontroller);
    using GameControllerGetSensorDataProto = int (*)(SDL_GameController *gamecontroller, SDL_SensorType type, float *data, int num_values);
    using GameControllerGetSensorDataRateProto = float (*)(SDL_GameController *gamecontroller, SDL_SensorType type);
    using GameControllerGetSensorDataWithTimestampProto = int (*)(SDL_GameController *gamecontroller, SDL_SensorType type, uint64_t *timestamp, float *data, int num_values);
    using GameControllerGetSerialProto = const char * (*)(SDL_GameController *gamecontroller);
    using GameControllerGetStringForAxisProto = const char* (*)(SDL_GameControllerAxis axis);
    using GameControllerGetStringForButtonProto = const char* (*)(SDL_GameControllerButton button);
    using GameControllerGetTouchpadFingerProto = int (*)(SDL_GameController *gamecontroller, int touchpad, int finger, uint8_t *state, float *x, float *y, float *pressure);
    using GameControllerGetTypeProto = SDL_GameControllerType (*)(SDL_GameController *gamecontroller);
    using GameControllerGetVendorProto = uint16_t (*)(SDL_GameController *gamecontroller);
    using GameControllerHasAxisProto = bool  (*)(SDL_GameController *gamecontroller, SDL_GameControllerAxis axis);
    using GameControllerHasButtonProto = bool (*)(SDL_GameController *gamecontroller, SDL_GameControllerButton button);
    using GameControllerHasLEDProto = bool (*)(SDL_GameController *gamecontroller);
    using GameControllerHasRumbleProto = bool (*)(SDL_GameController *gamecontroller);
    using GameControllerHasRumbleTriggersProto = bool (*)(SDL_GameController *gamecontroller);
    using GameControllerHasSensorProto = bool (*)(SDL_GameController *gamecontroller, SDL_SensorType type);
    using GameControllerIsSensorEnabledProto = bool (*)(SDL_GameController *gamecontroller, SDL_SensorType type);
    using GameControllerMappingProto = char * (*)(SDL_GameController *gamecontroller);
    using GameControllerMappingForDeviceIndexProto = char* (*)(int joystick_index);
    using GameControllerMappingForGUIDProto = char * (*)(SDL_JoystickGUID guid);
    using GameControllerMappingForIndexProto = char * (*)(int mapping_index);
    using GameControllerNameProto = const char* (*)(SDL_GameController *gamecontroller);
    using GameControllerNameForIndexProto = const char* (*)(int joystick_index);
    using GameControllerNumMappingsProto = int (*)(void);
    using GameControllerOpenProto = SDL_GameController* (*)(int joystick_index);
    using GameControllerPathProto = const char* (*)(SDL_GameController *gamecontroller);
    using GameControllerPathForIndexProto = const char* (*)(int joystick_index);
    using GameControllerRumbleProto = int (*)(SDL_GameController *gamecontroller, uint16_t low_frequency_rumble, uint16_t high_frequency_rumble, uint32_t duration_ms);
    using GameControllerRumbleTriggersProto = int (*)(SDL_GameController *gamecontroller, uint16_t left_rumble, uint16_t right_rumble, uint32_t duration_ms);
    using GameControllerSendEffectProto = int (*)(SDL_GameController *gamecontroller, void *data, int size);
    using GameControllerSetLEDProto = int (*)(SDL_GameController *gamecontroller, uint8_t red, uint8_t green, uint8_t blue);
    using GameControllerSetPlayerIndexProto = void (*)(SDL_GameController *gamecontroller, int player_index);
    using GameControllerSetSensorEnabledProto = int (*)(SDL_GameController *gamecontroller, SDL_SensorType type, bool enabled);
    using GameControllerTypeForIndexProto = SDL_GameControllerType (*)(int joystick_index);
    using GameControllerUpdateProto = void (*)(void);
    using GetAssertionHandlerProto = SDL_AssertionHandler (*)(void **puserdata);
    using GetAssertionReportProto = const SDL_AssertData * (*)(void);
    using GetAudioDeviceNameProto = const char* (*)(int index, int iscapture);
    using GetAudioDeviceSpecProto = int (*)(int index, int iscapture, SDL_AudioSpec *spec);
    using GetAudioDeviceStatusProto = SDL_AudioStatus (*)(SDL_AudioDeviceID dev);
    using GetAudioDriverProto = const char* (*)(int index);
    using GetAudioStatusProto = SDL_AudioStatus (*)(void);
    using GetBasePathProto = char* (*)(void);
    using GetClipboardTextProto = char * (*)(void);
    using GetClipRectProto = void (*)(SDL_Surface * surface, SDL_Rect * rect);
    using GetClosestDisplayModeProto = SDL_DisplayMode * (*)(int displayIndex, const SDL_DisplayMode * mode, SDL_DisplayMode * closest);
    using GetColorKeyProto = int (*)(SDL_Surface * surface, uint32_t * key);
    using GetCPUCacheLineSizeProto = int (*)(void);
    using GetCPUCountProto = int (*)(void);
    using GetCurrentAudioDriverProto = const char* (*)(void);
    using GetCurrentDisplayModeProto = int (*)(int displayIndex, SDL_DisplayMode * mode);
    using GetCurrentVideoDriverProto = const char* (*)(void);
    using GetCursorProto = SDL_Cursor* (*)(void);
    using GetDefaultAssertionHandlerProto = SDL_AssertionHandler (*)(void);
    using GetDefaultAudioInfoProto = int (*)(char **name, SDL_AudioSpec *spec, int iscapture);
    using GetDefaultCursorProto = SDL_Cursor* (*)(void);
    using GetDesktopDisplayModeProto = int (*)(int displayIndex, SDL_DisplayMode * mode);
    using GetDisplayBoundsProto = int (*)(int displayIndex, SDL_Rect * rect);
    using GetDisplayDPIProto = int (*)(int displayIndex, float * ddpi, float * hdpi, float * vdpi);
    using GetDisplayModeProto = int (*)(int displayIndex, int modeIndex, SDL_DisplayMode * mode);
    using GetDisplayNameProto = const char * (*)(int displayIndex);
    using GetDisplayOrientationProto = SDL_DisplayOrientation (*)(int displayIndex);
    using GetDisplayUsableBoundsProto = int (*)(int displayIndex, SDL_Rect * rect);
    using GetErrorProto = const char* (*)(void);
    using GetErrorMsgProto = char * (*)(char *errstr, int maxlen);
    using GetEventFilterProto = bool (*)(SDL_EventFilter * filter, void **userdata);
    using GetGlobalMouseStateProto = uint32_t (*)(int *x, int *y);
    using GetGrabbedWindowProto = SDL_Window * (*)(void);
    using GetHintProto = const char * (*)(const char *name);
    using GetHintBooleanProto = bool (*)(const char *name, bool default_value);
    using GetJoystickGUIDInfoProto = void (*)(SDL_JoystickGUID guid, uint16_t *vendor, uint16_t *product, uint16_t *version, uint16_t *crc16);
    using GetKeyboardFocusProto = SDL_Window * (*)(void);
    using GetKeyboardStateProto = const uint8_t* (*)(int *numkeys);
    using GetKeyFromNameProto = SDL_Keycode (*)(const char *name);
    using GetKeyFromScancodeProto = SDL_Keycode (*)(SDL_Scancode scancode);
    using GetKeyNameProto = const char* (*)(SDL_Keycode key);
    using GetMemoryFunctionsProto = void (*)(SDL_malloc_func *malloc_func, SDL_calloc_func *calloc_func, SDL_realloc_func *realloc_func, SDL_free_func *free_func);
    using GetModStateProto = SDL_Keymod (*)(void);
    using GetMouseFocusProto = SDL_Window * (*)(void);
    using GetMouseStateProto = uint32_t (*)(int *x, int *y);
    using GetNumAllocationsProto = int (*)(void);
    using GetNumAudioDevicesProto = int (*)(int iscapture);
    using GetNumAudioDriversProto = int (*)(void);
    using GetNumDisplayModesProto = int (*)(int displayIndex);
    using GetNumRenderDriversProto = int (*)(void);
    using GetNumTouchDevicesProto = int (*)(void);
    using GetNumTouchFingersProto = int (*)(SDL_TouchID touchID);
    using GetNumVideoDisplaysProto = int (*)(void);
    using GetNumVideoDriversProto = int (*)(void);
    using GetOriginalMemoryFunctionsProto = void (*)(SDL_malloc_func *malloc_func, SDL_calloc_func *calloc_func, SDL_realloc_func *realloc_func, SDL_free_func *free_func);
    using GetPerformanceCounterProto = uint64_t (*)(void);
    using GetPerformanceFrequencyProto = uint64_t (*)(void);
    using GetPixelFormatNameProto = const char* (*)(uint32_t format);
    using GetPlatformProto = const char * (*) (void);
    using GetPointDisplayIndexProto = int (*)(const SDL_Point * point);
    using GetPowerInfoProto = SDL_PowerState (*)(int *seconds, int *percent);
    using GetPreferredLocalesProto = SDL_Locale * (*)(void);
    using GetPrefPathProto = char* (*)(const char *org, const char *app);
    using GetPrimarySelectionTextProto = char * (*)(void);
    using GetQueuedAudioSizeProto = uint32_t (*)(SDL_AudioDeviceID dev);
    using GetRectDisplayIndexProto = int (*)(const SDL_Rect * rect);
    using GetRelativeMouseModeProto = bool (*)(void);
    using GetRelativeMouseStateProto = uint32_t (*)(int *x, int *y);
    using GetRenderDrawBlendModeProto = int (*)(SDL_Renderer * renderer, SDL_BlendMode *blendMode);
    using GetRenderDrawColorProto = int (*)(SDL_Renderer * renderer, uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * a);
    using GetRenderDriverInfoProto = int (*)(int index, SDL_RendererInfo * info);
    using GetRendererProto = SDL_Renderer * (*)(SDL_Window * window);
    using GetRendererInfoProto = int (*)(SDL_Renderer * renderer, SDL_RendererInfo * info);
    using GetRendererOutputSizeProto = int (*)(SDL_Renderer * renderer, int *w, int *h);
    using GetRenderTargetProto = SDL_Texture * (*)(SDL_Renderer *renderer);
    using GetRevisionProto = const char* (*)(void);
    using GetRevisionNumberProto = int (*)(void);
    using GetRGBProto = void (*)(uint32_t pixel, const SDL_PixelFormat * format, uint8_t * r, uint8_t * g, uint8_t * b);
    using GetRGBAProto = void (*)(uint32_t pixel, const SDL_PixelFormat * format, uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * a);
    using GetScancodeFromKeyProto = SDL_Scancode (*)(SDL_Keycode key);
    using GetScancodeFromNameProto = SDL_Scancode (*)(const char *name);
    using GetScancodeNameProto = const char* (*)(SDL_Scancode scancode);
    using GetShapedWindowModeProto = int (*)(SDL_Window *window,SDL_WindowShapeMode *shape_mode);
    using GetSurfaceAlphaModProto = int (*)(SDL_Surface * surface, uint8_t * alpha);
    using GetSurfaceBlendModeProto = int (*)(SDL_Surface * surface, SDL_BlendMode *blendMode);
    using GetSurfaceColorModProto = int (*)(SDL_Surface * surface, uint8_t * r, uint8_t * g, uint8_t * b);
    using GetSystemRAMProto = int (*)(void);
    using GetTextureAlphaModProto = int (*)(SDL_Texture * texture, uint8_t * alpha);
    using GetTextureBlendModeProto = int (*)(SDL_Texture * texture, SDL_BlendMode *blendMode);
    using GetTextureColorModProto = int (*)(SDL_Texture * texture, uint8_t * r, uint8_t * g, uint8_t * b);
    using GetTextureScaleModeProto = int (*)(SDL_Texture * texture, SDL_ScaleMode *scaleMode);
    using GetTextureUserDataProto = void * (*)(SDL_Texture * texture);
    using GetThreadIDProto = SDL_threadID (*)(SDL_Thread * thread);
    using GetThreadNameProto = const char* (*)(SDL_Thread *thread);
    using GetTicksProto = uint32_t (*)(void);
    using GetTicks64Proto = uint64_t (*)(void);
    using GetTouchDeviceProto = SDL_TouchID (*)(int index);
    using GetTouchDeviceTypeProto = SDL_TouchDeviceType (*)(SDL_TouchID touchID);
    using GetTouchFingerProto = SDL_Finger * (*)(SDL_TouchID touchID, int index);
    using GetTouchNameProto = const char* (*)(int index);
    using GetVersionProto = void (*)(SDL_version * ver);
    using GetVideoDriverProto = const char* (*)(int index);
    using GetWindowBordersSizeProto = int (*)(SDL_Window * window, int *top, int *left, int *bottom, int *right);
    using GetWindowBrightnessProto = float (*)(SDL_Window * window);
    using GetWindowDataProto = void* (*)(SDL_Window * window, const char *name);
    using GetWindowDisplayIndexProto = int (*)(SDL_Window * window);
    using GetWindowDisplayModeProto = int (*)(SDL_Window * window, SDL_DisplayMode * mode);
    using GetWindowFlagsProto = uint32_t (*)(SDL_Window * window);
    using GetWindowFromIDProto = SDL_Window * (*)(uint32_t id);
    using GetWindowGammaRampProto = int (*)(SDL_Window * window, uint16_t * red, uint16_t * green, uint16_t * blue);
    using GetWindowGrabProto = bool (*)(SDL_Window * window);
    using GetWindowICCProfileProto = void* (*)(SDL_Window * window, uint64_t* size);
    using GetWindowIDProto = uint32_t (*)(SDL_Window * window);
    using GetWindowKeyboardGrabProto = bool (*)(SDL_Window * window);
    using GetWindowMaximumSizeProto = void (*)(SDL_Window * window, int *w, int *h);
    using GetWindowMinimumSizeProto = void (*)(SDL_Window * window, int *w, int *h);
    using GetWindowMouseGrabProto = bool (*)(SDL_Window * window);
    using GetWindowMouseRectProto = const SDL_Rect * (*)(SDL_Window * window);
    using GetWindowOpacityProto = int (*)(SDL_Window * window, float * out_opacity);
    using GetWindowPixelFormatProto = uint32_t (*)(SDL_Window * window);
    using GetWindowPositionProto = void (*)(SDL_Window * window, int *x, int *y);
    using GetWindowSizeProto = void (*)(SDL_Window * window, int *w, int *h);
    using GetWindowSizeInPixelsProto = void (*)(SDL_Window * window, int *w, int *h);
    using GetWindowSurfaceProto = SDL_Surface * (*)(SDL_Window * window);
    using GetWindowTitleProto = const char* (*)(SDL_Window * window);
    using GetYUVConversionModeProto = SDL_YUV_CONVERSION_MODE (*)(void);
    using GetYUVConversionModeForResolutionProto = SDL_YUV_CONVERSION_MODE (*)(int width, int height);
    using GL_BindTextureProto = int (*)(SDL_Texture *texture, float *texw, float *texh);
    using GL_CreateContextProto = SDL_GLContext (*)(SDL_Window * window);
    using GL_DeleteContextProto = void (*)(SDL_GLContext context);
    using GL_ExtensionSupportedProto = bool (*)(const char *extension);
    using GL_GetAttributeProto = int (*)(SDL_GLattr attr, int *value);
    using GL_GetCurrentContextProto = SDL_GLContext (*)(void);
    using GL_GetCurrentWindowProto = SDL_Window* (*)(void);
    using GL_GetDrawableSizeProto = void (*)(SDL_Window * window, int *w, int *h);
    using GL_GetProcAddressProto = void* (*)(const char *proc);
    using GL_GetSwapIntervalProto = int (*)(void);
    using GL_LoadLibraryProto = int (*)(const char *path);
    using GL_MakeCurrentProto = int (*)(SDL_Window * window, SDL_GLContext context);
    using GL_ResetAttributesProto = void (*)(void);
    using GL_SetAttributeProto = int (*)(SDL_GLattr attr, int value);
    using GL_SetSwapIntervalProto = int (*)(int interval);
    using GL_SwapWindowProto = void (*)(SDL_Window * window);
    using GL_UnbindTextureProto = int (*)(SDL_Texture *texture);
    using GL_UnloadLibraryProto = void (*)(void);
    using GUIDFromStringProto = SDL_GUID (*)(const char *pchGUID);
    using GUIDToStringProto = void (*)(SDL_GUID guid, char *pszGUID, int cbGUID);
    using HapticCloseProto = void (*)(SDL_Haptic * haptic);
    using HapticDestroyEffectProto = void (*)(SDL_Haptic * haptic, int effect);
    using HapticEffectSupportedProto = int (*)(SDL_Haptic * haptic, SDL_HapticEffect * effect);
    using HapticGetEffectStatusProto = int (*)(SDL_Haptic * haptic, int effect);
    using HapticIndexProto = int (*)(SDL_Haptic * haptic);
    using HapticNameProto = const char* (*)(int device_index);
    using HapticNewEffectProto = int (*)(SDL_Haptic * haptic, SDL_HapticEffect * effect);
    using HapticNumAxesProto = int (*)(SDL_Haptic * haptic);
    using HapticNumEffectsProto = int (*)(SDL_Haptic * haptic);
    using HapticNumEffectsPlayingProto = int (*)(SDL_Haptic * haptic);
    using HapticOpenProto = SDL_Haptic* (*)(int device_index);
    using HapticOpenedProto = int (*)(int device_index);
    using HapticOpenFromJoystickProto = SDL_Haptic* (*)(SDL_Joystick * joystick);
    using HapticOpenFromMouseProto = SDL_Haptic* (*)(void);
    using HapticPauseProto = int (*)(SDL_Haptic * haptic);
    using HapticQueryProto = unsigned int (*)(SDL_Haptic * haptic);
    using HapticRumbleInitProto = int (*)(SDL_Haptic * haptic);
    using HapticRumblePlayProto = int (*)(SDL_Haptic * haptic, float strength, uint32_t length );
    using HapticRumbleStopProto = int (*)(SDL_Haptic * haptic);
    using HapticRumbleSupportedProto = int (*)(SDL_Haptic * haptic);
    using HapticRunEffectProto = int (*)(SDL_Haptic * haptic, int effect, uint32_t iterations);
    using HapticSetAutocenterProto = int (*)(SDL_Haptic * haptic, int autocenter);
    using HapticSetGainProto = int (*)(SDL_Haptic * haptic, int gain);
    using HapticStopAllProto = int (*)(SDL_Haptic * haptic);
    using HapticStopEffectProto = int (*)(SDL_Haptic * haptic, int effect);
    using HapticUnpauseProto = int (*)(SDL_Haptic * haptic);
    using HapticUpdateEffectProto = int (*)(SDL_Haptic * haptic, int effect, SDL_HapticEffect * data);
    using Has3DNowProto = bool (*)(void);
    using HasAltiVecProto = bool (*)(void);
    using HasARMSIMDProto = bool (*)(void);
    using HasAVXProto = bool (*)(void);
    using HasAVX2Proto = bool (*)(void);
    using HasAVX512FProto = bool (*)(void);
    using HasClipboardTextProto = bool (*)(void);
    using HasColorKeyProto = bool (*)(SDL_Surface * surface);
    using HasEventProto = bool (*)(uint32_t type);
    using HasEventsProto = bool (*)(uint32_t minType, uint32_t maxType);
    using HasIntersectionProto = bool (*)(const SDL_Rect * A, const SDL_Rect * B);
    using HasIntersectionFProto = bool (*)(const SDL_FRect * A, const SDL_FRect * B);
    using HasLASXProto = bool (*)(void);
    using HasLSXProto = bool (*)(void);
    using HasMMXProto = bool (*)(void);
    using HasNEONProto = bool (*)(void);
    using HasPrimarySelectionTextProto = bool (*)(void);
    using HasRDTSCProto = bool (*)(void);
    using HasScreenKeyboardSupportProto = bool (*)(void);
    using HasSSEProto = bool (*)(void);
    using HasSSE2Proto = bool (*)(void);
    using HasSSE3Proto = bool (*)(void);
    using HasSSE41Proto = bool (*)(void);
    using HasSSE42Proto = bool (*)(void);
    using HasSurfaceRLEProto = bool (*)(SDL_Surface * surface);
    using HasWindowSurfaceProto = bool (*)(SDL_Window *window);
    using hid_ble_scanProto = void (*)(bool active);
    using hid_closeProto = void (*)(SDL_hid_device *dev);
    using hid_device_change_countProto = uint32_t (*)(void);
    using hid_enumerateProto = SDL_hid_device_info * (*)(unsigned short vendor_id, unsigned short product_id);
    using hid_exitProto = int (*)(void);
    using hid_free_enumerationProto = void (*)(SDL_hid_device_info *devs);
    using hid_get_feature_reportProto = int (*)(SDL_hid_device *dev, unsigned char *data, uint64_t length);
    using hid_get_indexed_stringProto = int (*)(SDL_hid_device *dev, int string_index, wchar_t *string, uint64_t maxlen);
    using hid_get_manufacturer_stringProto = int (*)(SDL_hid_device *dev, wchar_t *string, uint64_t maxlen);
    using hid_get_product_stringProto = int (*)(SDL_hid_device *dev, wchar_t *string, uint64_t maxlen);
    using hid_get_serial_number_stringProto = int (*)(SDL_hid_device *dev, wchar_t *string, uint64_t maxlen);
    using hid_initProto = int (*)(void);
    using hid_openProto = SDL_hid_device * (*)(unsigned short vendor_id, unsigned short product_id, const wchar_t *serial_number);
    using hid_open_pathProto = SDL_hid_device * (*)(const char *path, int bExclusive /* = false */);
    using hid_readProto = int (*)(SDL_hid_device *dev, unsigned char *data, uint64_t length);
    using hid_read_timeoutProto = int (*)(SDL_hid_device *dev, unsigned char *data, uint64_t length, int milliseconds);
    using hid_send_feature_reportProto = int (*)(SDL_hid_device *dev, const unsigned char *data, uint64_t length);
    using hid_set_nonblockingProto = int (*)(SDL_hid_device *dev, int nonblock);
    using hid_writeProto = int (*)(SDL_hid_device *dev, const unsigned char *data, uint64_t length);
    using HideWindowProto = void (*)(SDL_Window * window);
    using iconv_stringProto = char* (*)(const char *tocode, const char *fromcode, const char *inbuf, uint64_t inbytesleft);
    using InitProto = int (*)(uint32_t flags);
    using InitSubSystemProto = int (*)(uint32_t flags);
    using IntersectFRectProto = bool (*)(const SDL_FRect * A, const SDL_FRect * B, SDL_FRect * result);
    using IntersectFRectAndLineProto = bool (*)(const SDL_FRect * rect, float *X1, float *Y1, float *X2, float *Y2);
    using IntersectRectProto = bool (*)(const SDL_Rect * A, const SDL_Rect * B, SDL_Rect * result);
    using IntersectRectAndLineProto = bool (*)(const SDL_Rect * rect, int *X1, int *Y1, int *X2, int *Y2);
    using IsGameControllerProto = bool (*)(int joystick_index);
    using IsScreenKeyboardShownProto = bool (*)(SDL_Window *window);
    using IsScreenSaverEnabledProto = bool (*)(void);
    using IsShapedWindowProto = bool (*)(const SDL_Window *window);
    using IsTabletProto = bool (*)(void);
    using IsTextInputActiveProto = bool (*)(void);
    using IsTextInputShownProto = bool (*)(void);
    using JoystickAttachVirtualProto = int (*)(SDL_JoystickType type, int naxes, int nbuttons, int nhats);
    using JoystickAttachVirtualExProto = int (*)(const SDL_VirtualJoystickDesc *desc);
    using JoystickCloseProto = void (*)(SDL_Joystick *joystick);
    using JoystickCurrentPowerLevelProto = SDL_JoystickPowerLevel (*)(SDL_Joystick *joystick);
    using JoystickDetachVirtualProto = int (*)(int device_index);
    using JoystickEventStateProto = int (*)(int state);
    using JoystickFromInstanceIDProto = SDL_Joystick* (*)(SDL_JoystickID instance_id);
    using JoystickFromPlayerIndexProto = SDL_Joystick* (*)(int player_index);
    using JoystickGetAttachedProto = bool (*)(SDL_Joystick *joystick);
    using JoystickGetAxisProto = int16_t (*)(SDL_Joystick *joystick, int axis);
    using JoystickGetAxisInitialStateProto = bool (*)(SDL_Joystick *joystick, int axis, int16_t *state);
    using JoystickGetBallProto = int (*)(SDL_Joystick *joystick, int ball, int *dx, int *dy);
    using JoystickGetButtonProto = uint8_t (*)(SDL_Joystick *joystick, int button);
    using JoystickGetDeviceGUIDProto = SDL_JoystickGUID (*)(int device_index);
    using JoystickGetDeviceInstanceIDProto = SDL_JoystickID (*)(int device_index);
    using JoystickGetDevicePlayerIndexProto = int (*)(int device_index);
    using JoystickGetDeviceProductProto = uint16_t (*)(int device_index);
    using JoystickGetDeviceProductVersionProto = uint16_t (*)(int device_index);
    using JoystickGetDeviceTypeProto = SDL_JoystickType (*)(int device_index);
    using JoystickGetDeviceVendorProto = uint16_t (*)(int device_index);
    using JoystickGetFirmwareVersionProto = uint16_t (*)(SDL_Joystick *joystick);
    using JoystickGetGUIDProto = SDL_JoystickGUID (*)(SDL_Joystick *joystick);
    using JoystickGetGUIDFromStringProto = SDL_JoystickGUID (*)(const char *pchGUID);
    using JoystickGetGUIDStringProto = void (*)(SDL_JoystickGUID guid, char *pszGUID, int cbGUID);
    using JoystickGetHatProto = uint8_t (*)(SDL_Joystick *joystick, int hat);
    using JoystickGetPlayerIndexProto = int (*)(SDL_Joystick *joystick);
    using JoystickGetProductProto = uint16_t (*)(SDL_Joystick *joystick);
    using JoystickGetProductVersionProto = uint16_t (*)(SDL_Joystick *joystick);
    using JoystickGetSerialProto = const char * (*)(SDL_Joystick *joystick);
    using JoystickGetTypeProto = SDL_JoystickType (*)(SDL_Joystick *joystick);
    using JoystickGetVendorProto = uint16_t (*)(SDL_Joystick *joystick);
    using JoystickHasLEDProto = bool (*)(SDL_Joystick *joystick);
    using JoystickHasRumbleProto = bool (*)(SDL_Joystick *joystick);
    using JoystickHasRumbleTriggersProto = bool (*)(SDL_Joystick *joystick);
    using JoystickInstanceIDProto = SDL_JoystickID (*)(SDL_Joystick *joystick);
    using JoystickIsHapticProto = int (*)(SDL_Joystick * joystick);
    using JoystickIsVirtualProto = bool (*)(int device_index);
    using JoystickNameProto = const char* (*)(SDL_Joystick *joystick);
    using JoystickNameForIndexProto = const char* (*)(int device_index);
    using JoystickNumAxesProto = int (*)(SDL_Joystick *joystick);
    using JoystickNumBallsProto = int (*)(SDL_Joystick *joystick);
    using JoystickNumButtonsProto = int (*)(SDL_Joystick *joystick);
    using JoystickNumHatsProto = int (*)(SDL_Joystick *joystick);
    using JoystickOpenProto = SDL_Joystick* (*)(int device_index);
    using JoystickPathProto = const char* (*)(SDL_Joystick *joystick);
    using JoystickPathForIndexProto = const char* (*)(int device_index);
    using JoystickRumbleProto = int (*)(SDL_Joystick *joystick, uint16_t low_frequency_rumble, uint16_t high_frequency_rumble, uint32_t duration_ms);
    using JoystickRumbleTriggersProto = int (*)(SDL_Joystick *joystick, uint16_t left_rumble, uint16_t right_rumble, uint32_t duration_ms);
    using JoystickSendEffectProto = int (*)(SDL_Joystick *joystick, void *data, int size);
    using JoystickSetLEDProto = int (*)(SDL_Joystick *joystick, uint8_t red, uint8_t green, uint8_t blue);
    using JoystickSetPlayerIndexProto = void (*)(SDL_Joystick *joystick, int player_index);
    using JoystickSetVirtualAxisProto = int (*)(SDL_Joystick *joystick, int axis, int16_t value);
    using JoystickSetVirtualButtonProto = int (*)(SDL_Joystick *joystick, int button, uint8_t value);
    using JoystickSetVirtualHatProto = int (*)(SDL_Joystick *joystick, int hat, uint8_t value);
    using JoystickUpdateProto = void (*)(void);
    using LinuxSetThreadPriorityProto = int (*)(Sint64 threadID, int priority);
    using LinuxSetThreadPriorityAndPolicyProto = int (*)(Sint64 threadID, int sdlPriority, int schedPolicy);
    using LoadBMP_RWProto = SDL_Surface* (*)(SDL_RWops * src, int freesrc);
    using LoadDollarTemplatesProto = int (*)(SDL_TouchID touchId, SDL_RWops *src);
    using LoadFileProto = void* (*)(const char *file, uint64_t *datasize);
    using LoadFile_RWProto = void* (*)(SDL_RWops *src, uint64_t *datasize, int freesrc);
    using LoadFunctionProto = void* (*)(void *handle, const char *name);
    using LoadObjectProto = void* (*)(const char *sofile);
    using LoadWAV_RWProto = SDL_AudioSpec* (*)(SDL_RWops * src, int freesrc, SDL_AudioSpec * spec, uint8_t ** audio_buf, uint32_t * audio_len);
    using LockAudioProto = void (*)(void);
    using LockAudioDeviceProto = void (*)(SDL_AudioDeviceID dev);
    using LockJoysticksProto = void (*)(void);
    using LockMutexProto = int (*)(SDL_mutex * mutex);
    using LockSensorsProto = void (*)(void);
    using LockSurfaceProto = int (*)(SDL_Surface * surface);
    using LockTextureProto = int (*)(SDL_Texture * texture, const SDL_Rect * rect, void **pixels, int *pitch);
    using LockTextureToSurfaceProto = int (*)(SDL_Texture *texture, const SDL_Rect *rect, SDL_Surface **surface);
    using LogProto = void (*)(const char *fmt, ...);
    using LogCriticalProto = void (*)(int category, const char *fmt, ...);
    using LogDebugProto = void (*)(int category, const char *fmt, ...);
    using LogGetOutputFunctionProto = void (*)(SDL_LogOutputFunction *callback, void **userdata);
    using LogGetPriorityProto = SDL_LogPriority (*)(int category);
    using LogInfoProto = void (*)(int category, const char *fmt, ...);
    using LogMessageProto = void (*)(int category, SDL_LogPriority priority, const char *fmt, ...);
    using LogResetPrioritiesProto = void (*)(void);
    using LogSetAllPriorityProto = void (*)(SDL_LogPriority priority);
    using LogSetOutputFunctionProto = void (*)(SDL_LogOutputFunction callback, void *userdata);
    using LogSetPriorityProto = void (*)(int category, SDL_LogPriority priority);
    using LogVerboseProto = void (*)(int category, const char *fmt, ...);
    using LogWarnProto = void (*)(int category, const char *fmt, ...);
    using LowerBlitProto = int (*) (SDL_Surface * src, SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect);
    using LowerBlitScaledProto = int (*) (SDL_Surface * src, SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect);
    using MapRGBProto = uint32_t (*)(const SDL_PixelFormat * format, uint8_t r, uint8_t g, uint8_t b);
    using MapRGBAProto = uint32_t (*)(const SDL_PixelFormat * format, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    using MasksToPixelFormatEnumProto = uint32_t (*)(int bpp, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask);
    using MaximizeWindowProto = void (*)(SDL_Window * window);
    using MemoryBarrierReleaseFunctionProto = void (*)(void);
    using Metal_CreateViewProto = SDL_MetalView (*)(SDL_Window * window);
    using Metal_DestroyViewProto = void (*)(SDL_MetalView view);
    using Metal_GetDrawableSizeProto = void (*)(SDL_Window* window, int *w, int *h);
    using Metal_GetLayerProto = void* (*)(SDL_MetalView view);
    using MinimizeWindowProto = void (*)(SDL_Window * window);
    using MixAudioProto = void (*)(uint8_t * dst, const uint8_t * src, uint32_t len, int volume);
    using MixAudioFormatProto = void (*)(uint8_t * dst, const uint8_t * src, SDL_AudioFormat format, uint32_t len, int volume);
    using MouseIsHapticProto = int (*)(void);
    using NewAudioStreamProto = SDL_AudioStream * (*)(const SDL_AudioFormat src_format, const uint8_t src_channels, const int src_rate, const SDL_AudioFormat dst_format, const uint8_t dst_channels, const int dst_rate);
    using NumHapticsProto = int (*)(void);
    using NumJoysticksProto = int (*)(void);
    using NumSensorsProto = int (*)(void);
    using OpenAudioProto = int (*)(SDL_AudioSpec * desired, SDL_AudioSpec * obtained);
    using OpenAudioDeviceProto = SDL_AudioDeviceID (*)( const char *device, int iscapture, const SDL_AudioSpec *desired, SDL_AudioSpec *obtained, int allowed_changes);
    using OpenURLProto = int (*)(const char *url);
    using PauseAudioProto = void (*)(int pause_on);
    using PauseAudioDeviceProto = void (*)(SDL_AudioDeviceID dev, int pause_on);
    using PeepEventsProto = int (*)(SDL_Event * events, int numevents, SDL_eventaction action, uint32_t minType, uint32_t maxType);
    using PixelFormatEnumToMasksProto = bool (*)(uint32_t format, int *bpp, uint32_t * Rmask, uint32_t * Gmask, uint32_t * Bmask, uint32_t * Amask);
    using PollEventProto = int (*)(SDL_Event * event);
    using PremultiplyAlphaProto = int (*)(int width, int height, uint32_t src_format, void * src, int src_pitch, uint32_t dst_format, void * dst, int dst_pitch);
    using PumpEventsProto = void (*)(void);
    using PushEventProto = int (*)(SDL_Event * event);
    using QueryTextureProto = int (*)(SDL_Texture * texture, uint32_t * format, int *access, int *w, int *h);
    using QueueAudioProto = int (*)(SDL_AudioDeviceID dev, void *data, uint32_t len);
    using QuitProto = void (*)(void);
    using QuitSubSystemProto = void (*)(uint32_t flags);
    using RaiseWindowProto = void (*)(SDL_Window * window);
    using ReadBE16Proto = uint16_t (*)(SDL_RWops * src);
    using ReadBE32Proto = uint32_t (*)(SDL_RWops * src);
    using ReadBE64Proto = uint64_t (*)(SDL_RWops * src);
    using ReadLE16Proto = uint16_t (*)(SDL_RWops * src);
    using ReadLE32Proto = uint32_t (*)(SDL_RWops * src);
    using ReadLE64Proto = uint64_t (*)(SDL_RWops * src);
    using ReadU8Proto = uint8_t (*)(SDL_RWops * src);
    using RecordGestureProto = int (*)(SDL_TouchID touchId);
    using RegisterEventsProto = uint32_t (*)(int numevents);
    using RemoveTimerProto = bool (*)(SDL_TimerID id);
    using RenderClearProto = int (*)(SDL_Renderer * renderer);
    using RenderCopyProto = int (*)(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect);
    using RenderCopyExProto = int (*)(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect, const double angle, const SDL_Point *center, const SDL_RendererFlip flip);
    using RenderCopyExFProto = int (*)(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_FRect * dstrect, const double angle, const SDL_FPoint *center, const SDL_RendererFlip flip);
    using RenderCopyFProto = int (*)(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_FRect * dstrect);
    using RenderDrawLineProto = int (*)(SDL_Renderer * renderer, int x1, int y1, int x2, int y2);
    using RenderDrawLineFProto = int (*)(SDL_Renderer * renderer, float x1, float y1, float x2, float y2);
    using RenderDrawLinesProto = int (*)(SDL_Renderer * renderer, const SDL_Point * points, int count);
    using RenderDrawLinesFProto = int (*)(SDL_Renderer * renderer, const SDL_FPoint * points, int count);
    using RenderDrawPointProto = int (*)(SDL_Renderer * renderer, int x, int y);
    using RenderDrawPointFProto = int (*)(SDL_Renderer * renderer, float x, float y);
    using RenderDrawPointsProto = int (*)(SDL_Renderer * renderer, const SDL_Point * points, int count);
    using RenderDrawPointsFProto = int (*)(SDL_Renderer * renderer, const SDL_FPoint * points, int count);
    using RenderDrawRectProto = int (*)(SDL_Renderer * renderer, const SDL_Rect * rect);
    using RenderDrawRectFProto = int (*)(SDL_Renderer * renderer, const SDL_FRect * rect);
    using RenderDrawRectsProto = int (*)(SDL_Renderer * renderer, const SDL_Rect * rects, int count);
    using RenderDrawRectsFProto = int (*)(SDL_Renderer * renderer, const SDL_FRect * rects, int count);
    using RenderFillRectProto = int (*)(SDL_Renderer * renderer, const SDL_Rect * rect);
    using RenderFillRectFProto = int (*)(SDL_Renderer * renderer, const SDL_FRect * rect);
    using RenderFillRectsProto = int (*)(SDL_Renderer * renderer, const SDL_Rect * rects, int count);
    using RenderFillRectsFProto = int (*)(SDL_Renderer * renderer, const SDL_FRect * rects, int count);
    using RenderFlushProto = int (*)(SDL_Renderer * renderer);
    using RenderGeometryProto = int (*)(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Vertex *vertices, int num_vertices, const int *indices, int num_indices);
    using RenderGeometryRawProto = int (*)(SDL_Renderer *renderer, SDL_Texture *texture, const float *xy, int xy_stride, const SDL_Color *color, int color_stride, const float *uv, int uv_stride, int num_vertices, void *indices, int num_indices, int size_indices);
    using RenderGetClipRectProto = void (*)(SDL_Renderer * renderer, SDL_Rect * rect);
    using RenderGetIntegerScaleProto = bool (*)(SDL_Renderer * renderer);
    using RenderGetLogicalSizeProto = void (*)(SDL_Renderer * renderer, int *w, int *h);
    using RenderGetMetalCommandEncoderProto = void* (*)(SDL_Renderer * renderer);
    using RenderGetMetalLayerProto = void* (*)(SDL_Renderer * renderer);
    using RenderGetScaleProto = void (*)(SDL_Renderer * renderer, float *scaleX, float *scaleY);
    using RenderGetViewportProto = void (*)(SDL_Renderer * renderer, SDL_Rect * rect);
    using RenderGetWindowProto = SDL_Window * (*)(SDL_Renderer *renderer);
    using RenderIsClipEnabledProto = bool (*)(SDL_Renderer * renderer);
    using RenderLogicalToWindowProto = void (*)(SDL_Renderer * renderer, float logicalX, float logicalY, int *windowX, int *windowY);
    using RenderPresentProto = void (*)(SDL_Renderer * renderer);
    using RenderReadPixelsProto = int (*)(SDL_Renderer * renderer, const SDL_Rect * rect, uint32_t format, void *pixels, int pitch);
    using RenderSetClipRectProto = int (*)(SDL_Renderer * renderer, const SDL_Rect * rect);
    using RenderSetIntegerScaleProto = int (*)(SDL_Renderer * renderer, bool enable);
    using RenderSetLogicalSizeProto = int (*)(SDL_Renderer * renderer, int w, int h);
    using RenderSetScaleProto = int (*)(SDL_Renderer * renderer, float scaleX, float scaleY);
    using RenderSetViewportProto = int (*)(SDL_Renderer * renderer, const SDL_Rect * rect);
    using RenderSetVSyncProto = int (*)(SDL_Renderer* renderer, int vsync);
    using RenderTargetSupportedProto = bool (*)(SDL_Renderer *renderer);
    using RenderWindowToLogicalProto = void (*)(SDL_Renderer * renderer, int windowX, int windowY, float *logicalX, float *logicalY);
    using ResetAssertionReportProto = void (*)(void);
    using ResetHintProto = bool (*)(const char *name);
    using ResetHintsProto = void (*)(void);
    using ResetKeyboardProto = void (*)(void);
    using RestoreWindowProto = void (*)(SDL_Window * window);
    using RWcloseProto = int (*)(SDL_RWops *context);
    using RWFromConstMemProto = SDL_RWops* (*)(void *mem, int size);
    using RWFromFileProto = SDL_RWops* (*)(const char *file, const char *mode);
    using RWFromFPProto = SDL_RWops* (*)(void * fp, bool autoclose);
    using RWFromMemProto = SDL_RWops* (*)(void *mem, int size);
    using RWreadProto = uint64_t (*)(SDL_RWops *context, void *ptr, uint64_t size, uint64_t maxnum);
    using RWseekProto = Sint64 (*)(SDL_RWops *context, Sint64 offset, int whence);
    using RWsizeProto = Sint64 (*)(SDL_RWops *context);
    using RWtellProto = Sint64 (*)(SDL_RWops *context);
    using RWwriteProto = uint64_t (*)(SDL_RWops *context, void *ptr, uint64_t size, uint64_t num);
    using SaveAllDollarTemplatesProto = int (*)(SDL_RWops *dst);
    using SaveBMP_RWProto = int (*) (SDL_Surface * surface, SDL_RWops * dst, int freedst);
    using SaveDollarTemplateProto = int (*)(SDL_GestureID gestureId,SDL_RWops *dst);
    using SemPostProto = int (*)(SDL_sem * sem);
    using SemTryWaitProto = int (*)(SDL_sem * sem);
    using SemValueProto = uint32_t (*)(SDL_sem * sem);
    using SemWaitProto = int (*)(SDL_sem * sem);
    using SemWaitTimeoutProto = int (*)(SDL_sem *sem, uint32_t timeout);
    using SensorCloseProto = void (*)(SDL_Sensor *sensor);
    using SensorFromInstanceIDProto = SDL_Sensor* (*)(SDL_SensorID instance_id);
    using SensorGetDataProto = int (*)(SDL_Sensor *sensor, float *data, int num_values);
    using SensorGetDataWithTimestampProto = int (*)(SDL_Sensor *sensor, uint64_t *timestamp, float *data, int num_values);
    using SensorGetDeviceInstanceIDProto = SDL_SensorID (*)(int device_index);
    using SensorGetDeviceNameProto = const char* (*)(int device_index);
    using SensorGetDeviceNonPortableTypeProto = int (*)(int device_index);
    using SensorGetDeviceTypeProto = SDL_SensorType (*)(int device_index);
    using SensorGetInstanceIDProto = SDL_SensorID (*)(SDL_Sensor *sensor);
    using SensorGetNameProto = const char* (*)(SDL_Sensor *sensor);
    using SensorGetNonPortableTypeProto = int (*)(SDL_Sensor *sensor);
    using SensorGetTypeProto = SDL_SensorType (*)(SDL_Sensor *sensor);
    using SensorOpenProto = SDL_Sensor* (*)(int device_index);
    using SensorUpdateProto = void (*)(void);
    using SetAssertionHandlerProto = void (*)( SDL_AssertionHandler handler, void *userdata);
    using SetClipboardTextProto = int (*)(const char *text);
    using SetClipRectProto = bool (*)(SDL_Surface * surface, const SDL_Rect * rect);
    using SetColorKeyProto = int (*)(SDL_Surface * surface, int flag, uint32_t key);
    using SetCursorProto = void (*)(SDL_Cursor * cursor);
    using SetErrorProto = int (*)(const char *fmt, ...);
    using SetEventFilterProto = void (*)(SDL_EventFilter filter, void *userdata);
    using SetHintProto = bool (*)(const char *name, const char *value);
    using SetHintWithPriorityProto = bool (*)(const char *name, const char *value, SDL_HintPriority priority);
    using SetMainReadyProto = void (*)(void);
    using SetMemoryFunctionsProto = int (*)(SDL_malloc_func malloc_func, SDL_calloc_func calloc_func, SDL_realloc_func realloc_func, SDL_free_func free_func);
    using SetModStateProto = void (*)(SDL_Keymod modstate);
    using SetPaletteColorsProto = int (*)(SDL_Palette * palette, const SDL_Color * colors, int firstcolor, int ncolors);
    using SetPixelFormatPaletteProto = int (*)(SDL_PixelFormat * format, SDL_Palette *palette);
    using SetPrimarySelectionTextProto = int (*)(const char *text);
    using SetRelativeMouseModeProto = int (*)(bool enabled);
    using SetRenderDrawBlendModeProto = int (*)(SDL_Renderer * renderer, SDL_BlendMode blendMode);
    using SetRenderDrawColorProto = int (*)(SDL_Renderer * renderer, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    using SetRenderTargetProto = int (*)(SDL_Renderer *renderer, SDL_Texture *texture);
    using SetSurfaceAlphaModProto = int (*)(SDL_Surface * surface, uint8_t alpha);
    using SetSurfaceBlendModeProto = int (*)(SDL_Surface * surface, SDL_BlendMode blendMode);
    using SetSurfaceColorModProto = int (*)(SDL_Surface * surface, uint8_t r, uint8_t g, uint8_t b);
    using SetSurfacePaletteProto = int (*)(SDL_Surface * surface, SDL_Palette * palette);
    using SetSurfaceRLEProto = int (*)(SDL_Surface * surface, int flag);
    using SetTextInputRectProto = void (*)(const SDL_Rect *rect);
    using SetTextureAlphaModProto = int (*)(SDL_Texture * texture, uint8_t alpha);
    using SetTextureBlendModeProto = int (*)(SDL_Texture * texture, SDL_BlendMode blendMode);
    using SetTextureColorModProto = int (*)(SDL_Texture * texture, uint8_t r, uint8_t g, uint8_t b);
    using SetTextureScaleModeProto = int (*)(SDL_Texture * texture, SDL_ScaleMode scaleMode);
    using SetTextureUserDataProto = int (*)(SDL_Texture * texture, void *userdata);
    using SetThreadPriorityProto = int (*)(SDL_ThreadPriority priority);
    using SetWindowAlwaysOnTopProto = void (*)(SDL_Window * window, bool on_top);
    using SetWindowBorderedProto = void (*)(SDL_Window * window, bool bordered);
    using SetWindowBrightnessProto = int (*)(SDL_Window * window, float brightness);
    using SetWindowDataProto = void* (*)(SDL_Window * window, const char *name, void *userdata);
    using SetWindowDisplayModeProto = int (*)(SDL_Window * window, const SDL_DisplayMode * mode);
    using SetWindowFullscreenProto = int (*)(SDL_Window * window, uint32_t flags);
    using SetWindowGammaRampProto = int (*)(SDL_Window * window, const uint16_t * red, const uint16_t * green, const uint16_t * blue);
    using SetWindowGrabProto = void (*)(SDL_Window * window, bool grabbed);
    using SetWindowHitTestProto = int (*)(SDL_Window * window, SDL_HitTest callback, void *callback_data);
    using SetWindowIconProto = void (*)(SDL_Window * window, SDL_Surface * icon);
    using SetWindowInputFocusProto = int (*)(SDL_Window * window);
    using SetWindowKeyboardGrabProto = void (*)(SDL_Window * window, bool grabbed);
    using SetWindowMaximumSizeProto = void (*)(SDL_Window * window, int max_w, int max_h);
    using SetWindowMinimumSizeProto = void (*)(SDL_Window * window, int min_w, int min_h);
    using SetWindowModalForProto = int (*)(SDL_Window * modal_window, SDL_Window * parent_window);
    using SetWindowMouseGrabProto = void (*)(SDL_Window * window, bool grabbed);
    using SetWindowMouseRectProto = int (*)(SDL_Window * window, const SDL_Rect * rect);
    using SetWindowOpacityProto = int (*)(SDL_Window * window, float opacity);
    using SetWindowPositionProto = void (*)(SDL_Window * window, int x, int y);
    using SetWindowResizableProto = void (*)(SDL_Window * window, bool resizable);
    using SetWindowShapeProto = int (*)(SDL_Window *window,SDL_Surface *shape,SDL_WindowShapeMode *shape_mode);
    using SetWindowSizeProto = void (*)(SDL_Window * window, int w, int h);
    using SetWindowTitleProto = void (*)(SDL_Window * window, const char *title);
    using SetYUVConversionModeProto = void (*)(SDL_YUV_CONVERSION_MODE mode);
    using ShowCursorProto = int (*)(int toggle);
    using ShowMessageBoxProto = int (*)(const SDL_MessageBoxData *messageboxdata, int *buttonid);
    using ShowSimpleMessageBoxProto = int (*)(uint32_t flags, const char *title, const char *message, SDL_Window *window);
    using ShowWindowProto = void (*)(SDL_Window * window);
    using SIMDAllocProto = void * (*)(const uint64_t len);
    using SIMDFreeProto = void (*)(void *ptr);
    using SIMDGetAlignmentProto = uint64_t (*)(void);
    using SIMDReallocProto = void * (*)(void *mem, const uint64_t len);
    using SoftStretchProto = int (*)(SDL_Surface * src, const SDL_Rect * srcrect, SDL_Surface * dst, const SDL_Rect * dstrect);
    using SoftStretchLinearProto = int (*)(SDL_Surface * src, const SDL_Rect * srcrect, SDL_Surface * dst, const SDL_Rect * dstrect);
    using StartTextInputProto = void (*)(void);
    using StopTextInputProto = void (*)(void);
    using ThreadIDProto = SDL_threadID (*)(void);
    using TLSCleanupProto = void (*)(void);
    using TLSCreateProto = SDL_TLSID (*)(void);
    using TLSGetProto = void * (*)(SDL_TLSID id);
    using TLSSetProto = int (*)(SDL_TLSID id, void *value, void ( *destructor)(void*));
    using TryLockMutexProto = int (*)(SDL_mutex * mutex);
    using UnionFRectProto = void (*)(const SDL_FRect * A, const SDL_FRect * B, SDL_FRect * result);
    using UnionRectProto = void (*)(const SDL_Rect * A, const SDL_Rect * B, SDL_Rect * result);
    using UnloadObjectProto = void (*)(void *handle);
    using UnlockAudioProto = void (*)(void);
    using UnlockAudioDeviceProto = void (*)(SDL_AudioDeviceID dev);
    using UnlockJoysticksProto = void (*)(void);
    using UnlockMutexProto = int (*)(SDL_mutex * mutex);
    using UnlockSurfaceProto = void (*)(SDL_Surface * surface);
    using UnlockTextureProto = void (*)(SDL_Texture * texture);
    using UpdateNVTextureProto = int (*)(SDL_Texture * texture, const SDL_Rect * rect, const uint8_t *Yplane, int Ypitch, const uint8_t *UVplane, int UVpitch);
    using UpdateTextureProto = int (*)(SDL_Texture * texture, const SDL_Rect * rect, void *pixels, int pitch);
    using UpdateWindowSurfaceProto = int (*)(SDL_Window * window);
    using UpdateWindowSurfaceRectsProto = int (*)(SDL_Window * window, const SDL_Rect * rects, int numrects);
    using UpdateYUVTextureProto = int (*)(SDL_Texture * texture, const SDL_Rect * rect, const uint8_t *Yplane, int Ypitch, const uint8_t *Uplane, int Upitch, const uint8_t *Vplane, int Vpitch);
    using UpperBlitProto = int (*) (SDL_Surface * src, const SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect);
    using UpperBlitScaledProto = int (*) (SDL_Surface * src, const SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect);
    using VideoInitProto = int (*)(const char *driver_name);
    using VideoQuitProto = void (*)(void);
#ifdef COSMO_SDL2_VULKAN
    using Vulkan_CreateSurfaceProto = bool (*)(SDL_Window *window, VkInstance instance, VkSurfaceKHR* surface);
    using Vulkan_GetDrawableSizeProto = void (*)(SDL_Window * window, int *w, int *h);
    using Vulkan_GetInstanceExtensionsProto = bool (*)(SDL_Window *window, unsigned int *pCount, const char **pNames);
    using Vulkan_GetVkGetInstanceProcAddrProto = void* (*)(void);
    using Vulkan_GetVkInstanceProcAddrProto = void * (*)(void);
    using Vulkan_LoadLibraryProto = int (*)(const char *path);
    using Vulkan_UnloadLibraryProto = void (*)(void);
#endif
    using WaitEventProto = int (*)(SDL_Event * event);
    using WaitEventTimeoutProto = int (*)(SDL_Event * event, int timeout);
    using WaitThreadProto = void (*)(SDL_Thread * thread, int *status);
    using WarpMouseGlobalProto = int (*)(int x, int y);
    using WarpMouseInWindowProto = void (*)(SDL_Window * window, int x, int y);
    using WasInitProto = uint32_t (*)(uint32_t flags);
    using WriteBE16Proto = uint64_t (*)(SDL_RWops * dst, uint16_t value);
    using WriteBE32Proto = uint64_t (*)(SDL_RWops * dst, uint32_t value);
    using WriteBE64Proto = uint64_t (*)(SDL_RWops * dst, uint64_t value);
    using WriteLE16Proto = uint64_t (*)(SDL_RWops * dst, uint16_t value);
    using WriteLE32Proto = uint64_t (*)(SDL_RWops * dst, uint32_t value);
    using WriteLE64Proto = uint64_t (*)(SDL_RWops * dst, uint64_t value);
    using WriteU8Proto = uint64_t (*)(SDL_RWops * dst, uint8_t value);
    using NetAddSocketProto = int (*)(SDLNet_SocketSet set, SDLNet_GenericSocket sock);
    using NetAllocPacketProto = UDPpacket * (*)(int size);
    using NetAllocPacketVProto = UDPpacket ** (*)(int howmany, int size);
    using NetAllocSocketSetProto = SDLNet_SocketSet (*)(int maxsockets);
    using NetCheckSocketsProto = int (*)(SDLNet_SocketSet set, Uint32 timeout);
    using NetDelSocketProto = int (*)(SDLNet_SocketSet set, SDLNet_GenericSocket sock);
    using NetFreePacketProto = void (*)(UDPpacket *packet);
    using NetFreePacketVProto = void (*)(UDPpacket **packetV);
    using NetFreeSocketSetProto = void (*)(SDLNet_SocketSet set);
    using NetGetErrorProto = const char * (*)(void);
    using NetGetLocalAddressesProto = int (*)(IPaddress *addresses, int maxcount);
    using NetInitProto = int (*)(void);
    using NetLinked_VersionProto = const SDLNet_version * (*)(void);
    using NetQuitProto = void (*)(void);
    using NetResizePacketProto = int (*)(UDPpacket *packet, int newsize);
    using NetResolveHostProto = int (*)(IPaddress *address, const char *host, uint16_t port);
    using NetResolveIPProto = const char * (*)(const IPaddress *ip);
    using NetTCP_AcceptProto = TCPsocket (*)(TCPsocket server);
    using NetTCP_CloseProto = void (*)(TCPsocket sock);
    using NetTCP_GetPeerAddressProto = IPaddress * (*)(TCPsocket sock);
    using NetTCP_OpenProto = TCPsocket (*)(IPaddress *ip);
    using NetTCP_RecvProto = int (*)(TCPsocket sock, void *data, int maxlen);
    using NetTCP_SendProto = int (*)(TCPsocket sock, void *data, int len);
    using NetUDP_BindProto = int (*)(UDPsocket sock, int channel, const IPaddress *address);
    using NetUDP_CloseProto = void (*)(UDPsocket sock);
    using NetUDP_GetPeerAddressProto = IPaddress * (*)(UDPsocket sock, int channel);
    using NetUDP_OpenProto = UDPsocket (*)(uint16_t port);
    using NetUDP_RecvProto = int (*)(UDPsocket sock, UDPpacket *packet);
    using NetUDP_RecvVProto = int (*)(UDPsocket sock, UDPpacket **packets);
    using NetUDP_SendProto = int (*)(UDPsocket sock, int channel, UDPpacket *packet);
    using NetUDP_SendVProto = int (*)(UDPsocket sock, UDPpacket **packets, int npackets);
    using NetUDP_SetPacketLossProto = void (*)(UDPsocket sock, int percent);
    using NetUDP_UnbindProto = void (*)(UDPsocket sock, int channel);
    using MixAllocateChannelsProto = int (*)(int numchans);
    using MixChannelFinishedProto = void (*)(void (*channel_finished)(int channel));
    using MixCloseAudioProto = void (*)(void);
    using MixEachSoundFontProto = int (*)(int (*function)(const char*, void*), void *data);
    using MixExpireChannelProto = int (*)(int channel, int ticks);
    using MixFadeInChannelProto = int (*)(int channel, Mix_Chunk *chunk, int loops, int ms);
    using MixFadeInChannelTimedProto = int (*)(int channel, Mix_Chunk *chunk, int loops, int ms, int ticks);
    using MixFadeInMusicProto = int (*)(Mix_Music *music, int loops, int ms);
    using MixFadeInMusicPosProto = int (*)(Mix_Music *music, int loops, int ms, double position);
    using MixFadeOutChannelProto = int (*)(int which, int ms);
    using MixFadeOutGroupProto = int (*)(int tag, int ms);
    using MixFadeOutMusicProto = int (*)(int ms);
    using MixFadingChannelProto = Mix_Fading (*)(int which);
    using MixFadingMusicProto = Mix_Fading (*)(void);
    using MixFreeChunkProto = void (*)(Mix_Chunk *chunk);
    using MixFreeMusicProto = void (*)(Mix_Music *music);
    using MixGetChunkProto = Mix_Chunk * (*)(int channel);
    using MixGetChunkDecoderProto = const char * (*)(int index);
    using MixGetMusicAlbumTagProto = const char* (*)(const Mix_Music *music);
    using MixGetMusicArtistTagProto = const char* (*)(const Mix_Music *music);
    using MixGetMusicCopyrightTagProto = const char* (*)(const Mix_Music *music);
    using MixGetMusicDecoderProto = const char * (*)(int index);
    using MixGetMusicHookDataProto = void * (*)(void);
    using MixGetMusicLoopEndTimeProto = double (*)(Mix_Music *music);
    using MixGetMusicLoopLengthTimeProto = double (*)(Mix_Music *music);
    using MixGetMusicLoopStartTimeProto = double (*)(Mix_Music *music);
    using MixGetMusicPositionProto = double (*)(Mix_Music *music);
    using MixGetMusicTitleProto = const char* (*)(const Mix_Music *music);
    using MixGetMusicTitleTagProto = const char* (*)(const Mix_Music *music);
    using MixGetMusicTypeProto = Mix_MusicType (*)(const Mix_Music *music);
    using MixGetMusicVolumeProto = int (*)(Mix_Music *music);
    using MixGetNumChunkDecodersProto = int (*)(void);
    using MixGetNumMusicDecodersProto = int (*)(void);
    using MixGetSoundFontsProto = const char* (*)(void);
    using MixGetSynchroValueProto = int (*)(void);
    using MixGetTimidityCfgProto = const char* (*)(void);
    using MixGroupAvailableProto = int (*)(int tag);
    using MixGroupChannelProto = int (*)(int which, int tag);
    using MixGroupChannelsProto = int (*)(int from, int to, int tag);
    using MixGroupCountProto = int (*)(int tag);
    using MixGroupNewerProto = int (*)(int tag);
    using MixGroupOldestProto = int (*)(int tag);
    using MixHaltChannelProto = int (*)(int channel);
    using MixHaltGroupProto = int (*)(int tag);
    using MixHaltMusicProto = int (*)(void);
    using MixHasChunkDecoderProto = bool (*)(const char *name);
    using MixHasMusicDecoderProto = bool (*)(const char *name);
    using MixHookMusicProto = void (*)(void ( *mix_func)(void *udata, Uint8 *stream, int len), void *arg);
    using MixHookMusicFinishedProto = void (*)(void ( *music_finished)(void));
    using MixInitProto = int (*)(int flags);
    using MixLinked_VersionProto = const SDL_version * (*)(void);
    using MixLoadMUSProto = Mix_Music * (*)(const char *file);
    using MixLoadMUS_RWProto = Mix_Music * (*)(SDL_RWops *src, int freesrc);
    using MixLoadMUSType_RWProto = Mix_Music * (*)(SDL_RWops *src, Mix_MusicType type, int freesrc);
    using MixLoadWAVProto = Mix_Chunk * (*)(const char *file);
    using MixLoadWAV_RWProto = Mix_Chunk * (*)(SDL_RWops *src, int freesrc);
    using MixMasterVolumeProto = int (*)(int volume);
    using MixModMusicJumpToOrderProto = int (*)(int order);
    using MixMusicDurationProto = double (*)(Mix_Music *music);
    using MixOpenAudioProto = int (*)(int frequency, uint16_t format, int channels, int chunksize);
    using MixOpenAudioDeviceProto = int (*)(int frequency, uint16_t format, int channels, int chunksize, const char* device, int allowed_changes);
    using MixPauseProto = void (*)(int channel);
    using MixPausedProto = int (*)(int channel);
    using MixPausedMusicProto = int (*)(void);
    using MixPauseMusicProto = void (*)(void);
    using MixPlayChannelProto = int (*)(int channel, Mix_Chunk *chunk, int loops);
    using MixPlayChannelTimedProto = int (*)(int channel, Mix_Chunk *chunk, int loops, int ticks);
    using MixPlayingProto = int (*)(int channel);
    using MixPlayingMusicProto = int (*)(void);
    using MixPlayMusicProto = int (*)(Mix_Music *music, int loops);
    using MixQuerySpecProto = int (*)(int *frequency, uint16_t *format, int *channels);
    using MixQuickLoad_RAWProto = Mix_Chunk * (*)(Uint8 *mem, Uint32 len);
    using MixQuickLoad_WAVProto = Mix_Chunk * (*)(Uint8 *mem);
    using MixQuitProto = void (*)(void);
    using MixRegisterEffectProto = int (*)(int chan, Mix_EffectFunc_t f, Mix_EffectDone_t d, void *arg);
    using MixReserveChannelsProto = int (*)(int num);
    using MixResumeProto = void (*)(int channel);
    using MixResumeMusicProto = void (*)(void);
    using MixRewindMusicProto = void (*)(void);
    using MixSetDistanceProto = int (*)(int channel, Uint8 distance);
    using MixSetMusicCMDProto = int (*)(const char *command);
    using MixSetMusicPositionProto = int (*)(double position);
    using MixSetPanningProto = int (*)(int channel, Uint8 left, Uint8 right);
    using MixSetPositionProto = int (*)(int channel, Sint16 angle, Uint8 distance);
    using MixSetPostMixProto = void (*)(void (*mix_func)(void *udata, Uint8 *stream, int len), void *arg);
    using MixSetReverseStereoProto = int (*)(int channel, int flip);
    using MixSetSoundFontsProto = int (*)(const char *paths);
    using MixSetSynchroValueProto = int (*)(int value);
    using MixSetTimidityCfgProto = int (*)(const char *path);
    using MixUnregisterAllEffectsProto = int (*)(int channel);
    using MixUnregisterEffectProto = int (*)(int channel, Mix_EffectFunc_t f);
    using MixVolumeProto = int (*)(int channel, int volume);
    using MixVolumeChunkProto = int (*)(Mix_Chunk *chunk, int volume);
    using MixVolumeMusicProto = int (*)(int volume);
    using TTFByteSwappedUNICODEProto = void (*)(bool swapped);
    using TTFCloseFontProto = void (*)(TTF_Font *font);
    using TTFFontAscentProto = int (*)(const TTF_Font *font);
    using TTFFontDescentProto = int (*)(const TTF_Font *font);
    using TTFFontFaceFamilyNameProto = const char * (*)(const TTF_Font *font);
    using TTFFontFaceIsFixedWidthProto = int (*)(const TTF_Font *font);
    using TTFFontFacesProto = long (*)(const TTF_Font *font);
    using TTFFontFaceStyleNameProto = const char * (*)(const TTF_Font *font);
    using TTFFontHeightProto = int (*)(const TTF_Font *font);
    using TTFFontLineSkipProto = int (*)(const TTF_Font *font);
    using TTFGetFontHintingProto = int (*)(const TTF_Font *font);
    using TTFGetFontKerningProto = int (*)(const TTF_Font *font);
    using TTFGetFontOutlineProto = int (*)(const TTF_Font *font);
    using TTFGetFontStyleProto = int (*)(const TTF_Font *font);
    using TTFGetFontWrappedAlignProto = int (*)(const TTF_Font *font);
    using TTFGetFreeTypeVersionProto = void (*)(int *major, int *minor, int *patch);
    using TTFGetHarfBuzzVersionProto = void (*)(int *major, int *minor, int *patch);
    using TTFGlyphIsProvidedProto = int (*)(TTF_Font *font, uint16_t ch);
    using TTFGlyphIsProvided32Proto = int (*)(TTF_Font *font, Uint32 ch);
    using TTFGlyphMetricsProto = int (*)(TTF_Font *font, uint16_t ch, int *minx, int *maxx, int *miny, int *maxy, int *advance);
    using TTFGlyphMetrics32Proto = int (*)(TTF_Font *font, Uint32 ch, int *minx, int *maxx, int *miny, int *maxy, int *advance);
    using TTFInitProto = int (*)(void);
    using TTFLinked_VersionProto = const SDL_version * (*)(void);
    using TTFMeasureTextProto = int (*)(TTF_Font *font, const char *text, int measure_width, int *extent, int *count);
    using TTFMeasureUNICODEProto = int (*)(TTF_Font *font, const uint16_t *text, int measure_width, int *extent, int *count);
    using TTFMeasureUTF8Proto = int (*)(TTF_Font *font, const char *text, int measure_width, int *extent, int *count);
    using TTFOpenFontProto = TTF_Font * (*)(const char *file, int ptsize);
    using TTFOpenFontDPIProto = TTF_Font * (*)(const char *file, int ptsize, unsigned int hdpi, unsigned int vdpi);
    using TTFOpenFontDPIRWProto = TTF_Font * (*)(SDL_RWops *src, int freesrc, int ptsize, unsigned int hdpi, unsigned int vdpi);
    using TTFOpenFontIndexProto = TTF_Font * (*)(const char *file, int ptsize, long index);
    using TTFOpenFontIndexDPIProto = TTF_Font * (*)(const char *file, int ptsize, long index, unsigned int hdpi, unsigned int vdpi);
    using TTFOpenFontIndexDPIRWProto = TTF_Font * (*)(SDL_RWops *src, int freesrc, int ptsize, long index, unsigned int hdpi, unsigned int vdpi);
    using TTFOpenFontIndexRWProto = TTF_Font * (*)(SDL_RWops *src, int freesrc, int ptsize, long index);
    using TTFOpenFontRWProto = TTF_Font * (*)(SDL_RWops *src, int freesrc, int ptsize);
    using TTFQuitProto = void (*)(void);
    using TTFRenderGlyph32_BlendedProto = SDL_Surface * (*)(TTF_Font *font, Uint32 ch, SDL_Color fg);
    using TTFRenderGlyph32_LCDProto = SDL_Surface * (*)(TTF_Font *font, Uint32 ch, SDL_Color fg, SDL_Color bg);
    using TTFRenderGlyph32_ShadedProto = SDL_Surface * (*)(TTF_Font *font, Uint32 ch, SDL_Color fg, SDL_Color bg);
    using TTFRenderGlyph32_SolidProto = SDL_Surface * (*)(TTF_Font *font, Uint32 ch, SDL_Color fg);
    using TTFRenderGlyph_BlendedProto = SDL_Surface * (*)(TTF_Font *font, uint16_t ch, SDL_Color fg);
    using TTFRenderGlyph_LCDProto = SDL_Surface * (*)(TTF_Font *font, uint16_t ch, SDL_Color fg, SDL_Color bg);
    using TTFRenderGlyph_ShadedProto = SDL_Surface * (*)(TTF_Font *font, uint16_t ch, SDL_Color fg, SDL_Color bg);
    using TTFRenderGlyph_SolidProto = SDL_Surface * (*)(TTF_Font *font, uint16_t ch, SDL_Color fg);
    using TTFRenderText_BlendedProto = SDL_Surface * (*)(TTF_Font *font, const char *text, SDL_Color fg);
    using TTFRenderText_Blended_WrappedProto = SDL_Surface * (*)(TTF_Font *font, const char *text, SDL_Color fg, Uint32 wrapLength);
    using TTFRenderText_LCDProto = SDL_Surface * (*)(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg);
    using TTFRenderText_LCD_WrappedProto = SDL_Surface * (*)(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);
    using TTFRenderText_ShadedProto = SDL_Surface * (*)(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg);
    using TTFRenderText_Shaded_WrappedProto = SDL_Surface * (*)(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);
    using TTFRenderText_SolidProto = SDL_Surface * (*)(TTF_Font *font, const char *text, SDL_Color fg);
    using TTFRenderText_Solid_WrappedProto = SDL_Surface * (*)(TTF_Font *font, const char *text, SDL_Color fg, Uint32 wrapLength);
    using TTFRenderUNICODE_BlendedProto = SDL_Surface * (*)(TTF_Font *font, const uint16_t *text, SDL_Color fg);
    using TTFRenderUNICODE_Blended_WrappedProto = SDL_Surface * (*)(TTF_Font *font, const uint16_t *text, SDL_Color fg, Uint32 wrapLength);
    using TTFRenderUNICODE_LCDProto = SDL_Surface * (*)(TTF_Font *font, const uint16_t *text, SDL_Color fg, SDL_Color bg);
    using TTFRenderUNICODE_LCD_WrappedProto = SDL_Surface * (*)(TTF_Font *font, const uint16_t *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);
    using TTFRenderUNICODE_ShadedProto = SDL_Surface * (*)(TTF_Font *font, const uint16_t *text, SDL_Color fg, SDL_Color bg);
    using TTFRenderUNICODE_Shaded_WrappedProto = SDL_Surface * (*)(TTF_Font *font, const uint16_t *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);
    using TTFRenderUNICODE_SolidProto = SDL_Surface * (*)(TTF_Font *font, const uint16_t *text, SDL_Color fg);
    using TTFRenderUNICODE_Solid_WrappedProto = SDL_Surface * (*)(TTF_Font *font, const uint16_t *text, SDL_Color fg, Uint32 wrapLength);
    using TTFRenderUTF8_BlendedProto = SDL_Surface * (*)(TTF_Font *font, const char *text, SDL_Color fg);
    using TTFRenderUTF8_Blended_WrappedProto = SDL_Surface * (*)(TTF_Font *font, const char *text, SDL_Color fg, Uint32 wrapLength);
    using TTFRenderUTF8_LCDProto = SDL_Surface * (*)(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg);
    using TTFRenderUTF8_LCD_WrappedProto = SDL_Surface * (*)(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);
    using TTFRenderUTF8_ShadedProto = SDL_Surface * (*)(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg);
    using TTFRenderUTF8_Shaded_WrappedProto = SDL_Surface * (*)(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);
    using TTFRenderUTF8_SolidProto = SDL_Surface * (*)(TTF_Font *font, const char *text, SDL_Color fg);
    using TTFRenderUTF8_Solid_WrappedProto = SDL_Surface * (*)(TTF_Font *font, const char *text, SDL_Color fg, Uint32 wrapLength);
    using TTFSetDirectionProto = int (*)(int direction) /* hb_direction_t */;
    using TTFSetFontDirectionProto = int (*)(TTF_Font *font, TTF_Direction direction);
    using TTFSetFontHintingProto = void (*)(TTF_Font *font, int hinting);
    using TTFSetFontKerningProto = void (*)(TTF_Font *font, int allowed);
    using TTFSetFontOutlineProto = void (*)(TTF_Font *font, int outline);
    using TTFSetFontScriptNameProto = int (*)(TTF_Font *font, const char *script);
    using TTFSetFontSizeProto = int (*)(TTF_Font *font, int ptsize);
    using TTFSetFontSizeDPIProto = int (*)(TTF_Font *font, int ptsize, unsigned int hdpi, unsigned int vdpi);
    using TTFSetFontStyleProto = void (*)(TTF_Font *font, int style);
    using TTFSetFontWrappedAlignProto = void (*)(TTF_Font *font, int align);
    using TTFSetScriptProto = int (*)(int script) /* hb_script_t */;
    using TTFSizeTextProto = int (*)(TTF_Font *font, const char *text, int *w, int *h);
    using TTFSizeUNICODEProto = int (*)(TTF_Font *font, const uint16_t *text, int *w, int *h);
    using TTFSizeUTF8Proto = int (*)(TTF_Font *font, const char *text, int *w, int *h);
    using TTFWasInitProto = int (*)(void);
    using IMGFreeAnimationProto = void (*)(IMG_Animation *anim);
    using IMGInitProto = int (*)(int flags);
    using IMGisAVIFProto = int (*)(SDL_RWops *src);
    using IMGisBMPProto = int (*)(SDL_RWops *src);
    using IMGisCURProto = int (*)(SDL_RWops *src);
    using IMGisGIFProto = int (*)(SDL_RWops *src);
    using IMGisICOProto = int (*)(SDL_RWops *src);
    using IMGisJPGProto = int (*)(SDL_RWops *src);
    using IMGisJXLProto = int (*)(SDL_RWops *src);
    using IMGisLBMProto = int (*)(SDL_RWops *src);
    using IMGisPCXProto = int (*)(SDL_RWops *src);
    using IMGisPNGProto = int (*)(SDL_RWops *src);
    using IMGisPNMProto = int (*)(SDL_RWops *src);
    using IMGisQOIProto = int (*)(SDL_RWops *src);
    using IMGisSVGProto = int (*)(SDL_RWops *src);
    using IMGisTIFProto = int (*)(SDL_RWops *src);
    using IMGisWEBPProto = int (*)(SDL_RWops *src);
    using IMGisXCFProto = int (*)(SDL_RWops *src);
    using IMGisXPMProto = int (*)(SDL_RWops *src);
    using IMGisXVProto = int (*)(SDL_RWops *src);
    using IMGLinked_VersionProto = const SDL_version * (*)(void);
    using IMGLoadProto = SDL_Surface * (*)(const char *file);
    using IMGLoad_RWProto = SDL_Surface * (*)(SDL_RWops *src, int freesrc);
    using IMGLoadAnimationProto = IMG_Animation * (*)(const char *file);
    using IMGLoadAnimation_RWProto = IMG_Animation * (*)(SDL_RWops *src, int freesrc);
    using IMGLoadAnimationTyped_RWProto = IMG_Animation * (*)(SDL_RWops *src, int freesrc, const char *type);
    using IMGLoadAVIF_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadBMP_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadCUR_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadGIF_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadGIFAnimation_RWProto = IMG_Animation * (*)(SDL_RWops *src);
    using IMGLoadICO_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadJPG_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadJXL_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadLBM_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadPCX_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadPNG_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadPNM_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadQOI_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadSizedSVG_RWProto = SDL_Surface * (*)(SDL_RWops *src, int width, int height);
    using IMGLoadSVG_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadTextureProto = SDL_Texture * (*)(SDL_Renderer *renderer, const char *file);
    using IMGLoadTexture_RWProto = SDL_Texture * (*)(SDL_Renderer *renderer, SDL_RWops *src, int freesrc);
    using IMGLoadTextureTyped_RWProto = SDL_Texture * (*)(SDL_Renderer *renderer, SDL_RWops *src, int freesrc, const char *type);
    using IMGLoadTGA_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadTIF_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadTyped_RWProto = SDL_Surface * (*)(SDL_RWops *src, int freesrc, const char *type);
    using IMGLoadWEBP_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadXCF_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadXPM_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGLoadXV_RWProto = SDL_Surface * (*)(SDL_RWops *src);
    using IMGQuitProto = void (*)(void);
    using IMGReadXPMFromArrayProto = SDL_Surface * (*)(char **xpm);
    using IMGReadXPMFromArrayToRGB888Proto = SDL_Surface * (*)(char **xpm);
    using IMGSaveJPGProto = int (*)(SDL_Surface *surface, const char *file, int quality);
    using IMGSaveJPG_RWProto = int (*)(SDL_Surface *surface, SDL_RWops *dst, int freedst, int quality);
    using IMGSavePNGProto = int (*)(SDL_Surface *surface, const char *file);
    using IMGSavePNG_RWProto = int (*)(SDL_Surface *surface, SDL_RWops *dst, int freedst);
#endif

    /* SDL2 functions prototypes with Microsoft ABI */

#if 1 // To hide the list
    using AddEventWatchProto_WIN = MSABI AddEventWatchProto;
    using AddHintCallbackProto_WIN = MSABI AddHintCallbackProto;
    using AddTimerProto_WIN = MSABI AddTimerProto;
    using AllocFormatProto_WIN = MSABI AllocFormatProto;
    using AllocPaletteProto_WIN = MSABI AllocPaletteProto;
    using AllocRWProto_WIN = MSABI AllocRWProto;
    using AtomicAddProto_WIN = MSABI AtomicAddProto;
    using AtomicCASProto_WIN = MSABI AtomicCASProto;
    using AtomicCASPtrProto_WIN = MSABI AtomicCASPtrProto;
    using AtomicGetProto_WIN = MSABI AtomicGetProto;
    using AtomicGetPtrProto_WIN = MSABI AtomicGetPtrProto;
    using AtomicLockProto_WIN = MSABI AtomicLockProto;
    using AtomicSetProto_WIN = MSABI AtomicSetProto;
    using AtomicSetPtrProto_WIN = MSABI AtomicSetPtrProto;
    using AtomicTryLockProto_WIN = MSABI AtomicTryLockProto;
    using AtomicUnlockProto_WIN = MSABI AtomicUnlockProto;
    using AudioInitProto_WIN = MSABI AudioInitProto;
    using AudioQuitProto_WIN = MSABI AudioQuitProto;
    using AudioStreamAvailableProto_WIN = MSABI AudioStreamAvailableProto;
    using AudioStreamClearProto_WIN = MSABI AudioStreamClearProto;
    using AudioStreamFlushProto_WIN = MSABI AudioStreamFlushProto;
    using AudioStreamGetProto_WIN = MSABI AudioStreamGetProto;
    using AudioStreamPutProto_WIN = MSABI AudioStreamPutProto;
    using BlitSurfaceProto_WIN = MSABI BlitSurfaceProto;
    using BuildAudioCVTProto_WIN = MSABI BuildAudioCVTProto;
    using CalculateGammaRampProto_WIN = MSABI CalculateGammaRampProto;
    using CaptureMouseProto_WIN = MSABI CaptureMouseProto;
    using ClearCompositionProto_WIN = MSABI ClearCompositionProto;
    using ClearErrorProto_WIN = MSABI ClearErrorProto;
    using ClearHintsProto_WIN = MSABI ClearHintsProto;
    using ClearQueuedAudioProto_WIN = MSABI ClearQueuedAudioProto;
    using CloseAudioProto_WIN = MSABI CloseAudioProto;
    using CloseAudioDeviceProto_WIN = MSABI CloseAudioDeviceProto;
    using ComposeCustomBlendModeProto_WIN = MSABI ComposeCustomBlendModeProto;
    using CondBroadcastProto_WIN = MSABI CondBroadcastProto;
    using CondSignalProto_WIN = MSABI CondSignalProto;
    using CondWaitProto_WIN = MSABI CondWaitProto;
    using CondWaitTimeoutProto_WIN = MSABI CondWaitTimeoutProto;
    using ConvertAudioProto_WIN = MSABI ConvertAudioProto;
    using ConvertPixelsProto_WIN = MSABI ConvertPixelsProto;
    using ConvertSurfaceProto_WIN = MSABI ConvertSurfaceProto;
    using ConvertSurfaceFormatProto_WIN = MSABI ConvertSurfaceFormatProto;
    using CreateColorCursorProto_WIN = MSABI CreateColorCursorProto;
    using CreateCondProto_WIN = MSABI CreateCondProto;
    using CreateCursorProto_WIN = MSABI CreateCursorProto;
    using CreateMutexProto_WIN = MSABI CreateMutexProto;
    using CreateRendererProto_WIN = MSABI CreateRendererProto;
    using CreateRGBSurfaceProto_WIN = MSABI CreateRGBSurfaceProto;
    using CreateRGBSurfaceFromProto_WIN = MSABI CreateRGBSurfaceFromProto;
    using CreateRGBSurfaceWithFormatProto_WIN = MSABI CreateRGBSurfaceWithFormatProto;
    using CreateRGBSurfaceWithFormatFromProto_WIN = MSABI CreateRGBSurfaceWithFormatFromProto;
    using CreateSemaphoreProto_WIN = MSABI CreateSemaphoreProto;
    using CreateShapedWindowProto_WIN = MSABI CreateShapedWindowProto;
    using CreateSoftwareRendererProto_WIN = MSABI CreateSoftwareRendererProto;
    using CreateSystemCursorProto_WIN = MSABI CreateSystemCursorProto;
    using CreateTextureProto_WIN = MSABI CreateTextureProto;
    using CreateTextureFromSurfaceProto_WIN = MSABI CreateTextureFromSurfaceProto;
    using CreateThreadProto_WIN = MSABI CreateThreadProto;
    using CreateThreadWithStackSizeProto_WIN = MSABI CreateThreadWithStackSizeProto;
    using CreateWindowProto_WIN = MSABI CreateWindowProto;
    using CreateWindowAndRendererProto_WIN = MSABI CreateWindowAndRendererProto;
    using CreateWindowFromProto_WIN = MSABI CreateWindowFromProto;
    using DelayProto_WIN = MSABI DelayProto;
    using DelEventWatchProto_WIN = MSABI DelEventWatchProto;
    using DelHintCallbackProto_WIN = MSABI DelHintCallbackProto;
    using DequeueAudioProto_WIN = MSABI DequeueAudioProto;
    using DestroyCondProto_WIN = MSABI DestroyCondProto;
    using DestroyMutexProto_WIN = MSABI DestroyMutexProto;
    using DestroyRendererProto_WIN = MSABI DestroyRendererProto;
    using DestroySemaphoreProto_WIN = MSABI DestroySemaphoreProto;
    using DestroyTextureProto_WIN = MSABI DestroyTextureProto;
    using DestroyWindowProto_WIN = MSABI DestroyWindowProto;
    using DestroyWindowSurfaceProto_WIN = MSABI DestroyWindowSurfaceProto;
    using DetachThreadProto_WIN = MSABI DetachThreadProto;
    using DisableScreenSaverProto_WIN = MSABI DisableScreenSaverProto;
    using EnableScreenSaverProto_WIN = MSABI EnableScreenSaverProto;
    using EncloseFPointsProto_WIN = MSABI EncloseFPointsProto;
    using EnclosePointsProto_WIN = MSABI EnclosePointsProto;
    using EventStateProto_WIN = MSABI EventStateProto;
    using FillRectProto_WIN = MSABI FillRectProto;
    using FillRectsProto_WIN = MSABI FillRectsProto;
    using FilterEventsProto_WIN = MSABI FilterEventsProto;
    using FlashWindowProto_WIN = MSABI FlashWindowProto;
    using FlushEventProto_WIN = MSABI FlushEventProto;
    using FlushEventsProto_WIN = MSABI FlushEventsProto;
    using FreeAudioStreamProto_WIN = MSABI FreeAudioStreamProto;
    using FreeCursorProto_WIN = MSABI FreeCursorProto;
    using FreeFormatProto_WIN = MSABI FreeFormatProto;
    using FreePaletteProto_WIN = MSABI FreePaletteProto;
    using FreeRWProto_WIN = MSABI FreeRWProto;
    using FreeSurfaceProto_WIN = MSABI FreeSurfaceProto;
    using FreeWAVProto_WIN = MSABI FreeWAVProto;
    using GameControllerAddMappingProto_WIN = MSABI GameControllerAddMappingProto;
    using GameControllerAddMappingsFromRWProto_WIN = MSABI GameControllerAddMappingsFromRWProto;
    using GameControllerCloseProto_WIN = MSABI GameControllerCloseProto;
    using GameControllerEventStateProto_WIN = MSABI GameControllerEventStateProto;
    using GameControllerFromInstanceIDProto_WIN = MSABI GameControllerFromInstanceIDProto;
    using GameControllerFromPlayerIndexProto_WIN = MSABI GameControllerFromPlayerIndexProto;
    using GameControllerGetAppleSFSymbolsNameForAxisProto_WIN = MSABI GameControllerGetAppleSFSymbolsNameForAxisProto;
    using GameControllerGetAppleSFSymbolsNameForButtonProto_WIN = MSABI GameControllerGetAppleSFSymbolsNameForButtonProto;
    using GameControllerGetAttachedProto_WIN = MSABI GameControllerGetAttachedProto;
    using GameControllerGetAxisProto_WIN = MSABI GameControllerGetAxisProto;
    using GameControllerGetAxisFromStringProto_WIN = MSABI GameControllerGetAxisFromStringProto;
    using GameControllerGetBindForAxisProto_WIN = MSABI GameControllerGetBindForAxisProto;
    using GameControllerGetBindForButtonProto_WIN = MSABI GameControllerGetBindForButtonProto;
    using GameControllerGetButtonProto_WIN = MSABI GameControllerGetButtonProto;
    using GameControllerGetButtonFromStringProto_WIN = MSABI GameControllerGetButtonFromStringProto;
    using GameControllerGetFirmwareVersionProto_WIN = MSABI GameControllerGetFirmwareVersionProto;
    using GameControllerGetJoystickProto_WIN = MSABI GameControllerGetJoystickProto;
    using GameControllerGetNumTouchpadFingersProto_WIN = MSABI GameControllerGetNumTouchpadFingersProto;
    using GameControllerGetNumTouchpadsProto_WIN = MSABI GameControllerGetNumTouchpadsProto;
    using GameControllerGetPlayerIndexProto_WIN = MSABI GameControllerGetPlayerIndexProto;
    using GameControllerGetProductProto_WIN = MSABI GameControllerGetProductProto;
    using GameControllerGetProductVersionProto_WIN = MSABI GameControllerGetProductVersionProto;
    using GameControllerGetSensorDataProto_WIN = MSABI GameControllerGetSensorDataProto;
    using GameControllerGetSensorDataRateProto_WIN = MSABI GameControllerGetSensorDataRateProto;
    using GameControllerGetSensorDataWithTimestampProto_WIN = MSABI GameControllerGetSensorDataWithTimestampProto;
    using GameControllerGetSerialProto_WIN = MSABI GameControllerGetSerialProto;
    using GameControllerGetStringForAxisProto_WIN = MSABI GameControllerGetStringForAxisProto;
    using GameControllerGetStringForButtonProto_WIN = MSABI GameControllerGetStringForButtonProto;
    using GameControllerGetTouchpadFingerProto_WIN = MSABI GameControllerGetTouchpadFingerProto;
    using GameControllerGetTypeProto_WIN = MSABI GameControllerGetTypeProto;
    using GameControllerGetVendorProto_WIN = MSABI GameControllerGetVendorProto;
    using GameControllerHasAxisProto_WIN = MSABI GameControllerHasAxisProto;
    using GameControllerHasButtonProto_WIN = MSABI GameControllerHasButtonProto;
    using GameControllerHasLEDProto_WIN = MSABI GameControllerHasLEDProto;
    using GameControllerHasRumbleProto_WIN = MSABI GameControllerHasRumbleProto;
    using GameControllerHasRumbleTriggersProto_WIN = MSABI GameControllerHasRumbleTriggersProto;
    using GameControllerHasSensorProto_WIN = MSABI GameControllerHasSensorProto;
    using GameControllerIsSensorEnabledProto_WIN = MSABI GameControllerIsSensorEnabledProto;
    using GameControllerMappingProto_WIN = MSABI GameControllerMappingProto;
    using GameControllerMappingForDeviceIndexProto_WIN = MSABI GameControllerMappingForDeviceIndexProto;
    using GameControllerMappingForGUIDProto_WIN = MSABI GameControllerMappingForGUIDProto;
    using GameControllerMappingForIndexProto_WIN = MSABI GameControllerMappingForIndexProto;
    using GameControllerNameProto_WIN = MSABI GameControllerNameProto;
    using GameControllerNameForIndexProto_WIN = MSABI GameControllerNameForIndexProto;
    using GameControllerNumMappingsProto_WIN = MSABI GameControllerNumMappingsProto;
    using GameControllerOpenProto_WIN = MSABI GameControllerOpenProto;
    using GameControllerPathProto_WIN = MSABI GameControllerPathProto;
    using GameControllerPathForIndexProto_WIN = MSABI GameControllerPathForIndexProto;
    using GameControllerRumbleProto_WIN = MSABI GameControllerRumbleProto;
    using GameControllerRumbleTriggersProto_WIN = MSABI GameControllerRumbleTriggersProto;
    using GameControllerSendEffectProto_WIN = MSABI GameControllerSendEffectProto;
    using GameControllerSetLEDProto_WIN = MSABI GameControllerSetLEDProto;
    using GameControllerSetPlayerIndexProto_WIN = MSABI GameControllerSetPlayerIndexProto;
    using GameControllerSetSensorEnabledProto_WIN = MSABI GameControllerSetSensorEnabledProto;
    using GameControllerTypeForIndexProto_WIN = MSABI GameControllerTypeForIndexProto;
    using GameControllerUpdateProto_WIN = MSABI GameControllerUpdateProto;
    using GetAssertionHandlerProto_WIN = MSABI GetAssertionHandlerProto;
    using GetAssertionReportProto_WIN = MSABI GetAssertionReportProto;
    using GetAudioDeviceNameProto_WIN = MSABI GetAudioDeviceNameProto;
    using GetAudioDeviceSpecProto_WIN = MSABI GetAudioDeviceSpecProto;
    using GetAudioDeviceStatusProto_WIN = MSABI GetAudioDeviceStatusProto;
    using GetAudioDriverProto_WIN = MSABI GetAudioDriverProto;
    using GetAudioStatusProto_WIN = MSABI GetAudioStatusProto;
    using GetBasePathProto_WIN = MSABI GetBasePathProto;
    using GetClipboardTextProto_WIN = MSABI GetClipboardTextProto;
    using GetClipRectProto_WIN = MSABI GetClipRectProto;
    using GetClosestDisplayModeProto_WIN = MSABI GetClosestDisplayModeProto;
    using GetColorKeyProto_WIN = MSABI GetColorKeyProto;
    using GetCPUCacheLineSizeProto_WIN = MSABI GetCPUCacheLineSizeProto;
    using GetCPUCountProto_WIN = MSABI GetCPUCountProto;
    using GetCurrentAudioDriverProto_WIN = MSABI GetCurrentAudioDriverProto;
    using GetCurrentDisplayModeProto_WIN = MSABI GetCurrentDisplayModeProto;
    using GetCurrentVideoDriverProto_WIN = MSABI GetCurrentVideoDriverProto;
    using GetCursorProto_WIN = MSABI GetCursorProto;
    using GetDefaultAssertionHandlerProto_WIN = MSABI GetDefaultAssertionHandlerProto;
    using GetDefaultAudioInfoProto_WIN = MSABI GetDefaultAudioInfoProto;
    using GetDefaultCursorProto_WIN = MSABI GetDefaultCursorProto;
    using GetDesktopDisplayModeProto_WIN = MSABI GetDesktopDisplayModeProto;
    using GetDisplayBoundsProto_WIN = MSABI GetDisplayBoundsProto;
    using GetDisplayDPIProto_WIN = MSABI GetDisplayDPIProto;
    using GetDisplayModeProto_WIN = MSABI GetDisplayModeProto;
    using GetDisplayNameProto_WIN = MSABI GetDisplayNameProto;
    using GetDisplayOrientationProto_WIN = MSABI GetDisplayOrientationProto;
    using GetDisplayUsableBoundsProto_WIN = MSABI GetDisplayUsableBoundsProto;
    using GetErrorProto_WIN = MSABI GetErrorProto;
    using GetErrorMsgProto_WIN = MSABI GetErrorMsgProto;
    using GetEventFilterProto_WIN = MSABI GetEventFilterProto;
    using GetGlobalMouseStateProto_WIN = MSABI GetGlobalMouseStateProto;
    using GetGrabbedWindowProto_WIN = MSABI GetGrabbedWindowProto;
    using GetHintProto_WIN = MSABI GetHintProto;
    using GetHintBooleanProto_WIN = MSABI GetHintBooleanProto;
    using GetJoystickGUIDInfoProto_WIN = MSABI GetJoystickGUIDInfoProto;
    using GetKeyboardFocusProto_WIN = MSABI GetKeyboardFocusProto;
    using GetKeyboardStateProto_WIN = MSABI GetKeyboardStateProto;
    using GetKeyFromNameProto_WIN = MSABI GetKeyFromNameProto;
    using GetKeyFromScancodeProto_WIN = MSABI GetKeyFromScancodeProto;
    using GetKeyNameProto_WIN = MSABI GetKeyNameProto;
    using GetMemoryFunctionsProto_WIN = MSABI GetMemoryFunctionsProto;
    using GetModStateProto_WIN = MSABI GetModStateProto;
    using GetMouseFocusProto_WIN = MSABI GetMouseFocusProto;
    using GetMouseStateProto_WIN = MSABI GetMouseStateProto;
    using GetNumAllocationsProto_WIN = MSABI GetNumAllocationsProto;
    using GetNumAudioDevicesProto_WIN = MSABI GetNumAudioDevicesProto;
    using GetNumAudioDriversProto_WIN = MSABI GetNumAudioDriversProto;
    using GetNumDisplayModesProto_WIN = MSABI GetNumDisplayModesProto;
    using GetNumRenderDriversProto_WIN = MSABI GetNumRenderDriversProto;
    using GetNumTouchDevicesProto_WIN = MSABI GetNumTouchDevicesProto;
    using GetNumTouchFingersProto_WIN = MSABI GetNumTouchFingersProto;
    using GetNumVideoDisplaysProto_WIN = MSABI GetNumVideoDisplaysProto;
    using GetNumVideoDriversProto_WIN = MSABI GetNumVideoDriversProto;
    using GetOriginalMemoryFunctionsProto_WIN = MSABI GetOriginalMemoryFunctionsProto;
    using GetPerformanceCounterProto_WIN = MSABI GetPerformanceCounterProto;
    using GetPerformanceFrequencyProto_WIN = MSABI GetPerformanceFrequencyProto;
    using GetPixelFormatNameProto_WIN = MSABI GetPixelFormatNameProto;
    using GetPlatformProto_WIN = MSABI GetPlatformProto;
    using GetPointDisplayIndexProto_WIN = MSABI GetPointDisplayIndexProto;
    using GetPowerInfoProto_WIN = MSABI GetPowerInfoProto;
    using GetPreferredLocalesProto_WIN = MSABI GetPreferredLocalesProto;
    using GetPrefPathProto_WIN = MSABI GetPrefPathProto;
    using GetPrimarySelectionTextProto_WIN = MSABI GetPrimarySelectionTextProto;
    using GetQueuedAudioSizeProto_WIN = MSABI GetQueuedAudioSizeProto;
    using GetRectDisplayIndexProto_WIN = MSABI GetRectDisplayIndexProto;
    using GetRelativeMouseModeProto_WIN = MSABI GetRelativeMouseModeProto;
    using GetRelativeMouseStateProto_WIN = MSABI GetRelativeMouseStateProto;
    using GetRenderDrawBlendModeProto_WIN = MSABI GetRenderDrawBlendModeProto;
    using GetRenderDrawColorProto_WIN = MSABI GetRenderDrawColorProto;
    using GetRenderDriverInfoProto_WIN = MSABI GetRenderDriverInfoProto;
    using GetRendererProto_WIN = MSABI GetRendererProto;
    using GetRendererInfoProto_WIN = MSABI GetRendererInfoProto;
    using GetRendererOutputSizeProto_WIN = MSABI GetRendererOutputSizeProto;
    using GetRenderTargetProto_WIN = MSABI GetRenderTargetProto;
    using GetRevisionProto_WIN = MSABI GetRevisionProto;
    using GetRevisionNumberProto_WIN = MSABI GetRevisionNumberProto;
    using GetRGBProto_WIN = MSABI GetRGBProto;
    using GetRGBAProto_WIN = MSABI GetRGBAProto;
    using GetScancodeFromKeyProto_WIN = MSABI GetScancodeFromKeyProto;
    using GetScancodeFromNameProto_WIN = MSABI GetScancodeFromNameProto;
    using GetScancodeNameProto_WIN = MSABI GetScancodeNameProto;
    using GetShapedWindowModeProto_WIN = MSABI GetShapedWindowModeProto;
    using GetSurfaceAlphaModProto_WIN = MSABI GetSurfaceAlphaModProto;
    using GetSurfaceBlendModeProto_WIN = MSABI GetSurfaceBlendModeProto;
    using GetSurfaceColorModProto_WIN = MSABI GetSurfaceColorModProto;
    using GetSystemRAMProto_WIN = MSABI GetSystemRAMProto;
    using GetTextureAlphaModProto_WIN = MSABI GetTextureAlphaModProto;
    using GetTextureBlendModeProto_WIN = MSABI GetTextureBlendModeProto;
    using GetTextureColorModProto_WIN = MSABI GetTextureColorModProto;
    using GetTextureScaleModeProto_WIN = MSABI GetTextureScaleModeProto;
    using GetTextureUserDataProto_WIN = MSABI GetTextureUserDataProto;
    using GetThreadIDProto_WIN = MSABI GetThreadIDProto;
    using GetThreadNameProto_WIN = MSABI GetThreadNameProto;
    using GetTicksProto_WIN = MSABI GetTicksProto;
    using GetTicks64Proto_WIN = MSABI GetTicks64Proto;
    using GetTouchDeviceProto_WIN = MSABI GetTouchDeviceProto;
    using GetTouchDeviceTypeProto_WIN = MSABI GetTouchDeviceTypeProto;
    using GetTouchFingerProto_WIN = MSABI GetTouchFingerProto;
    using GetTouchNameProto_WIN = MSABI GetTouchNameProto;
    using GetVersionProto_WIN = MSABI GetVersionProto;
    using GetVideoDriverProto_WIN = MSABI GetVideoDriverProto;
    using GetWindowBordersSizeProto_WIN = MSABI GetWindowBordersSizeProto;
    using GetWindowBrightnessProto_WIN = MSABI GetWindowBrightnessProto;
    using GetWindowDataProto_WIN = MSABI GetWindowDataProto;
    using GetWindowDisplayIndexProto_WIN = MSABI GetWindowDisplayIndexProto;
    using GetWindowDisplayModeProto_WIN = MSABI GetWindowDisplayModeProto;
    using GetWindowFlagsProto_WIN = MSABI GetWindowFlagsProto;
    using GetWindowFromIDProto_WIN = MSABI GetWindowFromIDProto;
    using GetWindowGammaRampProto_WIN = MSABI GetWindowGammaRampProto;
    using GetWindowGrabProto_WIN = MSABI GetWindowGrabProto;
    using GetWindowICCProfileProto_WIN = MSABI GetWindowICCProfileProto;
    using GetWindowIDProto_WIN = MSABI GetWindowIDProto;
    using GetWindowKeyboardGrabProto_WIN = MSABI GetWindowKeyboardGrabProto;
    using GetWindowMaximumSizeProto_WIN = MSABI GetWindowMaximumSizeProto;
    using GetWindowMinimumSizeProto_WIN = MSABI GetWindowMinimumSizeProto;
    using GetWindowMouseGrabProto_WIN = MSABI GetWindowMouseGrabProto;
    using GetWindowMouseRectProto_WIN = MSABI GetWindowMouseRectProto;
    using GetWindowOpacityProto_WIN = MSABI GetWindowOpacityProto;
    using GetWindowPixelFormatProto_WIN = MSABI GetWindowPixelFormatProto;
    using GetWindowPositionProto_WIN = MSABI GetWindowPositionProto;
    using GetWindowSizeProto_WIN = MSABI GetWindowSizeProto;
    using GetWindowSizeInPixelsProto_WIN = MSABI GetWindowSizeInPixelsProto;
    using GetWindowSurfaceProto_WIN = MSABI GetWindowSurfaceProto;
    using GetWindowTitleProto_WIN = MSABI GetWindowTitleProto;
    using GetYUVConversionModeProto_WIN = MSABI GetYUVConversionModeProto;
    using GetYUVConversionModeForResolutionProto_WIN = MSABI GetYUVConversionModeForResolutionProto;
    using GL_BindTextureProto_WIN = MSABI GL_BindTextureProto;
    using GL_CreateContextProto_WIN = MSABI GL_CreateContextProto;
    using GL_DeleteContextProto_WIN = MSABI GL_DeleteContextProto;
    using GL_ExtensionSupportedProto_WIN = MSABI GL_ExtensionSupportedProto;
    using GL_GetAttributeProto_WIN = MSABI GL_GetAttributeProto;
    using GL_GetCurrentContextProto_WIN = MSABI GL_GetCurrentContextProto;
    using GL_GetCurrentWindowProto_WIN = MSABI GL_GetCurrentWindowProto;
    using GL_GetDrawableSizeProto_WIN = MSABI GL_GetDrawableSizeProto;
    using GL_GetProcAddressProto_WIN = MSABI GL_GetProcAddressProto;
    using GL_GetSwapIntervalProto_WIN = MSABI GL_GetSwapIntervalProto;
    using GL_LoadLibraryProto_WIN = MSABI GL_LoadLibraryProto;
    using GL_MakeCurrentProto_WIN = MSABI GL_MakeCurrentProto;
    using GL_ResetAttributesProto_WIN = MSABI GL_ResetAttributesProto;
    using GL_SetAttributeProto_WIN = MSABI GL_SetAttributeProto;
    using GL_SetSwapIntervalProto_WIN = MSABI GL_SetSwapIntervalProto;
    using GL_SwapWindowProto_WIN = MSABI GL_SwapWindowProto;
    using GL_UnbindTextureProto_WIN = MSABI GL_UnbindTextureProto;
    using GL_UnloadLibraryProto_WIN = MSABI GL_UnloadLibraryProto;
    using GUIDFromStringProto_WIN = MSABI GUIDFromStringProto;
    using GUIDToStringProto_WIN = MSABI GUIDToStringProto;
    using HapticCloseProto_WIN = MSABI HapticCloseProto;
    using HapticDestroyEffectProto_WIN = MSABI HapticDestroyEffectProto;
    using HapticEffectSupportedProto_WIN = MSABI HapticEffectSupportedProto;
    using HapticGetEffectStatusProto_WIN = MSABI HapticGetEffectStatusProto;
    using HapticIndexProto_WIN = MSABI HapticIndexProto;
    using HapticNameProto_WIN = MSABI HapticNameProto;
    using HapticNewEffectProto_WIN = MSABI HapticNewEffectProto;
    using HapticNumAxesProto_WIN = MSABI HapticNumAxesProto;
    using HapticNumEffectsProto_WIN = MSABI HapticNumEffectsProto;
    using HapticNumEffectsPlayingProto_WIN = MSABI HapticNumEffectsPlayingProto;
    using HapticOpenProto_WIN = MSABI HapticOpenProto;
    using HapticOpenedProto_WIN = MSABI HapticOpenedProto;
    using HapticOpenFromJoystickProto_WIN = MSABI HapticOpenFromJoystickProto;
    using HapticOpenFromMouseProto_WIN = MSABI HapticOpenFromMouseProto;
    using HapticPauseProto_WIN = MSABI HapticPauseProto;
    using HapticQueryProto_WIN = MSABI HapticQueryProto;
    using HapticRumbleInitProto_WIN = MSABI HapticRumbleInitProto;
    using HapticRumblePlayProto_WIN = MSABI HapticRumblePlayProto;
    using HapticRumbleStopProto_WIN = MSABI HapticRumbleStopProto;
    using HapticRumbleSupportedProto_WIN = MSABI HapticRumbleSupportedProto;
    using HapticRunEffectProto_WIN = MSABI HapticRunEffectProto;
    using HapticSetAutocenterProto_WIN = MSABI HapticSetAutocenterProto;
    using HapticSetGainProto_WIN = MSABI HapticSetGainProto;
    using HapticStopAllProto_WIN = MSABI HapticStopAllProto;
    using HapticStopEffectProto_WIN = MSABI HapticStopEffectProto;
    using HapticUnpauseProto_WIN = MSABI HapticUnpauseProto;
    using HapticUpdateEffectProto_WIN = MSABI HapticUpdateEffectProto;
    using Has3DNowProto_WIN = MSABI Has3DNowProto;
    using HasAltiVecProto_WIN = MSABI HasAltiVecProto;
    using HasARMSIMDProto_WIN = MSABI HasARMSIMDProto;
    using HasAVXProto_WIN = MSABI HasAVXProto;
    using HasAVX2Proto_WIN = MSABI HasAVX2Proto;
    using HasAVX512FProto_WIN = MSABI HasAVX512FProto;
    using HasClipboardTextProto_WIN = MSABI HasClipboardTextProto;
    using HasColorKeyProto_WIN = MSABI HasColorKeyProto;
    using HasEventProto_WIN = MSABI HasEventProto;
    using HasEventsProto_WIN = MSABI HasEventsProto;
    using HasIntersectionProto_WIN = MSABI HasIntersectionProto;
    using HasIntersectionFProto_WIN = MSABI HasIntersectionFProto;
    using HasLASXProto_WIN = MSABI HasLASXProto;
    using HasLSXProto_WIN = MSABI HasLSXProto;
    using HasMMXProto_WIN = MSABI HasMMXProto;
    using HasNEONProto_WIN = MSABI HasNEONProto;
    using HasPrimarySelectionTextProto_WIN = MSABI HasPrimarySelectionTextProto;
    using HasRDTSCProto_WIN = MSABI HasRDTSCProto;
    using HasScreenKeyboardSupportProto_WIN = MSABI HasScreenKeyboardSupportProto;
    using HasSSEProto_WIN = MSABI HasSSEProto;
    using HasSSE2Proto_WIN = MSABI HasSSE2Proto;
    using HasSSE3Proto_WIN = MSABI HasSSE3Proto;
    using HasSSE41Proto_WIN = MSABI HasSSE41Proto;
    using HasSSE42Proto_WIN = MSABI HasSSE42Proto;
    using HasSurfaceRLEProto_WIN = MSABI HasSurfaceRLEProto;
    using HasWindowSurfaceProto_WIN = MSABI HasWindowSurfaceProto;
    using hid_ble_scanProto_WIN = MSABI hid_ble_scanProto;
    using hid_closeProto_WIN = MSABI hid_closeProto;
    using hid_device_change_countProto_WIN = MSABI hid_device_change_countProto;
    using hid_enumerateProto_WIN = MSABI hid_enumerateProto;
    using hid_exitProto_WIN = MSABI hid_exitProto;
    using hid_free_enumerationProto_WIN = MSABI hid_free_enumerationProto;
    using hid_get_feature_reportProto_WIN = MSABI hid_get_feature_reportProto;
    using hid_get_indexed_stringProto_WIN = MSABI hid_get_indexed_stringProto;
    using hid_get_manufacturer_stringProto_WIN = MSABI hid_get_manufacturer_stringProto;
    using hid_get_product_stringProto_WIN = MSABI hid_get_product_stringProto;
    using hid_get_serial_number_stringProto_WIN = MSABI hid_get_serial_number_stringProto;
    using hid_initProto_WIN = MSABI hid_initProto;
    using hid_openProto_WIN = MSABI hid_openProto;
    using hid_open_pathProto_WIN = MSABI hid_open_pathProto;
    using hid_readProto_WIN = MSABI hid_readProto;
    using hid_read_timeoutProto_WIN = MSABI hid_read_timeoutProto;
    using hid_send_feature_reportProto_WIN = MSABI hid_send_feature_reportProto;
    using hid_set_nonblockingProto_WIN = MSABI hid_set_nonblockingProto;
    using hid_writeProto_WIN = MSABI hid_writeProto;
    using HideWindowProto_WIN = MSABI HideWindowProto;
    using iconv_stringProto_WIN = MSABI iconv_stringProto;
    using InitProto_WIN = MSABI InitProto;
    using InitSubSystemProto_WIN = MSABI InitSubSystemProto;
    using IntersectFRectProto_WIN = MSABI IntersectFRectProto;
    using IntersectFRectAndLineProto_WIN = MSABI IntersectFRectAndLineProto;
    using IntersectRectProto_WIN = MSABI IntersectRectProto;
    using IntersectRectAndLineProto_WIN = MSABI IntersectRectAndLineProto;
    using IsGameControllerProto_WIN = MSABI IsGameControllerProto;
    using IsScreenKeyboardShownProto_WIN = MSABI IsScreenKeyboardShownProto;
    using IsScreenSaverEnabledProto_WIN = MSABI IsScreenSaverEnabledProto;
    using IsShapedWindowProto_WIN = MSABI IsShapedWindowProto;
    using IsTabletProto_WIN = MSABI IsTabletProto;
    using IsTextInputActiveProto_WIN = MSABI IsTextInputActiveProto;
    using IsTextInputShownProto_WIN = MSABI IsTextInputShownProto;
    using JoystickAttachVirtualProto_WIN = MSABI JoystickAttachVirtualProto;
    using JoystickAttachVirtualExProto_WIN = MSABI JoystickAttachVirtualExProto;
    using JoystickCloseProto_WIN = MSABI JoystickCloseProto;
    using JoystickCurrentPowerLevelProto_WIN = MSABI JoystickCurrentPowerLevelProto;
    using JoystickDetachVirtualProto_WIN = MSABI JoystickDetachVirtualProto;
    using JoystickEventStateProto_WIN = MSABI JoystickEventStateProto;
    using JoystickFromInstanceIDProto_WIN = MSABI JoystickFromInstanceIDProto;
    using JoystickFromPlayerIndexProto_WIN = MSABI JoystickFromPlayerIndexProto;
    using JoystickGetAttachedProto_WIN = MSABI JoystickGetAttachedProto;
    using JoystickGetAxisProto_WIN = MSABI JoystickGetAxisProto;
    using JoystickGetAxisInitialStateProto_WIN = MSABI JoystickGetAxisInitialStateProto;
    using JoystickGetBallProto_WIN = MSABI JoystickGetBallProto;
    using JoystickGetButtonProto_WIN = MSABI JoystickGetButtonProto;
    using JoystickGetDeviceGUIDProto_WIN = MSABI JoystickGetDeviceGUIDProto;
    using JoystickGetDeviceInstanceIDProto_WIN = MSABI JoystickGetDeviceInstanceIDProto;
    using JoystickGetDevicePlayerIndexProto_WIN = MSABI JoystickGetDevicePlayerIndexProto;
    using JoystickGetDeviceProductProto_WIN = MSABI JoystickGetDeviceProductProto;
    using JoystickGetDeviceProductVersionProto_WIN = MSABI JoystickGetDeviceProductVersionProto;
    using JoystickGetDeviceTypeProto_WIN = MSABI JoystickGetDeviceTypeProto;
    using JoystickGetDeviceVendorProto_WIN = MSABI JoystickGetDeviceVendorProto;
    using JoystickGetFirmwareVersionProto_WIN = MSABI JoystickGetFirmwareVersionProto;
    using JoystickGetGUIDProto_WIN = MSABI JoystickGetGUIDProto;
    using JoystickGetGUIDFromStringProto_WIN = MSABI JoystickGetGUIDFromStringProto;
    using JoystickGetGUIDStringProto_WIN = MSABI JoystickGetGUIDStringProto;
    using JoystickGetHatProto_WIN = MSABI JoystickGetHatProto;
    using JoystickGetPlayerIndexProto_WIN = MSABI JoystickGetPlayerIndexProto;
    using JoystickGetProductProto_WIN = MSABI JoystickGetProductProto;
    using JoystickGetProductVersionProto_WIN = MSABI JoystickGetProductVersionProto;
    using JoystickGetSerialProto_WIN = MSABI JoystickGetSerialProto;
    using JoystickGetTypeProto_WIN = MSABI JoystickGetTypeProto;
    using JoystickGetVendorProto_WIN = MSABI JoystickGetVendorProto;
    using JoystickHasLEDProto_WIN = MSABI JoystickHasLEDProto;
    using JoystickHasRumbleProto_WIN = MSABI JoystickHasRumbleProto;
    using JoystickHasRumbleTriggersProto_WIN = MSABI JoystickHasRumbleTriggersProto;
    using JoystickInstanceIDProto_WIN = MSABI JoystickInstanceIDProto;
    using JoystickIsHapticProto_WIN = MSABI JoystickIsHapticProto;
    using JoystickIsVirtualProto_WIN = MSABI JoystickIsVirtualProto;
    using JoystickNameProto_WIN = MSABI JoystickNameProto;
    using JoystickNameForIndexProto_WIN = MSABI JoystickNameForIndexProto;
    using JoystickNumAxesProto_WIN = MSABI JoystickNumAxesProto;
    using JoystickNumBallsProto_WIN = MSABI JoystickNumBallsProto;
    using JoystickNumButtonsProto_WIN = MSABI JoystickNumButtonsProto;
    using JoystickNumHatsProto_WIN = MSABI JoystickNumHatsProto;
    using JoystickOpenProto_WIN = MSABI JoystickOpenProto;
    using JoystickPathProto_WIN = MSABI JoystickPathProto;
    using JoystickPathForIndexProto_WIN = MSABI JoystickPathForIndexProto;
    using JoystickRumbleProto_WIN = MSABI JoystickRumbleProto;
    using JoystickRumbleTriggersProto_WIN = MSABI JoystickRumbleTriggersProto;
    using JoystickSendEffectProto_WIN = MSABI JoystickSendEffectProto;
    using JoystickSetLEDProto_WIN = MSABI JoystickSetLEDProto;
    using JoystickSetPlayerIndexProto_WIN = MSABI JoystickSetPlayerIndexProto;
    using JoystickSetVirtualAxisProto_WIN = MSABI JoystickSetVirtualAxisProto;
    using JoystickSetVirtualButtonProto_WIN = MSABI JoystickSetVirtualButtonProto;
    using JoystickSetVirtualHatProto_WIN = MSABI JoystickSetVirtualHatProto;
    using JoystickUpdateProto_WIN = MSABI JoystickUpdateProto;
    using LoadBMP_RWProto_WIN = MSABI LoadBMP_RWProto;
    using LoadDollarTemplatesProto_WIN = MSABI LoadDollarTemplatesProto;
    using LoadFileProto_WIN = MSABI LoadFileProto;
    using LoadFile_RWProto_WIN = MSABI LoadFile_RWProto;
    using LoadFunctionProto_WIN = MSABI LoadFunctionProto;
    using LoadObjectProto_WIN = MSABI LoadObjectProto;
    using LoadWAV_RWProto_WIN = MSABI LoadWAV_RWProto;
    using LockAudioProto_WIN = MSABI LockAudioProto;
    using LockAudioDeviceProto_WIN = MSABI LockAudioDeviceProto;
    using LockJoysticksProto_WIN = MSABI LockJoysticksProto;
    using LockMutexProto_WIN = MSABI LockMutexProto;
    using LockSensorsProto_WIN = MSABI LockSensorsProto;
    using LockSurfaceProto_WIN = MSABI LockSurfaceProto;
    using LockTextureProto_WIN = MSABI LockTextureProto;
    using LockTextureToSurfaceProto_WIN = MSABI LockTextureToSurfaceProto;
    using LogProto_WIN = MSABI LogProto;
    using LogCriticalProto_WIN = MSABI LogCriticalProto;
    using LogDebugProto_WIN = MSABI LogDebugProto;
    using LogGetOutputFunctionProto_WIN = MSABI LogGetOutputFunctionProto;
    using LogGetPriorityProto_WIN = MSABI LogGetPriorityProto;
    using LogInfoProto_WIN = MSABI LogInfoProto;
    using LogMessageProto_WIN = MSABI LogMessageProto;
    using LogResetPrioritiesProto_WIN = MSABI LogResetPrioritiesProto;
    using LogSetAllPriorityProto_WIN = MSABI LogSetAllPriorityProto;
    using LogSetOutputFunctionProto_WIN = MSABI LogSetOutputFunctionProto;
    using LogSetPriorityProto_WIN = MSABI LogSetPriorityProto;
    using LogVerboseProto_WIN = MSABI LogVerboseProto;
    using LogWarnProto_WIN = MSABI LogWarnProto;
    using LowerBlitProto_WIN = MSABI LowerBlitProto;
    using LowerBlitScaledProto_WIN = MSABI LowerBlitScaledProto;
    using MapRGBProto_WIN = MSABI MapRGBProto;
    using MapRGBAProto_WIN = MSABI MapRGBAProto;
    using MasksToPixelFormatEnumProto_WIN = MSABI MasksToPixelFormatEnumProto;
    using MaximizeWindowProto_WIN = MSABI MaximizeWindowProto;
    using MemoryBarrierReleaseFunctionProto_WIN = MSABI MemoryBarrierReleaseFunctionProto;
    using Metal_CreateViewProto_WIN = MSABI Metal_CreateViewProto;
    using Metal_DestroyViewProto_WIN = MSABI Metal_DestroyViewProto;
    using Metal_GetDrawableSizeProto_WIN = MSABI Metal_GetDrawableSizeProto;
    using Metal_GetLayerProto_WIN = MSABI Metal_GetLayerProto;
    using MinimizeWindowProto_WIN = MSABI MinimizeWindowProto;
    using MixAudioProto_WIN = MSABI MixAudioProto;
    using MixAudioFormatProto_WIN = MSABI MixAudioFormatProto;
    using MouseIsHapticProto_WIN = MSABI MouseIsHapticProto;
    using NewAudioStreamProto_WIN = MSABI NewAudioStreamProto;
    using NumHapticsProto_WIN = MSABI NumHapticsProto;
    using NumJoysticksProto_WIN = MSABI NumJoysticksProto;
    using NumSensorsProto_WIN = MSABI NumSensorsProto;
    using OpenAudioProto_WIN = MSABI OpenAudioProto;
    using OpenAudioDeviceProto_WIN = MSABI OpenAudioDeviceProto;
    using OpenURLProto_WIN = MSABI OpenURLProto;
    using PauseAudioProto_WIN = MSABI PauseAudioProto;
    using PauseAudioDeviceProto_WIN = MSABI PauseAudioDeviceProto;
    using PeepEventsProto_WIN = MSABI PeepEventsProto;
    using PixelFormatEnumToMasksProto_WIN = MSABI PixelFormatEnumToMasksProto;
    using PollEventProto_WIN = MSABI PollEventProto;
    using PremultiplyAlphaProto_WIN = MSABI PremultiplyAlphaProto;
    using PumpEventsProto_WIN = MSABI PumpEventsProto;
    using PushEventProto_WIN = MSABI PushEventProto;
    using QueryTextureProto_WIN = MSABI QueryTextureProto;
    using QueueAudioProto_WIN = MSABI QueueAudioProto;
    using QuitProto_WIN = MSABI QuitProto;
    using QuitSubSystemProto_WIN = MSABI QuitSubSystemProto;
    using RaiseWindowProto_WIN = MSABI RaiseWindowProto;
    using ReadBE16Proto_WIN = MSABI ReadBE16Proto;
    using ReadBE32Proto_WIN = MSABI ReadBE32Proto;
    using ReadBE64Proto_WIN = MSABI ReadBE64Proto;
    using ReadLE16Proto_WIN = MSABI ReadLE16Proto;
    using ReadLE32Proto_WIN = MSABI ReadLE32Proto;
    using ReadLE64Proto_WIN = MSABI ReadLE64Proto;
    using ReadU8Proto_WIN = MSABI ReadU8Proto;
    using RecordGestureProto_WIN = MSABI RecordGestureProto;
    using RegisterEventsProto_WIN = MSABI RegisterEventsProto;
    using RemoveTimerProto_WIN = MSABI RemoveTimerProto;
    using RenderClearProto_WIN = MSABI RenderClearProto;
    using RenderCopyProto_WIN = MSABI RenderCopyProto;
    using RenderCopyExProto_WIN = MSABI RenderCopyExProto;
    using RenderCopyExFProto_WIN = MSABI RenderCopyExFProto;
    using RenderCopyFProto_WIN = MSABI RenderCopyFProto;
    using RenderDrawLineProto_WIN = MSABI RenderDrawLineProto;
    using RenderDrawLineFProto_WIN = MSABI RenderDrawLineFProto;
    using RenderDrawLinesProto_WIN = MSABI RenderDrawLinesProto;
    using RenderDrawLinesFProto_WIN = MSABI RenderDrawLinesFProto;
    using RenderDrawPointProto_WIN = MSABI RenderDrawPointProto;
    using RenderDrawPointFProto_WIN = MSABI RenderDrawPointFProto;
    using RenderDrawPointsProto_WIN = MSABI RenderDrawPointsProto;
    using RenderDrawPointsFProto_WIN = MSABI RenderDrawPointsFProto;
    using RenderDrawRectProto_WIN = MSABI RenderDrawRectProto;
    using RenderDrawRectFProto_WIN = MSABI RenderDrawRectFProto;
    using RenderDrawRectsProto_WIN = MSABI RenderDrawRectsProto;
    using RenderDrawRectsFProto_WIN = MSABI RenderDrawRectsFProto;
    using RenderFillRectProto_WIN = MSABI RenderFillRectProto;
    using RenderFillRectFProto_WIN = MSABI RenderFillRectFProto;
    using RenderFillRectsProto_WIN = MSABI RenderFillRectsProto;
    using RenderFillRectsFProto_WIN = MSABI RenderFillRectsFProto;
    using RenderFlushProto_WIN = MSABI RenderFlushProto;
    using RenderGeometryProto_WIN = MSABI RenderGeometryProto;
    using RenderGeometryRawProto_WIN = MSABI RenderGeometryRawProto;
    using RenderGetClipRectProto_WIN = MSABI RenderGetClipRectProto;
    using RenderGetIntegerScaleProto_WIN = MSABI RenderGetIntegerScaleProto;
    using RenderGetLogicalSizeProto_WIN = MSABI RenderGetLogicalSizeProto;
    using RenderGetMetalCommandEncoderProto_WIN = MSABI RenderGetMetalCommandEncoderProto;
    using RenderGetMetalLayerProto_WIN = MSABI RenderGetMetalLayerProto;
    using RenderGetScaleProto_WIN = MSABI RenderGetScaleProto;
    using RenderGetViewportProto_WIN = MSABI RenderGetViewportProto;
    using RenderGetWindowProto_WIN = MSABI RenderGetWindowProto;
    using RenderIsClipEnabledProto_WIN = MSABI RenderIsClipEnabledProto;
    using RenderLogicalToWindowProto_WIN = MSABI RenderLogicalToWindowProto;
    using RenderPresentProto_WIN = MSABI RenderPresentProto;
    using RenderReadPixelsProto_WIN = MSABI RenderReadPixelsProto;
    using RenderSetClipRectProto_WIN = MSABI RenderSetClipRectProto;
    using RenderSetIntegerScaleProto_WIN = MSABI RenderSetIntegerScaleProto;
    using RenderSetLogicalSizeProto_WIN = MSABI RenderSetLogicalSizeProto;
    using RenderSetScaleProto_WIN = MSABI RenderSetScaleProto;
    using RenderSetViewportProto_WIN = MSABI RenderSetViewportProto;
    using RenderSetVSyncProto_WIN = MSABI RenderSetVSyncProto;
    using RenderTargetSupportedProto_WIN = MSABI RenderTargetSupportedProto;
    using RenderWindowToLogicalProto_WIN = MSABI RenderWindowToLogicalProto;
    using ResetAssertionReportProto_WIN = MSABI ResetAssertionReportProto;
    using ResetHintProto_WIN = MSABI ResetHintProto;
    using ResetHintsProto_WIN = MSABI ResetHintsProto;
    using ResetKeyboardProto_WIN = MSABI ResetKeyboardProto;
    using RestoreWindowProto_WIN = MSABI RestoreWindowProto;
    using RWcloseProto_WIN = MSABI RWcloseProto;
    using RWFromConstMemProto_WIN = MSABI RWFromConstMemProto;
    using RWFromFileProto_WIN = MSABI RWFromFileProto;
    using RWFromFPProto_WIN = MSABI RWFromFPProto;
    using RWFromMemProto_WIN = MSABI RWFromMemProto;
    using RWreadProto_WIN = MSABI RWreadProto;
    using RWseekProto_WIN = MSABI RWseekProto;
    using RWsizeProto_WIN = MSABI RWsizeProto;
    using RWtellProto_WIN = MSABI RWtellProto;
    using RWwriteProto_WIN = MSABI RWwriteProto;
    using SaveAllDollarTemplatesProto_WIN = MSABI SaveAllDollarTemplatesProto;
    using SaveBMP_RWProto_WIN = MSABI SaveBMP_RWProto;
    using SaveDollarTemplateProto_WIN = MSABI SaveDollarTemplateProto;
    using SemPostProto_WIN = MSABI SemPostProto;
    using SemTryWaitProto_WIN = MSABI SemTryWaitProto;
    using SemValueProto_WIN = MSABI SemValueProto;
    using SemWaitProto_WIN = MSABI SemWaitProto;
    using SemWaitTimeoutProto_WIN = MSABI SemWaitTimeoutProto;
    using SensorCloseProto_WIN = MSABI SensorCloseProto;
    using SensorFromInstanceIDProto_WIN = MSABI SensorFromInstanceIDProto;
    using SensorGetDataProto_WIN = MSABI SensorGetDataProto;
    using SensorGetDataWithTimestampProto_WIN = MSABI SensorGetDataWithTimestampProto;
    using SensorGetDeviceInstanceIDProto_WIN = MSABI SensorGetDeviceInstanceIDProto;
    using SensorGetDeviceNameProto_WIN = MSABI SensorGetDeviceNameProto;
    using SensorGetDeviceNonPortableTypeProto_WIN = MSABI SensorGetDeviceNonPortableTypeProto;
    using SensorGetDeviceTypeProto_WIN = MSABI SensorGetDeviceTypeProto;
    using SensorGetInstanceIDProto_WIN = MSABI SensorGetInstanceIDProto;
    using SensorGetNameProto_WIN = MSABI SensorGetNameProto;
    using SensorGetNonPortableTypeProto_WIN = MSABI SensorGetNonPortableTypeProto;
    using SensorGetTypeProto_WIN = MSABI SensorGetTypeProto;
    using SensorOpenProto_WIN = MSABI SensorOpenProto;
    using SensorUpdateProto_WIN = MSABI SensorUpdateProto;
    using SetAssertionHandlerProto_WIN = MSABI SetAssertionHandlerProto;
    using SetClipboardTextProto_WIN = MSABI SetClipboardTextProto;
    using SetClipRectProto_WIN = MSABI SetClipRectProto;
    using SetColorKeyProto_WIN = MSABI SetColorKeyProto;
    using SetCursorProto_WIN = MSABI SetCursorProto;
    using SetErrorProto_WIN = MSABI SetErrorProto;
    using SetEventFilterProto_WIN = MSABI SetEventFilterProto;
    using SetHintProto_WIN = MSABI SetHintProto;
    using SetHintWithPriorityProto_WIN = MSABI SetHintWithPriorityProto;
    using SetMainReadyProto_WIN = MSABI SetMainReadyProto;
    using SetMemoryFunctionsProto_WIN = MSABI SetMemoryFunctionsProto;
    using SetModStateProto_WIN = MSABI SetModStateProto;
    using SetPaletteColorsProto_WIN = MSABI SetPaletteColorsProto;
    using SetPixelFormatPaletteProto_WIN = MSABI SetPixelFormatPaletteProto;
    using SetPrimarySelectionTextProto_WIN = MSABI SetPrimarySelectionTextProto;
    using SetRelativeMouseModeProto_WIN = MSABI SetRelativeMouseModeProto;
    using SetRenderDrawBlendModeProto_WIN = MSABI SetRenderDrawBlendModeProto;
    using SetRenderDrawColorProto_WIN = MSABI SetRenderDrawColorProto;
    using SetRenderTargetProto_WIN = MSABI SetRenderTargetProto;
    using SetSurfaceAlphaModProto_WIN = MSABI SetSurfaceAlphaModProto;
    using SetSurfaceBlendModeProto_WIN = MSABI SetSurfaceBlendModeProto;
    using SetSurfaceColorModProto_WIN = MSABI SetSurfaceColorModProto;
    using SetSurfacePaletteProto_WIN = MSABI SetSurfacePaletteProto;
    using SetSurfaceRLEProto_WIN = MSABI SetSurfaceRLEProto;
    using SetTextInputRectProto_WIN = MSABI SetTextInputRectProto;
    using SetTextureAlphaModProto_WIN = MSABI SetTextureAlphaModProto;
    using SetTextureBlendModeProto_WIN = MSABI SetTextureBlendModeProto;
    using SetTextureColorModProto_WIN = MSABI SetTextureColorModProto;
    using SetTextureScaleModeProto_WIN = MSABI SetTextureScaleModeProto;
    using SetTextureUserDataProto_WIN = MSABI SetTextureUserDataProto;
    using SetThreadPriorityProto_WIN = MSABI SetThreadPriorityProto;
    using SetWindowAlwaysOnTopProto_WIN = MSABI SetWindowAlwaysOnTopProto;
    using SetWindowBorderedProto_WIN = MSABI SetWindowBorderedProto;
    using SetWindowBrightnessProto_WIN = MSABI SetWindowBrightnessProto;
    using SetWindowDataProto_WIN = MSABI SetWindowDataProto;
    using SetWindowDisplayModeProto_WIN = MSABI SetWindowDisplayModeProto;
    using SetWindowFullscreenProto_WIN = MSABI SetWindowFullscreenProto;
    using SetWindowGammaRampProto_WIN = MSABI SetWindowGammaRampProto;
    using SetWindowGrabProto_WIN = MSABI SetWindowGrabProto;
    using SetWindowHitTestProto_WIN = MSABI SetWindowHitTestProto;
    using SetWindowIconProto_WIN = MSABI SetWindowIconProto;
    using SetWindowInputFocusProto_WIN = MSABI SetWindowInputFocusProto;
    using SetWindowKeyboardGrabProto_WIN = MSABI SetWindowKeyboardGrabProto;
    using SetWindowMaximumSizeProto_WIN = MSABI SetWindowMaximumSizeProto;
    using SetWindowMinimumSizeProto_WIN = MSABI SetWindowMinimumSizeProto;
    using SetWindowModalForProto_WIN = MSABI SetWindowModalForProto;
    using SetWindowMouseGrabProto_WIN = MSABI SetWindowMouseGrabProto;
    using SetWindowMouseRectProto_WIN = MSABI SetWindowMouseRectProto;
    using SetWindowOpacityProto_WIN = MSABI SetWindowOpacityProto;
    using SetWindowPositionProto_WIN = MSABI SetWindowPositionProto;
    using SetWindowResizableProto_WIN = MSABI SetWindowResizableProto;
    using SetWindowShapeProto_WIN = MSABI SetWindowShapeProto;
    using SetWindowSizeProto_WIN = MSABI SetWindowSizeProto;
    using SetWindowTitleProto_WIN = MSABI SetWindowTitleProto;
    using SetYUVConversionModeProto_WIN = MSABI SetYUVConversionModeProto;
    using ShowCursorProto_WIN = MSABI ShowCursorProto;
    using ShowMessageBoxProto_WIN = MSABI ShowMessageBoxProto;
    using ShowSimpleMessageBoxProto_WIN = MSABI ShowSimpleMessageBoxProto;
    using ShowWindowProto_WIN = MSABI ShowWindowProto;
    using SIMDAllocProto_WIN = MSABI SIMDAllocProto;
    using SIMDFreeProto_WIN = MSABI SIMDFreeProto;
    using SIMDGetAlignmentProto_WIN = MSABI SIMDGetAlignmentProto;
    using SIMDReallocProto_WIN = MSABI SIMDReallocProto;
    using SoftStretchProto_WIN = MSABI SoftStretchProto;
    using SoftStretchLinearProto_WIN = MSABI SoftStretchLinearProto;
    using StartTextInputProto_WIN = MSABI StartTextInputProto;
    using StopTextInputProto_WIN = MSABI StopTextInputProto;
    using ThreadIDProto_WIN = MSABI ThreadIDProto;
    using TLSCleanupProto_WIN = MSABI TLSCleanupProto;
    using TLSCreateProto_WIN = MSABI TLSCreateProto;
    using TLSGetProto_WIN = MSABI TLSGetProto;
    using TLSSetProto_WIN = MSABI TLSSetProto;
    using TryLockMutexProto_WIN = MSABI TryLockMutexProto;
    using UnionFRectProto_WIN = MSABI UnionFRectProto;
    using UnionRectProto_WIN = MSABI UnionRectProto;
    using UnloadObjectProto_WIN = MSABI UnloadObjectProto;
    using UnlockAudioProto_WIN = MSABI UnlockAudioProto;
    using UnlockAudioDeviceProto_WIN = MSABI UnlockAudioDeviceProto;
    using UnlockJoysticksProto_WIN = MSABI UnlockJoysticksProto;
    using UnlockMutexProto_WIN = MSABI UnlockMutexProto;
    using UnlockSurfaceProto_WIN = MSABI UnlockSurfaceProto;
    using UnlockTextureProto_WIN = MSABI UnlockTextureProto;
    using UpdateNVTextureProto_WIN = MSABI UpdateNVTextureProto;
    using UpdateTextureProto_WIN = MSABI UpdateTextureProto;
    using UpdateWindowSurfaceProto_WIN = MSABI UpdateWindowSurfaceProto;
    using UpdateWindowSurfaceRectsProto_WIN = MSABI UpdateWindowSurfaceRectsProto;
    using UpdateYUVTextureProto_WIN = MSABI UpdateYUVTextureProto;
    using UpperBlitProto_WIN = MSABI UpperBlitProto;
    using UpperBlitScaledProto_WIN = MSABI UpperBlitScaledProto;
    using VideoInitProto_WIN = MSABI VideoInitProto;
    using VideoQuitProto_WIN = MSABI VideoQuitProto;
#ifdef COSMO_SDL2_VULKAN
    using Vulkan_CreateSurfaceProto_WIN = MSABI Vulkan_CreateSurfaceProto;
    using Vulkan_GetDrawableSizeProto_WIN = MSABI Vulkan_GetDrawableSizeProto;
    using Vulkan_GetInstanceExtensionsProto_WIN = MSABI Vulkan_GetInstanceExtensionsProto;
    using Vulkan_GetVkGetInstanceProcAddrProto_WIN = MSABI Vulkan_GetVkGetInstanceProcAddrProto;
    using Vulkan_GetVkInstanceProcAddrProto_WIN = MSABI Vulkan_GetVkInstanceProcAddrProto;
    using Vulkan_LoadLibraryProto_WIN = MSABI Vulkan_LoadLibraryProto;
    using Vulkan_UnloadLibraryProto_WIN = MSABI Vulkan_UnloadLibraryProto;
#endif
    using WaitEventProto_WIN = MSABI WaitEventProto;
    using WaitEventTimeoutProto_WIN = MSABI WaitEventTimeoutProto;
    using WaitThreadProto_WIN = MSABI WaitThreadProto;
    using WarpMouseGlobalProto_WIN = MSABI WarpMouseGlobalProto;
    using WarpMouseInWindowProto_WIN = MSABI WarpMouseInWindowProto;
    using WasInitProto_WIN = MSABI WasInitProto;
    using WriteBE16Proto_WIN = MSABI WriteBE16Proto;
    using WriteBE32Proto_WIN = MSABI WriteBE32Proto;
    using WriteBE64Proto_WIN = MSABI WriteBE64Proto;
    using WriteLE16Proto_WIN = MSABI WriteLE16Proto;
    using WriteLE32Proto_WIN = MSABI WriteLE32Proto;
    using WriteLE64Proto_WIN = MSABI WriteLE64Proto;
    using WriteU8Proto_WIN = MSABI WriteU8Proto;
    using NetAddSocketProto_WIN = MSABI NetAddSocketProto;
    using NetAllocPacketProto_WIN = MSABI NetAllocPacketProto;
    using NetAllocPacketVProto_WIN = MSABI NetAllocPacketVProto;
    using NetAllocSocketSetProto_WIN = MSABI NetAllocSocketSetProto;
    using NetCheckSocketsProto_WIN = MSABI NetCheckSocketsProto;
    using NetDelSocketProto_WIN = MSABI NetDelSocketProto;
    using NetFreePacketProto_WIN = MSABI NetFreePacketProto;
    using NetFreePacketVProto_WIN = MSABI NetFreePacketVProto;
    using NetFreeSocketSetProto_WIN = MSABI NetFreeSocketSetProto;
    using NetGetErrorProto_WIN = MSABI NetGetErrorProto;
    using NetGetLocalAddressesProto_WIN = MSABI NetGetLocalAddressesProto;
    using NetInitProto_WIN = MSABI NetInitProto;
    using NetLinked_VersionProto_WIN = MSABI NetLinked_VersionProto;
    using NetQuitProto_WIN = MSABI NetQuitProto;
    using NetResizePacketProto_WIN = MSABI NetResizePacketProto;
    using NetResolveHostProto_WIN = MSABI NetResolveHostProto;
    using NetResolveIPProto_WIN = MSABI NetResolveIPProto;
    using NetTCP_AcceptProto_WIN = MSABI NetTCP_AcceptProto;
    using NetTCP_CloseProto_WIN = MSABI NetTCP_CloseProto;
    using NetTCP_GetPeerAddressProto_WIN = MSABI NetTCP_GetPeerAddressProto;
    using NetTCP_OpenProto_WIN = MSABI NetTCP_OpenProto;
    using NetTCP_RecvProto_WIN = MSABI NetTCP_RecvProto;
    using NetTCP_SendProto_WIN = MSABI NetTCP_SendProto;
    using NetUDP_BindProto_WIN = MSABI NetUDP_BindProto;
    using NetUDP_CloseProto_WIN = MSABI NetUDP_CloseProto;
    using NetUDP_GetPeerAddressProto_WIN = MSABI NetUDP_GetPeerAddressProto;
    using NetUDP_OpenProto_WIN = MSABI NetUDP_OpenProto;
    using NetUDP_RecvProto_WIN = MSABI NetUDP_RecvProto;
    using NetUDP_RecvVProto_WIN = MSABI NetUDP_RecvVProto;
    using NetUDP_SendProto_WIN = MSABI NetUDP_SendProto;
    using NetUDP_SendVProto_WIN = MSABI NetUDP_SendVProto;
    using NetUDP_SetPacketLossProto_WIN = MSABI NetUDP_SetPacketLossProto;
    using NetUDP_UnbindProto_WIN = MSABI NetUDP_UnbindProto;
    using MixAllocateChannelsProto_WIN = MSABI MixAllocateChannelsProto;
    using MixChannelFinishedProto_WIN = MSABI MixChannelFinishedProto;
    using MixCloseAudioProto_WIN = MSABI MixCloseAudioProto;
    using MixEachSoundFontProto_WIN = MSABI MixEachSoundFontProto;
    using MixExpireChannelProto_WIN = MSABI MixExpireChannelProto;
    using MixFadeInChannelProto_WIN = MSABI MixFadeInChannelProto;
    using MixFadeInChannelTimedProto_WIN = MSABI MixFadeInChannelTimedProto;
    using MixFadeInMusicProto_WIN = MSABI MixFadeInMusicProto;
    using MixFadeInMusicPosProto_WIN = MSABI MixFadeInMusicPosProto;
    using MixFadeOutChannelProto_WIN = MSABI MixFadeOutChannelProto;
    using MixFadeOutGroupProto_WIN = MSABI MixFadeOutGroupProto;
    using MixFadeOutMusicProto_WIN = MSABI MixFadeOutMusicProto;
    using MixFadingChannelProto_WIN = MSABI MixFadingChannelProto;
    using MixFadingMusicProto_WIN = MSABI MixFadingMusicProto;
    using MixFreeChunkProto_WIN = MSABI MixFreeChunkProto;
    using MixFreeMusicProto_WIN = MSABI MixFreeMusicProto;
    using MixGetChunkProto_WIN = MSABI MixGetChunkProto;
    using MixGetChunkDecoderProto_WIN = MSABI MixGetChunkDecoderProto;
    using MixGetMusicAlbumTagProto_WIN = MSABI MixGetMusicAlbumTagProto;
    using MixGetMusicArtistTagProto_WIN = MSABI MixGetMusicArtistTagProto;
    using MixGetMusicCopyrightTagProto_WIN = MSABI MixGetMusicCopyrightTagProto;
    using MixGetMusicDecoderProto_WIN = MSABI MixGetMusicDecoderProto;
    using MixGetMusicHookDataProto_WIN = MSABI MixGetMusicHookDataProto;
    using MixGetMusicLoopEndTimeProto_WIN = MSABI MixGetMusicLoopEndTimeProto;
    using MixGetMusicLoopLengthTimeProto_WIN = MSABI MixGetMusicLoopLengthTimeProto;
    using MixGetMusicLoopStartTimeProto_WIN = MSABI MixGetMusicLoopStartTimeProto;
    using MixGetMusicPositionProto_WIN = MSABI MixGetMusicPositionProto;
    using MixGetMusicTitleProto_WIN = MSABI MixGetMusicTitleProto;
    using MixGetMusicTitleTagProto_WIN = MSABI MixGetMusicTitleTagProto;
    using MixGetMusicTypeProto_WIN = MSABI MixGetMusicTypeProto;
    using MixGetMusicVolumeProto_WIN = MSABI MixGetMusicVolumeProto;
    using MixGetNumChunkDecodersProto_WIN = MSABI MixGetNumChunkDecodersProto;
    using MixGetNumMusicDecodersProto_WIN = MSABI MixGetNumMusicDecodersProto;
    using MixGetSoundFontsProto_WIN = MSABI MixGetSoundFontsProto;
    using MixGetSynchroValueProto_WIN = MSABI MixGetSynchroValueProto;
    using MixGetTimidityCfgProto_WIN = MSABI MixGetTimidityCfgProto;
    using MixGroupAvailableProto_WIN = MSABI MixGroupAvailableProto;
    using MixGroupChannelProto_WIN = MSABI MixGroupChannelProto;
    using MixGroupChannelsProto_WIN = MSABI MixGroupChannelsProto;
    using MixGroupCountProto_WIN = MSABI MixGroupCountProto;
    using MixGroupNewerProto_WIN = MSABI MixGroupNewerProto;
    using MixGroupOldestProto_WIN = MSABI MixGroupOldestProto;
    using MixHaltChannelProto_WIN = MSABI MixHaltChannelProto;
    using MixHaltGroupProto_WIN = MSABI MixHaltGroupProto;
    using MixHaltMusicProto_WIN = MSABI MixHaltMusicProto;
    using MixHasChunkDecoderProto_WIN = MSABI MixHasChunkDecoderProto;
    using MixHasMusicDecoderProto_WIN = MSABI MixHasMusicDecoderProto;
    using MixHookMusicProto_WIN = MSABI MixHookMusicProto;
    using MixHookMusicFinishedProto_WIN = MSABI MixHookMusicFinishedProto;
    using MixInitProto_WIN = MSABI MixInitProto;
    using MixLinked_VersionProto_WIN = MSABI MixLinked_VersionProto;
    using MixLoadMUSProto_WIN = MSABI MixLoadMUSProto;
    using MixLoadMUS_RWProto_WIN = MSABI MixLoadMUS_RWProto;
    using MixLoadMUSType_RWProto_WIN = MSABI MixLoadMUSType_RWProto;
    using MixLoadWAVProto_WIN = MSABI MixLoadWAVProto;
    using MixLoadWAV_RWProto_WIN = MSABI MixLoadWAV_RWProto;
    using MixMasterVolumeProto_WIN = MSABI MixMasterVolumeProto;
    using MixModMusicJumpToOrderProto_WIN = MSABI MixModMusicJumpToOrderProto;
    using MixMusicDurationProto_WIN = MSABI MixMusicDurationProto;
    using MixOpenAudioProto_WIN = MSABI MixOpenAudioProto;
    using MixOpenAudioDeviceProto_WIN = MSABI MixOpenAudioDeviceProto;
    using MixPauseProto_WIN = MSABI MixPauseProto;
    using MixPausedProto_WIN = MSABI MixPausedProto;
    using MixPausedMusicProto_WIN = MSABI MixPausedMusicProto;
    using MixPauseMusicProto_WIN = MSABI MixPauseMusicProto;
    using MixPlayChannelProto_WIN = MSABI MixPlayChannelProto;
    using MixPlayChannelTimedProto_WIN = MSABI MixPlayChannelTimedProto;
    using MixPlayingProto_WIN = MSABI MixPlayingProto;
    using MixPlayingMusicProto_WIN = MSABI MixPlayingMusicProto;
    using MixPlayMusicProto_WIN = MSABI MixPlayMusicProto;
    using MixQuerySpecProto_WIN = MSABI MixQuerySpecProto;
    using MixQuickLoad_RAWProto_WIN = MSABI MixQuickLoad_RAWProto;
    using MixQuickLoad_WAVProto_WIN = MSABI MixQuickLoad_WAVProto;
    using MixQuitProto_WIN = MSABI MixQuitProto;
    using MixRegisterEffectProto_WIN = MSABI MixRegisterEffectProto;
    using MixReserveChannelsProto_WIN = MSABI MixReserveChannelsProto;
    using MixResumeProto_WIN = MSABI MixResumeProto;
    using MixResumeMusicProto_WIN = MSABI MixResumeMusicProto;
    using MixRewindMusicProto_WIN = MSABI MixRewindMusicProto;
    using MixSetDistanceProto_WIN = MSABI MixSetDistanceProto;
    using MixSetMusicCMDProto_WIN = MSABI MixSetMusicCMDProto;
    using MixSetMusicPositionProto_WIN = MSABI MixSetMusicPositionProto;
    using MixSetPanningProto_WIN = MSABI MixSetPanningProto;
    using MixSetPositionProto_WIN = MSABI MixSetPositionProto;
    using MixSetPostMixProto_WIN = MSABI MixSetPostMixProto;
    using MixSetReverseStereoProto_WIN = MSABI MixSetReverseStereoProto;
    using MixSetSoundFontsProto_WIN = MSABI MixSetSoundFontsProto;
    using MixSetSynchroValueProto_WIN = MSABI MixSetSynchroValueProto;
    using MixSetTimidityCfgProto_WIN = MSABI MixSetTimidityCfgProto;
    using MixUnregisterAllEffectsProto_WIN = MSABI MixUnregisterAllEffectsProto;
    using MixUnregisterEffectProto_WIN = MSABI MixUnregisterEffectProto;
    using MixVolumeProto_WIN = MSABI MixVolumeProto;
    using MixVolumeChunkProto_WIN = MSABI MixVolumeChunkProto;
    using MixVolumeMusicProto_WIN = MSABI MixVolumeMusicProto;
    using TTFByteSwappedUNICODEProto_WIN = MSABI TTFByteSwappedUNICODEProto;
    using TTFCloseFontProto_WIN = MSABI TTFCloseFontProto;
    using TTFFontAscentProto_WIN = MSABI TTFFontAscentProto;
    using TTFFontDescentProto_WIN = MSABI TTFFontDescentProto;
    using TTFFontFaceFamilyNameProto_WIN = MSABI TTFFontFaceFamilyNameProto;
    using TTFFontFaceIsFixedWidthProto_WIN = MSABI TTFFontFaceIsFixedWidthProto;
    using TTFFontFacesProto_WIN = MSABI TTFFontFacesProto;
    using TTFFontFaceStyleNameProto_WIN = MSABI TTFFontFaceStyleNameProto;
    using TTFFontHeightProto_WIN = MSABI TTFFontHeightProto;
    using TTFFontLineSkipProto_WIN = MSABI TTFFontLineSkipProto;
    using TTFGetFontHintingProto_WIN = MSABI TTFGetFontHintingProto;
    using TTFGetFontKerningProto_WIN = MSABI TTFGetFontKerningProto;
    using TTFGetFontOutlineProto_WIN = MSABI TTFGetFontOutlineProto;
    using TTFGetFontStyleProto_WIN = MSABI TTFGetFontStyleProto;
    using TTFGetFontWrappedAlignProto_WIN = MSABI TTFGetFontWrappedAlignProto;
    using TTFGetFreeTypeVersionProto_WIN = MSABI TTFGetFreeTypeVersionProto;
    using TTFGetHarfBuzzVersionProto_WIN = MSABI TTFGetHarfBuzzVersionProto;
    using TTFGlyphIsProvidedProto_WIN = MSABI TTFGlyphIsProvidedProto;
    using TTFGlyphIsProvided32Proto_WIN = MSABI TTFGlyphIsProvided32Proto;
    using TTFGlyphMetricsProto_WIN = MSABI TTFGlyphMetricsProto;
    using TTFGlyphMetrics32Proto_WIN = MSABI TTFGlyphMetrics32Proto;
    using TTFInitProto_WIN = MSABI TTFInitProto;
    using TTFLinked_VersionProto_WIN = MSABI TTFLinked_VersionProto;
    using TTFMeasureTextProto_WIN = MSABI TTFMeasureTextProto;
    using TTFMeasureUNICODEProto_WIN = MSABI TTFMeasureUNICODEProto;
    using TTFMeasureUTF8Proto_WIN = MSABI TTFMeasureUTF8Proto;
    using TTFOpenFontProto_WIN = MSABI TTFOpenFontProto;
    using TTFOpenFontDPIProto_WIN = MSABI TTFOpenFontDPIProto;
    using TTFOpenFontDPIRWProto_WIN = MSABI TTFOpenFontDPIRWProto;
    using TTFOpenFontIndexProto_WIN = MSABI TTFOpenFontIndexProto;
    using TTFOpenFontIndexDPIProto_WIN = MSABI TTFOpenFontIndexDPIProto;
    using TTFOpenFontIndexDPIRWProto_WIN = MSABI TTFOpenFontIndexDPIRWProto;
    using TTFOpenFontIndexRWProto_WIN = MSABI TTFOpenFontIndexRWProto;
    using TTFOpenFontRWProto_WIN = MSABI TTFOpenFontRWProto;
    using TTFQuitProto_WIN = MSABI TTFQuitProto;
    using TTFRenderGlyph32_BlendedProto_WIN = MSABI TTFRenderGlyph32_BlendedProto;
    using TTFRenderGlyph32_LCDProto_WIN = MSABI TTFRenderGlyph32_LCDProto;
    using TTFRenderGlyph32_ShadedProto_WIN = MSABI TTFRenderGlyph32_ShadedProto;
    using TTFRenderGlyph32_SolidProto_WIN = MSABI TTFRenderGlyph32_SolidProto;
    using TTFRenderGlyph_BlendedProto_WIN = MSABI TTFRenderGlyph_BlendedProto;
    using TTFRenderGlyph_LCDProto_WIN = MSABI TTFRenderGlyph_LCDProto;
    using TTFRenderGlyph_ShadedProto_WIN = MSABI TTFRenderGlyph_ShadedProto;
    using TTFRenderGlyph_SolidProto_WIN = MSABI TTFRenderGlyph_SolidProto;
    using TTFRenderText_BlendedProto_WIN = MSABI TTFRenderText_BlendedProto;
    using TTFRenderText_Blended_WrappedProto_WIN = MSABI TTFRenderText_Blended_WrappedProto;
    using TTFRenderText_LCDProto_WIN = MSABI TTFRenderText_LCDProto;
    using TTFRenderText_LCD_WrappedProto_WIN = MSABI TTFRenderText_LCD_WrappedProto;
    using TTFRenderText_ShadedProto_WIN = MSABI TTFRenderText_ShadedProto;
    using TTFRenderText_Shaded_WrappedProto_WIN = MSABI TTFRenderText_Shaded_WrappedProto;
    using TTFRenderText_SolidProto_WIN = MSABI TTFRenderText_SolidProto;
    using TTFRenderText_Solid_WrappedProto_WIN = MSABI TTFRenderText_Solid_WrappedProto;
    using TTFRenderUNICODE_BlendedProto_WIN = MSABI TTFRenderUNICODE_BlendedProto;
    using TTFRenderUNICODE_Blended_WrappedProto_WIN = MSABI TTFRenderUNICODE_Blended_WrappedProto;
    using TTFRenderUNICODE_LCDProto_WIN = MSABI TTFRenderUNICODE_LCDProto;
    using TTFRenderUNICODE_LCD_WrappedProto_WIN = MSABI TTFRenderUNICODE_LCD_WrappedProto;
    using TTFRenderUNICODE_ShadedProto_WIN = MSABI TTFRenderUNICODE_ShadedProto;
    using TTFRenderUNICODE_Shaded_WrappedProto_WIN = MSABI TTFRenderUNICODE_Shaded_WrappedProto;
    using TTFRenderUNICODE_SolidProto_WIN = MSABI TTFRenderUNICODE_SolidProto;
    using TTFRenderUNICODE_Solid_WrappedProto_WIN = MSABI TTFRenderUNICODE_Solid_WrappedProto;
    using TTFRenderUTF8_BlendedProto_WIN = MSABI TTFRenderUTF8_BlendedProto;
    using TTFRenderUTF8_Blended_WrappedProto_WIN = MSABI TTFRenderUTF8_Blended_WrappedProto;
    using TTFRenderUTF8_LCDProto_WIN = MSABI TTFRenderUTF8_LCDProto;
    using TTFRenderUTF8_LCD_WrappedProto_WIN = MSABI TTFRenderUTF8_LCD_WrappedProto;
    using TTFRenderUTF8_ShadedProto_WIN = MSABI TTFRenderUTF8_ShadedProto;
    using TTFRenderUTF8_Shaded_WrappedProto_WIN = MSABI TTFRenderUTF8_Shaded_WrappedProto;
    using TTFRenderUTF8_SolidProto_WIN = MSABI TTFRenderUTF8_SolidProto;
    using TTFRenderUTF8_Solid_WrappedProto_WIN = MSABI TTFRenderUTF8_Solid_WrappedProto;
    using TTFSetDirectionProto_WIN = MSABI TTFSetDirectionProto;
    using TTFSetFontDirectionProto_WIN = MSABI TTFSetFontDirectionProto;
    using TTFSetFontHintingProto_WIN = MSABI TTFSetFontHintingProto;
    using TTFSetFontKerningProto_WIN = MSABI TTFSetFontKerningProto;
    using TTFSetFontOutlineProto_WIN = MSABI TTFSetFontOutlineProto;
    using TTFSetFontScriptNameProto_WIN = MSABI TTFSetFontScriptNameProto;
    using TTFSetFontSizeProto_WIN = MSABI TTFSetFontSizeProto;
    using TTFSetFontSizeDPIProto_WIN = MSABI TTFSetFontSizeDPIProto;
    using TTFSetFontStyleProto_WIN = MSABI TTFSetFontStyleProto;
    using TTFSetFontWrappedAlignProto_WIN = MSABI TTFSetFontWrappedAlignProto;
    using TTFSetScriptProto_WIN = MSABI TTFSetScriptProto;
    using TTFSizeTextProto_WIN = MSABI TTFSizeTextProto;
    using TTFSizeUNICODEProto_WIN = MSABI TTFSizeUNICODEProto;
    using TTFSizeUTF8Proto_WIN = MSABI TTFSizeUTF8Proto;
    using TTFWasInitProto_WIN = MSABI TTFWasInitProto;
    using IMGFreeAnimationProto_WIN = MSABI IMGFreeAnimationProto;
    using IMGInitProto_WIN = MSABI IMGInitProto;
    using IMGisAVIFProto_WIN = MSABI IMGisAVIFProto;
    using IMGisBMPProto_WIN = MSABI IMGisBMPProto;
    using IMGisCURProto_WIN = MSABI IMGisCURProto;
    using IMGisGIFProto_WIN = MSABI IMGisGIFProto;
    using IMGisICOProto_WIN = MSABI IMGisICOProto;
    using IMGisJPGProto_WIN = MSABI IMGisJPGProto;
    using IMGisJXLProto_WIN = MSABI IMGisJXLProto;
    using IMGisLBMProto_WIN = MSABI IMGisLBMProto;
    using IMGisPCXProto_WIN = MSABI IMGisPCXProto;
    using IMGisPNGProto_WIN = MSABI IMGisPNGProto;
    using IMGisPNMProto_WIN = MSABI IMGisPNMProto;
    using IMGisQOIProto_WIN = MSABI IMGisQOIProto;
    using IMGisSVGProto_WIN = MSABI IMGisSVGProto;
    using IMGisTIFProto_WIN = MSABI IMGisTIFProto;
    using IMGisWEBPProto_WIN = MSABI IMGisWEBPProto;
    using IMGisXCFProto_WIN = MSABI IMGisXCFProto;
    using IMGisXPMProto_WIN = MSABI IMGisXPMProto;
    using IMGisXVProto_WIN = MSABI IMGisXVProto;
    using IMGLinked_VersionProto_WIN = MSABI IMGLinked_VersionProto;
    using IMGLoadProto_WIN = MSABI IMGLoadProto;
    using IMGLoad_RWProto_WIN = MSABI IMGLoad_RWProto;
    using IMGLoadAnimationProto_WIN = MSABI IMGLoadAnimationProto;
    using IMGLoadAnimation_RWProto_WIN = MSABI IMGLoadAnimation_RWProto;
    using IMGLoadAnimationTyped_RWProto_WIN = MSABI IMGLoadAnimationTyped_RWProto;
    using IMGLoadAVIF_RWProto_WIN = MSABI IMGLoadAVIF_RWProto;
    using IMGLoadBMP_RWProto_WIN = MSABI IMGLoadBMP_RWProto;
    using IMGLoadCUR_RWProto_WIN = MSABI IMGLoadCUR_RWProto;
    using IMGLoadGIF_RWProto_WIN = MSABI IMGLoadGIF_RWProto;
    using IMGLoadGIFAnimation_RWProto_WIN = MSABI IMGLoadGIFAnimation_RWProto;
    using IMGLoadICO_RWProto_WIN = MSABI IMGLoadICO_RWProto;
    using IMGLoadJPG_RWProto_WIN = MSABI IMGLoadJPG_RWProto;
    using IMGLoadJXL_RWProto_WIN = MSABI IMGLoadJXL_RWProto;
    using IMGLoadLBM_RWProto_WIN = MSABI IMGLoadLBM_RWProto;
    using IMGLoadPCX_RWProto_WIN = MSABI IMGLoadPCX_RWProto;
    using IMGLoadPNG_RWProto_WIN = MSABI IMGLoadPNG_RWProto;
    using IMGLoadPNM_RWProto_WIN = MSABI IMGLoadPNM_RWProto;
    using IMGLoadQOI_RWProto_WIN = MSABI IMGLoadQOI_RWProto;
    using IMGLoadSizedSVG_RWProto_WIN = MSABI IMGLoadSizedSVG_RWProto;
    using IMGLoadSVG_RWProto_WIN = MSABI IMGLoadSVG_RWProto;
    using IMGLoadTextureProto_WIN = MSABI IMGLoadTextureProto;
    using IMGLoadTexture_RWProto_WIN = MSABI IMGLoadTexture_RWProto;
    using IMGLoadTextureTyped_RWProto_WIN = MSABI IMGLoadTextureTyped_RWProto;
    using IMGLoadTGA_RWProto_WIN = MSABI IMGLoadTGA_RWProto;
    using IMGLoadTIF_RWProto_WIN = MSABI IMGLoadTIF_RWProto;
    using IMGLoadTyped_RWProto_WIN = MSABI IMGLoadTyped_RWProto;
    using IMGLoadWEBP_RWProto_WIN = MSABI IMGLoadWEBP_RWProto;
    using IMGLoadXCF_RWProto_WIN = MSABI IMGLoadXCF_RWProto;
    using IMGLoadXPM_RWProto_WIN = MSABI IMGLoadXPM_RWProto;
    using IMGLoadXV_RWProto_WIN = MSABI IMGLoadXV_RWProto;
    using IMGQuitProto_WIN = MSABI IMGQuitProto;
    using IMGReadXPMFromArrayProto_WIN = MSABI IMGReadXPMFromArrayProto;
    using IMGReadXPMFromArrayToRGB888Proto_WIN = MSABI IMGReadXPMFromArrayToRGB888Proto;
    using IMGSaveJPGProto_WIN = MSABI IMGSaveJPGProto;
    using IMGSaveJPG_RWProto_WIN = MSABI IMGSaveJPG_RWProto;
    using IMGSavePNGProto_WIN = MSABI IMGSavePNGProto;
    using IMGSavePNG_RWProto_WIN = MSABI IMGSavePNG_RWProto;
#endif

    /* SDL2 functions pointers */

#if 1 // To hide this list
    static void* AddEventWatch = nullptr;
    static void* AddHintCallback = nullptr;
    static void* AddTimer = nullptr;
    static void* AllocFormat = nullptr;
    static void* AllocPalette = nullptr;
    static void* AllocRW = nullptr;
    static void* AtomicAdd = nullptr;
    static void* AtomicCAS = nullptr;
    static void* AtomicCASPtr = nullptr;
    static void* AtomicGet = nullptr;
    static void* AtomicGetPtr = nullptr;
    static void* AtomicLock = nullptr;
    static void* AtomicSet = nullptr;
    static void* AtomicSetPtr = nullptr;
    static void* AtomicTryLock = nullptr;
    static void* AtomicUnlock = nullptr;
    static void* AudioInit = nullptr;
    static void* AudioQuit = nullptr;
    static void* AudioStreamAvailable = nullptr;
    static void* AudioStreamClear = nullptr;
    static void* AudioStreamFlush = nullptr;
    static void* AudioStreamGet = nullptr;
    static void* AudioStreamPut = nullptr;
    static void* BlitSurface = nullptr;
    static void* BuildAudioCVT = nullptr;
    static void* CalculateGammaRamp = nullptr;
    static void* CaptureMouse = nullptr;
    static void* ClearComposition = nullptr;
    static void* ClearError = nullptr;
    static void* ClearHints = nullptr;
    static void* ClearQueuedAudio = nullptr;
    static void* CloseAudio = nullptr;
    static void* CloseAudioDevice = nullptr;
    static void* ComposeCustomBlendMode = nullptr;
    static void* CondBroadcast = nullptr;
    static void* CondSignal = nullptr;
    static void* CondWait = nullptr;
    static void* CondWaitTimeout = nullptr;
    static void* ConvertAudio = nullptr;
    static void* ConvertPixels = nullptr;
    static void* ConvertSurface = nullptr;
    static void* ConvertSurfaceFormat = nullptr;
    static void* CreateColorCursor = nullptr;
    static void* CreateCond = nullptr;
    static void* CreateCursor = nullptr;
    static void* CreateMutex = nullptr;
    static void* CreateRenderer = nullptr;
    static void* CreateRGBSurface = nullptr;
    static void* CreateRGBSurfaceFrom = nullptr;
    static void* CreateRGBSurfaceWithFormat = nullptr;
    static void* CreateRGBSurfaceWithFormatFrom = nullptr;
    static void* CreateSemaphore = nullptr;
    static void* CreateShapedWindow = nullptr;
    static void* CreateSoftwareRenderer = nullptr;
    static void* CreateSystemCursor = nullptr;
    static void* CreateTexture = nullptr;
    static void* CreateTextureFromSurface = nullptr;
    static void* CreateThread = nullptr;
    static void* CreateThreadWithStackSize = nullptr;
    static void* CreateWindow = nullptr;
    static void* CreateWindowAndRenderer = nullptr;
    static void* CreateWindowFrom = nullptr;
    static void* Delay = nullptr;
    static void* DelEventWatch = nullptr;
    static void* DelHintCallback = nullptr;
    static void* DequeueAudio = nullptr;
    static void* DestroyCond = nullptr;
    static void* DestroyMutex = nullptr;
    static void* DestroyRenderer = nullptr;
    static void* DestroySemaphore = nullptr;
    static void* DestroyTexture = nullptr;
    static void* DestroyWindow = nullptr;
    static void* DestroyWindowSurface = nullptr;
    static void* DetachThread = nullptr;
    static void* DisableScreenSaver = nullptr;
    static void* EnableScreenSaver = nullptr;
    static void* EncloseFPoints = nullptr;
    static void* EnclosePoints = nullptr;
    static void* EventState = nullptr;
    static void* FillRect = nullptr;
    static void* FillRects = nullptr;
    static void* FilterEvents = nullptr;
    static void* FlashWindow = nullptr;
    static void* FlushEvent = nullptr;
    static void* FlushEvents = nullptr;
    static void* FreeAudioStream = nullptr;
    static void* FreeCursor = nullptr;
    static void* FreeFormat = nullptr;
    static void* FreePalette = nullptr;
    static void* FreeRW = nullptr;
    static void* FreeSurface = nullptr;
    static void* FreeWAV = nullptr;
    static void* GameControllerAddMapping = nullptr;
    static void* GameControllerAddMappingsFromRW = nullptr;
    static void* GameControllerClose = nullptr;
    static void* GameControllerEventState = nullptr;
    static void* GameControllerFromInstanceID = nullptr;
    static void* GameControllerFromPlayerIndex = nullptr;
    static void* GameControllerGetAppleSFSymbolsNameForAxis = nullptr;
    static void* GameControllerGetAppleSFSymbolsNameForButton = nullptr;
    static void* GameControllerGetAttached = nullptr;
    static void* GameControllerGetAxis = nullptr;
    static void* GameControllerGetAxisFromString = nullptr;
    static void* GameControllerGetBindForAxis = nullptr;
    static void* GameControllerGetBindForButton = nullptr;
    static void* GameControllerGetButton = nullptr;
    static void* GameControllerGetButtonFromString = nullptr;
    static void* GameControllerGetFirmwareVersion = nullptr;
    static void* GameControllerGetJoystick = nullptr;
    static void* GameControllerGetNumTouchpadFingers = nullptr;
    static void* GameControllerGetNumTouchpads = nullptr;
    static void* GameControllerGetPlayerIndex = nullptr;
    static void* GameControllerGetProduct = nullptr;
    static void* GameControllerGetProductVersion = nullptr;
    static void* GameControllerGetSensorData = nullptr;
    static void* GameControllerGetSensorDataRate = nullptr;
    static void* GameControllerGetSensorDataWithTimestamp = nullptr;
    static void* GameControllerGetSerial = nullptr;
    static void* GameControllerGetStringForAxis = nullptr;
    static void* GameControllerGetStringForButton = nullptr;
    static void* GameControllerGetTouchpadFinger = nullptr;
    static void* GameControllerGetType = nullptr;
    static void* GameControllerGetVendor = nullptr;
    static void* GameControllerHasAxis = nullptr;
    static void* GameControllerHasButton = nullptr;
    static void* GameControllerHasLED = nullptr;
    static void* GameControllerHasRumble = nullptr;
    static void* GameControllerHasRumbleTriggers = nullptr;
    static void* GameControllerHasSensor = nullptr;
    static void* GameControllerIsSensorEnabled = nullptr;
    static void* GameControllerMapping = nullptr;
    static void* GameControllerMappingForDeviceIndex = nullptr;
    static void* GameControllerMappingForGUID = nullptr;
    static void* GameControllerMappingForIndex = nullptr;
    static void* GameControllerName = nullptr;
    static void* GameControllerNameForIndex = nullptr;
    static void* GameControllerNumMappings = nullptr;
    static void* GameControllerOpen = nullptr;
    static void* GameControllerPath = nullptr;
    static void* GameControllerPathForIndex = nullptr;
    static void* GameControllerRumble = nullptr;
    static void* GameControllerRumbleTriggers = nullptr;
    static void* GameControllerSendEffect = nullptr;
    static void* GameControllerSetLED = nullptr;
    static void* GameControllerSetPlayerIndex = nullptr;
    static void* GameControllerSetSensorEnabled = nullptr;
    static void* GameControllerTypeForIndex = nullptr;
    static void* GameControllerUpdate = nullptr;
    static void* GetAssertionHandler = nullptr;
    static void* GetAssertionReport = nullptr;
    static void* GetAudioDeviceName = nullptr;
    static void* GetAudioDeviceSpec = nullptr;
    static void* GetAudioDeviceStatus = nullptr;
    static void* GetAudioDriver = nullptr;
    static void* GetAudioStatus = nullptr;
    static void* GetBasePath = nullptr;
    static void* GetClipboardText = nullptr;
    static void* GetClipRect = nullptr;
    static void* GetClosestDisplayMode = nullptr;
    static void* GetColorKey = nullptr;
    static void* GetCPUCacheLineSize = nullptr;
    static void* GetCPUCount = nullptr;
    static void* GetCurrentAudioDriver = nullptr;
    static void* GetCurrentDisplayMode = nullptr;
    static void* GetCurrentVideoDriver = nullptr;
    static void* GetCursor = nullptr;
    static void* GetDefaultAssertionHandler = nullptr;
    static void* GetDefaultAudioInfo = nullptr;
    static void* GetDefaultCursor = nullptr;
    static void* GetDesktopDisplayMode = nullptr;
    static void* GetDisplayBounds = nullptr;
    static void* GetDisplayDPI = nullptr;
    static void* GetDisplayMode = nullptr;
    static void* GetDisplayName = nullptr;
    static void* GetDisplayOrientation = nullptr;
    static void* GetDisplayUsableBounds = nullptr;
    static void* GetError = nullptr;
    static void* GetErrorMsg = nullptr;
    static void* GetEventFilter = nullptr;
    static void* GetGlobalMouseState = nullptr;
    static void* GetGrabbedWindow = nullptr;
    static void* GetHint = nullptr;
    static void* GetHintBoolean = nullptr;
    static void* GetJoystickGUIDInfo = nullptr;
    static void* GetKeyboardFocus = nullptr;
    static void* GetKeyboardState = nullptr;
    static void* GetKeyFromName = nullptr;
    static void* GetKeyFromScancode = nullptr;
    static void* GetKeyName = nullptr;
    static void* GetMemoryFunctions = nullptr;
    static void* GetModState = nullptr;
    static void* GetMouseFocus = nullptr;
    static void* GetMouseState = nullptr;
    static void* GetNumAllocations = nullptr;
    static void* GetNumAudioDevices = nullptr;
    static void* GetNumAudioDrivers = nullptr;
    static void* GetNumDisplayModes = nullptr;
    static void* GetNumRenderDrivers = nullptr;
    static void* GetNumTouchDevices = nullptr;
    static void* GetNumTouchFingers = nullptr;
    static void* GetNumVideoDisplays = nullptr;
    static void* GetNumVideoDrivers = nullptr;
    static void* GetOriginalMemoryFunctions = nullptr;
    static void* GetPerformanceCounter = nullptr;
    static void* GetPerformanceFrequency = nullptr;
    static void* GetPixelFormatName = nullptr;
    static void* GetPlatform = nullptr;
    static void* GetPointDisplayIndex = nullptr;
    static void* GetPowerInfo = nullptr;
    static void* GetPreferredLocales = nullptr;
    static void* GetPrefPath = nullptr;
    static void* GetPrimarySelectionText = nullptr;
    static void* GetQueuedAudioSize = nullptr;
    static void* GetRectDisplayIndex = nullptr;
    static void* GetRelativeMouseMode = nullptr;
    static void* GetRelativeMouseState = nullptr;
    static void* GetRenderDrawBlendMode = nullptr;
    static void* GetRenderDrawColor = nullptr;
    static void* GetRenderDriverInfo = nullptr;
    static void* GetRenderer = nullptr;
    static void* GetRendererInfo = nullptr;
    static void* GetRendererOutputSize = nullptr;
    static void* GetRenderTarget = nullptr;
    static void* GetRevision = nullptr;
    static void* GetRevisionNumber = nullptr;
    static void* GetRGB = nullptr;
    static void* GetRGBA = nullptr;
    static void* GetScancodeFromKey = nullptr;
    static void* GetScancodeFromName = nullptr;
    static void* GetScancodeName = nullptr;
    static void* GetShapedWindowMode = nullptr;
    static void* GetSurfaceAlphaMod = nullptr;
    static void* GetSurfaceBlendMode = nullptr;
    static void* GetSurfaceColorMod = nullptr;
    static void* GetSystemRAM = nullptr;
    static void* GetTextureAlphaMod = nullptr;
    static void* GetTextureBlendMode = nullptr;
    static void* GetTextureColorMod = nullptr;
    static void* GetTextureScaleMode = nullptr;
    static void* GetTextureUserData = nullptr;
    static void* GetThreadID = nullptr;
    static void* GetThreadName = nullptr;
    static void* GetTicks = nullptr;
    static void* GetTicks64 = nullptr;
    static void* GetTouchDevice = nullptr;
    static void* GetTouchDeviceType = nullptr;
    static void* GetTouchFinger = nullptr;
    static void* GetTouchName = nullptr;
    static void* GetVersion = nullptr;
    static void* GetVideoDriver = nullptr;
    static void* GetWindowBordersSize = nullptr;
    static void* GetWindowBrightness = nullptr;
    static void* GetWindowData = nullptr;
    static void* GetWindowDisplayIndex = nullptr;
    static void* GetWindowDisplayMode = nullptr;
    static void* GetWindowFlags = nullptr;
    static void* GetWindowFromID = nullptr;
    static void* GetWindowGammaRamp = nullptr;
    static void* GetWindowGrab = nullptr;
    static void* GetWindowICCProfile = nullptr;
    static void* GetWindowID = nullptr;
    static void* GetWindowKeyboardGrab = nullptr;
    static void* GetWindowMaximumSize = nullptr;
    static void* GetWindowMinimumSize = nullptr;
    static void* GetWindowMouseGrab = nullptr;
    static void* GetWindowMouseRect = nullptr;
    static void* GetWindowOpacity = nullptr;
    static void* GetWindowPixelFormat = nullptr;
    static void* GetWindowPosition = nullptr;
    static void* GetWindowSize = nullptr;
    static void* GetWindowSizeInPixels = nullptr;
    static void* GetWindowSurface = nullptr;
    static void* GetWindowTitle = nullptr;
    static void* GetYUVConversionMode = nullptr;
    static void* GetYUVConversionModeForResolution = nullptr;
    static void* GL_BindTexture = nullptr;
    static void* GL_CreateContext = nullptr;
    static void* GL_DeleteContext = nullptr;
    static void* GL_ExtensionSupported = nullptr;
    static void* GL_GetAttribute = nullptr;
    static void* GL_GetCurrentContext = nullptr;
    static void* GL_GetCurrentWindow = nullptr;
    static void* GL_GetDrawableSize = nullptr;
    static void* GL_GetProcAddress = nullptr;
    static void* GL_GetSwapInterval = nullptr;
    static void* GL_LoadLibrary = nullptr;
    static void* GL_MakeCurrent = nullptr;
    static void* GL_ResetAttributes = nullptr;
    static void* GL_SetAttribute = nullptr;
    static void* GL_SetSwapInterval = nullptr;
    static void* GL_SwapWindow = nullptr;
    static void* GL_UnbindTexture = nullptr;
    static void* GL_UnloadLibrary = nullptr;
    static void* GUIDFromString = nullptr;
    static void* GUIDToString = nullptr;
    static void* HapticClose = nullptr;
    static void* HapticDestroyEffect = nullptr;
    static void* HapticEffectSupported = nullptr;
    static void* HapticGetEffectStatus = nullptr;
    static void* HapticIndex = nullptr;
    static void* HapticName = nullptr;
    static void* HapticNewEffect = nullptr;
    static void* HapticNumAxes = nullptr;
    static void* HapticNumEffects = nullptr;
    static void* HapticNumEffectsPlaying = nullptr;
    static void* HapticOpen = nullptr;
    static void* HapticOpened = nullptr;
    static void* HapticOpenFromJoystick = nullptr;
    static void* HapticOpenFromMouse = nullptr;
    static void* HapticPause = nullptr;
    static void* HapticQuery = nullptr;
    static void* HapticRumbleInit = nullptr;
    static void* HapticRumblePlay = nullptr;
    static void* HapticRumbleStop = nullptr;
    static void* HapticRumbleSupported = nullptr;
    static void* HapticRunEffect = nullptr;
    static void* HapticSetAutocenter = nullptr;
    static void* HapticSetGain = nullptr;
    static void* HapticStopAll = nullptr;
    static void* HapticStopEffect = nullptr;
    static void* HapticUnpause = nullptr;
    static void* HapticUpdateEffect = nullptr;
    static void* Has3DNow = nullptr;
    static void* HasAltiVec = nullptr;
    static void* HasARMSIMD = nullptr;
    static void* HasAVX = nullptr;
    static void* HasAVX2 = nullptr;
    static void* HasAVX512F = nullptr;
    static void* HasClipboardText = nullptr;
    static void* HasColorKey = nullptr;
    static void* HasEvent = nullptr;
    static void* HasEvents = nullptr;
    static void* HasIntersection = nullptr;
    static void* HasIntersectionF = nullptr;
    static void* HasLASX = nullptr;
    static void* HasLSX = nullptr;
    static void* HasMMX = nullptr;
    static void* HasNEON = nullptr;
    static void* HasPrimarySelectionText = nullptr;
    static void* HasRDTSC = nullptr;
    static void* HasScreenKeyboardSupport = nullptr;
    static void* HasSSE = nullptr;
    static void* HasSSE2 = nullptr;
    static void* HasSSE3 = nullptr;
    static void* HasSSE41 = nullptr;
    static void* HasSSE42 = nullptr;
    static void* HasSurfaceRLE = nullptr;
    static void* HasWindowSurface = nullptr;
    static void* hid_ble_scan = nullptr;
    static void* hid_close = nullptr;
    static void* hid_device_change_count = nullptr;
    static void* hid_enumerate = nullptr;
    static void* hid_exit = nullptr;
    static void* hid_free_enumeration = nullptr;
    static void* hid_get_feature_report = nullptr;
    static void* hid_get_indexed_string = nullptr;
    static void* hid_get_manufacturer_string = nullptr;
    static void* hid_get_product_string = nullptr;
    static void* hid_get_serial_number_string = nullptr;
    static void* hid_init = nullptr;
    static void* hid_open = nullptr;
    static void* hid_open_path = nullptr;
    static void* hid_read = nullptr;
    static void* hid_read_timeout = nullptr;
    static void* hid_send_feature_report = nullptr;
    static void* hid_set_nonblocking = nullptr;
    static void* hid_write = nullptr;
    static void* HideWindow = nullptr;
    static void* iconv_string = nullptr;
    static void* Init = nullptr;
    static void* InitSubSystem = nullptr;
    static void* IntersectFRect = nullptr;
    static void* IntersectFRectAndLine = nullptr;
    static void* IntersectRect = nullptr;
    static void* IntersectRectAndLine = nullptr;
    static void* IsGameController = nullptr;
    static void* IsScreenKeyboardShown = nullptr;
    static void* IsScreenSaverEnabled = nullptr;
    static void* IsShapedWindow = nullptr;
    static void* IsTablet = nullptr;
    static void* IsTextInputActive = nullptr;
    static void* IsTextInputShown = nullptr;
    static void* JoystickAttachVirtual = nullptr;
    static void* JoystickAttachVirtualEx = nullptr;
    static void* JoystickClose = nullptr;
    static void* JoystickCurrentPowerLevel = nullptr;
    static void* JoystickDetachVirtual = nullptr;
    static void* JoystickEventState = nullptr;
    static void* JoystickFromInstanceID = nullptr;
    static void* JoystickFromPlayerIndex = nullptr;
    static void* JoystickGetAttached = nullptr;
    static void* JoystickGetAxis = nullptr;
    static void* JoystickGetAxisInitialState = nullptr;
    static void* JoystickGetBall = nullptr;
    static void* JoystickGetButton = nullptr;
    static void* JoystickGetDeviceGUID = nullptr;
    static void* JoystickGetDeviceInstanceID = nullptr;
    static void* JoystickGetDevicePlayerIndex = nullptr;
    static void* JoystickGetDeviceProduct = nullptr;
    static void* JoystickGetDeviceProductVersion = nullptr;
    static void* JoystickGetDeviceType = nullptr;
    static void* JoystickGetDeviceVendor = nullptr;
    static void* JoystickGetFirmwareVersion = nullptr;
    static void* JoystickGetGUID = nullptr;
    static void* JoystickGetGUIDFromString = nullptr;
    static void* JoystickGetGUIDString = nullptr;
    static void* JoystickGetHat = nullptr;
    static void* JoystickGetPlayerIndex = nullptr;
    static void* JoystickGetProduct = nullptr;
    static void* JoystickGetProductVersion = nullptr;
    static void* JoystickGetSerial = nullptr;
    static void* JoystickGetType = nullptr;
    static void* JoystickGetVendor = nullptr;
    static void* JoystickHasLED = nullptr;
    static void* JoystickHasRumble = nullptr;
    static void* JoystickHasRumbleTriggers = nullptr;
    static void* JoystickInstanceID = nullptr;
    static void* JoystickIsHaptic = nullptr;
    static void* JoystickIsVirtual = nullptr;
    static void* JoystickName = nullptr;
    static void* JoystickNameForIndex = nullptr;
    static void* JoystickNumAxes = nullptr;
    static void* JoystickNumBalls = nullptr;
    static void* JoystickNumButtons = nullptr;
    static void* JoystickNumHats = nullptr;
    static void* JoystickOpen = nullptr;
    static void* JoystickPath = nullptr;
    static void* JoystickPathForIndex = nullptr;
    static void* JoystickRumble = nullptr;
    static void* JoystickRumbleTriggers = nullptr;
    static void* JoystickSendEffect = nullptr;
    static void* JoystickSetLED = nullptr;
    static void* JoystickSetPlayerIndex = nullptr;
    static void* JoystickSetVirtualAxis = nullptr;
    static void* JoystickSetVirtualButton = nullptr;
    static void* JoystickSetVirtualHat = nullptr;
    static void* JoystickUpdate = nullptr;
    static void* LinuxSetThreadPriority = nullptr;
    static void* LinuxSetThreadPriorityAndPolicy = nullptr;
    static void* LoadBMP_RW = nullptr;
    static void* LoadDollarTemplates = nullptr;
    static void* LoadFile = nullptr;
    static void* LoadFile_RW = nullptr;
    static void* LoadFunction = nullptr;
    static void* LoadObject = nullptr;
    static void* LoadWAV_RW = nullptr;
    static void* LockAudio = nullptr;
    static void* LockAudioDevice = nullptr;
    static void* LockJoysticks = nullptr;
    static void* LockMutex = nullptr;
    static void* LockSensors = nullptr;
    static void* LockSurface = nullptr;
    static void* LockTexture = nullptr;
    static void* LockTextureToSurface = nullptr;
    static void* Log = nullptr;
    static void* LogCritical = nullptr;
    static void* LogDebug = nullptr;
    static void* LogGetOutputFunction = nullptr;
    static void* LogGetPriority = nullptr;
    static void* LogInfo = nullptr;
    static void* LogMessage = nullptr;
    static void* LogResetPriorities = nullptr;
    static void* LogSetAllPriority = nullptr;
    static void* LogSetOutputFunction = nullptr;
    static void* LogSetPriority = nullptr;
    static void* LogVerbose = nullptr;
    static void* LogWarn = nullptr;
    static void* LowerBlit = nullptr;
    static void* LowerBlitScaled = nullptr;
    static void* MapRGB = nullptr;
    static void* MapRGBA = nullptr;
    static void* MasksToPixelFormatEnum = nullptr;
    static void* MaximizeWindow = nullptr;
    static void* MemoryBarrierReleaseFunction = nullptr;
    static void* Metal_CreateView = nullptr;
    static void* Metal_DestroyView = nullptr;
    static void* Metal_GetDrawableSize = nullptr;
    static void* Metal_GetLayer = nullptr;
    static void* MinimizeWindow = nullptr;
    static void* MixAudio = nullptr;
    static void* MixAudioFormat = nullptr;
    static void* MouseIsHaptic = nullptr;
    static void* NewAudioStream = nullptr;
    static void* NumHaptics = nullptr;
    static void* NumJoysticks = nullptr;
    static void* NumSensors = nullptr;
    static void* OpenAudio = nullptr;
    static void* OpenAudioDevice = nullptr;
    static void* OpenURL = nullptr;
    static void* PauseAudio = nullptr;
    static void* PauseAudioDevice = nullptr;
    static void* PeepEvents = nullptr;
    static void* PixelFormatEnumToMasks = nullptr;
    static void* PollEvent = nullptr;
    static void* PremultiplyAlpha = nullptr;
    static void* PumpEvents = nullptr;
    static void* PushEvent = nullptr;
    static void* QueryTexture = nullptr;
    static void* QueueAudio = nullptr;
    static void* Quit = nullptr;
    static void* QuitSubSystem = nullptr;
    static void* RaiseWindow = nullptr;
    static void* ReadBE16 = nullptr;
    static void* ReadBE32 = nullptr;
    static void* ReadBE64 = nullptr;
    static void* ReadLE16 = nullptr;
    static void* ReadLE32 = nullptr;
    static void* ReadLE64 = nullptr;
    static void* ReadU8 = nullptr;
    static void* RecordGesture = nullptr;
    static void* RegisterEvents = nullptr;
    static void* RemoveTimer = nullptr;
    static void* RenderClear = nullptr;
    static void* RenderCopy = nullptr;
    static void* RenderCopyEx = nullptr;
    static void* RenderCopyExF = nullptr;
    static void* RenderCopyF = nullptr;
    static void* RenderDrawLine = nullptr;
    static void* RenderDrawLineF = nullptr;
    static void* RenderDrawLines = nullptr;
    static void* RenderDrawLinesF = nullptr;
    static void* RenderDrawPoint = nullptr;
    static void* RenderDrawPointF = nullptr;
    static void* RenderDrawPoints = nullptr;
    static void* RenderDrawPointsF = nullptr;
    static void* RenderDrawRect = nullptr;
    static void* RenderDrawRectF = nullptr;
    static void* RenderDrawRects = nullptr;
    static void* RenderDrawRectsF = nullptr;
    static void* RenderFillRect = nullptr;
    static void* RenderFillRectF = nullptr;
    static void* RenderFillRects = nullptr;
    static void* RenderFillRectsF = nullptr;
    static void* RenderFlush = nullptr;
    static void* RenderGeometry = nullptr;
    static void* RenderGeometryRaw = nullptr;
    static void* RenderGetClipRect = nullptr;
    static void* RenderGetIntegerScale = nullptr;
    static void* RenderGetLogicalSize = nullptr;
    static void* RenderGetMetalCommandEncoder = nullptr;
    static void* RenderGetMetalLayer = nullptr;
    static void* RenderGetScale = nullptr;
    static void* RenderGetViewport = nullptr;
    static void* RenderGetWindow = nullptr;
    static void* RenderIsClipEnabled = nullptr;
    static void* RenderLogicalToWindow = nullptr;
    static void* RenderPresent = nullptr;
    static void* RenderReadPixels = nullptr;
    static void* RenderSetClipRect = nullptr;
    static void* RenderSetIntegerScale = nullptr;
    static void* RenderSetLogicalSize = nullptr;
    static void* RenderSetScale = nullptr;
    static void* RenderSetViewport = nullptr;
    static void* RenderSetVSync = nullptr;
    static void* RenderTargetSupported = nullptr;
    static void* RenderWindowToLogical = nullptr;
    static void* ResetAssertionReport = nullptr;
    static void* ResetHint = nullptr;
    static void* ResetHints = nullptr;
    static void* ResetKeyboard = nullptr;
    static void* RestoreWindow = nullptr;
    static void* RWclose = nullptr;
    static void* RWFromConstMem = nullptr;
    static void* RWFromFile = nullptr;
    static void* RWFromFP = nullptr;
    static void* RWFromMem = nullptr;
    static void* RWread = nullptr;
    static void* RWseek = nullptr;
    static void* RWsize = nullptr;
    static void* RWtell = nullptr;
    static void* RWwrite = nullptr;
    static void* SaveAllDollarTemplates = nullptr;
    static void* SaveBMP_RW = nullptr;
    static void* SaveDollarTemplate = nullptr;
    static void* SemPost = nullptr;
    static void* SemTryWait = nullptr;
    static void* SemValue = nullptr;
    static void* SemWait = nullptr;
    static void* SemWaitTimeout = nullptr;
    static void* SensorClose = nullptr;
    static void* SensorFromInstanceID = nullptr;
    static void* SensorGetData = nullptr;
    static void* SensorGetDataWithTimestamp = nullptr;
    static void* SensorGetDeviceInstanceID = nullptr;
    static void* SensorGetDeviceName = nullptr;
    static void* SensorGetDeviceNonPortableType = nullptr;
    static void* SensorGetDeviceType = nullptr;
    static void* SensorGetInstanceID = nullptr;
    static void* SensorGetName = nullptr;
    static void* SensorGetNonPortableType = nullptr;
    static void* SensorGetType = nullptr;
    static void* SensorOpen = nullptr;
    static void* SensorUpdate = nullptr;
    static void* SetAssertionHandler = nullptr;
    static void* SetClipboardText = nullptr;
    static void* SetClipRect = nullptr;
    static void* SetColorKey = nullptr;
    static void* SetCursor = nullptr;
    static void* SetError = nullptr;
    static void* SetEventFilter = nullptr;
    static void* SetHint = nullptr;
    static void* SetHintWithPriority = nullptr;
    static void* SetMainReady = nullptr;
    static void* SetMemoryFunctions = nullptr;
    static void* SetModState = nullptr;
    static void* SetPaletteColors = nullptr;
    static void* SetPixelFormatPalette = nullptr;
    static void* SetPrimarySelectionText = nullptr;
    static void* SetRelativeMouseMode = nullptr;
    static void* SetRenderDrawBlendMode = nullptr;
    static void* SetRenderDrawColor = nullptr;
    static void* SetRenderTarget = nullptr;
    static void* SetSurfaceAlphaMod = nullptr;
    static void* SetSurfaceBlendMode = nullptr;
    static void* SetSurfaceColorMod = nullptr;
    static void* SetSurfacePalette = nullptr;
    static void* SetSurfaceRLE = nullptr;
    static void* SetTextInputRect = nullptr;
    static void* SetTextureAlphaMod = nullptr;
    static void* SetTextureBlendMode = nullptr;
    static void* SetTextureColorMod = nullptr;
    static void* SetTextureScaleMode = nullptr;
    static void* SetTextureUserData = nullptr;
    static void* SetThreadPriority = nullptr;
    static void* SetWindowAlwaysOnTop = nullptr;
    static void* SetWindowBordered = nullptr;
    static void* SetWindowBrightness = nullptr;
    static void* SetWindowData = nullptr;
    static void* SetWindowDisplayMode = nullptr;
    static void* SetWindowFullscreen = nullptr;
    static void* SetWindowGammaRamp = nullptr;
    static void* SetWindowGrab = nullptr;
    static void* SetWindowHitTest = nullptr;
    static void* SetWindowIcon = nullptr;
    static void* SetWindowInputFocus = nullptr;
    static void* SetWindowKeyboardGrab = nullptr;
    static void* SetWindowMaximumSize = nullptr;
    static void* SetWindowMinimumSize = nullptr;
    static void* SetWindowModalFor = nullptr;
    static void* SetWindowMouseGrab = nullptr;
    static void* SetWindowMouseRect = nullptr;
    static void* SetWindowOpacity = nullptr;
    static void* SetWindowPosition = nullptr;
    static void* SetWindowResizable = nullptr;
    static void* SetWindowShape = nullptr;
    static void* SetWindowSize = nullptr;
    static void* SetWindowTitle = nullptr;
    static void* SetYUVConversionMode = nullptr;
    static void* ShowCursor = nullptr;
    static void* ShowMessageBox = nullptr;
    static void* ShowSimpleMessageBox = nullptr;
    static void* ShowWindow = nullptr;
    static void* SIMDAlloc = nullptr;
    static void* SIMDFree = nullptr;
    static void* SIMDGetAlignment = nullptr;
    static void* SIMDRealloc = nullptr;
    static void* SoftStretch = nullptr;
    static void* SoftStretchLinear = nullptr;
    static void* StartTextInput = nullptr;
    static void* StopTextInput = nullptr;
    static void* ThreadID = nullptr;
    static void* TLSCleanup = nullptr;
    static void* TLSCreate = nullptr;
    static void* TLSGet = nullptr;
    static void* TLSSet = nullptr;
    static void* TryLockMutex = nullptr;
    static void* UnionFRect = nullptr;
    static void* UnionRect = nullptr;
    static void* UnloadObject = nullptr;
    static void* UnlockAudio = nullptr;
    static void* UnlockAudioDevice = nullptr;
    static void* UnlockJoysticks = nullptr;
    static void* UnlockMutex = nullptr;
    static void* UnlockSurface = nullptr;
    static void* UnlockTexture = nullptr;
    static void* UpdateNVTexture = nullptr;
    static void* UpdateTexture = nullptr;
    static void* UpdateWindowSurface = nullptr;
    static void* UpdateWindowSurfaceRects = nullptr;
    static void* UpdateYUVTexture = nullptr;
    static void* UpperBlit = nullptr;
    static void* UpperBlitScaled = nullptr;
    static void* VERSIONNUM = nullptr;
    static void* VideoInit = nullptr;
    static void* VideoQuit = nullptr;
#ifdef COSMO_SDL2_VULKAN
    static void* Vulkan_CreateSurface = nullptr;
    static void* Vulkan_GetDrawableSize = nullptr;
    static void* Vulkan_GetInstanceExtensions = nullptr;
    static void* Vulkan_GetVkGetInstanceProcAddr = nullptr;
    static void* Vulkan_GetVkInstanceProcAddr = nullptr;
    static void* Vulkan_LoadLibrary = nullptr;
    static void* Vulkan_UnloadLibrary = nullptr;
#endif
    static void* WaitEvent = nullptr;
    static void* WaitEventTimeout = nullptr;
    static void* WaitThread = nullptr;
    static void* WarpMouseGlobal = nullptr;
    static void* WarpMouseInWindow = nullptr;
    static void* WasInit = nullptr;
    static void* WriteBE16 = nullptr;
    static void* WriteBE32 = nullptr;
    static void* WriteBE64 = nullptr;
    static void* WriteLE16 = nullptr;
    static void* WriteLE32 = nullptr;
    static void* WriteLE64 = nullptr;
    static void* WriteU8 = nullptr;
    static void* NetAddSocket = nullptr;
    static void* NetAllocPacket = nullptr;
    static void* NetAllocPacketV = nullptr;
    static void* NetAllocSocketSet = nullptr;
    static void* NetCheckSockets = nullptr;
    static void* NetDelSocket = nullptr;
    static void* NetFreePacket = nullptr;
    static void* NetFreePacketV = nullptr;
    static void* NetFreeSocketSet = nullptr;
    static void* NetGetError = nullptr;
    static void* NetGetLocalAddresses = nullptr;
    static void* NetInit = nullptr;
    static void* NetLinked_Version = nullptr;
    static void* NetQuit = nullptr;
    static void* NetResizePacket = nullptr;
    static void* NetResolveHost = nullptr;
    static void* NetResolveIP = nullptr;
    static void* NetTCP_Accept = nullptr;
    static void* NetTCP_Close = nullptr;
    static void* NetTCP_GetPeerAddress = nullptr;
    static void* NetTCP_Open = nullptr;
    static void* NetTCP_Recv = nullptr;
    static void* NetTCP_Send = nullptr;
    static void* NetUDP_Bind = nullptr;
    static void* NetUDP_Close = nullptr;
    static void* NetUDP_GetPeerAddress = nullptr;
    static void* NetUDP_Open = nullptr;
    static void* NetUDP_Recv = nullptr;
    static void* NetUDP_RecvV = nullptr;
    static void* NetUDP_Send = nullptr;
    static void* NetUDP_SendV = nullptr;
    static void* NetUDP_SetPacketLoss = nullptr;
    static void* NetUDP_Unbind = nullptr;
    static void* MixAllocateChannels = nullptr;
    static void* MixChannelFinished = nullptr;
    static void* MixCloseAudio = nullptr;
    static void* MixEachSoundFont = nullptr;
    static void* MixExpireChannel = nullptr;
    static void* MixFadeInChannel = nullptr;
    static void* MixFadeInChannelTimed = nullptr;
    static void* MixFadeInMusic = nullptr;
    static void* MixFadeInMusicPos = nullptr;
    static void* MixFadeOutChannel = nullptr;
    static void* MixFadeOutGroup = nullptr;
    static void* MixFadeOutMusic = nullptr;
    static void* MixFadingChannel = nullptr;
    static void* MixFadingMusic = nullptr;
    static void* MixFreeChunk = nullptr;
    static void* MixFreeMusic = nullptr;
    static void* MixGetChunk = nullptr;
    static void* MixGetChunkDecoder = nullptr;
    static void* MixGetMusicAlbumTag = nullptr;
    static void* MixGetMusicArtistTag = nullptr;
    static void* MixGetMusicCopyrightTag = nullptr;
    static void* MixGetMusicDecoder = nullptr;
    static void* MixGetMusicHookData = nullptr;
    static void* MixGetMusicLoopEndTime = nullptr;
    static void* MixGetMusicLoopLengthTime = nullptr;
    static void* MixGetMusicLoopStartTime = nullptr;
    static void* MixGetMusicPosition = nullptr;
    static void* MixGetMusicTitle = nullptr;
    static void* MixGetMusicTitleTag = nullptr;
    static void* MixGetMusicType = nullptr;
    static void* MixGetMusicVolume = nullptr;
    static void* MixGetNumChunkDecoders = nullptr;
    static void* MixGetNumMusicDecoders = nullptr;
    static void* MixGetSoundFonts = nullptr;
    static void* MixGetSynchroValue = nullptr;
    static void* MixGetTimidityCfg = nullptr;
    static void* MixGroupAvailable = nullptr;
    static void* MixGroupChannel = nullptr;
    static void* MixGroupChannels = nullptr;
    static void* MixGroupCount = nullptr;
    static void* MixGroupNewer = nullptr;
    static void* MixGroupOldest = nullptr;
    static void* MixHaltChannel = nullptr;
    static void* MixHaltGroup = nullptr;
    static void* MixHaltMusic = nullptr;
    static void* MixHasChunkDecoder = nullptr;
    static void* MixHasMusicDecoder = nullptr;
    static void* MixHookMusic = nullptr;
    static void* MixHookMusicFinished = nullptr;
    static void* MixInit = nullptr;
    static void* MixLinked_Version = nullptr;
    static void* MixLoadMUS = nullptr;
    static void* MixLoadMUS_RW = nullptr;
    static void* MixLoadMUSType_RW = nullptr;
    static void* MixLoadWAV = nullptr;
    static void* MixLoadWAV_RW = nullptr;
    static void* MixMasterVolume = nullptr;
    static void* MixModMusicJumpToOrder = nullptr;
    static void* MixMusicDuration = nullptr;
    static void* MixOpenAudio = nullptr;
    static void* MixOpenAudioDevice = nullptr;
    static void* MixPause = nullptr;
    static void* MixPaused = nullptr;
    static void* MixPausedMusic = nullptr;
    static void* MixPauseMusic = nullptr;
    static void* MixPlayChannel = nullptr;
    static void* MixPlayChannelTimed = nullptr;
    static void* MixPlaying = nullptr;
    static void* MixPlayingMusic = nullptr;
    static void* MixPlayMusic = nullptr;
    static void* MixQuerySpec = nullptr;
    static void* MixQuickLoad_RAW = nullptr;
    static void* MixQuickLoad_WAV = nullptr;
    static void* MixQuit = nullptr;
    static void* MixRegisterEffect = nullptr;
    static void* MixReserveChannels = nullptr;
    static void* MixResume = nullptr;
    static void* MixResumeMusic = nullptr;
    static void* MixRewindMusic = nullptr;
    static void* MixSetDistance = nullptr;
    static void* MixSetMusicCMD = nullptr;
    static void* MixSetMusicPosition = nullptr;
    static void* MixSetPanning = nullptr;
    static void* MixSetPosition = nullptr;
    static void* MixSetPostMix = nullptr;
    static void* MixSetReverseStereo = nullptr;
    static void* MixSetSoundFonts = nullptr;
    static void* MixSetSynchroValue = nullptr;
    static void* MixSetTimidityCfg = nullptr;
    static void* MixUnregisterAllEffects = nullptr;
    static void* MixUnregisterEffect = nullptr;
    static void* MixVolume = nullptr;
    static void* MixVolumeChunk = nullptr;
    static void* MixVolumeMusic = nullptr;
    static void* TTFByteSwappedUNICODE = nullptr;
    static void* TTFCloseFont = nullptr;
    static void* TTFFontAscent = nullptr;
    static void* TTFFontDescent = nullptr;
    static void* TTFFontFaceFamilyName = nullptr;
    static void* TTFFontFaceIsFixedWidth = nullptr;
    static void* TTFFontFaces = nullptr;
    static void* TTFFontFaceStyleName = nullptr;
    static void* TTFFontHeight = nullptr;
    static void* TTFFontLineSkip = nullptr;
    static void* TTFGetFontHinting = nullptr;
    static void* TTFGetFontKerning = nullptr;
    static void* TTFGetFontOutline = nullptr;
    static void* TTFGetFontStyle = nullptr;
    static void* TTFGetFontWrappedAlign = nullptr;
    static void* TTFGetFreeTypeVersion = nullptr;
    static void* TTFGetHarfBuzzVersion = nullptr;
    static void* TTFGlyphIsProvided = nullptr;
    static void* TTFGlyphIsProvided32 = nullptr;
    static void* TTFGlyphMetrics = nullptr;
    static void* TTFGlyphMetrics32 = nullptr;
    static void* TTFInit = nullptr;
    static void* TTFLinked_Version = nullptr;
    static void* TTFMeasureText = nullptr;
    static void* TTFMeasureUNICODE = nullptr;
    static void* TTFMeasureUTF8 = nullptr;
    static void* TTFOpenFont = nullptr;
    static void* TTFOpenFontDPI = nullptr;
    static void* TTFOpenFontDPIRW = nullptr;
    static void* TTFOpenFontIndex = nullptr;
    static void* TTFOpenFontIndexDPI = nullptr;
    static void* TTFOpenFontIndexDPIRW = nullptr;
    static void* TTFOpenFontIndexRW = nullptr;
    static void* TTFOpenFontRW = nullptr;
    static void* TTFQuit = nullptr;
    static void* TTFRenderGlyph32_Blended = nullptr;
    static void* TTFRenderGlyph32_LCD = nullptr;
    static void* TTFRenderGlyph32_Shaded = nullptr;
    static void* TTFRenderGlyph32_Solid = nullptr;
    static void* TTFRenderGlyph_Blended = nullptr;
    static void* TTFRenderGlyph_LCD = nullptr;
    static void* TTFRenderGlyph_Shaded = nullptr;
    static void* TTFRenderGlyph_Solid = nullptr;
    static void* TTFRenderText_Blended = nullptr;
    static void* TTFRenderText_Blended_Wrapped = nullptr;
    static void* TTFRenderText_LCD = nullptr;
    static void* TTFRenderText_LCD_Wrapped = nullptr;
    static void* TTFRenderText_Shaded = nullptr;
    static void* TTFRenderText_Shaded_Wrapped = nullptr;
    static void* TTFRenderText_Solid = nullptr;
    static void* TTFRenderText_Solid_Wrapped = nullptr;
    static void* TTFRenderUNICODE_Blended = nullptr;
    static void* TTFRenderUNICODE_Blended_Wrapped = nullptr;
    static void* TTFRenderUNICODE_LCD = nullptr;
    static void* TTFRenderUNICODE_LCD_Wrapped = nullptr;
    static void* TTFRenderUNICODE_Shaded = nullptr;
    static void* TTFRenderUNICODE_Shaded_Wrapped = nullptr;
    static void* TTFRenderUNICODE_Solid = nullptr;
    static void* TTFRenderUNICODE_Solid_Wrapped = nullptr;
    static void* TTFRenderUTF8_Blended = nullptr;
    static void* TTFRenderUTF8_Blended_Wrapped = nullptr;
    static void* TTFRenderUTF8_LCD = nullptr;
    static void* TTFRenderUTF8_LCD_Wrapped = nullptr;
    static void* TTFRenderUTF8_Shaded = nullptr;
    static void* TTFRenderUTF8_Shaded_Wrapped = nullptr;
    static void* TTFRenderUTF8_Solid = nullptr;
    static void* TTFRenderUTF8_Solid_Wrapped = nullptr;
    static void* TTFSetDirection = nullptr;
    static void* TTFSetFontDirection = nullptr;
    static void* TTFSetFontHinting = nullptr;
    static void* TTFSetFontKerning = nullptr;
    static void* TTFSetFontOutline = nullptr;
    static void* TTFSetFontScriptName = nullptr;
    static void* TTFSetFontSize = nullptr;
    static void* TTFSetFontSizeDPI = nullptr;
    static void* TTFSetFontStyle = nullptr;
    static void* TTFSetFontWrappedAlign = nullptr;
    static void* TTFSetScript = nullptr;
    static void* TTFSizeText = nullptr;
    static void* TTFSizeUNICODE = nullptr;
    static void* TTFSizeUTF8 = nullptr;
    static void* TTFWasInit = nullptr;
    static void* IMGFreeAnimation = nullptr;
    static void* IMGInit = nullptr;
    static void* IMGisAVIF = nullptr;
    static void* IMGisBMP = nullptr;
    static void* IMGisCUR = nullptr;
    static void* IMGisGIF = nullptr;
    static void* IMGisICO = nullptr;
    static void* IMGisJPG = nullptr;
    static void* IMGisJXL = nullptr;
    static void* IMGisLBM = nullptr;
    static void* IMGisPCX = nullptr;
    static void* IMGisPNG = nullptr;
    static void* IMGisPNM = nullptr;
    static void* IMGisQOI = nullptr;
    static void* IMGisSVG = nullptr;
    static void* IMGisTIF = nullptr;
    static void* IMGisWEBP = nullptr;
    static void* IMGisXCF = nullptr;
    static void* IMGisXPM = nullptr;
    static void* IMGisXV = nullptr;
    static void* IMGLinked_Version = nullptr;
    static void* IMGLoad = nullptr;
    static void* IMGLoad_RW = nullptr;
    static void* IMGLoadAnimation = nullptr;
    static void* IMGLoadAnimation_RW = nullptr;
    static void* IMGLoadAnimationTyped_RW = nullptr;
    static void* IMGLoadAVIF_RW = nullptr;
    static void* IMGLoadBMP_RW = nullptr;
    static void* IMGLoadCUR_RW = nullptr;
    static void* IMGLoadGIF_RW = nullptr;
    static void* IMGLoadGIFAnimation_RW = nullptr;
    static void* IMGLoadICO_RW = nullptr;
    static void* IMGLoadJPG_RW = nullptr;
    static void* IMGLoadJXL_RW = nullptr;
    static void* IMGLoadLBM_RW = nullptr;
    static void* IMGLoadPCX_RW = nullptr;
    static void* IMGLoadPNG_RW = nullptr;
    static void* IMGLoadPNM_RW = nullptr;
    static void* IMGLoadQOI_RW = nullptr;
    static void* IMGLoadSizedSVG_RW = nullptr;
    static void* IMGLoadSVG_RW = nullptr;
    static void* IMGLoadTexture = nullptr;
    static void* IMGLoadTexture_RW = nullptr;
    static void* IMGLoadTextureTyped_RW = nullptr;
    static void* IMGLoadTGA_RW = nullptr;
    static void* IMGLoadTIF_RW = nullptr;
    static void* IMGLoadTyped_RW = nullptr;
    static void* IMGLoadWEBP_RW = nullptr;
    static void* IMGLoadXCF_RW = nullptr;
    static void* IMGLoadXPM_RW = nullptr;
    static void* IMGLoadXV_RW = nullptr;
    static void* IMGQuit = nullptr;
    static void* IMGReadXPMFromArray = nullptr;
    static void* IMGReadXPMFromArrayToRGB888 = nullptr;
    static void* IMGSaveJPG = nullptr;
    static void* IMGSaveJPG_RW = nullptr;
    static void* IMGSavePNG = nullptr;
    static void* IMGSavePNG_RW = nullptr;
#endif

} // namespace

namespace SDL2 {

    void SwitchLog(bool do_log) {
        ::do_log = do_log;
    }
    
    bool OpenRequiredLibrary(const std::string& libname, void*& pointer, std::string filename, const std::string& library_path) {
        if (not std::filesystem::exists(library_path + filename) and not UnpackFile(library_path + filename, ::do_log)) {
            if (::do_log) LogError("Couldn't find the " + libname + " library around. (Attempt to use system library)", ErrorLevel::warning);
        }
        else filename = library_path + filename;
        pointer = cosmo_dlopen(filename.c_str(), RTLD_LAZY);
        if (pointer == nullptr) {
            if (::do_log) LogError(std::string("Couldn't open the " + libname + " library (") + cosmo_dlerror() + ").");
            return false;
        }
        return true;
    }
    
    bool LoadSDLLibrary(const std::string& library_path) {
        if (not OpenRequiredLibrary("SDL2", ::sdllibptr, IsWindows() ? "SDL2.dll" : "libSDL2.so", library_path)) return false;
        if (not OpenRequiredLibrary("SDL2 Net", ::sdlnetlibptr, IsWindows() ? "SDL2_net.dll" : "libSDL2_net.so", library_path)) return false;
        if (not OpenRequiredLibrary("SDL2 Mixer", ::sdlmixlibptr, IsWindows() ? "SDL2_mixer.dll" : "libSDL2_mixer.so", library_path)) return false;
        if (not OpenRequiredLibrary("SDL2 TTF", ::sdlttflibptr, IsWindows() ? "SDL2_ttf.dll" : "libSDL2_ttf.so", library_path)) return false;
        if (not OpenRequiredLibrary("SDL2 Image", ::sdlimglibptr, IsWindows() ? "SDL2_image.dll" : "libSDL2_image.so", library_path)) return false;
        bool err = false;
#if 1 // To hide the list
        LOADFUNC(sdllibptr, AddEventWatch, "SDL_AddEventWatch")
        LOADFUNC(sdllibptr, AddHintCallback, "SDL_AddHintCallback")
        LOADFUNC(sdllibptr, AddTimer, "SDL_AddTimer")
        LOADFUNC(sdllibptr, AllocFormat, "SDL_AllocFormat")
        LOADFUNC(sdllibptr, AllocPalette, "SDL_AllocPalette")
        LOADFUNC(sdllibptr, AllocRW, "SDL_AllocRW")
        LOADFUNC(sdllibptr, AtomicAdd, "SDL_AtomicAdd")
        LOADFUNC(sdllibptr, AtomicCAS, "SDL_AtomicCAS")
        LOADFUNC(sdllibptr, AtomicCASPtr, "SDL_AtomicCASPtr")
        LOADFUNC(sdllibptr, AtomicGet, "SDL_AtomicGet")
        LOADFUNC(sdllibptr, AtomicGetPtr, "SDL_AtomicGetPtr")
        LOADFUNC(sdllibptr, AtomicLock, "SDL_AtomicLock")
        LOADFUNC(sdllibptr, AtomicSet, "SDL_AtomicSet")
        LOADFUNC(sdllibptr, AtomicSetPtr, "SDL_AtomicSetPtr")
        LOADFUNC(sdllibptr, AtomicTryLock, "SDL_AtomicTryLock")
        LOADFUNC(sdllibptr, AtomicUnlock, "SDL_AtomicUnlock")
        LOADFUNC(sdllibptr, AudioInit, "SDL_AudioInit")
        LOADFUNC(sdllibptr, AudioQuit, "SDL_AudioQuit")
        LOADFUNC(sdllibptr, AudioStreamAvailable, "SDL_AudioStreamAvailable")
        LOADFUNC(sdllibptr, AudioStreamClear, "SDL_AudioStreamClear")
        LOADFUNC(sdllibptr, AudioStreamFlush, "SDL_AudioStreamFlush")
        LOADFUNC(sdllibptr, AudioStreamGet, "SDL_AudioStreamGet")
        LOADFUNC(sdllibptr, AudioStreamPut, "SDL_AudioStreamPut")
        LOADFUNC(sdllibptr, BlitSurface, "SDL_UpperBlit")
        LOADFUNC(sdllibptr, BuildAudioCVT, "SDL_BuildAudioCVT")
        LOADFUNC(sdllibptr, CalculateGammaRamp, "SDL_CalculateGammaRamp")
        LOADFUNC(sdllibptr, CaptureMouse, "SDL_CaptureMouse")
        LOADFUNC(sdllibptr, ClearComposition, "SDL_ClearComposition")
        LOADFUNC(sdllibptr, ClearError, "SDL_ClearError")
        LOADFUNC(sdllibptr, ClearHints, "SDL_ClearHints")
        LOADFUNC(sdllibptr, ClearQueuedAudio, "SDL_ClearQueuedAudio")
        LOADFUNC(sdllibptr, CloseAudio, "SDL_CloseAudio")
        LOADFUNC(sdllibptr, CloseAudioDevice, "SDL_CloseAudioDevice")
        LOADFUNC(sdllibptr, ComposeCustomBlendMode, "SDL_ComposeCustomBlendMode")
        LOADFUNC(sdllibptr, CondBroadcast, "SDL_CondBroadcast")
        LOADFUNC(sdllibptr, CondSignal, "SDL_CondSignal")
        LOADFUNC(sdllibptr, CondWait, "SDL_CondWait")
        LOADFUNC(sdllibptr, CondWaitTimeout, "SDL_CondWaitTimeout")
        LOADFUNC(sdllibptr, ConvertAudio, "SDL_ConvertAudio")
        LOADFUNC(sdllibptr, ConvertPixels, "SDL_ConvertPixels")
        LOADFUNC(sdllibptr, ConvertSurface, "SDL_ConvertSurface")
        LOADFUNC(sdllibptr, ConvertSurfaceFormat, "SDL_ConvertSurfaceFormat")
        LOADFUNC(sdllibptr, CreateColorCursor, "SDL_CreateColorCursor")
        LOADFUNC(sdllibptr, CreateCond, "SDL_CreateCond")
        LOADFUNC(sdllibptr, CreateCursor, "SDL_CreateCursor")
        LOADFUNC(sdllibptr, CreateMutex, "SDL_CreateMutex")
        LOADFUNC(sdllibptr, CreateRenderer, "SDL_CreateRenderer")
        LOADFUNC(sdllibptr, CreateRGBSurface, "SDL_CreateRGBSurface")
        LOADFUNC(sdllibptr, CreateRGBSurfaceFrom, "SDL_CreateRGBSurfaceFrom")
        LOADFUNC(sdllibptr, CreateRGBSurfaceWithFormat, "SDL_CreateRGBSurfaceWithFormat")
        LOADFUNC(sdllibptr, CreateRGBSurfaceWithFormatFrom, "SDL_CreateRGBSurfaceWithFormatFrom")
        LOADFUNC(sdllibptr, CreateSemaphore, "SDL_CreateSemaphore")
        LOADFUNC(sdllibptr, CreateShapedWindow, "SDL_CreateShapedWindow")
        LOADFUNC(sdllibptr, CreateSoftwareRenderer, "SDL_CreateSoftwareRenderer")
        LOADFUNC(sdllibptr, CreateSystemCursor, "SDL_CreateSystemCursor")
        LOADFUNC(sdllibptr, CreateTexture, "SDL_CreateTexture")
        LOADFUNC(sdllibptr, CreateTextureFromSurface, "SDL_CreateTextureFromSurface")
        LOADFUNC(sdllibptr, CreateThread, "SDL_CreateThread")
        LOADFUNC(sdllibptr, CreateThreadWithStackSize, "SDL_CreateThreadWithStackSize")
        LOADFUNC(sdllibptr, CreateWindow, "SDL_CreateWindow")
        LOADFUNC(sdllibptr, CreateWindowAndRenderer, "SDL_CreateWindowAndRenderer")
        LOADFUNC(sdllibptr, CreateWindowFrom, "SDL_CreateWindowFrom")
        LOADFUNC(sdllibptr, Delay, "SDL_Delay")
        LOADFUNC(sdllibptr, DelEventWatch, "SDL_DelEventWatch")
        LOADFUNC(sdllibptr, DelHintCallback, "SDL_DelHintCallback")
        LOADFUNC(sdllibptr, DequeueAudio, "SDL_DequeueAudio")
        LOADFUNC(sdllibptr, DestroyCond, "SDL_DestroyCond")
        LOADFUNC(sdllibptr, DestroyMutex, "SDL_DestroyMutex")
        LOADFUNC(sdllibptr, DestroyRenderer, "SDL_DestroyRenderer")
        LOADFUNC(sdllibptr, DestroySemaphore, "SDL_DestroySemaphore")
        LOADFUNC(sdllibptr, DestroyTexture, "SDL_DestroyTexture")
        LOADFUNC(sdllibptr, DestroyWindow, "SDL_DestroyWindow")
        LOADFUNC(sdllibptr, DestroyWindowSurface, "SDL_DestroyWindowSurface")
        LOADFUNC(sdllibptr, DetachThread, "SDL_DetachThread")
        LOADFUNC(sdllibptr, DisableScreenSaver, "SDL_DisableScreenSaver")
        LOADFUNC(sdllibptr, EnableScreenSaver, "SDL_EnableScreenSaver")
        LOADFUNC(sdllibptr, EncloseFPoints, "SDL_EncloseFPoints")
        LOADFUNC(sdllibptr, EnclosePoints, "SDL_EnclosePoints")
        LOADFUNC(sdllibptr, EventState, "SDL_EventState")
        LOADFUNC(sdllibptr, FillRect, "SDL_FillRect")
        LOADFUNC(sdllibptr, FillRects, "SDL_FillRects")
        LOADFUNC(sdllibptr, FilterEvents, "SDL_FilterEvents")
        LOADFUNC(sdllibptr, FlashWindow, "SDL_FlashWindow")
        LOADFUNC(sdllibptr, FlushEvent, "SDL_FlushEvent")
        LOADFUNC(sdllibptr, FlushEvents, "SDL_FlushEvents")
        LOADFUNC(sdllibptr, FreeAudioStream, "SDL_FreeAudioStream")
        LOADFUNC(sdllibptr, FreeCursor, "SDL_FreeCursor")
        LOADFUNC(sdllibptr, FreeFormat, "SDL_FreeFormat")
        LOADFUNC(sdllibptr, FreePalette, "SDL_FreePalette")
        LOADFUNC(sdllibptr, FreeRW, "SDL_FreeRW")
        LOADFUNC(sdllibptr, FreeSurface, "SDL_FreeSurface")
        LOADFUNC(sdllibptr, FreeWAV, "SDL_FreeWAV")
        LOADFUNC(sdllibptr, GameControllerAddMapping, "SDL_GameControllerAddMapping")
        LOADFUNC(sdllibptr, GameControllerAddMappingsFromRW, "SDL_GameControllerAddMappingsFromRW")
        LOADFUNC(sdllibptr, GameControllerClose, "SDL_GameControllerClose")
        LOADFUNC(sdllibptr, GameControllerEventState, "SDL_GameControllerEventState")
        LOADFUNC(sdllibptr, GameControllerFromInstanceID, "SDL_GameControllerFromInstanceID")
        LOADFUNC(sdllibptr, GameControllerFromPlayerIndex, "SDL_GameControllerFromPlayerIndex")
        LOADFUNC(sdllibptr, GameControllerGetAppleSFSymbolsNameForAxis, "SDL_GameControllerGetAppleSFSymbolsNameForAxis")
        LOADFUNC(sdllibptr, GameControllerGetAppleSFSymbolsNameForButton, "SDL_GameControllerGetAppleSFSymbolsNameForButton")
        LOADFUNC(sdllibptr, GameControllerGetAttached, "SDL_GameControllerGetAttached")
        LOADFUNC(sdllibptr, GameControllerGetAxis, "SDL_GameControllerGetAxis")
        LOADFUNC(sdllibptr, GameControllerGetAxisFromString, "SDL_GameControllerGetAxisFromString")
        LOADFUNC(sdllibptr, GameControllerGetBindForAxis, "SDL_GameControllerGetBindForAxis")
        LOADFUNC(sdllibptr, GameControllerGetBindForButton, "SDL_GameControllerGetBindForButton")
        LOADFUNC(sdllibptr, GameControllerGetButton, "SDL_GameControllerGetButton")
        LOADFUNC(sdllibptr, GameControllerGetButtonFromString, "SDL_GameControllerGetButtonFromString")
        LOADFUNC(sdllibptr, GameControllerGetFirmwareVersion, "SDL_GameControllerGetFirmwareVersion")
        LOADFUNC(sdllibptr, GameControllerGetJoystick, "SDL_GameControllerGetJoystick")
        LOADFUNC(sdllibptr, GameControllerGetNumTouchpadFingers, "SDL_GameControllerGetNumTouchpadFingers")
        LOADFUNC(sdllibptr, GameControllerGetNumTouchpads, "SDL_GameControllerGetNumTouchpads")
        LOADFUNC(sdllibptr, GameControllerGetPlayerIndex, "SDL_GameControllerGetPlayerIndex")
        LOADFUNC(sdllibptr, GameControllerGetProduct, "SDL_GameControllerGetProduct")
        LOADFUNC(sdllibptr, GameControllerGetProductVersion, "SDL_GameControllerGetProductVersion")
        LOADFUNC(sdllibptr, GameControllerGetSensorData, "SDL_GameControllerGetSensorData")
        LOADFUNC(sdllibptr, GameControllerGetSensorDataRate, "SDL_GameControllerGetSensorDataRate")
        LOADFUNC(sdllibptr, GameControllerGetSensorDataWithTimestamp, "SDL_GameControllerGetSensorDataWithTimestamp")
        LOADFUNC(sdllibptr, GameControllerGetSerial, "SDL_GameControllerGetSerial")
        LOADFUNC(sdllibptr, GameControllerGetStringForAxis, "SDL_GameControllerGetStringForAxis")
        LOADFUNC(sdllibptr, GameControllerGetStringForButton, "SDL_GameControllerGetStringForButton")
        LOADFUNC(sdllibptr, GameControllerGetTouchpadFinger, "SDL_GameControllerGetTouchpadFinger")
        LOADFUNC(sdllibptr, GameControllerGetType, "SDL_GameControllerGetType")
        LOADFUNC(sdllibptr, GameControllerGetVendor, "SDL_GameControllerGetVendor")
        LOADFUNC(sdllibptr, GameControllerHasAxis, "SDL_GameControllerHasAxis")
        LOADFUNC(sdllibptr, GameControllerHasButton, "SDL_GameControllerHasButton")
        LOADFUNC(sdllibptr, GameControllerHasLED, "SDL_GameControllerHasLED")
        LOADFUNC(sdllibptr, GameControllerHasRumble, "SDL_GameControllerHasRumble")
        LOADFUNC(sdllibptr, GameControllerHasRumbleTriggers, "SDL_GameControllerHasRumbleTriggers")
        LOADFUNC(sdllibptr, GameControllerHasSensor, "SDL_GameControllerHasSensor")
        LOADFUNC(sdllibptr, GameControllerIsSensorEnabled, "SDL_GameControllerIsSensorEnabled")
        LOADFUNC(sdllibptr, GameControllerMapping, "SDL_GameControllerMapping")
        LOADFUNC(sdllibptr, GameControllerMappingForDeviceIndex, "SDL_GameControllerMappingForDeviceIndex")
        LOADFUNC(sdllibptr, GameControllerMappingForGUID, "SDL_GameControllerMappingForGUID")
        LOADFUNC(sdllibptr, GameControllerMappingForIndex, "SDL_GameControllerMappingForIndex")
        LOADFUNC(sdllibptr, GameControllerName, "SDL_GameControllerName")
        LOADFUNC(sdllibptr, GameControllerNameForIndex, "SDL_GameControllerNameForIndex")
        LOADFUNC(sdllibptr, GameControllerNumMappings, "SDL_GameControllerNumMappings")
        LOADFUNC(sdllibptr, GameControllerOpen, "SDL_GameControllerOpen")
        LOADFUNC(sdllibptr, GameControllerPath, "SDL_GameControllerPath")
        LOADFUNC(sdllibptr, GameControllerPathForIndex, "SDL_GameControllerPathForIndex")
        LOADFUNC(sdllibptr, GameControllerRumble, "SDL_GameControllerRumble")
        LOADFUNC(sdllibptr, GameControllerRumbleTriggers, "SDL_GameControllerRumbleTriggers")
        LOADFUNC(sdllibptr, GameControllerSendEffect, "SDL_GameControllerSendEffect")
        LOADFUNC(sdllibptr, GameControllerSetLED, "SDL_GameControllerSetLED")
        LOADFUNC(sdllibptr, GameControllerSetPlayerIndex, "SDL_GameControllerSetPlayerIndex")
        LOADFUNC(sdllibptr, GameControllerSetSensorEnabled, "SDL_GameControllerSetSensorEnabled")
        LOADFUNC(sdllibptr, GameControllerTypeForIndex, "SDL_GameControllerTypeForIndex")
        LOADFUNC(sdllibptr, GameControllerUpdate, "SDL_GameControllerUpdate")
        LOADFUNC(sdllibptr, GetAssertionHandler, "SDL_GetAssertionHandler")
        LOADFUNC(sdllibptr, GetAssertionReport, "SDL_GetAssertionReport")
        LOADFUNC(sdllibptr, GetAudioDeviceName, "SDL_GetAudioDeviceName")
        LOADFUNC(sdllibptr, GetAudioDeviceSpec, "SDL_GetAudioDeviceSpec")
        LOADFUNC(sdllibptr, GetAudioDeviceStatus, "SDL_GetAudioDeviceStatus")
        LOADFUNC(sdllibptr, GetAudioDriver, "SDL_GetAudioDriver")
        LOADFUNC(sdllibptr, GetAudioStatus, "SDL_GetAudioStatus")
        LOADFUNC(sdllibptr, GetBasePath, "SDL_GetBasePath")
        LOADFUNC(sdllibptr, GetClipboardText, "SDL_GetClipboardText")
        LOADFUNC(sdllibptr, GetClipRect, "SDL_GetClipRect")
        LOADFUNC(sdllibptr, GetClosestDisplayMode, "SDL_GetClosestDisplayMode")
        LOADFUNC(sdllibptr, GetColorKey, "SDL_GetColorKey")
        LOADFUNC(sdllibptr, GetCPUCacheLineSize, "SDL_GetCPUCacheLineSize")
        LOADFUNC(sdllibptr, GetCPUCount, "SDL_GetCPUCount")
        LOADFUNC(sdllibptr, GetCurrentAudioDriver, "SDL_GetCurrentAudioDriver")
        LOADFUNC(sdllibptr, GetCurrentDisplayMode, "SDL_GetCurrentDisplayMode")
        LOADFUNC(sdllibptr, GetCurrentVideoDriver, "SDL_GetCurrentVideoDriver")
        LOADFUNC(sdllibptr, GetCursor, "SDL_GetCursor")
        LOADFUNC(sdllibptr, GetDefaultAssertionHandler, "SDL_GetDefaultAssertionHandler")
        LOADFUNC(sdllibptr, GetDefaultAudioInfo, "SDL_GetDefaultAudioInfo")
        LOADFUNC(sdllibptr, GetDefaultCursor, "SDL_GetDefaultCursor")
        LOADFUNC(sdllibptr, GetDesktopDisplayMode, "SDL_GetDesktopDisplayMode")
        LOADFUNC(sdllibptr, GetDisplayBounds, "SDL_GetDisplayBounds")
        LOADFUNC(sdllibptr, GetDisplayDPI, "SDL_GetDisplayDPI")
        LOADFUNC(sdllibptr, GetDisplayMode, "SDL_GetDisplayMode")
        LOADFUNC(sdllibptr, GetDisplayName, "SDL_GetDisplayName")
        LOADFUNC(sdllibptr, GetDisplayOrientation, "SDL_GetDisplayOrientation")
        LOADFUNC(sdllibptr, GetDisplayUsableBounds, "SDL_GetDisplayUsableBounds")
        LOADFUNC(sdllibptr, GetError, "SDL_GetError")
        LOADFUNC(sdllibptr, GetErrorMsg, "SDL_GetErrorMsg")
        LOADFUNC(sdllibptr, GetEventFilter, "SDL_GetEventFilter")
        LOADFUNC(sdllibptr, GetGlobalMouseState, "SDL_GetGlobalMouseState")
        LOADFUNC(sdllibptr, GetGrabbedWindow, "SDL_GetGrabbedWindow")
        LOADFUNC(sdllibptr, GetHint, "SDL_GetHint")
        LOADFUNC(sdllibptr, GetHintBoolean, "SDL_GetHintBoolean")
        LOADFUNC(sdllibptr, GetJoystickGUIDInfo, "SDL_GetJoystickGUIDInfo")
        LOADFUNC(sdllibptr, GetKeyboardFocus, "SDL_GetKeyboardFocus")
        LOADFUNC(sdllibptr, GetKeyboardState, "SDL_GetKeyboardState")
        LOADFUNC(sdllibptr, GetKeyFromName, "SDL_GetKeyFromName")
        LOADFUNC(sdllibptr, GetKeyFromScancode, "SDL_GetKeyFromScancode")
        LOADFUNC(sdllibptr, GetKeyName, "SDL_GetKeyName")
        LOADFUNC(sdllibptr, GetMemoryFunctions, "SDL_GetMemoryFunctions")
        LOADFUNC(sdllibptr, GetModState, "SDL_GetModState")
        LOADFUNC(sdllibptr, GetMouseFocus, "SDL_GetMouseFocus")
        LOADFUNC(sdllibptr, GetMouseState, "SDL_GetMouseState")
        LOADFUNC(sdllibptr, GetNumAllocations, "SDL_GetNumAllocations")
        LOADFUNC(sdllibptr, GetNumAudioDevices, "SDL_GetNumAudioDevices")
        LOADFUNC(sdllibptr, GetNumAudioDrivers, "SDL_GetNumAudioDrivers")
        LOADFUNC(sdllibptr, GetNumDisplayModes, "SDL_GetNumDisplayModes")
        LOADFUNC(sdllibptr, GetNumRenderDrivers, "SDL_GetNumRenderDrivers")
        LOADFUNC(sdllibptr, GetNumTouchDevices, "SDL_GetNumTouchDevices")
        LOADFUNC(sdllibptr, GetNumTouchFingers, "SDL_GetNumTouchFingers")
        LOADFUNC(sdllibptr, GetNumVideoDisplays, "SDL_GetNumVideoDisplays")
        LOADFUNC(sdllibptr, GetNumVideoDrivers, "SDL_GetNumVideoDrivers")
        LOADFUNC(sdllibptr, GetOriginalMemoryFunctions, "SDL_GetOriginalMemoryFunctions")
        LOADFUNC(sdllibptr, GetPerformanceCounter, "SDL_GetPerformanceCounter")
        LOADFUNC(sdllibptr, GetPerformanceFrequency, "SDL_GetPerformanceFrequency")
        LOADFUNC(sdllibptr, GetPixelFormatName, "SDL_GetPixelFormatName")
        LOADFUNC(sdllibptr, GetPlatform, "SDL_GetPlatform")
        LOADFUNC(sdllibptr, GetPointDisplayIndex, "SDL_GetPointDisplayIndex")
        LOADFUNC(sdllibptr, GetPowerInfo, "SDL_GetPowerInfo")
        LOADFUNC(sdllibptr, GetPreferredLocales, "SDL_GetPreferredLocales")
        LOADFUNC(sdllibptr, GetPrefPath, "SDL_GetPrefPath")
        LOADFUNC(sdllibptr, GetPrimarySelectionText, "SDL_GetPrimarySelectionText")
        LOADFUNC(sdllibptr, GetQueuedAudioSize, "SDL_GetQueuedAudioSize")
        LOADFUNC(sdllibptr, GetRectDisplayIndex, "SDL_GetRectDisplayIndex")
        LOADFUNC(sdllibptr, GetRelativeMouseMode, "SDL_GetRelativeMouseMode")
        LOADFUNC(sdllibptr, GetRelativeMouseState, "SDL_GetRelativeMouseState")
        LOADFUNC(sdllibptr, GetRenderDrawBlendMode, "SDL_GetRenderDrawBlendMode")
        LOADFUNC(sdllibptr, GetRenderDrawColor, "SDL_GetRenderDrawColor")
        LOADFUNC(sdllibptr, GetRenderDriverInfo, "SDL_GetRenderDriverInfo")
        LOADFUNC(sdllibptr, GetRenderer, "SDL_GetRenderer")
        LOADFUNC(sdllibptr, GetRendererInfo, "SDL_GetRendererInfo")
        LOADFUNC(sdllibptr, GetRendererOutputSize, "SDL_GetRendererOutputSize")
        LOADFUNC(sdllibptr, GetRenderTarget, "SDL_GetRenderTarget")
        LOADFUNC(sdllibptr, GetRevision, "SDL_GetRevision")
        LOADFUNC(sdllibptr, GetRevisionNumber, "SDL_GetRevisionNumber")
        LOADFUNC(sdllibptr, GetRGB, "SDL_GetRGB")
        LOADFUNC(sdllibptr, GetRGBA, "SDL_GetRGBA")
        LOADFUNC(sdllibptr, GetScancodeFromKey, "SDL_GetScancodeFromKey")
        LOADFUNC(sdllibptr, GetScancodeFromName, "SDL_GetScancodeFromName")
        LOADFUNC(sdllibptr, GetScancodeName, "SDL_GetScancodeName")
        LOADFUNC(sdllibptr, GetShapedWindowMode, "SDL_GetShapedWindowMode")
        LOADFUNC(sdllibptr, GetSurfaceAlphaMod, "SDL_GetSurfaceAlphaMod")
        LOADFUNC(sdllibptr, GetSurfaceBlendMode, "SDL_GetSurfaceBlendMode")
        LOADFUNC(sdllibptr, GetSurfaceColorMod, "SDL_GetSurfaceColorMod")
        LOADFUNC(sdllibptr, GetSystemRAM, "SDL_GetSystemRAM")
        LOADFUNC(sdllibptr, GetTextureAlphaMod, "SDL_GetTextureAlphaMod")
        LOADFUNC(sdllibptr, GetTextureBlendMode, "SDL_GetTextureBlendMode")
        LOADFUNC(sdllibptr, GetTextureColorMod, "SDL_GetTextureColorMod")
        LOADFUNC(sdllibptr, GetTextureScaleMode, "SDL_GetTextureScaleMode")
        LOADFUNC(sdllibptr, GetTextureUserData, "SDL_GetTextureUserData")
        LOADFUNC(sdllibptr, GetThreadID, "SDL_GetThreadID")
        LOADFUNC(sdllibptr, GetThreadName, "SDL_GetThreadName")
        LOADFUNC(sdllibptr, GetTicks, "SDL_GetTicks")
        LOADFUNC(sdllibptr, GetTicks64, "SDL_GetTicks64")
        LOADFUNC(sdllibptr, GetTouchDevice, "SDL_GetTouchDevice")
        LOADFUNC(sdllibptr, GetTouchDeviceType, "SDL_GetTouchDeviceType")
        LOADFUNC(sdllibptr, GetTouchFinger, "SDL_GetTouchFinger")
        LOADFUNC(sdllibptr, GetTouchName, "SDL_GetTouchName")
        LOADFUNC(sdllibptr, GetVersion, "SDL_GetVersion")
        LOADFUNC(sdllibptr, GetVideoDriver, "SDL_GetVideoDriver")
        LOADFUNC(sdllibptr, GetWindowBordersSize, "SDL_GetWindowBordersSize")
        LOADFUNC(sdllibptr, GetWindowBrightness, "SDL_GetWindowBrightness")
        LOADFUNC(sdllibptr, GetWindowData, "SDL_GetWindowData")
        LOADFUNC(sdllibptr, GetWindowDisplayIndex, "SDL_GetWindowDisplayIndex")
        LOADFUNC(sdllibptr, GetWindowDisplayMode, "SDL_GetWindowDisplayMode")
        LOADFUNC(sdllibptr, GetWindowFlags, "SDL_GetWindowFlags")
        LOADFUNC(sdllibptr, GetWindowFromID, "SDL_GetWindowFromID")
        LOADFUNC(sdllibptr, GetWindowGammaRamp, "SDL_GetWindowGammaRamp")
        LOADFUNC(sdllibptr, GetWindowGrab, "SDL_GetWindowGrab")
        LOADFUNC(sdllibptr, GetWindowICCProfile, "SDL_GetWindowICCProfile")
        LOADFUNC(sdllibptr, GetWindowID, "SDL_GetWindowID")
        LOADFUNC(sdllibptr, GetWindowKeyboardGrab, "SDL_GetWindowKeyboardGrab")
        LOADFUNC(sdllibptr, GetWindowMaximumSize, "SDL_GetWindowMaximumSize")
        LOADFUNC(sdllibptr, GetWindowMinimumSize, "SDL_GetWindowMinimumSize")
        LOADFUNC(sdllibptr, GetWindowMouseGrab, "SDL_GetWindowMouseGrab")
        LOADFUNC(sdllibptr, GetWindowMouseRect, "SDL_GetWindowMouseRect")
        LOADFUNC(sdllibptr, GetWindowOpacity, "SDL_GetWindowOpacity")
        LOADFUNC(sdllibptr, GetWindowPixelFormat, "SDL_GetWindowPixelFormat")
        LOADFUNC(sdllibptr, GetWindowPosition, "SDL_GetWindowPosition")
        LOADFUNC(sdllibptr, GetWindowSize, "SDL_GetWindowSize")
        LOADFUNC(sdllibptr, GetWindowSizeInPixels, "SDL_GetWindowSizeInPixels")
        LOADFUNC(sdllibptr, GetWindowSurface, "SDL_GetWindowSurface")
        LOADFUNC(sdllibptr, GetWindowTitle, "SDL_GetWindowTitle")
        LOADFUNC(sdllibptr, GetYUVConversionMode, "SDL_GetYUVConversionMode")
        LOADFUNC(sdllibptr, GetYUVConversionModeForResolution, "SDL_GetYUVConversionModeForResolution")
        LOADFUNC(sdllibptr, GL_BindTexture, "SDL_GL_BindTexture")
        LOADFUNC(sdllibptr, GL_CreateContext, "SDL_GL_CreateContext")
        LOADFUNC(sdllibptr, GL_DeleteContext, "SDL_GL_DeleteContext")
        LOADFUNC(sdllibptr, GL_ExtensionSupported, "SDL_GL_ExtensionSupported")
        LOADFUNC(sdllibptr, GL_GetAttribute, "SDL_GL_GetAttribute")
        LOADFUNC(sdllibptr, GL_GetCurrentContext, "SDL_GL_GetCurrentContext")
        LOADFUNC(sdllibptr, GL_GetCurrentWindow, "SDL_GL_GetCurrentWindow")
        LOADFUNC(sdllibptr, GL_GetDrawableSize, "SDL_GL_GetDrawableSize")
        LOADFUNC(sdllibptr, GL_GetProcAddress, "SDL_GL_GetProcAddress")
        LOADFUNC(sdllibptr, GL_GetSwapInterval, "SDL_GL_GetSwapInterval")
        LOADFUNC(sdllibptr, GL_LoadLibrary, "SDL_GL_LoadLibrary")
        LOADFUNC(sdllibptr, GL_MakeCurrent, "SDL_GL_MakeCurrent")
        LOADFUNC(sdllibptr, GL_ResetAttributes, "SDL_GL_ResetAttributes")
        LOADFUNC(sdllibptr, GL_SetAttribute, "SDL_GL_SetAttribute")
        LOADFUNC(sdllibptr, GL_SetSwapInterval, "SDL_GL_SetSwapInterval")
        LOADFUNC(sdllibptr, GL_SwapWindow, "SDL_GL_SwapWindow")
        LOADFUNC(sdllibptr, GL_UnbindTexture, "SDL_GL_UnbindTexture")
        LOADFUNC(sdllibptr, GL_UnloadLibrary, "SDL_GL_UnloadLibrary")
        LOADFUNC(sdllibptr, GUIDFromString, "SDL_GUIDFromString")
        LOADFUNC(sdllibptr, GUIDToString, "SDL_GUIDToString")
        LOADFUNC(sdllibptr, HapticClose, "SDL_HapticClose")
        LOADFUNC(sdllibptr, HapticDestroyEffect, "SDL_HapticDestroyEffect")
        LOADFUNC(sdllibptr, HapticEffectSupported, "SDL_HapticEffectSupported")
        LOADFUNC(sdllibptr, HapticGetEffectStatus, "SDL_HapticGetEffectStatus")
        LOADFUNC(sdllibptr, HapticIndex, "SDL_HapticIndex")
        LOADFUNC(sdllibptr, HapticName, "SDL_HapticName")
        LOADFUNC(sdllibptr, HapticNewEffect, "SDL_HapticNewEffect")
        LOADFUNC(sdllibptr, HapticNumAxes, "SDL_HapticNumAxes")
        LOADFUNC(sdllibptr, HapticNumEffects, "SDL_HapticNumEffects")
        LOADFUNC(sdllibptr, HapticNumEffectsPlaying, "SDL_HapticNumEffectsPlaying")
        LOADFUNC(sdllibptr, HapticOpen, "SDL_HapticOpen")
        LOADFUNC(sdllibptr, HapticOpened, "SDL_HapticOpened")
        LOADFUNC(sdllibptr, HapticOpenFromJoystick, "SDL_HapticOpenFromJoystick")
        LOADFUNC(sdllibptr, HapticOpenFromMouse, "SDL_HapticOpenFromMouse")
        LOADFUNC(sdllibptr, HapticPause, "SDL_HapticPause")
        LOADFUNC(sdllibptr, HapticQuery, "SDL_HapticQuery")
        LOADFUNC(sdllibptr, HapticRumbleInit, "SDL_HapticRumbleInit")
        LOADFUNC(sdllibptr, HapticRumblePlay, "SDL_HapticRumblePlay")
        LOADFUNC(sdllibptr, HapticRumbleStop, "SDL_HapticRumbleStop")
        LOADFUNC(sdllibptr, HapticRumbleSupported, "SDL_HapticRumbleSupported")
        LOADFUNC(sdllibptr, HapticRunEffect, "SDL_HapticRunEffect")
        LOADFUNC(sdllibptr, HapticSetAutocenter, "SDL_HapticSetAutocenter")
        LOADFUNC(sdllibptr, HapticSetGain, "SDL_HapticSetGain")
        LOADFUNC(sdllibptr, HapticStopAll, "SDL_HapticStopAll")
        LOADFUNC(sdllibptr, HapticStopEffect, "SDL_HapticStopEffect")
        LOADFUNC(sdllibptr, HapticUnpause, "SDL_HapticUnpause")
        LOADFUNC(sdllibptr, HapticUpdateEffect, "SDL_HapticUpdateEffect")
        LOADFUNC(sdllibptr, Has3DNow, "SDL_Has3DNow")
        LOADFUNC(sdllibptr, HasAltiVec, "SDL_HasAltiVec")
        LOADFUNC(sdllibptr, HasARMSIMD, "SDL_HasARMSIMD")
        LOADFUNC(sdllibptr, HasAVX, "SDL_HasAVX")
        LOADFUNC(sdllibptr, HasAVX2, "SDL_HasAVX2")
        LOADFUNC(sdllibptr, HasAVX512F, "SDL_HasAVX512F")
        LOADFUNC(sdllibptr, HasClipboardText, "SDL_HasClipboardText")
        LOADFUNC(sdllibptr, HasColorKey, "SDL_HasColorKey")
        LOADFUNC(sdllibptr, HasEvent, "SDL_HasEvent")
        LOADFUNC(sdllibptr, HasEvents, "SDL_HasEvents")
        LOADFUNC(sdllibptr, HasIntersection, "SDL_HasIntersection")
        LOADFUNC(sdllibptr, HasIntersectionF, "SDL_HasIntersectionF")
        LOADFUNC(sdllibptr, HasLASX, "SDL_HasLASX")
        LOADFUNC(sdllibptr, HasLSX, "SDL_HasLSX")
        LOADFUNC(sdllibptr, HasMMX, "SDL_HasMMX")
        LOADFUNC(sdllibptr, HasNEON, "SDL_HasNEON")
        LOADFUNC(sdllibptr, HasPrimarySelectionText, "SDL_HasPrimarySelectionText")
        LOADFUNC(sdllibptr, HasRDTSC, "SDL_HasRDTSC")
        LOADFUNC(sdllibptr, HasScreenKeyboardSupport, "SDL_HasScreenKeyboardSupport")
        LOADFUNC(sdllibptr, HasSSE, "SDL_HasSSE")
        LOADFUNC(sdllibptr, HasSSE2, "SDL_HasSSE2")
        LOADFUNC(sdllibptr, HasSSE3, "SDL_HasSSE3")
        LOADFUNC(sdllibptr, HasSSE41, "SDL_HasSSE41")
        LOADFUNC(sdllibptr, HasSSE42, "SDL_HasSSE42")
        LOADFUNC(sdllibptr, HasSurfaceRLE, "SDL_HasSurfaceRLE")
        LOADFUNC(sdllibptr, HasWindowSurface, "SDL_HasWindowSurface")
        LOADFUNC(sdllibptr, hid_ble_scan, "SDL_hid_ble_scan")
        LOADFUNC(sdllibptr, hid_close, "SDL_hid_close")
        LOADFUNC(sdllibptr, hid_device_change_count, "SDL_hid_device_change_count")
        LOADFUNC(sdllibptr, hid_enumerate, "SDL_hid_enumerate")
        LOADFUNC(sdllibptr, hid_exit, "SDL_hid_exit")
        LOADFUNC(sdllibptr, hid_free_enumeration, "SDL_hid_free_enumeration")
        LOADFUNC(sdllibptr, hid_get_feature_report, "SDL_hid_get_feature_report")
        LOADFUNC(sdllibptr, hid_get_indexed_string, "SDL_hid_get_indexed_string")
        LOADFUNC(sdllibptr, hid_get_manufacturer_string, "SDL_hid_get_manufacturer_string")
        LOADFUNC(sdllibptr, hid_get_product_string, "SDL_hid_get_product_string")
        LOADFUNC(sdllibptr, hid_get_serial_number_string, "SDL_hid_get_serial_number_string")
        LOADFUNC(sdllibptr, hid_init, "SDL_hid_init")
        LOADFUNC(sdllibptr, hid_open, "SDL_hid_open")
        LOADFUNC(sdllibptr, hid_open_path, "SDL_hid_open_path")
        LOADFUNC(sdllibptr, hid_read, "SDL_hid_read")
        LOADFUNC(sdllibptr, hid_read_timeout, "SDL_hid_read_timeout")
        LOADFUNC(sdllibptr, hid_send_feature_report, "SDL_hid_send_feature_report")
        LOADFUNC(sdllibptr, hid_set_nonblocking, "SDL_hid_set_nonblocking")
        LOADFUNC(sdllibptr, hid_write, "SDL_hid_write")
        LOADFUNC(sdllibptr, HideWindow, "SDL_HideWindow")
        LOADFUNC(sdllibptr, iconv_string, "SDL_iconv_string")
        LOADFUNC(sdllibptr, Init, "SDL_Init")
        LOADFUNC(sdllibptr, InitSubSystem, "SDL_InitSubSystem")
        LOADFUNC(sdllibptr, IntersectFRect, "SDL_IntersectFRect")
        LOADFUNC(sdllibptr, IntersectFRectAndLine, "SDL_IntersectFRectAndLine")
        LOADFUNC(sdllibptr, IntersectRect, "SDL_IntersectRect")
        LOADFUNC(sdllibptr, IntersectRectAndLine, "SDL_IntersectRectAndLine")
        LOADFUNC(sdllibptr, IsGameController, "SDL_IsGameController")
        LOADFUNC(sdllibptr, IsScreenKeyboardShown, "SDL_IsScreenKeyboardShown")
        LOADFUNC(sdllibptr, IsScreenSaverEnabled, "SDL_IsScreenSaverEnabled")
        LOADFUNC(sdllibptr, IsShapedWindow, "SDL_IsShapedWindow")
        LOADFUNC(sdllibptr, IsTablet, "SDL_IsTablet")
        LOADFUNC(sdllibptr, IsTextInputActive, "SDL_IsTextInputActive")
        LOADFUNC(sdllibptr, IsTextInputShown, "SDL_IsTextInputShown")
        LOADFUNC(sdllibptr, JoystickAttachVirtual, "SDL_JoystickAttachVirtual")
        LOADFUNC(sdllibptr, JoystickAttachVirtualEx, "SDL_JoystickAttachVirtualEx")
        LOADFUNC(sdllibptr, JoystickClose, "SDL_JoystickClose")
        LOADFUNC(sdllibptr, JoystickCurrentPowerLevel, "SDL_JoystickCurrentPowerLevel")
        LOADFUNC(sdllibptr, JoystickDetachVirtual, "SDL_JoystickDetachVirtual")
        LOADFUNC(sdllibptr, JoystickEventState, "SDL_JoystickEventState")
        LOADFUNC(sdllibptr, JoystickFromInstanceID, "SDL_JoystickFromInstanceID")
        LOADFUNC(sdllibptr, JoystickFromPlayerIndex, "SDL_JoystickFromPlayerIndex")
        LOADFUNC(sdllibptr, JoystickGetAttached, "SDL_JoystickGetAttached")
        LOADFUNC(sdllibptr, JoystickGetAxis, "SDL_JoystickGetAxis")
        LOADFUNC(sdllibptr, JoystickGetAxisInitialState, "SDL_JoystickGetAxisInitialState")
        LOADFUNC(sdllibptr, JoystickGetBall, "SDL_JoystickGetBall")
        LOADFUNC(sdllibptr, JoystickGetButton, "SDL_JoystickGetButton")
        LOADFUNC(sdllibptr, JoystickGetDeviceGUID, "SDL_JoystickGetDeviceGUID")
        LOADFUNC(sdllibptr, JoystickGetDeviceInstanceID, "SDL_JoystickGetDeviceInstanceID")
        LOADFUNC(sdllibptr, JoystickGetDevicePlayerIndex, "SDL_JoystickGetDevicePlayerIndex")
        LOADFUNC(sdllibptr, JoystickGetDeviceProduct, "SDL_JoystickGetDeviceProduct")
        LOADFUNC(sdllibptr, JoystickGetDeviceProductVersion, "SDL_JoystickGetDeviceProductVersion")
        LOADFUNC(sdllibptr, JoystickGetDeviceType, "SDL_JoystickGetDeviceType")
        LOADFUNC(sdllibptr, JoystickGetDeviceVendor, "SDL_JoystickGetDeviceVendor")
        LOADFUNC(sdllibptr, JoystickGetFirmwareVersion, "SDL_JoystickGetFirmwareVersion")
        LOADFUNC(sdllibptr, JoystickGetGUID, "SDL_JoystickGetGUID")
        LOADFUNC(sdllibptr, JoystickGetGUIDFromString, "SDL_JoystickGetGUIDFromString")
        LOADFUNC(sdllibptr, JoystickGetGUIDString, "SDL_JoystickGetGUIDString")
        LOADFUNC(sdllibptr, JoystickGetHat, "SDL_JoystickGetHat")
        LOADFUNC(sdllibptr, JoystickGetPlayerIndex, "SDL_JoystickGetPlayerIndex")
        LOADFUNC(sdllibptr, JoystickGetProduct, "SDL_JoystickGetProduct")
        LOADFUNC(sdllibptr, JoystickGetProductVersion, "SDL_JoystickGetProductVersion")
        LOADFUNC(sdllibptr, JoystickGetSerial, "SDL_JoystickGetSerial")
        LOADFUNC(sdllibptr, JoystickGetType, "SDL_JoystickGetType")
        LOADFUNC(sdllibptr, JoystickGetVendor, "SDL_JoystickGetVendor")
        LOADFUNC(sdllibptr, JoystickHasLED, "SDL_JoystickHasLED")
        LOADFUNC(sdllibptr, JoystickHasRumble, "SDL_JoystickHasRumble")
        LOADFUNC(sdllibptr, JoystickHasRumbleTriggers, "SDL_JoystickHasRumbleTriggers")
        LOADFUNC(sdllibptr, JoystickInstanceID, "SDL_JoystickInstanceID")
        LOADFUNC(sdllibptr, JoystickIsHaptic, "SDL_JoystickIsHaptic")
        LOADFUNC(sdllibptr, JoystickIsVirtual, "SDL_JoystickIsVirtual")
        LOADFUNC(sdllibptr, JoystickName, "SDL_JoystickName")
        LOADFUNC(sdllibptr, JoystickNameForIndex, "SDL_JoystickNameForIndex")
        LOADFUNC(sdllibptr, JoystickNumAxes, "SDL_JoystickNumAxes")
        LOADFUNC(sdllibptr, JoystickNumBalls, "SDL_JoystickNumBalls")
        LOADFUNC(sdllibptr, JoystickNumButtons, "SDL_JoystickNumButtons")
        LOADFUNC(sdllibptr, JoystickNumHats, "SDL_JoystickNumHats")
        LOADFUNC(sdllibptr, JoystickOpen, "SDL_JoystickOpen")
        LOADFUNC(sdllibptr, JoystickPath, "SDL_JoystickPath")
        LOADFUNC(sdllibptr, JoystickPathForIndex, "SDL_JoystickPathForIndex")
        LOADFUNC(sdllibptr, JoystickRumble, "SDL_JoystickRumble")
        LOADFUNC(sdllibptr, JoystickRumbleTriggers, "SDL_JoystickRumbleTriggers")
        LOADFUNC(sdllibptr, JoystickSendEffect, "SDL_JoystickSendEffect")
        LOADFUNC(sdllibptr, JoystickSetLED, "SDL_JoystickSetLED")
        LOADFUNC(sdllibptr, JoystickSetPlayerIndex, "SDL_JoystickSetPlayerIndex")
        LOADFUNC(sdllibptr, JoystickSetVirtualAxis, "SDL_JoystickSetVirtualAxis")
        LOADFUNC(sdllibptr, JoystickSetVirtualButton, "SDL_JoystickSetVirtualButton")
        LOADFUNC(sdllibptr, JoystickSetVirtualHat, "SDL_JoystickSetVirtualHat")
        LOADFUNC(sdllibptr, JoystickUpdate, "SDL_JoystickUpdate")
        if (IsLinux()) {
            LOADFUNC(sdllibptr, LinuxSetThreadPriority, "SDL_LinuxSetThreadPriority")
            LOADFUNC(sdllibptr, LinuxSetThreadPriorityAndPolicy, "SDL_LinuxSetThreadPriorityAndPolicy")
        }
        LOADFUNC(sdllibptr, LoadBMP_RW, "SDL_LoadBMP_RW")
        LOADFUNC(sdllibptr, LoadDollarTemplates, "SDL_LoadDollarTemplates")
        LOADFUNC(sdllibptr, LoadFile, "SDL_LoadFile")
        LOADFUNC(sdllibptr, LoadFile_RW, "SDL_LoadFile_RW")
        LOADFUNC(sdllibptr, LoadFunction, "SDL_LoadFunction")
        LOADFUNC(sdllibptr, LoadObject, "SDL_LoadObject")
        LOADFUNC(sdllibptr, LoadWAV_RW, "SDL_LoadWAV_RW")
        LOADFUNC(sdllibptr, LockAudio, "SDL_LockAudio")
        LOADFUNC(sdllibptr, LockAudioDevice, "SDL_LockAudioDevice")
        LOADFUNC(sdllibptr, LockJoysticks, "SDL_LockJoysticks")
        LOADFUNC(sdllibptr, LockMutex, "SDL_LockMutex")
        LOADFUNC(sdllibptr, LockSensors, "SDL_LockSensors")
        LOADFUNC(sdllibptr, LockSurface, "SDL_LockSurface")
        LOADFUNC(sdllibptr, LockTexture, "SDL_LockTexture")
        LOADFUNC(sdllibptr, LockTextureToSurface, "SDL_LockTextureToSurface")
        LOADFUNC(sdllibptr, Log, "SDL_Log")
        LOADFUNC(sdllibptr, LogCritical, "SDL_LogCritical")
        LOADFUNC(sdllibptr, LogDebug, "SDL_LogDebug")
        LOADFUNC(sdllibptr, LogGetOutputFunction, "SDL_LogGetOutputFunction")
        LOADFUNC(sdllibptr, LogGetPriority, "SDL_LogGetPriority")
        LOADFUNC(sdllibptr, LogInfo, "SDL_LogInfo")
        LOADFUNC(sdllibptr, LogMessage, "SDL_LogMessage")
        LOADFUNC(sdllibptr, LogResetPriorities, "SDL_LogResetPriorities")
        LOADFUNC(sdllibptr, LogSetAllPriority, "SDL_LogSetAllPriority")
        LOADFUNC(sdllibptr, LogSetOutputFunction, "SDL_LogSetOutputFunction")
        LOADFUNC(sdllibptr, LogSetPriority, "SDL_LogSetPriority")
        LOADFUNC(sdllibptr, LogVerbose, "SDL_LogVerbose")
        LOADFUNC(sdllibptr, LogWarn, "SDL_LogWarn")
        LOADFUNC(sdllibptr, LowerBlit, "SDL_LowerBlit")
        LOADFUNC(sdllibptr, LowerBlitScaled, "SDL_LowerBlitScaled")
        LOADFUNC(sdllibptr, MapRGB, "SDL_MapRGB")
        LOADFUNC(sdllibptr, MapRGBA, "SDL_MapRGBA")
        LOADFUNC(sdllibptr, MasksToPixelFormatEnum, "SDL_MasksToPixelFormatEnum")
        LOADFUNC(sdllibptr, MaximizeWindow, "SDL_MaximizeWindow")
        LOADFUNC(sdllibptr, MemoryBarrierReleaseFunction, "SDL_MemoryBarrierReleaseFunction")
        LOADFUNC(sdllibptr, Metal_CreateView, "SDL_Metal_CreateView")
        LOADFUNC(sdllibptr, Metal_DestroyView, "SDL_Metal_DestroyView")
        LOADFUNC(sdllibptr, Metal_GetDrawableSize, "SDL_Metal_GetDrawableSize")
        LOADFUNC(sdllibptr, Metal_GetLayer, "SDL_Metal_GetLayer")
        LOADFUNC(sdllibptr, MinimizeWindow, "SDL_MinimizeWindow")
        LOADFUNC(sdllibptr, MixAudio, "SDL_MixAudio")
        LOADFUNC(sdllibptr, MixAudioFormat, "SDL_MixAudioFormat")
        LOADFUNC(sdllibptr, MouseIsHaptic, "SDL_MouseIsHaptic")
        LOADFUNC(sdllibptr, NewAudioStream, "SDL_NewAudioStream")
        LOADFUNC(sdllibptr, NumHaptics, "SDL_NumHaptics")
        LOADFUNC(sdllibptr, NumJoysticks, "SDL_NumJoysticks")
        LOADFUNC(sdllibptr, NumSensors, "SDL_NumSensors")
        LOADFUNC(sdllibptr, OpenAudio, "SDL_OpenAudio")
        LOADFUNC(sdllibptr, OpenAudioDevice, "SDL_OpenAudioDevice")
        LOADFUNC(sdllibptr, OpenURL, "SDL_OpenURL")
        LOADFUNC(sdllibptr, PauseAudio, "SDL_PauseAudio")
        LOADFUNC(sdllibptr, PauseAudioDevice, "SDL_PauseAudioDevice")
        LOADFUNC(sdllibptr, PeepEvents, "SDL_PeepEvents")
        LOADFUNC(sdllibptr, PixelFormatEnumToMasks, "SDL_PixelFormatEnumToMasks")
        LOADFUNC(sdllibptr, PollEvent, "SDL_PollEvent")
        LOADFUNC(sdllibptr, PremultiplyAlpha, "SDL_PremultiplyAlpha")
        LOADFUNC(sdllibptr, PumpEvents, "SDL_PumpEvents")
        LOADFUNC(sdllibptr, PushEvent, "SDL_PushEvent")
        LOADFUNC(sdllibptr, QueryTexture, "SDL_QueryTexture")
        LOADFUNC(sdllibptr, QueueAudio, "SDL_QueueAudio")
        LOADFUNC(sdllibptr, Quit, "SDL_Quit")
        LOADFUNC(sdllibptr, QuitSubSystem, "SDL_QuitSubSystem")
        LOADFUNC(sdllibptr, RaiseWindow, "SDL_RaiseWindow")
        LOADFUNC(sdllibptr, ReadBE16, "SDL_ReadBE16")
        LOADFUNC(sdllibptr, ReadBE32, "SDL_ReadBE32")
        LOADFUNC(sdllibptr, ReadBE64, "SDL_ReadBE64")
        LOADFUNC(sdllibptr, ReadLE16, "SDL_ReadLE16")
        LOADFUNC(sdllibptr, ReadLE32, "SDL_ReadLE32")
        LOADFUNC(sdllibptr, ReadLE64, "SDL_ReadLE64")
        LOADFUNC(sdllibptr, ReadU8, "SDL_ReadU8")
        LOADFUNC(sdllibptr, RecordGesture, "SDL_RecordGesture")
        LOADFUNC(sdllibptr, RegisterEvents, "SDL_RegisterEvents")
        LOADFUNC(sdllibptr, RemoveTimer, "SDL_RemoveTimer")
        LOADFUNC(sdllibptr, RenderClear, "SDL_RenderClear")
        LOADFUNC(sdllibptr, RenderCopy, "SDL_RenderCopy")
        LOADFUNC(sdllibptr, RenderCopyEx, "SDL_RenderCopyEx")
        LOADFUNC(sdllibptr, RenderCopyExF, "SDL_RenderCopyExF")
        LOADFUNC(sdllibptr, RenderCopyF, "SDL_RenderCopyF")
        LOADFUNC(sdllibptr, RenderDrawLine, "SDL_RenderDrawLine")
        LOADFUNC(sdllibptr, RenderDrawLineF, "SDL_RenderDrawLineF")
        LOADFUNC(sdllibptr, RenderDrawLines, "SDL_RenderDrawLines")
        LOADFUNC(sdllibptr, RenderDrawLinesF, "SDL_RenderDrawLinesF")
        LOADFUNC(sdllibptr, RenderDrawPoint, "SDL_RenderDrawPoint")
        LOADFUNC(sdllibptr, RenderDrawPointF, "SDL_RenderDrawPointF")
        LOADFUNC(sdllibptr, RenderDrawPoints, "SDL_RenderDrawPoints")
        LOADFUNC(sdllibptr, RenderDrawPointsF, "SDL_RenderDrawPointsF")
        LOADFUNC(sdllibptr, RenderDrawRect, "SDL_RenderDrawRect")
        LOADFUNC(sdllibptr, RenderDrawRectF, "SDL_RenderDrawRectF")
        LOADFUNC(sdllibptr, RenderDrawRects, "SDL_RenderDrawRects")
        LOADFUNC(sdllibptr, RenderDrawRectsF, "SDL_RenderDrawRectsF")
        LOADFUNC(sdllibptr, RenderFillRect, "SDL_RenderFillRect")
        LOADFUNC(sdllibptr, RenderFillRectF, "SDL_RenderFillRectF")
        LOADFUNC(sdllibptr, RenderFillRects, "SDL_RenderFillRects")
        LOADFUNC(sdllibptr, RenderFillRectsF, "SDL_RenderFillRectsF")
        LOADFUNC(sdllibptr, RenderFlush, "SDL_RenderFlush")
        LOADFUNC(sdllibptr, RenderGeometry, "SDL_RenderGeometry")
        LOADFUNC(sdllibptr, RenderGeometryRaw, "SDL_RenderGeometryRaw")
        LOADFUNC(sdllibptr, RenderGetClipRect, "SDL_RenderGetClipRect")
        LOADFUNC(sdllibptr, RenderGetIntegerScale, "SDL_RenderGetIntegerScale")
        LOADFUNC(sdllibptr, RenderGetLogicalSize, "SDL_RenderGetLogicalSize")
        LOADFUNC(sdllibptr, RenderGetMetalCommandEncoder, "SDL_RenderGetMetalCommandEncoder")
        LOADFUNC(sdllibptr, RenderGetMetalLayer, "SDL_RenderGetMetalLayer")
        LOADFUNC(sdllibptr, RenderGetScale, "SDL_RenderGetScale")
        LOADFUNC(sdllibptr, RenderGetViewport, "SDL_RenderGetViewport")
        LOADFUNC(sdllibptr, RenderGetWindow, "SDL_RenderGetWindow")
        LOADFUNC(sdllibptr, RenderIsClipEnabled, "SDL_RenderIsClipEnabled")
        LOADFUNC(sdllibptr, RenderLogicalToWindow, "SDL_RenderLogicalToWindow")
        LOADFUNC(sdllibptr, RenderPresent, "SDL_RenderPresent")
        LOADFUNC(sdllibptr, RenderReadPixels, "SDL_RenderReadPixels")
        LOADFUNC(sdllibptr, RenderSetClipRect, "SDL_RenderSetClipRect")
        LOADFUNC(sdllibptr, RenderSetIntegerScale, "SDL_RenderSetIntegerScale")
        LOADFUNC(sdllibptr, RenderSetLogicalSize, "SDL_RenderSetLogicalSize")
        LOADFUNC(sdllibptr, RenderSetScale, "SDL_RenderSetScale")
        LOADFUNC(sdllibptr, RenderSetViewport, "SDL_RenderSetViewport")
        LOADFUNC(sdllibptr, RenderSetVSync, "SDL_RenderSetVSync")
        LOADFUNC(sdllibptr, RenderTargetSupported, "SDL_RenderTargetSupported")
        LOADFUNC(sdllibptr, RenderWindowToLogical, "SDL_RenderWindowToLogical")
        LOADFUNC(sdllibptr, ResetAssertionReport, "SDL_ResetAssertionReport")
        LOADFUNC(sdllibptr, ResetHint, "SDL_ResetHint")
        LOADFUNC(sdllibptr, ResetHints, "SDL_ResetHints")
        LOADFUNC(sdllibptr, ResetKeyboard, "SDL_ResetKeyboard")
        LOADFUNC(sdllibptr, RestoreWindow, "SDL_RestoreWindow")
        LOADFUNC(sdllibptr, RWclose, "SDL_RWclose")
        LOADFUNC(sdllibptr, RWFromConstMem, "SDL_RWFromConstMem")
        LOADFUNC(sdllibptr, RWFromFile, "SDL_RWFromFile")
        LOADFUNC(sdllibptr, RWFromFP, "SDL_RWFromFP")
        LOADFUNC(sdllibptr, RWFromMem, "SDL_RWFromMem")
        LOADFUNC(sdllibptr, RWread, "SDL_RWread")
        LOADFUNC(sdllibptr, RWseek, "SDL_RWseek")
        LOADFUNC(sdllibptr, RWsize, "SDL_RWsize")
        LOADFUNC(sdllibptr, RWtell, "SDL_RWtell")
        LOADFUNC(sdllibptr, RWwrite, "SDL_RWwrite")
        LOADFUNC(sdllibptr, SaveAllDollarTemplates, "SDL_SaveAllDollarTemplates")
        LOADFUNC(sdllibptr, SaveBMP_RW, "SDL_SaveBMP_RW")
        LOADFUNC(sdllibptr, SaveDollarTemplate, "SDL_SaveDollarTemplate")
        LOADFUNC(sdllibptr, SemPost, "SDL_SemPost")
        LOADFUNC(sdllibptr, SemTryWait, "SDL_SemTryWait")
        LOADFUNC(sdllibptr, SemValue, "SDL_SemValue")
        LOADFUNC(sdllibptr, SemWait, "SDL_SemWait")
        LOADFUNC(sdllibptr, SemWaitTimeout, "SDL_SemWaitTimeout")
        LOADFUNC(sdllibptr, SensorClose, "SDL_SensorClose")
        LOADFUNC(sdllibptr, SensorFromInstanceID, "SDL_SensorFromInstanceID")
        LOADFUNC(sdllibptr, SensorGetData, "SDL_SensorGetData")
        LOADFUNC(sdllibptr, SensorGetDataWithTimestamp, "SDL_SensorGetDataWithTimestamp")
        LOADFUNC(sdllibptr, SensorGetDeviceInstanceID, "SDL_SensorGetDeviceInstanceID")
        LOADFUNC(sdllibptr, SensorGetDeviceName, "SDL_SensorGetDeviceName")
        LOADFUNC(sdllibptr, SensorGetDeviceNonPortableType, "SDL_SensorGetDeviceNonPortableType")
        LOADFUNC(sdllibptr, SensorGetDeviceType, "SDL_SensorGetDeviceType")
        LOADFUNC(sdllibptr, SensorGetInstanceID, "SDL_SensorGetInstanceID")
        LOADFUNC(sdllibptr, SensorGetName, "SDL_SensorGetName")
        LOADFUNC(sdllibptr, SensorGetNonPortableType, "SDL_SensorGetNonPortableType")
        LOADFUNC(sdllibptr, SensorGetType, "SDL_SensorGetType")
        LOADFUNC(sdllibptr, SensorOpen, "SDL_SensorOpen")
        LOADFUNC(sdllibptr, SensorUpdate, "SDL_SensorUpdate")
        LOADFUNC(sdllibptr, SetAssertionHandler, "SDL_SetAssertionHandler")
        LOADFUNC(sdllibptr, SetClipboardText, "SDL_SetClipboardText")
        LOADFUNC(sdllibptr, SetClipRect, "SDL_SetClipRect")
        LOADFUNC(sdllibptr, SetColorKey, "SDL_SetColorKey")
        LOADFUNC(sdllibptr, SetCursor, "SDL_SetCursor")
        LOADFUNC(sdllibptr, SetError, "SDL_SetError")
        LOADFUNC(sdllibptr, SetEventFilter, "SDL_SetEventFilter")
        LOADFUNC(sdllibptr, SetHint, "SDL_SetHint")
        LOADFUNC(sdllibptr, SetHintWithPriority, "SDL_SetHintWithPriority")
        LOADFUNC(sdllibptr, SetMainReady, "SDL_SetMainReady")
        LOADFUNC(sdllibptr, SetMemoryFunctions, "SDL_SetMemoryFunctions")
        LOADFUNC(sdllibptr, SetModState, "SDL_SetModState")
        LOADFUNC(sdllibptr, SetPaletteColors, "SDL_SetPaletteColors")
        LOADFUNC(sdllibptr, SetPixelFormatPalette, "SDL_SetPixelFormatPalette")
        LOADFUNC(sdllibptr, SetPrimarySelectionText, "SDL_SetPrimarySelectionText")
        LOADFUNC(sdllibptr, SetRelativeMouseMode, "SDL_SetRelativeMouseMode")
        LOADFUNC(sdllibptr, SetRenderDrawBlendMode, "SDL_SetRenderDrawBlendMode")
        LOADFUNC(sdllibptr, SetRenderDrawColor, "SDL_SetRenderDrawColor")
        LOADFUNC(sdllibptr, SetRenderTarget, "SDL_SetRenderTarget")
        LOADFUNC(sdllibptr, SetSurfaceAlphaMod, "SDL_SetSurfaceAlphaMod")
        LOADFUNC(sdllibptr, SetSurfaceBlendMode, "SDL_SetSurfaceBlendMode")
        LOADFUNC(sdllibptr, SetSurfaceColorMod, "SDL_SetSurfaceColorMod")
        LOADFUNC(sdllibptr, SetSurfacePalette, "SDL_SetSurfacePalette")
        LOADFUNC(sdllibptr, SetSurfaceRLE, "SDL_SetSurfaceRLE")
        LOADFUNC(sdllibptr, SetTextInputRect, "SDL_SetTextInputRect")
        LOADFUNC(sdllibptr, SetTextureAlphaMod, "SDL_SetTextureAlphaMod")
        LOADFUNC(sdllibptr, SetTextureBlendMode, "SDL_SetTextureBlendMode")
        LOADFUNC(sdllibptr, SetTextureColorMod, "SDL_SetTextureColorMod")
        LOADFUNC(sdllibptr, SetTextureScaleMode, "SDL_SetTextureScaleMode")
        LOADFUNC(sdllibptr, SetTextureUserData, "SDL_SetTextureUserData")
        LOADFUNC(sdllibptr, SetThreadPriority, "SDL_SetThreadPriority")
        LOADFUNC(sdllibptr, SetWindowAlwaysOnTop, "SDL_SetWindowAlwaysOnTop")
        LOADFUNC(sdllibptr, SetWindowBordered, "SDL_SetWindowBordered")
        LOADFUNC(sdllibptr, SetWindowBrightness, "SDL_SetWindowBrightness")
        LOADFUNC(sdllibptr, SetWindowData, "SDL_SetWindowData")
        LOADFUNC(sdllibptr, SetWindowDisplayMode, "SDL_SetWindowDisplayMode")
        LOADFUNC(sdllibptr, SetWindowFullscreen, "SDL_SetWindowFullscreen")
        LOADFUNC(sdllibptr, SetWindowGammaRamp, "SDL_SetWindowGammaRamp")
        LOADFUNC(sdllibptr, SetWindowGrab, "SDL_SetWindowGrab")
        LOADFUNC(sdllibptr, SetWindowHitTest, "SDL_SetWindowHitTest")
        LOADFUNC(sdllibptr, SetWindowIcon, "SDL_SetWindowIcon")
        LOADFUNC(sdllibptr, SetWindowInputFocus, "SDL_SetWindowInputFocus")
        LOADFUNC(sdllibptr, SetWindowKeyboardGrab, "SDL_SetWindowKeyboardGrab")
        LOADFUNC(sdllibptr, SetWindowMaximumSize, "SDL_SetWindowMaximumSize")
        LOADFUNC(sdllibptr, SetWindowMinimumSize, "SDL_SetWindowMinimumSize")
        LOADFUNC(sdllibptr, SetWindowModalFor, "SDL_SetWindowModalFor")
        LOADFUNC(sdllibptr, SetWindowMouseGrab, "SDL_SetWindowMouseGrab")
        LOADFUNC(sdllibptr, SetWindowMouseRect, "SDL_SetWindowMouseRect")
        LOADFUNC(sdllibptr, SetWindowOpacity, "SDL_SetWindowOpacity")
        LOADFUNC(sdllibptr, SetWindowPosition, "SDL_SetWindowPosition")
        LOADFUNC(sdllibptr, SetWindowResizable, "SDL_SetWindowResizable")
        LOADFUNC(sdllibptr, SetWindowShape, "SDL_SetWindowShape")
        LOADFUNC(sdllibptr, SetWindowSize, "SDL_SetWindowSize")
        LOADFUNC(sdllibptr, SetWindowTitle, "SDL_SetWindowTitle")
        LOADFUNC(sdllibptr, SetYUVConversionMode, "SDL_SetYUVConversionMode")
        LOADFUNC(sdllibptr, ShowCursor, "SDL_ShowCursor")
        LOADFUNC(sdllibptr, ShowMessageBox, "SDL_ShowMessageBox")
        LOADFUNC(sdllibptr, ShowSimpleMessageBox, "SDL_ShowSimpleMessageBox")
        LOADFUNC(sdllibptr, ShowWindow, "SDL_ShowWindow")
        LOADFUNC(sdllibptr, SIMDAlloc, "SDL_SIMDAlloc")
        LOADFUNC(sdllibptr, SIMDFree, "SDL_SIMDFree")
        LOADFUNC(sdllibptr, SIMDGetAlignment, "SDL_SIMDGetAlignment")
        LOADFUNC(sdllibptr, SIMDRealloc, "SDL_SIMDRealloc")
        LOADFUNC(sdllibptr, SoftStretch, "SDL_SoftStretch")
        LOADFUNC(sdllibptr, SoftStretchLinear, "SDL_SoftStretchLinear")
        LOADFUNC(sdllibptr, StartTextInput, "SDL_StartTextInput")
        LOADFUNC(sdllibptr, StopTextInput, "SDL_StopTextInput")
        LOADFUNC(sdllibptr, ThreadID, "SDL_ThreadID")
        LOADFUNC(sdllibptr, TLSCleanup, "SDL_TLSCleanup")
        LOADFUNC(sdllibptr, TLSCreate, "SDL_TLSCreate")
        LOADFUNC(sdllibptr, TLSGet, "SDL_TLSGet")
        LOADFUNC(sdllibptr, TLSSet, "SDL_TLSSet")
        LOADFUNC(sdllibptr, TryLockMutex, "SDL_TryLockMutex")
        LOADFUNC(sdllibptr, UnionFRect, "SDL_UnionFRect")
        LOADFUNC(sdllibptr, UnionRect, "SDL_UnionRect")
        LOADFUNC(sdllibptr, UnloadObject, "SDL_UnloadObject")
        LOADFUNC(sdllibptr, UnlockAudio, "SDL_UnlockAudio")
        LOADFUNC(sdllibptr, UnlockAudioDevice, "SDL_UnlockAudioDevice")
        LOADFUNC(sdllibptr, UnlockJoysticks, "SDL_UnlockJoysticks")
        LOADFUNC(sdllibptr, UnlockMutex, "SDL_UnlockMutex")
        LOADFUNC(sdllibptr, UnlockSurface, "SDL_UnlockSurface")
        LOADFUNC(sdllibptr, UnlockTexture, "SDL_UnlockTexture")
        LOADFUNC(sdllibptr, UpdateNVTexture, "SDL_UpdateNVTexture")
        LOADFUNC(sdllibptr, UpdateTexture, "SDL_UpdateTexture")
        LOADFUNC(sdllibptr, UpdateWindowSurface, "SDL_UpdateWindowSurface")
        LOADFUNC(sdllibptr, UpdateWindowSurfaceRects, "SDL_UpdateWindowSurfaceRects")
        LOADFUNC(sdllibptr, UpdateYUVTexture, "SDL_UpdateYUVTexture")
        LOADFUNC(sdllibptr, UpperBlit, "SDL_UpperBlit")
        LOADFUNC(sdllibptr, UpperBlitScaled, "SDL_UpperBlitScaled")
        LOADFUNC(sdllibptr, VideoInit, "SDL_VideoInit")
        LOADFUNC(sdllibptr, VideoQuit, "SDL_VideoQuit")
#ifdef COSMO_SDL2_VULKA
        LOADFUNC(sdllibptr, Vulkan_CreateSurface, "SDL_Vulkan_CreateSurface")
        LOADFUNC(sdllibptr, Vulkan_GetDrawableSize, "SDL_Vulkan_GetDrawableSize")
        LOADFUNC(sdllibptr, Vulkan_GetInstanceExtensions, "SDL_Vulkan_GetInstanceExtensions")
        LOADFUNC(sdllibptr, Vulkan_GetVkGetInstanceProcAddr, "SDL_Vulkan_GetVkGetInstanceProcAddr")
        LOADFUNC(sdllibptr, Vulkan_GetVkInstanceProcAddr, "SDL_Vulkan_GetVkInstanceProcAddr")
        LOADFUNC(sdllibptr, Vulkan_LoadLibrary, "SDL_Vulkan_LoadLibrary")
        LOADFUNC(sdllibptr, Vulkan_UnloadLibrary, "SDL_Vulkan_UnloadLibrary")
#endif
        LOADFUNC(sdllibptr, WaitEvent, "SDL_WaitEvent")
        LOADFUNC(sdllibptr, WaitEventTimeout, "SDL_WaitEventTimeout")
        LOADFUNC(sdllibptr, WaitThread, "SDL_WaitThread")
        LOADFUNC(sdllibptr, WarpMouseGlobal, "SDL_WarpMouseGlobal")
        LOADFUNC(sdllibptr, WarpMouseInWindow, "SDL_WarpMouseInWindow")
        LOADFUNC(sdllibptr, WasInit, "SDL_WasInit")
        LOADFUNC(sdllibptr, WriteBE16, "SDL_WriteBE16")
        LOADFUNC(sdllibptr, WriteBE32, "SDL_WriteBE32")
        LOADFUNC(sdllibptr, WriteBE64, "SDL_WriteBE64")
        LOADFUNC(sdllibptr, WriteLE16, "SDL_WriteLE16")
        LOADFUNC(sdllibptr, WriteLE32, "SDL_WriteLE32")
        LOADFUNC(sdllibptr, WriteLE64, "SDL_WriteLE64")
        LOADFUNC(sdllibptr, WriteU8, "SDL_WriteU8")
        LOADFUNC(sdlnetlibptr, NetAddSocket, "SDLNet_AddSocket")
        LOADFUNC(sdlnetlibptr, NetAllocPacket, "SDLNet_AllocPacket")
        LOADFUNC(sdlnetlibptr, NetAllocPacketV, "SDLNet_AllocPacketV")
        LOADFUNC(sdlnetlibptr, NetAllocSocketSet, "SDLNet_AllocSocketSet")
        LOADFUNC(sdlnetlibptr, NetCheckSockets, "SDLNet_CheckSockets")
        LOADFUNC(sdlnetlibptr, NetDelSocket, "SDLNet_DelSocket")
        LOADFUNC(sdlnetlibptr, NetFreePacket, "SDLNet_FreePacket")
        LOADFUNC(sdlnetlibptr, NetFreePacketV, "SDLNet_FreePacketV")
        LOADFUNC(sdlnetlibptr, NetFreeSocketSet, "SDLNet_FreeSocketSet")
        LOADFUNC(sdlnetlibptr, NetGetError, "SDLNet_GetError")
        LOADFUNC(sdlnetlibptr, NetGetLocalAddresses, "SDLNet_GetLocalAddresses")
        LOADFUNC(sdlnetlibptr, NetInit, "SDLNet_Init")
        LOADFUNC(sdlnetlibptr, NetLinked_Version, "SDLNet_Linked_Version")
        LOADFUNC(sdlnetlibptr, NetQuit, "SDLNet_Quit")
        LOADFUNC(sdlnetlibptr, NetResizePacket, "SDLNet_ResizePacket")
        LOADFUNC(sdlnetlibptr, NetResolveHost, "SDLNet_ResolveHost")
        LOADFUNC(sdlnetlibptr, NetResolveIP, "SDLNet_ResolveIP")
        LOADFUNC(sdlnetlibptr, NetTCP_Accept, "SDLNet_TCP_Accept")
        LOADFUNC(sdlnetlibptr, NetTCP_Close, "SDLNet_TCP_Close")
        LOADFUNC(sdlnetlibptr, NetTCP_GetPeerAddress, "SDLNet_TCP_GetPeerAddress")
        LOADFUNC(sdlnetlibptr, NetTCP_Open, "SDLNet_TCP_Open")
        LOADFUNC(sdlnetlibptr, NetTCP_Recv, "SDLNet_TCP_Recv")
        LOADFUNC(sdlnetlibptr, NetTCP_Send, "SDLNet_TCP_Send")
        LOADFUNC(sdlnetlibptr, NetUDP_Bind, "SDLNet_UDP_Bind")
        LOADFUNC(sdlnetlibptr, NetUDP_Close, "SDLNet_UDP_Close")
        LOADFUNC(sdlnetlibptr, NetUDP_GetPeerAddress, "SDLNet_UDP_GetPeerAddress")
        LOADFUNC(sdlnetlibptr, NetUDP_Open, "SDLNet_UDP_Open")
        LOADFUNC(sdlnetlibptr, NetUDP_Recv, "SDLNet_UDP_Recv")
        LOADFUNC(sdlnetlibptr, NetUDP_RecvV, "SDLNet_UDP_RecvV")
        LOADFUNC(sdlnetlibptr, NetUDP_Send, "SDLNet_UDP_Send")
        LOADFUNC(sdlnetlibptr, NetUDP_SendV, "SDLNet_UDP_SendV")
        LOADFUNC(sdlnetlibptr, NetUDP_SetPacketLoss, "SDLNet_UDP_SetPacketLoss")
        LOADFUNC(sdlnetlibptr, NetUDP_Unbind, "SDLNet_UDP_Unbind")
        LOADFUNC(sdlmixlibptr, MixAllocateChannels, "Mix_AllocateChannels")
        LOADFUNC(sdlmixlibptr, MixChannelFinished, "Mix_ChannelFinished")
        LOADFUNC(sdlmixlibptr, MixCloseAudio, "Mix_CloseAudio")
        LOADFUNC(sdlmixlibptr, MixEachSoundFont, "Mix_EachSoundFont")
        LOADFUNC(sdlmixlibptr, MixExpireChannel, "Mix_ExpireChannel")
        LOADFUNC(sdlmixlibptr, MixFadeInChannel, "Mix_FadeInChannel")
        LOADFUNC(sdlmixlibptr, MixFadeInChannelTimed, "Mix_FadeInChannelTimed")
        LOADFUNC(sdlmixlibptr, MixFadeInMusic, "Mix_FadeInMusic")
        LOADFUNC(sdlmixlibptr, MixFadeInMusicPos, "Mix_FadeInMusicPos")
        LOADFUNC(sdlmixlibptr, MixFadeOutChannel, "Mix_FadeOutChannel")
        LOADFUNC(sdlmixlibptr, MixFadeOutGroup, "Mix_FadeOutGroup")
        LOADFUNC(sdlmixlibptr, MixFadeOutMusic, "Mix_FadeOutMusic")
        LOADFUNC(sdlmixlibptr, MixFadingChannel, "Mix_FadingChannel")
        LOADFUNC(sdlmixlibptr, MixFadingMusic, "Mix_FadingMusic")
        LOADFUNC(sdlmixlibptr, MixFreeChunk, "Mix_FreeChunk")
        LOADFUNC(sdlmixlibptr, MixFreeMusic, "Mix_FreeMusic")
        LOADFUNC(sdlmixlibptr, MixGetChunk, "Mix_GetChunk")
        LOADFUNC(sdlmixlibptr, MixGetChunkDecoder, "Mix_GetChunkDecoder")
        LOADFUNC(sdlmixlibptr, MixGetMusicAlbumTag, "Mix_GetMusicAlbumTag")
        LOADFUNC(sdlmixlibptr, MixGetMusicArtistTag, "Mix_GetMusicArtistTag")
        LOADFUNC(sdlmixlibptr, MixGetMusicCopyrightTag, "Mix_GetMusicCopyrightTag")
        LOADFUNC(sdlmixlibptr, MixGetMusicDecoder, "Mix_GetMusicDecoder")
        LOADFUNC(sdlmixlibptr, MixGetMusicHookData, "Mix_GetMusicHookData")
        LOADFUNC(sdlmixlibptr, MixGetMusicLoopEndTime, "Mix_GetMusicLoopEndTime")
        LOADFUNC(sdlmixlibptr, MixGetMusicLoopLengthTime, "Mix_GetMusicLoopLengthTime")
        LOADFUNC(sdlmixlibptr, MixGetMusicLoopStartTime, "Mix_GetMusicLoopStartTime")
        LOADFUNC(sdlmixlibptr, MixGetMusicPosition, "Mix_GetMusicPosition")
        LOADFUNC(sdlmixlibptr, MixGetMusicTitle, "Mix_GetMusicTitle")
        LOADFUNC(sdlmixlibptr, MixGetMusicTitleTag, "Mix_GetMusicTitleTag")
        LOADFUNC(sdlmixlibptr, MixGetMusicType, "Mix_GetMusicType")
        LOADFUNC(sdlmixlibptr, MixGetMusicVolume, "Mix_GetMusicVolume")
        LOADFUNC(sdlmixlibptr, MixGetNumChunkDecoders, "Mix_GetNumChunkDecoders")
        LOADFUNC(sdlmixlibptr, MixGetNumMusicDecoders, "Mix_GetNumMusicDecoders")
        LOADFUNC(sdlmixlibptr, MixGetSoundFonts, "Mix_GetSoundFonts")
        LOADFUNC(sdlmixlibptr, MixGetSynchroValue, "Mix_GetSynchroValue")
        LOADFUNC(sdlmixlibptr, MixGetTimidityCfg, "Mix_GetTimidityCfg")
        LOADFUNC(sdlmixlibptr, MixGroupAvailable, "Mix_GroupAvailable")
        LOADFUNC(sdlmixlibptr, MixGroupChannel, "Mix_GroupChannel")
        LOADFUNC(sdlmixlibptr, MixGroupChannels, "Mix_GroupChannels")
        LOADFUNC(sdlmixlibptr, MixGroupCount, "Mix_GroupCount")
        LOADFUNC(sdlmixlibptr, MixGroupNewer, "Mix_GroupNewer")
        LOADFUNC(sdlmixlibptr, MixGroupOldest, "Mix_GroupOldest")
        LOADFUNC(sdlmixlibptr, MixHaltChannel, "Mix_HaltChannel")
        LOADFUNC(sdlmixlibptr, MixHaltGroup, "Mix_HaltGroup")
        LOADFUNC(sdlmixlibptr, MixHaltMusic, "Mix_HaltMusic")
        LOADFUNC(sdlmixlibptr, MixHasChunkDecoder, "Mix_HasChunkDecoder")
        LOADFUNC(sdlmixlibptr, MixHasMusicDecoder, "Mix_HasMusicDecoder")
        LOADFUNC(sdlmixlibptr, MixHookMusic, "Mix_HookMusic")
        LOADFUNC(sdlmixlibptr, MixHookMusicFinished, "Mix_HookMusicFinished")
        LOADFUNC(sdlmixlibptr, MixInit, "Mix_Init")
        LOADFUNC(sdlmixlibptr, MixLinked_Version, "Mix_Linked_Version")
        LOADFUNC(sdlmixlibptr, MixLoadMUS, "Mix_LoadMUS")
        LOADFUNC(sdlmixlibptr, MixLoadMUS_RW, "Mix_LoadMUS_RW")
        LOADFUNC(sdlmixlibptr, MixLoadMUSType_RW, "Mix_LoadMUSType_RW")
        LOADFUNC(sdlmixlibptr, MixLoadWAV, "Mix_LoadWAV")
        LOADFUNC(sdlmixlibptr, MixLoadWAV_RW, "Mix_LoadWAV_RW")
        LOADFUNC(sdlmixlibptr, MixMasterVolume, "Mix_MasterVolume")
        LOADFUNC(sdlmixlibptr, MixModMusicJumpToOrder, "Mix_ModMusicJumpToOrder")
        LOADFUNC(sdlmixlibptr, MixMusicDuration, "Mix_MusicDuration")
        LOADFUNC(sdlmixlibptr, MixOpenAudio, "Mix_OpenAudio")
        LOADFUNC(sdlmixlibptr, MixOpenAudioDevice, "Mix_OpenAudioDevice")
        LOADFUNC(sdlmixlibptr, MixPause, "Mix_Pause")
        LOADFUNC(sdlmixlibptr, MixPaused, "Mix_Paused")
        LOADFUNC(sdlmixlibptr, MixPausedMusic, "Mix_PausedMusic")
        LOADFUNC(sdlmixlibptr, MixPauseMusic, "Mix_PauseMusic")
        LOADFUNC(sdlmixlibptr, MixPlayChannel, "Mix_PlayChannel")
        LOADFUNC(sdlmixlibptr, MixPlayChannelTimed, "Mix_PlayChannelTimed")
        LOADFUNC(sdlmixlibptr, MixPlaying, "Mix_Playing")
        LOADFUNC(sdlmixlibptr, MixPlayingMusic, "Mix_PlayingMusic")
        LOADFUNC(sdlmixlibptr, MixPlayMusic, "Mix_PlayMusic")
        LOADFUNC(sdlmixlibptr, MixQuerySpec, "Mix_QuerySpec")
        LOADFUNC(sdlmixlibptr, MixQuickLoad_RAW, "Mix_QuickLoad_RAW")
        LOADFUNC(sdlmixlibptr, MixQuickLoad_WAV, "Mix_QuickLoad_WAV")
        LOADFUNC(sdlmixlibptr, MixQuit, "Mix_Quit")
        LOADFUNC(sdlmixlibptr, MixRegisterEffect, "Mix_RegisterEffect")
        LOADFUNC(sdlmixlibptr, MixReserveChannels, "Mix_ReserveChannels")
        LOADFUNC(sdlmixlibptr, MixResume, "Mix_Resume")
        LOADFUNC(sdlmixlibptr, MixResumeMusic, "Mix_ResumeMusic")
        LOADFUNC(sdlmixlibptr, MixRewindMusic, "Mix_RewindMusic")
        LOADFUNC(sdlmixlibptr, MixSetDistance, "Mix_SetDistance")
        LOADFUNC(sdlmixlibptr, MixSetMusicCMD, "Mix_SetMusicCMD")
        LOADFUNC(sdlmixlibptr, MixSetMusicPosition, "Mix_SetMusicPosition")
        LOADFUNC(sdlmixlibptr, MixSetPanning, "Mix_SetPanning")
        LOADFUNC(sdlmixlibptr, MixSetPosition, "Mix_SetPosition")
        LOADFUNC(sdlmixlibptr, MixSetPostMix, "Mix_SetPostMix")
        LOADFUNC(sdlmixlibptr, MixSetReverseStereo, "Mix_SetReverseStereo")
        LOADFUNC(sdlmixlibptr, MixSetSoundFonts, "Mix_SetSoundFonts")
        LOADFUNC(sdlmixlibptr, MixSetSynchroValue, "Mix_SetSynchroValue")
        LOADFUNC(sdlmixlibptr, MixSetTimidityCfg, "Mix_SetTimidityCfg")
        LOADFUNC(sdlmixlibptr, MixUnregisterAllEffects, "Mix_UnregisterAllEffects")
        LOADFUNC(sdlmixlibptr, MixUnregisterEffect, "Mix_UnregisterEffect")
        LOADFUNC(sdlmixlibptr, MixVolume, "Mix_Volume")
        LOADFUNC(sdlmixlibptr, MixVolumeChunk, "Mix_VolumeChunk")
        LOADFUNC(sdlmixlibptr, MixVolumeMusic, "Mix_VolumeMusic")
        LOADFUNC(sdlttflibptr, TTFByteSwappedUNICODE, "TTF_ByteSwappedUNICODE")
        LOADFUNC(sdlttflibptr, TTFCloseFont, "TTF_CloseFont")
        LOADFUNC(sdlttflibptr, TTFFontAscent, "TTF_FontAscent")
        LOADFUNC(sdlttflibptr, TTFFontDescent, "TTF_FontDescent")
        LOADFUNC(sdlttflibptr, TTFFontFaceFamilyName, "TTF_FontFaceFamilyName")
        LOADFUNC(sdlttflibptr, TTFFontFaceIsFixedWidth, "TTF_FontFaceIsFixedWidth")
        LOADFUNC(sdlttflibptr, TTFFontFaces, "TTF_FontFaces")
        LOADFUNC(sdlttflibptr, TTFFontFaceStyleName, "TTF_FontFaceStyleName")
        LOADFUNC(sdlttflibptr, TTFFontHeight, "TTF_FontHeight")
        LOADFUNC(sdlttflibptr, TTFFontLineSkip, "TTF_FontLineSkip")
        LOADFUNC(sdlttflibptr, TTFGetFontHinting, "TTF_GetFontHinting")
        LOADFUNC(sdlttflibptr, TTFGetFontKerning, "TTF_GetFontKerning")
        LOADFUNC(sdlttflibptr, TTFGetFontOutline, "TTF_GetFontOutline")
        LOADFUNC(sdlttflibptr, TTFGetFontStyle, "TTF_GetFontStyle")
        LOADFUNC(sdlttflibptr, TTFGetFontWrappedAlign, "TTF_GetFontWrappedAlign")
        LOADFUNC(sdlttflibptr, TTFGetFreeTypeVersion, "TTF_GetFreeTypeVersion")
        LOADFUNC(sdlttflibptr, TTFGetHarfBuzzVersion, "TTF_GetHarfBuzzVersion")
        LOADFUNC(sdlttflibptr, TTFGlyphIsProvided, "TTF_GlyphIsProvided")
        LOADFUNC(sdlttflibptr, TTFGlyphIsProvided32, "TTF_GlyphIsProvided32")
        LOADFUNC(sdlttflibptr, TTFGlyphMetrics, "TTF_GlyphMetrics")
        LOADFUNC(sdlttflibptr, TTFGlyphMetrics32, "TTF_GlyphMetrics32")
        LOADFUNC(sdlttflibptr, TTFInit, "TTF_Init")
        LOADFUNC(sdlttflibptr, TTFLinked_Version, "TTF_Linked_Version")
        LOADFUNC(sdlttflibptr, TTFMeasureText, "TTF_MeasureText")
        LOADFUNC(sdlttflibptr, TTFMeasureUNICODE, "TTF_MeasureUNICODE")
        LOADFUNC(sdlttflibptr, TTFMeasureUTF8, "TTF_MeasureUTF8")
        LOADFUNC(sdlttflibptr, TTFOpenFont, "TTF_OpenFont")
        LOADFUNC(sdlttflibptr, TTFOpenFontDPI, "TTF_OpenFontDPI")
        LOADFUNC(sdlttflibptr, TTFOpenFontDPIRW, "TTF_OpenFontDPIRW")
        LOADFUNC(sdlttflibptr, TTFOpenFontIndex, "TTF_OpenFontIndex")
        LOADFUNC(sdlttflibptr, TTFOpenFontIndexDPI, "TTF_OpenFontIndexDPI")
        LOADFUNC(sdlttflibptr, TTFOpenFontIndexDPIRW, "TTF_OpenFontIndexDPIRW")
        LOADFUNC(sdlttflibptr, TTFOpenFontIndexRW, "TTF_OpenFontIndexRW")
        LOADFUNC(sdlttflibptr, TTFOpenFontRW, "TTF_OpenFontRW")
        LOADFUNC(sdlttflibptr, TTFQuit, "TTF_Quit")
        LOADFUNC(sdlttflibptr, TTFRenderGlyph32_Blended, "TTF_RenderGlyph32_Blended")
        LOADFUNC(sdlttflibptr, TTFRenderGlyph32_LCD, "TTF_RenderGlyph32_LCD")
        LOADFUNC(sdlttflibptr, TTFRenderGlyph32_Shaded, "TTF_RenderGlyph32_Shaded")
        LOADFUNC(sdlttflibptr, TTFRenderGlyph32_Solid, "TTF_RenderGlyph32_Solid")
        LOADFUNC(sdlttflibptr, TTFRenderGlyph_Blended, "TTF_RenderGlyph_Blended")
        LOADFUNC(sdlttflibptr, TTFRenderGlyph_LCD, "TTF_RenderGlyph_LCD")
        LOADFUNC(sdlttflibptr, TTFRenderGlyph_Shaded, "TTF_RenderGlyph_Shaded")
        LOADFUNC(sdlttflibptr, TTFRenderGlyph_Solid, "TTF_RenderGlyph_Solid")
        LOADFUNC(sdlttflibptr, TTFRenderText_Blended, "TTF_RenderText_Blended")
        LOADFUNC(sdlttflibptr, TTFRenderText_Blended_Wrapped, "TTF_RenderText_Blended_Wrapped")
        LOADFUNC(sdlttflibptr, TTFRenderText_LCD, "TTF_RenderText_LCD")
        LOADFUNC(sdlttflibptr, TTFRenderText_LCD_Wrapped, "TTF_RenderText_LCD_Wrapped")
        LOADFUNC(sdlttflibptr, TTFRenderText_Shaded, "TTF_RenderText_Shaded")
        LOADFUNC(sdlttflibptr, TTFRenderText_Shaded_Wrapped, "TTF_RenderText_Shaded_Wrapped")
        LOADFUNC(sdlttflibptr, TTFRenderText_Solid, "TTF_RenderText_Solid")
        LOADFUNC(sdlttflibptr, TTFRenderText_Solid_Wrapped, "TTF_RenderText_Solid_Wrapped")
        LOADFUNC(sdlttflibptr, TTFRenderUNICODE_Blended, "TTF_RenderUNICODE_Blended")
        LOADFUNC(sdlttflibptr, TTFRenderUNICODE_Blended_Wrapped, "TTF_RenderUNICODE_Blended_Wrapped")
        LOADFUNC(sdlttflibptr, TTFRenderUNICODE_LCD, "TTF_RenderUNICODE_LCD")
        LOADFUNC(sdlttflibptr, TTFRenderUNICODE_LCD_Wrapped, "TTF_RenderUNICODE_LCD_Wrapped")
        LOADFUNC(sdlttflibptr, TTFRenderUNICODE_Shaded, "TTF_RenderUNICODE_Shaded")
        LOADFUNC(sdlttflibptr, TTFRenderUNICODE_Shaded_Wrapped, "TTF_RenderUNICODE_Shaded_Wrapped")
        LOADFUNC(sdlttflibptr, TTFRenderUNICODE_Solid, "TTF_RenderUNICODE_Solid")
        LOADFUNC(sdlttflibptr, TTFRenderUNICODE_Solid_Wrapped, "TTF_RenderUNICODE_Solid_Wrapped")
        LOADFUNC(sdlttflibptr, TTFRenderUTF8_Blended, "TTF_RenderUTF8_Blended")
        LOADFUNC(sdlttflibptr, TTFRenderUTF8_Blended_Wrapped, "TTF_RenderUTF8_Blended_Wrapped")
        LOADFUNC(sdlttflibptr, TTFRenderUTF8_LCD, "TTF_RenderUTF8_LCD")
        LOADFUNC(sdlttflibptr, TTFRenderUTF8_LCD_Wrapped, "TTF_RenderUTF8_LCD_Wrapped")
        LOADFUNC(sdlttflibptr, TTFRenderUTF8_Shaded, "TTF_RenderUTF8_Shaded")
        LOADFUNC(sdlttflibptr, TTFRenderUTF8_Shaded_Wrapped, "TTF_RenderUTF8_Shaded_Wrapped")
        LOADFUNC(sdlttflibptr, TTFRenderUTF8_Solid, "TTF_RenderUTF8_Solid")
        LOADFUNC(sdlttflibptr, TTFRenderUTF8_Solid_Wrapped, "TTF_RenderUTF8_Solid_Wrapped")
        LOADFUNC(sdlttflibptr, TTFSetDirection, "TTF_SetDirection")
        LOADFUNC(sdlttflibptr, TTFSetFontDirection, "TTF_SetFontDirection")
        LOADFUNC(sdlttflibptr, TTFSetFontHinting, "TTF_SetFontHinting")
        LOADFUNC(sdlttflibptr, TTFSetFontKerning, "TTF_SetFontKerning")
        LOADFUNC(sdlttflibptr, TTFSetFontOutline, "TTF_SetFontOutline")
        LOADFUNC(sdlttflibptr, TTFSetFontScriptName, "TTF_SetFontScriptName")
        LOADFUNC(sdlttflibptr, TTFSetFontSize, "TTF_SetFontSize")
        LOADFUNC(sdlttflibptr, TTFSetFontSizeDPI, "TTF_SetFontSizeDPI")
        LOADFUNC(sdlttflibptr, TTFSetFontStyle, "TTF_SetFontStyle")
        LOADFUNC(sdlttflibptr, TTFSetFontWrappedAlign, "TTF_SetFontWrappedAlign")
        LOADFUNC(sdlttflibptr, TTFSetScript, "TTF_SetScript")
        LOADFUNC(sdlttflibptr, TTFSizeText, "TTF_SizeText")
        LOADFUNC(sdlttflibptr, TTFSizeUNICODE, "TTF_SizeUNICODE")
        LOADFUNC(sdlttflibptr, TTFSizeUTF8, "TTF_SizeUTF8")
        LOADFUNC(sdlttflibptr, TTFWasInit, "TTF_WasInit")
        LOADFUNC(sdlimglibptr, IMGFreeAnimation, "IMG_FreeAnimation")
        LOADFUNC(sdlimglibptr, IMGInit, "IMG_Init")
        LOADFUNC(sdlimglibptr, IMGisAVIF, "IMG_isAVIF")
        LOADFUNC(sdlimglibptr, IMGisBMP, "IMG_isBMP")
        LOADFUNC(sdlimglibptr, IMGisCUR, "IMG_isCUR")
        LOADFUNC(sdlimglibptr, IMGisGIF, "IMG_isGIF")
        LOADFUNC(sdlimglibptr, IMGisICO, "IMG_isICO")
        LOADFUNC(sdlimglibptr, IMGisJPG, "IMG_isJPG")
        LOADFUNC(sdlimglibptr, IMGisJXL, "IMG_isJXL")
        LOADFUNC(sdlimglibptr, IMGisLBM, "IMG_isLBM")
        LOADFUNC(sdlimglibptr, IMGisPCX, "IMG_isPCX")
        LOADFUNC(sdlimglibptr, IMGisPNG, "IMG_isPNG")
        LOADFUNC(sdlimglibptr, IMGisPNM, "IMG_isPNM")
        LOADFUNC(sdlimglibptr, IMGisQOI, "IMG_isQOI")
        LOADFUNC(sdlimglibptr, IMGisSVG, "IMG_isSVG")
        LOADFUNC(sdlimglibptr, IMGisTIF, "IMG_isTIF")
        LOADFUNC(sdlimglibptr, IMGisWEBP, "IMG_isWEBP")
        LOADFUNC(sdlimglibptr, IMGisXCF, "IMG_isXCF")
        LOADFUNC(sdlimglibptr, IMGisXPM, "IMG_isXPM")
        LOADFUNC(sdlimglibptr, IMGisXV, "IMG_isXV")
        LOADFUNC(sdlimglibptr, IMGLinked_Version, "IMG_Linked_Version")
        LOADFUNC(sdlimglibptr, IMGLoad, "IMG_Load")
        LOADFUNC(sdlimglibptr, IMGLoad_RW, "IMG_Load_RW")
        LOADFUNC(sdlimglibptr, IMGLoadAnimation, "IMG_LoadAnimation")
        LOADFUNC(sdlimglibptr, IMGLoadAnimation_RW, "IMG_LoadAnimation_RW")
        LOADFUNC(sdlimglibptr, IMGLoadAnimationTyped_RW, "IMG_LoadAnimationTyped_RW")
        LOADFUNC(sdlimglibptr, IMGLoadAVIF_RW, "IMG_LoadAVIF_RW")
        LOADFUNC(sdlimglibptr, IMGLoadBMP_RW, "IMG_LoadBMP_RW")
        LOADFUNC(sdlimglibptr, IMGLoadCUR_RW, "IMG_LoadCUR_RW")
        LOADFUNC(sdlimglibptr, IMGLoadGIF_RW, "IMG_LoadGIF_RW")
        LOADFUNC(sdlimglibptr, IMGLoadGIFAnimation_RW, "IMG_LoadGIFAnimation_RW")
        LOADFUNC(sdlimglibptr, IMGLoadICO_RW, "IMG_LoadICO_RW")
        LOADFUNC(sdlimglibptr, IMGLoadJPG_RW, "IMG_LoadJPG_RW")
        LOADFUNC(sdlimglibptr, IMGLoadJXL_RW, "IMG_LoadJXL_RW")
        LOADFUNC(sdlimglibptr, IMGLoadLBM_RW, "IMG_LoadLBM_RW")
        LOADFUNC(sdlimglibptr, IMGLoadPCX_RW, "IMG_LoadPCX_RW")
        LOADFUNC(sdlimglibptr, IMGLoadPNG_RW, "IMG_LoadPNG_RW")
        LOADFUNC(sdlimglibptr, IMGLoadPNM_RW, "IMG_LoadPNM_RW")
        LOADFUNC(sdlimglibptr, IMGLoadQOI_RW, "IMG_LoadQOI_RW")
        LOADFUNC(sdlimglibptr, IMGLoadSizedSVG_RW, "IMG_LoadSizedSVG_RW")
        LOADFUNC(sdlimglibptr, IMGLoadSVG_RW, "IMG_LoadSVG_RW")
        LOADFUNC(sdlimglibptr, IMGLoadTexture, "IMG_LoadTexture")
        LOADFUNC(sdlimglibptr, IMGLoadTexture_RW, "IMG_LoadTexture_RW")
        LOADFUNC(sdlimglibptr, IMGLoadTextureTyped_RW, "IMG_LoadTextureTyped_RW")
        LOADFUNC(sdlimglibptr, IMGLoadTGA_RW, "IMG_LoadTGA_RW")
        LOADFUNC(sdlimglibptr, IMGLoadTIF_RW, "IMG_LoadTIF_RW")
        LOADFUNC(sdlimglibptr, IMGLoadTyped_RW, "IMG_LoadTyped_RW")
        LOADFUNC(sdlimglibptr, IMGLoadWEBP_RW, "IMG_LoadWEBP_RW")
        LOADFUNC(sdlimglibptr, IMGLoadXCF_RW, "IMG_LoadXCF_RW")
        LOADFUNC(sdlimglibptr, IMGLoadXPM_RW, "IMG_LoadXPM_RW")
        LOADFUNC(sdlimglibptr, IMGLoadXV_RW, "IMG_LoadXV_RW")
        LOADFUNC(sdlimglibptr, IMGQuit, "IMG_Quit")
        LOADFUNC(sdlimglibptr, IMGReadXPMFromArray, "IMG_ReadXPMFromArray")
        LOADFUNC(sdlimglibptr, IMGReadXPMFromArrayToRGB888, "IMG_ReadXPMFromArrayToRGB888")
        LOADFUNC(sdlimglibptr, IMGSaveJPG, "IMG_SaveJPG")
        LOADFUNC(sdlimglibptr, IMGSaveJPG_RW, "IMG_SaveJPG_RW")
        LOADFUNC(sdlimglibptr, IMGSavePNG, "IMG_SavePNG")
        LOADFUNC(sdlimglibptr, IMGSavePNG_RW, "IMG_SavePNG_RW")
#endif
        return (::loaded = not err);
    }

    void UnloadLibrary() {
        cosmo_dlclose(::sdllibptr);
        ::loaded = false;
    }
    
    bool IsLoaded() {
        return ::loaded and ::sdllibptr != nullptr;
    }
    /* SDL2 functions */
#if 1 // To hide the list
#define GENFUNC(Function, ...) if (IsWindows()) \
    return (reinterpret_cast<::Function##Proto_WIN>(::Function))(__VA_ARGS__ ); \
    return (reinterpret_cast<::Function##Proto>(::Function))(__VA_ARGS__ );
    
    int Init(uint32_t flags, const std::string& library_path) {
        if (::sdllibptr == nullptr and not LoadSDLLibrary(library_path)) return -1;
        GENFUNC(Init, flags)
    }
    
    void Quit() {
        if (IsWindows()) (reinterpret_cast<::QuitProto_WIN>(::Quit))();
        else (reinterpret_cast<::QuitProto>(::Quit))();
        UnloadLibrary();
    }
    
    SDL_RWops* RWFromFile(const char *file, const char *mode) { GENFUNC(RWFromFile, file, mode) }
    
    void AddEventWatch(SDL_EventFilter filter,  void*userdata) { GENFUNC(AddEventWatch, filter, userdata) }
    
    void AddHintCallback(const char *name,  SDL_HintCallback callback,  void*userdata) { GENFUNC(AddHintCallback, name, callback, userdata) }

    SDL_TimerID AddTimer(uint32_t interval,  SDL_TimerCallback callback,  void*param) { GENFUNC(AddTimer, interval, callback, param) }
    
    SDL_PixelFormat * AllocFormat(uint32_t pixel_format) { GENFUNC(AllocFormat, pixel_format) }
    
    SDL_Palette* AllocPalette(int ncolors) { GENFUNC(AllocPalette, ncolors) }
    
    SDL_RWops* AllocRW() { GENFUNC(AllocRW) }

    int AtomicAdd(SDL_atomic_t *a, int v) { GENFUNC(AtomicAdd, a, v) }

    bool AtomicCAS(SDL_atomic_t *a, int oldval, int newval) { GENFUNC(AtomicCAS, a, oldval, newval) }

    bool AtomicCASPtr(void**a, void*oldval, void*newval) { GENFUNC(AtomicCASPtr, a, oldval, newval) }
    
    bool AtomicDecRef(SDL_atomic_t* a) {
        if (IsWindows()) return ((reinterpret_cast<::AtomicAddProto_WIN>(::AtomicAdd))(a, -1) == 1);
        return ((reinterpret_cast<::AtomicAddProto>(::AtomicAdd))(a, -1) == 1);
    }
    
    int AtomicGet(SDL_atomic_t *a) { GENFUNC(AtomicGet, a) }
    
    void* AtomicGetPtr(void **a) { GENFUNC(AtomicGetPtr, a) }
    
    void AtomicIncRef(SDL_atomic_t* a) { 
        if (IsWindows()) (reinterpret_cast<::AtomicAddProto_WIN>(::AtomicAdd))(a, 1 );
        (reinterpret_cast<::AtomicAddProto>(::AtomicAdd))(a, 1 );
     }
    
    void AtomicLock(SDL_SpinLock *lock) { GENFUNC(AtomicLock, lock) }

    int AtomicSet(SDL_atomic_t *a, int v) { GENFUNC(AtomicSet, a, v) }

    void* AtomicSetPtr(void **a, void* v) { GENFUNC(AtomicSetPtr, a, v) }
    
    bool AtomicTryLock(SDL_SpinLock *lock) { GENFUNC(AtomicTryLock, lock) }
    
    void AtomicUnlock(SDL_SpinLock *lock) { GENFUNC(AtomicUnlock, lock) }
    
    int AudioInit(const char *driver_name) { GENFUNC(AudioInit, driver_name) }
    
    void AudioQuit() { GENFUNC(AudioQuit) }
    
    int AudioStreamAvailable(SDL_AudioStream *stream) { GENFUNC(AudioStreamAvailable, stream) }
    
    void AudioStreamClear(SDL_AudioStream *stream) { GENFUNC(AudioStreamClear, stream) }
    
    int AudioStreamFlush(SDL_AudioStream *stream) { GENFUNC(AudioStreamFlush, stream) }
    
    int AudioStreamGet(SDL_AudioStream *stream, void *buf, int len) { GENFUNC(AudioStreamGet, stream, buf, len) }
    
    int AudioStreamPut(SDL_AudioStream *stream, void *buf, int len) { GENFUNC(AudioStreamPut, stream, buf, len) }
    
    int BlitScaled(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Surface* dst, SDL_Rect* dstrect) { GENFUNC(UpperBlitScaled, src, srcrect, dst, dstrect) }
    
    int BlitSurface(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Surface* dst, SDL_Rect* dstrect) { GENFUNC(BlitSurface, src, srcrect, dst, dstrect) }
    
    int BuildAudioCVT(SDL_AudioCVT * cvt, SDL_AudioFormat src_format, Uint8 src_channels, int src_rate, SDL_AudioFormat dst_format, Uint8 dst_channels, int dst_rate) { GENFUNC(BuildAudioCVT, cvt, src_format, src_channels, src_rate, dst_format, dst_channels, dst_rate) }
    
    void CalculateGammaRamp(float gamma, uint16_t * ramp) { GENFUNC(CalculateGammaRamp, gamma, ramp) }
    
    int CaptureMouse(bool enabled) { GENFUNC(CaptureMouse, enabled) }
    
    void ClearComposition() { GENFUNC(ClearComposition) }
    
    void ClearError() { GENFUNC(ClearError) }
    
    void ClearHints() { GENFUNC(ClearHints) }
    
    void ClearQueuedAudio(SDL_AudioDeviceID dev) { GENFUNC(ClearQueuedAudio, dev) }
    
    void CloseAudio() { GENFUNC(CloseAudio) }
    
    void CloseAudioDevice(SDL_AudioDeviceID dev) { GENFUNC(CloseAudioDevice, dev) }
    
    void CompilerBarrier() {
        SDL_SpinLock _tmp = 0;
        if (IsWindows()) {
            (reinterpret_cast<::AtomicLockProto_WIN>(::AtomicLock))(&_tmp);
            (reinterpret_cast<::AtomicUnlockProto_WIN>(::AtomicUnlock))(&_tmp);
        }
        else {
            (reinterpret_cast<::AtomicLockProto>(::AtomicLock))(&_tmp);
            (reinterpret_cast<::AtomicUnlockProto>(::AtomicUnlock))(&_tmp);
        }
    }
    SDL_BlendMode ComposeCustomBlendMode(SDL_BlendFactor srcColorFactor, SDL_BlendFactor dstColorFactor, SDL_BlendOperation colorOperation, SDL_BlendFactor srcAlphaFactor, SDL_BlendFactor dstAlphaFactor, SDL_BlendOperation alphaOperation) { GENFUNC(ComposeCustomBlendMode, srcColorFactor, dstColorFactor, colorOperation, srcAlphaFactor, dstAlphaFactor, alphaOperation) }
    int CondBroadcast(SDL_cond * cond) { GENFUNC(CondBroadcast, cond) }
    int CondSignal(SDL_cond * cond) { GENFUNC(CondSignal, cond) }

    int CondWait(SDL_cond * cond, SDL_mutex * mutex) { GENFUNC(CondWait, cond, mutex) }

    int CondWaitTimeout(SDL_cond * cond, SDL_mutex * mutex, Uint32 ms) { GENFUNC(CondWaitTimeout, cond, mutex, ms) }
    
    int ConvertAudio(SDL_AudioCVT * cvt) { GENFUNC(ConvertAudio, cvt) }

    int ConvertPixels(int width, int height, Uint32 src_format, void * src, int src_pitch, Uint32 dst_format, void * dst, int dst_pitch) { GENFUNC(ConvertPixels, width, height, src_format, src, src_pitch, dst_format, dst, dst_pitch) }

    SDL_Surface* ConvertSurface(SDL_Surface * src, const SDL_PixelFormat * fmt, Uint32 flags) { GENFUNC(ConvertSurface, src, fmt, flags) }

    SDL_Surface* ConvertSurfaceFormat(SDL_Surface * src, Uint32 pixel_format, Uint32 flags) { GENFUNC(ConvertSurfaceFormat, src, pixel_format, flags) }

    SDL_Cursor* CreateColorCursor(SDL_Surface *surface, int hot_x, int hot_y) { GENFUNC(CreateColorCursor, surface, hot_x, hot_y) }
    
    SDL_cond* CreateCond() { GENFUNC(CreateCond) }

    SDL_Cursor* CreateCursor(const Uint8 * data, const Uint8 * mask, int w, int h, int hot_x, int hot_y) { GENFUNC(CreateCursor, data, mask, w, h, hot_x, hot_y) }
    
    SDL_mutex* CreateMutex() { GENFUNC(CreateMutex) }

    SDL_Renderer * CreateRenderer(SDL_Window * window, int index, Uint32 flags) { GENFUNC(CreateRenderer, window, index, flags) }

    SDL_Surface* CreateRGBSurface(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask) { GENFUNC(CreateRGBSurface, flags, width, height, depth, Rmask, Gmask, Bmask, Amask) }

    SDL_Surface* CreateRGBSurfaceFrom(void *pixels, int width, int height, int depth, int pitch, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask) { GENFUNC(CreateRGBSurfaceFrom, pixels, width, height, depth, pitch, Rmask, Gmask, Bmask, Amask) }

    SDL_Surface* CreateRGBSurfaceWithFormat(Uint32 flags, int width, int height, int depth, Uint32 format) { GENFUNC(CreateRGBSurfaceWithFormat, flags, width, height, depth, format) }

    SDL_Surface* CreateRGBSurfaceWithFormatFrom(void *pixels, int width, int height, int depth, int pitch, Uint32 format) { GENFUNC(CreateRGBSurfaceWithFormatFrom, pixels, width, height, depth, pitch, format) }
    
    SDL_sem* CreateSemaphore(Uint32 initial_value) { GENFUNC(CreateSemaphore, initial_value) }

    SDL_Window * CreateShapedWindow(const char *title,unsigned int x,unsigned int y,unsigned int w,unsigned int h,Uint32 flags) { GENFUNC(CreateShapedWindow, title, x, y, w, h, flags) }
    
    SDL_Renderer * CreateSoftwareRenderer(SDL_Surface * surface) { GENFUNC(CreateSoftwareRenderer, surface) }
    
    SDL_Cursor* CreateSystemCursor(SDL_SystemCursor id) { GENFUNC(CreateSystemCursor, id) }

    SDL_Texture * CreateTexture(SDL_Renderer * renderer, Uint32 format, int access, int w, int h) { GENFUNC(CreateTexture, renderer, format, access, w, h) }

    SDL_Texture * CreateTextureFromSurface(SDL_Renderer * renderer, SDL_Surface * surface) { GENFUNC(CreateTextureFromSurface, renderer, surface) }

    SDL_Thread * CreateThread(SDL_ThreadFunction fn, const char *name, void *data) { GENFUNC(CreateThread, fn, name, data) }

    SDL_Thread * CreateThreadWithStackSize(SDL_ThreadFunction fn, const char *name, const size_t stacksize, void *data) { GENFUNC(CreateThreadWithStackSize, fn, name, stacksize, data) }

    SDL_Window * CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags) { GENFUNC(CreateWindow, title, x, y, w, h, flags) }

    int CreateWindowAndRenderer( int width, int height, Uint32 window_flags, SDL_Window **window, SDL_Renderer **renderer) { GENFUNC(CreateWindowAndRenderer, width, height, window_flags, window, renderer) }
    
    SDL_Window * CreateWindowFrom(void *data) { GENFUNC(CreateWindowFrom, data) }
    
    void Delay(Uint32 ms) { GENFUNC(Delay, ms) }
    
    void DelEventWatch(SDL_EventFilter filter, void *userdata) { GENFUNC(DelEventWatch, filter, userdata) }

    void DelHintCallback(const char *name, SDL_HintCallback callback, void *userdata) { GENFUNC(DelHintCallback, name, callback, userdata) }

    Uint32 DequeueAudio(SDL_AudioDeviceID dev, void *data, Uint32 len) { GENFUNC(DequeueAudio, dev, data, len) }
    
    void DestroyCond(SDL_cond * cond) { GENFUNC(DestroyCond, cond) }
    
    void DestroyMutex(SDL_mutex * mutex) { GENFUNC(DestroyMutex, mutex) }
    
    void DestroyRenderer(SDL_Renderer * renderer) { GENFUNC(DestroyRenderer, renderer) }
    
    void DestroySemaphore(SDL_sem * sem) { GENFUNC(DestroySemaphore, sem) }
    
    void DestroyTexture(SDL_Texture * texture) { GENFUNC(DestroyTexture, texture) }
    
    void DestroyWindow(SDL_Window * window) { GENFUNC(DestroyWindow, window) }
    
    int DestroyWindowSurface(SDL_Window *window) { GENFUNC(DestroyWindowSurface, window) }
    
    void DetachThread(SDL_Thread * thread) { GENFUNC(DetachThread, thread) }
    void DisableScreenSaver() { GENFUNC(DisableScreenSaver) }
    void EnableScreenSaver() { GENFUNC(EnableScreenSaver) }

    bool EncloseFPoints(const SDL_FPoint * points, int count, const SDL_FRect * clip, SDL_FRect * result) { GENFUNC(EncloseFPoints, points, count, clip, result) }

    bool EnclosePoints(const SDL_Point * points, int count, const SDL_Rect * clip, SDL_Rect * result) { GENFUNC(EnclosePoints, points, count, clip, result) }

    Uint8 EventState(Uint32 type, int state) { GENFUNC(EventState, type, state) }

    int FillRect(SDL_Surface * dst, const SDL_Rect * rect, Uint32 color) { GENFUNC(FillRect, dst, rect, color) }

    int FillRects(SDL_Surface * dst, const SDL_Rect * rects, int count, Uint32 color) { GENFUNC(FillRects, dst, rects, count, color) }
    
    void FilterEvents(SDL_EventFilter filter, void *userdata) { GENFUNC(FilterEvents, filter, userdata) }

    int FlashWindow(SDL_Window * window, SDL_FlashOperation operation) { GENFUNC(FlashWindow, window, operation) }
    
    void FlushEvent(Uint32 type) { GENFUNC(FlushEvent, type) }
    
    void FlushEvents(Uint32 minType, Uint32 maxType) { GENFUNC(FlushEvents, minType, maxType) }
    
    bool FRectEmpty(const SDL_FRect* r) { return SDL_FRectEmpty(r); }

    bool FRectEquals(const SDL_FRect* a,  const SDL_FRect* b) {
        return (a && b && ((a == b) ||
            ((fabsf(a->x - b->x) <= FLT_EPSILON) &&
            (fabsf(a->y - b->y) <= FLT_EPSILON) &&
            (fabsf(a->w - b->w) <= FLT_EPSILON) &&
            (fabsf(a->h - b->h) <= FLT_EPSILON))))
            ? SDL_TRUE : SDL_FALSE;
    }
    void FreeAudioStream(SDL_AudioStream *stream) { GENFUNC(FreeAudioStream, stream) }
    
    void FreeCursor(SDL_Cursor * cursor) { GENFUNC(FreeCursor, cursor) }
    
    void FreeFormat(SDL_PixelFormat *format) { GENFUNC(FreeFormat, format) }
    
    void FreePalette(SDL_Palette * palette) { GENFUNC(FreePalette, palette) }
    
    void FreeRW(SDL_RWops * area) { GENFUNC(FreeRW, area) }
    
    void FreeSurface(SDL_Surface * surface) { GENFUNC(FreeSurface, surface) }
    
    void FreeWAV(Uint8 * audio_buf) { GENFUNC(FreeWAV, audio_buf) }
    
    int GameControllerAddMapping(const char* mappingString) { GENFUNC(GameControllerAddMapping, mappingString) }

    int GameControllerAddMappingsFromRW(SDL_RWops * rw, int freerw) { GENFUNC(GameControllerAddMappingsFromRW, rw, freerw) }
    
    void GameControllerClose(SDL_GameController *gamecontroller) { GENFUNC(GameControllerClose, gamecontroller) }
    
    int GameControllerEventState(int state) { GENFUNC(GameControllerEventState, state) }
    
    SDL_GameController* GameControllerFromInstanceID(SDL_JoystickID joyid) { GENFUNC(GameControllerFromInstanceID, joyid) }
    
    SDL_GameController* GameControllerFromPlayerIndex(int player_index) { GENFUNC(GameControllerFromPlayerIndex, player_index) }

    const char* GameControllerGetAppleSFSymbolsNameForAxis(SDL_GameController *gamecontroller, SDL_GameControllerAxis axis) { GENFUNC(GameControllerGetAppleSFSymbolsNameForAxis, gamecontroller, axis) }

    const char* GameControllerGetAppleSFSymbolsNameForButton(SDL_GameController *gamecontroller, SDL_GameControllerButton button) { GENFUNC(GameControllerGetAppleSFSymbolsNameForButton, gamecontroller, button) }
    
    bool GameControllerGetAttached(SDL_GameController *gamecontroller) { GENFUNC(GameControllerGetAttached, gamecontroller) }

    Sint16 GameControllerGetAxis(SDL_GameController *gamecontroller, SDL_GameControllerAxis axis) { GENFUNC(GameControllerGetAxis, gamecontroller, axis) }
    
    SDL_GameControllerAxis GameControllerGetAxisFromString(const char *str) { GENFUNC(GameControllerGetAxisFromString, str) }

    SDL_GameControllerButtonBind GameControllerGetBindForAxis(SDL_GameController *gamecontroller, SDL_GameControllerAxis axis) { GENFUNC(GameControllerGetBindForAxis, gamecontroller, axis) }

    SDL_GameControllerButtonBind GameControllerGetBindForButton(SDL_GameController *gamecontroller, SDL_GameControllerButton button) { GENFUNC(GameControllerGetBindForButton, gamecontroller, button) }

    Uint8 GameControllerGetButton(SDL_GameController *gamecontroller, SDL_GameControllerButton button) { GENFUNC(GameControllerGetButton, gamecontroller, button) }
    
    SDL_GameControllerButton GameControllerGetButtonFromString(const char *str) { GENFUNC(GameControllerGetButtonFromString, str) }
    
    uint16_t GameControllerGetFirmwareVersion(SDL_GameController *gamecontroller) { GENFUNC(GameControllerGetFirmwareVersion, gamecontroller) }
    
    SDL_Joystick* GameControllerGetJoystick(SDL_GameController *gamecontroller) { GENFUNC(GameControllerGetJoystick, gamecontroller) }
    int GameControllerGetNumTouchpadFingers(SDL_GameController *gamecontroller, int touchpad) { GENFUNC(GameControllerGetNumTouchpadFingers, gamecontroller, touchpad) }
    int GameControllerGetNumTouchpads(SDL_GameController *gamecontroller) { GENFUNC(GameControllerGetNumTouchpads, gamecontroller) }
    
    int GameControllerGetPlayerIndex(SDL_GameController *gamecontroller) { GENFUNC(GameControllerGetPlayerIndex, gamecontroller) }
    
    uint16_t GameControllerGetProduct(SDL_GameController *gamecontroller) { GENFUNC(GameControllerGetProduct, gamecontroller) }
    
    uint16_t GameControllerGetProductVersion(SDL_GameController *gamecontroller) { GENFUNC(GameControllerGetProductVersion, gamecontroller) }

    int GameControllerGetSensorData(SDL_GameController *gamecontroller, SDL_SensorType type, float *data, int num_values) { GENFUNC(GameControllerGetSensorData, gamecontroller, type, data, num_values) }

    float GameControllerGetSensorDataRate(SDL_GameController *gamecontroller, SDL_SensorType type) { GENFUNC(GameControllerGetSensorDataRate, gamecontroller, type) }

    int GameControllerGetSensorDataWithTimestamp(SDL_GameController *gamecontroller, SDL_SensorType type, Uint64 *timestamp, float *data, int num_values) { GENFUNC(GameControllerGetSensorDataWithTimestamp, gamecontroller, type, timestamp, data, num_values) }
    
    const char * GameControllerGetSerial(SDL_GameController *gamecontroller) { GENFUNC(GameControllerGetSerial, gamecontroller) }
    
    const char* GameControllerGetStringForAxis(SDL_GameControllerAxis axis) { GENFUNC(GameControllerGetStringForAxis, axis) }
    
    const char* GameControllerGetStringForButton(SDL_GameControllerButton button) { GENFUNC(GameControllerGetStringForButton, button) }
    int GameControllerGetTouchpadFinger(SDL_GameController *gamecontroller, int touchpad, int finger, Uint8 *state, float *x, float *y, float *pressure) { GENFUNC(GameControllerGetTouchpadFinger, gamecontroller, touchpad, finger, state, x, y, pressure) }
    
    SDL_GameControllerType GameControllerGetType(SDL_GameController *gamecontroller) { GENFUNC(GameControllerGetType, gamecontroller) }
    
    uint16_t GameControllerGetVendor(SDL_GameController *gamecontroller) { GENFUNC(GameControllerGetVendor, gamecontroller) }

    bool  GameControllerHasAxis(SDL_GameController *gamecontroller, SDL_GameControllerAxis axis) { GENFUNC(GameControllerHasAxis, gamecontroller, axis) }

    bool GameControllerHasButton(SDL_GameController *gamecontroller, SDL_GameControllerButton button) { GENFUNC(GameControllerHasButton, gamecontroller, button) }
    
    bool GameControllerHasLED(SDL_GameController *gamecontroller) { GENFUNC(GameControllerHasLED, gamecontroller) }
    
    bool GameControllerHasRumble(SDL_GameController *gamecontroller) { GENFUNC(GameControllerHasRumble, gamecontroller) }
    
    bool GameControllerHasRumbleTriggers(SDL_GameController *gamecontroller) { GENFUNC(GameControllerHasRumbleTriggers, gamecontroller) }

    bool GameControllerHasSensor(SDL_GameController *gamecontroller, SDL_SensorType type) { GENFUNC(GameControllerHasSensor, gamecontroller, type) }

    bool GameControllerIsSensorEnabled(SDL_GameController *gamecontroller, SDL_SensorType type) { GENFUNC(GameControllerIsSensorEnabled, gamecontroller, type) }
    
    char * GameControllerMapping(SDL_GameController *gamecontroller) { GENFUNC(GameControllerMapping, gamecontroller) }
    
    char* GameControllerMappingForDeviceIndex(int joystick_index) { GENFUNC(GameControllerMappingForDeviceIndex, joystick_index) }
    
    char * GameControllerMappingForGUID(SDL_JoystickGUID guid) { GENFUNC(GameControllerMappingForGUID, guid) }
    
    char * GameControllerMappingForIndex(int mapping_index) { GENFUNC(GameControllerMappingForIndex, mapping_index) }
    
    const char* GameControllerName(SDL_GameController *gamecontroller) { GENFUNC(GameControllerName, gamecontroller) }
    
    const char* GameControllerNameForIndex(int joystick_index) { GENFUNC(GameControllerNameForIndex, joystick_index) }
    
    int GameControllerNumMappings() { GENFUNC(GameControllerNumMappings) }
    
    SDL_GameController* GameControllerOpen(int joystick_index) { GENFUNC(GameControllerOpen, joystick_index) }
    
    const char* GameControllerPath(SDL_GameController *gamecontroller) { GENFUNC(GameControllerPath, gamecontroller) }
    
    const char* GameControllerPathForIndex(int joystick_index) { GENFUNC(GameControllerPathForIndex, joystick_index) }

    int GameControllerRumble(SDL_GameController *gamecontroller, uint16_t low_frequency_rumble, uint16_t high_frequency_rumble, Uint32 duration_ms) { GENFUNC(GameControllerRumble, gamecontroller, low_frequency_rumble, high_frequency_rumble, duration_ms) }

    int GameControllerRumbleTriggers(SDL_GameController *gamecontroller, uint16_t left_rumble, uint16_t right_rumble, Uint32 duration_ms) { GENFUNC(GameControllerRumbleTriggers, gamecontroller, left_rumble, right_rumble, duration_ms) }

    int GameControllerSendEffect(SDL_GameController *gamecontroller, void *data, int size) { GENFUNC(GameControllerSendEffect, gamecontroller, data, size) }

    int GameControllerSetLED(SDL_GameController *gamecontroller, Uint8 red, Uint8 green, Uint8 blue) { GENFUNC(GameControllerSetLED, gamecontroller, red, green, blue) }
    
    void GameControllerSetPlayerIndex(SDL_GameController *gamecontroller, int player_index) { GENFUNC(GameControllerSetPlayerIndex, gamecontroller, player_index) }

    int GameControllerSetSensorEnabled(SDL_GameController *gamecontroller, SDL_SensorType type, bool enabled) { GENFUNC(GameControllerSetSensorEnabled, gamecontroller, type, enabled) }
    
    SDL_GameControllerType GameControllerTypeForIndex(int joystick_index) { GENFUNC(GameControllerTypeForIndex, joystick_index) }
    void GameControllerUpdate() { GENFUNC(GameControllerUpdate) }
    
    SDL_AssertionHandler GetAssertionHandler(void **puserdata) { GENFUNC(GetAssertionHandler, puserdata) }
    
    const SDL_AssertData * GetAssertionReport() { GENFUNC(GetAssertionReport) }

    const char* GetAudioDeviceName(int index, int iscapture) { GENFUNC(GetAudioDeviceName, index, iscapture) }

    int GetAudioDeviceSpec(int index, int iscapture, SDL_AudioSpec *spec) { GENFUNC(GetAudioDeviceSpec, index, iscapture, spec) }
    
    SDL_AudioStatus GetAudioDeviceStatus(SDL_AudioDeviceID dev) { GENFUNC(GetAudioDeviceStatus, dev) }
    
    const char* GetAudioDriver(int index) { GENFUNC(GetAudioDriver, index) }
    
    SDL_AudioStatus GetAudioStatus() { GENFUNC(GetAudioStatus) }
    
    char* GetBasePath() { GENFUNC(GetBasePath) }
    
    char * GetClipboardText() { GENFUNC(GetClipboardText) }
    
    void GetClipRect(SDL_Surface * surface, SDL_Rect * rect) { GENFUNC(GetClipRect, surface, rect) }

    SDL_DisplayMode * GetClosestDisplayMode(int displayIndex, const SDL_DisplayMode * mode, SDL_DisplayMode * closest) { GENFUNC(GetClosestDisplayMode, displayIndex, mode, closest) }

    int GetColorKey(SDL_Surface * surface, Uint32 * key) { GENFUNC(GetColorKey, surface, key) }
    
    int GetCPUCacheLineSize() { GENFUNC(GetCPUCacheLineSize) }
    
    int GetCPUCount() { GENFUNC(GetCPUCount) }
    
    const char* GetCurrentAudioDriver() { GENFUNC(GetCurrentAudioDriver) }

    int GetCurrentDisplayMode(int displayIndex, SDL_DisplayMode * mode) { GENFUNC(GetCurrentDisplayMode, displayIndex, mode) }
    
    const char* GetCurrentVideoDriver() { GENFUNC(GetCurrentVideoDriver) }
    
    SDL_Cursor* GetCursor() { GENFUNC(GetCursor) }
    
    SDL_AssertionHandler GetDefaultAssertionHandler() { GENFUNC(GetDefaultAssertionHandler) }

    int GetDefaultAudioInfo(char **name, SDL_AudioSpec *spec, int iscapture) { GENFUNC(GetDefaultAudioInfo, name, spec, iscapture) }
    
    SDL_Cursor* GetDefaultCursor() { GENFUNC(GetDefaultCursor) }

    int GetDesktopDisplayMode(int displayIndex, SDL_DisplayMode * mode) { GENFUNC(GetDesktopDisplayMode, displayIndex, mode) }

    int GetDisplayBounds(int displayIndex, SDL_Rect * rect) { GENFUNC(GetDisplayBounds, displayIndex, rect) }

    int GetDisplayDPI(int displayIndex, float * ddpi, float * hdpi, float * vdpi) { GENFUNC(GetDisplayDPI, displayIndex, ddpi, hdpi, vdpi) }

    int GetDisplayMode(int displayIndex, int modeIndex, SDL_DisplayMode * mode) { GENFUNC(GetDisplayMode, displayIndex, modeIndex, mode) }
    
    const char * GetDisplayName(int displayIndex) { GENFUNC(GetDisplayName, displayIndex) }
    
    SDL_DisplayOrientation GetDisplayOrientation(int displayIndex) { GENFUNC(GetDisplayOrientation, displayIndex) }

    int GetDisplayUsableBounds(int displayIndex, SDL_Rect * rect) { GENFUNC(GetDisplayUsableBounds, displayIndex, rect) }
    
    const char* GetError() { GENFUNC(GetError) }

    char * GetErrorMsg(char *errstr, int maxlen) { GENFUNC(GetErrorMsg, errstr, maxlen) }

    bool GetEventFilter(SDL_EventFilter * filter, void **userdata) { GENFUNC(GetEventFilter, filter, userdata) }
    
    uint8_t GetEventState(uint32_t type) { GENFUNC(EventState, type, SDL_QUERY) }

    Uint32 GetGlobalMouseState(int *x, int *y) { GENFUNC(GetGlobalMouseState, x, y) }
    
    SDL_Window * GetGrabbedWindow() { GENFUNC(GetGrabbedWindow) }
    
    const char * GetHint(const char *name) { GENFUNC(GetHint, name) }

    bool GetHintBoolean(const char *name, bool default_value) { GENFUNC(GetHintBoolean, name, default_value) }

    void GetJoystickGUIDInfo(SDL_JoystickGUID guid, uint16_t *vendor, uint16_t *product, uint16_t *version, uint16_t *crc16) { GENFUNC(GetJoystickGUIDInfo, guid, vendor, product, version, crc16) }
    
    SDL_Window * GetKeyboardFocus() { GENFUNC(GetKeyboardFocus) }
    
    const Uint8* GetKeyboardState(int *numkeys) { GENFUNC(GetKeyboardState, numkeys) }
    
    SDL_Keycode GetKeyFromName(const char *name) { GENFUNC(GetKeyFromName, name) }
    
    SDL_Keycode GetKeyFromScancode(SDL_Scancode scancode) { GENFUNC(GetKeyFromScancode, scancode) }
    
    const char* GetKeyName(SDL_Keycode key) { GENFUNC(GetKeyName, key) }
    void GetMemoryFunctions(SDL_malloc_func *malloc_func, SDL_calloc_func *calloc_func, SDL_realloc_func *realloc_func, SDL_free_func *free_func) { GENFUNC(GetMemoryFunctions, malloc_func, calloc_func, realloc_func, free_func) }
    
    SDL_Keymod GetModState() { GENFUNC(GetModState) }
    
    SDL_Window * GetMouseFocus() { GENFUNC(GetMouseFocus) }

    Uint32 GetMouseState(int *x, int *y) { GENFUNC(GetMouseState, x, y) }
    int GetNumAllocations() { GENFUNC(GetNumAllocations) }
    
    int GetNumAudioDevices(int iscapture) { GENFUNC(GetNumAudioDevices, iscapture) }
    
    int GetNumAudioDrivers() { GENFUNC(GetNumAudioDrivers) }
    
    int GetNumDisplayModes(int displayIndex) { GENFUNC(GetNumDisplayModes, displayIndex) }
    
    int GetNumRenderDrivers() { GENFUNC(GetNumRenderDrivers) }
    
    int GetNumTouchDevices() { GENFUNC(GetNumTouchDevices) }
    
    int GetNumTouchFingers(SDL_TouchID touchID) { GENFUNC(GetNumTouchFingers, touchID) }
    
    int GetNumVideoDisplays() { GENFUNC(GetNumVideoDisplays) }
    
    int GetNumVideoDrivers() { GENFUNC(GetNumVideoDrivers) }
    void GetOriginalMemoryFunctions(SDL_malloc_func *malloc_func, SDL_calloc_func *calloc_func, SDL_realloc_func *realloc_func, SDL_free_func *free_func) { GENFUNC(GetOriginalMemoryFunctions, malloc_func, calloc_func, realloc_func, free_func) }
    
    Uint64 GetPerformanceCounter() { GENFUNC(GetPerformanceCounter) }
    
    Uint64 GetPerformanceFrequency() { GENFUNC(GetPerformanceFrequency) }
    
    const char* GetPixelFormatName(Uint32 format) { GENFUNC(GetPixelFormatName, format) }
    
    const char * GetPlatform() { GENFUNC(GetPlatform) }
    
    int GetPointDisplayIndex(const SDL_Point * point) { GENFUNC(GetPointDisplayIndex, point) }

    SDL_PowerState GetPowerInfo(int *seconds, int *percent) { GENFUNC(GetPowerInfo, seconds, percent) }
    
    SDL_Locale * GetPreferredLocales() { GENFUNC(GetPreferredLocales) }

    char* GetPrefPath(const char *org, const char *app) { GENFUNC(GetPrefPath, org, app) }
    
    char * GetPrimarySelectionText() { GENFUNC(GetPrimarySelectionText) }
    
    Uint32 GetQueuedAudioSize(SDL_AudioDeviceID dev) { GENFUNC(GetQueuedAudioSize, dev) }
    
    int GetRectDisplayIndex(const SDL_Rect * rect) { GENFUNC(GetRectDisplayIndex, rect) }
    
    bool GetRelativeMouseMode() { GENFUNC(GetRelativeMouseMode) }

    Uint32 GetRelativeMouseState(int *x, int *y) { GENFUNC(GetRelativeMouseState, x, y) }

    int GetRenderDrawBlendMode(SDL_Renderer * renderer, SDL_BlendMode *blendMode) { GENFUNC(GetRenderDrawBlendMode, renderer, blendMode) }

    int GetRenderDrawColor(SDL_Renderer * renderer, Uint8 * r, Uint8 * g, Uint8 * b, Uint8 * a) { GENFUNC(GetRenderDrawColor, renderer, r, g, b, a) }

    int GetRenderDriverInfo(int index, SDL_RendererInfo * info) { GENFUNC(GetRenderDriverInfo, index, info) }
    
    SDL_Renderer * GetRenderer(SDL_Window * window) { GENFUNC(GetRenderer, window) }

    int GetRendererInfo(SDL_Renderer * renderer, SDL_RendererInfo * info) { GENFUNC(GetRendererInfo, renderer, info) }

    int GetRendererOutputSize(SDL_Renderer * renderer, int *w, int *h) { GENFUNC(GetRendererOutputSize, renderer, w, h) }
    
    SDL_Texture * GetRenderTarget(SDL_Renderer *renderer) { GENFUNC(GetRenderTarget, renderer) }
    
    const char* GetRevision() { GENFUNC(GetRevision) }
    
    int GetRevisionNumber() { GENFUNC(GetRevisionNumber) }

    void GetRGB(Uint32 pixel, const SDL_PixelFormat * format, Uint8 * r, Uint8 * g, Uint8 * b) { GENFUNC(GetRGB, pixel, format, r, g, b) }

    void GetRGBA(Uint32 pixel, const SDL_PixelFormat * format, Uint8 * r, Uint8 * g, Uint8 * b, Uint8 * a) { GENFUNC(GetRGBA, pixel, format, r, g, b, a) }
    
    SDL_Scancode GetScancodeFromKey(SDL_Keycode key) { GENFUNC(GetScancodeFromKey, key) }
    
    SDL_Scancode GetScancodeFromName(const char *name) { GENFUNC(GetScancodeFromName, name) }
    
    const char* GetScancodeName(SDL_Scancode scancode) { GENFUNC(GetScancodeName, scancode) }

    int GetShapedWindowMode(SDL_Window *window,SDL_WindowShapeMode *shape_mode) { GENFUNC(GetShapedWindowMode, window, shape_mode) }

    int GetSurfaceAlphaMod(SDL_Surface * surface, Uint8 * alpha) { GENFUNC(GetSurfaceAlphaMod, surface, alpha) }

    int GetSurfaceBlendMode(SDL_Surface * surface, SDL_BlendMode *blendMode) { GENFUNC(GetSurfaceBlendMode, surface, blendMode) }

    int GetSurfaceColorMod(SDL_Surface * surface, Uint8 * r, Uint8 * g, Uint8 * b) { GENFUNC(GetSurfaceColorMod, surface, r, g, b) }
    
    int GetSystemRAM() { GENFUNC(GetSystemRAM) }

    int GetTextureAlphaMod(SDL_Texture * texture, Uint8 * alpha) { GENFUNC(GetTextureAlphaMod, texture, alpha) }

    int GetTextureBlendMode(SDL_Texture * texture, SDL_BlendMode *blendMode) { GENFUNC(GetTextureBlendMode, texture, blendMode) }

    int GetTextureColorMod(SDL_Texture * texture, Uint8 * r, Uint8 * g, Uint8 * b) { GENFUNC(GetTextureColorMod, texture, r, g, b) }

    int GetTextureScaleMode(SDL_Texture * texture, SDL_ScaleMode *scaleMode) { GENFUNC(GetTextureScaleMode, texture, scaleMode) }
    
    void * GetTextureUserData(SDL_Texture * texture) { GENFUNC(GetTextureUserData, texture) }
    
    SDL_threadID GetThreadID(SDL_Thread * thread) { GENFUNC(GetThreadID, thread) }
    
    const char* GetThreadName(SDL_Thread *thread) { GENFUNC(GetThreadName, thread) }
    
    Uint32 GetTicks() { GENFUNC(GetTicks) }
    
    Uint64 GetTicks64() { GENFUNC(GetTicks64) }
    
    SDL_TouchID GetTouchDevice(int index) { GENFUNC(GetTouchDevice, index) }
    SDL_TouchDeviceType GetTouchDeviceType(SDL_TouchID touchID) { GENFUNC(GetTouchDeviceType, touchID) }

    SDL_Finger * GetTouchFinger(SDL_TouchID touchID, int index) { GENFUNC(GetTouchFinger, touchID, index) }
    const char* GetTouchName(int index) { GENFUNC(GetTouchName, index) }
    
    void GetVersion(SDL_version * ver) { GENFUNC(GetVersion, ver) }
    
    const char* GetVideoDriver(int index) { GENFUNC(GetVideoDriver, index) }

    int GetWindowBordersSize(SDL_Window * window, int *top, int *left, int *bottom, int *right) { GENFUNC(GetWindowBordersSize, window, top, left, bottom, right) }
    
    float GetWindowBrightness(SDL_Window * window) { GENFUNC(GetWindowBrightness, window) }

    void* GetWindowData(SDL_Window * window, const char *name) { GENFUNC(GetWindowData, window, name) }
    
    int GetWindowDisplayIndex(SDL_Window * window) { GENFUNC(GetWindowDisplayIndex, window) }

    int GetWindowDisplayMode(SDL_Window * window, SDL_DisplayMode * mode) { GENFUNC(GetWindowDisplayMode, window, mode) }
    
    Uint32 GetWindowFlags(SDL_Window * window) { GENFUNC(GetWindowFlags, window) }
    
    SDL_Window * GetWindowFromID(Uint32 id) { GENFUNC(GetWindowFromID, id) }

    int GetWindowGammaRamp(SDL_Window * window, uint16_t * red, uint16_t * green, uint16_t * blue) { GENFUNC(GetWindowGammaRamp, window, red, green, blue) }
    
    bool GetWindowGrab(SDL_Window * window) { GENFUNC(GetWindowGrab, window) }

    void* GetWindowICCProfile(SDL_Window * window, size_t* size) { GENFUNC(GetWindowICCProfile, window, size) }
    
    Uint32 GetWindowID(SDL_Window * window) { GENFUNC(GetWindowID, window) }
    
    bool GetWindowKeyboardGrab(SDL_Window * window) { GENFUNC(GetWindowKeyboardGrab, window) }

    void GetWindowMaximumSize(SDL_Window * window, int *w, int *h) { GENFUNC(GetWindowMaximumSize, window, w, h) }

    void GetWindowMinimumSize(SDL_Window * window, int *w, int *h) { GENFUNC(GetWindowMinimumSize, window, w, h) }
    
    bool GetWindowMouseGrab(SDL_Window * window) { GENFUNC(GetWindowMouseGrab, window) }
    
    const SDL_Rect * GetWindowMouseRect(SDL_Window * window) { GENFUNC(GetWindowMouseRect, window) }

    int GetWindowOpacity(SDL_Window * window, float * out_opacity) { GENFUNC(GetWindowOpacity, window, out_opacity) }
    
    Uint32 GetWindowPixelFormat(SDL_Window * window) { GENFUNC(GetWindowPixelFormat, window) }

    void GetWindowPosition(SDL_Window * window, int *x, int *y) { GENFUNC(GetWindowPosition, window, x, y) }

    void GetWindowSize(SDL_Window * window, int *w, int *h) { GENFUNC(GetWindowSize, window, w, h) }

    void GetWindowSizeInPixels(SDL_Window * window, int *w, int *h) { GENFUNC(GetWindowSizeInPixels, window, w, h) }
    
    SDL_Surface * GetWindowSurface(SDL_Window * window) { GENFUNC(GetWindowSurface, window) }
    
    const char* GetWindowTitle(SDL_Window * window) { GENFUNC(GetWindowTitle, window) }
    SDL_YUV_CONVERSION_MODE GetYUVConversionMode() { GENFUNC(GetYUVConversionMode) }
    SDL_YUV_CONVERSION_MODE GetYUVConversionModeForResolution(int width, int height) { GENFUNC(GetYUVConversionModeForResolution, width, height) }

    int GL_BindTexture(SDL_Texture *texture, float *texw, float *texh) { GENFUNC(GL_BindTexture, texture, texw, texh) }
    
    SDL_GLContext GL_CreateContext(SDL_Window * window) { GENFUNC(GL_CreateContext, window) }
    
    void GL_DeleteContext(SDL_GLContext context) { GENFUNC(GL_DeleteContext, context) }
    
    bool GL_ExtensionSupported(const char *extension) { GENFUNC(GL_ExtensionSupported, extension) }

    int GL_GetAttribute(SDL_GLattr attr, int *value) { GENFUNC(GL_GetAttribute, attr, value) }
    
    SDL_GLContext GL_GetCurrentContext() { GENFUNC(GL_GetCurrentContext) }
    
    SDL_Window* GL_GetCurrentWindow() { GENFUNC(GL_GetCurrentWindow) }

    void GL_GetDrawableSize(SDL_Window * window, int *w, int *h) { GENFUNC(GL_GetDrawableSize, window, w, h) }
    
    void* GL_GetProcAddress(const char *proc) { GENFUNC(GL_GetProcAddress, proc) }
    
    int GL_GetSwapInterval() { GENFUNC(GL_GetSwapInterval) }
    
    int GL_LoadLibrary(const char *path) { GENFUNC(GL_LoadLibrary, path) }

    int GL_MakeCurrent(SDL_Window * window, SDL_GLContext context) { GENFUNC(GL_MakeCurrent, window, context) }
    void GL_ResetAttributes() { GENFUNC(GL_ResetAttributes) }

    int GL_SetAttribute(SDL_GLattr attr, int value) { GENFUNC(GL_SetAttribute, attr, value) }
    
    int GL_SetSwapInterval(int interval) { GENFUNC(GL_SetSwapInterval, interval) }
    
    void GL_SwapWindow(SDL_Window * window) { GENFUNC(GL_SwapWindow, window) }
    
    int GL_UnbindTexture(SDL_Texture *texture) { GENFUNC(GL_UnbindTexture, texture) }
    void GL_UnloadLibrary() { GENFUNC(GL_UnloadLibrary) }
    
    SDL_GUID GUIDFromString(const char *pchGUID) { GENFUNC(GUIDFromString, pchGUID) }

    void GUIDToString(SDL_GUID guid, char *pszGUID, int cbGUID) { GENFUNC(GUIDToString, guid, pszGUID, cbGUID) }
    
    void HapticClose(SDL_Haptic * haptic) { GENFUNC(HapticClose, haptic) }
    
    void HapticDestroyEffect(SDL_Haptic * haptic, int effect) { GENFUNC(HapticDestroyEffect, haptic, effect) }

    int HapticEffectSupported(SDL_Haptic * haptic, SDL_HapticEffect * effect) { GENFUNC(HapticEffectSupported, haptic, effect) }

    int HapticGetEffectStatus(SDL_Haptic * haptic, int effect) { GENFUNC(HapticGetEffectStatus, haptic, effect) }
    
    int HapticIndex(SDL_Haptic * haptic) { GENFUNC(HapticIndex, haptic) }
    
    const char* HapticName(int device_index) { GENFUNC(HapticName, device_index) }

    int HapticNewEffect(SDL_Haptic * haptic, SDL_HapticEffect * effect) { GENFUNC(HapticNewEffect, haptic, effect) }
    
    int HapticNumAxes(SDL_Haptic * haptic) { GENFUNC(HapticNumAxes, haptic) }
    
    int HapticNumEffects(SDL_Haptic * haptic) { GENFUNC(HapticNumEffects, haptic) }
    
    int HapticNumEffectsPlaying(SDL_Haptic * haptic) { GENFUNC(HapticNumEffectsPlaying, haptic) }
    
    SDL_Haptic* HapticOpen(int device_index) { GENFUNC(HapticOpen, device_index) }
    
    int HapticOpened(int device_index) { GENFUNC(HapticOpened, device_index) }
    
    SDL_Haptic* HapticOpenFromJoystick(SDL_Joystick * joystick) { GENFUNC(HapticOpenFromJoystick, joystick) }
    
    SDL_Haptic* HapticOpenFromMouse() { GENFUNC(HapticOpenFromMouse) }
    
    int HapticPause(SDL_Haptic * haptic) { GENFUNC(HapticPause, haptic) }
    
    unsigned int HapticQuery(SDL_Haptic * haptic) { GENFUNC(HapticQuery, haptic) }
    
    int HapticRumbleInit(SDL_Haptic * haptic) { GENFUNC(HapticRumbleInit, haptic) }

    int HapticRumblePlay(SDL_Haptic * haptic, float strength, Uint32 length ) { GENFUNC(HapticRumblePlay, haptic, strength, length) }
    
    int HapticRumbleStop(SDL_Haptic * haptic) { GENFUNC(HapticRumbleStop, haptic) }
    
    int HapticRumbleSupported(SDL_Haptic * haptic) { GENFUNC(HapticRumbleSupported, haptic) }

    int HapticRunEffect(SDL_Haptic * haptic, int effect, Uint32 iterations) { GENFUNC(HapticRunEffect, haptic, effect, iterations) }

    int HapticSetAutocenter(SDL_Haptic * haptic, int autocenter) { GENFUNC(HapticSetAutocenter, haptic, autocenter) }

    int HapticSetGain(SDL_Haptic * haptic, int gain) { GENFUNC(HapticSetGain, haptic, gain) }
    
    int HapticStopAll(SDL_Haptic * haptic) { GENFUNC(HapticStopAll, haptic) }

    int HapticStopEffect(SDL_Haptic * haptic, int effect) { GENFUNC(HapticStopEffect, haptic, effect) }
    
    int HapticUnpause(SDL_Haptic * haptic) { GENFUNC(HapticUnpause, haptic) }

    int HapticUpdateEffect(SDL_Haptic * haptic, int effect, SDL_HapticEffect * data) { GENFUNC(HapticUpdateEffect, haptic, effect, data) }
    
    bool Has3DNow() { GENFUNC(Has3DNow) }
    
    bool HasAltiVec() { GENFUNC(HasAltiVec) }
    
    bool HasARMSIMD() { GENFUNC(HasARMSIMD) }
    
    bool HasAVX() { GENFUNC(HasAVX) }
    
    bool HasAVX2() { GENFUNC(HasAVX2) }
    
    bool HasAVX512F() { GENFUNC(HasAVX512F) }
    
    bool HasClipboardText() { GENFUNC(HasClipboardText) }
    
    bool HasColorKey(SDL_Surface * surface) { GENFUNC(HasColorKey, surface) }
    
    bool HasEvent(Uint32 type) { GENFUNC(HasEvent, type) }

    bool HasEvents(Uint32 minType, Uint32 maxType) { GENFUNC(HasEvents, minType, maxType) }

    bool HasIntersection(const SDL_Rect * A, const SDL_Rect * B) { GENFUNC(HasIntersection, A, B) }

    bool HasIntersectionF(const SDL_FRect * A, const SDL_FRect * B) { GENFUNC(HasIntersectionF, A, B) }
    
    bool HasLASX() { GENFUNC(HasLASX) }
    
    bool HasLSX() { GENFUNC(HasLSX) }
    
    bool HasMMX() { GENFUNC(HasMMX) }
    
    bool HasNEON() { GENFUNC(HasNEON) }
    
    bool HasPrimarySelectionText() { GENFUNC(HasPrimarySelectionText) }
    
    bool HasRDTSC() { GENFUNC(HasRDTSC) }
    
    bool HasScreenKeyboardSupport() { GENFUNC(HasScreenKeyboardSupport) }
    
    bool HasSSE() { GENFUNC(HasSSE) }
    
    bool HasSSE2() { GENFUNC(HasSSE2) }
    
    bool HasSSE3() { GENFUNC(HasSSE3) }
    
    bool HasSSE41() { GENFUNC(HasSSE41) }
    
    bool HasSSE42() { GENFUNC(HasSSE42) }
    
    bool HasSurfaceRLE(SDL_Surface * surface) { GENFUNC(HasSurfaceRLE, surface) }
    
    bool HasWindowSurface(SDL_Window *window) { GENFUNC(HasWindowSurface, window) }
    
    void hid_ble_scan(bool active) { GENFUNC(hid_ble_scan, active) }
    
    void hid_close(SDL_hid_device *dev) { GENFUNC(hid_close, dev) }
    
    Uint32 hid_device_change_count() { GENFUNC(hid_device_change_count) }

    SDL_hid_device_info * hid_enumerate(unsigned short vendor_id, unsigned short product_id) { GENFUNC(hid_enumerate, vendor_id, product_id) }
    
    int hid_exit() { GENFUNC(hid_exit) }
    
    void hid_free_enumeration(SDL_hid_device_info *devs) { GENFUNC(hid_free_enumeration, devs) }

    int hid_get_feature_report(SDL_hid_device *dev, unsigned char *data, size_t length) { GENFUNC(hid_get_feature_report, dev, data, length) }

    int hid_get_indexed_string(SDL_hid_device *dev, int string_index, wchar_t *string, size_t maxlen) { GENFUNC(hid_get_indexed_string, dev, string_index, string, maxlen) }

    int hid_get_manufacturer_string(SDL_hid_device *dev, wchar_t *string, size_t maxlen) { GENFUNC(hid_get_manufacturer_string, dev, string, maxlen) }

    int hid_get_product_string(SDL_hid_device *dev, wchar_t *string, size_t maxlen) { GENFUNC(hid_get_product_string, dev, string, maxlen) }

    int hid_get_serial_number_string(SDL_hid_device *dev, wchar_t *string, size_t maxlen) { GENFUNC(hid_get_serial_number_string, dev, string, maxlen) }
    
    int hid_init() { GENFUNC(hid_init) }

    SDL_hid_device * hid_open(unsigned short vendor_id, unsigned short product_id, const wchar_t *serial_number) { GENFUNC(hid_open, vendor_id, product_id, serial_number) }
    
    SDL_hid_device * hid_open_path(const char *path, int bExclusive /* = false */) { GENFUNC(hid_open_path, path, bExclusive) }

    int hid_read(SDL_hid_device *dev, unsigned char *data, size_t length) { GENFUNC(hid_read, dev, data, length) }

    int hid_read_timeout(SDL_hid_device *dev, unsigned char *data, size_t length, int milliseconds) { GENFUNC(hid_read_timeout, dev, data, length, milliseconds) }

    int hid_send_feature_report(SDL_hid_device *dev, const unsigned char *data, size_t length) { GENFUNC(hid_send_feature_report, dev, data, length) }

    int hid_set_nonblocking(SDL_hid_device *dev, int nonblock) { GENFUNC(hid_set_nonblocking, dev, nonblock) }

    int hid_write(SDL_hid_device *dev, const unsigned char *data, size_t length) { GENFUNC(hid_write, dev, data, length) }
    
    void HideWindow(SDL_Window * window) { GENFUNC(HideWindow, window) }
    char* iconv_string(const char *tocode, const char *fromcode, const char *inbuf, size_t inbytesleft) { GENFUNC(iconv_string, tocode, fromcode, inbuf, inbytesleft) }
    
    int InitSubSystem(Uint32 flags) { GENFUNC(InitSubSystem, flags) }

    bool IntersectFRect(const SDL_FRect * A, const SDL_FRect * B, SDL_FRect * result) { GENFUNC(IntersectFRect, A, B, result) }

    bool IntersectFRectAndLine(const SDL_FRect * rect, float *X1, float *Y1, float *X2, float *Y2) { GENFUNC(IntersectFRectAndLine, rect, X1, Y1, X2, Y2) }

    bool IntersectRect(const SDL_Rect * A, const SDL_Rect * B, SDL_Rect * result) { GENFUNC(IntersectRect, A, B, result) }

    bool IntersectRectAndLine(const SDL_Rect * rect, int *X1, int *Y1, int *X2, int *Y2) { GENFUNC(IntersectRectAndLine, rect, X1, Y1, X2, Y2) }
    
    bool IsGameController(int joystick_index) { GENFUNC(IsGameController, joystick_index) }
    
    bool IsScreenKeyboardShown(SDL_Window *window) { GENFUNC(IsScreenKeyboardShown, window) }
    
    bool IsScreenSaverEnabled() { GENFUNC(IsScreenSaverEnabled) }
    
    bool IsShapedWindow(const SDL_Window *window) { GENFUNC(IsShapedWindow, window) }
    
    bool IsTablet() { GENFUNC(IsTablet) }
    
    bool IsTextInputActive() { GENFUNC(IsTextInputActive) }
    bool IsTextInputShown() { GENFUNC(IsTextInputShown) }
    
    int JoystickAttachVirtual(SDL_JoystickType type, int naxes, int nbuttons, int nhats) { GENFUNC(JoystickAttachVirtual, type, naxes, nbuttons, nhats) }
    
    int JoystickAttachVirtualEx(const SDL_VirtualJoystickDesc *desc) { GENFUNC(JoystickAttachVirtualEx, desc) }
    
    void JoystickClose(SDL_Joystick *joystick) { GENFUNC(JoystickClose, joystick) }
    
    SDL_JoystickPowerLevel JoystickCurrentPowerLevel(SDL_Joystick *joystick) { GENFUNC(JoystickCurrentPowerLevel, joystick) }
    
    int JoystickDetachVirtual(int device_index) { GENFUNC(JoystickDetachVirtual, device_index) }
    
    int JoystickEventState(int state) { GENFUNC(JoystickEventState, state) }
    
    SDL_Joystick* JoystickFromInstanceID(SDL_JoystickID instance_id) { GENFUNC(JoystickFromInstanceID, instance_id) }
    
    SDL_Joystick* JoystickFromPlayerIndex(int player_index) { GENFUNC(JoystickFromPlayerIndex, player_index) }
    
    bool JoystickGetAttached(SDL_Joystick *joystick) { GENFUNC(JoystickGetAttached, joystick) }

    Sint16 JoystickGetAxis(SDL_Joystick *joystick, int axis) { GENFUNC(JoystickGetAxis, joystick, axis) }

    bool JoystickGetAxisInitialState(SDL_Joystick *joystick, int axis, Sint16 *state) { GENFUNC(JoystickGetAxisInitialState, joystick, axis, state) }

    int JoystickGetBall(SDL_Joystick *joystick, int ball, int *dx, int *dy) { GENFUNC(JoystickGetBall, joystick, ball, dx, dy) }

    Uint8 JoystickGetButton(SDL_Joystick *joystick, int button) { GENFUNC(JoystickGetButton, joystick, button) }
    
    SDL_JoystickGUID JoystickGetDeviceGUID(int device_index) { GENFUNC(JoystickGetDeviceGUID, device_index) }
    
    SDL_JoystickID JoystickGetDeviceInstanceID(int device_index) { GENFUNC(JoystickGetDeviceInstanceID, device_index) }
    int JoystickGetDevicePlayerIndex(int device_index) { GENFUNC(JoystickGetDevicePlayerIndex, device_index) }
    
    uint16_t JoystickGetDeviceProduct(int device_index) { GENFUNC(JoystickGetDeviceProduct, device_index) }
    
    uint16_t JoystickGetDeviceProductVersion(int device_index) { GENFUNC(JoystickGetDeviceProductVersion, device_index) }
    
    SDL_JoystickType JoystickGetDeviceType(int device_index) { GENFUNC(JoystickGetDeviceType, device_index) }
    
    uint16_t JoystickGetDeviceVendor(int device_index) { GENFUNC(JoystickGetDeviceVendor, device_index) }
    
    uint16_t JoystickGetFirmwareVersion(SDL_Joystick *joystick) { GENFUNC(JoystickGetFirmwareVersion, joystick) }
    
    SDL_JoystickGUID JoystickGetGUID(SDL_Joystick *joystick) { GENFUNC(JoystickGetGUID, joystick) }
    
    SDL_JoystickGUID JoystickGetGUIDFromString(const char *pchGUID) { GENFUNC(JoystickGetGUIDFromString, pchGUID) }

    void JoystickGetGUIDString(SDL_JoystickGUID guid, char *pszGUID, int cbGUID) { GENFUNC(JoystickGetGUIDString, guid, pszGUID, cbGUID) }

    Uint8 JoystickGetHat(SDL_Joystick *joystick, int hat) { GENFUNC(JoystickGetHat, joystick, hat) }
    
    int JoystickGetPlayerIndex(SDL_Joystick *joystick) { GENFUNC(JoystickGetPlayerIndex, joystick) }
    
    uint16_t JoystickGetProduct(SDL_Joystick *joystick) { GENFUNC(JoystickGetProduct, joystick) }
    
    uint16_t JoystickGetProductVersion(SDL_Joystick *joystick) { GENFUNC(JoystickGetProductVersion, joystick) }
    
    const char * JoystickGetSerial(SDL_Joystick *joystick) { GENFUNC(JoystickGetSerial, joystick) }
    
    SDL_JoystickType JoystickGetType(SDL_Joystick *joystick) { GENFUNC(JoystickGetType, joystick) }
    
    uint16_t JoystickGetVendor(SDL_Joystick *joystick) { GENFUNC(JoystickGetVendor, joystick) }
    
    bool JoystickHasLED(SDL_Joystick *joystick) { GENFUNC(JoystickHasLED, joystick) }
    
    bool JoystickHasRumble(SDL_Joystick *joystick) { GENFUNC(JoystickHasRumble, joystick) }
    
    bool JoystickHasRumbleTriggers(SDL_Joystick *joystick) { GENFUNC(JoystickHasRumbleTriggers, joystick) }
    
    SDL_JoystickID JoystickInstanceID(SDL_Joystick *joystick) { GENFUNC(JoystickInstanceID, joystick) }
    
    int JoystickIsHaptic(SDL_Joystick * joystick) { GENFUNC(JoystickIsHaptic, joystick) }
    
    bool JoystickIsVirtual(int device_index) { GENFUNC(JoystickIsVirtual, device_index) }
    
    const char* JoystickName(SDL_Joystick *joystick) { GENFUNC(JoystickName, joystick) }
    
    const char* JoystickNameForIndex(int device_index) { GENFUNC(JoystickNameForIndex, device_index) }
    
    int JoystickNumAxes(SDL_Joystick *joystick) { GENFUNC(JoystickNumAxes, joystick) }
    
    int JoystickNumBalls(SDL_Joystick *joystick) { GENFUNC(JoystickNumBalls, joystick) }
    
    int JoystickNumButtons(SDL_Joystick *joystick) { GENFUNC(JoystickNumButtons, joystick) }
    
    int JoystickNumHats(SDL_Joystick *joystick) { GENFUNC(JoystickNumHats, joystick) }
    
    SDL_Joystick* JoystickOpen(int device_index) { GENFUNC(JoystickOpen, device_index) }
    
    const char* JoystickPath(SDL_Joystick *joystick) { GENFUNC(JoystickPath, joystick) }
    
    const char* JoystickPathForIndex(int device_index) { GENFUNC(JoystickPathForIndex, device_index) }

    int JoystickRumble(SDL_Joystick *joystick, uint16_t low_frequency_rumble, uint16_t high_frequency_rumble, Uint32 duration_ms) { GENFUNC(JoystickRumble, joystick, low_frequency_rumble, high_frequency_rumble, duration_ms) }

    int JoystickRumbleTriggers(SDL_Joystick *joystick, uint16_t left_rumble, uint16_t right_rumble, Uint32 duration_ms) { GENFUNC(JoystickRumbleTriggers, joystick, left_rumble, right_rumble, duration_ms) }

    int JoystickSendEffect(SDL_Joystick *joystick, void *data, int size) { GENFUNC(JoystickSendEffect, joystick, data, size) }

    int JoystickSetLED(SDL_Joystick *joystick, Uint8 red, Uint8 green, Uint8 blue) { GENFUNC(JoystickSetLED, joystick, red, green, blue) }
    
    void JoystickSetPlayerIndex(SDL_Joystick *joystick, int player_index) { GENFUNC(JoystickSetPlayerIndex, joystick, player_index) }

    int JoystickSetVirtualAxis(SDL_Joystick *joystick, int axis, Sint16 value) { GENFUNC(JoystickSetVirtualAxis, joystick, axis, value) }

    int JoystickSetVirtualButton(SDL_Joystick *joystick, int button, Uint8 value) { GENFUNC(JoystickSetVirtualButton, joystick, button, value) }

    int JoystickSetVirtualHat(SDL_Joystick *joystick, int hat, Uint8 value) { GENFUNC(JoystickSetVirtualHat, joystick, hat, value) }
    void JoystickUpdate() { GENFUNC(JoystickUpdate) }

    int LinuxSetThreadPriority(Sint64 threadID, int priority) {
        if (not IsLinux()) return -1;
        return (reinterpret_cast<::LinuxSetThreadPriorityProto>(::LinuxSetThreadPriority))(threadID, priority);
    }

    int LinuxSetThreadPriorityAndPolicy(Sint64 threadID, int sdlPriority, int schedPolicy) {
        if (not IsLinux()) return -1;
        return (reinterpret_cast<::LinuxSetThreadPriorityAndPolicyProto>(::LinuxSetThreadPriorityAndPolicy))(threadID, sdlPriority, schedPolicy);
    }

    SDL_Surface* LoadBMP_RW(SDL_RWops * src, int freesrc) { GENFUNC(LoadBMP_RW, src, freesrc) }
    
    SDL_Surface* LoadBMP(const char* file) { return LoadBMP_RW(RWFromFile(file, "rb"), 1); }

    int LoadDollarTemplates(SDL_TouchID touchId, SDL_RWops *src) { GENFUNC(LoadDollarTemplates, touchId, src) }

    void* LoadFile(const char *file, size_t *datasize) { GENFUNC(LoadFile, file, datasize) }

    void* LoadFile_RW(SDL_RWops *src, size_t *datasize, int freesrc) { GENFUNC(LoadFile_RW, src, datasize, freesrc) }

    void* LoadFunction(void *handle, const char *name) { GENFUNC(LoadFunction, handle, name) }
    
    void* LoadObject(const char *sofile) { GENFUNC(LoadObject, sofile) }

    SDL_AudioSpec* LoadWAV_RW(SDL_RWops * src, int freesrc, SDL_AudioSpec * spec, Uint8 ** audio_buf, Uint32 * audio_len) { GENFUNC(LoadWAV_RW, src, freesrc, spec, audio_buf, audio_len) }

    SDL_AudioSpec* LoadWAV(const char* file, SDL_AudioSpec* spec, Uint8** audio_buf, Uint32* audio_len) { return LoadWAV_RW(RWFromFile(file, "rb"),1, spec,audio_buf,audio_len); }
    void LockAudio() { GENFUNC(LockAudio) }
    
    void LockAudioDevice(SDL_AudioDeviceID dev) { GENFUNC(LockAudioDevice, dev) }
    void LockJoysticks() { GENFUNC(LockJoysticks) }
    
    int LockMutex(SDL_mutex * mutex) { GENFUNC(LockMutex, mutex) }
    void LockSensors() { GENFUNC(LockSensors) }
    
    int LockSurface(SDL_Surface * surface) { GENFUNC(LockSurface, surface) }

    int LockTexture(SDL_Texture * texture, const SDL_Rect * rect, void **pixels, int *pitch) { GENFUNC(LockTexture, texture, rect, pixels, pitch) }

    int LockTextureToSurface(SDL_Texture *texture, const SDL_Rect *rect, SDL_Surface **surface) { GENFUNC(LockTextureToSurface, texture, rect, surface) }
    
    void LogGetOutputFunction(SDL_LogOutputFunction *callback, void **userdata) { GENFUNC(LogGetOutputFunction, callback, userdata) }
    
    SDL_LogPriority LogGetPriority(int category) { GENFUNC(LogGetPriority, category) }
    void LogResetPriorities() { GENFUNC(LogResetPriorities) }
    
    void LogSetAllPriority(SDL_LogPriority priority) { GENFUNC(LogSetAllPriority, priority) }
    
    void LogSetOutputFunction(SDL_LogOutputFunction callback, void *userdata) { GENFUNC(LogSetOutputFunction, callback, userdata) }
    
    void LogSetPriority(int category, SDL_LogPriority priority) { GENFUNC(LogSetPriority, category, priority) }

    int LowerBlit(SDL_Surface * src, SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect) { GENFUNC(LowerBlit, src, srcrect, dst, dstrect) }

    int LowerBlitScaled(SDL_Surface * src, SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect) { GENFUNC(LowerBlitScaled, src, srcrect, dst, dstrect) }

    Uint32 MapRGB(const SDL_PixelFormat * format, Uint8 r, Uint8 g, Uint8 b) { GENFUNC(MapRGB, format, r, g, b) }

    Uint32 MapRGBA(const SDL_PixelFormat * format, Uint8 r, Uint8 g, Uint8 b, Uint8 a) { GENFUNC(MapRGBA, format, r, g, b, a) }

    Uint32 MasksToPixelFormatEnum(int bpp, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask) { GENFUNC(MasksToPixelFormatEnum, bpp, Rmask, Gmask, Bmask, Amask) }
    
    void MaximizeWindow(SDL_Window * window) { GENFUNC(MaximizeWindow, window) }
    void MemoryBarrierReleaseFunction() { GENFUNC(MemoryBarrierReleaseFunction) }
    SDL_MetalView Metal_CreateView(SDL_Window * window) { GENFUNC(Metal_CreateView, window) }
    void Metal_DestroyView(SDL_MetalView view) { GENFUNC(Metal_DestroyView, view) }

    void Metal_GetDrawableSize(SDL_Window* window, int *w, int *h) { GENFUNC(Metal_GetDrawableSize, window, w, h) }
    void* Metal_GetLayer(SDL_MetalView view) { GENFUNC(Metal_GetLayer, view) }
    
    void MinimizeWindow(SDL_Window * window) { GENFUNC(MinimizeWindow, window) }

    void MixAudio(Uint8 * dst, const Uint8 * src, Uint32 len, int volume) { GENFUNC(MixAudio, dst, src, len, volume) }

    void MixAudioFormat(Uint8 * dst, const Uint8 * src, SDL_AudioFormat format, Uint32 len, int volume) { GENFUNC(MixAudioFormat, dst, src, format, len, volume) }
    
    int MostSignificantBitIndex32(uint32_t x) {
        if (x == 0) return -1;
        return 31 - __builtin_clz(x);
    }
    
    int MouseIsHaptic() { GENFUNC(MouseIsHaptic) }
    
    bool MUSTLOCK(SDL_Surface* surface) { return SDL_MUSTLOCK(surface); }

    SDL_AudioStream * NewAudioStream(const SDL_AudioFormat src_format, const Uint8 src_channels, const int src_rate, const SDL_AudioFormat dst_format, const Uint8 dst_channels, const int dst_rate) { GENFUNC(NewAudioStream, src_format, src_channels, src_rate, dst_format, dst_channels, dst_rate) }
    
    int NumHaptics() { GENFUNC(NumHaptics) }
    
    int NumJoysticks() { GENFUNC(NumJoysticks) }
    
    int NumSensors() { GENFUNC(NumSensors) }

    int OpenAudio(SDL_AudioSpec * desired, SDL_AudioSpec * obtained) { GENFUNC(OpenAudio, desired, obtained) }

    SDL_AudioDeviceID OpenAudioDevice( const char *device, int iscapture, const SDL_AudioSpec *desired, SDL_AudioSpec *obtained, int allowed_changes) { GENFUNC(OpenAudioDevice, device, iscapture, desired, obtained, allowed_changes) }
    
    int OpenURL(const char *url) { GENFUNC(OpenURL, url) }
    
    void PauseAudio(int pause_on) { GENFUNC(PauseAudio, pause_on) }
    
    void PauseAudioDevice(SDL_AudioDeviceID dev, int pause_on) { GENFUNC(PauseAudioDevice, dev, pause_on) }

    int PeepEvents(SDL_Event * events, int numevents, SDL_eventaction action, Uint32 minType, Uint32 maxType) { GENFUNC(PeepEvents, events, numevents, action, minType, maxType) }

    bool PixelFormatEnumToMasks(Uint32 format, int *bpp, Uint32 * Rmask, Uint32 * Gmask, Uint32 * Bmask, Uint32 * Amask) { GENFUNC(PixelFormatEnumToMasks, format, bpp, Rmask, Gmask, Bmask, Amask) }

    bool PointInFRect(const SDL_FPoint* p,  const SDL_FRect* r) { return PointInFRect(p, r); }

    bool PointInRect(const SDL_Point* p,  const SDL_Rect* r) { return SDL_PointInRect(p, r); }
    
    int PollEvent(SDL_Event * event) { GENFUNC(PollEvent, event) }

    int PremultiplyAlpha(int width, int height, Uint32 src_format, void * src, int src_pitch, Uint32 dst_format, void * dst, int dst_pitch) { GENFUNC(PremultiplyAlpha, width, height, src_format, src, src_pitch, dst_format, dst, dst_pitch) }
    void PumpEvents() { GENFUNC(PumpEvents) }
    
    int PushEvent(SDL_Event * event) { GENFUNC(PushEvent, event) }

    int QueryTexture(SDL_Texture * texture, Uint32 * format, int *access, int *w, int *h) { GENFUNC(QueryTexture, texture, format, access, w, h) }

    int QueueAudio(SDL_AudioDeviceID dev, void *data, Uint32 len) { GENFUNC(QueueAudio, dev, data, len) }
    
    bool QuitRequested() { PumpEvents(); return (PeepEvents(NULL,0,SDL_PEEKEVENT,SDL_QUIT,SDL_QUIT) > 0); }
    
    void QuitSubSystem(Uint32 flags) { GENFUNC(QuitSubSystem, flags) }
    
    void RaiseWindow(SDL_Window * window) { GENFUNC(RaiseWindow, window) }
    
    uint16_t ReadBE16(SDL_RWops * src) { GENFUNC(ReadBE16, src) }
    
    Uint32 ReadBE32(SDL_RWops * src) { GENFUNC(ReadBE32, src) }
    
    Uint64 ReadBE64(SDL_RWops * src) { GENFUNC(ReadBE64, src) }
    
    uint16_t ReadLE16(SDL_RWops * src) { GENFUNC(ReadLE16, src) }
    
    Uint32 ReadLE32(SDL_RWops * src) { GENFUNC(ReadLE32, src) }
    
    Uint64 ReadLE64(SDL_RWops * src) { GENFUNC(ReadLE64, src) }
    
    Uint8 ReadU8(SDL_RWops * src) { GENFUNC(ReadU8, src) }
    
    int RecordGesture(SDL_TouchID touchId) { GENFUNC(RecordGesture, touchId) }
    
    bool RectEmpty(const SDL_Rect* r) { return SDL_RectEmpty(r); }

    bool RectEquals(const SDL_Rect* a,  const SDL_Rect* b) { return SDL_RectEquals(a, b); }
    
    Uint32 RegisterEvents(int numevents) { GENFUNC(RegisterEvents, numevents) }
    
    bool RemoveTimer(SDL_TimerID id) { GENFUNC(RemoveTimer, id) }
    
    int RenderClear(SDL_Renderer * renderer) { GENFUNC(RenderClear, renderer) }

    int RenderCopy(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect) { GENFUNC(RenderCopy, renderer, texture, srcrect, dstrect) }

    int RenderCopyEx(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect, const double angle, const SDL_Point *center, const SDL_RendererFlip flip) { GENFUNC(RenderCopyEx, renderer, texture, srcrect, dstrect, angle, center, flip) }

    int RenderCopyExF(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_FRect * dstrect, const double angle, const SDL_FPoint *center, const SDL_RendererFlip flip) { GENFUNC(RenderCopyExF, renderer, texture, srcrect, dstrect, angle, center, flip) }

    int RenderCopyF(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_FRect * dstrect) { GENFUNC(RenderCopyF, renderer, texture, srcrect, dstrect) }

    int RenderDrawLine(SDL_Renderer * renderer, int x1, int y1, int x2, int y2) { GENFUNC(RenderDrawLine, renderer, x1, y1, x2, y2) }

    int RenderDrawLineF(SDL_Renderer * renderer, float x1, float y1, float x2, float y2) { GENFUNC(RenderDrawLineF, renderer, x1, y1, x2, y2) }

    int RenderDrawLines(SDL_Renderer * renderer, const SDL_Point * points, int count) { GENFUNC(RenderDrawLines, renderer, points, count) }

    int RenderDrawLinesF(SDL_Renderer * renderer, const SDL_FPoint * points, int count) { GENFUNC(RenderDrawLinesF, renderer, points, count) }

    int RenderDrawPoint(SDL_Renderer * renderer, int x, int y) { GENFUNC(RenderDrawPoint, renderer, x, y) }

    int RenderDrawPointF(SDL_Renderer * renderer, float x, float y) { GENFUNC(RenderDrawPointF, renderer, x, y) }

    int RenderDrawPoints(SDL_Renderer * renderer, const SDL_Point * points, int count) { GENFUNC(RenderDrawPoints, renderer, points, count) }

    int RenderDrawPointsF(SDL_Renderer * renderer, const SDL_FPoint * points, int count) { GENFUNC(RenderDrawPointsF, renderer, points, count) }

    int RenderDrawRect(SDL_Renderer * renderer, const SDL_Rect * rect) { GENFUNC(RenderDrawRect, renderer, rect) }

    int RenderDrawRectF(SDL_Renderer * renderer, const SDL_FRect * rect) { GENFUNC(RenderDrawRectF, renderer, rect) }

    int RenderDrawRects(SDL_Renderer * renderer, const SDL_Rect * rects, int count) { GENFUNC(RenderDrawRects, renderer, rects, count) }

    int RenderDrawRectsF(SDL_Renderer * renderer, const SDL_FRect * rects, int count) { GENFUNC(RenderDrawRectsF, renderer, rects, count) }

    int RenderFillRect(SDL_Renderer * renderer, const SDL_Rect * rect) { GENFUNC(RenderFillRect, renderer, rect) }

    int RenderFillRectF(SDL_Renderer * renderer, const SDL_FRect * rect) { GENFUNC(RenderFillRectF, renderer, rect) }

    int RenderFillRects(SDL_Renderer * renderer, const SDL_Rect * rects, int count) { GENFUNC(RenderFillRects, renderer, rects, count) }

    int RenderFillRectsF(SDL_Renderer * renderer, const SDL_FRect * rects, int count) { GENFUNC(RenderFillRectsF, renderer, rects, count) }
    
    int RenderFlush(SDL_Renderer * renderer) { GENFUNC(RenderFlush, renderer) }

    int RenderGeometry(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Vertex *vertices, int num_vertices, const int *indices, int num_indices) { GENFUNC(RenderGeometry, renderer, texture, vertices, num_vertices, indices, num_indices) }

    int RenderGeometryRaw(SDL_Renderer *renderer, SDL_Texture *texture, const float *xy, int xy_stride, const SDL_Color *color, int color_stride, const float *uv, int uv_stride, int num_vertices, void *indices, int num_indices, int size_indices) { GENFUNC(RenderGeometryRaw, renderer, texture, xy, xy_stride, color, color_stride, uv, uv_stride, num_vertices, indices, num_indices, size_indices) }
    
    void RenderGetClipRect(SDL_Renderer * renderer, SDL_Rect * rect) { GENFUNC(RenderGetClipRect, renderer, rect) }
    
    bool RenderGetIntegerScale(SDL_Renderer * renderer) { GENFUNC(RenderGetIntegerScale, renderer) }

    void RenderGetLogicalSize(SDL_Renderer * renderer, int *w, int *h) { GENFUNC(RenderGetLogicalSize, renderer, w, h) }
    
    void* RenderGetMetalCommandEncoder(SDL_Renderer * renderer) { GENFUNC(RenderGetMetalCommandEncoder, renderer) }
    
    void* RenderGetMetalLayer(SDL_Renderer * renderer) { GENFUNC(RenderGetMetalLayer, renderer) }

    void RenderGetScale(SDL_Renderer * renderer, float *scaleX, float *scaleY) { GENFUNC(RenderGetScale, renderer, scaleX, scaleY) }
    
    void RenderGetViewport(SDL_Renderer * renderer, SDL_Rect * rect) { GENFUNC(RenderGetViewport, renderer, rect) }
    
    SDL_Window * RenderGetWindow(SDL_Renderer *renderer) { GENFUNC(RenderGetWindow, renderer) }
    
    bool RenderIsClipEnabled(SDL_Renderer * renderer) { GENFUNC(RenderIsClipEnabled, renderer) }

    void RenderLogicalToWindow(SDL_Renderer * renderer, float logicalX, float logicalY, int *windowX, int *windowY) { GENFUNC(RenderLogicalToWindow, renderer, logicalX, logicalY, windowX, windowY) }
    
    void RenderPresent(SDL_Renderer * renderer) { GENFUNC(RenderPresent, renderer) }

    int RenderReadPixels(SDL_Renderer * renderer, const SDL_Rect * rect, Uint32 format, void *pixels, int pitch) { GENFUNC(RenderReadPixels, renderer, rect, format, pixels, pitch) }

    int RenderSetClipRect(SDL_Renderer * renderer, const SDL_Rect * rect) { GENFUNC(RenderSetClipRect, renderer, rect) }

    int RenderSetIntegerScale(SDL_Renderer * renderer, bool enable) { GENFUNC(RenderSetIntegerScale, renderer, enable) }

    int RenderSetLogicalSize(SDL_Renderer * renderer, int w, int h) { GENFUNC(RenderSetLogicalSize, renderer, w, h) }

    int RenderSetScale(SDL_Renderer * renderer, float scaleX, float scaleY) { GENFUNC(RenderSetScale, renderer, scaleX, scaleY) }

    int RenderSetViewport(SDL_Renderer * renderer, const SDL_Rect * rect) { GENFUNC(RenderSetViewport, renderer, rect) }

    int RenderSetVSync(SDL_Renderer* renderer, int vsync) { GENFUNC(RenderSetVSync, renderer, vsync) }
    
    bool RenderTargetSupported(SDL_Renderer *renderer) { GENFUNC(RenderTargetSupported, renderer) }

    void RenderWindowToLogical(SDL_Renderer * renderer, int windowX, int windowY, float *logicalX, float *logicalY) { GENFUNC(RenderWindowToLogical, renderer, windowX, windowY, logicalX, logicalY) }
    void ResetAssertionReport() { GENFUNC(ResetAssertionReport) }
    
    bool ResetHint(const char *name) { GENFUNC(ResetHint, name) }
    void ResetHints() { GENFUNC(ResetHints) }
    void ResetKeyboard() { GENFUNC(ResetKeyboard) }
    
    void RestoreWindow(SDL_Window * window) { GENFUNC(RestoreWindow, window) }
    
    int RWclose(SDL_RWops *context) { GENFUNC(RWclose, context) }

    SDL_RWops* RWFromConstMem(void *mem, int size) { GENFUNC(RWFromConstMem, mem, size) }

    SDL_RWops* RWFromFP(void * fp, bool autoclose) { GENFUNC(RWFromFP, fp, autoclose) }

    SDL_RWops* RWFromMem(void *mem, int size) { GENFUNC(RWFromMem, mem, size) }

    size_t RWread(SDL_RWops *context, void *ptr, size_t size, size_t maxnum) { GENFUNC(RWread, context, ptr, size, maxnum) }

    Sint64 RWseek(SDL_RWops *context, Sint64 offset, int whence) { GENFUNC(RWseek, context, offset, whence) }
    
    Sint64 RWsize(SDL_RWops *context) { GENFUNC(RWsize, context) }
    
    Sint64 RWtell(SDL_RWops *context) { GENFUNC(RWtell, context) }

    size_t RWwrite(SDL_RWops *context, void *ptr, size_t size, size_t num) { GENFUNC(RWwrite, context, ptr, size, num) }
    
    int SaveAllDollarTemplates(SDL_RWops *dst) { GENFUNC(SaveAllDollarTemplates, dst) }

    int SaveBMP_RW(SDL_Surface * surface, SDL_RWops * dst, int freedst) { GENFUNC(SaveBMP_RW, surface, dst, freedst) }

    int SaveBMP(SDL_Surface* surface,  const char* file) { return SaveBMP_RW(surface, RWFromFile(file, "wb"), 1); }

    int SaveDollarTemplate(SDL_GestureID gestureId,SDL_RWops *dst) { GENFUNC(SaveDollarTemplate, gestureId, dst) }
    
    int SemPost(SDL_sem * sem) { GENFUNC(SemPost, sem) }
    
    int SemTryWait(SDL_sem * sem) { GENFUNC(SemTryWait, sem) }
    
    Uint32 SemValue(SDL_sem * sem) { GENFUNC(SemValue, sem) }
    
    int SemWait(SDL_sem * sem) { GENFUNC(SemWait, sem) }

    int SemWaitTimeout(SDL_sem *sem, Uint32 timeout) { GENFUNC(SemWaitTimeout, sem, timeout) }
    
    void SensorClose(SDL_Sensor *sensor) { GENFUNC(SensorClose, sensor) }
    
    SDL_Sensor* SensorFromInstanceID(SDL_SensorID instance_id) { GENFUNC(SensorFromInstanceID, instance_id) }

    int SensorGetData(SDL_Sensor *sensor, float *data, int num_values) { GENFUNC(SensorGetData, sensor, data, num_values) }

    int SensorGetDataWithTimestamp(SDL_Sensor *sensor, Uint64 *timestamp, float *data, int num_values) { GENFUNC(SensorGetDataWithTimestamp, sensor, timestamp, data, num_values) }
    
    SDL_SensorID SensorGetDeviceInstanceID(int device_index) { GENFUNC(SensorGetDeviceInstanceID, device_index) }
    
    const char* SensorGetDeviceName(int device_index) { GENFUNC(SensorGetDeviceName, device_index) }
    
    int SensorGetDeviceNonPortableType(int device_index) { GENFUNC(SensorGetDeviceNonPortableType, device_index) }
    
    SDL_SensorType SensorGetDeviceType(int device_index) { GENFUNC(SensorGetDeviceType, device_index) }
    
    SDL_SensorID SensorGetInstanceID(SDL_Sensor *sensor) { GENFUNC(SensorGetInstanceID, sensor) }
    
    const char* SensorGetName(SDL_Sensor *sensor) { GENFUNC(SensorGetName, sensor) }
    
    int SensorGetNonPortableType(SDL_Sensor *sensor) { GENFUNC(SensorGetNonPortableType, sensor) }
    
    SDL_SensorType SensorGetType(SDL_Sensor *sensor) { GENFUNC(SensorGetType, sensor) }
    
    SDL_Sensor* SensorOpen(int device_index) { GENFUNC(SensorOpen, device_index) }
    void SensorUpdate() { GENFUNC(SensorUpdate) }
    
    void SetAssertionHandler( SDL_AssertionHandler handler, void *userdata) { GENFUNC(SetAssertionHandler, handler, userdata) }
    
    int SetClipboardText(const char *text) { GENFUNC(SetClipboardText, text) }

    bool SetClipRect(SDL_Surface * surface, const SDL_Rect * rect) { GENFUNC(SetClipRect, surface, rect) }

    int SetColorKey(SDL_Surface * surface, int flag, Uint32 key) { GENFUNC(SetColorKey, surface, flag, key) }
    
    void SetCursor(SDL_Cursor * cursor) { GENFUNC(SetCursor, cursor) }
    
    void SetEventFilter(SDL_EventFilter filter, void *userdata) { GENFUNC(SetEventFilter, filter, userdata) }

    bool SetHint(const char *name, const char *value) { GENFUNC(SetHint, name, value) }

    bool SetHintWithPriority(const char *name, const char *value, SDL_HintPriority priority) { GENFUNC(SetHintWithPriority, name, value, priority) }
    void SetMainReady() { GENFUNC(SetMainReady) }
    int SetMemoryFunctions(SDL_malloc_func malloc_func, SDL_calloc_func calloc_func, SDL_realloc_func realloc_func, SDL_free_func free_func) { GENFUNC(SetMemoryFunctions, malloc_func, calloc_func, realloc_func, free_func) }
    
    void SetModState(SDL_Keymod modstate) { GENFUNC(SetModState, modstate) }

    int SetPaletteColors(SDL_Palette * palette, const SDL_Color * colors, int firstcolor, int ncolors) { GENFUNC(SetPaletteColors, palette, colors, firstcolor, ncolors) }

    int SetPixelFormatPalette(SDL_PixelFormat * format, SDL_Palette *palette) { GENFUNC(SetPixelFormatPalette, format, palette) }
    
    int SetPrimarySelectionText(const char *text) { GENFUNC(SetPrimarySelectionText, text) }
    
    int SetRelativeMouseMode(bool enabled) { GENFUNC(SetRelativeMouseMode, enabled) }

    int SetRenderDrawBlendMode(SDL_Renderer * renderer, SDL_BlendMode blendMode) { GENFUNC(SetRenderDrawBlendMode, renderer, blendMode) }

    int SetRenderDrawColor(SDL_Renderer * renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a) { GENFUNC(SetRenderDrawColor, renderer, r, g, b, a) }

    int SetRenderTarget(SDL_Renderer *renderer, SDL_Texture *texture) { GENFUNC(SetRenderTarget, renderer, texture) }

    int SetSurfaceAlphaMod(SDL_Surface * surface, Uint8 alpha) { GENFUNC(SetSurfaceAlphaMod, surface, alpha) }

    int SetSurfaceBlendMode(SDL_Surface * surface, SDL_BlendMode blendMode) { GENFUNC(SetSurfaceBlendMode, surface, blendMode) }

    int SetSurfaceColorMod(SDL_Surface * surface, Uint8 r, Uint8 g, Uint8 b) { GENFUNC(SetSurfaceColorMod, surface, r, g, b) }

    int SetSurfacePalette(SDL_Surface * surface, SDL_Palette * palette) { GENFUNC(SetSurfacePalette, surface, palette) }

    int SetSurfaceRLE(SDL_Surface * surface, int flag) { GENFUNC(SetSurfaceRLE, surface, flag) }
    
    void SetTextInputRect(const SDL_Rect *rect) { GENFUNC(SetTextInputRect, rect) }

    int SetTextureAlphaMod(SDL_Texture * texture, Uint8 alpha) { GENFUNC(SetTextureAlphaMod, texture, alpha) }

    int SetTextureBlendMode(SDL_Texture * texture, SDL_BlendMode blendMode) { GENFUNC(SetTextureBlendMode, texture, blendMode) }

    int SetTextureColorMod(SDL_Texture * texture, Uint8 r, Uint8 g, Uint8 b) { GENFUNC(SetTextureColorMod, texture, r, g, b) }

    int SetTextureScaleMode(SDL_Texture * texture, SDL_ScaleMode scaleMode) { GENFUNC(SetTextureScaleMode, texture, scaleMode) }

    int SetTextureUserData(SDL_Texture * texture, void *userdata) { GENFUNC(SetTextureUserData, texture, userdata) }
    
    int SetThreadPriority(SDL_ThreadPriority priority) { GENFUNC(SetThreadPriority, priority) }
    
    void SetWindowAlwaysOnTop(SDL_Window * window, bool on_top) { GENFUNC(SetWindowAlwaysOnTop, window, on_top) }
    
    void SetWindowBordered(SDL_Window * window, bool bordered) { GENFUNC(SetWindowBordered, window, bordered) }

    int SetWindowBrightness(SDL_Window * window, float brightness) { GENFUNC(SetWindowBrightness, window, brightness) }

    void* SetWindowData(SDL_Window * window, const char *name, void *userdata) { GENFUNC(SetWindowData, window, name, userdata) }

    int SetWindowDisplayMode(SDL_Window * window, const SDL_DisplayMode * mode) { GENFUNC(SetWindowDisplayMode, window, mode) }

    int SetWindowFullscreen(SDL_Window * window, Uint32 flags) { GENFUNC(SetWindowFullscreen, window, flags) }

    int SetWindowGammaRamp(SDL_Window * window, const uint16_t * red, const uint16_t * green, const uint16_t * blue) { GENFUNC(SetWindowGammaRamp, window, red, green, blue) }
    
    void SetWindowGrab(SDL_Window * window, bool grabbed) { GENFUNC(SetWindowGrab, window, grabbed) }

    int SetWindowHitTest(SDL_Window * window, SDL_HitTest callback, void *callback_data) { GENFUNC(SetWindowHitTest, window, callback, callback_data) }
    
    void SetWindowIcon(SDL_Window * window, SDL_Surface * icon) { GENFUNC(SetWindowIcon, window, icon) }
    
    int SetWindowInputFocus(SDL_Window * window) { GENFUNC(SetWindowInputFocus, window) }
    
    void SetWindowKeyboardGrab(SDL_Window * window, bool grabbed) { GENFUNC(SetWindowKeyboardGrab, window, grabbed) }

    void SetWindowMaximumSize(SDL_Window * window, int max_w, int max_h) { GENFUNC(SetWindowMaximumSize, window, max_w, max_h) }

    void SetWindowMinimumSize(SDL_Window * window, int min_w, int min_h) { GENFUNC(SetWindowMinimumSize, window, min_w, min_h) }

    int SetWindowModalFor(SDL_Window * modal_window, SDL_Window * parent_window) { GENFUNC(SetWindowModalFor, modal_window, parent_window) }
    
    void SetWindowMouseGrab(SDL_Window * window, bool grabbed) { GENFUNC(SetWindowMouseGrab, window, grabbed) }

    int SetWindowMouseRect(SDL_Window * window, const SDL_Rect * rect) { GENFUNC(SetWindowMouseRect, window, rect) }

    int SetWindowOpacity(SDL_Window * window, float opacity) { GENFUNC(SetWindowOpacity, window, opacity) }

    void SetWindowPosition(SDL_Window * window, int x, int y) { GENFUNC(SetWindowPosition, window, x, y) }
    
    void SetWindowResizable(SDL_Window * window, bool resizable) { GENFUNC(SetWindowResizable, window, resizable) }

    int SetWindowShape(SDL_Window *window,SDL_Surface *shape,SDL_WindowShapeMode *shape_mode) { GENFUNC(SetWindowShape, window, shape, shape_mode) }

    void SetWindowSize(SDL_Window * window, int w, int h) { GENFUNC(SetWindowSize, window, w, h) }
    
    void SetWindowTitle(SDL_Window * window, const char *title) { GENFUNC(SetWindowTitle, window, title) }
    void SetYUVConversionMode(SDL_YUV_CONVERSION_MODE mode) { GENFUNC(SetYUVConversionMode, mode) }
    
    int ShowCursor(int toggle) { GENFUNC(ShowCursor, toggle) }

    int ShowMessageBox(const SDL_MessageBoxData *messageboxdata, int *buttonid) { GENFUNC(ShowMessageBox, messageboxdata, buttonid) }

    int ShowSimpleMessageBox(Uint32 flags, const char *title, const char *message, SDL_Window *window) { GENFUNC(ShowSimpleMessageBox, flags, title, message, window) }
    
    void ShowWindow(SDL_Window * window) { GENFUNC(ShowWindow, window) }
    
    void * SIMDAlloc(const size_t len) { GENFUNC(SIMDAlloc, len) }
    
    void SIMDFree(void *ptr) { GENFUNC(SIMDFree, ptr) }
    
    size_t SIMDGetAlignment() { GENFUNC(SIMDGetAlignment) }

    void * SIMDRealloc(void *mem, const size_t len) { GENFUNC(SIMDRealloc, mem, len) }
    int SoftStretch(SDL_Surface * src, const SDL_Rect * srcrect, SDL_Surface * dst, const SDL_Rect * dstrect) { GENFUNC(SoftStretch, src, srcrect, dst, dstrect) }
    int SoftStretchLinear(SDL_Surface * src, const SDL_Rect * srcrect, SDL_Surface * dst, const SDL_Rect * dstrect) { GENFUNC(SoftStretchLinear, src, srcrect, dst, dstrect) }
    void StartTextInput() { GENFUNC(StartTextInput) }
    void StopTextInput() { GENFUNC(StopTextInput) }
    
    uint16_t Swap16(uint16_t x) { return SDL_Swap16(x); }
    
    uint32_t Swap32(uint32_t x) { return SDL_Swap32(x); }
    
    uint64_t Swap64(uint64_t x) { return SDL_Swap64(x); }
    
    uint16_t SwapBE16(uint16_t x) { return SDL_SwapBE16(x); }
    
    uint32_t SwapBE32(uint32_t x) { return SDL_SwapBE32(x); }
    
    uint64_t SwapBE64(uint64_t x) { return SDL_SwapBE64(x); }
    
    float SwapFloat(float x) { return SDL_SwapFloat(x); }
    
    float SwapFloatBE(float x) { return SDL_SwapFloatBE(x); }
    
    float SwapFloatLE(float x) { return SDL_SwapFloatLE(x); }
    
    uint16_t SwapLE16(uint16_t x) { return SDL_SwapLE16(x); }
    
    uint32_t SwapLE32(uint32_t x) { return SDL_SwapLE32(x); }
    
    uint64_t SwapLE64(uint64_t x) { return SDL_SwapLE64(x); }
    
    SDL_threadID ThreadID() { GENFUNC(ThreadID) }
    void TLSCleanup() { GENFUNC(TLSCleanup) }
    
    SDL_TLSID TLSCreate() { GENFUNC(TLSCreate) }
    
    void * TLSGet(SDL_TLSID id) { GENFUNC(TLSGet, id) }

    int TLSSet(SDL_TLSID id, void *value, void ( *destructor)(void*)) { GENFUNC(TLSSet, id, value, destructor) }
    void TriggerBreakpoint() { SDL_TriggerBreakpoint(); }
    
    int TryLockMutex(SDL_mutex * mutex ) { GENFUNC(TryLockMutex, mutex) }

    void UnionFRect(const SDL_FRect * A, const SDL_FRect * B, SDL_FRect * result) { GENFUNC(UnionFRect, A, B, result) }

    void UnionRect(const SDL_Rect * A, const SDL_Rect * B, SDL_Rect * result) { GENFUNC(UnionRect, A, B, result) }
    
    void UnloadObject(void *handle) { GENFUNC(UnloadObject, handle) }
    void UnlockAudio() { GENFUNC(UnlockAudio) }
    
    void UnlockAudioDevice(SDL_AudioDeviceID dev) { GENFUNC(UnlockAudioDevice, dev) }
    void UnlockJoysticks() { GENFUNC(UnlockJoysticks) }
    
    int UnlockMutex(SDL_mutex * mutex ) { GENFUNC(UnlockMutex, mutex) }
    
    void UnlockSurface(SDL_Surface * surface) { GENFUNC(UnlockSurface, surface) }
    
    void UnlockTexture(SDL_Texture * texture) { GENFUNC(UnlockTexture, texture) }

    int UpdateNVTexture(SDL_Texture * texture, const SDL_Rect * rect, const Uint8 *Yplane, int Ypitch, const Uint8 *UVplane, int UVpitch) { GENFUNC(UpdateNVTexture, texture, rect, Yplane, Ypitch, UVplane, UVpitch) }

    int UpdateTexture(SDL_Texture * texture, const SDL_Rect * rect, void *pixels, int pitch) { GENFUNC(UpdateTexture, texture, rect, pixels, pitch) }
    
    int UpdateWindowSurface(SDL_Window * window) { GENFUNC(UpdateWindowSurface, window) }

    int UpdateWindowSurfaceRects(SDL_Window * window, const SDL_Rect * rects, int numrects) { GENFUNC(UpdateWindowSurfaceRects, window, rects, numrects) }

    int UpdateYUVTexture(SDL_Texture * texture, const SDL_Rect * rect, const Uint8 *Yplane, int Ypitch, const Uint8 *Uplane, int Upitch, const Uint8 *Vplane, int Vpitch) { GENFUNC(UpdateYUVTexture, texture, rect, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch) }
    int UpperBlit(SDL_Surface * src, const SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect) { GENFUNC(UpperBlit, src, srcrect, dst, dstrect) }
    int UpperBlitScaled(SDL_Surface * src, const SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect) { GENFUNC(UpperBlitScaled, src, srcrect, dst, dstrect) }
    
    int VideoInit(const char *driver_name) { GENFUNC(VideoInit, driver_name) }
    void VideoQuit() { GENFUNC(VideoQuit) }
#ifdef COSMO_SDL2_VULKAN

    bool Vulkan_CreateSurface(SDL_Window *window, VkInstance instance, VkSurfaceKHR* surface) { GENFUNC(Vulkan_CreateSurface, window, instance, surface) }

    void Vulkan_GetDrawableSize(SDL_Window * window, int *w, int *h) { GENFUNC(Vulkan_GetDrawableSize, window, w, h) }

    bool Vulkan_GetInstanceExtensions(SDL_Window *window, unsigned int *pCount, const char **pNames) { GENFUNC(Vulkan_GetInstanceExtensions, window, pCount, pNames) }
    
    void* Vulkan_GetVkGetInstanceProcAddr() { GENFUNC(Vulkan_GetVkGetInstanceProcAddr) }
    
    void * SDL_Vulkan_GetVkGetInstanceProcAddr()Vulkan_GetVkInstanceProcAddr() { GENFUNC(Vulkan_GetVkInstanceProcAddr) }
    
    int Vulkan_LoadLibrary(const char *path) { GENFUNC(Vulkan_LoadLibrary, path) }
    void Vulkan_UnloadLibrary() { GENFUNC(Vulkan_UnloadLibrary) }
#endif
    
    int WaitEvent(SDL_Event * event) { GENFUNC(WaitEvent, event) }

    int WaitEventTimeout(SDL_Event * event, int timeout) { GENFUNC(WaitEventTimeout, event, timeout) }
    
    void WaitThread(SDL_Thread * thread, int *status) { GENFUNC(WaitThread, thread, status) }

    int WarpMouseGlobal(int x, int y) { GENFUNC(WarpMouseGlobal, x, y) }

    void WarpMouseInWindow(SDL_Window * window, int x, int y) { GENFUNC(WarpMouseInWindow, window, x, y) }
    
    Uint32 WasInit(Uint32 flags) { GENFUNC(WasInit, flags) }

    size_t WriteBE16(SDL_RWops * dst, Uint16 value) { GENFUNC(WriteBE16, dst, value) }

    size_t WriteBE32(SDL_RWops * dst, Uint32 value) { GENFUNC(WriteBE32, dst, value) }

    size_t WriteBE64(SDL_RWops * dst, Uint64 value) { GENFUNC(WriteBE64, dst, value) }

    size_t WriteLE16(SDL_RWops * dst, Uint16 value) { GENFUNC(WriteLE16, dst, value) }

    size_t WriteLE32(SDL_RWops * dst, Uint32 value) { GENFUNC(WriteLE32, dst, value) }

    size_t WriteLE64(SDL_RWops * dst, Uint64 value) { GENFUNC(WriteLE64, dst, value) }

    size_t WriteU8(SDL_RWops * dst, Uint8 value) { GENFUNC(WriteU8, dst, value) }
    namespace Net {

        int AddSocket(SDLNet_SocketSet set, SDLNet_GenericSocket sock) { GENFUNC(NetAddSocket, set, sock) }
        
        UDPpacket * AllocPacket(int size) { GENFUNC(NetAllocPacket, size) }

        UDPpacket ** AllocPacketV(int howmany, int size) { GENFUNC(NetAllocPacketV, howmany, size) }
        
        SDLNet_SocketSet AllocSocketSet(int maxsockets) { GENFUNC(NetAllocSocketSet, maxsockets) }

        int CheckSockets(SDLNet_SocketSet set, Uint32 timeout) { GENFUNC(NetCheckSockets, set, timeout) }

        int DelSocket(SDLNet_SocketSet set, SDLNet_GenericSocket sock) { GENFUNC(NetDelSocket, set, sock) }
        
        void FreePacket(UDPpacket *packet) { GENFUNC(NetFreePacket, packet) }
        
        void FreePacketV(UDPpacket **packetV) { GENFUNC(NetFreePacketV, packetV) }
        
        void FreeSocketSet(SDLNet_SocketSet set) { GENFUNC(NetFreeSocketSet, set) }
        
        const char * GetError() { GENFUNC(NetGetError) }

        int GetLocalAddresses(IPaddress *addresses, int maxcount) { GENFUNC(NetGetLocalAddresses, addresses, maxcount) }
        
        int Init() { GENFUNC(NetInit) }
        
        const SDLNet_version * Linked_Version() { GENFUNC(NetLinked_Version) }
        void Quit() { GENFUNC(NetQuit) }
        
        int ResizePacket(UDPpacket *packet, int newsize) { GENFUNC(NetResizePacket, packet, newsize) }

        int ResolveHost(IPaddress *address, const char *host, uint16_t port) { GENFUNC(NetResolveHost, address, host, port) }
        
        const char * ResolveIP(const IPaddress *ip) { GENFUNC(NetResolveIP, ip) }
        
        TCPsocket TCP_Accept(TCPsocket server) { GENFUNC(NetTCP_Accept, server) }
        
        void TCP_Close(TCPsocket sock) { GENFUNC(NetTCP_Close, sock) }
        
        IPaddress * TCP_GetPeerAddress(TCPsocket sock) { GENFUNC(NetTCP_GetPeerAddress, sock) }
        
        TCPsocket TCP_Open(IPaddress *ip) { GENFUNC(NetTCP_Open, ip) }

        int TCP_Recv(TCPsocket sock, void *data, int maxlen) { GENFUNC(NetTCP_Recv, sock, data, maxlen) }

        int TCP_Send(TCPsocket sock, void *data, int len) { GENFUNC(NetTCP_Send, sock, data, len) }

        int UDP_Bind(UDPsocket sock, int channel, const IPaddress *address) { GENFUNC(NetUDP_Bind, sock, channel, address) }
        
        void UDP_Close(UDPsocket sock) { GENFUNC(NetUDP_Close, sock) }

        IPaddress * UDP_GetPeerAddress(UDPsocket sock, int channel) { GENFUNC(NetUDP_GetPeerAddress, sock, channel) }
        
        UDPsocket UDP_Open(uint16_t port) { GENFUNC(NetUDP_Open, port) }

        int UDP_Recv(UDPsocket sock, UDPpacket *packet) { GENFUNC(NetUDP_Recv, sock, packet) }

        int UDP_RecvV(UDPsocket sock, UDPpacket **packets) { GENFUNC(NetUDP_RecvV, sock, packets) }

        int UDP_Send(UDPsocket sock, int channel, UDPpacket *packet) { GENFUNC(NetUDP_Send, sock, channel, packet) }

        int UDP_SendV(UDPsocket sock, UDPpacket **packets, int npackets) { GENFUNC(NetUDP_SendV, sock, packets, npackets) }
        
        void UDP_SetPacketLoss(UDPsocket sock, int percent) { GENFUNC(NetUDP_SetPacketLoss, sock, percent) }
        
        void UDP_Unbind(UDPsocket sock, int channel) { GENFUNC(NetUDP_Unbind, sock, channel) }
    }
    namespace Mixer {
        
        int AllocateChannels(int numchans) { GENFUNC(MixAllocateChannels, numchans) }
        
        void ChannelFinished(void (*channel_finished)(int channel)) { GENFUNC(MixChannelFinished, channel_finished) }
        void CloseAudio() { GENFUNC(MixCloseAudio) }

        int EachSoundFont(int (*function)(const char*, void*), void *data) { GENFUNC(MixEachSoundFont, function, data) }

        int ExpireChannel(int channel, int ticks) { GENFUNC(MixExpireChannel, channel, ticks) }

        int FadeInChannel(int channel, Mix_Chunk *chunk, int loops, int ms) { GENFUNC(MixFadeInChannel, channel, chunk, loops, ms) }

        int FadeInChannelTimed(int channel, Mix_Chunk *chunk, int loops, int ms, int ticks) { GENFUNC(MixFadeInChannelTimed, channel, chunk, loops, ms, ticks) }

        int FadeInMusic(Mix_Music *music, int loops, int ms) { GENFUNC(MixFadeInMusic, music, loops, ms) }

        int FadeInMusicPos(Mix_Music *music, int loops, int ms, double position) { GENFUNC(MixFadeInMusicPos, music, loops, ms, position) }

        int FadeOutChannel(int which, int ms) { GENFUNC(MixFadeOutChannel, which, ms) }

        int FadeOutGroup(int tag, int ms) { GENFUNC(MixFadeOutGroup, tag, ms) }
        
        int FadeOutMusic(int ms) { GENFUNC(MixFadeOutMusic, ms) }
        
        Mix_Fading FadingChannel(int which) { GENFUNC(MixFadingChannel, which) }
        
        Mix_Fading FadingMusic() { GENFUNC(MixFadingMusic) }
        
        void FreeChunk(Mix_Chunk *chunk) { GENFUNC(MixFreeChunk, chunk) }
        
        void FreeMusic(Mix_Music *music) { GENFUNC(MixFreeMusic, music) }
        
        Mix_Chunk * GetChunk(int channel) { GENFUNC(MixGetChunk, channel) }
        
        const char * GetChunkDecoder(int index) { GENFUNC(MixGetChunkDecoder, index) }
        
        const char* GetMusicAlbumTag(const Mix_Music *music) { GENFUNC(MixGetMusicAlbumTag, music) }
        
        const char* GetMusicArtistTag(const Mix_Music *music) { GENFUNC(MixGetMusicArtistTag, music) }
        
        const char* GetMusicCopyrightTag(const Mix_Music *music) { GENFUNC(MixGetMusicCopyrightTag, music) }
        
        const char * GetMusicDecoder(int index) { GENFUNC(MixGetMusicDecoder, index) }
        
        void * GetMusicHookData() { GENFUNC(MixGetMusicHookData) }
        
        double GetMusicLoopEndTime(Mix_Music *music) { GENFUNC(MixGetMusicLoopEndTime, music) }
        
        double GetMusicLoopLengthTime(Mix_Music *music) { GENFUNC(MixGetMusicLoopLengthTime, music) }
        
        double GetMusicLoopStartTime(Mix_Music *music) { GENFUNC(MixGetMusicLoopStartTime, music) }
        
        double GetMusicPosition(Mix_Music *music) { GENFUNC(MixGetMusicPosition, music) }
        
        const char* GetMusicTitle(const Mix_Music *music) { GENFUNC(MixGetMusicTitle, music) }
        
        const char* GetMusicTitleTag(const Mix_Music *music) { GENFUNC(MixGetMusicTitleTag, music) }
        
        Mix_MusicType GetMusicType(const Mix_Music *music) { GENFUNC(MixGetMusicType, music) }
        
        int GetMusicVolume(Mix_Music *music) { GENFUNC(MixGetMusicVolume, music) }
        
        int GetNumChunkDecoders() { GENFUNC(MixGetNumChunkDecoders) }
        
        int GetNumMusicDecoders() { GENFUNC(MixGetNumMusicDecoders) }
        
        const char* GetSoundFonts() { GENFUNC(MixGetSoundFonts) }
        
        int GetSynchroValue() { GENFUNC(MixGetSynchroValue) }
        
        const char* GetTimidityCfg() { GENFUNC(MixGetTimidityCfg) }
        
        int GroupAvailable(int tag) { GENFUNC(MixGroupAvailable, tag) }

        int GroupChannel(int which, int tag) { GENFUNC(MixGroupChannel, which, tag) }

        int GroupChannels(int from, int to, int tag) { GENFUNC(MixGroupChannels, from, to, tag) }
        
        int GroupCount(int tag) { GENFUNC(MixGroupCount, tag) }
        
        int GroupNewer(int tag) { GENFUNC(MixGroupNewer, tag) }
        
        int GroupOldest(int tag) { GENFUNC(MixGroupOldest, tag) }
        
        int HaltChannel(int channel) { GENFUNC(MixHaltChannel, channel) }
        
        int HaltGroup(int tag) { GENFUNC(MixHaltGroup, tag) }
        
        int HaltMusic() { GENFUNC(MixHaltMusic) }
        
        bool HasChunkDecoder(const char *name) { GENFUNC(MixHasChunkDecoder, name) }
        
        bool HasMusicDecoder(const char *name) { GENFUNC(MixHasMusicDecoder, name) }
        
        void HookMusic(void ( *mix_func)(void *udata, Uint8 *stream, int len), void *arg) { GENFUNC(MixHookMusic, mix_func, arg) }
        
        void HookMusicFinished(void ( *music_finished)(void)) { GENFUNC(MixHookMusicFinished, music_finished) }
        
        int Init(int flags) { GENFUNC(MixInit, flags) }
        
        const SDL_version * Linked_Version() { GENFUNC(MixLinked_Version) }
        
        Mix_Music * LoadMUS(const char *file) { GENFUNC(MixLoadMUS, file) }

        Mix_Music * LoadMUS_RW(SDL_RWops *src, int freesrc) { GENFUNC(MixLoadMUS_RW, src, freesrc) }

        Mix_Music * LoadMUSType_RW(SDL_RWops *src, Mix_MusicType type, int freesrc) { GENFUNC(MixLoadMUSType_RW, src, type, freesrc) }
        
        Mix_Chunk * LoadWAV(const char *file) { GENFUNC(MixLoadWAV, file) }

        Mix_Chunk * LoadWAV_RW(SDL_RWops *src, int freesrc) { GENFUNC(MixLoadWAV_RW, src, freesrc) }
        
        int MasterVolume(int volume) { GENFUNC(MixMasterVolume, volume) }
        
        int ModMusicJumpToOrder(int order) { GENFUNC(MixModMusicJumpToOrder, order) }
        
        double MusicDuration(Mix_Music *music) { GENFUNC(MixMusicDuration, music) }

        int OpenAudio(int frequency, uint16_t format, int channels, int chunksize) { GENFUNC(MixOpenAudio, frequency, format, channels, chunksize) }

        int OpenAudioDevice(int frequency, uint16_t format, int channels, int chunksize, const char* device, int allowed_changes) { GENFUNC(MixOpenAudioDevice, frequency, format, channels, chunksize, device, allowed_changes) }
        
        void Pause(int channel) { GENFUNC(MixPause, channel) }
        
        int Paused(int channel) { GENFUNC(MixPaused, channel) }
        
        int PausedMusic() { GENFUNC(MixPausedMusic) }
        void PauseMusic() { GENFUNC(MixPauseMusic) }

        int PlayChannel(int channel, Mix_Chunk *chunk, int loops) { GENFUNC(MixPlayChannel, channel, chunk, loops) }

        int PlayChannelTimed(int channel, Mix_Chunk *chunk, int loops, int ticks) { GENFUNC(MixPlayChannelTimed, channel, chunk, loops, ticks) }
        
        int Playing(int channel) { GENFUNC(MixPlaying, channel) }
        
        int PlayingMusic() { GENFUNC(MixPlayingMusic) }

        int PlayMusic(Mix_Music *music, int loops) { GENFUNC(MixPlayMusic, music, loops) }

        int QuerySpec(int *frequency, uint16_t *format, int *channels) { GENFUNC(MixQuerySpec, frequency, format, channels) }

        Mix_Chunk * QuickLoad_RAW(Uint8 *mem, Uint32 len) { GENFUNC(MixQuickLoad_RAW, mem, len) }
        
        Mix_Chunk * QuickLoad_WAV(Uint8 *mem) { GENFUNC(MixQuickLoad_WAV, mem) }
        void Quit() { GENFUNC(MixQuit) }

        int RegisterEffect(int chan, Mix_EffectFunc_t f, Mix_EffectDone_t d, void *arg) { GENFUNC(MixRegisterEffect, chan, f, d, arg) }
        
        int ReserveChannels(int num) { GENFUNC(MixReserveChannels, num) }
        
        void Resume(int channel) { GENFUNC(MixResume, channel) }
        void ResumeMusic() { GENFUNC(MixResumeMusic) }
        void RewindMusic() { GENFUNC(MixRewindMusic) }

        int SetDistance(int channel, Uint8 distance) { GENFUNC(MixSetDistance, channel, distance) }
        
        int SetMusicCMD(const char *command) { GENFUNC(MixSetMusicCMD, command) }
        
        int SetMusicPosition(double position) { GENFUNC(MixSetMusicPosition, position) }

        int SetPanning(int channel, Uint8 left, Uint8 right) { GENFUNC(MixSetPanning, channel, left, right) }

        int SetPosition(int channel, Sint16 angle, Uint8 distance) { GENFUNC(MixSetPosition, channel, angle, distance) }
        
        void SetPostMix(void (*mix_func)(void *udata, Uint8 *stream, int len), void *arg) { GENFUNC(MixSetPostMix, mix_func, arg) }

        int SetReverseStereo(int channel, int flip) { GENFUNC(MixSetReverseStereo, channel, flip) }
        
        int SetSoundFonts(const char *paths) { GENFUNC(MixSetSoundFonts, paths) }
        
        int SetSynchroValue(int value) { GENFUNC(MixSetSynchroValue, value) }
        
        int SetTimidityCfg(const char *path) { GENFUNC(MixSetTimidityCfg, path) }
        
        int UnregisterAllEffects(int channel) { GENFUNC(MixUnregisterAllEffects, channel) }

        int UnregisterEffect(int channel, Mix_EffectFunc_t f) { GENFUNC(MixUnregisterEffect, channel, f) }

        int Volume(int channel, int volume) { GENFUNC(MixVolume, channel, volume) }

        int VolumeChunk(Mix_Chunk *chunk, int volume) { GENFUNC(MixVolumeChunk, chunk, volume) }
        
        int VolumeMusic(int volume) { GENFUNC(MixVolumeMusic, volume) }
    } // namespace Mixer
    namespace TTF {
        
        void ByteSwappedUNICODE(bool swapped) { GENFUNC(TTFByteSwappedUNICODE,swapped) }
        
        void CloseFont(TTF_Font *font) { GENFUNC(TTFCloseFont,font) }
        
        int FontAscent(const TTF_Font *font) { GENFUNC(TTFFontAscent,font) }
        
        int FontDescent(const TTF_Font *font) { GENFUNC(TTFFontDescent,font) }
        
        const char * FontFaceFamilyName(const TTF_Font *font) { GENFUNC(TTFFontFaceFamilyName,font) }
        
        int FontFaceIsFixedWidth(const TTF_Font *font) { GENFUNC(TTFFontFaceIsFixedWidth,font) }
        
        long FontFaces(const TTF_Font *font) { GENFUNC(TTFFontFaces,font) }
        
        const char * FontFaceStyleName(const TTF_Font *font) { GENFUNC(TTFFontFaceStyleName,font) }
        
        int FontHeight(const TTF_Font *font) { GENFUNC(TTFFontHeight,font) }
        
        int FontLineSkip(const TTF_Font *font) { GENFUNC(TTFFontLineSkip,font) }
        
        int GetFontHinting(const TTF_Font *font) { GENFUNC(TTFGetFontHinting,font) }
        
        int GetFontKerning(const TTF_Font *font) { GENFUNC(TTFGetFontKerning,font) }
        
        int GetFontOutline(const TTF_Font *font) { GENFUNC(TTFGetFontOutline,font) }
        
        int GetFontStyle(const TTF_Font *font) { GENFUNC(TTFGetFontStyle,font) }
        
        int GetFontWrappedAlign(const TTF_Font *font) { GENFUNC(TTFGetFontWrappedAlign,font) }

        void GetFreeTypeVersion(int *major, int *minor, int *patch) { GENFUNC(TTFGetFreeTypeVersion,major, minor, patch) }

        void GetHarfBuzzVersion(int *major, int *minor, int *patch) { GENFUNC(TTFGetHarfBuzzVersion,major, minor, patch) }

        int GlyphIsProvided(TTF_Font *font, uint16_t ch) { GENFUNC(TTFGlyphIsProvided,font, ch) }

        int GlyphIsProvided32(TTF_Font *font, Uint32 ch) { GENFUNC(TTFGlyphIsProvided32,font, ch) }
        
        int GlyphMetrics(TTF_Font *font, uint16_t ch, int *minx, int *maxx, int *miny, int *maxy, int *advance) { GENFUNC(TTFGlyphMetrics,font, ch, minx, maxx, miny, maxy, advance) }
        
        int GlyphMetrics32(TTF_Font *font, Uint32 ch, int *minx, int *maxx, int *miny, int *maxy, int *advance) { GENFUNC(TTFGlyphMetrics32,font, ch, minx, maxx, miny, maxy, advance) }
        
        int Init() { GENFUNC(TTFInit) }
        
        const SDL_version * Linked_Version() { GENFUNC(TTFLinked_Version) }

        int MeasureText(TTF_Font *font, const char *text, int measure_width, int *extent, int *count) { GENFUNC(TTFMeasureText,font, text, measure_width, extent, count) }

        int MeasureUNICODE(TTF_Font *font, const uint16_t *text, int measure_width, int *extent, int *count) { GENFUNC(TTFMeasureUNICODE,font, text, measure_width, extent, count) }

        int MeasureUTF8(TTF_Font *font, const char *text, int measure_width, int *extent, int *count) { GENFUNC(TTFMeasureUTF8,font, text, measure_width, extent, count) }

        TTF_Font * OpenFont(const char *file, int ptsize) { GENFUNC(TTFOpenFont,file, ptsize) }

        TTF_Font * OpenFontDPI(const char *file, int ptsize, unsigned int hdpi, unsigned int vdpi) { GENFUNC(TTFOpenFontDPI,file, ptsize, hdpi, vdpi) }

        TTF_Font * OpenFontDPIRW(SDL_RWops *src, int freesrc, int ptsize, unsigned int hdpi, unsigned int vdpi) { GENFUNC(TTFOpenFontDPIRW,src, freesrc, ptsize, hdpi, vdpi) }

        TTF_Font * OpenFontIndex(const char *file, int ptsize, long index) { GENFUNC(TTFOpenFontIndex,file, ptsize, index) }

        TTF_Font * OpenFontIndexDPI(const char *file, int ptsize, long index, unsigned int hdpi, unsigned int vdpi) { GENFUNC(TTFOpenFontIndexDPI,file, ptsize, index, hdpi, vdpi) }

        TTF_Font * OpenFontIndexDPIRW(SDL_RWops *src, int freesrc, int ptsize, long index, unsigned int hdpi, unsigned int vdpi) { GENFUNC(TTFOpenFontIndexDPIRW,src, freesrc, ptsize, index, hdpi, vdpi) }

        TTF_Font * OpenFontIndexRW(SDL_RWops *src, int freesrc, int ptsize, long index) { GENFUNC(TTFOpenFontIndexRW,src, freesrc, ptsize, index) }

        TTF_Font * OpenFontRW(SDL_RWops *src, int freesrc, int ptsize) { GENFUNC(TTFOpenFontRW,src, freesrc, ptsize) }
        void Quit() { GENFUNC(TTFQuit) }

        SDL_Surface * RenderGlyph32_Blended(TTF_Font *font, Uint32 ch, SDL_Color fg) { GENFUNC(TTFRenderGlyph32_Blended,font, ch, fg) }

        SDL_Surface * RenderGlyph32_LCD(TTF_Font *font, Uint32 ch, SDL_Color fg, SDL_Color bg) { GENFUNC(TTFRenderGlyph32_LCD,font, ch, fg, bg) }

        SDL_Surface * RenderGlyph32_Shaded(TTF_Font *font, Uint32 ch, SDL_Color fg, SDL_Color bg) { GENFUNC(TTFRenderGlyph32_Shaded,font, ch, fg, bg) }

        SDL_Surface * RenderGlyph32_Solid(TTF_Font *font, Uint32 ch, SDL_Color fg) { GENFUNC(TTFRenderGlyph32_Solid,font, ch, fg) }

        SDL_Surface * RenderGlyph_Blended(TTF_Font *font, uint16_t ch, SDL_Color fg) { GENFUNC(TTFRenderGlyph_Blended,font, ch, fg) }

        SDL_Surface * RenderGlyph_LCD(TTF_Font *font, uint16_t ch, SDL_Color fg, SDL_Color bg) { GENFUNC(TTFRenderGlyph_LCD,font, ch, fg, bg) }

        SDL_Surface * RenderGlyph_Shaded(TTF_Font *font, uint16_t ch, SDL_Color fg, SDL_Color bg) { GENFUNC(TTFRenderGlyph_Shaded,font, ch, fg, bg) }

        SDL_Surface * RenderGlyph_Solid(TTF_Font *font, uint16_t ch, SDL_Color fg) { GENFUNC(TTFRenderGlyph_Solid,font, ch, fg) }

        SDL_Surface * RenderText_Blended(TTF_Font *font, const char *text, SDL_Color fg) { GENFUNC(TTFRenderText_Blended,font, text, fg) }

        SDL_Surface * RenderText_Blended_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, Uint32 wrapLength) { GENFUNC(TTFRenderText_Blended_Wrapped,font, text, fg, wrapLength) }

        SDL_Surface * RenderText_LCD(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg) { GENFUNC(TTFRenderText_LCD,font, text, fg, bg) }

        SDL_Surface * RenderText_LCD_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength) { GENFUNC(TTFRenderText_LCD_Wrapped,font, text, fg, bg, wrapLength) }

        SDL_Surface * RenderText_Shaded(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg) { GENFUNC(TTFRenderText_Shaded,font, text, fg, bg) }

        SDL_Surface * RenderText_Shaded_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength) { GENFUNC(TTFRenderText_Shaded_Wrapped,font, text, fg, bg, wrapLength) }

        SDL_Surface * RenderText_Solid(TTF_Font *font, const char *text, SDL_Color fg) { GENFUNC(TTFRenderText_Solid,font, text, fg) }

        SDL_Surface * RenderText_Solid_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, Uint32 wrapLength) { GENFUNC(TTFRenderText_Solid_Wrapped,font, text, fg, wrapLength) }

        SDL_Surface * RenderUNICODE_Blended(TTF_Font *font, const uint16_t *text, SDL_Color fg) { GENFUNC(TTFRenderUNICODE_Blended,font, text, fg) }

        SDL_Surface * RenderUNICODE_Blended_Wrapped(TTF_Font *font, const uint16_t *text, SDL_Color fg, Uint32 wrapLength) { GENFUNC(TTFRenderUNICODE_Blended_Wrapped,font, text, fg, wrapLength) }

        SDL_Surface * RenderUNICODE_LCD(TTF_Font *font, const uint16_t *text, SDL_Color fg, SDL_Color bg) { GENFUNC(TTFRenderUNICODE_LCD,font, text, fg, bg) }

        SDL_Surface * RenderUNICODE_LCD_Wrapped(TTF_Font *font, const uint16_t *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength) { GENFUNC(TTFRenderUNICODE_LCD_Wrapped,font, text, fg, bg, wrapLength) }

        SDL_Surface * RenderUNICODE_Shaded(TTF_Font *font, const uint16_t *text, SDL_Color fg, SDL_Color bg) { GENFUNC(TTFRenderUNICODE_Shaded,font, text, fg, bg) }

        SDL_Surface * RenderUNICODE_Shaded_Wrapped(TTF_Font *font, const uint16_t *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength) { GENFUNC(TTFRenderUNICODE_Shaded_Wrapped,font, text, fg, bg, wrapLength) }

        SDL_Surface * RenderUNICODE_Solid(TTF_Font *font, const uint16_t *text, SDL_Color fg) { GENFUNC(TTFRenderUNICODE_Solid,font, text, fg) }

        SDL_Surface * RenderUNICODE_Solid_Wrapped(TTF_Font *font, const uint16_t *text, SDL_Color fg, Uint32 wrapLength) { GENFUNC(TTFRenderUNICODE_Solid_Wrapped,font, text, fg, wrapLength) }

        SDL_Surface * RenderUTF8_Blended(TTF_Font *font, const char *text, SDL_Color fg) { GENFUNC(TTFRenderUTF8_Blended,font, text, fg) }

        SDL_Surface * RenderUTF8_Blended_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, Uint32 wrapLength) { GENFUNC(TTFRenderUTF8_Blended_Wrapped,font, text, fg, wrapLength) }

        SDL_Surface * RenderUTF8_LCD(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg) { GENFUNC(TTFRenderUTF8_LCD,font, text, fg, bg) }

        SDL_Surface * RenderUTF8_LCD_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength) { GENFUNC(TTFRenderUTF8_LCD_Wrapped,font, text, fg, bg, wrapLength) }

        SDL_Surface * RenderUTF8_Shaded(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg) { GENFUNC(TTFRenderUTF8_Shaded,font, text, fg, bg) }

        SDL_Surface * RenderUTF8_Shaded_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength) { GENFUNC(TTFRenderUTF8_Shaded_Wrapped,font, text, fg, bg, wrapLength) }

        SDL_Surface * RenderUTF8_Solid(TTF_Font *font, const char *text, SDL_Color fg) { GENFUNC(TTFRenderUTF8_Solid,font, text, fg) }

        SDL_Surface * RenderUTF8_Solid_Wrapped(TTF_Font *font, const char *text, SDL_Color fg, Uint32 wrapLength) { GENFUNC(TTFRenderUTF8_Solid_Wrapped,font, text, fg, wrapLength) }
        
        int SetDirection(int direction /* hb_direction_t */) { GENFUNC(TTFSetDirection, direction) }

        int SetFontDirection(TTF_Font *font, TTF_Direction direction) { GENFUNC(TTFSetFontDirection,font, direction) }
        
        void SetFontHinting(TTF_Font *font, int hinting) { GENFUNC(TTFSetFontHinting,font, hinting) }
        
        void SetFontKerning(TTF_Font *font, int allowed) { GENFUNC(TTFSetFontKerning,font, allowed) }
        
        void SetFontOutline(TTF_Font *font, int outline) { GENFUNC(TTFSetFontOutline,font, outline) }

        int SetFontScriptName(TTF_Font *font, const char *script) { GENFUNC(TTFSetFontScriptName,font, script) }

        int SetFontSize(TTF_Font *font, int ptsize) { GENFUNC(TTFSetFontSize,font, ptsize) }

        int SetFontSizeDPI(TTF_Font *font, int ptsize, unsigned int hdpi, unsigned int vdpi) { GENFUNC(TTFSetFontSizeDPI,font, ptsize, hdpi, vdpi) }
        
        void SetFontStyle(TTF_Font *font, int style) { GENFUNC(TTFSetFontStyle,font, style) }
        
        void SetFontWrappedAlign(TTF_Font *font, int align) { GENFUNC(TTFSetFontWrappedAlign,font, align) }
        
        int SetScript(int script /* hb_script_t */) { GENFUNC(TTFSetScript, script) }

        int SizeText(TTF_Font *font, const char *text, int *w, int *h) { GENFUNC(TTFSizeText,font, text, w, h) }

        int SizeUNICODE(TTF_Font *font, const uint16_t *text, int *w, int *h) { GENFUNC(TTFSizeUNICODE,font, text, w, h) }

        int SizeUTF8(TTF_Font *font, const char *text, int *w, int *h) { GENFUNC(TTFSizeUTF8,font, text, w, h) }
        
        int WasInit() { GENFUNC(TTFWasInit) }
    } // namespace TTF
    namespace Image {
        
        void FreeAnimation(IMG_Animation *anim) { GENFUNC(IMGFreeAnimation,anim) }
        
        int Init(int flags) { GENFUNC(IMGInit,flags) }
        
        int isAVIF(SDL_RWops *src) { GENFUNC(IMGisAVIF,src) }
        
        int isBMP(SDL_RWops *src) { GENFUNC(IMGisBMP,src) }
        
        int isCUR(SDL_RWops *src) { GENFUNC(IMGisCUR,src) }
        
        int isGIF(SDL_RWops *src) { GENFUNC(IMGisGIF,src) }
        
        int isICO(SDL_RWops *src) { GENFUNC(IMGisICO,src) }
        
        int isJPG(SDL_RWops *src) { GENFUNC(IMGisJPG,src) }
        
        int isJXL(SDL_RWops *src) { GENFUNC(IMGisJXL,src) }
        
        int isLBM(SDL_RWops *src) { GENFUNC(IMGisLBM,src) }
        
        int isPCX(SDL_RWops *src) { GENFUNC(IMGisPCX,src) }
        
        int isPNG(SDL_RWops *src) { GENFUNC(IMGisPNG,src) }
        
        int isPNM(SDL_RWops *src) { GENFUNC(IMGisPNM,src) }
        
        int isQOI(SDL_RWops *src) { GENFUNC(IMGisQOI,src) }
        
        int isSVG(SDL_RWops *src) { GENFUNC(IMGisSVG,src) }
        
        int isTIF(SDL_RWops *src) { GENFUNC(IMGisTIF,src) }
        
        int isWEBP(SDL_RWops *src) { GENFUNC(IMGisWEBP,src) }
        
        int isXCF(SDL_RWops *src) { GENFUNC(IMGisXCF,src) }
        
        int isXPM(SDL_RWops *src) { GENFUNC(IMGisXPM,src) }
        
        int isXV(SDL_RWops *src) { GENFUNC(IMGisXV,src) }
        
        const SDL_version * Linked_Version() { GENFUNC(IMGLinked_Version) }
        
        SDL_Surface * Load(const char *file) { GENFUNC(IMGLoad,file) }

        SDL_Surface * Load_RW(SDL_RWops *src, int freesrc) { GENFUNC(IMGLoad_RW,src, freesrc) }
        
        IMG_Animation * LoadAnimation(const char *file) { GENFUNC(IMGLoadAnimation,file) }

        IMG_Animation * LoadAnimation_RW(SDL_RWops *src, int freesrc) { GENFUNC(IMGLoadAnimation_RW,src, freesrc) }

        IMG_Animation * LoadAnimationTyped_RW(SDL_RWops *src, int freesrc, const char *type) { GENFUNC(IMGLoadAnimationTyped_RW,src, freesrc, type) }
        
        SDL_Surface * LoadAVIF_RW(SDL_RWops *src) { GENFUNC(IMGLoadAVIF_RW,src) }
        
        SDL_Surface * LoadBMP_RW(SDL_RWops *src) { GENFUNC(IMGLoadBMP_RW,src) }
        
        SDL_Surface * LoadCUR_RW(SDL_RWops *src) { GENFUNC(IMGLoadCUR_RW,src) }
        
        SDL_Surface * LoadGIF_RW(SDL_RWops *src) { GENFUNC(IMGLoadGIF_RW,src) }
        
        IMG_Animation * LoadGIFAnimation_RW(SDL_RWops *src) { GENFUNC(IMGLoadGIFAnimation_RW,src) }
        
        SDL_Surface * LoadICO_RW(SDL_RWops *src) { GENFUNC(IMGLoadICO_RW,src) }
        
        SDL_Surface * LoadJPG_RW(SDL_RWops *src) { GENFUNC(IMGLoadJPG_RW,src) }
        
        SDL_Surface * LoadJXL_RW(SDL_RWops *src) { GENFUNC(IMGLoadJXL_RW,src) }
        
        SDL_Surface * LoadLBM_RW(SDL_RWops *src) { GENFUNC(IMGLoadLBM_RW,src) }
        
        SDL_Surface * LoadPCX_RW(SDL_RWops *src) { GENFUNC(IMGLoadPCX_RW,src) }
        
        SDL_Surface * LoadPNG_RW(SDL_RWops *src) { GENFUNC(IMGLoadPNG_RW,src) }
        
        SDL_Surface * LoadPNM_RW(SDL_RWops *src) { GENFUNC(IMGLoadPNM_RW,src) }
        
        SDL_Surface * LoadQOI_RW(SDL_RWops *src) { GENFUNC(IMGLoadQOI_RW,src) }

        SDL_Surface * LoadSizedSVG_RW(SDL_RWops *src, int width, int height) { GENFUNC(IMGLoadSizedSVG_RW,src, width, height) }
        
        SDL_Surface * LoadSVG_RW(SDL_RWops *src) { GENFUNC(IMGLoadSVG_RW,src) }

        SDL_Texture * LoadTexture(SDL_Renderer *renderer, const char *file) { GENFUNC(IMGLoadTexture,renderer, file) }

        SDL_Texture * LoadTexture_RW(SDL_Renderer *renderer, SDL_RWops *src, int freesrc) { GENFUNC(IMGLoadTexture_RW,renderer, src, freesrc) }

        SDL_Texture * LoadTextureTyped_RW(SDL_Renderer *renderer, SDL_RWops *src, int freesrc, const char *type) { GENFUNC(IMGLoadTextureTyped_RW,renderer, src, freesrc, type) }
        
        SDL_Surface * LoadTGA_RW(SDL_RWops *src) { GENFUNC(IMGLoadTGA_RW,src) }
        
        SDL_Surface * LoadTIF_RW(SDL_RWops *src) { GENFUNC(IMGLoadTIF_RW,src) }

        SDL_Surface * LoadTyped_RW(SDL_RWops *src, int freesrc, const char *type) { GENFUNC(IMGLoadTyped_RW,src, freesrc, type) }
        
        SDL_Surface * LoadWEBP_RW(SDL_RWops *src) { GENFUNC(IMGLoadWEBP_RW,src) }
        
        SDL_Surface * LoadXCF_RW(SDL_RWops *src) { GENFUNC(IMGLoadXCF_RW,src) }
        
        SDL_Surface * LoadXPM_RW(SDL_RWops *src) { GENFUNC(IMGLoadXPM_RW,src) }
        
        SDL_Surface * LoadXV_RW(SDL_RWops *src) { GENFUNC(IMGLoadXV_RW,src) }
        void Quit() { GENFUNC(IMGQuit) }
        
        SDL_Surface * ReadXPMFromArray(char **xpm) { GENFUNC(IMGReadXPMFromArray,xpm) }
        
        SDL_Surface * ReadXPMFromArrayToRGB888(char **xpm) { GENFUNC(IMGReadXPMFromArrayToRGB888,xpm) }

        int SaveJPG(SDL_Surface *surface, const char *file, int quality) { GENFUNC(IMGSaveJPG,surface, file, quality) }

        int SaveJPG_RW(SDL_Surface *surface, SDL_RWops *dst, int freedst, int quality) { GENFUNC(IMGSaveJPG_RW,surface, dst, freedst, quality) }

        int SavePNG(SDL_Surface *surface, const char *file) { GENFUNC(IMGSavePNG,surface, file) }

        int SavePNG_RW(SDL_Surface *surface, SDL_RWops *dst, int freedst) { GENFUNC(IMGSavePNG_RW,surface, dst, freedst) }
    } // namespace Image
#endif
} // namespace SDL2