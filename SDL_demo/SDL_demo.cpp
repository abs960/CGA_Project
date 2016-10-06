#include "stdafx.h"
#include "draw.h"

bool init();
void close();
void set_S_values(float xS, float yS);
void set_R_values(float angle);
void set_T_values(float xT, float yT);
void init_matrices();
void alloc_matrices();
void free_matrices();
void recount_F(int target_mtrx_count);

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;
SDL_Surface* loadedSurface = NULL;

Matrix mtrx[4] = { NULL };

			// Matrices counts
const int	SCL = 0,
			ROT = 1,
			TRS = 2,
			FIN = 3,
			// Side count limits
			MAX_SIDE_COUNT = 20,
			MIN_SIDE_COUNT = 3, 
			START_SIDE_COUNT = 4,
			// Figure count limits
			MAX_FIGURE_COUNT = 20, 
			MIN_FIGURE_COUNT = 2,
			START_FIGURE_COUNT = 9;

const float XS_START = 1,
			YS_START = 1,
			ANGLE_START = 0, 
			XT_START = SCREEN_WIDTH / 2, 
			YT_START = SCREEN_HEIGHT / 2, 
			STEP_S = 0.01,
			STEP_T = 3,
			STEP_R = 1;

bool init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("AKG Lab 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				alloc_matrices();
				init_matrices();
			}
		}
	}
	return success;
}

void close()
{
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	free_matrices();
	SDL_Quit();
}

void set_S_values(float xS, float yS) {
	mtrx[SCL][0][0] = xS;
	mtrx[SCL][1][1] = yS;
	mtrx[SCL][2][2] = 1;
}

void set_R_values(float angle) {
	mtrx[ROT][0][0] = cos(RAD(angle));
	mtrx[ROT][0][1] = -sin(RAD(angle));
	mtrx[ROT][1][0] = sin(RAD(angle));
	mtrx[ROT][1][1] = cos(RAD(angle));
	mtrx[ROT][2][2] = 1;
}

void set_T_values(float xT, float yT) {
	mtrx[TRS][0][0] = 1;
	mtrx[TRS][0][2] = xT;
	mtrx[TRS][1][1] = 1;
	mtrx[TRS][1][2] = yT;
	mtrx[TRS][2][2] = 1;
}

void init_matrices() {
	set_S_values(XS_START, YS_START);
	set_R_values(ANGLE_START);
	set_T_values(XT_START, YT_START);

	Matrix mtrx_tmp = (Matrix)calloc(3, sizeof(float*));
	for (int i = 0; i < 3; i++)
		mtrx_tmp[i] = (float*)calloc(3, sizeof(float));

	mult_mtrx(mtrx[TRS], mtrx[ROT], mtrx_tmp);
	mult_mtrx(mtrx[SCL], mtrx_tmp, mtrx[FIN]);

	for (int i = 0; i < 3; i++)
		free(mtrx_tmp[i]);
	free(mtrx_tmp);
}

void alloc_matrices() {
	for (int i = 0; i < 4; i++) {
		mtrx[i] = (Matrix)calloc(3, sizeof(float*));
		for (int j = 0; j < 3; j++)
			mtrx[i][j] = (float*)calloc(3, sizeof(float));
	}
}

void free_matrices() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++)
			free(mtrx[i][j]);
		free(mtrx[i]);
	}
}

void recount_F(int target_mtrx_count) {
	float** mtrx_tmp = (float**)calloc(3, sizeof(float*));
	for (int i = 0; i < 3; i++)
		mtrx_tmp[i] = (float*)calloc(3, sizeof(float));

	mult_mtrx(mtrx[FIN], mtrx[target_mtrx_count], mtrx_tmp);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			mtrx[FIN][i][j] = mtrx_tmp[i][j];

	for (int i = 0; i < 3; i++)
		free(mtrx_tmp[i]);
	free(mtrx_tmp);
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		loadedSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
			0x000000FF,  // R
			0x0000FF00,  // G
			0x00FF0000,  // B
			0x00000000); // alpha

		gTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if ( NULL == gTexture )
		{
			printf("Failed to load media!\n");
		}
		else
		{
			SDL_Event e;
			bool quit = false;
			int figure_count = START_FIGURE_COUNT;
			int side_count = START_SIDE_COUNT;
			int angle = ANGLE_START;
			float scale = XS_START;
			Point center;
			center.x = XT_START;
			center.y = YT_START;
			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if ( SDL_QUIT == e.type )
					{
						quit = true;
					}
					if ( SDL_KEYDOWN == e.type )
					{
						bool matrix_changed = true;
						int target_mtrx_count = -1;
						switch (e.key.keysym.scancode)
						{
						// Sides count changing
						case SDL_SCANCODE_Q:
							side_count = side_count == MIN_SIDE_COUNT ? MIN_SIDE_COUNT : side_count - 1;
							printf("Side count = %d\n", side_count);
							matrix_changed = false;
							break;
						case SDL_SCANCODE_W:
							side_count = side_count == MAX_SIDE_COUNT ? MAX_SIDE_COUNT : side_count + 1;
							printf("Side count = %d\n", side_count);
							matrix_changed = false;
							break;
						// Figure count changing
						case SDL_SCANCODE_A:
							figure_count = figure_count == MIN_FIGURE_COUNT ? MIN_FIGURE_COUNT : figure_count - 1;
							printf("Figure count = %d\n", figure_count);
							matrix_changed = false;
							break;
						case SDL_SCANCODE_S:
							figure_count = figure_count == MAX_FIGURE_COUNT ? MAX_FIGURE_COUNT : figure_count + 1;
							printf("Figure count = %d\n", figure_count);
							matrix_changed = false;
							break;
						// Scaling
						case SDL_SCANCODE_SLASH:
							scale += STEP_S;
							set_S_values(XS_START + STEP_S, YS_START + STEP_S);
							printf("Current scale = %f\n", scale);
							target_mtrx_count = SCL;
							break;
						case SDL_SCANCODE_PERIOD:
							scale -= STEP_S;
							set_S_values(XS_START - STEP_S, YS_START - STEP_S);
							printf("Current scale = %f\n", scale);
							target_mtrx_count = SCL;
							break;
						// Moving/Translating
						case SDL_SCANCODE_RIGHT:
							center.x += STEP_T;
							set_T_values(STEP_T, 0);
							printf("Current center coordinates:\n\tx = %f\n\ty = %f\n", center.x, center.y);
							target_mtrx_count = TRS;
							break;
						case SDL_SCANCODE_LEFT:
							center.x -= STEP_T;
							set_T_values(-STEP_T, 0);
							printf("Current center coordinates:\n\tx = %f\n\ty = %f\n", center.x, center.y);
							target_mtrx_count = TRS;
							break;
						case SDL_SCANCODE_DOWN:
							center.y += STEP_T;
							set_T_values(0, STEP_T);
							printf("Current center coordinates:\n\tx = %f\n\ty = %f\n", center.x, center.y);
							target_mtrx_count = TRS;
							break;
						case SDL_SCANCODE_UP:
							center.y -= STEP_T;
							set_T_values(0, -STEP_T);
							printf("Current center coordinates:\n\tx = %f\n\ty = %f\n", center.x, center.y);
							target_mtrx_count = TRS;
							break;
						// Rotating
						case SDL_SCANCODE_X:
							angle += STEP_R;
							set_R_values(STEP_R);
							printf("Current angle = %d\n", angle);
							target_mtrx_count = ROT;
							break;
						case SDL_SCANCODE_Z:
							angle -= STEP_R;
							set_R_values(-STEP_R);
							printf("Current angle = %d\n", angle);
							target_mtrx_count = ROT;
							break;
						default:
							matrix_changed = false;
						}

						if (matrix_changed)
							recount_F(target_mtrx_count);
					}
				}
				SDL_RenderClear(gRenderer);

				// Clearing the surface
				SDL_FillRect(loadedSurface, NULL, 0x00000000);
				
				draw(loadedSurface, mtrx[FIN], side_count, figure_count);

				SDL_UpdateTexture(gTexture, NULL, loadedSurface->pixels, loadedSurface->pitch);
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	close();
	return 0;
}