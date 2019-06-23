#pragma once

#include <chrono>

class FPS_Clock {
private:
	std::chrono::time_point<std::chrono::system_clock> previous, current;

public:
	FPS_Clock();
	void setPrevious();
	void setCurrent();
	float getFPS();
};