# Download puzzles

Execute `./setup.sh <day>` to download the input from that day, and initialize the folder.
Put your session id in a `.env` file first.

# Build/Run project

- <code>make day<i>\<num></i></code>: Build solution for specific day, eg. `make day3` to build solution for day 3 (will be outputted at <code>build/solution<i>\<num></i></code>)

- <code>make day<i>\<num></i> OUTPUT=<i>\<name></i></code>: Build solution for specific day and name executable `<name>`, eg. `make day3 OUTPUT=solution_day3` will place executable at `build/solution_day3`

- <code>make run<i>\<num></i></code>: Build and run specific day

  - pass <code>ARGS="<i>\<arg1> \<arg2></i> ..."</code> to pass command line arguments to the program

- pass `DEBUG=true` to a make command to build with debug symbols enabled (eg. to enable breakpoints in an IDE)

- `python3 test/test.py`: Test and benchmark all solutions
- `make test`: Same as `python3 test/test.py`

- `make` defaults to `make test`

- Using VSCode:
  Use the launch configuration in `launch.json`. This is setup to use the Microsoft C/C++ extension to run/debug the active file. This launch configuration uses a pre-launch task to build the executable, which is defined in `task.json`. It calls <code>make "vscode_Day <i>\<num></i>"</code> which is an alias for <code>make day<i>\<num></i></code> with appropriate `DEBUG` and `OUTPUT` variables set .

# Explanation for `lib/`

- `cutils`: Utilities to load the input for C
- `utils`: Utilities to load the input for C
- `runner`: Included in every build. It contains the `main()` function that is called at the start of the program
  - `./executable`: Run the `part1()` and `part2()` functions that should included in the bundled solution, and output their results
  - <code>./executable <i>\<num></i></code>: Run part 1 or 2 specifically
  - <code>./executable benchmark</code>: Benchmark solution (this is used by `make test`), can be combined with a number in front of `benchmark` to benchmark a specific part.
