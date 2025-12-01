mod days;
mod utils;

use std::env;

fn print_error(err: String) {
    println!("Error running aoc: {err}");
}

fn main() {
    match run() {
        Err(e) => print_error(e),
        Ok(_) => {}
    }
}

fn run() -> Result<(), String> {
    let day = get_day()?;
    days::execute(day)?;
    return Ok(());
}

fn get_day() -> Result<u8, String> {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        return Err("Expected the day as the second argument".to_owned());
    }

    match args[1].parse() {
        Err(_) => Err("Please enter a number for the day".to_owned()),
        Ok(number) => Ok(number),
    }
}
