import os

from copy import deepcopy as dp

with open(os.path.join(os.path.dirname(__file__),"input.txt")) as f:
    data = f.read().split("\n\n")

def col(mat, x):
    return [mat[y][x] for y in len(mat)]

def rot_mat(mat):
    return list(zip(*mat[::-1]))

def find_ref(pattern, disallow=None, depth=0, ref_pos = -1,):
    if ref_pos == -1:
        # Start Case
        for row_index in range(len(pattern)-1):
            if find_ref(pattern, depth=0, ref_pos=row_index) != -1:
                if row_index != disallow:
                    return row_index
    else:
        # ref_pos is always the index of the first element of the two center reflexion elements
        first = ref_pos - depth
        second = ref_pos + depth + 1
        row = pattern[ref_pos]
        if first < 0 or second >= len(pattern):
            # Default Case
            return ref_pos
        if pattern[first] == pattern[second]:
            return find_ref(pattern, depth=depth + 1, ref_pos=ref_pos)
        return -1
        
answer = 0
import sys
for i,pattern in enumerate(data):
    pattern = pattern.split("\n")
    height = len(pattern)
    width = len(pattern[0])
    org_row = find_ref(pattern)
    org_col = find_ref(rot_mat(pattern))
    additional = 0
    for x in range(width):
        for y in range(height):
            smudge_pos = (x, y)
            pt = dp(pattern)
            if pt[y][x] == ".":
                pt[y] = pt[y][:x] + "#" + pt[y][x+1:]
            else:
                pt[y] = pt[y][:x] + "." + pt[y][x+1:]
            new_row = find_ref(pt, disallow=org_row)
            new_col = find_ref(rot_mat(pt), disallow=org_col)
            if new_row is not None:
                additional = (new_row+1) * 100
                break
            elif new_col is not None:
                additional = (new_col+1)
                break
        if additional != 0:
            break
    print(i, additional, "row:", org_row, "col:", org_col)
    answer += additional
print(answer)

