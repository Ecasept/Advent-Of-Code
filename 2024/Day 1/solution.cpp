#include "../lib/utils.h"
#include <algorithm>
#include <unordered_map>
#include <vector>

llu part1() {
    auto file = utils::getInput(1);
    std::vector<int> list1;
    std::vector<int> list2;

    std::string line;

    std::string del = "   ";

    while (std::getline(file, line)) {
        auto first = line.substr(0, line.find(del));
        auto second = line.substr(line.find(del) + del.length());
        list1.push_back(std::stoi(first));
        list2.push_back(std::stoi(second));
    }
    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    int sum = 0;

    for (int i = 0; i < list1.size(); i++) {
        sum += abs(list1[i] - list2[i]);
    }

    return sum;
}

llu part2() {
    auto file = utils::getInput(1);
    std::vector<int> list1;
    std::vector<int> list2;

    std::string line;

    std::string del = "   ";

    while (std::getline(file, line)) {
        auto first = line.substr(0, line.find(del));
        auto second = line.substr(line.find(del) + del.length());
        list1.push_back(std::stoi(first));
        list2.push_back(std::stoi(second));
    }

    // cache lookups
    std::unordered_map<int, int> map;

    for (const auto &num : list2) {
        map[num]++;
    }

    int sum = 0;

    for (const auto &num : list1) {
        if (map.find(num) != map.end()) {
            sum += num * map.at(num);
        }
    }

    return sum;
}
