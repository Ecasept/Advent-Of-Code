import sys
import os
import re
import subprocess as sp
import time
from typing import Generator

# Change the directory to the root of the project
os.chdir(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

valgrind_exclude = []  # [16]


def compile_single_debug(day: int):
    """Compile a single solution with debug symbols enabled"""
    sp.run(args=["make", f"day{day}", "DEBUG=true",
                 f"OUTPUT=solution{day}-debug"])


def compile_all():
    """Compile all solutions and measure the time taken"""
    print("\033[1mCompiling all solutions...\033[0m")
    start = time.time()
    args = ["make"]
    args.extend(["day" + str(day) for day in days])
    sp.run(args=args)
    end = time.time()
    print(f"Total time taken: \033[1m{end - start:.2f}s\033[0m")
    print()


def execute_solution_with_benchmark(day: int):
    """Benchmark a single solution"""
    print(f"\033[1mBenchmarking day {day}...\033[0m")
    sp.run(args=[f"../build/solution{day}", "benchmark"])
    print()


def benchmark_all():
    """Benchmark all solutions"""
    for day in days:

        os.chdir(f"Day {day}")
        execute_solution_with_benchmark(day)
        os.chdir("..")


def benchmark_with_python(day: int):
    """Benchmark a single solution using python to measure the time taken and return it"""
    start = time.time()
    sp.run(args=[f"../build/solution{day}"], stdout=sp.DEVNULL)
    end = time.time()
    return end - start


def benchmark_overall():
    """Benchmark all solutions and print overall statistics"""
    print("\033[1mRunning overall benchmark...\033[0m")
    total_time = 0
    for day in days:

        os.chdir(f"Day {day}")
        total_time += benchmark_with_python(day)
        os.chdir("..")
    print(
        f"""Total time taken: \033[1m{total_time:.3f}s\033[0m for \033[1m{
            len(days)}\033[0m solutions (avg: \033[1m{total_time / len(days):.3f}s\033[0m)"""
    )
    print()


def parse_valgrind_output(log_file: str) -> tuple[int, int]:
    """Parse the valgrind output file and return the number of bytes in use at exit and the number of errors"""
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


def print_valgrind_output(in_use_bytes: int, error_count: int):
    # print the results
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


def valgrind_single(day: int):
    if day in valgrind_exclude:
        print(f"\033[1mSkipping day {day}\033[0m")
        return
    """Run valgrind on a single solution and print the results"""
    print(f"\033[1mRunning valgrind on day {day}...\033[0m", end="")
    sys.stdout.flush()
    sp.run(args=["/usr/bin/valgrind",
                 f"--log-file=../build/valgrind-log{day}.txt", f"../build/solution{day}"], stdout=sp.DEVNULL)
    in_use_bytes, error_count = parse_valgrind_output(
        f"../build/valgrind-log{day}.txt")

    print(f"\r\033[1mRunning valgrind on day {day}:\033[0m", end=" ")
    print_valgrind_output(in_use_bytes, error_count)

    if not (in_use_bytes == 0 and error_count == 0):
        # leaks/errors detected, rerun with full output
        print("Compiling with debug symbols and rerunning valgrind with full output...")

        os.chdir("..")
        compile_single_debug(day)
        os.chdir(f"Day {day}")

        print("\033[1mRerunning valgrind with full output...\033[0m", end="")
        sys.stdout.flush()

        sp.run(args=["/usr/bin/valgrind",
                     f"--leak-check=full", f"--show-leak-kinds=all", f"--track-origins=yes", f"--verbose", f"--log-file=../build/valgrind-log{day}-full.txt", f"../build/solution{day}-debug"], stdout=sp.DEVNULL)

        in_use_bytes, error_count = parse_valgrind_output(
            f"../build/valgrind-log{day}-full.txt")
        print("\r\033[1mRerunning valgrind with full output:\033[0m", end=" ")
        print_valgrind_output(in_use_bytes, error_count)

        print(
            f"Full output saved to \033[1mbuild/valgrind-log{day}-full.txt\033[0m")


def valgrind_all():
    """Run valgrind on all solutions"""
    for day in days:
        os.chdir(f"Day {day}")
        valgrind_single(day)
        os.chdir("..")


def get_solution_dirs() -> Generator[int, None, None]:
    """Yield all days with solutions"""
    dirs = os.listdir(".")
    for dir_ in dirs:
        if os.path.isdir(dir_):
            # is of format "Day <num>"
            if not re.match(r"Day \d+", dir_):
                continue
            # get the day number
            day = int(re.search(r"\d+", dir_).group())
            yield day


def verify_all():
    """Verify all solutions"""
    print(f"\033[1mVerifying Solutions...\033[0m")
    try:
        solutions = [s.strip()
                     for s in open("test/solutions.txt", "r").readlines()]
    except FileNotFoundError:
        print(
            "\n\033[31mError: Could not verify solutions - solutions.txt not found\033[0m")

    i = 0
    incorrect_count = 0
    for day in days:
        os.chdir(f"Day {day}")

        p = sp.run(args=["../build/solution" + str(day)], stdout=sp.PIPE)
        out = p.stdout.decode("utf-8").strip().split("\n")

        if out[0] != solutions[i]:
            incorrect_count += 1
            print(
                f"\033[31mDay {day} part 1: Incorrect solution\033[0m\nExpected: {solutions[i]}\nGot: {out[0]}")
        if out[1] != solutions[i + 1]:
            incorrect_count += 1
            print(
                f"\033[31mDay {day} part 2: Incorrect solution\033[0m\nExpected: {solutions[i + 1]}\nGot: {out[1]}")

        os.chdir("..")
        i += 2
    if incorrect_count == 0:
        print("\r\033[32mAll solutions are correct\033[0m")
    else:
        print(
            f"\r\033[31m{incorrect_count} solution{" is" if incorrect_count == 1 else "s are"} incorrect\033[0m")
    print()


days = list(get_solution_dirs())
if __name__ == "__main__":
    compile_all()
    benchmark_all()
    benchmark_overall()
    verify_all()
    valgrind_all()
