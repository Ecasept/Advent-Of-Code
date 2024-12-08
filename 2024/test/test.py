import os
import re
import subprocess as sp
import time

# Change the directory to the root of the project
os.chdir(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def compile_single_debug(day: int):
    sp.run(args=["make", f"day{day}", "DEBUG=true",
           f"OUTPUT=solution{day}-debug"])


def compile_all():
    print("\033[1mCompiling all solutions\033[0m")
    start = time.time()
    sp.run(args=["make", "all"])
    end = time.time()
    print(f"Total time taken: \033[1m{end - start:.2f}s\033[0m")
    print()


def execute_solution_with_benchmark(day: int):
    print(f"\033[1mBenchmarking day {day}\033[0m")
    sp.run(args=[f"../build/solution{day}", "benchmark"])
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
            execute_solution_with_benchmark(day)
            os.chdir("..")


def benchmark_with_python(day: int):
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
            total_time += benchmark_with_python(day)
            os.chdir("..")
            dir_count += 1
    print(
        f"""Total time taken: \033[1m{total_time:.3f}s\033[0m for \033[1m{
            dir_count}\033[0m solutions (avg: \033[1m{total_time / dir_count:.3f}s\033[0m)"""
    )
    print()


def parse_valgrind_output(log_file: str) -> tuple[int, int]:
    try:
        with open(log_file, "r") as f:
            contents = f.read()
    except FileNotFoundError:
        print(f"\033[31mError: Log file {log_file} not found.\033[0m")
        return 0, 0

    in_use_match = re.search(r"in use at exit: ([\d,]+) bytes", contents)
    in_use_bytes = int(in_use_match.group(
        1).replace(",", "")) if in_use_match else 0

    error_match = re.search(r"ERROR SUMMARY: ([\d,]+) errors", contents)
    error_count = int(error_match.group(
        1).replace(",", "")) if error_match else 0

    return in_use_bytes, error_count


def valgrind_single(day: int):
    print(f"\033[1mRunning valgrind on day {day}:\033[0m", end=" ")
    sp.run(args=["/usr/bin/valgrind",
           f"--log-file=../build/valgrind-log{day}.txt", f"../build/solution{day}"], stdout=sp.DEVNULL)
    in_use_bytes, error_count = parse_valgrind_output(
        f"../build/valgrind-log{day}.txt")

    if in_use_bytes == 0 and error_count == 0:
        print("\033[32mNo leaks or errors detected\033[0m")
    else:
        print(f"\033[31mLeaks or errors detected\033[0m")

        if in_use_bytes > 0:
            print(
                f"\033[31mPotential memory leak: \033[1m{in_use_bytes:,}\033[0m\033[31m bytes in use at exit\033[0m")
        if error_count > 0:
            print(f"\033[31mErrors: \033[1m{
                  error_count}\033[0m\033[31m errors detected\033[0m")
        print("Compiling with debug symbols and rerunning valgrind with full output...")

        os.chdir("..")
        compile_single_debug(day)
        os.chdir(f"Day {day}")

        sp.run(args=["/usr/bin/valgrind",
               f"--leak-check=full", f"--show-leak-kinds=all", f"--track-origins=yes", f"--verbose", f"--log-file=../build/valgrind-log{day}-full.txt", f"../build/solution{day}-debug"], stdout=sp.DEVNULL)

        print(
            f"Full output saved to \033[1mbuild/valgrind-log{day}-full.txt\033[0m")


def valgrind_all():
    dirs = os.listdir(".")
    for dir in dirs:
        if os.path.isdir(dir):
            # is of format "Day <num>"
            if not re.match(r"Day \d+", dir):
                continue
            # get the day number
            day = int(re.search(r"\d+", dir).group())

            os.chdir(dir)
            valgrind_single(day)
            os.chdir("..")


compile_all()
benchmark_all()
benchmark_overall()
valgrind_all()
