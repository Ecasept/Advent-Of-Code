#include "../lib/utils.h"
#include <algorithm>
#include <set>
#include <unordered_map>
#include <vector>

typedef std::unordered_map<std::string, std::vector<std::string>> Connections;
typedef std::unordered_map<std::string, std::set<std::string>> ConnectionsSet;
typedef std::set<std::tuple<std::string, std::string, std::string>> Triples;

bool hasT(const std::string &port) { return port.find("t") == 0; }

ConnectionsSet connections;

void getTriples(const std::string &port1, Connections &cons, Triples &triples) {
	for (const auto &port2 : cons[port1]) {
		for (const auto &port3 : cons[port2]) {
			// Check if connection from port3 to port1 exists
			for (const auto &connection : cons[port3]) {
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
					triples.insert(std::make_tuple(a, b, c));
				}
			}
		}
	}
}

llu part1() {
	auto file = utils::getInput();

	std::string line;

	Triples triples;

	Connections cons;

	while (std::getline(file, line)) {
		std::string aStr = line.substr(0, line.find("-"));
		std::string bStr = line.substr(line.find("-") + 1);
		cons[aStr].push_back(bStr);
		cons[bStr].push_back(aStr);
	}

	for (const auto &connection : cons) {
		getTriples(connection.first, cons, triples);
	}

	return triples.size();
}

size_t maxCliqueSize = 0;
std::set<std::string> maxClique;

void bronKerbosch(std::set<std::string> &R, std::set<std::string> P,
				  std::set<std::string> X) {
	if (maxCliqueSize == 13) {
		// based on inspection of the input
		return;
	}
	if (P.empty() and X.empty()) {
		if (R.size() > maxCliqueSize) {
			maxClique = R;
			maxCliqueSize = R.size();
		}
		return;
	}
	while (!P.empty()) {
		const auto vertex = *P.begin();
		const auto &neighbors = connections[vertex];
		R.insert(vertex);
		std::set<std::string> newP;
		std::set<std::string> newX;
		std::set_intersection(P.begin(), P.end(), neighbors.begin(),
							  neighbors.end(),
							  std::inserter(newP, newP.begin()));
		std::set_intersection(X.begin(), X.end(), neighbors.begin(),
							  neighbors.end(),
							  std::inserter(newX, newX.begin()));
		bronKerbosch(R, newP, newX);
		R.erase(vertex);
		P.erase(vertex);
		X.insert(vertex);
	}
}

std::string part2() {
	connections.clear();
	auto file = utils::getInput();

	std::string line;

	std::set<std::string> P;
	while (std::getline(file, line)) {
		std::string aStr = line.substr(0, line.find("-"));
		std::string bStr = line.substr(line.find("-") + 1);
		connections[aStr].insert(bStr);
		connections[bStr].insert(aStr);
		P.insert(aStr);
		P.insert(bStr);
	}

	std::set<std::string> R;
	std::set<std::string> X;
	bronKerbosch(R, P, X);

	std::string out = "";
	bool first = true;
	for (const auto &s : maxClique) {
		if (first) {
			out += s;
			first = false;
		} else {
			out += "," + s;
		}
	}
	return out;
}
