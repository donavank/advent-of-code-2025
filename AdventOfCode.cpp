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
	long long startValue{ std::atoll(start.c_str()) };
	long long endValue{ std::atoll(end.c_str()) };
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
	std::ifstream batteriesFile{ "batteries.txt" };

	std::string battery;

	int joltageSum{ 0 };

	while (batteriesFile >> battery) {
		int firstDigit{ -1 };
		int secondDigit{ -1 };

		for (char c : battery.substr(0, battery.length()-1)) {
			int digit = std::atoi(&c);
			if (digit > firstDigit) {
				secondDigit = -1;
				firstDigit = digit;
			}
			else if (digit > secondDigit) {
				secondDigit = digit;
			}
		}

		int lastDigit{ std::atoi(&battery[battery.length() - 1]) };
		if (secondDigit < lastDigit) {
			secondDigit = lastDigit;
		}

		joltageSum += (firstDigit * 10) + secondDigit;
	}
	return joltageSum;
}

void printDigits(int digits[]) {
	std::stringstream ss;
	for (int i = 0; i < sizeof(digits); ++i) {
		ss << digits[i];
	}
	std::string s;
	ss >> s;
	std::cout << "Digits: " << s << "\n";
}

long long maximizeJoltages2() {
	std::ifstream batteriesFile{ "batteries.txt" };

	std::string battery;

	long long joltageSum{ 0 };
	const int digitsLength{ 12 };
	while (batteriesFile >> battery) {
		int digits[digitsLength];

		for (int i = 0; i < digitsLength; ++i) {
			digits[i] = -1;
		}

		for (size_t i = 0; i < battery.length(); ++i) {
			char c = battery[i];
			int digit = std::atoi(&c);
			bool wasDigitSet = false;
			// Start wherever there is enough leftover in the batteries to fill out the digits (if need-be)
			size_t digitsStart = battery.length() - i;
			digitsStart = digitsStart < digitsLength ? digitsLength - digitsStart : 0;

			//std::cout << "Char: " << c << "\n";
			//printDigits(digits);
			//std::cout << "Digits Start: " << digitsStart << "\n";
			for (size_t j = digitsStart; j < digitsLength; ++j) {
				if (wasDigitSet) {
					digits[j] = -1;
				}
				else if (digit > digits[j]) {
					digits[j] = digit;
					wasDigitSet = true;
				}
			}
		}

		std::stringstream sumStream;
		for (int i = 0; i < digitsLength; ++i) {
			sumStream << digits[i];
		}

		std::string sumString;
		sumStream >> sumString;
		//std::cout << sumString << "\n";
		joltageSum += std::atoll(sumString.c_str());
	}
	return joltageSum;
}

int findAccessiblePaperRolls1() {
	std::ifstream paperRollsFile{ "paper-rolls.txt" };

	std::string row;
	std::vector<std::vector<char>> paperGrid;

	while (paperRollsFile >> row) {
		std::vector<char> gridRow;
		for (char c : row) {
			gridRow.push_back(c);
		}
		paperGrid.push_back(gridRow);
	}

	int accessibleRolls{ 0 };
	std::vector<std::string> accessibleLocations;
	for (size_t i = 0; i < paperGrid.size(); ++i) {
		for (size_t j = 0; j < paperGrid[i].size(); ++j) {
			if (paperGrid[i][j] == '@' && countNeighbors(paperGrid, i, j) < 4) {
				++accessibleRolls;
			}
		}
	}
	return accessibleRolls;
}

int findAccessiblePaperRolls2() {
	std::ifstream paperRollsFile{ "paper-rolls.txt" };

	std::string row;
	std::vector<std::vector<char>> paperGrid;

	while (paperRollsFile >> row) {
		std::vector<char> gridRow;
		for (char c : row) {
			gridRow.push_back(c);
		}
		paperGrid.push_back(gridRow);
	}

	int accessibleRolls{ 0 };
	int rollsLeft{ 0 };
	bool removedOne = false;
	// This could probably be improved by backtracking the moment we remove a roll. 
	// Or checking up, left, up-left, up-right neighbors every time one is removed
	// I have not done the math on this though.
	do {
		removedOne = false;
		for (size_t i = 0; i < paperGrid.size(); ++i) {
			for (size_t j = 0; j < paperGrid[i].size(); ++j) {
				if (paperGrid[i][j] == '@') {
					++rollsLeft;
					if (countNeighbors(paperGrid, i, j) < 4) {
						++accessibleRolls;
						--rollsLeft;
						removedOne = true;
						paperGrid[i][j] = '.';
					}
				}
			}
		}
	} while (rollsLeft > 0 && removedOne);

	return accessibleRolls;
}

static bool isInBounds(size_t row, size_t column, size_t rowCount, size_t columnCount) {
	return row >= 0 && column >= 0 && row < rowCount && column < columnCount;
}

int countNeighbors(std::vector<std::vector<char>> grid, size_t row, size_t column) {
	size_t rowCount = grid.size();
	size_t columnCount = grid.front().size();

	int neighbors{ 0 };

	// Check adjacent positions
	for (int i = -1; i <= 1; ++i) {
		size_t neighborRow = row + i;
		for (int j = -1; j <= 1; ++j) {
			// Don't count target position
			if (j == 0 && i == 0) {
				continue;
			}

			size_t neighborColumn = column + j;
			if (isInBounds(neighborRow, neighborColumn, rowCount, columnCount) && grid[neighborRow][neighborColumn] == '@') {
				++neighbors;
			}
		}
	}

	return neighbors;
}

int findFreshIngredients1() {
	std::ifstream ingredientsFile{ "ingredients.txt" };

	std::string range;
	std::vector<long long> rangeStarts;
	std::vector<long long> rangeEnds;

	while (std::getline(ingredientsFile, range) && !range.empty()) {
		auto splitter{ range.find_first_of('-') };
		// Start of range is up to '-'
		long long start{ std::atoll(range.substr(0, splitter).c_str()) };
		// End of range starts after '-'
		long long end{ std::atoll(range.substr(splitter + 1, range.length()).c_str()) };

		rangeStarts.push_back(start);
		rangeEnds.push_back(end);
	}

	std::string idString;
	int freshIngredientsCount{ 0 };

	while (ingredientsFile >> idString) {
		long long id{ std::atoll(idString.c_str()) };
		bool isFresh{ false };
		for (size_t i = 0; i < rangeStarts.size(); ++i) {
			if (id >= rangeStarts[i] && id <= rangeEnds[i]) {
				isFresh = true;
				break;
			}
		}

		if (isFresh) {
			++freshIngredientsCount;
		}
	}
	return freshIngredientsCount;
}

struct RangePoint {
	long long start;
	long long end;
};

long long findFreshIngredients2() {
	std::ifstream ingredientsFile{ "ingredients.txt" };

	std::string range;
	std::vector<RangePoint> rangePoints;

	while (std::getline(ingredientsFile, range) && !range.empty()) {
		auto splitter{ range.find_first_of('-') };
		// Start of range is up to '-'
		long long start{ std::atoll(range.substr(0, splitter).c_str()) };
		// End of range starts after '-'
		long long end{ std::atoll(range.substr(splitter + 1, range.length()).c_str()) };


		bool isNewRange{ true };
		// Check whether this range falls in any other range
		// Adjust this point's start and end to not overlap with existing ranges.
		for (size_t i = 0; i < rangePoints.size(); ++i) {
			RangePoint point = rangePoints[i];

			// Disregard points like this because we have determined they are covered by another range.
			if (point.start > point.end) {
				continue;
			}

			// this range completely covers one that was already read
			// Because this range no longer needs to be considered, we assign it values
			// that let us know to ignore it when summing by making start > end. 
			// I believe that 
			if (start <= point.start && end >= point.end) { 
				// Have to access vector directly because apparently the struct is being copied otherwise.
				rangePoints[i].start = point.end + 1;
				rangePoints[i].end = point.start;
			}
			// this range completely falls within an existing range
			else if (start >= point.start && end <= point.end) {
				isNewRange = false;
			}
			// bottom part of new range falls within existing range
			else if (start <= point.end && end > point.end) {
				start = point.end + 1;
			} 
			// top part of new range falls within existing range
			else if (end >= point.start && start < point.start) {
				end = point.start - 1;
			}
		}

		if (isNewRange && start <= end) {
			RangePoint newRange{ start, end };
			rangePoints.push_back(newRange);
		}
	}

	long long totalFreshIngredients{ 0 };
	for (RangePoint rangePoint : rangePoints) {
		if (!(rangePoint.start > rangePoint.end)) {
			totalFreshIngredients += rangePoint.end - rangePoint.start + 1; // range is inclusive, so add 1
		}
	}
	return totalFreshIngredients;
}