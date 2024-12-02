with open("input.txt") as file:
    data = file.read().split("\n")

cur_pos = (0, 0)

vertices = []
perimeter_distance = 0

for line in data:
    color = line.split(" ")[2]
    num = int(color[2:7], base=16)
    print(num)
    dir = color[7]
    old_pos = cur_pos
    if dir == "3":
        cur_pos = (cur_pos[0], cur_pos[1] - num)
    elif dir == "0":
        cur_pos = (cur_pos[0] + num, cur_pos[1])
    elif dir == "1":
        cur_pos = (cur_pos[0], cur_pos[1] + num)
    elif dir == "2":
        cur_pos = (cur_pos[0] - num, cur_pos[1])
    dist = abs(old_pos[0] - cur_pos[0]) + abs(old_pos[1] - cur_pos[1])
    perimeter_distance += dist
    print(dir)
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

# print(vertices)
print(shoelace(vertices))
print(perimeter_distance)
print(picks_theorem(vertices, perimeter_distance))
