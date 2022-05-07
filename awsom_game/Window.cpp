#include "Window.h"

Window::Window()
{
	window = NULL;
	width = 0;
	height = 0;
	hasMouseFocus = false;
	hasKeyboardFocus = false;
	isFullScreen = false;
	isMinimized = false;
}

bool Window::init(int swidth, int sheight)
{
	window = SDL_CreateWindow( "awsom gaem", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, swidth, sheight, SDL_WINDOW_SHOWN );
	if (window) {
		hasMouseFocus = true;
		hasKeyboardFocus = true;
		width = swidth;
		height = sheight;
	}
	return window != NULL;
}

SDL_Renderer* Window::createRenderer()
{
	if (renderer == NULL) {
		renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	}
	return renderer;
}

void Window::handleEvent( SDL_Event& e )
{
	if (e.type == SDL_WINDOWEVENT) {
		switch (e.window.event)
		{
			//Repaint on exposure
		case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent( renderer );
			break;

			//Mouse entered window
		case SDL_WINDOWEVENT_ENTER:
			hasMouseFocus = true;
			break;

			//Mouse left window
		case SDL_WINDOWEVENT_LEAVE:
			hasMouseFocus = false;
			break;

			//Window has keyboard focus
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			hasKeyboardFocus = true;
			break;

			//Window lost keyboard focus
		case SDL_WINDOWEVENT_FOCUS_LOST:
			hasKeyboardFocus = false;
			break;

			//Window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			isMinimized = true;
			break;

			//Window maxized
		case SDL_WINDOWEVENT_MAXIMIZED:
			isMinimized = false;
			break;

			//Window restored
		case SDL_WINDOWEVENT_RESTORED:
			isMinimized = false;
			break;
		}
	}
	else if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.sym == SDLK_F11) {
			isFullScreen = !isFullScreen;
			if(isFullScreen)
				SDL_SetWindowFullscreen( window, SDL_WINDOW_FULLSCREEN );
			else
				SDL_SetWindowFullscreen( window, 0 );
		}
	}
}

void Window::free()
{
	SDL_DestroyWindow( window );
	window = NULL;
	SDL_DestroyRenderer( renderer );
	renderer = NULL;
}

int Window::GetWidth() const
{
	return width;
}

int Window::GetHeight() const
{
	return height;
}

RectI Window::GetWindowRect() const
{
	return RectI( 0, GetWidth(), 0, GetHeight() );
}

bool Window::HasMouseFocus()
{
	return hasMouseFocus;
}

bool Window::HasKeyboardFocus()
{
	return hasKeyboardFocus;
}

bool Window::IsMinimized()
{
	return isMinimized;
}

bool Window::IsFullscreen()
{
	return isFullScreen;
}