from copy import deepcopy as dp
import os

with open(os.path.join(os.path.dirname(__file__),"input.txt")) as f:
    data = f.read()

def rot_mat(mat):
    return ["".join(tpl) for tpl in list(zip(*mat[::-1]))]

def tilt_platform_up(platform):
    def assign(mat, x, y, val):
        mat[y] = mat[y][:x] + val + mat[y][x+1:]
    
    plt = rot_mat(rot_mat(rot_mat(platform)))
    new_plt = dp(plt)
    # print(platform, plt, new_plt)
    for y ,row in enumerate(plt):
        for j ,char in enumerate(row):
            if char == "O":
                replacement_pos = -1
                for pos in range(j-1, -1, -1):
                    if new_plt[y][pos] != ".":
                        replacement_pos = pos+1 # previous pos
                        break
                else:
                    replacement_pos = 0
                if replacement_pos != -1:
                    assign(new_plt, j, y, ".")
                    assign(new_plt, replacement_pos, y, "O")
    return rot_mat(new_plt)

def calculate_load(platform):
    load = 0
    for i, row in enumerate(platform):
        for char in row:
            if char == "O":
                load += len(platform) - i
    return load

platform = tilt_platform_up(data.split("\n"))
load = calculate_load(platform)
print(load)
