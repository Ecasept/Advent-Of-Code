#include <fstream>
#include <iostream>

namespace utils {

std::ifstream getExample(int num) {
    std::ifstream test_file;
    test_file.open("ex" + std::to_string(num) + ".txt");
    return test_file;
}

std::ifstream getInput(int num) {
    std::ifstream input_File;
    input_File.open("input" + std::to_string(num) + ".txt");
    return input_File;
}

} // namespace utils
