#include<iostream>
#include<SDL.h>

using namespace std;
bool quit = false;
void Destroy();
SDL_Event event; // SDL built in to identify events
int ballx = 10, bally = 10;
int ballvelx = 1, ballvely = 1;
int bkw = 800, bkh = 600, bkwmin = 0, bkhmin = 0;
int batx = bkw/2, baty = bkh-30; 
int brickw = 80, brickh = 35;
SDL_Surface *brick;
SDL_Texture *bricktexture;
SDL_Surface *ball, *bk, *bat;
SDL_Texture *balltexture, *bktexture, *battexture;
SDL_Renderer * renderer;
SDL_Window *window;
SDL_Rect ballrect;
int delete_brick_count = 0;
int no_of_bricks = 21;
SDL_Rect brickrect[3][7];

void InitializeBrick()
{
	brickrect[0][0] = { 50, 50, brickw, brickh };
	brickrect[0][1] = { 150, 50, brickw, brickh };
	brickrect[0][2] = { 250, 50, brickw, brickh };
	brickrect[0][3] = { 350, 50, brickw, brickh };
	brickrect[0][4] = { 450, 50, brickw, brickh };
	brickrect[0][5] = { 550, 50, brickw, brickh };
	brickrect[0][6] = { 650, 50, brickw, brickh };

	brickrect[1][0] = { 50, 100, brickw, brickh };
	brickrect[1][1] = { 150, 100, brickw, brickh };
	brickrect[1][2] = { 250, 100, brickw, brickh };
	brickrect[1][3] = { 350, 100, brickw, brickh };
	brickrect[1][4] = { 450, 100, brickw, brickh };
	brickrect[1][5] = { 550, 100, brickw, brickh };
	brickrect[1][6] = { 650, 100, brickw, brickh };

	brickrect[2][0] = { 50, 150, brickw, brickh };
	brickrect[2][1] = { 150, 150, brickw, brickh };
	brickrect[2][2] = { 250, 150, brickw, brickh };
	brickrect[2][3] = { 350, 150, brickw, brickh };
	brickrect[2][4] = { 450, 150, brickw, brickh };
	brickrect[2][5] = { 550, 150, brickw, brickh };
	brickrect[2][6] = { 650, 150, brickw, brickh };
}

void EventHandler() { // listens to an event

	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT) { // if the close button is pressed
		quit = true;
	}
	else if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_LEFT && batx > 0)
		{
			batx = batx - 1;
		}
		if (event.key.keysym.sym == SDLK_RIGHT && batx < bkw - 60) // because width of bat is 60
		{
			batx = batx + 1;
		}
	}

}

void gameOver()
{	
	SDL_Surface *go = SDL_LoadBMP("C:/Users/Vaishnavi Kannan/Desktop/SDLimages/3dslogo.bmp");
	SDL_Texture *gotexture = SDL_CreateTextureFromSurface(renderer, go);
	SDL_Rect gorect = { 0,0,bkw, bkh };
	SDL_RenderCopy(renderer, gotexture, NULL, &gorect);
	SDL_RenderPresent(renderer);
	SDL_Delay(20000);
	Destroy();
	SDL_Quit();
}

void moveBall() {
	ballx = ballx + ballvelx;
	bally = bally + ballvely;
}

void ball_collision() {
	if (ballx < bkwmin || ballx>bkw - 30) { // 30 is the width of the ball. we want ball to rebounce when its outer surface hits the wall
		ballvelx = -ballvelx; // reverse the direction
	}
	if (bally < bkhmin) { // 30 is the width of the ball. we want ball to rebounce when its outer surface hits the wall
		ballvely = -ballvely; // reverse the direction
	}
	if (bally > bkh + 60)
	{
		gameOver();
	}
	int ballscaling = 20;
	if (bally + ballscaling >= baty && bally + ballscaling <= baty + 30 && ballx + ballscaling >= batx && ballx + ballscaling <= batx + 60) {
		ballvely = -ballvely;
	}

}

bool ball_brick_collision_detect(SDL_Rect rect1, SDL_Rect rect2)
{
	if (rect1.x > rect2.x + rect2.w)
	{
		return false;
	}
	if (rect1.x + rect1.w < rect2.x)
	{
		return false;
	}
	if (rect1.y > rect2.y + rect2.h)
	{
		return false;
	}
	if (rect1.y + rect1.h < rect2.y)
	{
		return false;
	}
	return true;
}


void ball_brick_collision()
{
	bool a;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			a = ball_brick_collision_detect(brickrect[i][j], ballrect);
			if (a)
			{
				brickrect[i][j].x = 3000;
				ballvely = -ballvely;
				delete_brick_count++;
			}
		}
	}
}

void Destroy()
{
	SDL_DestroyTexture(battexture);
	SDL_DestroyTexture(bricktexture);
	SDL_DestroyTexture(bktexture);
	SDL_DestroyTexture(balltexture);
	SDL_FreeSurface(bat);
	SDL_FreeSurface(brick);
	SDL_FreeSurface(bk);
	SDL_FreeSurface(ball);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

}


void winning()
{
	SDL_Surface *win = SDL_LoadBMP("C:/Users/Vaishnavi Kannan/Desktop/SDLimages/win.bmp");
	SDL_Texture *wintexture = SDL_CreateTextureFromSurface(renderer, win);
	SDL_Rect winrect = { 250, 200, 350, 350 };
	SDL_RenderCopy(renderer, wintexture, NULL, &winrect);
	SDL_RenderPresent(renderer);
	SDL_Delay(10000);
	Destroy();
	SDL_Quit();
}

int main(int argc, char *argv[])
{
	/*if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			cout << "SDL could not be initialised" << SDL_GetError() << endl;
		}
		else
			cout << "SDL ran successfully!!!!!!" << endl;
			*/
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("The Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
	renderer = SDL_CreateRenderer(window, -1, 0); // -1 here means that select the first available display driver and 0 means we dont require any additional parameters on the renderer
	
	SDL_Rect bkrect = { 0,0,800,600 };
	InitializeBrick();
	ball = SDL_LoadBMP("C:/Users/Vaishnavi Kannan/Desktop/SDLimages/ball.bmp");
	bk = SDL_LoadBMP("C:/Users/Vaishnavi Kannan/Desktop/SDLimages/bk.bmp");
	bat = SDL_LoadBMP("C:/Users/Vaishnavi Kannan/Desktop/SDLimages/bat.bmp");
	brick = SDL_LoadBMP("C:/Users/Vaishnavi Kannan/Desktop/SDLimages/brick.bmp");
	balltexture = SDL_CreateTextureFromSurface(renderer, ball);
	bktexture = SDL_CreateTextureFromSurface(renderer, bk);
	battexture = SDL_CreateTextureFromSurface(renderer, bat);
	bricktexture = SDL_CreateTextureFromSurface(renderer, brick);
	SDL_RenderCopy(renderer, bktexture, NULL, &bkrect);
	
	// SDL_Delay(5000);
		
	while (!quit) {
		EventHandler();
		ballrect = { ballx,bally,20,30 };
		SDL_Rect batrect = { batx,baty,60,30 };
		moveBall();
		ball_collision();
		ball_brick_collision();
		if (delete_brick_count == no_of_bricks)
		{
			winning();
		}
		SDL_RenderCopy(renderer, bktexture, NULL, &bkrect);
		SDL_RenderCopy(renderer, balltexture, NULL, &ballrect);
		SDL_RenderCopy(renderer, battexture, NULL, &batrect);

		// Initialize all brick instances
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[0][0]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[0][1]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[0][2]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[0][3]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[0][4]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[0][5]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[0][6]);

		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[1][0]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[1][1]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[1][2]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[1][3]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[1][4]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[1][5]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[1][6]);

		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[2][0]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[2][1]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[2][2]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[2][3]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[2][4]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[2][5]);
		SDL_RenderCopy(renderer, bricktexture, NULL, &brickrect[2][6]);


		SDL_RenderPresent(renderer); // display the image
		SDL_RenderClear(renderer);
	}
	Destroy();
	SDL_Quit();

	return EXIT_SUCCESS;
}