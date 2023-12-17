import os


with open(os.path.join(os.path.dirname(__file__),"input.txt")) as f:
    data = f.read().split("\n\n")

def col(mat, x):
    return [mat[y][x] for y in len(mat)]

def rot_mat(mat):
    return list(zip(*mat[::-1]))

def find_ref(pattern, depth=0, ref_pos = -1):
    if ref_pos == -1:
        # Start Case
        for row_index in range(len(pattern)-1):
            if find_ref(pattern, 0, row_index) != -1:
                return row_index
    else:
        # ref_pos is always the index of the first element of the two center reflexion elements
        first = ref_pos - depth
        second = ref_pos + depth + 1
        row = pattern[ref_pos]
        if first < 0 or second >= len(pattern):
            # Default Case
            print(pattern[ref_pos])
            return ref_pos
        if pattern[first] == pattern[second]:
            return find_ref(pattern, depth + 1, ref_pos)
        return -1
        
print(data)
answer = 0

for i,pattern in enumerate(data):
    pattern = pattern.split("\n")
    ref = find_ref(pattern)
    for p in pattern:print(p)
    print(i)
    if ref is None:
        ref = find_ref(rot_mat(pattern)) + 1
    else:
        ref = (ref+1)*100
    answer += ref
print(answer)

