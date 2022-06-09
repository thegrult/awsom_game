#pragma once
#include <SDL.h>
#include "Rect.h"

class Window {
public:
    //Intializes internals
	Window();

    //Creates window
    bool init(int width, int height);

    //Creates renderer from internal window
    SDL_Renderer* createRenderer();

    //Handles window events
    bool handleEvent( SDL_Event& e );

    //Deallocates internals
    void free();

    //Window dimensions
    int GetWidth() const;
    int GetHeight() const;

    RectI GetWindowRect() const;

#ifdef _DEBUG
    SDL_Window* GetWindow() { return window; }
#endif // _DEBUG


    //Window focii
    bool HasMouseFocus();
    bool HasKeyboardFocus();
    bool IsMinimized();
    bool IsFullscreen();
private:
    //Window data
    SDL_Window* window;
    SDL_Renderer* renderer;

    //Window dimensions
    int width;
    int height;

    //Window focus
    bool hasMouseFocus;
    bool hasKeyboardFocus;
    bool isFullScreen;
    bool isMinimized;
};