#include "Timer.h"

using namespace std::chrono;

Timer::Timer()
{
	last = steady_clock::now();
}

float Timer::mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frame_time = last - old;
	return frame_time.count();
}

float Timer::peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}
