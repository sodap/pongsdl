#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "gamewindow.h"
#include "texture.h"

#include <time.h>
#include <stdlib.h>

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 960;

int CPU_SPEED = 12;
int CPU_IMPRECISION = 0.53;
int CPU_RANGE = 28;
int PLAYER_SPEED = 12;
int BALL_SPEED = 8;


SDL_Renderer* game_renderer;
GameWindow game_window;
Texture  my_texture;

typedef struct Entity
{
	int x;
	int y;
	
	signed int speed_x;
	signed int speed_y;

	int speed;
	Texture* texture;
	int rebound;
} Entity;


void print_error(char error[])
{
	printf("(!) %s! SDL_ERROR: %s\n",error, SDL_GetError() );
}

int collide(Entity* a, Entity* b)
{
	int collide = 0;
	if ( ( a->x + a->texture->width >= b->x && a->x < b->x + b->texture->width ) && (a->y + a->texture->height > b->y && a->y < b->y + b->texture->height ) )
	{
		collide = 1;
	}	
	return collide;
}

int rebound(Entity* a, Entity* b)
{
	if ( collide(a, b) )
	{
		if (!a->rebound)
		{	
			int a_center = a->x + a->texture->width/2;
			int sign_b_xspeed = ( (b->speed_x >= 0 ) - (b->speed_x < 0) );

			/* 	control X speed with paddle positioning!! */

			if (a->speed_y < 0) //its different if we rebound on the bottom or the top of a paddle
			{
				if (a->speed_x >= 0)
				{
					a->speed_x = ( 1 + a->speed * (b->x + b->texture->width - a_center) / ( b->texture->width ) ) * sign_b_xspeed;
				}
				else
				{
					a->speed_x = ( 1 + a->speed * (a_center - b->x ) / ( b->texture->width ) ) * sign_b_xspeed;
				}
				
			}
			else
			{
				if (a->speed_x >= 0)
					a->speed_x = ( 1 + a->speed * (a_center - b->x ) / ( b->texture->width ) ) * sign_b_xspeed;
				else
					a->speed_x = (1 + a->speed * (b->x + b->texture->width - a_center) / ( b->texture->width ) ) * sign_b_xspeed;
			}
			a->speed_y = ( (a->speed_y < 0) - (a->speed_y > 0) ) * ( a->speed );

			SDL_Delay(50); // freezing 3x0.016s for GAME FEEL!!!!!
		}
		return 1;
	}
	else 
	{
		return 0;
	}
}

void entity_set(Entity* entity, int x, int y, int speed_x, int speed_y, int speed)
{
	entity->x = x;
	entity->y = y;
	entity->speed_x = speed_x;
	entity->speed_y = speed_y;
	entity->speed = speed;
}

int main(int argc, char* args[])
{
	/* Starts the SDL Window & Renderer */
	
	int quit = 0;
	SDL_Event sdlEvent;
	srand( time( NULL ) );

	if ( SDL_Init( SDL_INIT_VIDEO ) < 0)
	{
		print_error( "SDL could not initialize" );
	}
	else
	{
		
		if (!game_window_create( &game_window, "Sodap's Pong", SCREEN_WIDTH, SCREEN_HEIGHT ))
		{
			print_error( "Window could not be created" );
		}
		else
		{
			if ( ( game_renderer = game_window_create_renderer( &game_window ) ) == NULL)
			{
				print_error( "Renderer could not be created" );
			}
			else
			{
				SDL_SetRenderDrawColor( game_renderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
				}
			}
		}
		
		
	}

	Texture paddleTexture;
	Texture ballTexture;
	texture_create(&paddleTexture);
	texture_create(&ballTexture);
	texture_loadFromFile(&paddleTexture,"res/paddle.png");
	texture_loadFromFile(&ballTexture,"res/ball.png");
	Entity player = { SCREEN_WIDTH / 2 - paddleTexture.width/2, SCREEN_HEIGHT - 100, 0, 0, PLAYER_SPEED, &paddleTexture, 0 };
	Entity cpu = { SCREEN_WIDTH / 2 - paddleTexture.width/2, 100, 0, 0, CPU_SPEED, &paddleTexture, 0 };
	Entity ball = { player.x + paddleTexture.width/2 - ballTexture.width/2, player.y-ballTexture.height, 0, 0, BALL_SPEED, &ballTexture, 0 };

	int point_served = 0;
	int rebound_count = 0;
	int restart_game = 0;
	int player_score = 0;
	int cpu_score = 0;
	game_window_update_title( &game_window, "Sodap's pong [ 0 : 0 ]\n" );

	/* MAIN LOOP */
	while ( quit == 0 )
	{
		/* Handle SDL Events */
		while ( SDL_PollEvent( &sdlEvent ) != 0)
		{
			switch (sdlEvent.type)
			{
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_WINDOWEVENT:
					game_window_handle_event(&game_window, &sdlEvent);
					break;
				case SDL_KEYDOWN:
					game_window_handle_event(&game_window, &sdlEvent);
					break;
			}
		}

		//GAME LOGIC ///

		if (restart_game)
		{
			entity_set(&player, SCREEN_WIDTH / 2 - paddleTexture.width/2, SCREEN_HEIGHT - 100, 0, 0, PLAYER_SPEED);
			entity_set(&cpu, SCREEN_WIDTH / 2 - paddleTexture.width/2, 100, 0, 0, CPU_SPEED);
			entity_set(&ball, player.x + paddleTexture.width/2 - ballTexture.width/2, player.y-ballTexture.height, 0, 0, BALL_SPEED);

			point_served = 0;
			rebound_count = 0;
			restart_game = 0;
		}

		/* move player */

		const unsigned char *currentKeyStates = SDL_GetKeyboardState( NULL );
		restart_game = currentKeyStates[ SDL_SCANCODE_F5 ];

		player.speed_x = ( currentKeyStates[ SDL_SCANCODE_RIGHT ] - currentKeyStates[ SDL_SCANCODE_LEFT ] ) * player.speed;
		if(!collide(&player, &ball)) player.x += player.speed_x;
		
		/* constrain player to game bounds */
		player.x < 0 ? 										player.x = 0 : player.x ;
		player.x > SCREEN_WIDTH - player.texture->width ? 	player.x = SCREEN_WIDTH - player.texture->width : player.x ;

		/* move cpu */
		if ((float) rand() / RAND_MAX > CPU_IMPRECISION) //add imprecisions
			cpu.speed_x = ( ( ball.x + CPU_RANGE  > cpu.x ) - ( cpu.x + CPU_RANGE > ball.x ) ) * cpu.speed; // if ball.x is not within CPU_RANGE, move towards it

		if(!collide(&cpu, &ball))	cpu.x += cpu.speed_x;
		cpu.x < 0 ? 									cpu.x = 0 : cpu.x ;
		cpu.x > SCREEN_WIDTH - cpu.texture->width ? 	cpu.x = SCREEN_WIDTH - cpu.texture->width : cpu.x ;

		/* move ball */
		if (!point_served)
		{
			ball.x = player.x + player.texture->width/2 - ball.texture->width/2;
			if (currentKeyStates[ SDL_SCANCODE_RETURN ])
			{
				point_served = 1;
				ball.speed_y = ball.speed;
			}
		}
		else
		{
			if (ball.speed_x == 0 && ball.speed_y == 0)
			{
				ball.speed_y = -ball.speed;
				ball.speed_x = ball.speed;
			}
			else
			{
				ball.x += ball.speed_x;
				ball.y += ball.speed_y;

				ball.speed_x = ball.x > SCREEN_WIDTH - ball.texture->width ? -ball.speed_x : ball.speed_x;
				ball.speed_x = ball.x < 0 ? -ball.speed_x : ball.speed_x;
				ball.x += ( ball.x < 0 ) - (ball.x > SCREEN_WIDTH - ball.texture->width);

				if (ball.y > SCREEN_HEIGHT - ball.texture->height )
				{
					cpu_score++;
					char string[120];
            		sprintf( string, "Sodap's pong [ %d : %d ]\n", player_score, cpu_score );
					game_window_update_title( &game_window, string);
					ball.speed_y = -ball.speed_y;
					restart_game = 1;
				}
				if ( ball.y < 0 )
				{
					player_score++;
					char string[120];
            		sprintf( string, "Sodap's pong [ %d : %d ]\n", player_score, cpu_score );
					game_window_update_title( &game_window, string);
					ball.speed_y = -ball.speed_y;
					restart_game = 1;
				}

				ball.rebound = rebound(&ball,&player) || rebound(&ball,&cpu);		
				rebound_count += ball.rebound;
				if (rebound_count >= 4) 
				{
					ball.speed += ball.rebound;			
					rebound_count = 0;
				}

			}
		/// end of game logic
		}
		


		/* Clear Screen */
		SDL_SetRenderDrawColor( game_renderer, 0x00, 0x00, 0x00, 0xFF );
		SDL_RenderClear( game_renderer );

		/* Render textures */
		//texture_render_ext(&my_texture, 50, 50, NULL, 45, NULL, SDL_FLIP_NONE);
		texture_render(player.texture, player.x, player.y);
		texture_render(cpu.texture, cpu.x, cpu.y);
		texture_render(ball.texture, ball.x, ball.y);

		/* Render */
		SDL_RenderPresent(game_renderer);
	}
	
	/* Cleans the SDL Window and quits*/
	game_window_free(&game_window);
	texture_free(&my_texture);
	SDL_Quit();
	return 0;
}