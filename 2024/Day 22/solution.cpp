#include "../lib/utils.h"
#include <unordered_map>

inline llu mix(llu value, llu secretValue) { return value ^ secretValue; }
inline llu prune(llu value) { return value % 16777216; }

llu step(llu secretValue) {
	secretValue = prune(mix(secretValue * 64, secretValue));
	secretValue = prune(mix(secretValue / 32, secretValue));
	secretValue = prune(mix(secretValue * 2048, secretValue));
	return secretValue;
}

llu part1() {
	auto file = utils::getInput();
	std::string line;

	llu sum = 0;
	while (std::getline(file, line)) {
		llu secretValue = std::stoull(line);
		for (int i = 0; i < 2000; i++) {
			secretValue = step(secretValue);
		}
		sum += secretValue;
	}
	return sum;
}

// llu hashChange(llu changes[4]) {
// 	llu changeHash = 0;
// 	changeHash += changes[0] + 10;
// 	changeHash += (changes[1] + 10) * 100;
// 	changeHash += (changes[2] + 10) * 10000;
// 	changeHash += (changes[3] + 10) * 1000000;
// 	return changeHash;
// }

llu hashChanges(int changes[4], int firstChangeIndex) {
	llu changeHash = 0;
	changeHash += changes[firstChangeIndex] + 10;
	changeHash += (changes[(firstChangeIndex + 1) % 4] + 10) * 100;
	changeHash += (changes[(firstChangeIndex + 2) % 4] + 10) * 10000;
	changeHash += (changes[(firstChangeIndex + 3) % 4] + 10) * 1000000;
	return changeHash;
}

llu part2() {
	auto file = utils::getExample();
	std::string line;

	llu sum = 0;
	std::unordered_map<llu, int> priceForChange;
	while (std::getline(file, line)) {
		llu secretValue = std::stoull(line);
		int changes[4];
		int prevPrice = secretValue % 10;

		std::unordered_map<llu, bool> seenChanges;

		for (int i = 0; i < 2000; i++) {
			secretValue = step(secretValue);

			int price = secretValue % 10;

			int lastChange = (i + 4) % 4;
			int firstChange = (i + 1) % 4;
			changes[lastChange] = price - prevPrice;
			std::cout << "Price: " << price
					  << ", Change: " << changes[lastChange] << std::endl;
			if (i >= 3) {
				llu changeHash = hashChanges(changes, firstChange);
				if (seenChanges.find(changeHash) != seenChanges.end()) {
					break;
				}
				seenChanges[changeHash] = true;
				priceForChange[changeHash] += price;
				std::cout << "Change hash: " << changeHash << std::endl;
			}

			std::cout << "List of changes: ";
			for (int i = firstChange; true; i = (i + 1) % 4) {
				std::cout << changes[i] << " ";
				if (i == lastChange) {
					break;
				}
			}

			prevPrice = price;
			std::cout << '\n' << std::endl;
		}
		sum += secretValue;
	}

	int test[4] = {-2, 1, -1, 3};
	std::cout << "Hash of ex: " << hashChanges(test, 0) << std::endl;
	// for every change
	for (auto &change : priceForChange) {
		std::cout << change.second << std::endl;
	}
	return sum;
}
