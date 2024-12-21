#include "../lib/utils.h"
#include <algorithm>
#include <limits>
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

bool isValidPermutation(const std::string &permutation,
						const utils::Point &startPoint,
						const utils::Point &invalidPoint) {
	utils::Point currentPoint = startPoint;
	for (const char &c : permutation) {
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

llu solveInput(const std::string &input, int depth, bool isDirPad) {
	if (isDirPad && memo[depth].find(input) != memo[depth].end()) {
		return memo[depth].at(input);
	}

	if (depth == 0) {
		if (isDirPad) {
			memo[depth][input] = input.size();
		}
		return input.size();
	}

	llu totalLength = 0;

	const auto map = isDirPad ? dirToPos : charToPos;
	const auto invalidPoint =
		isDirPad ? utils::Point{0, 0} : utils::Point{0, 3};

	char prev = 'A';
	for (const char &c : input) {
		llu minCurrentLength = std::numeric_limits<llu>::max();
		auto startPoint = map.at(prev);
		auto endPoint = map.at(c);

		int horCount = endPoint.x - startPoint.x;
		char horChar = horCount > 0 ? '>' : '<';
		int verCount = endPoint.y - startPoint.y;
		char verChar = verCount > 0 ? 'v' : '^';

		std::string str = std::string(std::abs(horCount), horChar) +
						  std::string(std::abs(verCount), verChar) + 'A';

		do {
			if (!isValidPermutation(str, startPoint, invalidPoint)) {
				continue;
			}

			llu length = solveInput(str, depth - 1, true);
			if (length < minCurrentLength) {
				minCurrentLength = length;
			}

		} while (std::next_permutation(str.begin(), str.end() - 1));
		totalLength += minCurrentLength;
		prev = c;
	}
	if (isDirPad) {
		memo[depth][input] = totalLength;
	}
	return totalLength;
}

llu solve(const int dirPadCount) {

	auto file = utils::getInput();

	memo.resize(dirPadCount + 1);

	std::string line;

	llu sum = 0;

	while (std::getline(file, line)) {
		llu res = solveInput(line, dirPadCount + 1, false);
		llu numericPart = std::stoi(line.substr(0, line.size() - 1));
		sum += res * numericPart;
	}
	return sum;
}

llu part1() { return solve(2); }

llu part2() {
	memo.clear();
	return solve(25);
}
