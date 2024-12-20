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

		if (distances[index] >= dist) {
			distances[index] = dist;
		} else {
			continue;
		}

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

llu part1() {
	std::ifstream file = utils::getInput();

	utils::Grid grid;
	grid.loadFromFile(file);

	std::vector<int> distances(grid.size, std::numeric_limits<int>::max());

	Point startPoint = grid.find('S');

	bfs(grid, startPoint, distances);

	// std::map<int, int> savedTimes;

	llu sum = 0;
	for (int cheatStart = 0; cheatStart < grid.size; cheatStart++) {
		if (*grid.at(cheatStart) == '#') {
			// Can't start cheating in wall
			continue;
		}

		std::vector<Point> cheatEnds;

		for (int dir1 = 0; dir1 < 4; dir1++) {
			Point cheat1 = grid.inDir(grid.toPoint(cheatStart), dir1);
			if (!grid.isValidPosition(cheat1)) {
				continue;
			}
			for (int dir2 = 0; dir2 < 4; dir2++) {
				Point cheat2 = grid.inDir(cheat1, dir2);
				if (!grid.isValidPosition(cheat2)) {
					continue;
				}
				if (*grid.at(cheat2) == '#') {
					// Can't end cheating in wall
					continue;
				}

				if (std::find(cheatEnds.begin(), cheatEnds.end(), cheat2) !=
					cheatEnds.end()) {
					continue;
				}
				cheatEnds.push_back(cheat2);
			}
		}

		for (Point cheatEnd : cheatEnds) {
			auto startSeconds = distances[cheatStart];
			auto endSeconds = distances[grid.toIndex(cheatEnd)];
			int timeSaved = startSeconds - endSeconds - 2;
			// savedTimes[timeSaved]++;
			if (timeSaved >= 100) {
				sum++;
			}
		}
	}

	// for (auto [timeSaved, count] : savedTimes) {
	// 	std::cout << timeSaved << ": " << count << std::endl;
	// }

	return sum;
}

llu part2() {
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

		int cheatSearchStart =
			std::max(cheatStartIndex - 20 - grid.width * 20, 0);
		int cheatSearchEnd =
			std::min(cheatStartIndex + 20 + grid.width * 20, grid.size);

		for (int cheatEndIndex = cheatSearchStart;
			 cheatEndIndex < cheatSearchEnd; cheatEndIndex++) {

			Point cheatEnd = grid.toPoint(cheatEndIndex);
			int distance = abs(cheatStart.first - cheatEnd.first) +
						   abs(cheatStart.second - cheatEnd.second);

			if (distance >= 2 and distance <= 20) {
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
