#include "AdventOfCode.h"
#include <iostream>
#include <fstream>
#include <string>

int secretEntrance1() {
	std::ifstream rotationsFile { "rotations.txt" };

	int position{ 50 };
	int timesAtZero{ 0 };

	std::string rotation;

	while (rotationsFile >> rotation) {
		int shiftAmount{ std::atoi(rotation.substr(1).c_str()) };

		if (rotation.at(0) == 'L') {
			position -= shiftAmount;
		}
		else {
			position += shiftAmount;
		}

		position %= 100;

		if (position == 0) {
			++timesAtZero;
		}
	}

	return timesAtZero;
}

int secretEntrance2() {
	std::ifstream rotationsFile{ "rotations.txt" };

	int position{ 50 };
	int timesAtZero{ 0 };

	std::string rotation;

	while (rotationsFile >> rotation) {
		bool didStartAtZero{ position == 0 };
		int shiftAmount{ std::atoi(rotation.substr(1).c_str()) };

		if (rotation.at(0) == 'L') {
			position -= shiftAmount;
		}
		else {
			position += shiftAmount;
		}

		int hundreds{ position / 100 };

		
		// Every time we go over a hundred in either the negative or positive direction, we pass zero
		timesAtZero += std::abs(hundreds);

		// Readjust to dial 
		if (position <= 0) {
			// If ending at a negative position, we crossed 0 to get there UNLESS we started at zero.
			// If ending on zero, we also are on zero UNLESS we didn't actually shift
			if (!didStartAtZero || (position == 0 && shiftAmount != 0)) {
				++timesAtZero;
			}

			// position is negative, so we find it's remainder in regards to 100 and shift it right
			position = 100 + (position % 100);
		} 
		// Always do this because the logic in the negative case will give position = 100 when position % 100 is 0 (i.e. at -100)
		position %= 100;

		// debug output
		//std::cout << "Rotation: " << rotation << "\nPosition: " << position << "\nTimes At Zero: " << timesAtZero << "\n";
	}

	return timesAtZero;
}
