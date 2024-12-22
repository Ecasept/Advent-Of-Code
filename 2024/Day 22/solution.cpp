#include "../lib/utils.h"
#include <unordered_map>

inline llu mix(llu value, llu secretValue) { return value ^ secretValue; }
inline llu prune(llu value) { return value % 16777216; }

inline llu step(llu secretValue) {
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

inline llu hashChanges(int changes[4], int firstChangeIndex) {
	llu changeHash = 0;
	changeHash += changes[firstChangeIndex] + 10;
	changeHash += (changes[(firstChangeIndex + 1) % 4] + 10) * 100;
	changeHash += (changes[(firstChangeIndex + 2) % 4] + 10) * 10000;
	changeHash += (changes[(firstChangeIndex + 3) % 4] + 10) * 1000000;
	return changeHash;
}

llu part2() {
	auto file = utils::getInput();
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
			if (i >= 3) {
				llu changeHash = hashChanges(changes, firstChange);
				if (!seenChanges[changeHash]) {
					seenChanges[changeHash] = true;
					priceForChange[changeHash] += price;
				}
			}

			prevPrice = price;
		}
		sum += secretValue;
	}

	int maxPrice = 0;
	for (auto &change : priceForChange) {
		if (change.second > maxPrice) {
			maxPrice = change.second;
		}
	}
	return maxPrice;
}
