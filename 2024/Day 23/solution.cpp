#include "../lib/utils.h"
#include <algorithm>
#include <set>
#include <unordered_map>
#include <vector>

typedef std::unordered_map<std::string, std::vector<std::string>> Connections;
typedef std::vector<std::tuple<std::string, std::string, std::string>> Triples;

bool hasT(const std::string &port) { return port.find("t") == 0; }

void getTriples(const std::string &port1, Connections &connections,
				Triples &triples) {
	for (const auto &port2 : connections[port1]) {
		for (const auto &port3 : connections[port2]) {
			// Check if connection from port3 to port1 exists
			for (const auto &connection : connections[port3]) {
				if (connection == port1) {
					auto hasT = [](const std::string &port) {
						return port.find("t") == 0;
					};

					if (!(hasT(port1) || hasT(port2) || hasT(port3))) {
						continue;
					}

					std::string a = port1;
					std::string b = port2;
					std::string c = port3;
					if (a > b) {
						std::swap(a, b);
					}
					if (a > c) {
						std::swap(a, c);
					}
					if (b > c) {
						std::swap(b, c);
					}
					triples.push_back(std::make_tuple(a, b, c));
				}
			}
		}
	}
}

llu part1() {
	auto file = utils::getInput();

	std::string line;

	Connections connections;

	Triples triples;

	while (std::getline(file, line)) {
		std::string aStr = line.substr(0, line.find("-"));
		std::string bStr = line.substr(line.find("-") + 1);
		connections[aStr].push_back(bStr);
		connections[bStr].push_back(aStr);
	}

	for (const auto &connection : connections) {
		// Append triples
		getTriples(connection.first, connections, triples);
	}

	// Sort out duplicates
	std::sort(triples.begin(), triples.end());
	triples.erase(std::unique(triples.begin(), triples.end()), triples.end());

	return triples.size();
}
struct MemoKey {
	std::set<std::string> cluster;
	std::string port;
	int size;

	bool operator==(const MemoKey &other) const {
		return cluster == other.cluster && port == other.port &&
			   size == other.size;
	}
};

// Add hash function specialization
namespace std {
template <> struct hash<MemoKey> {
	size_t operator()(const MemoKey &k) const {
		size_t h1 = hash<string>{}(k.port);
		size_t h2 = hash<int>{}(k.size);

		// Hash the vector contents
		size_t h3 = 0;
		for (const string &s : k.cluster) {
			h3 ^= hash<string>{}(s) + 0x9e3779b9 + (h3 << 6) + (h3 >> 2);
		}

		// Combine all hashes
		return h1 ^ (h2 << 1) ^ (h3 << 2);
	}
};
} // namespace std

std::unordered_map<MemoKey, int> memo;
int getLargestClusterSize(const std::string &port1, Connections &connections,
						  std::set<std::string> currentCluster,
						  int currentClusterSize) {
	MemoKey key{currentCluster, port1, currentClusterSize};
	if (memo.find(key) != memo.end()) {
		return memo[key];
	}

	int maxClusterSize = currentClusterSize;
	for (const auto &newPort : connections[port1]) {

		bool isNew = true;
		for (const auto &port : currentCluster) {
			if (newPort == port) {
				// newPort already present in cluster
				isNew = false;
				break;
			}
			// If newPort is not connected to port
			if (std::find(connections[newPort].begin(),
						  connections[newPort].end(),
						  port) == connections[newPort].end()) {
				isNew = false;
				break;
			}
		}
		if (isNew) {

			auto [index, _] = currentCluster.insert(newPort);

			// std::push_heap(currentCluster.begin(), currentCluster.end(),
			//                std::greater<std::string>());
			int newSize = getLargestClusterSize(
				newPort, connections, currentCluster, currentClusterSize + 1);

			currentCluster.erase(index);

			if (newSize > maxClusterSize) {
				maxClusterSize = newSize;
			}
		}
	}
	memo[key] = maxClusterSize;
	return maxClusterSize;
}

std::string part2() {
	auto file = utils::getInput();

	std::string line;

	Connections connections;

	while (std::getline(file, line)) {
		std::string aStr = line.substr(0, line.find("-"));
		std::string bStr = line.substr(line.find("-") + 1);
		connections[aStr].push_back(bStr);
		connections[bStr].push_back(aStr);
	}

	int maxClusterSize = 0;
	// int i = 0;
	for (const auto &port : connections) {
		// std::cout << i++ << std::endl;

		int size =
			getLargestClusterSize(port.first, connections, {port.first}, 1);
		if (size > maxClusterSize) {
			maxClusterSize = size;
		}
	}

	for (const auto &m : memo) {
		if (m.first.size == maxClusterSize) {
			std::string out = "";
			bool first = true;
			for (const auto &s : m.first.cluster) {
				if (first) {
					out += s;
					first = false;
				} else {
					out += "," + s;
				}
			}
			return out;
		}
	}
	std::cout << "Max cluster size: " << maxClusterSize << std::endl;
	std::cerr << "No cluster found" << std::endl;
	exit(1);
}
