#include <SDL.h>
#include <stdio.h>

typedef struct Texture
{
	SDL_Texture* texture;
	
	int width;
	int height;
} Texture;


void texture_create( Texture* texture );

/*  Loads a texture from a file \return success */
int texture_loadFromFile( Texture* texture, char* path );

/*  Renders the texture */
void texture_render( Texture* texture, int x, int y );

/*  Renders the texture with additional parameters */
void texture_render_ext( Texture* texture, int x, int y, SDL_Rect* clip_rect, double angle, SDL_Point* center, SDL_RendererFlip flip );

void texture_free( Texture* texture );