#include "../utils.h"
#include <vector>

bool isNumber(char chr) { return chr >= '0' and chr <= '9'; }

bool isAlpha(char chr) { return chr >= 'a' and chr <= 'z'; }

bool endsWith(std::string str, std::string ending) {
    if (str.size() < ending.size()) {
        return false;
    }
    for (int i = 0; i < ending.size(); i++) {
        if (str[str.size() - ending.size() + i] != ending[i]) {
            return false;
        }
    }
    return true;
}

void part1() {
    auto file = utils::getInput(1);
    char chr;
    std::string curToken = "";
    std::vector<std::string> tokens;
    // tokens: "<alpha>", "(", ")", "<number>", ","

    while (file) {
        chr = file.get();
        if (curToken.empty()) {
            curToken.append(1, chr);
        } else if (isAlpha(curToken[0]) and isAlpha(chr)) {
            curToken.append(1, chr);
        } else if (isNumber(curToken[0]) and isNumber(chr)) {
            curToken.append(1, chr);
        } else {
            tokens.push_back(curToken);
            curToken = chr;
        }
    }
    int op = -1; // 0 = mul
    std::vector<std::string> params;
    bool parsingParams = false;
    bool nextIsParam = false;

    auto resetState = [&]() {
        op = -1;
        params.clear();
        parsingParams = false;
        nextIsParam = false;
    };

    int sum = 0;

    for (auto token : tokens) {
        if (endsWith(token, "mul")) {
            if (parsingParams or nextIsParam) {
                // can't have mul in the middle of a param list
                resetState();
            }
            op = 0;
        } else if (token == "(") {
            if (parsingParams or nextIsParam) {
                // can't have ( in the middle of a param list
                resetState();
            }
            parsingParams = true;
            nextIsParam = true;
        } else if (token == ")") {
            if (not parsingParams or nextIsParam) {
                // can't have ) without ( or with a dangling param
                resetState();
                continue;
            }

            if (op == 0) {
                if (params.size() != 2) {
                    // can only have 2 params for multiplication
                    resetState();
                    continue;
                }
                int mul = 1;
                for (auto param : params) {
                    mul *= std::stoi(param);
                }
                sum += mul;
            }

            resetState();
        } else if (parsingParams) {
            if (nextIsParam and isNumber(token[0])) {
                params.push_back(token);
                nextIsParam = false;
            } else if (token == "," and not nextIsParam) {
                nextIsParam = true;
            } else {
                resetState();
            }
        } else {
            resetState();
        }
    }

    std::cout << sum << '\n';
}

bool isAlpha2(char chr) { return (chr >= 'a' and chr <= 'z') or chr == '\''; }

void part2() {
    auto file = utils::getInput(1);
    char chr;
    std::string curToken = "";
    std::vector<std::string> tokens;
    // tokens: "<alpha>", "(", ")", "<number>", ","

    while (file) {
        chr = file.get();
        if (curToken.empty()) {
            curToken.append(1, chr);
        } else if (isAlpha2(curToken[0]) and isAlpha2(chr)) {
            curToken.append(1, chr);
        } else if (isNumber(curToken[0]) and isNumber(chr)) {
            curToken.append(1, chr);
        } else {
            tokens.push_back(curToken);
            curToken = chr;
        }
    }
    int op = -1; // 0 = mul
    std::vector<std::string> params;
    bool parsingParams = false;
    bool nextIsParam = false;
    bool wasComma = false;

    bool mulEnabled = true;

    auto resetState = [&]() {
        op = -1;
        params.clear();
        parsingParams = false;
        nextIsParam = false;
        wasComma = false;
    };

    int sum = 0;

    for (auto token : tokens) {
        if (endsWith(token, "mul")) {
            resetState();
            op = 0;
        } else if (endsWith(token, "do")) {
            resetState();
            op = 1;
        } else if (endsWith(token, "don't")) {
            resetState();
            op = 2;
        } else if (token == "(") {
            if (parsingParams or nextIsParam) {
                // can't have ( in the middle of a param list
                resetState();
            }
            parsingParams = true;
            nextIsParam = true;
            wasComma = false;
        } else if (token == ")") {
            if (not parsingParams or (nextIsParam and wasComma)) {
                // can't have ) without ( or with a dangling param
                resetState();
                continue;
            }

            if (op == 1) {
                if (params.size() != 0) {
                    // can't have params for do
                    resetState();
                    continue;
                }
                mulEnabled = true;
            } else if (op == 2) {
                if (params.size() != 0) {
                    // can't have params for don't
                    resetState();
                    continue;
                }
                mulEnabled = false;
            } else if (op == 0 and mulEnabled) {
                if (params.size() != 2) {
                    // can only have 2 params for multiplication
                    resetState();
                    continue;
                }
                int mul = 1;
                for (auto param : params) {
                    mul *= std::stoi(param);
                }
                sum += mul;
            }

            resetState();
        } else if (parsingParams) {
            if (nextIsParam and isNumber(token[0])) {
                params.push_back(token);
                nextIsParam = false;
            } else if (token == "," and not nextIsParam) {
                nextIsParam = true;
                wasComma = true;
            } else {
                resetState();
            }
        } else {
            resetState();
        }
    }

    std::cout << sum << '\n';
}

int main() {
    part1();
    part2();
    return 0;
}
