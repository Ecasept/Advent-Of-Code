import os
import re
import subprocess as sp

# Change the directory to the root of the project
os.chdir(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def compile_all():
    os.system("make all")
    print()


def benchmark(day: int):
    print(f"\033[1mRunning test for Day {day}\033[0m")
    sp.run(args=[f"../build/solution{day}", "1", "benchmark"])
    sp.run(args=[f"../build/solution{day}", "2", "benchmark"])
    print()


def benchmark_all():
    dirs = os.listdir(".")
    for dir in dirs:
        if os.path.isdir(dir):
            # is of format "Day <num>"
            if not re.match(r"Day \d+", dir):
                continue
            # get the day number
            day = int(re.search(r"\d+", dir).group())

            os.chdir(dir)
            benchmark(day)
            os.chdir("..")


compile_all()
benchmark_all()
