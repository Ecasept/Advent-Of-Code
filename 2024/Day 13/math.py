def solvey(ax, ay, bx, by, cx, cy):
    numerator = cy - (ay * cx / ax)
    denominator = -(ay * bx / ax) + by
    return numerator / denominator


def solvex(cx, bx, ax, y):
    return (cx - bx*y)/ax


ax = 17
ay = 86
bx = 84
by = 37
cx = 7870
cy = 6450

y = solvey(ax, ay, bx, by, cx, cy)
x = solvex(cx, bx, ax, y)
print(y)
print(x)


"""
??? how does this work ???
this is the file where I tested the algorithm. solution.c implements it in c

ax and ay are how much button A moves the claw machine
same for bx, by and button B
cx and cy are the position of the claw machine

x and y are the required pushes for a and b respectively

if you push button A x times, and button B y times, the claw machine's:
- x coordinate will be:
    ax * x + bx * y
- y coordinate will be:
    ay * x + by * y

if we set them equal to cx and cy respectively, we get two linear equations:
ax * x + bx * y = cx
ay * x + by * y = cy

if you solve for x and y, you get the above formulas

if no integer solution exists, you cannot press A and B an integer amount of times to get to the claw machine's position -> no price

"""
