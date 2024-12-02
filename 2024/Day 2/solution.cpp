#include "../utils.h"
#include <sstream>
#include <vector>

void part1() {
    auto file = utils::getInput(1);

    std::string line;

    int sum = 0;

    while (std::getline(file, line)) {
        int prev = 0;
        bool checkedIncreasing = false;
        bool isIncreasing = false;
        std::istringstream stream(line);
        bool isSafe = true;
        std::string num_str;

        while (std::getline(stream, num_str, ' ')) {
            int num = std::stoi(num_str);
            int dif = num - prev;
            if (prev == 0) {
                // set prev on first run
                prev = num;
                continue;
            }
            if (!checkedIncreasing) {
                // set increasing on second run
                checkedIncreasing = true;
                isIncreasing = dif > 0;
            }

            if (abs(dif) >= 4 or abs(dif) <= 0) {
                isSafe = false;
                break;
            } else {
                if ((isIncreasing and dif < 0) or (!isIncreasing and dif > 0)) {
                    isSafe = false;
                    break;
                }
            }
            prev = num;
        }
        if (isSafe) {
            sum++;
        }
    }

    std::cout << sum << '\n';
}

void part2() {
    // yeah im not putting in the effort to optimize this

    auto file = utils::getInput(1);

    std::string line;

    int sum = 0;

    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::vector<int> nums;
        std::string num_str;

        while (std::getline(stream, num_str, ' ')) {
            nums.push_back(std::stoi(num_str));
        }
        for (int excludeIndex = 0; excludeIndex < nums.size(); excludeIndex++) {
            int prev = 0;
            bool checkedIncreasing = false;
            bool isIncreasing = false;
            bool isSafe = true;
            for (int i = 0; i < nums.size(); i++) {
                if (i == excludeIndex) {
                    continue;
                }
                int num = nums[i];
                int dif = num - prev;
                if (prev == 0) {
                    // set prev on first run
                    prev = num;
                    continue;
                }
                if (!checkedIncreasing) {
                    // set increasing on second run
                    checkedIncreasing = true;
                    isIncreasing = dif > 0;
                }

                if (abs(dif) >= 4 or abs(dif) <= 0) {
                    isSafe = false;
                    break;
                } else {
                    if ((isIncreasing and dif < 0) or
                        (!isIncreasing and dif > 0)) {
                        isSafe = false;
                        break;
                    }
                }
                prev = num;
            }
            if (isSafe) {
                sum++;
                break;
            }
        }
    }

    std::cout << sum << '\n';
}

int main() {
    part1();
    part2();
    return 0;
}
