#pragma once

const int SCREEN_WIDTH {1200};
const int SCREEN_HEIGHT {900};

class EngineHelper {
public:
	static inline float GetScreenWidthMidPoint() { return SCREEN_WIDTH / 2; }
	static inline float GetScreenHeightMidPoint() { return SCREEN_HEIGHT / 2; }
	static inline float GetScreenTitlePosition() { return SCREEN_HEIGHT / 8; }
};