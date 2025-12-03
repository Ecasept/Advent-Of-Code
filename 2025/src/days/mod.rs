pub mod d1;
pub mod d2;
pub mod d2_brute;
pub mod d3;

type SolutionFn = fn() -> Result<i64, String>;

pub struct Solution {
    pub day: u8,
    pub part: u8,
    pub func: SolutionFn,
}

inventory::collect!(Solution);

fn get_solution(day: u8, part: u8) -> Option<&'static Solution> {
    for solution in inventory::iter::<Solution> {
        if solution.day == day {
            if solution.part == part {
                return Some(solution);
            }
        }
    }
    return None;
}

pub fn run_solution(day: u8, part: u8) -> Result<u64, String> {
    let solution = get_solution(day, part);
    if let Some(s) = solution {
        let result = (s.func)()?;
        return Ok(result as u64);
    } else {
        return Err(format!("Could not find a solution for day {day} part {part}"));
    }
}

pub fn print_result(day: u8, part: u8, result: u64) {
    println!("Day {day} part {part}: {result}");
}

pub fn benchmark_solution(day: u8, part: u8) -> Result<(), String> {
    let result = run_solution(day, part)?;
    print_result(day, part, result);
    println!("Benchmarked");
    Ok(())
}

pub fn execute_solution(day: u8, part: u8) -> Result<(), String> {
    let result = run_solution(day, part)?;
    print_result(day, part, result);
    println!("Benchmarked");
    Ok(())
}

pub fn benchmark_day(day: u8) -> Result<(), String> {
    benchmark_solution(day, 1)?;
    benchmark_solution(day, 2)?;
    Ok(())
}

pub fn execute_day(day: u8) -> Result<(), String> {
    execute_solution(day, 1)?;
    execute_solution(day, 2)?;
    Ok(())
}
