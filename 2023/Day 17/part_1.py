import bisect
from dataclasses import dataclass
import functools
import os
from queue import Queue
import sys


with open(os.path.join(os.path.dirname(__file__),"input.txt")) as f:
    data = f.read()

city_map = data.split("\n")

import enum

class Dir(enum.Enum):
    N = 0
    E = 1
    S = 2
    W = 3

type Node = tuple(int, int)

def get_heat_loss(node: Node):
    return int(city_map[node[1]][node[0]])

def node_in_direction(node, direction):
    x, y = node
    if direction == Dir.N:
        return (x, y-1)
    elif direction == Dir.E:
        return (x+1, y)
    elif direction == Dir.S:
        return (x, y+1)
    elif direction == Dir.W:
        return (x-1, y)

def out_of_bounds(node: Node):
    x, y = node
    if x < 0 or y < 0:
        return True
    if x >= len(city_map[0]) or y >= len(city_map):
        return True
    return False

def next_nodes(current_node: Node, last_direction: Dir, direction_count: int):
    # x, y = current_node
    nodes = []
    for dir in [Dir.N, Dir.E, Dir.S, Dir.W]:
        if dir != last_direction:
            conf = Configuration(node=node_in_direction(current_node, dir), current_direction=dir, direction_count=1)
            if not out_of_bounds(conf.node):
                nodes.append(conf)
        else:
            if direction_count < 3:
                conf = Configuration(node=node_in_direction(current_node, dir), current_direction=dir, direction_count=direction_count+1)
                if not out_of_bounds(conf.node):
                    nodes.append(conf)
    return nodes

@dataclass(frozen=True)
class Configuration:
    node: Node
    current_direction: Dir = Dir.E
    direction_count: int = 0

visited = []

start_node = (0, 0)
start_conf = Configuration(node=start_node)

distances = {start_conf: 0}
prev_nodes = {}



unvisited: list[Configuration] = [start_conf]

while len(unvisited) > 0:
    current = unvisited.pop(0)
    neighbors = next_nodes(current.node, current.current_direction, current.direction_count)
    
    visited.append(current)
    if len(visited) % 1000 == 0:
        for i in range(0, len(city_map)):
            for j in range(0, len(city_map[0])):
                if (j, i) in visited:
                    print("X", end="")
                else:
                    print(city_map[i][j], end="")
            print()

    for nb in neighbors:
        prev_nodes[nb] = current
        other_distance = distances[prev_nodes[nb]] + get_heat_loss(nb.node)
        if other_distance < distances.get(nb, sys.maxsize):
            distances[nb] = other_distance
        if nb not in visited and nb not in unvisited:
            bisect.insort(unvisited, nb, key=lambda x: distances.get(x, sys.maxsize))

for conf in distances:
    if conf.node == (len(city_map[0])-1, len(city_map)-1):
        print(conf, distances[conf])

"""

start_node = (0, 0)
end_node = (len(city_map[0])-1, len(city_map)-1)

paths: list[Path] = [Path(nodes=(start_node,), heat_loss=0)]


winner_path = None
winner_heat_loss = None

while True:
    new_paths: list[Path] = []
    print(len(paths))
    print(max([len(path.nodes) for path in paths]))
    for path in paths:
        end_node = path.nodes[-1]
        heat_loss = path.heat_loss
        nodes = next_nodes(end_node, path.current_direction, path.direction_count)
        for node, cur_dir, dir_count in nodes:
            node_heat_loss = int(city_map[node[1]][node[0]])
            total_heat_loss = heat_loss + node_heat_loss

            new_path = Path(nodes=path.nodes + (node,), heat_loss = total_heat_loss, current_direction=cur_dir, direction_count=dir_count)
            
            if node == end_node:
                if winner_path:
                    if winner_heat_loss > total_heat_loss:
                        winner_path = new_path
                        winner_heat_loss = total_heat_loss
                else:
                    winner_path = new_path
                    winner_heat_loss = total_heat_loss
            else:
                new_paths.append(new_path)

    paths = new_paths
    if paths == []:
        break

print(winner_heat_loss)

"""
