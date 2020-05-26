#pragma once


static const int SPRITE_WIDTH = 16;
static const int SPRITE_HEIGHT = 16;
static const int TEST_TILE_OFF_X = 2;
static const float scale = 6.0f;
static const int SCALED_SPRITE_WIDTH = SPRITE_WIDTH * scale;
static const int SCALED_SPRITE_HEIGHT = SPRITE_HEIGHT * scale;
static const int TILES_ROWS = 1080 / SCALED_SPRITE_HEIGHT;
static const int TILES_COLS = 1920 / SCALED_SPRITE_WIDTH;
static const int HERO_SPRITE_WIDTH = 51;
static const int HERO_SPRITE_HEIGHT = 72;
static const float h_scale = 2.4f;
static const int SCALED_HERO_SPRITE_WIDTH = HERO_SPRITE_WIDTH * h_scale;
static const int SCALED_HERO_SPRITE_HEIGHT = HERO_SPRITE_HEIGHT * h_scale;