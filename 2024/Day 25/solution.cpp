#include "../lib/utils.h"

typedef std::vector<int> Key;
typedef std::vector<int> Lock;

std::vector<int> convertToVec(std::string schematic) {
	const int WIDTH = 5;
	const int HEIGHT = 7;
	std::vector<int> vec;
	for (int x = 0; x < WIDTH; x++) {
		int filledCount = -1;
		for (int y = 0; y < HEIGHT; y++) {
			int index = utils::toIndex(x, y, WIDTH);
			if (schematic[index] == '#') {
				filledCount++;
			}
		}
		vec.push_back(filledCount);
	}
	return vec;
}

bool overlap(Key key, Lock lock) {
	const int MAX = 5;
	for (size_t i = 0; i < key.size(); i++) {
		if (key[i] + lock[i] > MAX) {
			return false;
		}
	}
	return true;
}

llu part1() {
	auto file = utils::getInput();

	std::string schematic;
	std::string line;
	bool isKey = false;

	std::vector<Key> keys;
	std::vector<Lock> locks;

	while (std::getline(file, line)) {
		if (schematic.empty()) {
			isKey = line.find("#") ==
					std::string::npos; // if top row is empty, it's a key
		}

		if (line.empty()) {
			if (isKey) {
				keys.push_back(convertToVec(schematic));
			} else {
				locks.push_back(convertToVec(schematic));
			}
			schematic.clear();
		} else {
			schematic += line;
		}
	}
	// Add the last schematic
	if (isKey) {
		keys.push_back(convertToVec(schematic));
	} else {
		locks.push_back(convertToVec(schematic));
	}

	llu pairs = 0;
	for (const auto &key : keys) {
		for (const auto &lock : locks) {
			if (overlap(key, lock)) {
				pairs++;
			}
		}
	}

	return pairs;
}

std::string part2() { return "Deliver The Chronicle!"; }
