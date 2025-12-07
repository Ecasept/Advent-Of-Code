use std::fs::File;
use std::io::BufReader;

pub mod grid;

const INPUT_FILE_NAME: &str = "input.txt";

fn read_file(path: String) -> Result<BufReader<File>, String> {
    let file = match File::open(&path).map_err(|err| err.to_string()) {
        Ok(f) => f,
        Err(e) => return Err(format!("Failed to read {path}: {e}")),
    };
    Ok(BufReader::new(file))
}

pub fn get_input(day: u8) -> Result<BufReader<File>, String> {
    read_file(format!("inputs/d{day}/{INPUT_FILE_NAME}"))
}

pub fn get_example(day: u8, num: u8) -> Result<BufReader<File>, String> {
    read_file(format!("inputs/d{day}/ex{num}.txt"))
}

pub fn get_i(day: u8) -> Result<BufReader<File>, String> {
    get_input(day)
}

#[allow(dead_code)]
pub fn get_e(day: u8) -> Result<BufReader<File>, String> {
    get_example(day, 1)
}
