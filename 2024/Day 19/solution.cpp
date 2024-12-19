#include "../lib/utils.h"
#include <algorithm>
#include <map>
#include <vector>

typedef std::string Pattern;
typedef std::string Towel;

std::map<Pattern, bool> memo1;

bool isPossible(std::vector<Towel> towels, Pattern pattern) {
	if (memo1.find(pattern) != memo1.end()) {
		return memo1[pattern];
	}

	if (pattern.empty()) {
		memo1[pattern] = true;
		return true;
	}
	for (const auto &towel : towels) {
		if (pattern.find(towel) == 0) {
			Pattern newPattern = pattern.substr(towel.size());
			if (isPossible(towels, newPattern)) {
				memo1[pattern] = true;
				return true;
			}
		}
	}
	memo1[pattern] = false;
	return false;
}

std::map<Pattern, llu> memo2;

llu countPossible(std::vector<Towel> towels, Pattern pattern) {
	if (memo2.find(pattern) != memo2.end()) {
		return memo2[pattern];
	}

	if (pattern.empty()) {
		memo2[pattern] = 1;
		return 1;
	}
	llu count = 0;
	for (const auto &towel : towels) {
		if (pattern.find(towel) == 0) {
			Pattern newPattern = pattern.substr(towel.size());
			count += countPossible(towels, newPattern);
		}
	}
	memo2[pattern] = count;
	return count;
}

llu part1() {
	auto file = utils::getInput();
	std::string line;

	std::getline(file, line);

	std::vector<Towel> towels = utils::split(line, ", ");

	std::getline(file, line); // skip empty line

	llu sum = 0;
	while (std::getline(file, line)) {
		sum += isPossible(towels, line);
	}

	return sum;
}

llu part2() {
	auto file = utils::getInput();
	std::string line;

	std::getline(file, line);

	std::vector<Towel> towels = utils::split(line, ", ");

	std::getline(file, line); // skip empty line

	llu sum = 0;
	while (std::getline(file, line)) {
		sum += countPossible(towels, line);
	}

	return sum;
}
