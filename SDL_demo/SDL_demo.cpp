#include "stdafx.h"
#include "draw.h"

bool init();
void close();
void set_S_values(float xS, float yS, int figure_count);
void set_R_values(float angle, int figure_count);
void set_T_values(float xT, float yT, int figure_count);
void init_colours();
void init_SRT_matrices();
void init_matrices();
void alloc_matrices();
void free_matrices();
void recount_F(int target_mtrx_count, int figure_count);

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;
SDL_Surface* loadedSurface = NULL;

Matrix mtrx[DRAWN_FIGURES_COUNT][4];

			// Matrices counts
const int		SCL = 0,
				ROT = 1,
				TRS = 2,
				FIN = 3,
				// Side count limits
				MAX_SIDE_COUNT = 30,
				MIN_SIDE_COUNT = 3, 
				START_SIDE_COUNT = 4,
				// Nested figure count limits
				MAX_FIGURE_COUNT = 50, 
				MIN_FIGURE_COUNT = 2,
				START_FIGURE_COUNT = 9;

			// Matrices initial values
const float		S_START = 1,
				ANGLE_START = 0, 
				XT_START = SCREEN_WIDTH / 2, 
				YT_START = SCREEN_HEIGHT / 2, 
				// Delta values for matrices
				STEP_S = 0.01,
				STEP_T = 3,
				STEP_R = 1;

// Extern global variables from draw.cpp
extern int		figures_side_count[DRAWN_FIGURES_COUNT];
extern int		nested_figure_count[DRAWN_FIGURES_COUNT];
extern Uint32	colours[DRAWN_FIGURES_COUNT];

// Secondary info about figures
int				angles[DRAWN_FIGURES_COUNT];
float			scales[DRAWN_FIGURES_COUNT];
Point			centers[DRAWN_FIGURES_COUNT];

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
		gWindow = SDL_CreateWindow("CGA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
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

				init_colours();
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
	mtrx[figure_count][SCL].set_element(0, 0, xS);
	mtrx[figure_count][SCL].set_element(1, 1, yS);
	mtrx[figure_count][SCL].set_element(2, 2, 1);
}

void set_R_values(float angle, int figure_count) {
	mtrx[figure_count][ROT].set_element(0, 0, cos(RAD(angle)));
	mtrx[figure_count][ROT].set_element(0, 1, -sin(RAD(angle)));
	mtrx[figure_count][ROT].set_element(1, 0, sin(RAD(angle)));
	mtrx[figure_count][ROT].set_element(1, 1, cos(RAD(angle)));
	mtrx[figure_count][ROT].set_element(2, 2, 1);
}

void set_T_values(float xT, float yT, int figure_count) {
	mtrx[figure_count][TRS].set_element(0, 0, 1);
	mtrx[figure_count][TRS].set_element(0, 2, xT);
	mtrx[figure_count][TRS].set_element(1, 1, 1);
	mtrx[figure_count][TRS].set_element(1, 2, yT);
	mtrx[figure_count][TRS].set_element(2, 2, 1);
}

void init_colours() {
	colours[0] = USE_SECTION_WINDOW ? COLOUR_BLUE : COLOUR_GREEN;
	for (int i = 1; i < DRAWN_FIGURES_COUNT; i++) 
		colours[i] = COLOUR_GREEN;
}

void init_SRT_matrices() {
	float xT = USE_SECTION_WINDOW ? XT_START : RADIUS;
	float yT = USE_SECTION_WINDOW ? YT_START : RADIUS;
	for (int i = 0; i < DRAWN_FIGURES_COUNT; i++) {
		set_S_values(S_START, S_START, i);
		set_R_values(ANGLE_START, i);
		set_T_values(xT, yT, i);
		// Seconary info init
		angles[i] = ANGLE_START;
		scales[i] = S_START;
		centers[i].x = xT;
		centers[i].y = yT;
		figures_side_count[i] = START_SIDE_COUNT;
		nested_figure_count[i] = USE_SECTION_WINDOW && i == 0 ? 0 : START_FIGURE_COUNT;
		if (USE_SECTION_WINDOW) {
			xT += 25;
			yT += 25;
		} else {
			xT += RADIUS * 1.5;
			if (xT > SCREEN_WIDTH - (RADIUS * 0.75) - 1) {
				xT = RADIUS;
				yT += RADIUS * 1.5;
				if (yT > SCREEN_HEIGHT - (RADIUS * 0.75) - 1)
					yT = RADIUS;
			}
		}		
	}
}

void init_matrices() {
	init_SRT_matrices();

	Matrix mtrx_tmp = Matrix(3);

	for (int i = 0; i < DRAWN_FIGURES_COUNT; i++) {
		mtrx_tmp = mtrx[i][TRS] * mtrx[i][ROT];
		mtrx[i][FIN] = mtrx[i][SCL] * mtrx_tmp;
	}
}

void alloc_matrices() {
	for (int i = 0; i < DRAWN_FIGURES_COUNT; i++)
		for (int j = 0; j < 4; j++) {
			mtrx[i][j] = Matrix(3);
		}
}

void free_matrices() {
	/*for (int i = 0; i < DRAWN_FIGURES_COUNT; i++)
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 3; k++)
				free(mtrx[i][j][k]);
			free(mtrx[i][j]);
		}*/
}

void recount_F(int target_mtrx_count, int figure_count) {
	mtrx[figure_count][FIN] *= mtrx[figure_count][target_mtrx_count];
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (!init()) {
		printf("Failed to initialize!\n");
	} else {
		loadedSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
			0x00FF0000,  // R
			0x0000FF00,  // G
			0x000000FF,  // B
			0x00000000); // alpha

		gTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if ( NULL == gTexture ) {
			printf("Failed to load media!\n");
		} else {
			SDL_Event e;
			bool quit = false;

			Matrix mtrx_finals[DRAWN_FIGURES_COUNT];
			float step_x, step_y;
			int figure_count = 0;
			bool draw_inside = true;
			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if ( SDL_QUIT == e.type ) {
						quit = true;
					}
					if ( SDL_KEYDOWN == e.type ) {
						int target_mtrx_count = -1;
						switch (e.key.keysym.scancode) {
						// Changing figure count
						case SDL_SCANCODE_MINUS:
							if (figure_count > 0) 
								figure_count--;
							printf("Current figure count = %d\n", figure_count);
							break;
						case SDL_SCANCODE_EQUALS:
							if (figure_count < DRAWN_FIGURES_COUNT - 1)
								figure_count++;
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
							if (figures_side_count[figure_count] > MIN_SIDE_COUNT)
								figures_side_count[figure_count]--;
							printf("Side count = %d\n", figures_side_count[figure_count]);
							break;
						case SDL_SCANCODE_W:
							if (figures_side_count[figure_count] < MAX_SIDE_COUNT)
								figures_side_count[figure_count]++;
							printf("Side count = %d\n", figures_side_count[figure_count]);
							break;
						// Nested figure count changing
						case SDL_SCANCODE_A:
							if (nested_figure_count[figure_count] > MIN_FIGURE_COUNT)
								nested_figure_count[figure_count]--;
							printf("Nested figure count = %d\n", nested_figure_count[figure_count]);
							break;
						case SDL_SCANCODE_S:
							if (nested_figure_count[figure_count] < MAX_FIGURE_COUNT)
								nested_figure_count[figure_count]++;
							printf("Nested figure count = %d\n", nested_figure_count[figure_count]);
							break;
						// Scaling
						case SDL_SCANCODE_SLASH:
							scales[figure_count] += STEP_S;
							set_S_values(S_START + STEP_S, S_START + STEP_S, figure_count);
							printf("Current scale = %f\n", scales[figure_count]);
							target_mtrx_count = SCL;
							break;
						case SDL_SCANCODE_PERIOD:
							scales[figure_count] -= STEP_S;
							set_S_values(S_START - STEP_S, S_START - STEP_S, figure_count);
							printf("Current scale = %f\n", scales[figure_count]);
							target_mtrx_count = SCL;
							break;
						// Moving/Translating
						case SDL_SCANCODE_RIGHT:
							step_x = STEP_T * cos(RAD(angles[figure_count]));
							step_y = -STEP_T * sin(RAD(angles[figure_count]));
							centers[figure_count].x += round(step_x);
							centers[figure_count].y += round(step_y);
							set_T_values(step_x, step_y, figure_count);
							printf("Current center coordinates:\n\tx = %f\n\ty = %f\n", 
								   centers[figure_count].x, centers[figure_count].y);
							target_mtrx_count = TRS;
							break;
						case SDL_SCANCODE_LEFT:
							step_x = -STEP_T * cos(RAD(angles[figure_count]));
							step_y = STEP_T * sin(RAD(angles[figure_count]));
							centers[figure_count].x += round(step_x);
							centers[figure_count].y += round(step_y);
							set_T_values(step_x, step_y, figure_count);
							printf("Current center coordinates:\n\tx = %f\n\ty = %f\n", 
								   centers[figure_count].x, centers[figure_count].y);
							target_mtrx_count = TRS;
							break;
						case SDL_SCANCODE_DOWN:
							step_x = STEP_T * sin(RAD(angles[figure_count]));
							step_y = STEP_T * cos(RAD(angles[figure_count]));
							centers[figure_count].x += round(step_x);
							centers[figure_count].y += round(step_y);
							set_T_values(step_x, step_y, figure_count);
							printf("Current center coordinates:\n\tx = %f\n\ty = %f\n", 
								   centers[figure_count].x, centers[figure_count].y);
							target_mtrx_count = TRS;
							break;
						case SDL_SCANCODE_UP:
							step_x = -STEP_T * sin(RAD(angles[figure_count]));
							step_y = -STEP_T * cos(RAD(angles[figure_count]));
							centers[figure_count].x += round(step_x);
							centers[figure_count].y += round(step_y);
							set_T_values(step_x, step_y, figure_count);
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
				SDL_FillRect(loadedSurface, NULL, COLOUR_BLACK);

				for (int i = 0; i < DRAWN_FIGURES_COUNT; i++)
					mtrx_finals[i] = mtrx[i][FIN];
				draw(loadedSurface, mtrx_finals, draw_inside);

				SDL_UpdateTexture(gTexture, NULL, loadedSurface->pixels, loadedSurface->pitch);
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	close();
	return 0;
}