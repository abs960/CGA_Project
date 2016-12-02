#include "stdafx.h"
#include "draw.h"
#include "Matrix.h"
#include "Point.h"
#include "Colour.h"
#include "BrezenheimLine.h"
#include "WuLine.h"
#include "NestedShape2D.h"
#include "CyrusBeckLine.h"
#include "Scene3D.h"

bool init();
void init_shapes();
void init_matrices();
void destroy_shapes();
void close();

void add_3d_objects(int layer_count);

const bool USES_3D = true;

const int
	// Transformation matrices counts
	SCL = 0,
	ROT = 1,
	TRS = 2,
	FIN = 3,
	// Side count limits
	MAX_SIDE_COUNT = 50,
	MIN_SIDE_COUNT = 3,
	START_SIDE_COUNT = 4,
	// Nested figure count limits
	MAX_NESTED_COUNT = 100,
	MIN_NESTED_COUNT = 2,
	START_NESTED_COUNT = 9,
	// Shapes count limits
	MIN_SHAPES_COUNT = 1,
	MAX_SHAPES_COUNT = 10,
	// Section windows count limits
	MIN_SECTION_COUNT = 1,
	MAX_SECTION_COUNT = 5;
		
const float	
	// Matrices initial values
	SCALE_START = 1,
	ANGLE_START = 0, 
	XT_START = SCREEN_WIDTH / 2, 
	YT_START = SCREEN_HEIGHT / 2, 
	// Steps values for matrices
	STEP_S = 0.01,
	STEP_T = 5,
	STEP_R = 2;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;
SDL_Surface* loadedSurface = NULL;

MatrixShape2D** shapes;
int	shapes_count,
	sections_count,
	total_count;

Scene3D* scene3d;

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

				init_shapes();
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
	destroy_shapes();
	SDL_Quit();
}

void init_shapes() {
	if (USES_3D) {
		scene3d = new Scene3D();
		scene3d->set_base_line(new WuLine());
		scene3d->set_colour(Colour(Colour::COLOUR_GREEN));
	} else {
		shapes_count = MIN_SHAPES_COUNT;
		sections_count = MIN_SECTION_COUNT;
		total_count = shapes_count + sections_count;
		shapes = new MatrixShape2D*[total_count];

		for (int i = 0; i < sections_count; i++) {
			SectionWindow* sw = new SectionWindow(START_SIDE_COUNT, SIDE_LENGTH * 2);
			sw->set_transparent(true);
			sw->set_colour(Colour(Colour::COLOUR_BLUE));
			sw->set_brush(new WuLine());
			shapes[i] = sw;
		}

		for (int i = sections_count; i < total_count; i++) {
			CyrusBeckLine* l = new CyrusBeckLine();
			l->set_basic_brush(new WuLine());
			for (int j = 0; j < sections_count; j++) {
				SectionWindow* sw = dynamic_cast<SectionWindow*>(shapes[j]);
				l->push_section_window(sw);
			}

			NestedShape2D* s = new NestedShape2D(START_SIDE_COUNT, SIDE_LENGTH);
			s->set_nested_count(START_NESTED_COUNT);
			s->set_colour(Colour(Colour::COLOUR_GREEN));
			s->set_brush(l);
			shapes[i] = s;
		}

		init_matrices();
	}
}

void init_matrices() {
	Matrix s = Matrix(3);
	s.set_element(0, 0, SCALE_START);
	s.set_element(1, 1, SCALE_START);
	s.set_element(2, 2, 1);
	Matrix r = Matrix(3);
	r.set_element(0, 0, cos(RAD(ANGLE_START)));
	r.set_element(0, 1, -sin(RAD(ANGLE_START)));
	r.set_element(1, 0, sin(RAD(ANGLE_START)));
	r.set_element(1, 1, cos(RAD(ANGLE_START)));
	r.set_element(2, 2, 1);
	double xt = XT_START, yt = YT_START;
	Matrix t = Matrix(3);
	t.set_element(0, 0, 1);
	t.set_element(1, 1, 1);
	t.set_element(2, 2, 1);
	for (int i = 0; i < total_count; i++) {
		t.set_element(0, 2, xt);
		t.set_element(1, 2, yt);

		Matrix* tmp = new Matrix(3);
		*tmp = t * r * s;
		shapes[i]->set_matrix(tmp);
		shapes[i]->set_scale(SCALE_START);
		shapes[i]->set_angle(ANGLE_START);
		shapes[i]->set_center(Point(xt, yt));
		xt += XT_START / 10;
		yt += YT_START / 10;
	}
}

void destroy_shapes() {
	if (USES_3D) {
		delete scene3d;
	} else {
		for (int i = 0; i < total_count; i++)
			delete shapes[i];
		delete[] shapes;
	}
}

void add_3d_objects(int layer_count) {
	scene3d->clear_scene();
	/*for (int i = 0; i < layer_count; i++) {
		float x = 0;
		float y = (layer_count - i) * SIDE_LENGTH;
		float z = (layer_count - i + 1) * SIDE_LENGTH;

		for (int j = 0; j < i; j++) {
			Point pivot = Point();


			Shape3D* shape = new Shape3D(pivot, SIDE_LENGTH);
			scene3d->add_object(shape);
		}
	}*/

	scene3d->add_object(new Shape3D(Point(0, -2 * SIDE_LENGTH, SIDE_LENGTH), SIDE_LENGTH));
	scene3d->add_object(new Shape3D(Point(0, -SIDE_LENGTH, 2 * SIDE_LENGTH), SIDE_LENGTH));
	scene3d->add_object(new Shape3D(Point(0, -SIDE_LENGTH, SIDE_LENGTH), SIDE_LENGTH));
	scene3d->add_object(new Shape3D(Point(SIDE_LENGTH, -SIDE_LENGTH, SIDE_LENGTH), SIDE_LENGTH));
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
			int chosen_shape_count = 0;
			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (SDL_QUIT == e.type) {
						quit = true;
					}
					if (SDL_KEYDOWN == e.type) {
						if (USES_3D) {
							switch (e.key.keysym.scancode) {
							case SDL_SCANCODE_J:
								scene3d->move(-STEP_T, 0, 0);
								printf("left\n");
								break;
							case SDL_SCANCODE_L:
								scene3d->move(STEP_T, 0, 0);
								printf("right\n");
								break;
							case SDL_SCANCODE_I:
								scene3d->move(0, -STEP_T, 0);
								printf("up\n");
								break;
							case SDL_SCANCODE_K:
								scene3d->move(0, STEP_T, 0);
								printf("down\n");
								break;
							case SDL_SCANCODE_U:
								scene3d->move(0, 0, -STEP_T);
								printf("out\n");
								break;
							case SDL_SCANCODE_O:
								scene3d->move(0, 0, STEP_T);
								printf("in\n");
								break;
							case SDL_SCANCODE_S:
								scene3d->rotate_x(STEP_R);
								printf("+x\n");
								break;
							case SDL_SCANCODE_W:
								scene3d->rotate_x(-STEP_R);
								printf("-x\n");
								break;
							case SDL_SCANCODE_D:
								scene3d->rotate_y(STEP_R);
								printf("+y\n");
								break;
							case SDL_SCANCODE_A:
								scene3d->rotate_y(-STEP_R);
								printf("-y\n");
								break;
							case SDL_SCANCODE_E:
								scene3d->rotate_z(STEP_R);
								printf("+z\n");
								break;
							case SDL_SCANCODE_Q:
								scene3d->rotate_z(-STEP_R);
								printf("-z\n");
								break;
							case SDL_SCANCODE_PERIOD:
								scene3d->scale(1-STEP_S);
								printf("-s\n");
								break;
							case SDL_SCANCODE_SLASH:
								scene3d->scale(1+STEP_S);
								printf("+s\n");
								break;
							/*case SDL_SCANCODE_Z:
								shape3d->rotateAroundEdge(-2);
								printf("");
								break;
							case SDL_SCANCODE_C:
								shape3d->rotateAroundEdge(2);
								printf("");
								break;*/
							}
						} else {
							int sides;
							float step_x;
							float step_y;
							switch (e.key.keysym.scancode) {
								// Changing figure count
							case SDL_SCANCODE_MINUS:
								if (chosen_shape_count > 0)
									chosen_shape_count--;
								printf("Current figure count = %d\n", chosen_shape_count);
								break;
							case SDL_SCANCODE_EQUALS:
								if (chosen_shape_count < total_count - 1)
									chosen_shape_count++;
								printf("Current figure count = %d\n", chosen_shape_count);
								break;
								// Do drawing inside section window
							case SDL_SCANCODE_V:
								if (chosen_shape_count < sections_count) {
									if (SectionWindow* sw = dynamic_cast<SectionWindow*>(shapes[chosen_shape_count])) {
										bool trans = sw->is_transparent();
										sw->set_transparent(!trans);
										if (trans)
											printf("Currently drawing outside section window\n");
										else
											printf("Currently drawing inside section window\n");
									}
								}
								break;
								// Sides count changing
							case SDL_SCANCODE_Q:
								sides = shapes[chosen_shape_count]->get_side_count();
								if (sides > MIN_SIDE_COUNT) {
									sides--;
									shapes[chosen_shape_count]->set_side_count(sides);
								}
								printf("Side count = %d\n", sides);
								break;
							case SDL_SCANCODE_W:
								sides = shapes[chosen_shape_count]->get_side_count();
								if (sides < MAX_SIDE_COUNT) {
									sides++;
									shapes[chosen_shape_count]->set_side_count(sides);
								}
								printf("Side count = %d\n", sides);
								break;
								// Nested count changing
							case SDL_SCANCODE_A:
								if (chosen_shape_count >= sections_count) {
									if (NestedShape2D* ns = dynamic_cast<NestedShape2D*>(shapes[chosen_shape_count])) {
										int nested = ns->get_nested_count();
										if (nested > MIN_NESTED_COUNT) {
											nested--;
											ns->set_nested_count(nested);
										}
										printf("Nested figure count = %d\n", nested);
									}
								}
								break;
							case SDL_SCANCODE_S:
								if (chosen_shape_count >= sections_count) {
									if (NestedShape2D* ns = dynamic_cast<NestedShape2D*>(shapes[chosen_shape_count])) {
										int nested = ns->get_nested_count();
										if (nested < MAX_NESTED_COUNT) {
											nested++;
											ns->set_nested_count(nested);
										}
										printf("Nested figure count = %d\n", nested);
									}
								}
								break;
								// Scaling
							case SDL_SCANCODE_SLASH:
								shapes[chosen_shape_count]->do_scale(STEP_S);
								printf("Current scale = %f\n", shapes[chosen_shape_count]->get_scale());
								break;
							case SDL_SCANCODE_PERIOD:
								shapes[chosen_shape_count]->do_scale(-STEP_S);
								printf("Current scale = %f\n", shapes[chosen_shape_count]->get_scale());
								break;
								// Moving/Translating
							case SDL_SCANCODE_RIGHT:
								step_x = STEP_T * cos(RAD(shapes[chosen_shape_count]->get_angle()));
								step_y = -STEP_T * sin(RAD(shapes[chosen_shape_count]->get_angle()));
								shapes[chosen_shape_count]->do_move(step_x, step_y);
								printf("Current center coordinates:\n\tx = %f\n\ty = %f\n",
									   shapes[chosen_shape_count]->get_center().x, shapes[chosen_shape_count]->get_center().y);
								break;
							case SDL_SCANCODE_LEFT:
								step_x = -STEP_T * cos(RAD(shapes[chosen_shape_count]->get_angle()));
								step_y = STEP_T * sin(RAD(shapes[chosen_shape_count]->get_angle()));
								shapes[chosen_shape_count]->do_move(step_x, step_y);
								printf("Current center coordinates:\n\tx = %f\n\ty = %f\n",
									   shapes[chosen_shape_count]->get_center().x, shapes[chosen_shape_count]->get_center().y);
								break;
							case SDL_SCANCODE_DOWN:
								step_x = STEP_T * sin(RAD(shapes[chosen_shape_count]->get_angle()));
								step_y = STEP_T * cos(RAD(shapes[chosen_shape_count]->get_angle()));
								shapes[chosen_shape_count]->do_move(step_x, step_y);
								printf("Current center coordinates:\n\tx = %f\n\ty = %f\n",
									   shapes[chosen_shape_count]->get_center().x, shapes[chosen_shape_count]->get_center().y);
								break;
							case SDL_SCANCODE_UP:
								step_x = -STEP_T * sin(RAD(shapes[chosen_shape_count]->get_angle()));
								step_y = -STEP_T * cos(RAD(shapes[chosen_shape_count]->get_angle()));
								shapes[chosen_shape_count]->do_move(step_x, step_y);
								printf("Current center coordinates:\n\tx = %f\n\ty = %f\n",
									   shapes[chosen_shape_count]->get_center().x, shapes[chosen_shape_count]->get_center().y);
								break;
								// Rotating
							case SDL_SCANCODE_X:
								shapes[chosen_shape_count]->do_rotate(STEP_R);
								printf("Current angle = %d\n", shapes[chosen_shape_count]->get_angle());
								break;
							case SDL_SCANCODE_Z:
								shapes[chosen_shape_count]->do_rotate(-STEP_R);
								printf("Current angle = %d\n", shapes[chosen_shape_count]->get_angle());
								break;
							}
						}
					}
				}
				SDL_RenderClear(gRenderer);

				// Clearing the surface
				SDL_FillRect(loadedSurface, NULL, Colour::COLOUR_BLACK);

				if (USES_3D) {
					add_3d_objects(2);
					scene3d->draw(loadedSurface);
				} else {
					for (int i = 0; i < total_count; i++)
						shapes[i]->draw(loadedSurface);
				}

				SDL_UpdateTexture(gTexture, NULL, loadedSurface->pixels, loadedSurface->pitch);
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	close();
	return 0;
}