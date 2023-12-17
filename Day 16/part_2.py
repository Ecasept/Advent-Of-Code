from calendar import c
from copy import deepcopy as dp
import functools
import os
import sys
import time

with open(os.path.join(os.path.dirname(__file__),"test.txt")) as f:
    data = f.read()

from enum import Enum
from typing import Final

class Direction(Enum):
    UP = 0
    RIGHT = 1
    DOWN = 2
    LEFT = 3

type Position = tuple[int, int]
type Beam = tuple[Position, Direction]

beams_called: list[Beam] = []

grid = data.split("\n")

width: Final[int] = len(grid[0])
height: Final[int] = len(grid)

# def deep_tuple(lst):
    # return tuple(deep_tuple(i) if isinstance(i, list) else i for i in lst)

# def deep_list(tpl):
    # return list(deep_list(i) if isinstance(i, tuple) else i for i in tpl)

cache = {}
beems_called = []

def next_beam(beam: Beam, first=False):
    # if beam in beems_called:
    #     return []
    # else:
    #     beems_called.append(beam)
    
    # if cache.get(beam, None) is not None: # in cache
    #     return cache[beam]
    # if cache.get(beam, 0) is None: # being traversed
    #     return []
    # cache[beam] = None

    position = beam[0]
    direction = beam[1]
    match beam[1]:
        case Direction.UP:
            position = (position[0], position[1]-1)
        case Direction.RIGHT:
            position = (position[0]+1, position[1])
        case Direction.DOWN:
            position = (position[0], position[1]+1)
        case Direction.LEFT:
            position = (position[0]-1, position[1])
    energized_points = {position}
    
    if (position[0] < 0 or position[0] >= width or position[1] < 0 or position[1] >= height) and not first:
        cache[beam] = set()
        return []
    # print grid with beam position colored red
    # for y, row in enumerate(grid):
    #     for x, char in enumerate(row):
    #         if [x, y] == beam[0]:
    #             print("\033[31m" + char + "\033[0m", end="")
    #         else:
    #             print(char, end="")
    #     print()
    # print()
    symbol = grid[position[1]][position[0]]
    match symbol:
        case ".":
            energized_points.update(next_beam((position, direction)))
        case "/":
            match direction:
                case Direction.UP:
                    direction = Direction.RIGHT
                case Direction.RIGHT:
                    direction = Direction.UP
                case Direction.DOWN:
                    direction = Direction.LEFT
                case Direction.LEFT:
                    direction = Direction.DOWN
            energized_points.update(next_beam((position, direction)))
        case "\\":
            match direction:
                case Direction.UP:
                    direction = Direction.LEFT
                case Direction.RIGHT:
                    direction = Direction.DOWN
                case Direction.DOWN:
                    direction = Direction.RIGHT
                case Direction.LEFT:
                    direction = Direction.UP
            energized_points.update(next_beam((position, direction)))
        case "-":
            if direction in [Direction.LEFT, Direction.RIGHT]:
                energized_points.update(next_beam((position, direction)))
            else:
                direction = Direction.LEFT
                energized_points.update(next_beam((position, direction)))
                direction = Direction.RIGHT
                energized_points.update(next_beam((position, direction)))
        case "|":
            if direction in [Direction.UP, Direction.DOWN]:
                energized_points.update(next_beam((position, direction)))
            else:
                direction = Direction.UP
                energized_points.update(next_beam((position, direction)))
                direction = Direction.DOWN
                energized_points.update(next_beam((position, direction)))
    # cache[beam] = energized_points
    return energized_points

sys.setrecursionlimit(1000000000)

max_energized = 0
energized = set()
for x in range(width):
    beams_called = []
    points = next_beam(((x, -1), Direction.DOWN), True)
    if len(points) > max_energized:
        max_energized = len(points)
        energized = points
    
    if x == 3:
        for y, row in enumerate(grid):
            for x, char in enumerate(row):
                if (x, y) in points:
                    # print("\033[31m" + char + "\033[0m", end="")
                    print("#", end="")
                else:
                    print(char, end="")
            print()
    
    beams_called = []
    points = next_beam(((x, height), Direction.UP), True)
    if len(points) > max_energized:
        max_energized = len(points)
        energized = points
for y in range(height):
    beams_called = []
    points = next_beam(((-1, y), Direction.RIGHT), True)
    if len(points) > max_energized:
        max_energized = len(points)
        energized = points
    beams_called = []
    points = next_beam(((width, y), Direction.LEFT), True)
    if len(points) > max_energized:
        max_energized = len(points)
        energized = points
print(max_energized)
# print grid with energized points colored red

