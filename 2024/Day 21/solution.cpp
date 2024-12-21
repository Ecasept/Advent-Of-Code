#include "../lib/utils.h"
#include <limits>
#include <queue>
#include <stdlib.h>
#include <unordered_map>
#include <vector>

typedef std::vector<int> Path;
typedef std::vector<Path> ShortestPaths;
typedef std::vector<ShortestPaths> StartToEndShortestPaths;

void dfs(utils::Point currentPoint, const utils::Point &endPoint, int distance,
		 int minDistance, Path &currentPath, ShortestPaths &shortestPaths,
		 const int WIDTH, const int HEIGHT, const utils::Point &invalidPoint) {
	if (distance > minDistance) {
		return;
	}

	if (currentPoint == endPoint) {
		if (distance == minDistance) {
			currentPath.push_back(-1);
			shortestPaths.push_back(currentPath);
		}
		return;
	}

	const std::vector<utils::Point> directions = {
		{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

	for (int dir = 0; dir < 4; dir++) {
		utils::Point nextPoint = currentPoint + directions[dir];

		if (!utils::isValidPosition(nextPoint.x, nextPoint.y, WIDTH, HEIGHT)) {
			continue;
		}
		if (nextPoint == invalidPoint) {
			continue;
		}

		currentPath.push_back(dir);
		dfs(nextPoint, endPoint, distance + 1, minDistance, currentPath,
			shortestPaths, WIDTH, HEIGHT, invalidPoint);
		currentPath.pop_back();
	}
}

llu dirPad(std::vector<std::vector<std::vector<int>>> &paths) {
	// Map char so positions
	const std::unordered_map<int, utils::Point> dirToPos = {
		{0, {1, 0}}, {-1, {2, 0}}, {3, {0, 1}}, {2, {1, 1}}, {1, {2, 1}}};
}

llu solve(const std::string input) {
	// Map char so positions
	const std::unordered_map<char, utils::Point> charToPos = {
		{'A', {2, 3}}, {'0', {1, 3}}, {'1', {0, 2}}, {'2', {1, 2}},
		{'3', {2, 2}}, {'4', {0, 1}}, {'5', {1, 1}}, {'6', {2, 1}},
		{'7', {0, 0}}, {'8', {1, 0}}, {'9', {2, 0}},
	};

	std::vector<std::vector<std::vector<int>>> paths;

	char prev = 'A';
	for (char c : input) {
		int distance = charToPos.at(prev).manhattanTo(charToPos.at(c));

		std::vector<std::vector<int>> shortestPaths;
		std::vector<int> currentPath;
		dfs(charToPos.at(prev), charToPos.at(c), 0, distance, currentPath,
			shortestPaths, 3, 4, utils::Point{0, 3});

		for (auto path : shortestPaths) {
			std::cout << "from " << prev << " to " << c << ": ";
			for (int dir : path) {
				std::cout << dir << " ";
			}
			std::cout << std::endl;
		}

		paths.push_back(shortestPaths);

		prev = c;
	}
	std::cout << std::endl;
	return 0;
}

llu part1() {
	auto file = utils::getExample();

	std::string line;

	llu sum = 0;

	while (std::getline(file, line)) {
		sum += solve(line);
	}
	return 0;
}

llu part2() { return 0; }

// A>^^A  AvA<^AA>A
// A^>^A  A<Av>A<^A>A
// A^^>A  A<AAv>A^A
