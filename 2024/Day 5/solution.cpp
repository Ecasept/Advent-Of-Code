#include "../utils.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

std::vector<std::string> splitString(const std::string &str, char delimiter) {
	std::vector<std::string> result;
	std::istringstream stream(str);
	std::string token;

	while (std::getline(stream, token, delimiter)) {
		result.push_back(token);
	}

	return result;
}

void part1() {
	auto file = utils::getInput(1);
	std::string line;

	std::unordered_map<int, std::vector<int>> pagesAfter;

	while (std::getline(file, line)) {
		if (line == "") {
			// first part parsed
			break;
		}
		auto splitIndex = line.find('|');
		int firstNum = std::stoi(line.substr(0, splitIndex));
		int secondNum = std::stoi(line.substr(splitIndex + 1));
		pagesAfter[firstNum].push_back(secondNum);
	}

	int sum = 0;

	while (std::getline(file, line)) {
		std::unordered_map<int, bool> pageHasOccurred;
		auto nums = splitString(line, ',');
		bool isInOrder = true;

		for (const std::string &num : nums) {
			int numInt = std::stoi(num);
			auto after = pagesAfter[numInt];
			for (int page : after) {
				if (pageHasOccurred[page]) {
					isInOrder = false;
					break;
				}
			}
			if (!isInOrder) {
				break;
			}
			pageHasOccurred[numInt] = true;
		}

		if (!isInOrder) {
			continue;
		}

		// page is ordered correctly
		auto middlePage = std::stoi(nums[nums.size() / 2]);
		sum += middlePage;
	}

	// for all in pages_after, ensure page_has_ocurred is false

	std::cout << sum << '\n';
}

void part2() {
	auto file = utils::getInput(1);
	std::string line;

	std::unordered_map<int, std::vector<int>> pagesAfter;

	while (std::getline(file, line)) {
		if (line == "") {
			// first part parsed
			break;
		}
		auto splitIndex = line.find('|');
		int firstNum = std::stoi(line.substr(0, splitIndex));
		int secondNum = std::stoi(line.substr(splitIndex + 1));
		pagesAfter[firstNum].push_back(secondNum);
	}

	int sum = 0;

	while (std::getline(file, line)) {
		std::unordered_map<int, bool> pageHasOccurred;
		auto nums = splitString(line, ',');
		bool isInOrder = true;

		// return true if a is infront of b
		auto comparisonFunction = [&](const std::string &a,
									  const std::string &b) {
			int num1 = std::stoi(a);
			int num2 = std::stoi(b);

			auto after = pagesAfter[num1];
			for (int page : after) {
				if (page == num2) {
					return false;
				}
			}
			return true;
		};

		for (const std::string &num : nums) {
			int numInt = std::stoi(num);
			auto after = pagesAfter[numInt];
			for (int page : after) {
				if (pageHasOccurred[page]) {
					isInOrder = false;
					break;
				}
			}
			if (!isInOrder) {
				break;
			}
			pageHasOccurred[numInt] = true;
		}

		if (!isInOrder) {
			std::sort(nums.begin(), nums.end(), comparisonFunction);
			auto middlePage = std::stoi(nums[nums.size() / 2]);
			sum += middlePage;
		}
	}

	// for all in pages_after, ensure page_has_ocurred is false

	std::cout << sum << '\n';
}

int main() {
	part1();
	part2();
	return 0;
}
