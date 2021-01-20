#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

extern SDL_Renderer *game_renderer;
typedef struct Texture
{
	SDL_Texture* texture;
	
	int width;
	int height;
} Texture;


void texture_create( Texture* texture )
{
    texture->width = 0;
    texture->height = 0;
    texture->texture = NULL;
}

void texture_free(Texture* texture)
{
    if( texture->texture != NULL )
	{
		SDL_DestroyTexture( texture->texture );
		texture->texture = NULL;
		texture->width = 0;
		texture->height = 0;
	}
}

/*  Loads a texture from a file \return success */
int texture_loadFromFile(Texture* texture, char* path)
{
    texture_free(texture);
    SDL_Texture* new_texture = NULL;
    SDL_Surface* loaded_surface = IMG_Load(path);
    if( loaded_surface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
	}
    else
    {
        SDL_SetColorKey( loaded_surface, SDL_TRUE, SDL_MapRGB( loaded_surface->format, 0, 0xFF, 0xFF ) );
        new_texture = SDL_CreateTextureFromSurface( game_renderer, loaded_surface );
        if( new_texture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			texture->width = loaded_surface->w;
			texture->height = loaded_surface->h;
		}

        SDL_FreeSurface( loaded_surface );
    }
    
    texture->texture = new_texture;
    return texture->texture != NULL;

}

/*  Renders the texture */
void texture_render( Texture* texture, int x, int y )
{

    SDL_Rect render_quad = { x, y, texture->width, texture->height };

    SDL_RenderCopy( game_renderer, texture->texture, NULL, &render_quad );

}

/*  Renders the texture with additional params*/
void texture_render_ext( Texture* texture, int x, int y, SDL_Rect* clip_rect, double angle, SDL_Point* center, SDL_RendererFlip flip )
{

    SDL_Rect render_quad = { x, y, texture->width, texture->height };

    if ( clip_rect != NULL)
    {
        render_quad.w = clip_rect->w;
        render_quad.h = clip_rect->h;
    }

    SDL_RenderCopyEx( game_renderer, texture->texture, clip_rect, &render_quad, angle, center, flip );

}

