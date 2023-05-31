#pragma once

static int SCREEN_WIDTH = 1920;
static int SCREEN_HEIGHT = 1080;

static const int HALF_WIDTH_VIEWPORT = SCREEN_WIDTH / 2;
static const int HALF_HEIGHT_VIEWPORT = SCREEN_HEIGHT / 2;

static const int BASE_SPRITE_WIDTH = 16;
static const int BASE_SPRITE_HEIGHT = 16;
static const int TEST_TILE_OFF_X = 2;
static const float SPRITE_SCALE = 6.0f;
static const int SCALED_SPRITE_WIDTH = BASE_SPRITE_WIDTH * SPRITE_SCALE;
static const int SCALED_SPRITE_HEIGHT = BASE_SPRITE_HEIGHT * SPRITE_SCALE;

const char CHARACTER_DATABASE_EXTRACT_LOCATION[] = "res/database_extracts/characters.csv";

static const int TILES_ROWS = 1080 / SCALED_SPRITE_HEIGHT;
static const int TILES_COLS = 1920 / SCALED_SPRITE_WIDTH;
static const int HERO_SPRITE_WIDTH = 51;
static const int HERO_SPRITE_HEIGHT = 72;
static const float h_scale = 2.4f;
static const int SCALED_HERO_SPRITE_WIDTH = HERO_SPRITE_WIDTH * h_scale;
static const int SCALED_HERO_SPRITE_HEIGHT = HERO_SPRITE_HEIGHT * h_scale;

static const int HEIGHT_OF_SINGLE_LINE = SCREEN_HEIGHT / 20;
static const int X_OFFSET_TO_FRAME_BORDER = SCREEN_WIDTH / 96;

enum EdgeEnum
{
	NONE = 0x00,
	LEFT = 0x01,
	RIGHT = 0x02,
	TOP = 0x04,
	BOTTOM = 0x08,
	TOP_LEFT = 0x05,
	TOP_RIGHT = 0x03,
	BOTTOM_LEFT = 0x09,
	BOTTOM_RIGHT = 0x0A
};