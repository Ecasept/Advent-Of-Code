import os
with open(os.path.join(os.path.dirname(__file__),"input.txt")) as f:
    data = f.readlines()

UP = 0
RIGHT = 1
DOWN = 2
LEFT = 3

class Pos:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    def __repr__(self):
        return f"({self.x}|{self.y})"

class Maze:
    def __init__(self, data):
        self.data = data
    def dirs_by_symbol(self, symbol):
        mp = {
            "|": [UP, DOWN],
            "-": [RIGHT, LEFT],
            "L": [UP, RIGHT],
            "J": [UP, LEFT],
            "7": [DOWN, LEFT],
            "F": [DOWN, RIGHT],
            # ".": []
        }
        if symbol in mp:
            return mp[symbol]
        elif symbol == "S":
            return self.start_dirs

    def get_loop_tiles(self):
        def is_possible(s, dir):
            p = self.get_pos(s, dir)
            symbol = self.data[p.y][p.x]
            if symbol == ".":
                return []
            dirs = self.dirs_by_symbol(symbol)
            if self.invert(dir) in dirs:
                return [dir]
            return []

        for y, line in enumerate(self.data):
            for x, chr in enumerate(line):
                if chr == "S":
                    start = Pos(x,y)
                    self.start_dirs = []
                    self.start_dirs.extend(is_possible(start, UP))
                    self.start_dirs.extend(is_possible(start, RIGHT))
                    self.start_dirs.extend(is_possible(start, DOWN))
                    self.start_dirs.extend(is_possible(start, LEFT))
                    for pdir in self.start_dirs:
                        success, tiles = self.next_pos(start, pdir)
                        if success:
                            return tiles


    
    def get_pos(self, pos: Pos, dir: int):
        match dir:
            case 0: # up
                return Pos(pos.x, pos.y-1)
            case 1: # right
                return Pos(pos.x + 1, pos.y)
            case 2: # down
                return Pos(pos.x, pos.y+1)
            case 3: # left
                return Pos(pos.x-1, pos.y)
    def get_possible_dirs(self, pos, source_dir):
        symbol = self.data[pos.y][pos.x]
        if symbol == ".":
            return []
        possible_dirs = self.dirs_by_symbol(symbol).copy()
        possible_dirs.remove(source_dir)
        return possible_dirs
    
    def invert(self, dir):
        match dir:
            case 0:
                return 2
            case 1:
                return 3
            case 2:
                return 0
            case 3:
                return 1

    def next_pos(self, last_pos: Pos, dir: int) -> tuple[bool, list[Pos]]:
        pos = self.get_pos(last_pos, dir)
        if data[pos.y][pos.x] == "S":
            return True, [pos]
        possible_dirs = self.get_possible_dirs(pos, self.invert(dir))
        for pdir in possible_dirs:
            success, tiles = self.next_pos(pos, pdir)
            if success:
                tiles.insert(0, pos)
                return True, tiles
        return False, None

maze = Maze([line.strip() for line in data])

import sys
chr_count = sum([len(line) for line in maze.data])
sys.setrecursionlimit(chr_count*2)

loop_tiles = maze.get_loop_tiles()

def print_maze(maze, loop_tiles, insides=None):
    for y in range(len(maze.data)):
        line = maze.data[y]
        for x in range(len(line)):
            symbol = line[x]
            if [p for p in loop_tiles if p.x == x and p.y == y]:
                print("\033[42m" + symbol + "\033[0m", end="")
            elif insides and [p for p in insides if p.x == x and p.y == y]:
                print("\033[41m" + symbol + "\033[0m", end="")
            else:
                print(symbol, end="")
        print()

insides = []
for y in range(len(maze.data)):
    line = maze.data[y]
    inversions = 0
    sidewards = False
    last_sw_symbols = []
    for x in range(len(line)):
        symbol = line[x]
        if [p for p in loop_tiles if p.x == x and p.y == y]:
            # in mainloop
            if symbol in ["F", "7", "L", "J", "S"]:
                last_sw_symbols.append(symbol)
                if len(last_sw_symbols) == 2:
                    s0 = maze.dirs_by_symbol(last_sw_symbols[0])
                    s1 = maze.dirs_by_symbol(last_sw_symbols[1])
                    last_sw_symbols = []
                    if not ((UP in s0 and UP in s1) or (DOWN in s0 and DOWN in s1)):
                        inversions += 1
            elif symbol == "-":
                pass
            elif symbol == "|":
                inversions += 1
        else:
            if inversions%2==1:
                insides.append(Pos(x, y))

print(len(insides))
        
# print_maze(maze, loop_tiles, insides)
