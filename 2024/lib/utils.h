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
} // namespace utils
