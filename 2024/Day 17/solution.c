#include "../lib/cutils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int to_combo(int val, int reg_a, int reg_b, int reg_c) {
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

int intpow(int a, int b) {
	int res = 1;
	while (b > 0) {
		res *= a;
		b--;
	}
	return res;
}

int dv(int operand, int *reg_a, int *reg_b, int *reg_c) {
	int numerator = *reg_a;
	int denominator = intpow(2, to_combo(operand, *reg_a, *reg_b, *reg_c));
	return numerator / denominator;
}

void adv(int operand, int *reg_a, int *reg_b, int *reg_c) {
	*reg_a = dv(operand, reg_a, reg_b, reg_c);
}

void bxl(int operand, int *reg_b) { *reg_b = operand ^ *reg_b; }

void bst(int operand, int *reg_a, int *reg_b, int *reg_c) {
	*reg_b = to_combo(operand, *reg_a, *reg_b, *reg_c) % 8;
}

int jnz(int operand, int *reg_a) {
	if (*reg_a == 0) {
		return -1;
	}
	return operand;
}

void bxc(int *reg_b, int *reg_c) { *reg_b = *reg_b ^ *reg_c; }

void out(int operand, int *reg_a, int *reg_b, int *reg_c, char **output_buffer,
		 int *output_buffer_size) {
	int val = to_combo(operand, *reg_a, *reg_b, *reg_c) % 8;
	if (*output_buffer_size == 0) {
		*output_buffer = realloc(*output_buffer, *output_buffer_size + 1);
		(*output_buffer_size)++;
		(*output_buffer)[*output_buffer_size - 1] = val + '0';
	} else {
		*output_buffer = realloc(*output_buffer, *output_buffer_size + 2);
		(*output_buffer_size) += 2;
		(*output_buffer)[*output_buffer_size - 2] = ',';
		(*output_buffer)[*output_buffer_size - 1] = val + '0';
	}
}

void bdv(int operand, int *reg_a, int *reg_b, int *reg_c) {
	*reg_b = dv(operand, reg_a, reg_b, reg_c);
}

void cdv(int operand, int *reg_a, int *reg_b, int *reg_c) {
	*reg_c = dv(operand, reg_a, reg_b, reg_c);
}

char *interpret(int *ip, int instruction_count, int *reg_a, int *reg_b,
				int *reg_c, int *output_buffer_size) {
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
	int reg_a, reg_b, reg_c;
	sscanf(data, "Register A: %d\nRegister B: %d\nRegister C: %d\n", &reg_a,
		   &reg_b, &reg_c);

	char *instruction_start = strstr(data, "Program: ") + 9;

	ptrdiff_t instruction_len =
		fsize - (instruction_start - data) - 1; // -1 for \n
	ptrdiff_t instruction_count =
		(instruction_len - 1) / 2 + 1; // remove commas

	int instructions[instruction_count];
	size_t i = 0;
	while (*instruction_start != '\0') {
		instructions[i] = atoi(instruction_start);
		instruction_start += 2;
		i++;
	}

	int output_buffer_size;
	char *output_buffer = interpret(instructions, instruction_count, &reg_a,
									&reg_b, &reg_c, &output_buffer_size);

	for (int i = 0; i < output_buffer_size; i++) {
		printf("%c", output_buffer[i]);
	}

	llu sum = 0;

	free(data);
	return sum;
}

llu part2() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);
	llu sum = 0;

	free(data);
	return sum;
}
