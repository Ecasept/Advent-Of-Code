import functools


with open("input.txt") as f:
    data = f.read().split("\n")

width = len(data[0])
height = len(data)

@functools.cache
def next_plots(current_plot, depth):
    if current_plot[0] < 0 or current_plot[0] >= width or current_plot[1] < 0 or current_plot[1] >= height:
        return set()
    if data[current_plot[1]][current_plot[0]] == "#":
        return set()
    if depth == 0:
        return set([current_plot])


    plots = set()
    plots.update(next_plots((current_plot[0] + 1, current_plot[1]), depth - 1))
    plots.update(next_plots((current_plot[0] - 1, current_plot[1]), depth - 1))
    plots.update(next_plots((current_plot[0], current_plot[1] + 1), depth - 1))
    plots.update(next_plots((current_plot[0], current_plot[1] - 1), depth - 1))
    return plots

for x in range(width):
    for y in range(height):
        if data[y][x] == "S":
            start_plot = (x, y)
            break
res = next_plots(start_plot, 64)
print(len(res))
