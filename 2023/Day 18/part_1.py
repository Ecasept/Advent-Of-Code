with open("input.txt") as file:
    data = file.read().split("\n")

cur_pos = (0, 0)

vertices = []
perimeter_distance = 0

for line in data:
    dir = line.split(" ")[0]
    num = int(line.split(" ")[1])
    old_pos = cur_pos
    if dir == "U":
        cur_pos = (cur_pos[0], cur_pos[1] - num)
    elif dir == "R":
        cur_pos = (cur_pos[0] + num, cur_pos[1])
    elif dir == "D":
        cur_pos = (cur_pos[0], cur_pos[1] + num)
    elif dir == "L":
        cur_pos = (cur_pos[0] - num, cur_pos[1])
    dist = abs(old_pos[0] - cur_pos[0]) + abs(old_pos[1] - cur_pos[1])
    perimeter_distance += dist
    vertices.append((cur_pos, dir))


width = max(vertices, key=lambda x: x[0][0])[0][0] + 1
heigth = max(vertices, key=lambda x: x[0][1])[0][1] + 1

def shoelace(verts):
    sum1 = 0
    sum2 = 0
    for i in range(0, len(verts)-1):
        sum1 += verts[i][0][0] * verts[i+1][0][1]
        sum2 += verts[i][0][1] * verts[i+1][0][0]
    sum1 += verts[-1][0][0] * verts[0][0][1]
    sum2 += verts[-1][0][1] * verts[0][0][0]
    return abs(sum1 - sum2) / 2

def picks_theorem(verts, dist):
    return shoelace(verts) + dist/2 +1

print(vertices)
print(shoelace(vertices))
print(perimeter_distance)
print(picks_theorem(vertices, perimeter_distance))
