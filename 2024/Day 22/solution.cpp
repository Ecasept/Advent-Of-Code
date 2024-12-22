#include "../lib/utils.h"

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

llu part2() { return 0; }
