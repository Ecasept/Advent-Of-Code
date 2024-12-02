from copy import deepcopy as dp
import functools
from itertools import cycle
import os
from typing import OrderedDict

with open(os.path.join(os.path.dirname(__file__),"input.txt")) as f:
    data = f.read()

def rot_mat(mat):
    return ["".join(tpl) for tpl in list(zip(*mat[::-1]))]

def rot_mat_counter(mat):
    return ["".join(tpl) for tpl in list(zip(*mat))[::-1]]


def tilt_platform(platform):
    def assign(mat, x, y, val):
        mat[y] = mat[y][:x] + val + mat[y][x+1:]
    new_plt = list(platform)
    # print(platform, plt, new_plt)
    for y ,row in enumerate(platform):
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
    return new_plt

def calculate_load(platform):
    load = 0
    for i, row in enumerate(platform):
        for char in row:
            if char == "O":
                load += len(platform) - i
    return load

def tilt_north(platform):
    platform = rot_mat_counter(platform)
    platform = tilt_platform(tuple(platform))
    platform = rot_mat(platform)
    return platform

def tilt_east(platform):
    platform = rot_mat_counter(platform)
    platform = rot_mat_counter(platform)
    platform = tilt_platform(tuple(platform))
    platform = rot_mat(platform)
    platform = rot_mat(platform)
    return platform

def tilt_south(platform):
    platform = rot_mat(platform)
    platform = tilt_platform(tuple(platform))
    platform = rot_mat_counter(platform)
    return platform

def tilt_west(platform):
    platform = tilt_platform(tuple(platform))
    return platform

def full_cycle(platform):
    return tilt_east(tilt_south(tilt_west(tilt_north(platform))))

cache = {}
cycle_num = 0
orig_platform = data.split("\n")
platform = dp(orig_platform)
cycle_start_length = 0
target = 10**9
i=0
skipped = False
while i<target:
    platform = full_cycle(platform)
    
    if tuple(platform) in cache and not skipped:
        # print("found", i)
        cycle_start = cache[tuple(platform)]
        cycle_size = i - cycle_start
        skip_amount = (target - i) // cycle_size
        i += skip_amount * cycle_size
        
        skipped = True
        # print(i)
    else:
        cache[tuple(platform)] = i
    i+=1

load = calculate_load(platform)
print(load)
