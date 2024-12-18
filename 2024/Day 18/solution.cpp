#include "../lib/utils.h"
#include <limits.h>
#include <queue>
#include <vector>

std::tuple<int, std::vector<int>> dijkstra(int WIDTH, int HEIGHT,
										   std::vector<bool> &isByte,
										   int startX, int startY, int endX,
										   int endY) {
	using Node = std::pair<int, int>; // Pair of (distance, index)
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> q;
	std::vector<int> distances(WIDTH * HEIGHT, INT_MAX);
	std::vector<bool> visited(WIDTH * HEIGHT, false);
	std::vector<int> prev(WIDTH * HEIGHT, -1);

	int startIndex = startY * WIDTH + startX;
	distances[startIndex] = 0;

	q.push({0, startIndex});

	std::vector<std::pair<int, int>> directions = {
		{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

	while (!q.empty()) {
		int currentDist = q.top().first;
		int currentIndex = q.top().second;
		q.pop();

		if (visited[currentIndex]) {
			continue;
		}
		visited[currentIndex] = true;

		int x = currentIndex % WIDTH;
		int y = currentIndex / WIDTH;

		if (x == endX && y == endY) {
			return std::make_tuple(distances[currentIndex], prev);
		}

		for (auto dir : directions) {
			int newX = x + dir.first;
			int newY = y + dir.second;
			int newIndex = newY * WIDTH + newX;
			if (newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT) {
				continue;
			}
			// if (visited[newIndex]) {
			// 	std::cout << "Visited" << std::endl;
			// 	continue;
			// }
			if (isByte[newIndex]) {
				continue;
			}
			int newDist = currentDist + 1;
			if (newDist < distances[newIndex]) {
				distances[newIndex] = newDist;
				q.push({newDist, newIndex});
				prev[newIndex] = currentIndex;
			}
		}
	}
	// no path found
	return std::make_tuple(INT_MAX, prev);
}

void printMap(int WIDTH, int HEIGHT, std::vector<bool> &isByte) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			std::cout << (isByte[y * WIDTH + x] ? "#" : ".");
		}
		std::cout << std::endl;
	}
}

llu part1() {
	auto file = utils::getInput();

	const int WIDTH = 71;
	const int HEIGHT = 71;
	const int STEP_SIM = 1024;

	std::vector<bool> isByte(WIDTH * HEIGHT, false);
	std::string line;

	int step = 0;
	while (std::getline(file, line) && step < STEP_SIM) {
		int x = 0, y = 0;
		sscanf(line.c_str(), "%d,%d", &x, &y);
		isByte[x + y * WIDTH] = true;
		step++;
	}
	if (step != STEP_SIM) {
		std::cerr << "Error: step = " << step << std::endl;
	}

	// printMap(WIDTH, HEIGHT, isByte);

	return std::get<0>(
		dijkstra(WIDTH, HEIGHT, isByte, 0, 0, WIDTH - 1, HEIGHT - 1));
}

bool isInPath(int findIndex, std::vector<int> &prev, int endIndex) {
	int index = endIndex;
	while (index != -1) {
		if (index == findIndex) {
			return true;
		}
		index = prev[index];
	}
	return false;
}

std::string part2() {
	auto file = utils::getInput();

	const int WIDTH = 71;
	const int HEIGHT = 71;

	std::vector<bool> isByte(WIDTH * HEIGHT, false);
	std::string line;

	int step = 0;
	std::vector<int> prev;
	while (std::getline(file, line)) {
		int x = 0, y = 0;
		sscanf(line.c_str(), "%d,%d", &x, &y);
		isByte[x + y * WIDTH] = true;
		step++;

		if (step != 1 && !isInPath(y * WIDTH + x, prev, WIDTH * HEIGHT - 1)) {
			continue;
		}

		auto res = dijkstra(WIDTH, HEIGHT, isByte, 0, 0, WIDTH - 1, HEIGHT - 1);
		if (std::get<0>(res) < INT_MAX) {
			prev = std::get<1>(res);
		} else {
			// not possible to reach the end anymore
			return std::to_string(x) + "," + std::to_string(y);
		}
	}
	// printMap(WIDTH, HEIGHT, isByte);
	std::cerr << "Error: no solution found" << std::endl;
	exit(1);
}
