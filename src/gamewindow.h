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


/*  Creates a game window \return success */
int game_window_create(GameWindow* game_window, char* title, int w, int h);

/*  Creates a renderer \return SDL_Renderer */
SDL_Renderer* game_window_create_renderer( GameWindow* game_window);

void game_window_handle_event(GameWindow* game_window, SDL_Event* event );

void game_window_free(GameWindow* game_window);

/* Updates the window title */
void game_window_update_title(GameWindow* game_window, char* string);