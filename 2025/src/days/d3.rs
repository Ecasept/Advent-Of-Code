use aoc_macros::aoc;
use crate::utils;
use std::io::BufRead;

fn max(arr: &Vec<u64>, from: u64, to: u64) -> (u64, u64) {
    let from = from as usize;
    let to = to as usize;
    let mut max_val = arr[from];
    let mut max_idx = from;
    for i in (from + 1)..=to {
        if arr[i] > max_val {
            max_val = arr[i];
            max_idx = i;
        }
    }
    (max_val, max_idx as u64)
}

pub fn solve(l: u64) -> Result<i64, String> {
    let input = utils::get_i(3)?;
    let mut sum = 0_u64;
    for line in input.lines() {
        let line = match line {
            Ok(l) => l,
            Err(e) => return Err(e.to_string()),
        };
        let chars: Vec<u64> = line.chars().map(|c| c.to_digit(10).unwrap() as u64).collect();
        
        let mut idx = 0;
        for x in 0..l {
            let inv = l-x-1; // counts down from l-1 to 0;
            let val = max(&chars, idx, (chars.len() as u64 - 1 - inv) as u64);
            sum += 10_u64.pow((inv) as u32) * val.0;
            idx = val.1 + 1;
        }
    };
    return Ok(sum as i64);
}

#[aoc(3, 1)]
pub fn day1_part1() -> Result<i64, String> {
    return solve(2);
}

#[aoc(3, 2)]
pub fn day1_part2() -> Result<i64, String> {
    return solve(12);
}
