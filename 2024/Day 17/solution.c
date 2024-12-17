#include "../lib/cutils.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

llu to_combo(int val, llu reg_a, llu reg_b, llu reg_c) {
	if (val <= 3) {
		return val;
	}
	switch (val) {
	case 4:
		return reg_a;
	case 5:
		return reg_b;
	case 6:
		return reg_c;
	}
	printf("Invalid combo value: %d\n", val);
	exit(1);
}

llu intpow(llu a, llu b) {
	llu res = 1;
	while (b > 0) {
		res *= a;
		b--;
	}
	return res;
}

llu dv(int operand, llu *reg_a, llu *reg_b, llu *reg_c) {
	llu numerator = *reg_a;
	llu denominator = intpow(2, to_combo(operand, *reg_a, *reg_b, *reg_c));
	return numerator / denominator;
}

void adv(int operand, llu *reg_a, llu *reg_b, llu *reg_c) {
	*reg_a = dv(operand, reg_a, reg_b, reg_c);
}

void bxl(int operand, llu *reg_b) { *reg_b = operand ^ *reg_b; }

void bst(int operand, llu *reg_a, llu *reg_b, llu *reg_c) {
	*reg_b = to_combo(operand, *reg_a, *reg_b, *reg_c) % 8;
}

int jnz(int operand, llu *reg_a) {
	if (*reg_a == 0) {
		return -1;
	}
	return operand;
}

void bxc(llu *reg_b, llu *reg_c) { *reg_b = *reg_b ^ *reg_c; }

void out(int operand, llu *reg_a, llu *reg_b, llu *reg_c, char **output_buffer,
		 int *output_buffer_size) {
	int val = to_combo(operand, *reg_a, *reg_b, *reg_c) % 8;
	*output_buffer = realloc(*output_buffer, *output_buffer_size + 1);
	(*output_buffer_size)++;
	(*output_buffer)[*output_buffer_size - 1] = val + '0';
}

void bdv(int operand, llu *reg_a, llu *reg_b, llu *reg_c) {
	*reg_b = dv(operand, reg_a, reg_b, reg_c);
}

void cdv(int operand, llu *reg_a, llu *reg_b, llu *reg_c) {
	*reg_c = dv(operand, reg_a, reg_b, reg_c);
}

char easyout(int operand, llu *reg_a, llu *reg_b, llu *reg_c) {
	int val = to_combo(operand, *reg_a, *reg_b, *reg_c) % 8;
	return val + '0';
}

char *interpret(int *ip, int instruction_count, llu *reg_a, llu *reg_b,
				llu *reg_c, int *output_buffer_size) {
	int *istart = ip;
	int *iend = ip + instruction_count;

	char *output_buffer = NULL;
	*output_buffer_size = 0;

	while (ip < iend) {
		int op_code = *ip;
		int operand = *(ip + 1);
		bool jnz_jumped = false;
		switch (op_code) {
		case 0:
			adv(operand, reg_a, reg_b, reg_c);
			break;
		case 1:
			bxl(operand, reg_b);
			break;
		case 2:
			bst(operand, reg_a, reg_b, reg_c);
			break;
		case 3:; // idk why c is so weird
			int new_ip_index = jnz(operand, reg_a);
			if (new_ip_index != -1) {
				ip = istart + new_ip_index;
				jnz_jumped = true;
			}
			break;
		case 4:
			bxc(reg_b, reg_c);
			break;
		case 5:
			out(operand, reg_a, reg_b, reg_c, &output_buffer,
				output_buffer_size);
			break;
		case 6:
			bdv(operand, reg_a, reg_b, reg_c);
			break;
		case 7:
			cdv(operand, reg_a, reg_b, reg_c);
			break;
		default:
			printf("Invalid op code %d\n", op_code);
			exit(1);
		}
		if (!jnz_jumped) {
			ip += 2;
		}
	}
	return output_buffer;
}

llu part1() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);
	llu reg_a, reg_b, reg_c;
	sscanf(data, "Register A: %llu\nRegister B: %llu\nRegister C: %llu\n",
		   &reg_a, &reg_b, &reg_c);

	char *instruction_start = strstr(data, "Program: ") + 9;

	ptrdiff_t instruction_len =
		fsize - (instruction_start - data) - 1; // -1 for \n
	ptrdiff_t instruction_count =
		(instruction_len - 1) / 2 + 1; // remove commas

	int instructions[instruction_count];
	size_t i = 0;
	while (true) {
		instructions[i] = *instruction_start - '0';
		if (*(instruction_start + 1) == '\0') {
			break;
		}
		instruction_start += 2;
		i++;
	}

	int output_buffer_size;
	char *output_buffer = interpret(instructions, instruction_count, &reg_a,
									&reg_b, &reg_c, &output_buffer_size);

	output_buffer = realloc(output_buffer, output_buffer_size + 1);
	output_buffer[output_buffer_size] = '\0';
	free(data);

	llu result = atoll(output_buffer);
	free(output_buffer);

	return result;
}

char output_for_a(llu a, char *ip, int instruction_count) {
	char *iend = ip + instruction_count;

	llu reg_a = a;
	llu reg_b = 0;
	llu reg_c = 0;

	while (ip < iend) {
		// printf("Registers: %llu %llu %llu\n", reg_a, reg_b, reg_c);
		int op_code = *ip - '0';
		int operand = *(ip + 1) - '0';
		switch (op_code) {
		case 0:
			adv(operand, &reg_a, &reg_b, &reg_c);
			break;
		case 1:
			bxl(operand, &reg_b);
			break;
		case 2:
			bst(operand, &reg_a, &reg_b, &reg_c);
			break;
		case 3:
			printf("Should not jump\n");
			exit(1);
		case 4:
			bxc(&reg_b, &reg_c);
			break;
		case 5:
			return easyout(operand, &reg_a, &reg_b, &reg_c);
			break;
		case 6:
			bdv(operand, &reg_a, &reg_b, &reg_c);
			break;
		case 7:
			cdv(operand, &reg_a, &reg_b, &reg_c);
			break;
		default:
			printf("Invalid op code %d\n", op_code);
			exit(1);
		}
		ip += 2;
	}
	printf("Error no output\n");
	exit(1);
}

llu dfs(char *instructions, int instruction_count, llu a, int depth) {
	if (depth == -1) {
		return a;
	}
	char instruction = instructions[depth];
	for (int i = 0; i < 8; i++) {
		llu next_a = (a << 3) | i;
		char out = output_for_a(next_a, instructions, instruction_count);
		if (out == instruction) {
			// printf("a: %llu, i: %d, out: %c, instruction: %c\n", next_a, i,
			// out, 	   instruction);
			llu res = dfs(instructions, instruction_count, next_a, depth - 1);
			if (res != ULLONG_MAX) {
				return res;
			} else {
				// printf("Backtrack\n");
			}
		}
	}
	return ULLONG_MAX;
}

llu part2() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);

	char *instruction_start = strstr(data, "Program: ") + 9;

	ptrdiff_t instruction_len =
		fsize - (instruction_start - data) - 1; // -1 for \n
	ptrdiff_t instruction_count =
		(instruction_len - 1) / 2 + 1 + 1; // remove commas

	char instructions[instruction_count];
	size_t i = 0;
	while (true) {
		instructions[i] = *instruction_start;
		if (*(instruction_start + 1) == '\0') {
			break;
		}
		instruction_start += 2;
		i++;
	}

	llu res = dfs(instructions, instruction_count, 0, instruction_count - 1);
	// printf("Value of a: %llu\n", res);

	free(data);

	return res;
}
