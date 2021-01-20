#include <SDL.h>
#include <stdio.h>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern SDL_Renderer *game_renderer;
struct _GameWindow 
{
	SDL_Window* window;
	
	int width;
	int height;
    char *title;

	int fullscreen;
	int minimized;
	int mouseFocus;
	int keyboardFocus;
};

typedef struct _GameWindow GameWindow;

int game_window_create(GameWindow* game_window, char* title, int w, int h)
{
    /* init game window*/
    game_window->minimized = 0;
    game_window->mouseFocus = 0;
    game_window->keyboardFocus = 0;
    game_window->fullscreen = 0;
    game_window->title = NULL;    
    game_window->width = 0;
    game_window->height = 0;
    /* create */
    game_window ->window = SDL_CreateWindow( title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
    if (game_window->window != NULL)
    {
        game_window->mouseFocus = 1;
        game_window->keyboardFocus = 1;
        game_window->width = w;
        game_window->height = h;
        game_window->title = title;
    }

    return game_window->window != NULL;
}

SDL_Renderer* game_window_create_renderer(GameWindow* game_window)
{
    return SDL_CreateRenderer( game_window->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
}


void game_window_update_title(GameWindow* game_window, char *string)
{
    //char string[120];
    //sprintf( string, "%s - MouseFocus: %s | KeyboardFocus: %s", game_window->title, game_window->mouseFocus ? "ON" : "OFF", game_window->keyboardFocus ? "ON" : "OFF" );
    SDL_SetWindowTitle( game_window->window, string );
}

void game_window_handle_event(GameWindow* game_window, SDL_Event* event )
{
    if ( event->type == SDL_WINDOWEVENT )
    {
        int updateCaption = 0;
        
        switch (event->window.event)
        {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                game_window->width = event->window.data1;
                game_window->height = event->window.data2;
                SDL_RenderSetScale( game_renderer, (float) game_window->width/ (float) SCREEN_WIDTH, (float) game_window->height/(float) SCREEN_HEIGHT );
                SDL_RenderPresent( game_renderer );
                
                break;

            case SDL_WINDOWEVENT_EXPOSED:
                SDL_RenderPresent( game_renderer );
                break;

            case SDL_WINDOWEVENT_MAXIMIZED:
                game_window->minimized = 0;
                break;

            case SDL_WINDOWEVENT_MINIMIZED:
                game_window->minimized = 1;
                break;

            case SDL_WINDOWEVENT_RESTORED:
                game_window->minimized = 0;
                break;
            
            case SDL_WINDOWEVENT_ENTER:
                game_window->mouseFocus = 1;
                //updateCaption = 1;
                break;

            case SDL_WINDOWEVENT_LEAVE:
                game_window->mouseFocus = 0;
                //updateCaption = 1;
                break;

            case SDL_WINDOWEVENT_FOCUS_GAINED:
                game_window->keyboardFocus = 1;
                game_window->mouseFocus = 1;
                break;

            case SDL_WINDOWEVENT_FOCUS_LOST:
                game_window->keyboardFocus = 0;
                //updateCaption = 1;
                break;            
        }
        
        if (updateCaption) 
        {
            char string[120];
            sprintf( string, "%s - MouseFocus: %s | KeyboardFocus: %s", game_window->title, game_window->mouseFocus ? "ON" : "OFF", game_window->keyboardFocus ? "ON" : "OFF" );
            game_window_update_title( game_window, string );
        }
        
    }
    else if ( event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_RETURN )
    {
       
        if ( SDL_GetModState() & KMOD_ALT ){
            if ( game_window->fullscreen )
            {
                SDL_SetWindowFullscreen( game_window->window, 0 );
                game_window->fullscreen = 0;
            }
            else
            {
                SDL_SetWindowFullscreen( game_window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                game_window->fullscreen = 1;
                game_window->minimized = 0;
            }
            
        }
    }

}

void game_window_free(GameWindow* game_window)
{
    if( game_window->window != NULL )
	{
		SDL_DestroyWindow( game_window->window );
	}
    
    game_window->minimized = 0;
    game_window->mouseFocus = 0;
    game_window->keyboardFocus = 0;
    game_window->fullscreen = 0;
    
    game_window->width = 0;
    game_window->height = 0;
}