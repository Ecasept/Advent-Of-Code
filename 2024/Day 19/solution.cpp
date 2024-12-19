#include "../lib/utils.h"
#include <algorithm>
#include <string.h>
#include <string>
#include <unordered_map>
#include <vector>

typedef std::string Pattern;
typedef std::string Towel;

std::unordered_map<const char *, bool> memo1;

bool isPossible(const char *towels[], llu towelCount, const char *pattern) {
	if (memo1.find(pattern) != memo1.end()) {
		return memo1[pattern];
	}

	if (*pattern == '\0') {
		memo1[pattern] = true;
		return true;
	}
	for (llu i = 0; i < towelCount; i++) {
		const char *towel = towels[i];
		int towelLen = strlen(towel);
		bool startsWithTowel = strncmp(pattern, towel, towelLen) == 0;
		if (startsWithTowel) {
			if (isPossible(towels, towelCount, pattern + towelLen)) {
				memo1[pattern] = true;
				return true;
			}
		}
	}
	memo1[pattern] = false;
	return false;
}

std::unordered_map<const char *, llu> memo2;

llu countPossible(const char *towels[], llu towelCount, const char *pattern) {
	if (memo2.find(pattern) != memo2.end()) {
		return memo2[pattern];
	}

	if (*pattern == '\0') {
		memo2[pattern] = 1;
		return 1;
	}
	llu count = 0;
	for (llu i = 0; i < towelCount; i++) {
		const char *towel = towels[i];
		int towelLen = strlen(towel);
		bool startsWithTowel = strncmp(pattern, towel, towelLen) == 0;
		if (startsWithTowel) {
			count += countPossible(towels, towelCount, pattern + towelLen);
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

	int towelCount = towels.size();
	const char **towelsArr = new const char *[towelCount];

	for (int i = 0; i < towelCount; i++) {
		towelsArr[i] = towels[i].c_str();
	}

	std::getline(file, line); // skip empty line

	llu sum = 0;
	while (std::getline(file, line)) {
		memo1.clear();
		sum += isPossible(towelsArr, towelCount, line.c_str());
	}

	return sum;
}

llu part2() {
	auto file = utils::getInput();
	std::string line;

	std::getline(file, line);

	std::vector<Towel> towels = utils::split(line, ", ");

	int towelCount = towels.size();
	const char **towelsArr = new const char *[towelCount];

	for (int i = 0; i < towelCount; i++) {
		towelsArr[i] = towels[i].c_str();
	}

	std::getline(file, line); // skip empty line

	llu sum = 0;
	while (std::getline(file, line)) {
		memo2.clear();
		sum += countPossible(towelsArr, towelCount, line.c_str());
	}

	delete[] towelsArr;

	return sum;
}
