#include <fstream>
#include <iostream>
#include <vector>

typedef unsigned long long llu;

namespace utils {

// Forward declaration
class Point;

std::ifstream getExample(int num);

std::ifstream getInput(int num);

std::ifstream getExample();

std::ifstream getInput();

bool isValidPosition(int x, int y, int width, int height);

int toIndex(int x, int y, int width);

std::vector<std::string> split(std::string s, std::string delim);

class Grid {
  private:
	std::vector<char> grid;

  public:
	int width;
	int height;
	int size;

  public:
	void loadFromFile(std::ifstream &file);
	Point find(char c);

	// -- Accessors --
	char *at(Point p);
	char *at(int index);

	// -- Conversion --
	int toIndex(Point p);
	Point toPoint(int index);

	// -- Other --
	Point inDir(Point p, int dir);
	bool isValidPosition(Point p);
};
class Point {
  public:
	int x, y;

  public:
	Point(int xCoord, int yCoord);

	Point operator+(const Point &other) const;

	Point operator-(const Point &other) const;

	Point operator-() const;

	Point &operator+=(const Point &other);

	Point &operator-=(const Point &other);

	bool operator==(const Point &other) const;

	int manhattan() const;

	double euklidian() const;

	int manhattanTo(const Point &other) const;
};

} // namespace utils
