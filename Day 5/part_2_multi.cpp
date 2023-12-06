#include <fstream>
#include <iostream>
#include <limits>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

std::mutex m;

std::vector<long> toNumList(const std::string &str) {
    std::vector<long> result;
    std::istringstream iss(str);
    long num;
    while (iss >> num) {
        result.push_back(num);
    }
    return result;
}

void prlongMaps(const std::vector<std::vector<std::vector<long>>> &maps) {
    for (const auto &layer : maps) {
        std::cout << "Layer:" << std::endl;
        for (const auto &row : layer) {
            for (long num : row) {
                std::cout << num << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "-----------------------" << std::endl;
    }
}

class Range {
  public:
    Range(long startNum, long endNum) {
        start = startNum;
        end = endNum;
        currentNum = startNum;
    }
    long next() {
        if (currentNum == end) {
            return -1;
        }
        currentNum++;
        return currentNum - 1;
    }

    friend std::ostream &operator<<(std::ostream &os, const Range &range) {
        os << "Range(" << range.start << ", " << range.end << ")";
        return os;
    }

  private:
    long start;
    long end;
    long currentNum;
};

long calc(long num, const std::vector<std::vector<std::vector<long>>> &maps) {
    for (const auto &map : maps) {
        for (const auto &mapping : map) {
            long dst = mapping[0];
            long src = mapping[1];
            long length = mapping[2];
            if (src <= num && num <= src + length - 1) {
                long dif = num - src;
                num = dst + dif;
                break;
            }
        }
    }
    return num;
}

long threadFunc(Range &seed,
                const std::vector<std::vector<std::vector<long>>> &maps,
                long &minResult) {
    long i = 0;
    long result = seed.next();
    while (result != -1) {
        long calcResult = calc(result, maps);
        if (calcResult < minResult) {
            m.lock();
            minResult = std::min(minResult, calcResult);
            m.unlock();
        }

        result = seed.next(); // Get the next value from the Range

        if (i % 100000 == 0) {
            if (i != 0) {
                std::cout << i << std::endl;
            }
        }
        i++;
    }

    return minResult;
}

int main() {
    std::ifstream file("input.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }
    std::string line;

    std::vector<std::vector<std::string>> data;
    data.push_back(std::vector<std::string>()); // add first element

    long map_index = 0;

    // Get seeds
    std::getline(file, line);
    std::string trimmedString = line.substr(line.find(": ") + 2);
    // Remove the newline at the end
    if (!trimmedString.empty() && trimmedString.back() == '\n') {
        trimmedString.pop_back();
    }
    std::vector<long> seed_numbers = toNumList(trimmedString);
    std::getline(file, line); // remove empty line after seeds

    while (std::getline(file, line)) {
        if (line.empty()) {
            map_index += 1;

            if (map_index >= data.size()) {
                data.push_back(std::vector<std::string>());
            }
        } else {
            data[map_index].push_back(line);
        }
    }

    std::vector<std::vector<std::vector<long>>> maps;
    long mapIndex = 0;
    for (const auto &map : data) {
        maps.push_back(std::vector<std::vector<long>>());
        long firstLine = true;
        for (const auto &line : map) {
            if (firstLine) {
                firstLine = false;
                continue;
            }
            std::string modifiedLine = line;
            if (modifiedLine.back() == '\n') {
                modifiedLine.pop_back();
            }
            std::vector<long> numList = toNumList(modifiedLine);
            maps[mapIndex].push_back(numList);
        }
        mapIndex++;
    }

    std::vector<Range> seeds;
    for (size_t i = 0; i < seed_numbers.size(); i += 2) {
        long start = seed_numbers[i];
        long length = seed_numbers[i + 1];
        seeds.emplace_back(start, start + length);
    }

    /*prlongMaps(maps);
    std::cout << "Seeds:" << std::endl;
    for (const auto &seed : seeds) {
        std::cout << seed << std::endl;
    }*/

    long minResult = std::numeric_limits<long>::max();
    std::vector<std::thread> threads;

    for (auto &seedRange : seeds) {
        threads.emplace_back(threadFunc, std::ref(seedRange), std::cref(maps),
                             std::ref(minResult));
    }

    for (auto &thread : threads) {
        thread.join();
    }

    // long min = findMinCalc(seeds, maps);
    std::cout << minResult << std::endl;
}
