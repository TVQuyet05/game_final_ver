
#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <windows.h>
#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#undef main  

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

static Mix_Chunk* g_sound_bullet;
static Mix_Music* g_sound_track;

const int FRAME_PER_SECOND = 60;

const int SCREEN_WIDTH = 1480;
const int SCREEN_HEIGHT = 735;
const int SCREEN_BPP = 32;

const int MIDDLE_SCREEN_WIDTH = 735;
const int MIDDLE_SCREEN_HEIGHT = 360;


#define COLOR_KEY_R 0
#define COLOR_KEY_G 0xff
#define COLOR_KEY_B 0xff

const int RENDER_DRAW_COLOR = 0xff;

#define BLANK_TILE 0
#define TILE_SIZE 64
#define MAX_MAP_X 400
#define MAX_MAP_Y 10



#endif