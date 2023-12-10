import os
with open(os.path.join(os.path.dirname(__file__),"test_3.txt")) as f:
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
        self.dirs_by_symbol = {
            "|": [UP, DOWN],
            "-": [RIGHT, LEFT],
            "L": [UP, RIGHT],
            "J": [UP, LEFT],
            "7": [DOWN, LEFT],
            "F": [DOWN, RIGHT],
            # ".": []
        }
    
    def get_loop_tiles(self):
        def is_possible(s, dir):
            p = self.get_pos(s, dir)
            symbol = self.data[p.y][p.x]
            if symbol == ".":
                return []
            dirs = self.dirs_by_symbol[symbol]
            if self.invert(dir) in dirs:
                return [dir]
            return []

        for y, line in enumerate(self.data):
            for x, chr in enumerate(line):
                if chr == "S":
                    start = Pos(x,y)
                    possible_dirs = []
                    possible_dirs.extend(is_possible(start, UP))
                    possible_dirs.extend(is_possible(start, RIGHT))
                    possible_dirs.extend(is_possible(start, DOWN))
                    possible_dirs.extend(is_possible(start, LEFT))
                    for pdir in possible_dirs:
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
        possible_dirs = self.dirs_by_symbol[symbol].copy()
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
print(len(loop_tiles)/2)

def print_maze(maze, loop_tiles):
    for y in range(len(maze.data)):
        line = maze.data[y]
        for x in range(len(line)):
            symbol = line[x]
            if [p for p in loop_tiles if p.x == x and p.y == y]:
                print("\033[42m" + symbol + "\033[0m", end="")
            else:
                print(symbol, end="")
        print()

print_maze(maze, loop_tiles)
