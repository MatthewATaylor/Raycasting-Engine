#include "FPS_Clock.h"

//Constructor
FPS_Clock::FPS_Clock() {
	previous = std::chrono::system_clock::now();
	current = std::chrono::system_clock::now();
}

//Public
void FPS_Clock::setCurrent() {
	current = std::chrono::system_clock::now();
}
void FPS_Clock::setPrevious() {
	previous = std::chrono::system_clock::now();
}
float FPS_Clock::getFPS() {
	std::chrono::duration<float> duration = current - previous;
	float timeChange = duration.count();
	return 1.0f / timeChange;
}