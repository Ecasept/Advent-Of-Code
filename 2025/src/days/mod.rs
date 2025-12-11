pub mod d1;
pub mod d10;
pub mod d11;
pub mod d2;
pub mod d3;
pub mod d4;
pub mod d5;
pub mod d6;
pub mod d7;
pub mod d8;
pub mod d9;
use colored::*;
use std::time::Instant;

type SolutionFn = fn() -> Result<String, String>;

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

pub fn run_solution(day: u8, part: u8) -> Result<String, String> {
    let solution = get_solution(day, part);
    if let Some(s) = solution {
        return (s.func)();
    } else {
        return Err(format!(
            "Could not find a solution for day {day} part {part}"
        ));
    }
}

pub fn print_result(day: u8, part: u8, result: String) {
    println!(
        "Day {} part {}: {}",
        day.to_string().bold(),
        part.to_string().bold(),
        result.green()
    );
}

pub fn benchmark_solution(day: u8, part: u8) -> Result<(), String> {
    let start = Instant::now();
    match run_solution(day, part) {
        Ok(result) => {
            let elapsed = start.elapsed();
            print_result(day, part, result);
            println!("Ran in {}", format!("{:.2?}", elapsed).yellow());
        }
        Err(e) => {
            println!("{}", e.red());
        }
    }
    Ok(())
}

pub fn execute_solution(day: u8, part: u8) -> Result<(), String> {
    match run_solution(day, part) {
        Ok(result) => {
            print_result(day, part, result);
        }
        Err(e) => {
            println!("{}", e.red());
        }
    }
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

pub fn execute_all() -> Result<(), String> {
    let mut days: Vec<u8> = inventory::iter::<Solution>
        .into_iter()
        .map(|s| s.day)
        .collect();
    days.sort();
    days.dedup();

    for day in days {
        execute_day(day)?;
        println!();
    }
    Ok(())
}

pub fn benchmark_all() -> Result<(), String> {
    let mut days: Vec<u8> = inventory::iter::<Solution>
        .into_iter()
        .map(|s| s.day)
        .collect();
    days.sort();
    days.dedup();

    let start = Instant::now();
    for day in days {
        benchmark_day(day)?;
        println!();
    }
    let elapsed = start.elapsed();
    println!("Total time: {}", format!("{:.2?}", elapsed).bold().cyan());
    Ok(())
}
