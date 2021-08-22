#include "PositionWrapper.h"
#include "Core/EngineData.h"

void PositionWrapper::OnUpdate(float deltaTime) {
	if (transform->Position().X > SCREEN_WIDTH - (texDimensions.X * transform->Scale().X))
		transform->Position().X = 0;
	if (transform->Position().X < 0)
		transform->Position().X = (SCREEN_WIDTH - (texDimensions.X * transform->Scale().X));
	if (transform->Position().Y > SCREEN_HEIGHT - (texDimensions.Y * transform->Scale().Y))
		transform->Position().Y = 0;
	if (transform->Position().Y < 0)
		transform->Position().Y = (SCREEN_HEIGHT - (texDimensions.Y * transform->Scale().Y));
}


void PositionWrapper::SetTexDimensions(SDL_Rect rect) {
	texDimensions.X = rect.w;
	texDimensions.Y = rect.h;
}