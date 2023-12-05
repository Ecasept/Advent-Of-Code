with open("input.txt") as f:
    data = f.readlines()

def extract(throws: str):
    min_red = 0
    min_green = 0
    min_blue = 0
    for throw in throws.split(";"):
        throw = throw.strip()
        for balls in throw.split(","):
            balls = balls.strip()
            count, color = balls.split(" ")
            count = int(count)
            if color == "red" and count > min_red:
                min_red = count
            if color == "green" and count > min_green:
                min_green = count
            if color == "blue" and count > min_blue:
                min_blue = count
    return min_green * min_blue * min_red

answer = 0
for line in data:
    line = line.strip()
    
    first_colon = line.index(":")
    throws = line[first_colon+2:] # colon and after
    game_info = line[:first_colon] # everything before the colon

    game_id = int(game_info[5:]) # remove "Game "

    power = extract(throws)
    answer += power
print(answer)
