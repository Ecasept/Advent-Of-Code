from itertools import combinations

with open("input.txt") as f:
    data: list[str] = f.read().split("\n")

def rot_mat(mat):
    return list(zip(*mat[::-1]))

def apply_cosmic_inflation(data: list[str]):
    dt_copy = []
    for line in data:
        if all(char=="." for char in line):
            dt_copy.extend([line]*2)
        else:
            dt_copy.append(line)
    return dt_copy

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
    return abs(p2[0] - p1[0]) + abs(p2[1] - p1[1])

data = rot_mat(rot_mat(rot_mat(apply_cosmic_inflation(rot_mat(apply_cosmic_inflation(data))))))
galaxies = find_galaxies(data)
combs = list(combinations(galaxies, 2))
# combs.sort(key=connection_length)
print(sum(connection_length(c) for c in combs))
