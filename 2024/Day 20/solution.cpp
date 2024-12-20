#include "../lib/utils.h"
#include <algorithm>
#include <limits>
#include <map>
#include <queue>

typedef utils::Point Point;

std::vector<int> bfs(utils::Grid &grid, Point startPoint,
					 std::vector<int> &distances) {
	std::queue<std::pair<int, Point>> queue; // Queue of (distance, index)

	queue.push({0, startPoint});

	std::vector<bool> visited(grid.size, false);

	while (!queue.empty()) {
		auto [dist, point] = queue.front();
		queue.pop();

		auto index = grid.toIndex(point);

		if (visited[index]) {
			continue;
		}
		visited[index] = true;

		for (int i = 0; i < 4; i++) {
			auto nextPoint = grid.inDir(point, i);
			if (!grid.isValidPosition(nextPoint)) {
				continue;
			}

			if (*grid.at(nextPoint) == '#') {
				continue;
			}

			queue.push({dist + 1, nextPoint});
		}
	}

	return distances;
}

llu solve(int minDistance, int maxDistance) {
	std::ifstream file = utils::getInput();

	utils::Grid grid;
	grid.loadFromFile(file);

	std::vector<int> distances(grid.size, std::numeric_limits<int>::max());

	Point startPoint = grid.find('S');

	bfs(grid, startPoint, distances);

	llu sum = 0;
	for (int cheatStartIndex = 0; cheatStartIndex < grid.size;
		 cheatStartIndex++) {
		if (*grid.at(cheatStartIndex) == '#') {
			// Can't start cheating in wall
			continue;
		}
		Point cheatStart = grid.toPoint(cheatStartIndex);

		std::vector<bool> checked(grid.size, false);

		int cheatSearchStart = std::max(
			cheatStartIndex - maxDistance - grid.width * maxDistance, 0);
		int cheatSearchEnd =
			std::min(cheatStartIndex + maxDistance + grid.width * maxDistance,
					 grid.size);

		for (int cheatEndIndex = cheatSearchStart;
			 cheatEndIndex < cheatSearchEnd; cheatEndIndex++) {

			Point cheatEnd = grid.toPoint(cheatEndIndex);
			int distance = abs(cheatStart.first - cheatEnd.first) +
						   abs(cheatStart.second - cheatEnd.second);

			if (distance >= minDistance and distance <= maxDistance) {
				if (*grid.at(cheatEndIndex) == '#') {
					continue;
				}

				if (checked[cheatEndIndex]) {
					continue;
				}
				checked[cheatEndIndex] = true;
				auto startSeconds = distances[cheatStartIndex];
				auto endSeconds = distances[cheatEndIndex];
				int timeSaved = startSeconds - endSeconds - distance;

				if (timeSaved >= 100) {
					sum++;
				}
			}
		}
	}

	return sum;
}

llu part1() { return solve(2, 2); }
llu part2() { return solve(2, 20); }
