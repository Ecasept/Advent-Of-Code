# Download puzzles

Execute `./setup.sh <day>` to download the input from that day, and initialize the folder.
Put your session id in a `.env` file first.

# Build/Run project

- `make all`: Build all solutions

- <code>make day<i>\<num></i></code>: Build solution for specific day, eg. `make day3` to build solution for day 3 (will be outputted at <code>build/solution<i>\<num></i></code>)

- <code>make day<i>\<num></i> OUTPUT=<i>\<name></i></code>: Build solution for specific day and name executable `<name>`, eg. `make day3 OUTPUT=solution_day3` will place executable at `build/solution_day3`

- <code>make run<i>\<num></i></code>: Build and run specific day

  - pass <code>ARGS="<i>\<arg1> \<arg2></i> ..."</code> to pass command line arguments to the program

- pass `DEBUG=true` to a make command to build with debug symbols enabled (eg. to enable breakpoints in an IDE)

- `python3 test/benchmark.py`: Benchmark all solutions
- `make benchmark`: Same as `python3 test/benchmark.py`

- Using VSCode:
  Use the launch configuration in `launch.json`. This will build the executable and launch it. The configuration calls the task in `task.json`, which calls `build.sh`, which finally executes the correct make command (with debug mode enabled). The launch configuration then launches gdb with the built executable.

# Explanation for `lib/`

- `cutils`: Utilities to load the input for C
- `utils`: Utilities to load the input for C
- `runner`: Included in every build. It contains the `main()` function that is called at the start of the program
  - `./executable`: Run the `part1()` and `part2()` functions that should included in the bundled solution, and output their results
  - <code>./executable <i>\<num></i></code>: Run part 1/2 specifically
  - <code>./executable <i>\<num></i> benchmark</code>: Benchmark a specific part (this is used by `make benchmark`)
