mod days;
mod utils;

use std::env;
use colored::*;

fn print_error(err: String) {
    println!("Error running aoc: {}", err.red());
}

fn main() {
    match run() {
        Err(e) => print_error(e),
        Ok(_) => {}
    }
}

fn cmd_run(day: Option<&String>) -> Result<(), String> {
    if let Some(d) = day {
        match d.parse() {
            Ok(d) => days::execute_day(d)?,
            Err(_) => return Err(format!("Could not parse day \"{d}\""))
        }
    } else {
        days::execute_all()?;
    }
    Ok(())
}

fn cmd_bench(day: Option<&String>) -> Result<(), String> {
    if let Some(d) = day {
        match d.parse() {
            Ok(d) => days::benchmark_day(d)?,
            Err(_) => return Err(format!("Could not parse day \"{d}\""))
        }
    } else {
        days::benchmark_all()?;
    }
    Ok(())
}

fn run() -> Result<(), String> {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        return Err("Usage: ./aoc <run|bench> [<day>]".to_owned());
    }

    if args[1] == "run" {
        return cmd_run(args.get(2));
    } else if args[1] == "bench" {
        return cmd_bench(args.get(2));
    } else {
        return cmd_run(args.get(1));
    }
}
