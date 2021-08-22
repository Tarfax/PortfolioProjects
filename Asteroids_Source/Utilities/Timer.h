#pragma once

#include <chrono>
#include <iostream>
#include <string>


class Timer {

public:
	Timer() {
		start = std::chrono::high_resolution_clock::now();
	}
	~Timer() {
		end = std::chrono::high_resolution_clock::now();
		difference = end - start;
		std::cout << "Timer: " << std::to_string(difference.count()) << std::endl;
	}

private:
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;
	std::chrono::duration<float> difference;
};

