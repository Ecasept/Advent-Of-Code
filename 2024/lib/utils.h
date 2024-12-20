#include <fstream>
#include <iostream>
#include <vector>

typedef unsigned long long llu;

namespace utils {
std::ifstream getExample(int num);

std::ifstream getInput(int num);

std::ifstream getExample();

std::ifstream getInput();

bool isValidPosition(int x, int y, int width, int height);

int toIndex(int x, int y, int width);

std::vector<std::string> split(std::string s, std::string delim);

typedef std::pair<int, int> Point;

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

} // namespace utils
