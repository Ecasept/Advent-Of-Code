#include "../lib/utils.h"
#include <unordered_map>

// I guess you could optimize this with bit operations but the compiler is
// probably going to do that for you anyway

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

inline constexpr int hashChanges(int changes[4], int firstChangeIndex) {
	int result = 0;
	for (int i = 0; i < 4; i++) {
		result *= 19;
		result += changes[(firstChangeIndex + i) % 4] + 9;
	}
	return result;
}

llu part2() {
	const int MAX_HASH = 19 * 19 * 19 * 19;
	auto file = utils::getInput();
	std::string line;

	int priceForChange[MAX_HASH + 1] = {0};
	while (std::getline(file, line)) {
		llu secretValue = std::stoull(line);
		int changes[4];
		unsigned prevPrice = secretValue % 10;

		bool seenChanges[MAX_HASH + 1] = {false};

		for (int i = 0; i < 2000; i++) {
			secretValue = step(secretValue);

			int price = secretValue % 10;

			int lastChange = (i + 4) % 4;
			int firstChange = (i + 1) % 4;
			changes[lastChange] = price - prevPrice;
			if (i >= 3) {
				int changeHash = hashChanges(changes, firstChange);
				if (!seenChanges[changeHash]) {
					seenChanges[changeHash] = true;
					priceForChange[changeHash] += price;
				}
			}

			prevPrice = price;
		}
	}

	int maxPrice = 0;
	for (auto &change : priceForChange) {
		if (change > maxPrice) {
			maxPrice = change;
		}
	}
	return maxPrice;
}
