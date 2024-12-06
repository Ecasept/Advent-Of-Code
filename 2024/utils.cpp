#include <fstream>
#include <iostream>

namespace utils {

std::ifstream getExample(int num) {
	std::ifstream test_file;
	test_file.open("ex" + std::to_string(num) + ".txt");
	return test_file;
}

std::ifstream getInput() {
	std::ifstream input_File;
	input_File.open("input.txt");
	return input_File;
}

// too lazy to replace the parameter when I change from getExample to getInput
std::ifstream getInput(int num) { return getInput(); }

} // namespace utils
