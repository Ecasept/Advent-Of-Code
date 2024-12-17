"""
Thanks to
https://www.reddit.com/r/adventofcode/comments/1hg38ah/2024_day_17_solutions/m2gizvj/
for this code so that I could go step by step through the debugger on both my and this program and understand where my program went wrong
"""

instructions = list(
    map(int, open("Day 17/input.txt").read().split()[-1].split(',')))


def solve(depth, next_a):
    if depth < 0:
        print(next_a)
        return True
    for i in range(8):
        a, ip = next_a << 3 | i, 0
        while ip < len(instructions):
            # b, c, out = 0, 0, 0
            # print(f"Registers: {a}, {b}, {c}, {ip}")
            if instructions[ip + 1] <= 3:
                o = instructions[ip + 1]
            elif instructions[ip + 1] == 4:
                o = a
            elif instructions[ip + 1] == 5:
                o = b
            elif instructions[ip + 1] == 6:
                o = c
            if instructions[ip] == 0:
                a >>= o
            elif instructions[ip] == 1:
                b ^= instructions[ip + 1]
            elif instructions[ip] == 2:
                b = o & 7
            elif instructions[ip] == 3:
                ip = instructions[ip + 1] - 2 if a != 0 else ip
            elif instructions[ip] == 4:
                b ^= c
            elif instructions[ip] == 5:
                out = o & 7
                break
            elif instructions[ip] == 6:
                b = a >> o
            elif instructions[ip] == 7:
                c = a >> o
            ip += 2

        if out == instructions[depth]:
            print(next_a << 3 | i, i)
            if solve(depth - 1, next_a << 3 | i):
                return True
            print("backtrack")
    return False


solve(len(instructions) - 1, 0)
