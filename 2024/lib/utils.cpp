#include <fstream>
#include <iostream>

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
std::ifstream getInput(int num) {
	num = num; // to avoid unused parameter warning
	return getInput();
}

bool isValidPosition(int x, int y, int width, int height) {
	return x >= 0 && x < width && y >= 0 && y < height;
}

int toIndex(int x, int y, int width) { return y * width + x; }

} // namespace utils
