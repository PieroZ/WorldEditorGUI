#include "CFPS.h"

CFPS CFPS::FPSControl;

const float CFPS::FRAME_RATE = 32.0f;


CFPS::CFPS() {
	OldTime = 0;
	LastTime = 0;

	SpeedFactor = 0;

	Frames = 0;
	NumFrames = 0;
}

void CFPS::OnLoop() {
	if (OldTime + 1000 < SDL_GetTicks()) {
		OldTime = SDL_GetTicks();

		NumFrames = Frames;

		Frames = 0;
	}

	SpeedFactor = ((SDL_GetTicks() - LastTime) / 1000.0f) * FRAME_RATE;

	LastTime = SDL_GetTicks();

	Frames++;
}

int CFPS::GetFPS() {
	return NumFrames;
}

float CFPS::GetSpeedFactor() {
	return SpeedFactor;
}