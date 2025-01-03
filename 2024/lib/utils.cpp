#include "utils.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

namespace utils {

std::ifstream getExample(int num) {
	std::ifstream test_file;
	auto filename = "ex" + std::to_string(num) + ".txt";
	test_file.open(filename);
	if (!test_file) {
		std::cerr << "Could not open \"" << filename << "\"" << std::endl;
		exit(1);
	}
	return test_file;
}

std::ifstream getExample() {
	std::ifstream test_file;
	std::string filename = "ex1.txt";
	test_file.open(filename);
	if (!test_file) {
		std::cerr << "Could not open \"" << filename << "\"" << std::endl;
		exit(1);
	}
	return test_file;
}

std::ifstream getInput() {
	std::ifstream input_file;
	input_file.open("input.txt");
	if (!input_file) {
		std::cerr << "Could not open \"input.txt\"" << std::endl;
		exit(1);
	}
	return input_file;
}

// too lazy to replace the parameter when I change from getExample to getInput
std::ifstream getInput([[maybe_unused]] int num) { return getInput(); }

bool isValidPosition(int x, int y, int width, int height) {
	return x >= 0 && x < width && y >= 0 && y < height;
}

std::vector<std::string> split(std::string s, std::string delim) {
	std::vector<std::string> result;
	while (true) {
		auto pos = s.find(delim);
		if (pos == std::string::npos) {
			result.push_back(s);
			break;
		}
		result.push_back(s.substr(0, pos));
		s = s.substr(pos + delim.size());
	}
	return result;
}

int toIndex(int x, int y, int width) { return y * width + x; }

void Grid::loadFromFile(std::ifstream &file) {
	std::string line;

	size_t height = 0;
	size_t width = 0;
	while (std::getline(file, line)) {
		width = line.size();
		for (char c : line) {
			grid.push_back(c);
		}
		height++;
	}
	if (height > std::numeric_limits<int>::max() ||
		width > std::numeric_limits<int>::max()) {
		std::cerr << "Grid too large" << std::endl;
		exit(1);
	}
	if (height == 0 || width == 0) {
		std::cerr << "Empty grid" << std::endl;
		exit(1);
	}
	this->width = width;
	this->height = height;
	this->size = width * height;
}

Point Grid::find(char c) {
	for (int i = 0; i < this->size; i++) {
		if (grid[i] == c) {
			return this->toPoint(i);
		}
	}
	std::cerr << "Could not find " << c << std::endl;
	exit(1);
}

char *Grid::at(Point p) { return &grid[this->toIndex(p)]; }
char *Grid::at(int index) { return &grid[index]; }

Point Grid::inDir(Point p, int dir) {
	switch (dir) {
	case 0:
		p.y--;
		break;
	case 1:
		p.x++;
		break;
	case 2:
		p.y++;
		break;
	case 3:
		p.x--;
		break;
	default:
		std::cerr << "Invalid direction " << dir << std::endl;
		exit(1);
	}
	return p;
}

bool Grid::isValidPosition(Point p) {
	return p.x >= 0 && p.x < width && p.y >= 0 && p.y < height;
}
int Grid::toIndex(Point p) {
	if (!this->isValidPosition(p)) {
		std::cerr << "Invalid point " << p.x << ", " << p.y << std::endl;
		exit(1);
	}
	return p.y * width + p.x;
}
Point Grid::toPoint(int index) { return {index % width, index / width}; }

// -- Point --

Point::Point(int xCoord, int yCoord) : x(xCoord), y(yCoord) {}

Point Point::operator+(const Point &other) const {
	return {x + other.x, y + other.y};
}
Point Point::operator-(const Point &other) const {
	return {x - other.x, y - other.y};
}

Point Point::operator-() const { return Point(-x, -y); }

Point &Point::operator+=(const Point &other) {
	x += other.x;
	y += other.y;
	return *this;
}

Point &Point::operator-=(const Point &other) {
	x -= other.x;
	y -= other.y;
	return *this;
}

bool Point::operator==(const Point &other) const {
	return x == other.x && y == other.y;
}

int Point::manhattan() const { return std::abs(x) + std::abs(y); }

double Point::euklidian() const { return std::sqrt(x * x + y * y); }

int Point::manhattanTo(const Point &other) const {
	return std::abs(x - other.x) + std::abs(y - other.y);
}

} // namespace utils
