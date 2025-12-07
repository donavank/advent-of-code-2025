#include "AdventOfCode.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <numeric>
#include <iostream>
#include <math.h>

int secretEntrance1() {
	std::ifstream rotationsFile{ "rotations.txt" };

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
	}

	return timesAtZero;
}

long long validateIds1() {
	//std::cout <<	 "Validate Ids: " << validationType << '\n';
	// Read input
	std::ifstream rotationsFile{ "product-id-ranges.txt" };
	std::string input;
	rotationsFile >> input;

	// Split the input 
	std::regex commaDelimiter{ ',' };
	std::sregex_token_iterator inputIterator{ input.begin(), input.end(), commaDelimiter, -1 };
	std::vector<std::string> ranges{ inputIterator, {} };

	long long sum = 0;

	for (std::string range : ranges) {
		auto splitter{ range.find_first_of('-') };
		// Start of range is up to '-'
		long long start{ std::atoll(range.substr(0, splitter).c_str()) };
		// End of range starts after '-'
		long long end{ std::atoll(range.substr(splitter + 1, range.length()).c_str()) };

		long long index{ start };
		while (index <= end) {

			bool isValidId{ isValidId1(index) };

			if (!isValidId) {
				sum += index;
			}
			++index;
		}
	}
	return sum;
}

static bool isValidId1(long long id) {
	std::string idString{ std::to_string(id) };

	// Odd length cannot be a set of digits repeated
	if (idString.length() % 2 == 1) {
		return true;
	}

	auto midpoint{ idString.length() / 2 };

	std::string firstHalf{ idString.substr(0, midpoint) };
	std::string secondHalf{ idString.substr(midpoint, idString.length()) };

	return !(firstHalf == secondHalf);
}

long long validateIds2() {
	//std::cout <<	 "Validate Ids: " << validationType << '\n';
	// Read input
	std::ifstream rotationsFile{ "product-id-ranges.txt" };
	std::string input;
	rotationsFile >> input;

	// Split the input 
	std::regex commaDelimiter{ ',' };
	std::sregex_token_iterator inputIterator{ input.begin(), input.end(), commaDelimiter, -1 };
	std::vector<std::string> ranges{ inputIterator, {} };

	std::vector<long long> results{};
	for (std::string range : ranges) {
		//std::cout << range << '\n';

		auto splitter{ range.find_first_of('-') };
		// Start of range is up to '-'
		std::string start{ range.substr(0, splitter).c_str() };
		// End of range starts after '-'
		std::string end{ range.substr(splitter + 1, range.length()).c_str() };

		std::vector<long long> invalidIds{ getInvalidIdsInRange(start, end) };
		results.push_back(std::reduce(invalidIds.begin(), invalidIds.end()));
		//std::cout << "Result: " << results.back() << '\n';
	}
	long long sum = 0;
	for (size_t i = 0; i < results.size(); ++i) {
		sum += results[i];
		//std::cout << "Sum: " << sum << '\n';
	}
	return std::reduce(results.begin(), results.end());
}

static std::vector<long long> getInvalidIdsInRange(std::string start, std::string end) {
	long long startValue{ atoll(start.c_str()) };
	long long endValue{ atoll(end.c_str()) };
	//std::cout << "Start Value: " << startValue << '\n';
	//std::cout << "End Value: " << endValue << '\n';
	std::vector<long long> invalidIds{};

	for (long long id = startValue; id <= endValue;++id) {
		int validIdIndex{ isInvalidId2(id) };
		if (validIdIndex == -1) {
			//std::cout << "Invalid ID: " << id << '\n';
			invalidIds.push_back(id);
			//++id;
		}
		else {
			//std::cout << "Valid ID: " << id << " due to char at " << validIdIndex << '\n';
			//std::string idString{ std::to_string(id) };
			// Substract 1 because because a validIndex of len - 1 corresponds to power 0 (1's place)
			// and length with be the total length of the string while validIndex is an index of the 1's place.
			//int powerOfTen = static_cast<int>(idString.length()) - validIdIndex - 1;
			//id = id + static_cast<long long>(std::pow(10, powerOfTen));
			//std::cout << "Skipping to id: " << id << '\n';
		}
	}

	return invalidIds;
}

/// <summary>
/// Returns the index of the last character that caused the id to be valid so the caller knows how much an id would have to change
/// in order to possibly cause an invalid id
/// </summary>
/// <param name="id"></param>
/// <returns></returns>
static int isInvalidId2(long long id) {
	if (id < 10) {
		return 0;
	}
	std::string idString{ std::to_string(id) };
	size_t tokenLength{ idString.length() / 2 };
	int badIndex = -1;

	while (tokenLength >= 1) {
		// If the token length does not divide the string evenly,
		// don't even check it.
		if (idString.length() % tokenLength != 0) {
			--tokenLength;
			continue;
		}

		// Build out repeat token
		std::stringstream tokenStream;
		tokenStream << idString.substr(0, tokenLength);

		// Test repeat token
		std::string token;
		tokenStream >> token;

		// If the idString isn't a multiple of the token length, token doesn't prove invalidity
		if (idString.length() % token.length() != 0) {
			--tokenLength;
			continue;
		}

		int tokenCount{ 0 };
		bool tokenProvesInvalid{ true };
		while ((tokenCount + 1) * token.length() <= idString.length()) {
			auto tokenStart{ tokenCount * token.length() };
			std::string tokenSubstring = idString.substr(tokenStart, tokenStart + token.length());
			for (size_t i = 0; i < token.length(); ++i) {
				if (tokenSubstring[i] != token[i]) {
					int newBadIndex = static_cast<int>(tokenStart + i);
					badIndex = badIndex < newBadIndex ? newBadIndex : badIndex;
					tokenProvesInvalid = false;
				}
			}
			if (!tokenProvesInvalid) {
				//std::cout << "Bad token: " << token << '\n';
				break;
			}
			++tokenCount;
		}

		if (tokenProvesInvalid) {
			//std::cout << "Proved invalid by: " << token << '\n';
			return -1;
		}
		--tokenLength;
	}
	return badIndex;
}

int maximizeJoltages1() {
	std::ifstream batteriesFile{ "joltages-test.txt" };

	std::string battery;

	while (batteriesFile >> battery) {
	}
	return 0;
}
