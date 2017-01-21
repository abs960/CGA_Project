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
#include "QuaternionScene3D.h"
#include "PerspectiveShape3D.h"
#include "Window.h"

const bool TESTING_WINDOW_CLASS = false;

bool init();
void init_shapes();
void init_matrices();
void destroy_shapes();
void close();

void add_3d_objects(Scene3D* scene, int layer_count, int length);

Colour BACKGROUND = Colour(Colour::COLOUR_BLACK);
Vector ROTATION = Vector(300, 150, 225);

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
	MAX_SECTION_COUNT = 5,
	// Layers count in 3D scene limits
	MIN_LAYER_COUNT = 1,
	MAX_LAYER_COUNT = 10,
	START_LAYER_COUNT = 2,
	// 3D Task parameters
	MAX_TASK_COUNT = 3,
	START_SHAPE_LENGTH = 3,
	MAX_SHAPE_LENGTH = 10,
	MIN_SHAPE_LENGTH = 1;
		
const float	
	// Matrices initial values
	SCALE_START = 1,
	ANGLE_START = 0, 
	XT_START = 0, 
	YT_START = 0, 
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
	total_count,
	task_count;

Scene3D* left_scene;
Scene3D* right_scene;

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
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
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

void init_shapes() {
	if (USES_3D) {
		QuaternionScene3D* q = new QuaternionScene3D();
		left_scene = q;
		left_scene->set_base_line(new WuLine());
		left_scene->set_colour(Colour(Colour::COLOUR_GREEN));
		add_3d_objects(left_scene, START_LAYER_COUNT, START_SHAPE_LENGTH);
		left_scene->move(-200, 0, 0);

		right_scene = new QuaternionScene3D();
		right_scene->set_base_line(new WuLine());
		right_scene->set_colour(Colour(Colour::COLOUR_GREEN));
		add_3d_objects(right_scene, START_LAYER_COUNT, START_SHAPE_LENGTH);
		right_scene->move(200, 0, 0);
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
		delete left_scene;
		delete right_scene;
	} else {
		for (int i = 0; i < total_count; i++)
			delete shapes[i];
		delete[] shapes;
	}
}

void close() {
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	destroy_shapes();
	SDL_Quit();
}

void add_3d_objects(Scene3D* scene, int layer_count, int length) {
	scene->clear_scene();
	switch (task_count) {
	case 0:
		// stairs
		for (int i = 0; i < layer_count; i++) {
			int y = -(layer_count - i) * SIDE_LENGTH;
			int x = -(layer_count * SIDE_LENGTH) / 2;
			for (int j = 0; j < i + 1; j++) {
				x += SIDE_LENGTH;
				for (int k = 0; k < length; k++) {
					int z = k * SIDE_LENGTH;
					scene->add_object(new PerspectiveShape3D(Point(x, y, z), SIDE_LENGTH));
				}
			}
		}
		break;
	case 1:
		// cube pyramid
		for (int i = 0; i < layer_count; i++) {
			int y = -(layer_count - i) * SIDE_LENGTH;
			int x = -(layer_count * SIDE_LENGTH) / 2;
			for (int j = 0; j < i + 1; j++) {
				x += SIDE_LENGTH;
				for (int k = i - j + 1; k > 0; k--) {
					int z = k * SIDE_LENGTH;
					scene->add_object(new PerspectiveShape3D(Point(x, y, z), SIDE_LENGTH));
				}
			}
		}
		break;
	case 2:
		// cube with a hole
		for (int i = 0; i < layer_count + 2; i++) {
			int y = -i * SIDE_LENGTH;
			if (i == 0 || i == layer_count + 1) {
				int x = -((layer_count + 2) * SIDE_LENGTH) / 2;
				for (int j = 0; j < layer_count + 2; j++) {
					for (int k = 0; k < length; k++) {
						int z = k * SIDE_LENGTH;
						scene->add_object(new PerspectiveShape3D(Point(x, y, z), SIDE_LENGTH));
					}
					x += SIDE_LENGTH;
				}
			} else {
				int x = -((layer_count + 2) * SIDE_LENGTH) / 2;
				for (int j = 0; j < 2; j++) {
					for (int k = 0; k < length; k++) {
						int z = k * SIDE_LENGTH;
						scene->add_object(new PerspectiveShape3D(Point(x, y, z), SIDE_LENGTH));
					}
					x += SIDE_LENGTH * (layer_count + 1);
				}
			}
		}
		break;
	case 3:
		// spiral
		bool b = true;
		bool b0 = true;
		for (int i = 0; i < layer_count; i++) {
			int y = -(layer_count - i) * SIDE_LENGTH;
			if (i % 2 == 0) {
				int x = -SIDE_LENGTH + (-length * SIDE_LENGTH) / 2;
				for (int j = 0; j < length + 1; j++) {
					x += SIDE_LENGTH;
					int z = b ? 0 : (length) * SIDE_LENGTH;
					scene->add_object(new PerspectiveShape3D(Point(x, y, z), SIDE_LENGTH));
				}
				b = !b;
			} else {
				int x = b0 ? 0 : (length) * SIDE_LENGTH;
				x += (-length * SIDE_LENGTH) / 2;
				for (int j = 0; j < length + 1; j++) {
					int z = j * SIDE_LENGTH;
					scene->add_object(new PerspectiveShape3D(Point(x, y, z), SIDE_LENGTH));
				}
				b0 = !b0;
			}
		}
		break;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (TESTING_WINDOW_CLASS) {

		Window * wnd = new Window();
		do {
			if (!wnd->init()) {
				break;
			}

			if (!wnd->run()) {
				break;
			}

		} while (false);

		wnd->close();
		delete wnd;

		getchar();

	} else {

		if (!init()) {
			printf("Failed to initialize!\n");
			close();
			return -1;
		}

		loadedSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
											 0x00FF0000,  // R
											 0x0000FF00,  // G
											 0x000000FF,  // B
											 0x00000000); // alpha

		gTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (NULL == gTexture) {
			printf("Failed to load media!\n");
			close();
			return -1;
		}

		SDL_Event e;
		bool quit = false;
		int chosen_shape_count = 0;
		int layer_count = START_LAYER_COUNT;
		int length = START_SHAPE_LENGTH;
		task_count = 1;
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (SDL_QUIT == e.type) {
					quit = true;
				}
				if (SDL_KEYDOWN == e.type) {
					if (USES_3D) {
						switch (e.key.keysym.scancode) {
						case SDL_SCANCODE_UP:
							if (layer_count < MAX_LAYER_COUNT) {
								layer_count++;
							}
							printf("Layers - %d\n", layer_count);
							break;
						case SDL_SCANCODE_DOWN:
							if (layer_count > MIN_LAYER_COUNT) {
								layer_count--;
							}
							printf("Layers - %d\n", layer_count);
							break;
						case SDL_SCANCODE_RIGHT:
							if (length < MAX_SHAPE_LENGTH) {
								length++;
							}
							printf("Length - %d\n", length);
							break;
						case SDL_SCANCODE_LEFT:
							if (length > MIN_SHAPE_LENGTH) {
								length--;
							}
							printf("Length - %d\n", length);
							break;
						case SDL_SCANCODE_B:
							left_scene->set_lines_visible(true);
							left_scene->set_base_line(new BrezenheimLine());
							right_scene->set_lines_visible(true);
							right_scene->set_base_line(new BrezenheimLine());
							printf("Is using Brezenheim algorithm\n");
							break;
						case SDL_SCANCODE_V:
							left_scene->set_lines_visible(true);
							left_scene->set_base_line(new WuLine());
							right_scene->set_lines_visible(true);
							right_scene->set_base_line(new WuLine());
							printf("Is using Wu algorithm\n");
							break;
						case SDL_SCANCODE_N:
							left_scene->set_lines_visible(false);
							right_scene->set_lines_visible(false);
							printf("Lines hidden\n");
							break;
						case SDL_SCANCODE_J:
							left_scene->move(-STEP_T, 0, 0);
							right_scene->move(STEP_T, 0, 0);
							printf("left\n");
							break;
						case SDL_SCANCODE_L:
							left_scene->move(STEP_T, 0, 0);
							right_scene->move(-STEP_T, 0, 0);
							printf("right\n");
							break;
						case SDL_SCANCODE_I:
							left_scene->move(0, -STEP_T, 0);
							right_scene->move(0, -STEP_T, 0);
							printf("up\n");
							break;
						case SDL_SCANCODE_K:
							left_scene->move(0, STEP_T, 0);
							right_scene->move(0, STEP_T, 0);
							printf("down\n");
							break;
						case SDL_SCANCODE_U:
							left_scene->move(0, 0, -STEP_T);
							right_scene->move(0, 0, -STEP_T);
							printf("out\n");
							break;
						case SDL_SCANCODE_O:
							left_scene->move(0, 0, STEP_T);
							right_scene->move(0, 0, STEP_T);
							printf("in\n");
							break;
						case SDL_SCANCODE_S:
							left_scene->rotate_x(STEP_R);
							right_scene->rotate_x(STEP_R);
							printf("+x\n");
							break;
						case SDL_SCANCODE_W:
							left_scene->rotate_x(-STEP_R);
							right_scene->rotate_x(-STEP_R);
							printf("-x\n");
							break;
						case SDL_SCANCODE_D:
							left_scene->rotate_y(STEP_R);
							right_scene->rotate_y(STEP_R);
							printf("+y\n");
							break;
						case SDL_SCANCODE_A:
							left_scene->rotate_y(-STEP_R);
							right_scene->rotate_y(-STEP_R);
							printf("-y\n");
							break;
						case SDL_SCANCODE_E:
							left_scene->rotate_z(STEP_R);
							right_scene->rotate_z(STEP_R);
							printf("+z\n");
							break;
						case SDL_SCANCODE_Q:
							left_scene->rotate_z(-STEP_R);
							right_scene->rotate_z(-STEP_R);
							printf("-z\n");
							break;
						case SDL_SCANCODE_Z:
							left_scene->rotate_vector(ROTATION, STEP_R);
							right_scene->rotate_vector(ROTATION, STEP_R);
							printf("+v\n");
							break;
						case SDL_SCANCODE_C:
							left_scene->rotate_vector(ROTATION, -STEP_R);
							right_scene->rotate_vector(ROTATION, -STEP_R);
							printf("-v\n");
							break;
						case SDL_SCANCODE_PERIOD:
							left_scene->scale(1 - STEP_S);
							right_scene->scale(1 - STEP_S);
							printf("-s\n");
							break;
						case SDL_SCANCODE_SLASH:
							left_scene->scale(1 + STEP_S);
							right_scene->scale(1 + STEP_S);
							printf("+s\n");
							break;
						case SDL_SCANCODE_1:
							if (task_count > 0)
								task_count--;
							break;
						case SDL_SCANCODE_2:
							if (task_count < MAX_TASK_COUNT)
								task_count++;
							break;
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
			SDL_FillRect(loadedSurface, NULL, BACKGROUND.get_value());

			if (USES_3D) {
				add_3d_objects(left_scene, layer_count, length);
				add_3d_objects(right_scene, layer_count, length);
				left_scene->set_surface(loadedSurface);
				right_scene->set_surface(loadedSurface);
				left_scene->draw();
				right_scene->draw();
			} else {
				for (int i = 0; i < total_count; i++)
					shapes[i]->draw(loadedSurface);
			}

			SDL_UpdateTexture(gTexture, NULL, loadedSurface->pixels, loadedSurface->pitch);
			SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
			SDL_RenderPresent(gRenderer);
		}
		close();
	}
	return 0;
}