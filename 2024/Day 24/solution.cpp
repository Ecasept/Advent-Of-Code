#include "../lib/utils.h"
#include <map>
#include <set>

enum class GateType { AND, OR, XOR, INIT };

class Gate {
  public:
	GateType type;
	std::vector<std::string> inputs;
	int initValue = -1;

  public:
};

GateType getGateType(std::string op) {
	switch (op[0]) {
	case 'A':
		return GateType::AND;
	case 'O':
		return GateType::OR;
	case 'X':
		return GateType::XOR;
	default:
		std::cerr << "Invalid gate type: " << op << std::endl;
		exit(1);
	}
}

int getGateOutput(std::map<std::string, Gate> &gates, std::string name) {
	Gate gate = gates[name];
	switch (gate.type) {
	case GateType::INIT:
		return gate.initValue;
	case GateType::AND:
		return getGateOutput(gates, gate.inputs[0]) &
			   getGateOutput(gates, gate.inputs[1]);
	case GateType::OR:
		return getGateOutput(gates, gate.inputs[0]) |
			   getGateOutput(gates, gate.inputs[1]);
	case GateType::XOR:
		return getGateOutput(gates, gate.inputs[0]) ^
			   getGateOutput(gates, gate.inputs[1]);
	default:
		std::cerr << "Invalid gate type: " << (int)gate.type << std::endl;
		exit(1);
	}
}

std::string toStr(GateType type) {
	switch (type) {
	case GateType::AND:
		return "AND";
	case GateType::OR:
		return "OR";
	case GateType::XOR:
		return "XOR";
	case GateType::INIT:
		return "INIT";
	default:
		std::cerr << "Invalid gate type: " << (int)type << std::endl;
		exit(1);
	}
}

int getPrintGateOutput(std::map<std::string, Gate> &gates, std::string name,
					   int depth) {
	auto gate = gates[name];

	// if (gate.type == GateType::INIT) {
	// 	std::cout << std::string(depth, ' ') << name << " : " << gate.initValue
	// 			  << std::endl;
	// } else {
	// 	std::cout << std::string(depth, ' ') << name << " : "
	// 			  << toStr(gate.type) << std::endl;
	// }

	switch (gate.type) {
	case GateType::INIT:
		return gate.initValue;
	case GateType::AND:
		return getPrintGateOutput(gates, gate.inputs[0], depth + 1) &
			   getPrintGateOutput(gates, gate.inputs[1], depth + 1);
	case GateType::OR:
		return getPrintGateOutput(gates, gate.inputs[0], depth + 1) |
			   getPrintGateOutput(gates, gate.inputs[1], depth + 1);
	case GateType::XOR:
		return getPrintGateOutput(gates, gate.inputs[0], depth + 1) ^
			   getPrintGateOutput(gates, gate.inputs[1], depth + 1);
	default:
		std::cerr << "Invalid gate type: " << (int)gate.type << std::endl;
		exit(1);
	}
}

size_t findNth(std::string s, char c, int n) {
	size_t pos = -1;
	for (int i = 0; i < n; i++) {
		pos = s.find(c, pos + 1);
		if (pos == std::string::npos) {
			return -1;
		}
	}
	return pos;
}

void initGates(std::map<std::string, Gate> &gates, std::ifstream &file,
			   int initVal) {
	std::string line;

	while (std::getline(file, line)) {
		if (line == "") {
			// go to second part
			break;
		}

		std::string name = line.substr(0, line.find(":"));
		int value = std::stoi(line.substr(line.find(": ") + 2));

		gates[name] = {GateType::INIT, {}, initVal == -1 ? value : initVal};
	}

	while (std::getline(file, line)) {
		size_t firstSpace = line.find(" ");
		size_t secondSpace = findNth(line, ' ', 2);
		size_t arrow = line.find(" -> ");

		std::string input1 = line.substr(0, firstSpace);
		std::string op =
			line.substr(firstSpace + 1, secondSpace - firstSpace - 1);
		std::string input2 =
			line.substr(secondSpace + 1, arrow - secondSpace - 1);
		std::string output = line.substr(arrow + 4);

		GateType type = getGateType(op);
		gates[output] = {type, {input1, input2}};
	}
}

llu getResult(std::map<std::string, Gate> &gates) {
	llu result = 0;
	for (auto &gate : gates) {
		if (gate.first[0] == 'z') {
			llu res = getGateOutput(gates, gate.first);
			result ^= res;
		}
	}
	return result;
}

llu part1() {
	auto file = utils::getInput();
	std::string line;

	std::map<std::string, Gate> gates;

	initGates(gates, file, -1);

	int i = 0;
	llu result = 0;
	for (auto &gate : gates) {
		if (gate.first[0] == 'z') {
			llu res = getGateOutput(gates, gate.first);
			result ^= res << i;
			i++;
		}
	}

	return result;
}

std::string part2() {
	auto file = utils::getInput();
	std::string line;

	std::map<std::string, Gate> gates;

	initGates(gates, file, 0);

	std::set<std::string> swappedGates;

	for (int i = 0; i < 46; i++) {
		// Get next number
		std::string num = std::to_string(i);
		num.insert(num.begin(), 2 - num.size(), '0');
		gates["x" + num].initValue = 1;
		gates["y" + num].initValue = 1;

		getPrintGateOutput(gates, "z" + num, 0);
	}

	for (auto &entry : gates) {
		auto &gate = entry.second;
		auto &name = entry.first;

		if (gate.type == GateType::XOR) {
			auto ip1 = gate.inputs[0][0];
			auto ip2 = gate.inputs[1][0];
			if ((ip1 == 'x' or ip1 == 'y') or (ip2 == 'x' or ip2 == 'y')) {
				if (name[0] == 'z' && name != "z00") {
					swappedGates.insert(name);
				}
			} else {
				if (name[0] != 'z') {
					swappedGates.insert(name);
				}
			}
		}

		if (name[0] == 'z') {
			if (gate.type != GateType::XOR) {
				if (name != "z45") {
					swappedGates.insert(name);
				}
			}
		}

		if (gate.type == GateType::OR) {
			auto ip1 = gates[gate.inputs[0]];
			auto ip2 = gates[gate.inputs[1]];
			if (ip1.type != GateType::AND) {
				swappedGates.insert(gate.inputs[0]);
			}
			if (ip2.type != GateType::AND) {
				swappedGates.insert(gate.inputs[1]);
			}
		}
		if (gate.type == GateType::AND) {
			if (name == "x45" || name == "y45") {
				continue;
			}
			auto ip1 = gates[gate.inputs[0]];
			auto ip2 = gates[gate.inputs[1]];
			if (ip1.type == GateType::AND) {
				if (ip1.inputs[0] != "x00" and ip1.inputs[0] != "y00") {
					swappedGates.insert(gate.inputs[0]);
				}
			}
			if (ip2.type == GateType::AND) {
				if (ip2.inputs[0] == "x00" and ip2.inputs[0] != "y00") {
					swappedGates.insert(gate.inputs[0]);
				}
			}
		}
	}

	std::string result = "";
	bool first = true;
	for (auto &gate : swappedGates) {
		if (!first) {
			result += ",";
		}
		result += gate;
		first = false;
	}

	return result;
}
