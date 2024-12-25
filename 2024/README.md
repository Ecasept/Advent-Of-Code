# Quick start

### Run code on a solution with make

Place a file called `input.txt` in the folder for that day.
Execute <code>make run<i>\<num></i></code> in the parent directory.

Example: If you want to solve day 6:

- place your input in `Day 6/input.txt`
- execute `make run6` while you are in the `2024` folder

### Run code on solution with an executable

If you want an executable that you can run, follow these steps

- Build with <code>make day<i>\<num></i></code>.
- The executable will be placed under `<code>build/solution<i>\<num></i></code>. (Or you can specify the name by passing <code>OUTPUT=<i>\<filename></i></code> to the build command)
- Place your `input.txt` in a directory.
- From the **same directory that you placed your input in**, execute the executable

Example: If you want to solve day 6:

- place your input in some folder, we will use `Day 6/input.txt`
- execute `make build6` while you are in the `2024` folder
- cd into the folder with your input: `cd "Day 6"`
- execute the executable: `../build/solution6`

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
  - place a `input.txt` file in every solution folder, and place a `solutions.txt` file in `test/` with the solution for each part of every solution on its own line
- `make test`: Same as `python3 test/test.py`

- `make` defaults to `make test`

- Using VSCode:
  Use the launch configuration in `launch.json`. This is setup to use the Microsoft C/C++ extension to run/debug the active file. This launch configuration uses a pre-launch task to build the executable, which is defined in `task.json`. It calls <code>make "vscode_Day <i>\<num></i>"</code> which is an alias for <code>make day<i>\<num></i></code> with appropriate `DEBUG` and `OUTPUT` variables set .

# Explanation for `lib/`

- `cutils`: Utilities to load the input for C
- `utils`: Utilities to load the input for C++
- `runner`: Included in every build. It contains the `main()` function that is called at the start of the program
  - `./executable`: Run the `part1()` and `part2()` functions that should included in the bundled solution, and output their results
  - <code>./executable <i>\<num></i></code>: Run part 1 or 2 specifically
  - <code>./executable benchmark</code>: Benchmark solution (this is used by `make test`), can be combined with a number in front of `benchmark` to benchmark a specific part.
