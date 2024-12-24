#include "../lib/utils.h"
#include <map>

enum class GateType { AND, OR, XOR, INIT };

class Gate {
  public:
	GateType type;
	std::vector<std::string> inputs;
	int initValue = -1;
	int output = -1;

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
	if (gate.output != -1) {
		return gate.output;
	}
	if (gate.type == GateType::INIT) {
		return gate.initValue;
	}
	int val1 = getGateOutput(gates, gate.inputs[0]);
	int val2 = getGateOutput(gates, gate.inputs[1]);
	int val;
	switch (gate.type) {
	case GateType::AND:
		val = val1 & val2;
		break;
	case GateType::OR:
		val = val1 | val2;
		break;
	case GateType::XOR:
		val = val1 ^ val2;
		break;
	default:
		std::cerr << "Invalid gate type: " << (int)gate.type << std::endl;
		exit(1);
	}
	gate.output = val;
	return val;
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

llu part1() {
	auto file = utils::getInput();
	std::string line;

	std::map<std::string, Gate> gates;

	while (std::getline(file, line)) {
		if (line == "") {
			// go to second part
			break;
		}

		std::string name = line.substr(0, line.find(":"));
		int value = std::stoi(line.substr(line.find(": ") + 2));

		gates[name] = {GateType::INIT, {}, value};
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

llu part2() { return 0; }
