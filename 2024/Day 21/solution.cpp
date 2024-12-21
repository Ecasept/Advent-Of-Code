#include "../lib/utils.h"
#include <algorithm>
#include <limits>
#include <queue>
#include <stdlib.h>
#include <unordered_map>
#include <vector>
const std::unordered_map<char, utils::Point> dirToPos = {
	{'^', {1, 0}}, {'A', {2, 0}}, {'<', {0, 1}}, {'v', {1, 1}}, {'>', {2, 1}}};

const std::unordered_map<char, utils::Point> charToPos = {
	{'A', {2, 3}}, {'0', {1, 3}}, {'1', {0, 2}}, {'2', {1, 2}},
	{'3', {2, 2}}, {'4', {0, 1}}, {'5', {1, 1}}, {'6', {2, 1}},
	{'7', {0, 0}}, {'8', {1, 0}}, {'9', {2, 0}},
};
const std::unordered_map<char, utils::Point> directions = {
	{'^', {0, -1}}, {'v', {0, 1}}, {'<', {-1, 0}}, {'>', {1, 0}}};

bool isValidPermutation(const std::string &permutation, utils::Point startPoint,
						utils::Point invalidPoint) {
	utils::Point currentPoint = startPoint;
	for (char c : permutation) {
		if (c == 'A') {
			continue;
		}
		currentPoint += directions.at(c);
		if (currentPoint == invalidPoint) {
			return false;
		}
	}
	return true;
}

std::vector<std::unordered_map<std::string, llu>> memo;

llu solveDirpad(const std::string input, int depth) {
	// Map char so positions

	if (memo[depth].find(input) != memo[depth].end()) {
		return memo[depth].at(input);
	}

	if (depth == 0) {
		memo[depth][input] = input.size();
		return input.size();
	}

	llu totalLength = 0;

	char prev = 'A';
	for (char c : input) {
		llu minCurrentLength = std::numeric_limits<llu>::max();
		auto startPoint = dirToPos.at(prev);
		auto endPoint = dirToPos.at(c);

		int horCount = endPoint.x - startPoint.x;
		char horChar = horCount > 0 ? '>' : '<';
		int verCount = endPoint.y - startPoint.y;
		char verChar = verCount > 0 ? 'v' : '^';

		std::string str = std::string(std::abs(horCount), horChar) +
						  std::string(std::abs(verCount), verChar);

		str.append("A");

		std::sort(str.begin(), str.end() - 1);

		do {
			if (!isValidPermutation(str, startPoint, utils::Point{0, 0})) {
				continue;
			}

			llu length = solveDirpad(str, depth - 1);
			if (length < minCurrentLength) {
				minCurrentLength = length;
			}

		} while (std::next_permutation(str.begin(), str.end() - 1));
		totalLength += minCurrentLength;
		prev = c;
	}
	memo[depth][input] = totalLength;
	return totalLength;
}

llu solve(const std::string input, int depth) {
	// Map char so positions

	memo.clear();
	memo.resize(depth + 1);

	char prev = 'A';

	llu totalLength = 0;

	for (char c : input) {
		llu minCurrentLength = std::numeric_limits<llu>::max();
		auto startPoint = charToPos.at(prev);
		auto endPoint = charToPos.at(c);

		int horCount = endPoint.x - startPoint.x;
		char horChar = horCount > 0 ? '>' : '<';
		int verCount = endPoint.y - startPoint.y;
		char verChar = verCount > 0 ? 'v' : '^';

		std::string str = std::string(std::abs(horCount), horChar) +
						  std::string(std::abs(verCount), verChar);
		str.append("A");

		std::sort(str.begin(), str.end() - 1);

		do {
			if (!isValidPermutation(str, startPoint, utils::Point{0, 3})) {
				continue;
			}

			llu length = solveDirpad(str, depth);
			if (length < minCurrentLength) {
				minCurrentLength = length;
			}

		} while (std::next_permutation(str.begin(), str.end() - 1));

		totalLength += minCurrentLength;
		prev = c;
	}
	return totalLength;
}

llu part1() {
	auto file = utils::getInput();

	std::string line;

	llu sum = 0;

	while (std::getline(file, line)) {
		llu res = solve(line, 2);
		llu numericPart = std::stoi(line.substr(0, line.size() - 1));
		sum += res * numericPart;
	}
	return sum;
}

llu part2() {
	auto file = utils::getInput();

	std::string line;

	llu sum = 0;

	while (std::getline(file, line)) {
		llu res = solve(line, 25);
		llu numericPart = std::stoi(line.substr(0, line.size() - 1));
		sum += res * numericPart;
	}
	return sum;
}
