from itertools import combinations
import os
with open(os.path.join(os.path.dirname(__file__),"input.txt")) as f:
    data: list[str] = f.read().split("\n")

inflated_lines = []
inflated_columns = []

INFLATION = 10**6

def rot_mat(mat):
    return list(zip(*mat[::-1]))

def apply_cosmic_inflation(data: list[str]):
    inflated = []
    for i, line in enumerate(data):
        if all(char=="." for char in line):
            inflated.append(i)    
    return inflated

def find_galaxies(inp: list[str]):
    galaxies: list[tuple[int, int]] = []
    for y, line in enumerate(inp):
        for x, symbol in enumerate(line):
            if symbol == "#":
                # print(f"{len(galaxies)+1}: ({x}, {y})")
                galaxies.append((x, y))
    return galaxies

def connection_length(comb):
    p1, p2 = comb
    extra = 0
    # print(p1, p2)
    for row in inflated_rows:
        if p1[1] < row < p2[1] or p2[1] < row < p1[1]:
            extra += INFLATION-1
    for column in inflated_columns:
        if p1[0] < column < p2[0] or p2[0] < column < p1[0]:
            extra += INFLATION-1
    # print(f"{p1[1]} - row - {p2[1]}")
    # print(f"{p1[0]} - col - {p2[0]}")
    # print()
    return abs(p2[0] - p1[0]) + abs(p2[1] - p1[1]) + extra

inflated_rows = apply_cosmic_inflation(data)
data = rot_mat(data)
inflated_columns = apply_cosmic_inflation(data)

print("row",inflated_rows)
print("col",inflated_columns)

data = rot_mat(rot_mat(rot_mat(data)))

print((len(inflated_rows) + len(inflated_columns)) * INFLATION)

galaxies = find_galaxies(data)
combs = list(combinations(galaxies, 2))
# combs.sort(key=connection_length)
print(sum(connection_length(c) for c in combs))

