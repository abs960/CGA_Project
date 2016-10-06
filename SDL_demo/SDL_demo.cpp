#include "stdafx.h"
#include "draw.h"

bool init();
void close();
void set_S_values(float xS, float yS, int figure_count);
void set_R_values(float angle, int figure_count);
void set_T_values(float xT, float yT, int figure_count);
void init_SRT_matrices();
void init_matrices();
void alloc_matrices();
void free_matrices();
void recount_F(int target_mtrx_count, int figure_count);

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;
SDL_Surface* loadedSurface = NULL;

Matrix mtrx[DRAWN_FIGURES_COUNT][4] = { NULL };

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

			// Matrices initial values
const float XS_START = 1,
			YS_START = 1,
			ANGLE_START = 0, 
			XT_START = RADIUS, 
			YT_START = RADIUS, 
			// Delta values for matrices
			STEP_S = 0.01,
			STEP_T = 3,
			STEP_R = 1;

int			side_count;

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
		gWindow = SDL_CreateWindow("AKG Lab", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
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

void set_S_values(float xS, float yS, int figure_count) {
	mtrx[figure_count][SCL][0][0] = xS;
	mtrx[figure_count][SCL][1][1] = yS;
	mtrx[figure_count][SCL][2][2] = 1;
}

void set_R_values(float angle, int figure_count) {
	mtrx[figure_count][ROT][0][0] = cos(RAD(angle));
	mtrx[figure_count][ROT][0][1] = -sin(RAD(angle));
	mtrx[figure_count][ROT][1][0] = sin(RAD(angle));
	mtrx[figure_count][ROT][1][1] = cos(RAD(angle));
	mtrx[figure_count][ROT][2][2] = 1;
}

void set_T_values(float xT, float yT, int figure_count) {
	mtrx[figure_count][TRS][0][0] = 1;
	mtrx[figure_count][TRS][0][2] = xT;
	mtrx[figure_count][TRS][1][1] = 1;
	mtrx[figure_count][TRS][1][2] = yT;
	mtrx[figure_count][TRS][2][2] = 1;
}

void init_SRT_matrices() {
	float xT = XT_START;
	float yT = YT_START;
	for (int i = 0; i < DRAWN_FIGURES_COUNT; i++) {
		set_S_values(XS_START, YS_START, i);
		set_R_values(ANGLE_START, i);
		set_T_values(xT, yT, i);

		// Lazy solution for section window
		xT += XT_START / 2;
		yT += YT_START / 2;

		// Not so lazy solution for drawing of multple figures
		/*xT += XT_START;
		if (xT > SCREEN_WIDTH - XT_START - 1) {
			xT = XT_START;
			yT += YT_START;
			if (yT > SCREEN_HEIGHT - YT_START - 1)
				yT = YT_START;
		}*/
		// Uncomment one of these
	}
}

void init_matrices() {
	init_SRT_matrices();

	Matrix mtrx_tmp = (Matrix)calloc(3, sizeof(float*));
	for (int i = 0; i < 3; i++)
		mtrx_tmp[i] = (float*)calloc(3, sizeof(float));

	for (int i = 0; i < DRAWN_FIGURES_COUNT; i++) {
		mult_mtrx(mtrx[i][TRS], mtrx[i][ROT], mtrx_tmp);
		mult_mtrx(mtrx[i][SCL], mtrx_tmp, mtrx[i][FIN]);
	}

	for (int i = 0; i < 3; i++)
		free(mtrx_tmp[i]);
	free(mtrx_tmp);
}

void alloc_matrices() {
	for (int i = 0; i < DRAWN_FIGURES_COUNT; i++)
		for (int j = 0; j < 4; j++) {
			mtrx[i][j] = (Matrix)calloc(3, sizeof(float*));
			for (int k = 0; k < 3; k++)
				mtrx[i][j][k] = (float*)calloc(3, sizeof(float));
		}
}

void free_matrices() {
	for (int i = 0; i < DRAWN_FIGURES_COUNT; i++)
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 3; k++)
				free(mtrx[i][j][k]);
			free(mtrx[i][j]);
		}
}

void recount_F(int target_mtrx_count, int figure_count) {
	float** mtrx_tmp = (float**)calloc(3, sizeof(float*));
	for (int i = 0; i < 3; i++)
		mtrx_tmp[i] = (float*)calloc(3, sizeof(float));

	mult_mtrx(mtrx[figure_count][FIN], mtrx[figure_count][target_mtrx_count], mtrx_tmp);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			mtrx[figure_count][FIN][i][j] = mtrx_tmp[i][j];

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
		else {
			SDL_Event e;
			bool quit = false;

			int nested_figure_count = START_FIGURE_COUNT;
			side_count = START_SIDE_COUNT;

			// Info about figures
			int angles[DRAWN_FIGURES_COUNT] = { ANGLE_START };
			float scales[DRAWN_FIGURES_COUNT] = { XS_START };
			Point centers[DRAWN_FIGURES_COUNT];
			for (int i = 0; i < DRAWN_FIGURES_COUNT; i++) {
				centers[i].x = XT_START;
				centers[i].y = YT_START;
			}

			int figure_count = 0;
			bool draw_inside = true;
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
						int target_mtrx_count = -1;
						switch (e.key.keysym.scancode)
						{
						// Changing figure count
						case SDL_SCANCODE_MINUS:
							figure_count = figure_count == 0 ? 0 : figure_count - 1;
							printf("Current figure count = %d\n", figure_count);
							break;
						case SDL_SCANCODE_EQUALS:
							figure_count = figure_count == DRAWN_FIGURES_COUNT - 1 ? DRAWN_FIGURES_COUNT - 1 : figure_count + 1;
							printf("Current figure count = %d\n", figure_count);
							break;
						// Do drawing inside section window
						case SDL_SCANCODE_V:
							draw_inside = !draw_inside;
							if (draw_inside)
								printf("Currently drawing inside section window\n");
							else
								printf("Currently drawing outside section window\n");
							break;
						// Sides count changing
						case SDL_SCANCODE_Q:
							side_count = side_count == MIN_SIDE_COUNT ? MIN_SIDE_COUNT : side_count - 1;
							printf("Side count = %d\n", side_count);
							break;
						case SDL_SCANCODE_W:
							side_count = side_count == MAX_SIDE_COUNT ? MAX_SIDE_COUNT : side_count + 1;
							printf("Side count = %d\n", side_count);
							break;
						// Figure count changing
						case SDL_SCANCODE_A:
							nested_figure_count = nested_figure_count == MIN_FIGURE_COUNT ? MIN_FIGURE_COUNT : nested_figure_count - 1;
							printf("Nested figure count = %d\n", nested_figure_count);
							break;
						case SDL_SCANCODE_S:
							nested_figure_count = nested_figure_count == MAX_FIGURE_COUNT ? MAX_FIGURE_COUNT : nested_figure_count + 1;
							printf("Nested figure count = %d\n", nested_figure_count);
							break;
						// Scaling
						case SDL_SCANCODE_SLASH:
							scales[figure_count] += STEP_S;
							set_S_values(XS_START + STEP_S, YS_START + STEP_S, figure_count);
							printf("Current scale = %f\n", scales[figure_count]);
							target_mtrx_count = SCL;
							break;
						case SDL_SCANCODE_PERIOD:
							scales[figure_count] -= STEP_S;
							set_S_values(XS_START - STEP_S, YS_START - STEP_S, figure_count);
							printf("Current scale = %f\n", scales[figure_count]);
							target_mtrx_count = SCL;
							break;
						// Moving/Translating
						case SDL_SCANCODE_RIGHT:
							centers[figure_count].x += STEP_T;
							set_T_values(STEP_T, 0, figure_count);
							printf("Current center coordinates:\n\tx = %f\n\ty = %f\n", 
								   centers[figure_count].x, centers[figure_count].y);
							target_mtrx_count = TRS;
							break;
						case SDL_SCANCODE_LEFT:
							centers[figure_count].x -= STEP_T;
							set_T_values(-STEP_T, 0, figure_count);
							printf("Current center coordinates:\n\tx = %f\n\ty = %f\n", 
								   centers[figure_count].x, centers[figure_count].y);
							target_mtrx_count = TRS;
							break;
						case SDL_SCANCODE_DOWN:
							centers[figure_count].y += STEP_T;
							set_T_values(0, STEP_T, figure_count);
							printf("Current center coordinates:\n\tx = %f\n\ty = %f\n", 
								   centers[figure_count].x, centers[figure_count].y);
							target_mtrx_count = TRS;
							break;
						case SDL_SCANCODE_UP:
							centers[figure_count].y -= STEP_T;
							set_T_values(0, -STEP_T, figure_count);
							printf("Current center coordinates:\n\tx = %f\n\ty = %f\n", 
								   centers[figure_count].x, centers[figure_count].y);
							target_mtrx_count = TRS;
							break;
						// Rotating
						case SDL_SCANCODE_X:
							angles[figure_count] += STEP_R;
							set_R_values(STEP_R, figure_count);
							printf("Current angle = %d\n", angles[figure_count]);
							target_mtrx_count = ROT;
							break;
						case SDL_SCANCODE_Z:
							angles[figure_count] -= STEP_R;
							set_R_values(-STEP_R, figure_count);
							printf("Current angle = %d\n", angles[figure_count]);
							target_mtrx_count = ROT;
							break;
						default:
							target_mtrx_count = -1;
						}

						if (target_mtrx_count != -1)
							recount_F(target_mtrx_count, figure_count);
					}
				}
				SDL_RenderClear(gRenderer);

				// Clearing the surface
				SDL_FillRect(loadedSurface, NULL, 0x00000000);
				
				Matrix mtrx_finals[DRAWN_FIGURES_COUNT];
				for (int i = 0; i < DRAWN_FIGURES_COUNT; i++)
					mtrx_finals[i] = mtrx[i][FIN];

				draw(loadedSurface, mtrx_finals, nested_figure_count, draw_inside);

				SDL_UpdateTexture(gTexture, NULL, loadedSurface->pixels, loadedSurface->pitch);
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	close();
	return 0;
}