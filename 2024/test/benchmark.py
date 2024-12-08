import os
import re
import subprocess as sp
import time

# Change the directory to the root of the project
os.chdir(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def compile_all():
    print("\033[1mCompiling all solutions\033[0m")
    start = time.time()
    os.system("make all")
    end = time.time()
    print(f"Total time taken: \033[1m{end - start:.2f}s\033[0m")
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


def benchmark_single(day: int):
    start = time.time()
    sp.run(args=[f"../build/solution{day}"], stdout=sp.DEVNULL)
    end = time.time()
    return end - start


def benchmark_overall():
    print("\033[1mRunning overall benchmark\033[0m")
    total_time = 0
    dir_count = 0
    dirs = os.listdir(".")
    for dir in dirs:
        if os.path.isdir(dir):
            # is of format "Day <num>"
            if not re.match(r"Day \d+", dir):
                continue
            # get the day number
            day = int(re.search(r"\d+", dir).group())

            os.chdir(dir)
            total_time += benchmark_single(day)
            os.chdir("..")
            dir_count += 1
    print(
        f"""Total time taken: \033[1m{total_time:.3f}s\033[0m for \033[1m{
            dir_count}\033[0m problems (avg: \033[1m{total_time / dir_count:.3f}s\033[0m)"""
    )


compile_all()
benchmark_all()
benchmark_overall()
